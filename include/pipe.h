// pipe.h
#define NBYTES 1 // size of pipe buffer
struct pipe {
	bool8 open;
	int32 *buf[NBYTES * 32];
	int32 *head;
	int32 *tail;
	sid32 psem; // producer semaphore
	sid32 csem; // consumer semaphore
};

extern struct pipe pipe;