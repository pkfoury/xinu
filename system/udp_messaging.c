/* udp_messaging.c - udp_sender, udp_receiver */

#include <xinu.h>

/*------------------------------------------------------------------------
 * udp_sender - send new message to remote ip
 *------------------------------------------------------------------------
 */
void	udp_sender(uid32 slot)
{
	char *buff = getmem(128);
	while(TRUE) {
		read(CONSOLE, buff, 128);
		udp_send(slot, buff, 128);
	}
}


/*------------------------------------------------------------------------
 * udp_receiver - listen for new messages
 *------------------------------------------------------------------------
 */
void	udp_receiver(uid32 slot)
{	
	char *received = getmem(128);
	while(TRUE) {
		udp_recv(slot, received, 128, 500);
		kprintf("%s", received);
	}
}