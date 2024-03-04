//#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdint.h>

struct pid_info
{
        pid_t   pid;
        long    state;
        void    *stack;
	unsigned long start_time;
        pid_t   *child_pids;
        int     num_child_pids;
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
