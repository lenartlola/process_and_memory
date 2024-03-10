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
        pid_t   *children;
        int     num_child_pids;
	pid_t	parent_pid;
	char 	*root, *pwd;
};

int main(int argc, char **argv)
{
	struct pid_info p_info;
	p_info.children = malloc(200 * sizeof(pid_t));
	p_info.root = malloc(200);
	p_info.pwd = malloc(200);	
	
	if (argc < 2 || !p_info.children || !p_info.root || !p_info.pwd) {
		fprintf(stderr, "Usage: %s <pid>\n", argv[0]);
		return 1;
	}
	int pid = atoi(argv[1]);
	long res = syscall(642, &p_info, pid);

	if (res < 0) {
		perror("syscall");
		return 1;
	}
	printf("PID: %d\n", p_info.pid);
	printf("State: %ld\n", p_info.state);
	printf("Start Time: %lu\n", p_info.start_time);
	printf("Parent PID: %d\n", p_info.parent_pid);
	printf("Root: %s\n", p_info.root);
	printf("PWD: %s\n", p_info.pwd);
	
	printf("Child PIDs:\n");
	for (int i = 0; i < p_info.num_child_pids; i++) {
	    printf("%d\n", p_info.children[i]);
	}
	
	// Free allocated memory
	free(p_info.children);
	free(p_info.root);
	free(p_info.pwd);
	printf("RES: %d\n", res);
	return res;
}
