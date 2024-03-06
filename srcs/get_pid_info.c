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
	char 	*root, *pwd;
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
	char			*root_path;
	char			*buffer;
	char			*pwd_buffer;
	int			path_len;
	//struct path		root;
	
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

	ret = copy_to_user(&info->parent_pid, &task->parent->pid, sizeof(pid_t));
	if (ret)
		return -EFAULT;

	root_path = NULL;
	root_path = kmalloc(PATH_MAX + 1, GFP_KERNEL);
	if (!root_path)
		goto clean_out;

	task_lock(task);
	buffer = dentry_path_raw(task->fs->root.dentry, root_path, PATH_MAX);
	task_unlock(task);
	kfree(root_path);
	path_len = strlen(buffer);
	ret = copy_to_user(info->root, buffer, path_len);
	if (ret)
		return -EFAULT;

	root_path = NULL;
	root_path = kmalloc(PATH_MAX + 1, GFP_KERNEL);
	if (!root_path)
		goto clean_out;

	task_lock(task);
	pwd_buffer = dentry_path_raw(task->fs->pwd.dentry, root_path, PATH_MAX);
	task_unlock(task);
	kfree(root_path);
	path_len = strlen(pwd_buffer);
	ret = copy_to_user(info->pwd, pwd_buffer, path_len);
	if (ret)
		return -EFAULT;

	return 0;
clean_out:
	task_unlock(task);
	return -EFAULT;
}

