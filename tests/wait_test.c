//#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdint.h>

int main(int argc, char **argv)
{
	if (fork() == 0) {
		printf("Child process: %d\n", getpid());
		for (int i = 0; i < 100; i++) {
			write(0, "Waiting...\n", 11);
			usleep(500);
		}
		write(0, "Exit child\n", 11);
		exit(127);
	} else {
		long res = syscall(644, NULL);
		printf("RES: %d\n", res);
	}
	return 0;
}
