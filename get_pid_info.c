// long sys_get_pid_info(struct pid_info *ret, int pid);
#include <linux/init.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/fs_struct.h>
#include <linux/dcache.h>
#include <linux/pid.h>

struct pid_info
{
	pid_t	pid;
	long	state;
	void	*stack;
	u64	start_time;
	pid_t	*children;
	int	num_child_pids;
	pid_t	parent_pid;
};


SYSCALL_DEFINE2(get_pid_info, struct pid_info __user *, info, int, pid)
{
	unsigned long 		ret;
	struct task_struct	*task;
	struct task_struct	*child;
	struct task_struct	*parent;
	struct pid		*pid_struct;
	struct list_head 	*list;
	int 			num_child_pids;
	int			i;
	
	pid_struct = find_get_pid(pid);
	task = pid_task(pid_struct, PIDTYPE_PID);
	if (!task)
		return -ESRCH;
	ret = copy_to_user(&info->pid, &task->pid, sizeof(pid_t));
	if (ret)
		return -EFAULT;
	ret = copy_to_user(&info->state, &task->__state, sizeof(long));
	if (ret)
		return -EFAULT;
	ret = copy_to_user(&info->stack, &task->stack, sizeof(long));
	if (ret)
		return -EFAULT;
	ret = copy_to_user(&info->start_time, &task->start_time, sizeof(u64));
	if (ret)
		return -EFAULT;


	num_child_pids = 0;
	list_for_each(list, &task->children) {
		num_child_pids++;
	}
	ret = copy_to_user(&info->num_child_pids, &num_child_pids, sizeof(int));
	if (ret)
		return -EFAULT;


	task_lock(task);
	list = NULL;
	i = 0;
	list_for_each(list, &task->children) {
		child = list_entry(list, struct task_struct, sibling);
		ret = copy_to_user(&info->children[i], &child->pid, sizeof(pid_t));
		if (ret)
			goto clean_out;
		i++;
	}
	task_unlock(task);

	parent = task->parent;
	printk(KERN_INFO "Parent pid: %d\n", parent->pid);
	ret = copy_to_user(&info->parent_pid, &parent->pid, sizeof(pid_t));
	if (ret)
		return -EFAULT;

	return 0;
clean_out:
	task_unlock(task);
	return -EFAULT;
}

