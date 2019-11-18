/* udp_messaging.c - udp_sender, udp_receiver */

#include <xinu.h>

/*------------------------------------------------------------------------
 * udp_sender - send new message to remote ip
 *------------------------------------------------------------------------
 */
void	udp_sender(uid32 slot)
{
	uint16 newlinecount = 0; // flag for termination
	char *buff = getmem(128);
	while(TRUE) {
		fgets(buff, 128, CONSOLE);
		udp_send(slot, buff, 128);
		if(strcmp("\n", buff) == 0) { // check for possible termination
			newlinecount++;
			if(newlinecount == 2) { // double newline will terminate process
				kprintf("exiting chat room.\n");
				exit();
			}
			continue;
		}
		newlinecount = 0;
	}
}


/*------------------------------------------------------------------------
 * udp_receiver - listen for new messages
 *------------------------------------------------------------------------
 */
void	udp_receiver(uid32 slot)
{	
	uint16 newlinecount = 0;
	char *received = getmem(128);
	while(TRUE) {
		uint32 status = udp_recv(slot, received, 128, 500);
		if(status != SYSERR && status != TIMEOUT) {
			if(strcmp("\n", received) == 0) { // check for possible termination
				newlinecount++;
				if(newlinecount == 2) { // double newline will terminate process
					kprintf("exiting chat room.\n");
					exit();
				}
				continue;
			}
			kprintf("%s", received);
			newlinecount = 0;
		}
	}
}