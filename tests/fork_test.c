#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

#define __NR_ft_fork 645

int main()
{
	long ret = syscall(__NR_ft_fork);
	ret = syscall(__NR_ft_fork);
	ret = syscall(__NR_ft_fork);
	dprintf(0, "Child process %d\n", getpid());
//	if (ret == 0)
//	{
//		dprintf(0, "Child process %d\n", getpid());
//		for (int i = 0; i < 10; i++)
//			usleep(1000);
//		exit(0);
//	}
//	else
//		wait(ret);

	return 0;
}

