// long sys_get_pid_info(struct pid_info *ret, int pid);
#include <linux/init.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/kernel.h>

//struct pid_info
//{
//	int	pid;
//};

SYSCALL_DEFINE2(get_pid_info, struct pid_info __user *, info, int, pid)
{	
	struct task_struct	*task;
	struct pid		*pid_struct;
	
	pid_struct = find_get_pid(pid);
	task = pid_task(pid_struct, PIDTYPE_PID);
	printk(KERN_INFO "pid_info: %s\n", task->comm);
	return 0;
}

