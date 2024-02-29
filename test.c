//#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <string.h>

struct pid_info
{
	int	pid;
};

int main(int argc, char **argv)
{
	struct pid_info p_info;
	if (argc < 2)
		return 1;
	int pid = atoi(argv[1]);
	long res = syscall(642, &p_info, pid);
	printf("RES: %d\n", pid);
	return res;
}
