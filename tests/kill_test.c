//#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdint.h>

int main(int argc, char **argv)
{
	int pause;
	if (argc < 3)
	{
		printf("Not enough params\n ./test PID SIG\n");
		return 1;
	}
	printf("My gid: %d\n", getgid());
	scanf("%d", &pause);
	int pid = atoi(argv[1]);
	int sig = atoi(argv[2]);
	long res = syscall(643, pid, sig);
	printf("RES: %d\n", res);
	return res;
}
