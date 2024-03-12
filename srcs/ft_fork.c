#include <linux/init.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/fs_struct.h>
#include <linux/dcache.h>
#include <linux/pid.h>
#include <linux/sched.h>

SYSCALL_DEFINE0(ft_fork)
{
	struct kernel_clone_args	clone_args;
	struct pid			*pid;
	struct task_struct		*task;
	int				trace = PTRACE_EVENT_FORK;
	pid_t				nr;

	clone_args.exit_signal = SIGCHLD;
	if (likely(!ptrace_event_enabled(current, trace)))
		trace = 0;
	task = copy_process(NULL, trace, NUMA_NO_NODE, &clone_args);
	add_latent_entropy();
	if (IS_ERR(task))
		return PTR_ERR(task);
	pid = get_task_pid(task, PIDTYPE_PID);
	nr = pid_vnr(pid);
	wake_up_new_task(task);
	if (unlikely(trace))
		ptrace_event_pid(trace, pid);
	put_pid(pid);
	return nr;
}
