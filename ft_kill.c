#include <linux/init.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/fs_struct.h>
#include <linux/dcache.h>
#include <linux/pid.h>

static inline bool has_permission(struct task_struct *caller, struct task_struct *callee)
{
	if (uid_eq(caller->cred->uid, callee->cred->uid) 
		|| uid_eq(caller->cred->euid, callee->cred->euid))
		return true;

	/* 
	 * Check for super user 
	 * PF_SUPERPRIV 0x00000100 -> Used super-user privileges
	 */

	if (caller->flags == PF_SUPERPRIV)
		return true;

	return false;
}

SYSCALL_DEFINE2(ft_kill, int, pid, int, sig)
{
	struct pid		*pid_struct;
	struct task_struct	*task_struct;
	int			ret;
	if (pid > 0)
		goto send_normal_signal;
	if (pid == 0)
		goto send_signal_to_caller_gid;

send_signal_to_caller_gid:
	kgid_t cgid = current->cred->gid;
	ret = -1;
	for_each_process(task_struct) {
		if (gid_eq(task_struct->cred->gid, cgid)) {
			if (has_permission(current, task_struct))
				ret = send_sig(sig, task_struct, 1);
		}
	}
	return ret;

send_normal_signal:
	pid_struct = find_get_pid(pid);
	ret = -1;
	if (!pid_struct)
		return -ESRCH;
	
	task_struct = pid_task(pid_struct, PIDTYPE_PID);
	if (!task_struct)
		return -ESRCH;

	if (has_permission(current, task_struct))
		ret = send_sig(sig, task_struct, 1);
	return ret;
}
