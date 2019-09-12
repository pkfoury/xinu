/* pc.c -  produce, consume, produce_consume_init, create_producer, create_consumer, produce_consume_destory*/

#include <xinu.h>

//standard variables used by testcases

char buffer[PCBUFFER];
uint8 producer_index = 0;
uint8 consumer_index = 0;
sid32 producer_sem = NULL;
sid32 consumer_sem = NULL;
uint32 produced = 0;
uint32 consumed = 0;
uint8 nproducers = 0;
uint8 nconsumers = 0;
pid32 producers[MAXPRODUCERS];
pid32 consumers[MAXCONSUMERS];

/*------------------------------------------------------------------------
 * produce - produces quantity of characters for the consumer to print
 *------------------------------------------------------------------------
 */
void produce(
		char c,
		uint8 quantity
	)
{
	//TODO: safely produce a character and place it in the buffer, track number of characters produced and index into the buffer.
}

/*------------------------------------------------------------------------
 * consume - consumes characters from the producer
 *------------------------------------------------------------------------
 */
void consume(
		uint8 quantity
	)
{
	//TODO: safely consume a character and remove it from the buffer, track number of characters consumed and index into the buffer.
	char c = 'a';
	kprintf("%c", c);	//feel free to rename 'c', but print in this format
}

/*------------------------------------------------------------------------
 * produce_consume_init - initializes producer and consumer variables
 *------------------------------------------------------------------------
 */
void produce_consume_init(
		void
	)
{
	producer_sem = semcreate(1);
	consumer_sem = semcreate(0);
}

/*------------------------------------------------------------------------
 * create_producer - creates a producer thread
 *------------------------------------------------------------------------
 */
void create_producer(
		char c,
		uint8 quantity
	)
{
	if (nproducers == MAXPRODUCERS) {
		return;
	}
	producers[nproducers] = create(produce, 512, 10, "Producer", c, quantity);
	resume(producers[nproducers]);
	nproducers++;
}

/*------------------------------------------------------------------------
 * create_consumer - creates a consumer thread
 *------------------------------------------------------------------------
 */
void create_consumer(
		uint8 quantity
	)
{
	if (nconsumers == MAXCONSUMERS) {
		return;
	}
	consumers[nconsumers] = create(consume, 512, 10, "Consumer", quantity);
	resume(consumers[nconsumers]);
	nconsumers++;
}

/*------------------------------------------------------------------------
 * produc_consume_destroy - gracefully destroys producers and consumers
 *------------------------------------------------------------------------
 */
void produce_consume_destroy(
		void
	)
{
	wait(producer_sem);
	uint8 i;
	for (i = 0; i < nproducers; i++) {
		kill(producers[i]);
	}
	for (i = 0; i < nconsumers; i++) {
		kill(consumers[i]);
	}
	semdelete(producer_sem);
	semdelete(consumer_sem);
	kprintf("Produced %d characters, consumed %d characters, %d leftover.\n",
		produced, consumed, producer_index - consumer_index);
	kprintf("Leftovers:\n");
	for (i = 0; i < PCBUFFER; i++) {
		kprintf("%c", buffer[i]);
	}
}
