#include <linux/init.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/fs_struct.h>
#include <linux/dcache.h>
#include <linux/pid.h>
#include <linux/wait.h>

SYSCALL_DEFINE1(ft_wait, int *, wstatus)
{
	long ret = kernel_wait4(-1, wstatus, 0, NULL);
	return ret;
}
