// long sys_get_pid_info(struct pid_info *ret, int pid);
#include <linux/init.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/fs_struct.h>
#include <linux/dcache.h>
#include <linux/pid.h>

//static char* root_path(struct task_struct *task)
//{
//	struct fs_struct *fs = task->fs;
//	if (!fs)
//	{
//		return NULL;
//	}
//	return dentry_path_raw(fs->root.dentry, NULL, 0);
//}

SYSCALL_DEFINE2(get_pid_info, struct pid_info __user *, info, int, pid)
{	
	struct task_struct	*task;
	struct pid		*pid_struct;
	
	pid_struct = find_get_pid(pid);
	task = pid_task(pid_struct, PIDTYPE_PID);
	if (!task)
	{
		printk(KERN_INFO "PID task not found %d\n", pid);
		return -1;
	}
	printk(KERN_INFO "pid_info: pid -> %d\n", task->pid);
	//char *root = root_path(task);
	//if (!root) 
	//{
	//	printk(KERN_INFO "pid_info: root path not found\n");
	//	return -1;
	//}
	//else
	//{
	//	printk(KERN_INFO "pid_info: state -> %d, root path: %s\n", task->pid, root);
	//	kfree(root);
	//}
	return 0;
}

