/*  main.c  - main */

#include <xinu.h>

/*------------------------------------------------------------------------
 * udp_echo  -  echo message sent over UDP
 *------------------------------------------------------------------------
 */
uint16 udp_echo()
{
	uint32 remoteip = dnslookup("xinu14.cs.purdue.edu");
	uint16 remoteport = 44000;
	uint16 localport = 42069;

	uid32 slot = udp_register(remoteip, remoteport, localport);
	if (slot == SYSERR)
	{
		return SYSERR;
	}

	char *buff = "udp echo test";
	udp_send(slot, buff, 14);

	char *response = "";
	int32 responselength = 0;
	if ((responselength = udp_recv(slot, response, 14, 500)) == TIMEOUT)
	{
		kprintf("timeout, running attempt 2\n");
		udp_send(slot, buff, 14);
		if (udp_recv(slot, response, 14, 500) == TIMEOUT)
		{
			kprintf("timeout\n");
			return SYSERR;
		}
	}
	if (responselength != 14 || strcmp(buff, response) != 0)
	{
		kprintf("bad response\n");
		kprintf("buff: %s\n", buff);
		kprintf("response: %s\n", response);
		kprintf("strcmp: %d\n", strcmp(buff, response));
		return SYSERR;
	}
	kprintf("success\n");
	return OK;
}

/*------------------------------------------------------------------------
 * chatroom  -  send messages to remote user over UDP
 *------------------------------------------------------------------------
 */
uint16 chatroom(uint32 remoteip, uint32 localip)
{

	uint16 port1 = 42069;
	uint16 port2 = 44000;

	uid32 slot;
	if (remoteip > localip)
	{
		slot = udp_register(remoteip, port2, port1);
	}
	else
	{
		slot = udp_register(remoteip, port1, port2);
	}

	if (slot == SYSERR)
	{
		return SYSERR;
	}

	resume(create(udp_receiver, 4096, 20, "udp_receiver", 1, slot));
	resume(create(udp_sender, 4096, 20, "udp_sender", 1, slot));

	return OK;
}


process main(void)
{
	recvclr();

	kprintf("input 1 for udp_echo or 2 for chat room.\n");
	char *input = getmem(2);
	read(CONSOLE, input, 2);

	// choose which function to run. 1 for echo, 2 for messaging
	if (input[0] == '1') {
		kprintf("running udp_echo\n");
		udp_echo();
	}
	else if (input[0] == '2') {
		kprintf("running chat room\n");
		uint32 localip = getlocalip(); // get local ip and print in dotted form
		char str[128];
		if ((int32)localip == SYSERR) {
			kprintf("Cannot obtain an IP address\n");
		}
		else {
			sprintf(str, "%d.%d.%d.%d",
							(localip >> 24) & 0xff, (localip >> 16) & 0xff,
							(localip >> 8) & 0xff, localip & 0xff);

			kprintf("local ip address: %s\n", str);
		}

		kprintf("input remote ip address (dotted):\n"); // input other side's ip
		char *remoteipdotted = getmem(32);
		fgets(remoteipdotted, 32, CONSOLE);
		kprintf("%s\n", remoteipdotted);
		// remoteipdotted[strlen(remoteipdotted)-2] = '\0';
		uint32 remoteip;
		dot2ip(remoteipdotted, &remoteip); // change dotted to uint32
		kprintf("localip: %u\n", localip);
		kprintf("remoteip: %u\n", remoteip);
		chatroom(remoteip, localip);
	}
	else
	{
		kprintf("invalid; running shell\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}

	return OK;
}
