/* shell.c  -  shell */

#include <xinu.h>
#include <stdio.h>
#include "shprototypes.h"

/************************************************************************/
/* Table of Xinu shell commands and the function associated with each.	
  for lab7, only builtin command is exit. */
/************************************************************************/
const	struct	cmdent	cmdtab[] = {
	{"argecho",	FALSE,	xsh_argecho},
	{"arp",		FALSE,	xsh_arp},
	{"cat",		FALSE,	xsh_cat},
	{"clear",	FALSE,	xsh_clear},
	{"date",	FALSE,	xsh_date},
	{"devdump",	FALSE,	xsh_devdump},
	{"echo",	FALSE,	xsh_echo},
	{"exit",	TRUE,	xsh_exit},
	{"help",	FALSE,	xsh_help},
	{"kill",	FALSE,	xsh_kill},
	{"ls",		FALSE,	xsh_ls},
	{"memdump",	FALSE,	xsh_memdump},
	{"memstat",	FALSE,	xsh_memstat},
	{"netinfo",	FALSE,	xsh_netinfo},
	{"ping",	FALSE,	xsh_ping},
	{"ps",		FALSE,	xsh_ps},
	{"sleep",	FALSE,	xsh_sleep},
	{"udp",		FALSE,	xsh_udpdump},
	{"udpecho",	FALSE,	xsh_udpecho},
	{"udpeserver",	FALSE,	xsh_udpeserver},
	{"uptime",	FALSE,	xsh_uptime},
	{"?",		FALSE,	xsh_help}

};

uint32	ncmd = sizeof(cmdtab) / sizeof(struct cmdent);

/************************************************************************/
/* shell  -  Provide an interactive user interface that executes	*/
/*	     commands.  Each command begins with a command name, has	*/
/*	     a set of optional arguments, has optional input or		*/
/*	     output redirection, and an optional specification for	*/
/*	     background execution (ampersand).  The syntax is:		*/
/*									*/
/*		   command_name [args*] [redirection] [&]		*/
/*									*/
/*	     Redirection is either or both of:				*/
/*									*/
/*				< input_file				*/
/*			or						*/
/*				> output_file				*/
/*									*/
/************************************************************************/

process	shell (
		did32	dev		/* ID of tty device from which	*/
	)				/*   to accept commands		*/
{
	char	buf[SHELL_BUFLEN];	/* Input line (large enough for	*/
					/*   one line from a tty device	*/
	int32	len;			/* Length of line read		*/
	char	tokbuf[SHELL_BUFLEN + SHELL_MAXTOK]; /* Buffer to hold a set of contiguous null-terminated strings of tokens		*/
	int32	tlen;			/* Current length of all data	*/
					/*   in array tokbuf		*/
	int32	tok[SHELL_MAXTOK];	/* Index of each token in	*/
					/*   array tokbuf		*/
	int32	toktyp[SHELL_MAXTOK];	/* Type of each token in tokbuf	*/
	int32	ntok;			/* Number of tokens on line	*/
	pid32	child;			/* Process ID of spawned child	*/
	bool8	backgnd;		/* Run command in background?	*/
	char	*outname, *inname;	/* Pointers to strings for file	*/
					/*   names that follow > and <	*/
	did32	stdinput, stdoutput;	/* Descriptors for redirected	*/
					/*   input and output		*/
	int32	i;			/* Index into array of tokens	*/
	int32	j;			/* Index into array of commands	*/
	int32	k;			// index of piped command
	int32	msg;			/* Message from receive() for	*/
					/*   child termination		*/
	int32	tmparg;			/* Address of this var is used	*/
					/*   when first creating child	*/
					/*   process, but is replaced	*/
	int32 tmparg2;
	char *src, *cmp; /* Pointers used during name	*/
									 /*   comparison			*/
	bool8	diff;			/* Was difference found during	*/
					/*   comparison			*/
	char	*args[SHELL_MAXTOK];	/* Argument vector passed to	*/
					/*   builtin commands		*/
	int32 pipeindex; // index of pipe in tokbuf

	/* Print shell banner and startup message */

	fprintf(dev, "\n\n%s%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
		SHELL_BAN0,SHELL_BAN1,SHELL_BAN2,SHELL_BAN3,SHELL_BAN4,
		SHELL_BAN5,SHELL_BAN6,SHELL_BAN7,SHELL_BAN8,SHELL_BAN9);

	fprintf(dev, "%s\n\n", SHELL_STRTMSG);

	/* Continually prompt the user, read input, and execute command	*/

	while (TRUE) {

		/* Display prompt */

		fprintf(dev, SHELL_PROMPT);

		/* Read a command */

		len = read(dev, buf, sizeof(buf));

		/* Exit gracefully on end-of-file */

		if (len == EOF) {
			break;
		}

		/* If line contains only NEWLINE, go to next line */

		if (len <= 1) {
			continue;
		}

		buf[len] = SH_NEWLINE;	/* terminate line */

		/* Parse input line and divide into tokens */

		ntok = lexan(buf, len, tokbuf, &tlen, tok, toktyp);

		/* Handle parsing error */

		if (ntok == SYSERR) {
			fprintf(dev,"%s\n", SHELL_SYNERRMSG);
			continue;
		}

		/* If line is empty, go to next input line */

		if (ntok == 0) {
			fprintf(dev, "\n");
			continue;
		}

		/* If last token is '&', set background */

		if (toktyp[ntok-1] == SH_TOK_AMPER) {
			ntok-- ;
			tlen-= 2;
			backgnd = TRUE;
		} else {
			backgnd = FALSE;
		}

		// look for pipe token, to be processed later
		bool8 piped = FALSE;
		int32 pipetok = 0; // index of pipe in tok[]

		for(; pipetok < ntok; pipetok++) {
			if(toktyp[pipetok] == SH_TOK_PIPE) { // find pipe token
				piped = TRUE;
				pipeindex = tok[pipetok];
				break;
			}
		}
		if (piped && ntok < 3) {
			fprintf(dev, "%s\n", SHELL_SYNERRMSG);
			continue;
		}

		/* Check for input/output redirection (default is none) */

		outname = inname = NULL;
		if ( (ntok >=3) && ( (toktyp[ntok-2] == SH_TOK_LESS)
				   ||(toktyp[ntok-2] == SH_TOK_GREATER))){ // if redirection token is present
			if (toktyp[ntok-1] != SH_TOK_OTHER) { // usage check
				fprintf(dev,"%s\n", SHELL_SYNERRMSG);
				continue;
			}
			if (toktyp[ntok-2] == SH_TOK_LESS) {
				inname = &tokbuf[tok[ntok - 1]]; // less token = input
			} else {
				outname = &tokbuf[tok[ntok - 1]]; // greater token = output
			}
			ntok -= 2;
			tlen = tok[ntok];
		}


		if ( (ntok >=3) && ( (toktyp[ntok-2] == SH_TOK_LESS)
				   ||(toktyp[ntok-2] == SH_TOK_GREATER))){
			if (toktyp[ntok-1] != SH_TOK_OTHER) {
				fprintf(dev,"%s\n", SHELL_SYNERRMSG);
				continue;
			}
			if (toktyp[ntok-2] == SH_TOK_LESS) {
				if (inname != NULL) {
				    fprintf(dev,"%s\n", SHELL_SYNERRMSG);
				    continue;
				}
				inname = &tokbuf[tok[ntok-1]];
			} else {
				if (outname != NULL) {
				    fprintf(dev,"%s\n", SHELL_SYNERRMSG);
				    continue;
				}
				outname = &tokbuf[tok[ntok-1]];
			}
			ntok -= 2;
			tlen = tok[ntok];
		}

		/* Verify remaining tokens are type "other" */

		for (i=0; i<ntok; i++) {
			if (toktyp[i] != SH_TOK_OTHER && toktyp[i] != SH_TOK_PIPE) { // allow pipe to continue
				break;
			}
		}
		if ((ntok == 0) || (i < ntok)) {
			fprintf(dev, SHELL_SYNERRMSG);
			continue;
		}

		stdinput = stdoutput = dev;

		/* Lookup first token in the command table */

		for (j = 0; j < ncmd; j++) {
			src = cmdtab[j].cname;
			cmp = tokbuf;
			diff = FALSE;
			while (*src != NULLCH) {
				if (*cmp != *src) {
					diff = TRUE;
					break;
				}
				src++;
				cmp++;
			}
			if (diff || (*cmp != NULLCH)) {
				continue;
			} else {
				break;
			}
		}

		/* Handle command not found */

		if (j >= ncmd) {
			fprintf(dev, "command %s not found\n", tokbuf);
			continue;
		}

		/* Handle built-in command */

		if (cmdtab[j].cbuiltin) { /* No background or redirect. */
			if (inname != NULL || outname != NULL || backgnd){
				fprintf(dev, SHELL_BGERRMSG);
				continue;
			} else {
				/* Set up arg vector for call */

				for (i=0; i<ntok; i++) {
					args[i] = &tokbuf[tok[i]];
				}

				/* Call builtin shell function */

				if ((*cmdtab[j].cfunc)(ntok, args)
							== SHELL_EXIT) {
					break;
				}
			}
			continue;
		}

		// handle pipe. only non-builtin commands supported
		if (piped) {
			// get second command
			for (k = 0; k < ncmd; k++) {
				src = cmdtab[k].cname;
				cmp = &tokbuf[tok[pipetok + 1]]; // start looking after pipe token				
				diff = FALSE;
				while (*src != NULLCH) {
					if (*cmp != *src) {
						diff = TRUE;
						break;
					}
					src++;
					cmp++;
				}
				if (diff || (*cmp != NULLCH)) {
					continue;
				} else {
					break;
				}
			}
		
			// handle command not found
			if (k >= ncmd) {
				fprintf(dev, "command not found\n");
				continue;
			}

			struct cmdent cmd1 = cmdtab[j];
			struct cmdent cmd2 = cmdtab[k];

			if (cmd2.cbuiltin) {
				kprintf("Builtin command in pipe not allowed\n");
				continue;
			}

			// kprintf("ntok %d\n", ntok);
			// kprintf("pipetok %d\n", pipetok);
			// kprintf("tlen %d\n", tlen);
			// kprintf("pipeindex %d\n", pipeindex);
			// kprintf("tok[pipetok] %d\n", tok[pipetok]);
			// kprintf("tokbuf[pipeindex] %c\n", tokbuf[pipeindex]);

			// create piped processes
			pid32 proc1 = create(cmd1.cfunc, SHELL_CMDSTK, SHELL_CMDPRIO, cmd1.cname, 2, ntok, &tmparg);
			pid32 proc2 = create(cmd2.cfunc, SHELL_CMDSTK, SHELL_CMDPRIO, cmd2.cname, 2, ntok, &tmparg2);

			// open pipe and set in process table
			open(PIPE, NULL, NULL);
			proctab[proc1].prdesc[1] = PIPE; // set stdout to PIPE
			proctab[proc2].prdesc[0] = PIPE; // set stdin to PIPE

			// add args to new processes
			status argstatus1 = addargs(proc1, 
				pipetok, // count of args. pipetok will be number of args before pipe
				tok,
				pipeindex - 1, // 0 to index of pipe - 1
				tokbuf, 
				&tmparg);

			status argstatus2 = addargs(proc2,
				ntok - pipetok - 1, // total - args before the pipe - 1 for the pipe itself
				tok, // ??????
				tlen - pipeindex, // total length of tokbuf - pipe index for len after pipe
				&tokbuf[tok[pipetok + 1]], // start at tokbuf after pipe
				&tmparg2);

			if ((proc1 == SYSERR) || (proc2 == SYSERR) || argstatus1 == SYSERR || argstatus2 == SYSERR) {
				fprintf(dev, SHELL_CREATMSG);
				continue;
			}

			resume(proc1);
			resume(proc2);

		} else {

			/* Open files and redirect I/O if specified */

			if (inname != NULL) {
				stdinput = open(NAMESPACE,inname,"ro");
				if (stdinput == SYSERR) {
					fprintf(dev, SHELL_INERRMSG, inname);
					continue;
				}
			}
			if (outname != NULL) {
				stdoutput = open(NAMESPACE,outname,"w");
				if (stdoutput == SYSERR) {
					fprintf(dev, SHELL_OUTERRMSG, outname);
					continue;
				} else {
					control(stdoutput, F_CTL_TRUNC, 0, 0);
				}
			}

			/* Spawn child thread for non-built-in commands */

			child = create(cmdtab[j].cfunc,
				SHELL_CMDSTK, SHELL_CMDPRIO,
				cmdtab[j].cname, 2, ntok, &tmparg);

			/* If creation or argument copy fails, report error */

			if ((child == SYSERR) ||
					(addargs(child, ntok, tok, tlen, tokbuf, &tmparg)
								== SYSERR) ) {
				fprintf(dev, SHELL_CREATMSG);
				continue;
			}

			/* Set stdinput and stdoutput in child to redirect I/O */

			proctab[child].prdesc[0] = stdinput;
			proctab[child].prdesc[1] = stdoutput;

			msg = recvclr();
			resume(child);
			if (! backgnd) {
				msg = receive();
				while (msg != child) {
					msg = receive();
				}
			}
		}
	}

    /* Terminate the shell process by returning from the top level */

    fprintf(dev,SHELL_EXITMSG);
    return OK;
}
