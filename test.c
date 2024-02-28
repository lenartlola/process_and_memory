//#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	struct pid_info pid_info;
	if (argc < 2)
		return 1;
	int pid = atoi(argv[1]);
	long res = syscall(SYS_get_pid_info, pid);
	printf("RES: %d\n", pid);
	return res;
}
