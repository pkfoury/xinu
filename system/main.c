/*  main.c  - main */

#include <xinu.h>

local process part1_test(void);
local process sender(void);
local process receiver(void);

process main(void)
{
	part1_test();

	/* Set up and empty file system on the RAMDISK device */
	lfscreate(RAMDISK, 40, 20480);

	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;

}

process part1_test(void)
{
	struct dentry *devptr = (struct dentry *)&devtab[PIPE];
	pipeopen(devptr, NULL, NULL);
	kprintf("Open pipe\n");
	resume(create(sender, 512, 50, "part1_sender", 0));
	resume(create(receiver, 512, 50, "part1_receiver", 0));
	return OK;
}

process sender(void)
{
	struct dentry *devptr = (struct dentry *)&devtab[PIPE];
	char *string_to_send = "hello, world!\n";
	int i = 0;
	kprintf("Start to send the string...\n");
	for (; i < strlen(string_to_send); ++i)
		pipeputc(devptr, string_to_send[i]);
	pipeclose(devptr);
	return OK;
}

process receiver(void)
{
	int32 ch;
	char recv_buff[20];
	int i = 0;
	struct dentry *devptr = (struct dentry *)&devtab[PIPE];
	kprintf("Start to receive the string...\n");
	while ((ch = pipegetc(devptr)) != EOF)
	{
		kprintf("Recv char %c\n", (char)ch);
		recv_buff[i++] = (char)ch;
	}
	recv_buff[i] = '\0';

	kprintf("%s\n", recv_buff);
	return OK;
}