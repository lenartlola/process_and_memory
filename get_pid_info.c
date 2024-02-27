// long sys_get_pid_info(struct pid_info *ret, int pid);
#include <linux/module.h>
#include <linux/kernel.h>

struct pid_info
{
	int	pid;

};

SYSCALL_DEFINE2(get_pid_info, struct pid_info __user *, info, int, pid, int, flags)
{	
	struct task_struct	*task;
	struct pid		*pid_info;
	
	if (flags & ~(NO_FLAG | NO_FLAG1))
		return -EINVAL;
	if (flag != 0)
		return -EINVAL;
	
	pid_info = find_get_pid(pid);

}
