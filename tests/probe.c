void main()
{
	setgid(1001);
	int count = 0;
	while(1)
	{
		printf("My pid: %d\n", getpid());
		printf("My gid %d\n", getgid());
		printf("Seconds in process: %i\n", count);
         	sleep(2);
		count += 2;
	}
}
