// pipe.h

#define PIPESIZE 32 // one byte buffer
struct pipe {
	bool8 open;
	int32 *buf[PIPESIZE];
	int32 *head;
	int32 *tail;
	sid32 psem;
	sid32 csem;
};

extern struct pipe pipe;