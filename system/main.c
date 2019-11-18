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
	if (slot == SYSERR) {
		return SYSERR;
	}
	
	char *buff = "udp echo test";
	udp_send(slot, buff, 14);

	char *response = "";
	int32 responselength = 0;
	if( (responselength = udp_recv(slot, response, 14, 500)) == TIMEOUT) {
		kprintf("timeout, running attempt 2\n");
		udp_send(slot, buff, 14);
		if(udp_recv(slot, response, 14, 500) == TIMEOUT) {
			kprintf("timeout\n");
			return SYSERR;
		}
	}
	if(responselength != 14 || strcmp(buff, response) != 0) {
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
 * chatroom  -  
 *------------------------------------------------------------------------
 */
uint16 chatroom()
{

	return OK;
}


process main(void)
{
	recvclr();

	kprintf("input 1 for udp_echo or 2 for chat room.\n");
	char *input = getmem(2);
	read(CONSOLE, input, 2);

	if (input[0] == '1') {
		kprintf("running udp_echo\n");
		udp_echo();
	}
	else if (input[0] == '2') {
		kprintf("running udp_echo\n");
		chatroom();
	}
	else {
		kprintf("invalid; running shell\n");
		while (TRUE) {
			resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
		}
	}

	return OK;
}
