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
	unsigned long	start_time;
	pid_t	*child_pids;
	int	num_child_pids;
};


SYSCALL_DEFINE2(get_pid_info, struct pid_info __user *, info, int, pid)
{
	unsigned long 		ret;
	struct task_struct	*task;
	struct task_struct	*child;
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
	//info->child_pids = kmalloc(num_child_pids + 1, GFP_USER);
	list_for_each(list, &task->children) {
		child = list_entry(list, struct task_struct, sibling);
		printk(KERN_INFO "CHILD: %d\n", child->pid);
	}
	task_unlock(task);

	printk(KERN_INFO "num ret: %lu\n", ret);
	return 0;

}

