// pipe.h

struct pipe {
	int32 *buff;
	int32 *head;
	int32 *tail;
	sid32 psem;
	sid32 csem;
};

extern struct pipe *pipe;