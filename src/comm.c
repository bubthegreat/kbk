/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Thanks to abaddon for proof-reading our comm.c and pointing out bugs.  *
 *  Any remaining bugs are, of course, our work, not his.  :)              *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
 *	ROM 2.4 is copyright 1993-1996 Russ Taylor			   *
 *	ROM has been brought to you by the ROM consortium		   *
 *	    Russ Taylor (rtaylor@pacinfo.com)				   *
 *	    Gabrielle Taylor (gtaylor@pacinfo.com)			   *
 *	    Brian Moore (rom@rom.efn.org)				   *
 *	By using this code, you have agreed to follow the terms of the	   *
 *	ROM license, in the file Tartarus/doc/rom.license                  *
 ***************************************************************************/

/***************************************************************************
 *       Tartarus code is copyright (C) 1997-1998 by Daniel Graham          *
 *	In using this code you agree to comply with the Tartarus license   *
 *       found in the file /Tartarus/doc/tartarus.doc                       *
 ***************************************************************************/

#include "include.h"

#include <signal.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "telnet.h"

void announce_login args((CHAR_DATA * ch));
void announce_logout args((CHAR_DATA * ch));
const char *lowstring(const char *i);
void reverse_word args((char *w, int n));
bool wizlock = FALSE;
bool newlock = FALSE;
void init_signals args((void));
void do_auto_shutdown args((void));

/*
 * Malloc debugging stuff.
 */
#if defined(sun)
#undef MALLOC_DEBUG
#endif

#if defined(MALLOC_DEBUG)
#include <malloc.h>
extern int malloc_debug args((int));
extern int malloc_verify args((void));
#endif

char echo_off_str[] = {IAC, WILL, TELOPT_ECHO, '\0'};
const char echo_on_str[] = {IAC, WONT, TELOPT_ECHO, '\0'};
const char go_ahead_str[] = {IAC, GA, '\0'};

char *const he_she[] = {"it", "he", "she"};
char *const him_her[] = {"it", "him", "her"};
char *const his_her[] = {"its", "his", "her"};

int close args((int fd));
int gofday args((struct timeval * tp, struct timezone *tzp));
int select args((int width, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout));
int socket args((int domain, int type, int protocol));

/*
 * Global variables.
 */
DESCRIPTOR_DATA *descriptor_list;	  /* All open descriptors		*/
DESCRIPTOR_DATA *d_next;			  /* Next descriptor in loop	*/
FILE *fpReserve;					  /* Reserved file handle		*/
bool god;							  /* All new chars are gods!	*/
bool merc_down;						  /* Shutdown			        */
bool wizlock;						  /* Game is wizlocked		*/
bool newlock;						  /* Game is newlocked		*/
bool arena;							  /* Game is in arena		    */
bool MOBtrigger = TRUE;				  /* act() switch             */
char str_boot_time[MAX_INPUT_LENGTH]; /* Dunno what this bitch is */
time_t current_time;				  /* time of this pulse       */
time_t boot_time;					  /* Boot Time                */

void game_loop_unix args((int control));
int init_socket args((int port));
void init_descriptor args((int control));
bool read_from_descriptor args((DESCRIPTOR_DATA * d));
bool write_to_descriptor args((int desc, char *txt, int length));
bool check_parse_name args((char *name));
bool check_reconnect args((DESCRIPTOR_DATA * d, char *name, bool fConn));
bool check_playing args((DESCRIPTOR_DATA * d, char *name));
int main args((int argc, char **argv));
void nanny args((DESCRIPTOR_DATA * d, char *argument));
bool process_output args((DESCRIPTOR_DATA * d, bool fPrompt));
void read_from_buffer args((DESCRIPTOR_DATA * d));
void stop_idling args((CHAR_DATA * ch));
void bust_a_prompt args((CHAR_DATA * ch));
bool output_buffer args((DESCRIPTOR_DATA * d));
void process_text args((CHAR_DATA * ch, char *text));

int port;
int control;
bool fCopyOver;

int main(int argc, char **argv)
{
	log_string("Entered Main Loop.");
	struct timeval now_time;

	fCopyOver = FALSE;

/* Memory debugging if needed. */
#if defined(MALLOC_DEBUG)
	malloc_debug(2);
#endif

	/* Init time. */
	gettimeofday(&now_time, NULL);
	current_time = (time_t)now_time.tv_sec;
	strcpy(str_boot_time, ctime(&current_time));
	boot_time = current_time;

	/* Reserve one channel for our use. */
	if ((fpReserve = fopen(NULL_FILE, "r")) == NULL)
	{
		perror(NULL_FILE);
		exit(1);
	}

	/*
	 * Get the port number.
	 */
	port = 8989;
	if (argc != 1)
	{
		if (!is_number(argv[1]))
		{
			fprintf(stderr, "Usage: %s [port #]\n", argv[0]);
			exit(1);
		}
		else if ((port = atoi(argv[1])) <= 1024)
		{
			fprintf(stderr, "Port number must be above 1024.\n");
			exit(1);
		}
		/* Are we recovering from a copyover? */
		if (argv[2] && argv[2][0])
		{
			fCopyOver = TRUE;
			control = atoi(argv[3]);
		}
		else
			fCopyOver = FALSE;
	}

	/* Run the game. */
	if (!fCopyOver)
		control = init_socket(port);
	boot_db();
	sprintf(log_buf, "KBK booted, binding on port %d.", port);
	log_string(log_buf);

	if (fCopyOver)
		copyover_recover();
	game_loop_unix(control);
	close(control);

	/* That's all, folks. */
	log_string("Normal termination of game.");
	exit(0);
	return 0;
}

#if defined(unix)
int init_socket(int port)
{
	log_string("init_socket");
	static struct sockaddr_in sa_zero;
	struct sockaddr_in sa;
	int x = 1;
	int fd;

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Init_socket: socket");
		exit(1);
	}

	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&x, sizeof(x)) < 0)
	{
		perror("Init_socket: SO_REUSEADDR");
		close(fd);
		exit(1);
	}

#if defined(SO_DONTLINGER) && !defined(SYSV)
	{
		struct linger ld;

		ld.l_onoff = 1;
		ld.l_linger = 1000;

		if (setsockopt(fd, SOL_SOCKET, SO_DONTLINGER, (char *)&ld, sizeof(ld)) < 0)
		{
			perror("Init_socket: SO_DONTLINGER");
			close(fd);
			exit(1);
		}
	}
#endif

	sa = sa_zero;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);

	if (bind(fd, (struct sockaddr *)&sa, sizeof(sa)) < 0)
	{
		perror("Init socket: bind");
		close(fd);
		exit(1);
	}

	if (listen(fd, 3) < 0)
	{
		perror("Init socket: listen");
		close(fd);
		exit(1);
	}
	return fd;
}
#endif

#if defined(macintosh) || defined(MSDOS)
void game_loop_mac_msdos(void)
{
	struct timeval last_time;
	struct timeval now_time;
	static DESCRIPTOR_DATA dcon;

	gettimeofday(&last_time, NULL);
	current_time = (time_t)last_time.tv_sec;

	/* New_descriptor analogue. */
	dcon.descriptor = 0;
	dcon.connected = CON_GET_NAME;
	dcon.host = str_dup("localhost");
	dcon.outsize = 2000;
	dcon.outbuf = alloc_mem(dcon.outsize);
	dcon.next = descriptor_list;
	dcon.showstr_head = NULL;
	dcon.showstr_point = NULL;
	dcon.pEdit = NULL;
	dcon.pString = NULL;
	dcon.editor = 0;
	descriptor_list = &dcon;

	/* Send the greeting. */
	{
		log_string("Send Greeting");
		extern char *help_greeting;
		if (help_greeting[0] == '.')
			write_to_buffer(&dcon, help_greeting + 1, 0);
		else
			write_to_buffer(&dcon, help_greeting, 0);
	}

	/* Main loop */
	while (!merc_down)
	{
		DESCRIPTOR_DATA *d;

		/* Process input. */
		for (d = descriptor_list; d != NULL; d = d_next)
		{
			d_next = d->next;
			d->fcommand = FALSE;

#if defined(MSDOS)
			if (kbhit())
#endif
			{
				if (d->character != NULL)
					d->character->timer = 0;
				if (!read_from_descriptor(d))
				{
					if (d->character != NULL)
						save_char_obj(d->character);
					d->outtop = 0;
					close_socket(d);
					continue;
				}
			}

			if (d->character != NULL && d->character->daze > 0)
				--d->character->daze;

			if (d->character != NULL && d->character->wait > 0)
			{
				--d->character->wait;
				continue;
			}

			read_from_buffer(d);
			if (d->incomm[0] != '\0')
			{
				d->fcommand = TRUE;
				stop_idling(d->character);
				/* OLC */
				if (d->showstr_point)
					show_string(d, d->incomm);
				else if (d->pString)
					string_add(d->character, d->incomm);
				else
					switch (d->connected)
					{
					case CON_PLAYING:
						if (!run_olc_editor(d))
							substitute_alias(d, d->incomm);
						break;
					default:
						nanny(d, d->incomm);
						break;
					}
				d->incomm[0] = '\0';
			}
		}

		log_string("Handle autonomous game motion");
		/* Autonomous game motion. */
		update_handler();

		/* Output. */
		for (d = descriptor_list; d != NULL; d = d_next)
		{
			d_next = d->next;
			if ((d->fcommand || d->outtop > 0))
			{
				if (!process_output(d, TRUE))
				{
					if (d->character != NULL && d->character->level > 1)
						save_char_obj(d->character);
					d->outtop = 0;
					close_socket(d);
				}
			}
		}

		/*
		 * Synchronize to a clock.
		 * Busy wait (blargh).
		 */
		now_time = last_time;
		for (;;)
		{
			int delta;

#if defined(MSDOS)
			if (kbhit())
#endif
			{
				if (dcon.character != NULL)
					dcon.character->timer = 0;
				if (!read_from_descriptor(&dcon))
				{
					if (dcon.character != NULL && d->character->level > 1)
						save_char_obj(d->character);
					dcon.outtop = 0;
					close_socket(&dcon);
				}
#if defined(MSDOS)
				break;
#endif
			}

			gettimeofday(&now_time, NULL);
			delta = (now_time.tv_sec - last_time.tv_sec) * 1000 * 1000 + (now_time.tv_usec - last_time.tv_usec);
			if (delta >= 1000000 / PULSE_PER_SECOND)
				break;
		}
		last_time = now_time;
		current_time = (time_t)last_time.tv_sec;
	}

	return;
}
#endif

#if defined(unix)
void game_loop_unix(int control)
{
	log_string("Entered game_loop_unix.");
	static struct timeval null_time;
	struct timeval last_time;

	// signal( SIGPIPE, SIG_IGN );
	gettimeofday(&last_time, NULL);
	current_time = (time_t)last_time.tv_sec;

	/* Main loop */

	// init_signals();

	while (!merc_down)
	{
		fd_set in_set;
		fd_set out_set;
		fd_set exc_set;
		DESCRIPTOR_DATA *d;
		int maxdesc;

#if defined(MALLOC_DEBUG)
		if (malloc_verify() != 1)
			abort();
#endif

		/*
		 * Poll all active descriptors.
		 */
		FD_ZERO(&in_set);
		FD_ZERO(&out_set);
		FD_ZERO(&exc_set);
		FD_SET(control, &in_set);
		maxdesc = control;
		for (d = descriptor_list; d; d = d->next)
		{
			maxdesc = UMAX(maxdesc, d->descriptor);
			FD_SET(d->descriptor, &in_set);
			FD_SET(d->descriptor, &out_set);
			FD_SET(d->descriptor, &exc_set);
		}

		if (select(maxdesc + 1, &in_set, &out_set, &exc_set, &null_time) < 0)
		{
			perror("Game_loop: select: poll");
			exit(1);
		}

		/*
		 * New connection?
		 */
		if (FD_ISSET(control, &in_set))
			init_descriptor(control);

		/*
		 * Kick out the freaky folks.
		 */
		for (d = descriptor_list; d != NULL; d = d_next)
		{
			d_next = d->next;
			if (FD_ISSET(d->descriptor, &exc_set))
			{
				FD_CLR(d->descriptor, &in_set);
				FD_CLR(d->descriptor, &out_set);
				if (d->character && d->character->level > 1)
					save_char_obj(d->character);
				d->outtop = 0;
				close_socket(d);
			}
		}

		/*
		 * Process input.
		 */
		for (d = descriptor_list; d != NULL; d = d_next)
		{
			d_next = d->next;
			d->fcommand = FALSE;

			if (FD_ISSET(d->descriptor, &in_set))
			{
				if (d->character != NULL)
					d->character->timer = 0;
				if (!read_from_descriptor(d))
				{
					FD_CLR(d->descriptor, &out_set);
					if (d->character != NULL && d->character->level > 1)
						save_char_obj(d->character);
					d->outtop = 0;
					close_socket(d);
					continue;
				}
			}

			if (d->character != NULL && d->character->daze > 0)
				--d->character->daze;

			if (d->character != NULL && d->character->wait > 0)
			{
				--d->character->wait;
				continue;
			}

			read_from_buffer(d);
			if (d->incomm[0] != '\0')
			{
				d->fcommand = TRUE;
				stop_idling(d->character);

				/* OLC */
				if (d->showstr_point)
					show_string(d, d->incomm);
				else if (d->pString)
					string_add(d->character, d->incomm);
				else
					switch (d->connected)
					{
					case CON_PLAYING:
						if (d->character->pcdata && d->character->pcdata->entering_text)
							process_text(d->character, d->incomm);
						else if (!run_olc_editor(d))
							substitute_alias(d, d->incomm);
						break;
					default:
						nanny(d, d->incomm);
						break;
					}

				d->incomm[0] = '\0';
			}
		}

		/*
		 * Autonomous game motion.
		 */
		update_handler();

		/*
		 * Output.
		 */
		for (d = descriptor_list; d != NULL; d = d_next)
		{
			d_next = d->next;

			if ((d->fcommand || d->outtop > 0) && FD_ISSET(d->descriptor, &out_set))
			{
				if (!process_output(d, TRUE))
				{
					if (d->character != NULL && d->character->level > 1)
						save_char_obj(d->character);
					d->outtop = 0;
					close_socket(d);
				}
			}
		}

		/*
		 * Synchronize to a clock.
		 * Sleep( last_time + 1/PULSE_PER_SECOND - now ).
		 * Careful here of signed versus unsigned arithmetic.
		 */
		{
			struct timeval now_time;
			long secDelta;
			long usecDelta;

			gettimeofday(&now_time, NULL);
			usecDelta = ((int)last_time.tv_usec) - ((int)now_time.tv_usec) + 1000000 / PULSE_PER_SECOND;
			secDelta = ((int)last_time.tv_sec) - ((int)now_time.tv_sec);
			while (usecDelta < 0)
			{
				usecDelta += 1000000;
				secDelta -= 1;
			}

			while (usecDelta >= 1000000)
			{
				usecDelta -= 1000000;
				secDelta += 1;
			}

			if (secDelta > 0 || (secDelta == 0 && usecDelta > 0))
			{
				struct timeval stall_time;

				stall_time.tv_usec = usecDelta;
				stall_time.tv_sec = secDelta;
				if (select(0, NULL, NULL, NULL, &stall_time) < 0)
				{
					perror("Game_loop: select: stall");
					exit(1);
				}
			}
		}

		gettimeofday(&last_time, NULL);
		current_time = (time_t)last_time.tv_sec;
	}

	return;
}
#endif

#if defined(unix)
void init_descriptor(int control)
{
	log_string("Entered init_descriptor.");
	char buf[MAX_STRING_LENGTH];
	DESCRIPTOR_DATA *dnew;
	struct sockaddr_in sock;
	struct hostent *from;
	int desc;
	socklen_t size;

	size = sizeof(sock);
	getsockname(control, (struct sockaddr *)&sock, &size);
	if ((desc = accept(control, (struct sockaddr *)&sock, &size)) < 0)
	{
		perror("New_descriptor: accept");
		return;
	}

#if !defined(FNDELAY)
#define FNDELAY O_NDELAY
#endif

	if (fcntl(desc, F_SETFL, FNDELAY) == -1)
	{
		perror("New_descriptor: fcntl: FNDELAY");
		return;
	}

	/*
	 * Cons a new descriptor.
	 */
	dnew = new_descriptor();
	dnew->descriptor = desc;

	size = sizeof(sock);
	if (getpeername(desc, (struct sockaddr *)&sock, &size) < 0)
	{
		perror("New_descriptor: getpeername");
		dnew->host = str_dup("(unknown)");
	}
	else
	{
		/*
		 * Would be nice to use inet_ntoa here but it takes a struct arg,
		 * which ain't very compatible between gcc and system libraries.
		 */
		int addr;

		addr = ntohl(sock.sin_addr.s_addr);
		sprintf(buf, "%d.%d.%d.%d",
				(addr >> 24) & 0xFF, (addr >> 16) & 0xFF,
				(addr >> 8) & 0xFF, (addr) & 0xFF);
		dnew->ip = str_dup(buf);
		sprintf(log_buf, "Sock.sinaddr:  %s", buf);
		log_string(log_buf);
		from = gethostbyaddr((char *)&sock.sin_addr,
							 sizeof(sock.sin_addr), AF_INET);

		dnew->host = str_dup(from ? from->h_name : buf);
	}

	/*
	 * Swiftest: I added the following to ban sites.  I don't
	 * endorse banning of sites, but Copper has few descriptors now
	 * and some people from certain sites keep abusing access by
	 * using automated 'autodialers' and leaving connections hanging.
	 *
	 * Furey: added suffix check by request of Nickel of HiddenWorlds.
	 */

	if (check_ban(dnew->host, BAN_ALL))
	{
		write_to_descriptor(desc,
							"You're sitebanned. Go away.\n\r", 0);
		close(desc);
		free_descriptor(dnew);
		return;
	}
	log_string("You're not site banned, that's good.");
	/*
	 * Init descriptor data.
	 */
	dnew->next = descriptor_list;
	descriptor_list = dnew;

	/*
	 * Send the greeting.
	 */
	{
		extern char *help_greeting;
		if (help_greeting[0] == '.')
			write_to_buffer(dnew, help_greeting + 1, 0);
		else
			write_to_buffer(dnew, help_greeting, 0);
	}
	return;
}
#endif

void sig_handler(int sig)
{
	log_string("Entered sig_handler.");
	extern bool merc_down;

	if (!merc_down)
	{
		switch (sig)
		{
		case SIGBUS:
			bug("CRASH: Signal - SIGBUS.", 0);
			do_auto_shutdown();
			merc_down = TRUE;
			break;
		case SIGTERM:
			bug("CRASH: Signal - SIGTERM.", 0);
			do_auto_shutdown();
			merc_down = TRUE;
			break;
		case SIGABRT:
			bug("CRASH: Signal - SIGABRT", 0);
			do_auto_shutdown();
			merc_down = TRUE;
			break;
		case SIGSEGV:
			bug("CRASH: Signal - SIGSEGV", 0);
			do_auto_shutdown();
			merc_down = TRUE;
			break;
		}
	}
	else
	{
		exit(0);
	}
}

void init_signals()
{
	log_string("Entered init_signals.");
	signal(SIGBUS, sig_handler);
	signal(SIGTERM, sig_handler);
	signal(SIGABRT, sig_handler);
	signal(SIGSEGV, sig_handler);
}
void close_socket(DESCRIPTOR_DATA *dclose)
{
	log_string("Entered close_socket.");
	CHAR_DATA *ch;
	char buf[MAX_STRING_LENGTH];

	if (dclose->outtop > 0)
		process_output(dclose, FALSE);

	if (dclose->snoop_by != NULL)
	{
		write_to_buffer(dclose->snoop_by,
						"Your victim has left the game.\n\r", 0);
	}

	{
		DESCRIPTOR_DATA *d;

		for (d = descriptor_list; d != NULL; d = d->next)
		{
			if (d->snoop_by == dclose)
				d->snoop_by = NULL;
		}
	}

	if ((ch = dclose->character) != NULL)
	{
		sprintf(log_buf, "Closing link to %s.", ch->name);
		log_string(log_buf);
		/* cut down on wiznet spam when rebooting */
		if (dclose->connected == CON_PLAYING && !merc_down)
		{
			sprintf(buf, "(%s) $N has lost $S link: (Last fought %s %d seconds ago).",
					timestamp(),
					ch->last_fight_name != NULL ? ch->last_fight_name : "nobody",
					ch->last_fight_time ? (int)(current_time - ch->last_fight_time) : -1);
			save_char_obj(ch);

			wiznet(buf, ch, NULL, WIZ_LINKS, 0, get_trust(ch));
			ch->desc = NULL;
		}
		else
		{
			free_char(dclose->original ? dclose->original : dclose->character);
		}
	}

	if (d_next == dclose)
		d_next = d_next->next;

	if (dclose == descriptor_list)
	{
		descriptor_list = descriptor_list->next;
	}
	else
	{
		DESCRIPTOR_DATA *d;

		for (d = descriptor_list; d && d->next != dclose; d = d->next)
			;
		if (d != NULL)
			d->next = dclose->next;
		else
			bug("Close_socket: dclose not found.", 0);
	}

	close(dclose->descriptor);
	free_descriptor(dclose);
#if defined(MSDOS) || defined(macintosh)
	exit(1);
#endif
	return;
}
bool read_from_descriptor(DESCRIPTOR_DATA *d)
{
	log_string("Entered read_from_descriptor.");
	int iStart;

	/* Hold horses if pending command already. */
	if (d->incomm[0] != '\0')
		return TRUE;

	/* Check for overflow. */
	iStart = strlen(d->inbuf);
	if (iStart >= sizeof(d->inbuf) - 10)
	{
		sprintf(log_buf, "%s input overflow!", d->host);
		log_string(log_buf);
		write_to_descriptor(d->descriptor, "\n\r*** PUT A LID ON IT!!! ***\n\r", 0);
		return FALSE;
	}

	for (;;)
	{
		int nRead;

		nRead = read(d->descriptor, d->inbuf + iStart,
					 sizeof(d->inbuf) - 10 - iStart);
		if (nRead > 0)
		{
			iStart += nRead;
			if (d->inbuf[iStart - 1] == '\n' || d->inbuf[iStart - 1] == '\r')
				break;
		}
		else if (nRead == 0)
		{
			log_string("EOF encountered on read.");
			return FALSE;
		}
		else if (errno == EAGAIN)
			break;
		else
		{
			perror("Read_from_descriptor");
			return FALSE;
		}
	}

	d->inbuf[iStart] = '\0';
	return TRUE;
}
void read_from_buffer(DESCRIPTOR_DATA *d)
{
	/*
	 * Transfer one line from input buffer to input line.
	 */
	int i, j, k;

	/*
	 * Hold horses if pending command already.
	 */
	if (d->incomm[0] != '\0')
		return;

	/*
	 * Look for at least one new line.
	 */
	for (i = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++)
	{
		if (d->inbuf[i] == '\0')
			return;
	}

	/*
	 * Canonical input processing.
	 */
	for (i = 0, k = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++)
	{
		if (k >= MAX_INPUT_LENGTH - 2)
		{
			write_to_descriptor(d->descriptor, "Line too long.\n\r", 0);

			/* skip the rest of the line */
			for (; d->inbuf[i] != '\0'; i++)
			{
				if (d->inbuf[i] == '\n' || d->inbuf[i] == '\r')
					break;
			}
			d->inbuf[i] = '\n';
			d->inbuf[i + 1] = '\0';
			break;
		}

		if (d->inbuf[i] == '\b' && k > 0)
			--k;
		else if (isascii(d->inbuf[i]) && isprint(d->inbuf[i]))
			d->incomm[k++] = d->inbuf[i];
	}

	/*
	 * Finish off the line.
	 */
	if (k == 0)
		d->incomm[k++] = ' ';
	d->incomm[k] = '\0';

	/*
	 * Deal with bozos with #repeat 1000 ...
	 */

	if (k > 1 || d->incomm[0] == '!')
	{
		if (d->incomm[0] != '!' && strcmp(d->incomm, d->inlast))
		{
			d->repeat = 0;
		}
		else
		{
			if (++d->repeat >= 25 && d->character && d->connected == CON_PLAYING)
			{
				sprintf(log_buf, "%s input spamming!", d->host);
				log_string(log_buf);
				wiznet("Spam spam spam $N spam spam spam spam spam!",
					   d->character, NULL, WIZ_SPAM, 0, get_trust(d->character));
				if (d->incomm[0] == '!')
					wiznet(d->inlast, d->character, NULL, WIZ_SPAM, 0,
						   get_trust(d->character));
				else
					wiznet(d->incomm, d->character, NULL, WIZ_SPAM, 0,
						   get_trust(d->character));

				d->repeat = 0;
				d->character->move /= 2;
				d->character->mana -= 20;
				d->character->exp = d->character->exp - 100;
				write_to_descriptor(d->descriptor,
									"\n\rYour mind feels drained from trying to do the same thing over and over again...\n\r", 0);
				if (!IS_IMMORTAL(d->character))
				{
					WAIT_STATE(d->character, 24);
				}
			}
		}
	}

	/*
	 * Do '!' substitution.
	 */
	if (d->incomm[0] == '!')
		strcpy(d->incomm, d->inlast);
	else
		strcpy(d->inlast, d->incomm);

	/*
	 * Shift the input buffer.
	 */
	while (d->inbuf[i] == '\n' || d->inbuf[i] == '\r')
		i++;
	for (j = 0; (d->inbuf[j] = d->inbuf[i + j]) != '\0'; j++)
		;
	return;
}
bool process_output(DESCRIPTOR_DATA *d, bool fPrompt)
{
	log_string("Entered process_output.");
	/*
	 * Low level output function.
	 */
	extern bool merc_down;

	/*
	 * Bust a prompt.
	 */
	if (!merc_down && d->showstr_point)
		write_to_buffer(d, "[Hit Return to continue]\n\r", 0);
	else if (fPrompt && d->pString && d->connected == CON_PLAYING)
		write_to_buffer(d, "> ", 2);
	else if (fPrompt && d->connected == CON_PLAYING)
	{
		CHAR_DATA *ch;

		CHAR_DATA *victim;

		ch = d->character;

		/* battle prompt */
		if ((victim = ch->fighting) != NULL && can_see(ch, victim))
		{
			int percent;
			char wound[100];
			char *pbuff;
			char buf[MAX_STRING_LENGTH];
			char buffer[MAX_STRING_LENGTH * 2];

			if (victim->max_hit > 0)
				percent = victim->hit * 100 / victim->max_hit;
			else
				percent = -1;

			if (percent >= 100)
				sprintf(wound, "is in perfect condition.");
			else if (percent >= 90)
				sprintf(wound, "has a few scratches.");
			else if (percent >= 75)
				sprintf(wound, "has some small wounds.");
			else if (percent >= 50)
				sprintf(wound, "is covered in bleeding wounds.");
			else if (percent >= 30)
				sprintf(wound, "is gushing blood.");
			else if (percent >= 15)
				sprintf(wound, "is writhing in pain.");
			else if (percent >= 0)
				sprintf(wound, "is convulsing on the ground.");
			else
				sprintf(wound, "is nearly dead.");

			sprintf(buf, "%s %s \n\r",
					IS_NPC(victim) ? victim->short_descr : victim->name, wound);
			buf[0] = UPPER(buf[0]);
			pbuff = buffer;
			colorconv(pbuff, buf, d->character);
			write_to_buffer(d, buffer, 0);
		}

		ch = d->original ? d->original : d->character;
		if (!IS_SET(ch->comm, COMM_COMPACT))
			write_to_buffer(d, "\n\r", 2);

		if (!IS_NPC(ch) && ch->pcdata->entering_text)
			write_to_buffer(d, ": ", 2);

		else if (IS_SET(ch->comm, COMM_PROMPT))
			bust_a_prompt(d->character);

		if (IS_SET(ch->comm, COMM_TELNET_GA))
			write_to_buffer(d, go_ahead_str, 0);
	}

	/*
	 * Short-circuit if nothing to write.
	 */
	if (d->outtop == 0)
		return TRUE;

	/*
	 * Snoop-o-rama.
	 */
	if (d->snoop_by != NULL)
	{
		if (d->character != NULL)
		{
			write_to_buffer(d->snoop_by, d->character->original_name, 0);
		}
		write_to_buffer(d->snoop_by, "> ", 2);
		write_to_buffer(d->snoop_by, d->outbuf, d->outtop);
	}

	/*
	 * OS-dependent output.
	 *
	 * now done at output_buffer( ) to deal with color codes.
	 * - Wreck
	 */
	if (!write_to_descriptor(d->descriptor, d->outbuf, d->outtop))
	{
		d->outtop = 0;
		return FALSE;
	}
	else
	{
		d->outtop = 0;
		return TRUE;
	}
}
void bust_a_prompt(CHAR_DATA *ch)
{
	log_string("Entered bust_a_prompt.");
	/*
	 * Bust a prompt (player settable prompt)
	 * coded by Morgenes for Aldara Mud
	 */
	char buf[MAX_STRING_LENGTH];
	char buf2[MAX_STRING_LENGTH];
	const char *str;
	const char *i;
	char *point;
	char *pbuff;
	char buffer[MAX_STRING_LENGTH * 2];
	char doors[MAX_INPUT_LENGTH];
	EXIT_DATA *pexit;
	bool found;
	const char *dir_name[] = {"N", "E", "S", "W", "U", "D"};
	int door;

	point = buf;
	str = ch->prompt;

	if (!str || str[0] == '\0')
	{
		sprintf(buf, "<%dhp %dm %dmv> %s", ch->hit, ch->mana, ch->move, ch->prefix);
		send_to_char(buf, ch);
		return;
	}

	if (ch->desc->editor != 0)
	{
		send_to_char("[OLC Mode] ", ch);
		return;
	}

	while (*str != '\0')
	{
		if (*str != '%')
		{
			*point++ = *str++;
			continue;
		}

		++str;
		switch (*str)
		{
		default:
			i = " ";
			break;

		// Exits
		case 'e':
			found = FALSE;
			doors[0] = '\0';
			for (door = 0; door < 6; door++)
			{
				if ((pexit = ch->in_room->exit[door]) != NULL && pexit->u1.to_room != NULL && (can_see_room(ch, pexit->u1.to_room) || (IS_AFFECTED(ch, AFF_INFRARED) && !IS_AFFECTED(ch, AFF_BLIND))) && !IS_SET(pexit->exit_info, EX_CLOSED))
				{
					found = TRUE;
					strcat(doors, dir_name[door]);
				}
			}
			if (!found)
				strcat(buf, "none");
			sprintf(buf2, "%s", doors);
			i = buf2;
			break;

		// Carraige return
		case 'c':
			sprintf(buf2, "%s", "\n\r");
			i = buf2;
			break;

			// Current hp
		case 'h':
			sprintf(buf2, "%d", ch->hit);
			i = buf2;
			break;

		// Max hit
		case 'H':
			sprintf(buf2, "%d", ch->max_hit);
			i = buf2;
			break;

		case 'l':
			sprintf(buf2, "%d", PERCENT(ch->hit, ch->max_hit));
			i = buf2;
			break;

		case 'j':
			sprintf(buf2, "%d", PERCENT(ch->mana, ch->max_mana));
			i = buf2;
			break;

		case 'k':
			sprintf(buf2, "%d", PERCENT(ch->move, ch->max_move));
			i = buf2;
			break;

		// Current mana
		case 'm':
			sprintf(buf2, "%d", ch->mana);
			i = buf2;
			break;

			// Max mana
		case 'M':
			sprintf(buf2, "%d", ch->max_mana);
			i = buf2;
			break;

			// Current move
		case 'v':
			sprintf(buf2, "%d", ch->move);
			i = buf2;
			break;

			// Max move
		case 'V':
			sprintf(buf2, "%d", ch->max_move);
			i = buf2;
			break;

			// Gold
		case 'g':
			sprintf(buf2, "%ld", ch->gold);
			i = buf2;
			break;

		// Quest credits
		case 'Q':
			sprintf(buf2, "%ld", ch->quest_credits);
			i = buf2;
			break;

		// Swordplay Counter PCs
		case 'Z':
			if (ch->class == class_lookup("paladin"))
			{
				if (is_affected(ch, gsn_paladin_tech_counter_pc))
					sprintf(buf2, "%d", get_affect_level(ch, gsn_paladin_tech_counter_pc));
				else
					sprintf(buf2, "0");
			}
			else
				sprintf(buf2, "X");
			i = buf2;
			break;

		// Swordplay Counter NPCs
		case 'z':
			if (ch->class == class_lookup("paladin"))
			{
				if (is_affected(ch, gsn_paladin_tech_counter_npc))
					sprintf(buf2, "%d", get_affect_level(ch, gsn_paladin_tech_counter_npc));
				else
					sprintf(buf2, "0");
			}
			else
				sprintf(buf2, "X");
			i = buf2;
			break;

			// Room name
		case 'r':
			if (ch->in_room != NULL)
				sprintf(buf2, "%s",
						((!IS_NPC(ch) && IS_SET(ch->act, PLR_HOLYLIGHT)) ||
						 (!IS_AFFECTED(ch, AFF_BLIND) && !room_is_dark(ch->in_room)))
							? ch->in_room->name
							: "darkness");
			else
				sprintf(buf2, " ");
			i = buf2;
			break;

			// Vnum (immortals)
		case 'R':
			if (IS_IMMORTAL(ch) && ch->in_room != NULL)
				sprintf(buf2, "%ld", ch->in_room->vnum);
			else
				sprintf(buf2, " ");
			i = buf2;
			break;

		// Time
		case 'T':
			sprintf(buf2, "%d%s", (time_info.hour % 12 == 0) ? 12 : time_info.hour % 12, time_info.hour >= 12 ? "pm" : "am");
			i = buf2;
			break;

			// Area name (immortals)
		case 'A':
			if (IS_IMMORTAL(ch) && ch->in_room != NULL)
				sprintf(buf2, "%s", ch->in_room->area->name);
			else
				sprintf(buf2, " ");
			i = buf2;
			break;

		// Invis level (immortals)
		case 'w':
			sprintf(buf2, "%d", ch->invis_level);
			i = buf2;
			break;
		case 'W':
			sprintf(buf2, "%s", sector_table[ch->in_room->sector_type].desc);
			i = buf2;
			break;
		case 'I':
			sprintf(buf2, "%s", IS_OUTSIDE(ch) ? "outside" : "inside");
			i = buf2;
			break;

		case '%':
			sprintf(buf2, "%%");
			i = buf2;
			break;
		}
		++str;
		while ((*point = *i) != '\0')
			++point, ++i;
	}
	*point = '\0';
	pbuff = buffer;
	colorconv(pbuff, buf, ch);
	write_to_buffer(ch->desc, buffer, 0);

	if (ch->prefix[0] != '\0')
		write_to_buffer(ch->desc, ch->prefix, 0);
	return;
}

bool output_buffer(DESCRIPTOR_DATA *d)
{
	log_string("Entered output_buffer.");
	char buf[MAX_STRING_LENGTH];
	char buf2[128];
	const char *str;
	char *i;
	char *point;
	bool flash = FALSE, o_flash,
		 bold = FALSE, o_bold;
	bool act = FALSE, ok = TRUE, color_code = FALSE;
	int color = 7, o_color;

	/* discard NULL descriptor */
	if (d == NULL)
		return FALSE;

	bzero(buf, MAX_STRING_LENGTH);
	point = buf;
	str = d->outbuf;
	o_color = color;
	o_bold = bold;
	o_flash = flash;

	while (*str != '\0' && (str - d->outbuf) < d->outtop)
	{
		if ((int)(point - buf) >= MAX_STRING_LENGTH - 32)
		{
			/* buffer is full, so send it through the socket */
			*point++ = '\0';
			if (!(ok = write_to_descriptor(d->descriptor,
										   buf,
										   strlen(buf))))
				break;
			bzero(buf, MAX_STRING_LENGTH);
			point = buf;
		}

		if (*str != '#')
		{
			color_code = FALSE;
			*point++ = *str++;
			continue;
		}

		if (!color_code && *(str + 1) != '<')
		{
			o_color = color;
			o_bold = bold;
			o_flash = flash;
		}
		color_code = TRUE;

		act = FALSE;
		str++;
		switch (*str)
		{
		default:
			sprintf(buf2, "#%c", *str);
			break;
		case 'x':
			sprintf(buf2, "#");
			break;
		case '-':
			sprintf(buf2, "~");
			break;
		case '<':
			color = o_color;
			bold = o_bold;
			flash = o_flash;
			act = TRUE;
			break;
		case '0':
			color = 0;
			act = TRUE;
			break;
		case '1':
			color = 1;
			act = TRUE;
			break;
		case '2':
			color = 2;
			act = TRUE;
			break;
		case '3':
			color = 3;
			act = TRUE;
			break;
		case '4':
			color = 4;
			act = TRUE;
			break;
		case '5':
			color = 5;
			act = TRUE;
			break;
		case '6':
			color = 6;
			act = TRUE;
			break;
		case '7':
			color = 7;
			act = TRUE;
			break;
		case 'B':
			bold = TRUE;
			act = TRUE;
			break;
		case 'b':
			bold = FALSE;
			act = TRUE;
			break;
		case 'F':
			flash = TRUE;
			act = TRUE;
			break;
		case 'f':
			flash = FALSE;
			act = TRUE;
			break;
		case 'n':
			if (d->character && IS_ANSI(d->character))
				sprintf(buf2, "%s", ANSI_NORMAL);
			else
				buf2[0] = '\0';
			bold = FALSE;
			color = 7;
			flash = FALSE;
			break;
		}
		if (act)
		{
			buf2[0] = '\0';
		}

		i = buf2;
		str++;
		while ((*point = *i) != '\0')
			++point, ++i;
	}

	*point++ = '\0';
	ok = ok && (write_to_descriptor(d->descriptor, buf, strlen(buf)));
	d->outtop = 0;

	return ok;
}
void write_to_buffer(DESCRIPTOR_DATA *d, const char *txt, int length)
{
	log_string("Entered write_to_buffer.");
	/*
	 * Append onto an output buffer.
	 */
	int reverse;
	if (d->character != NULL && IS_AFFECTED(d->character, AFF_LOOKING_GLASS) && d->connected == CON_PLAYING)
		reverse = TRUE;

	else
		reverse = FALSE;

	/*
	 * Find length in case caller didn't.
	 */
	if (length <= 0)
		length = strlen(txt);

	/*
	 * Initial \n\r if needed.
	 */
	if (d->outtop == 0 && !d->fcommand)
	{
		d->outbuf[0] = '\n';
		d->outbuf[1] = '\r';
		d->outtop = 2;
	}

	/*
	 * Expand the buffer as needed.
	 */
	while (d->outtop + length >= d->outsize)
	{
		char *outbuf;
		if (d->outsize >= 32000)
		{
			bug("Buffer overflow. Closing.\n\r", 0);
			close_socket(d);
			return;
		}
		outbuf = alloc_mem(2 * d->outsize);
		strncpy(outbuf, d->outbuf, d->outtop);
		free_mem(d->outbuf, d->outsize);
		d->outbuf = outbuf;
		d->outsize *= 2;
	}

	/*
	 * Copy.
	 */
	strcpy(d->outbuf + d->outtop, txt);
	if (reverse)
		reverse_txt(d->outbuf + d->outtop, length);
	d->outtop += length;
	return;
}
bool write_to_descriptor(int desc, char *txt, int length)
{
	log_string("Entered write_to_descriptor.");
	/*
	 * Lowest level output function.
	 * Write a block of text to the file descriptor.
	 * If this gives errors on very long blocks (like 'ofind all'),
	 *   try lowering the max block size.
	 */
	int iStart;
	int nWrite;
	int nBlock;

#if defined(macintosh) || defined(MSDOS)
	if (desc == 0)
		desc = 1;
#endif

	if (length <= 0)
		length = strlen(txt);

	for (iStart = 0; iStart < length; iStart += nWrite)
	{
		nBlock = UMIN(length - iStart, 4096);
		if ((nWrite = write(desc, txt + iStart, nBlock)) < 0)
		{
			perror("Write_to_descriptor");
			return FALSE;
		}
	}

	return TRUE;
}
void nanny(DESCRIPTOR_DATA *d, char *argument)
{
	log_string("Entered nanny.");
	DESCRIPTOR_DATA *d_old, *d_next;
	char buf[MAX_STRING_LENGTH];
	char arg[MAX_INPUT_LENGTH];
	char pbuf[MSL];
	CHAR_DATA *ch;
	OBJ_DATA *fobj;
	OBJ_DATA *fobj_next;
	char *pwdnew;
	char *p;
	int iClass;
	int race, i, type, sn, iCabal;
	bool fOld;
	int dh, col, hometown;
	FILE *deadchar;
	char buffile[MAX_STRING_LENGTH];

	while (isspace(*argument))
		argument++;

	ch = d->character;

	switch (d->connected)
	{
	default:
		bug("Nanny: bad d->connected %d.", d->connected);
		close_socket(d);
		return;
	case CON_GET_NAME:
		if (argument[0] == '\0')
		{
			close_socket(d);
			return;
		}

		argument[0] = UPPER(argument[0]);
		if (!check_parse_name(argument))
		{
			write_to_buffer(d, "Illegal name, try another.\n\rName: ", 0);
			return;
		}
		sprintf(buffile, "%sdead_char/%s.plr", PLAYER_DIR, argument);
		if ((deadchar = fopen(buffile, "r")) != NULL)
		{
			write_to_buffer(d, "You are denied access.\n\r", 0);
			close_socket(d);
			return;
		}

		fOld = load_char_obj(d, argument);
		ch = d->character;

		if (IS_SET(ch->act, PLR_DENY))
		{
			sprintf(log_buf, "Denying access to %s@%s.", argument, d->host);
			log_string(log_buf);
			write_to_buffer(d, "You are denied access.\n\r", 0);
			close_socket(d);
			return;
		}

		if (check_ban(d->host, BAN_PERMIT) && !IS_SET(ch->act, PLR_PERMIT))
		{
			write_to_buffer(d, "Your site has been banned from this mud.\n\r", 0);
			close_socket(d);
			return;
		}

		if (check_reconnect(d, argument, FALSE))
		{
			fOld = TRUE;
		}
		else
		{
			if (arena && !IS_IMMORTAL(ch))
			{
				write_to_buffer(d, "The game is in arena mode, try again later.\n\r", 0);
				close_socket(d);
				return;
			}

			if (wizlock && !IS_IMMORTAL(ch))
			{
				write_to_buffer(d, "The game is wizlocked.\n\r", 0);
				close_socket(d);
				return;
			}
		}

		if (fOld)
		{
			/* Old player */
			write_to_buffer(d, "Password: ", 0);
			write_to_buffer(d, echo_off_str, 0);
			d->connected = CON_GET_OLD_PASSWORD;
			return;
		}
		else
		{
			/* New player */
			if (newlock)
			{
				write_to_buffer(d, "The game is newlocked.\n\r", 0);
				close_socket(d);
				return;
			}

			if (check_ban(d->host, BAN_NEWBIES))
			{
				write_to_buffer(d,
								"New players are not allowed from your site.\n\r", 0);
				close_socket(d);
				return;
			}

			sprintf(buf, "Did I get that right, %s (Y/N)? ", argument);
			write_to_buffer(d, buf, 0);
			d->connected = CON_CONFIRM_NEW_NAME;

			return;
		}
		break;

	case CON_GET_OLD_PASSWORD:
		write_to_buffer(d, "\n\r", 2);

		if (strcmp(argument, ch->pcdata->pwd))
		{
			write_to_buffer(d, "Wrong password.\n\r", 0);
			sprintf(pbuf, "BAD PASSWORD ATTEMPT ON PLAYER %s BY SITE %s.",
					ch->name, d->host);
			wiznet(pbuf, ch, NULL, 0, 0, 0);

			for (fobj = ch->carrying; fobj != NULL; fobj = fobj_next)
			{
				fobj_next = fobj->next_content;
				fobj->pIndexData->limcount++;
			}

			close_socket(d);
			return;
		}

		write_to_buffer(d, echo_on_str, 0);

		if (check_playing(d, (ch->original_name ? ch->original_name : ch->name)))
			return;

		if (check_reconnect(d, ch->name, TRUE))
			return;

		free_string(ch->pcdata->logon_time);
		ch->pcdata->logon_time = str_dup(ctime(&current_time));
		ch->pcdata->logon_time[strlen(ch->pcdata->logon_time) - 1] = '\0';

		sprintf(log_buf, "(%s) %s@%s has connected. Room: %ld %s",
				timestamp(),
				(ch->original_name ? ch->original_name : ch->name),
				d->host,
				ch->in_room->vnum,
				auto_check_multi(d, d->host) ? " (MULTI-CHAR?)" : "");

		log_string(log_buf);
		log_string("Trying to update site track.");
		update_sitetrack(ch, d->host);
		log_string("Updated site tracker.");
		log_string("Running login_log.");
		login_log(ch, LTYPE_IN);
		log_string("Ran login_log.");
		log_string("Running wiznet.");
		wiznet(log_buf, NULL, NULL, WIZ_SITES, 0, get_trust(ch));
		log_string("Ran wiznet.");

		if (IS_IMMORTAL(ch))
		{
			do_help(ch, "imotd");
			d->connected = CON_READ_IMOTD;
		}
		else
		{
			do_help(ch, "motd");
			d->connected = CON_READ_MOTD;
		}
		break;

	case CON_BREAK_CONNECT:
		switch (*argument)
		{
		case 'y':
		case 'Y':
			for (d_old = descriptor_list; d_old != NULL; d_old = d_next)
			{
				d_next = d_old->next;
				if (d_old == d || d_old->character == NULL)
					continue;

				if (str_cmp((ch->original_name ? ch->original_name : ch->name), d_old->original ? d_old->original->name : (d_old->character->original_name ? d_old->character->original_name : d_old->character->name)))
					continue;

				close_socket(d_old);
			}
			if (check_reconnect(d, ch->name, TRUE))
				return;
			write_to_buffer(d, "Reconnect attempt failed.\n\rName: ", 0);
			if (d->character != NULL)
			{
				free_char(d->character);
				d->character = NULL;
			}
			d->connected = CON_GET_NAME;
			break;

		case 'n':
		case 'N':
			write_to_buffer(d, "Name: ", 0);
			if (d->character != NULL)
			{
				free_char(d->character);
				d->character = NULL;
			}
			d->connected = CON_GET_NAME;
			break;

		default:
			write_to_buffer(d, "Please type Y or N? ", 0);
			break;
		}
		break;

	case CON_CONFIRM_NEW_NAME:
		switch (*argument)
		{
		case 'y':
		case 'Y':
			sprintf(buf, "New character.\n\rGive me a password for %s: %s", ch->name, echo_off_str);
			write_to_buffer(d, buf, 0);
			log_string("wrote to buffer.");
			d->connected = CON_GET_NEW_PASSWORD;
			log_string("State is now CON_GET_NEW_PASSWORD.");
			break;
		case 'n':
		case 'N':
			write_to_buffer(d, "Ok, what IS it, then? ", 0);
			free_char(d->character);
			d->character = NULL;
			d->connected = CON_GET_NAME;
			break;

		default:
			write_to_buffer(d, "Please type Yes or No? ", 0);
			break;
		}
		break;

	case CON_GET_NEW_PASSWORD:
		write_to_buffer(d, "\n\r", 2);

		if (strlen(argument) < 4)
		{
			write_to_buffer(d, "Password must be at least five characters long.\n\rPassword: ", 0);
			return;
		}

		free_string(ch->pcdata->pwd);
		ch->pcdata->pwd = str_dup(argument);
		write_to_buffer(d, "Please retype password: ", 0);
		d->connected = CON_CONFIRM_NEW_PASSWORD;
		break;

	case CON_CONFIRM_NEW_PASSWORD:
		write_to_buffer(d, "\n\r", 2);
		log_string("Got past first write buffer in CON_CONFIRM_NEW_PASSWORD");

		if (strcmp(argument, ch->pcdata->pwd))
		{
			write_to_buffer(d, "Passwords don't match.\n\rRetype password: ", 0);
			d->connected = CON_GET_NEW_PASSWORD;
			return;
		}
		else
		{
			log_string("Passwords match.");
		}

		log_string("Got past password match check");

		write_to_buffer(d, echo_on_str, 0);
		write_to_buffer(d, "\n\rChoose your race:\n\r", 0);
		col = 0;

		for (race = 1; race < MAX_PC_RACE; race++)
		{
			if (!race_table[race].pc_race)
				break;
			if (pc_race_table[race].set_race)
				continue;
			((pc_race_table[race].xpadd == 0) ? sprintf(buf, "     %-15s", pc_race_table[race].name) : sprintf(buf, "     %-15s", pc_race_table[race].name));

			write_to_buffer(d, buf, 0);
			if (++col == 2)
			{
				write_to_buffer(d, "\n\r", 0);
				col = 0;
			}
			else
				write_to_buffer(d, "   ", 0);
		}

		write_to_buffer(d, "\n\r", 0);
		race = race_lookup(argument);

		write_to_buffer(d, "What is your race (type 'help' for more information)? ", 0);
		d->connected = CON_GET_NEW_RACE;
		break;

	case CON_GET_NEW_RACE:
		one_argument(argument, arg);

		if (!strcmp(arg, "help"))
		{
			argument = one_argument(argument, arg);
			if (argument[0] == '\0')
				do_help(ch, "race");
			else
				do_help(ch, argument);
			write_to_buffer(d, "\n\rWhat is your race (type 'help' for more information)?", 0);
			break;
		}
		race = race_lookup(argument);

		if (race == 0 || !race_table[race].pc_race || pc_race_table[race].set_race)
		{
			write_to_buffer(d, "\n\rThat is not a valid race. Choose one of the following:\n\r", 0);

			col = 0;
			for (race = 1; race < MAX_PC_RACE; race++)
			{
				if (!race_table[race].pc_race)
					break;
				if (pc_race_table[race].set_race)
					continue;

				((pc_race_table[race].xpadd == 0) ? sprintf(buf, "     %-15s", pc_race_table[race].name) : sprintf(buf, "     %-15s", pc_race_table[race].name));

				write_to_buffer(d, buf, 0);

				if (++col == 2)
				{
					write_to_buffer(d, "\n\r", 0);
					col = 0;
				}
				else
					write_to_buffer(d, "   ", 0);
			}

			write_to_buffer(d, "\n\rWhat is your race? (type 'help' for more information)", 0);
			break;
		}
		ch->race = race;

		for (i = 0; i < MAX_STATS; i++)
			ch->perm_stat[i] = pc_race_table[race].stats[i];
		ch->race = race;
		ch->affected_by = ch->affected_by | race_table[race].aff;
		ch->imm_flags = ch->imm_flags | race_table[race].imm;
		ch->res_flags = ch->res_flags | race_table[race].res;
		ch->vuln_flags = ch->vuln_flags | race_table[race].vuln;
		ch->form = race_table[race].form;
		ch->parts = race_table[race].parts;

		/* add skills */
		for (i = 0; i < 5; i++)
		{
			if (pc_race_table[race].skills[i] == NULL)
				break;
			sn = skill_lookup(pc_race_table[race].skills[i]);
			ch->pcdata->learned[sn] = 100;
		}

		ch->size = pc_race_table[race].size;

		write_to_buffer(d, "\n\rWhat is your sex (M/F)? ", 0);
		d->connected = CON_GET_NEW_SEX;
		break;

	case CON_GET_NEW_SEX:
		switch (argument[0])
		{
		case 'm':
		case 'M':
			ch->sex = SEX_MALE;
			ch->pcdata->true_sex = SEX_MALE;
			break;
		case 'f':
		case 'F':
			ch->sex = SEX_FEMALE;
			ch->pcdata->true_sex = SEX_FEMALE;
			break;
		default:
			write_to_buffer(d, "That's not a sex.\n\rWhat IS your sex? ", 0);
			return;
		}

		write_to_buffer(d, "\n\rThe following classes are available:\n\r", 0);

		col = 0;
		for (race = 0; race < MAX_CLASS; race++)
		{
			if (race == class_lookup("bard") || race == class_lookup("druid"))
				continue;
			((class_table[race].xpadd == 0) ? sprintf(buf, "     %-15s", class_table[race].name) : sprintf(buf, "     %-15s", class_table[race].name));

			write_to_buffer(d, buf, 0);
			if (++col == 2)
			{
				write_to_buffer(d, "\n\r", 0);
				col = 0;
			}
			else
			{
				write_to_buffer(d, "   ", 0);
			}
		}
		buf[0] = '\0';
		write_to_buffer(d, "\n\rYour race may be one of these classes:\n\r", 0);

		for (iClass = 0; iClass < MAX_CLASS; iClass++)
		{
			if (pc_race_table[ch->race].classes[iClass] == 1)
			{
				strcat(buf, class_table[iClass].name);
				strcat(buf, " ");
			}
		}
		strcat(buf, "\n\r");
		write_to_buffer(d, buf, 0);
		write_to_buffer(d, "Choose your class (type 'help' for more infomation): ", 0);
		d->connected = CON_GET_NEW_CLASS;
		break;

	case CON_GET_NEW_CLASS:
		one_argument(argument, arg);

		if (!strcmp(arg, "help"))
		{
			argument = one_argument(argument, arg); // first argument is 'help'
			argument = one_argument(argument, arg); // second is ...

			if (arg[0] == '\0')
				do_help(ch, "class");
			else
				do_help(ch, arg);

			write_to_buffer(d, "\n\rChoose your class (type 'help' for more information): ", 0);
			break;
		}

		iClass = class_lookup(argument);

		if (iClass == -1)
		{
			write_to_buffer(d, "That's not a class.\n\rChoose your class (type 'help' for more information): ", 0);
			return;
		}

		if (pc_race_table[ch->race].classes[iClass] != 1)
		{
			strcpy(buf, "Your race may only be one of these classes:\n\r");
			for (iClass = 0; iClass < MAX_CLASS; iClass++)
			{
				if (pc_race_table[ch->race].classes[iClass] == 1)
				{
					strcat(buf, class_table[iClass].name);
					strcat(buf, " ");
				}
			}
			strcat(buf, "\n\r");
			write_to_buffer(d, buf, 0);
			write_to_buffer(d, "What is your class ? (type 'help' for more infomation): ", 0);
			return;
		}

		ch->class = iClass;
		group_add(ch, "class basics", FALSE);
		ch->pcdata->learned[gsn_recall] = 100;
		for (type = 0; weapon_table[type].name != NULL; type++)
		{
			if (class_table[ch->class].weapon == weapon_table[type].vnum)
				ch->pcdata->learned[*weapon_table[type].gsn] = 40;
		}
		SET_BIT(ch->comm, COMM_SHOW_AFFECTS);

		sprintf(log_buf, "%s@%s new player.%s", ch->name, d->host,
				auto_check_multi(d, d->host) ? " (MULTI-CHAR?)" : "");
		log_string(log_buf);
		login_log(ch, LTYPE_NEW);
		wiznet("Newbie alert!  $N sighted.", ch, NULL, WIZ_NEWBIE, 0, 0);
		wiznet(log_buf, NULL, NULL, WIZ_SITES, 0, get_trust(ch));

		write_to_buffer(d, "[Hit Return to Continue]\n\r", 0);
		d->connected = CON_ROLLING_STATS;
		break;

	case CON_ROLLING_STATS:
		ch->perm_stat[STAT_STR] = pc_race_table[ch->race].max_stats[STAT_STR];
		ch->perm_stat[STAT_INT] = pc_race_table[ch->race].max_stats[STAT_INT];
		ch->perm_stat[STAT_WIS] = pc_race_table[ch->race].max_stats[STAT_WIS];
		ch->perm_stat[STAT_DEX] = pc_race_table[ch->race].max_stats[STAT_DEX];
		ch->perm_stat[STAT_CON] = pc_race_table[ch->race].max_stats[STAT_CON];

		ch->mod_stat[STAT_STR] = 0;
		ch->mod_stat[STAT_INT] = 0;
		ch->mod_stat[STAT_WIS] = 0;
		ch->mod_stat[STAT_DEX] = 0;
		ch->mod_stat[STAT_CON] = 0;

		sprintf(buf,
				"Your stats are Str: %2d Int: %2d Wis: %2d Dex: %2d Con: %2d\n\r",
				ch->perm_stat[STAT_STR],
				ch->perm_stat[STAT_INT],
				ch->perm_stat[STAT_WIS],
				ch->perm_stat[STAT_DEX],
				ch->perm_stat[STAT_CON]);
		send_to_char(buf, ch);

		if (pc_race_table[ch->race].align == ALIGN_ANY && class_table[ch->class].align == ALIGN_ANY)
		{
			write_to_buffer(d, "\n\r", 2);
			write_to_buffer(d, "You may be good, neutral, or evil.\n\r", 0);
			write_to_buffer(d, "Which alignment (G/N/E)? ", 0);
		}
		else if ((pc_race_table[ch->race].align == ALIGN_GE && class_table[ch->class].align == ALIGN_GE) || (pc_race_table[ch->race].align == ALIGN_ANY && class_table[ch->class].align == ALIGN_GE) || (pc_race_table[ch->race].align == ALIGN_GE && class_table[ch->class].align == ALIGN_ANY))
		{
			write_to_buffer(d, "\n\r", 2);
			write_to_buffer(d, "You may be good or evil.\n\r", 0);
			write_to_buffer(d, "Which alignment (G/E)? ", 0);
		}
		else if ((pc_race_table[ch->race].align == ALIGN_GN && class_table[ch->class].align == ALIGN_GN) || (pc_race_table[ch->race].align == ALIGN_ANY && class_table[ch->class].align == ALIGN_GN) || (pc_race_table[ch->race].align == ALIGN_GN && class_table[ch->class].align == ALIGN_ANY))
		{
			write_to_buffer(d, "\n\r", 2);
			write_to_buffer(d, "You may be good or neutral.\n\r", 0);
			write_to_buffer(d, "Which alignment (G/N)? ", 0);
		}
		else if ((pc_race_table[ch->race].align == ALIGN_NE && class_table[ch->class].align == ALIGN_NE) || (pc_race_table[ch->race].align == ALIGN_ANY && class_table[ch->class].align == ALIGN_NE) || (pc_race_table[ch->race].align == ALIGN_NE && class_table[ch->class].align == ALIGN_ANY))
		{
			write_to_buffer(d, "\n\r", 2);
			write_to_buffer(d, "You may be neutral or evil.\n\r", 0);
			write_to_buffer(d, "Which alignment (N/E)? ", 0);
		}
		else
		{
			write_to_buffer(d, "[Hit Return to Continue]", 0);
		}
		d->connected = CON_GET_ALIGNMENT;
		break;

	case CON_GET_ALIGNMENT:
		if (pc_race_table[ch->race].align == ALIGN_ANY && class_table[ch->class].align == ALIGN_ANY)
		{
			switch (argument[0])
			{
			case 'g':
			case 'G':
				ch->alignment = 1000;
				break;
			case 'n':
			case 'N':
				ch->alignment = 0;
				break;
			case 'e':
			case 'E':
				ch->alignment = -1000;
				break;
			default:
				write_to_buffer(d, "That's not a valid alignment.\n\r", 0);
				write_to_buffer(d, "Which alignment (G/N/E)? ", 0);
				return;
			}
		}
		else if ((pc_race_table[ch->race].align == ALIGN_GE && class_table[ch->class].align == ALIGN_GE) || (pc_race_table[ch->race].align == ALIGN_ANY && class_table[ch->class].align == ALIGN_GE) || (pc_race_table[ch->race].align == ALIGN_GE && class_table[ch->class].align == ALIGN_ANY))
		{
			switch (argument[0])
			{
			case 'g':
			case 'G':
				ch->alignment = 1000;
				break;
			case 'e':
			case 'E':
				ch->alignment = -1000;
				break;
			default:
				write_to_buffer(d, "That's not a valid alignment.\n\r", 0);
				write_to_buffer(d, "Which alignment (G/E)? ", 0);
				return;
			}
		}
		else if ((pc_race_table[ch->race].align == ALIGN_GN && class_table[ch->class].align == ALIGN_GN) || (pc_race_table[ch->race].align == ALIGN_ANY && class_table[ch->class].align == ALIGN_GN) || (pc_race_table[ch->race].align == ALIGN_GN && class_table[ch->class].align == ALIGN_ANY))
		{
			switch (argument[0])
			{
			case 'g':
			case 'G':
				ch->alignment = 1000;
				break;
			case 'n':
			case 'N':
				ch->alignment = 0;
				break;
			default:
				write_to_buffer(d, "That's not a valid alignment.\n\r", 0);
				write_to_buffer(d, "Which alignment (G/N)? ", 0);
				return;
			}
		}
		else if ((pc_race_table[ch->race].align == ALIGN_NE && class_table[ch->class].align == ALIGN_NE) || (pc_race_table[ch->race].align == ALIGN_ANY && class_table[ch->class].align == ALIGN_NE) || (pc_race_table[ch->race].align == ALIGN_NE && class_table[ch->class].align == ALIGN_ANY))
		{
			switch (argument[0])
			{
			case 'n':
			case 'N':
				ch->alignment = 0;
				break;
			case 'e':
			case 'E':
				ch->alignment = -1000;
				break;
			default:
				write_to_buffer(d, "That's not a valid alignment.\n\r", 0);
				write_to_buffer(d, "Which alignment (N/E)? ", 0);
				return;
			}
		}
		else if (pc_race_table[ch->race].align == ALIGN_G || class_table[ch->class].align == ALIGN_G)
		{
			write_to_buffer(d, "\n\rYou must be of good alignment.\n\r", 0);
			ch->alignment = 1000;
		}
		else if (pc_race_table[ch->race].align == ALIGN_N || class_table[ch->class].align == ALIGN_N || (pc_race_table[ch->race].align == ALIGN_GN && class_table[ch->class].align == ALIGN_NE) || (pc_race_table[ch->race].align == ALIGN_NE && class_table[ch->class].align == ALIGN_GN))
		{
			write_to_buffer(d, "\n\rYou must be of neutral alignment.\n\r", 0);
			ch->alignment = 0;
		}
		else if (pc_race_table[ch->race].align == ALIGN_E || class_table[ch->class].align == ALIGN_E)
		{
			write_to_buffer(d, "\n\rYou must be of evil alignment.\n\r", 0);
			ch->alignment = -1000;
		}

		if (ch->class != 3 && ch->class != 4 && ch->class != 6)
		{
			write_to_buffer(d, "\n\r", 2);
			write_to_buffer(d, "You may be lawful, neutral, or chaotic.\n\r", 0);
			write_to_buffer(d, "Which ethos (L/N/C)? ", 0);
		}
		else
		{
			write_to_buffer(d, "\n\r", 0);
			write_to_buffer(d, "[Hit Return to Continue]", 0);
		}
		d->connected = CON_GET_ETHOS;
		break;

	case CON_GET_ETHOS:
		if (ch->class == class_lookup("paladin"))
		{
			write_to_buffer(d, "\n\rYou must be lawful.\n\r", 0);
			ch->pcdata->ethos = 1000;
		}
		else
		{
			switch (argument[0])
			{
			case 'l':
			case 'L':
				ch->pcdata->ethos = 1000;
				break;
			case 'n':
			case 'N':
				ch->pcdata->ethos = 0;
				break;
			case 'c':
			case 'C':
				ch->pcdata->ethos = -1000;
				break;
			default:
				write_to_buffer(d, "That's not a valid ethos.\n\r", 0);
				write_to_buffer(d, "Which ethos (L/N/C)? ", 0);
				return;
			}
		}

		write_to_buffer(d, "The following cabals are available to you based on your race, class, alignment, and ethos.\n\r\n\r", 0);
		write_to_buffer(d, "Cabal                           Name       Align  Ethos  Active Chars  Limit\n\r", 0);
		for (iCabal = 1; iCabal < MAX_CABAL; iCabal++)
		{
			if (!can_join_cabal(ch, iCabal))
				continue;

			sprintf(buf, "%-30s  %-10s %-6s %-6s %-2d            %-2d\n\r",
					cabal_table[iCabal].long_name,
					cabal_table[iCabal].name,
					get_align_restr_name(cabal_restr_table[iCabal].acc_align),
					get_ethos_restr_name(cabal_restr_table[iCabal].acc_ethos),
					cabal_members[iCabal],
					cabal_table[iCabal].max_members == 1 ? 0 : cabal_table[iCabal].max_members);
			write_to_buffer(d, buf, 0);
		}

		write_to_buffer(d, "Type 'none' to be uncaballed.\n\r", 0);
		write_to_buffer(d, "Which cabal do you wish to join? ", 0);

		d->connected = CON_GET_CABAL;
		break;

	case CON_GET_CABAL:
		if (argument[0] == '\0')
		{
			write_to_buffer(d, "Which cabal do you wish to join? ", 0);
			return;
		}
		if (cabal_lookup(argument) == 0 || can_join_cabal(ch, cabal_lookup(argument)))
		{
			ch->cabal = cabal_lookup(argument);
		}
		else
		{
			write_to_buffer(d, "That cabal is either closed, full, or incompatible with your character. Please select a valid cabal or 'none'.\n\r", 0);
			write_to_buffer(d, "Which cabal do you wish to join? ", 0);
			return;
		}

		sprintf(buf, "You have been inducted into the %s.\n\r", cabal_table[ch->cabal].long_name);
		write_to_buffer(d, buf, 0);

		sprintf(buf, "AUTO: Inducted into %s by Nanny.\n\r", capitalize(cabal_table[ch->cabal].name));
		add_history(NULL, ch, buf);

		group_add(ch, cabal_table[ch->cabal].name, FALSE);
		cabal_members[ch->cabal]++;

		int gn = group_lookup(cabal_table[ch->cabal].name);
		int gns;

		for (gns = 0; gns < MAX_SKILL; gns++)
		{
			if (gn < 1 || group_table[gn].spells[gns] == NULL)
				break;
			sn = skill_lookup(group_table[gn].spells[gns]);
			if (sn > 0 && skill_table[sn].skill_level[ch->class] < LEVEL_HERO + 1)
				ch->pcdata->learned[sn] = 70;
		}

		if (ch->cabal == CABAL_KNIGHT)
			set_extitle(ch, ", Knight of Thera");
		if (ch->cabal == CABAL_BOUNTY)
			set_extitle(ch, ", Neophyte Hunter");
		if (ch->cabal == CABAL_EMPIRE)
		{
			ch->pcdata->empire = EMPIRE_CITIZEN;
			set_extitle(ch, ", Citizen of the Empire");
		}

		sprintf(buf, "\n\rYour hometown can be: ");
		for (hometown = 0; hometown < MAX_HOMETOWN; hometown++)
		{
			if (can_live_in(ch, hometown))
			{
				strcat(buf, hometown_table[hometown].name);
				strcat(buf, ", ");
			}
		}
		chop(buf);
		chop(buf);
		strcat(buf, ".\n\rWhich hometown will you begin in? ");
		write_to_buffer(d, buf, 0);

		d->connected = CON_DEFAULT_CHOICE;
		break;

	case CON_DEFAULT_CHOICE:
		if (hometown_lookup(argument) && (can_live_in(ch, hometown_lookup(argument))))
		{
			ch->hometown = hometown_lookup(argument);
		}
		else
		{
			sprintf(buf, "Invalid selection. Your hometown can be: ");
			for (hometown = 0; hometown < MAX_HOMETOWN; hometown++)
			{
				if (can_live_in(ch, hometown))
				{
					strcat(buf, hometown_table[hometown].name);
					strcat(buf, ", ");
				}
			}
			chop(buf);
			chop(buf);
			strcat(buf, ".\n\rChoose your hometown? ");
			write_to_buffer(d, buf, 0);
			return;
		}
		write_to_buffer(d, "\n\r", 2);
		d->connected = CON_NEW_CHAR;

	case CON_NEW_CHAR:
		do_help(ch, "motd");
		d->connected = CON_READ_MOTD;
		break;

	case CON_GEN_GROUPS:
		send_to_char("\n\r", ch);
		do_help(ch, "menu choice");
		break;

	case CON_READ_IMOTD:
		write_to_buffer(d, "\n\r", 2);
		do_help(ch, "motd");
		d->connected = CON_READ_MOTD;
		break;

	case CON_READ_MOTD:
		if (ch->pcdata == NULL || ch->pcdata->pwd[0] == '\0')
		{
			write_to_buffer(d, "Warning! Null password!\n\r", 0);
			write_to_buffer(d, "Please report old password with bug.\n\r", 0);
			write_to_buffer(d, "Type 'password null <new password>' to fix.\n\r", 0);
		}

		write_to_buffer(d, "\n\rWelcome to Kill or be Killed. Let the bloodshed begin!\n\r\n\r", 0);

		if (is_affected(ch, gsn_rot) && ch != NULL)
		{
			AFFECT_DATA af;
			AFFECT_DATA *paf;
			AFFECT_DATA *paf_next;
			int affectduration = 0, affectmodifier = 0, affectlevel = 0;

			for (paf = ch->affected; paf != NULL; paf = paf_next)
			{
				paf_next = paf->next;
				if (paf->type == gsn_rot)
				{
					affectduration = paf->duration;
					affectmodifier = paf->modifier;
					affectlevel = paf->level;
					affect_strip(ch, gsn_rot);

					init_affect(&af);
					af.where = TO_AFFECTS;
					af.aftype = AFT_MALADY;
					af.type = gsn_rot;
					af.level = affectlevel;
					af.duration = affectduration;
					af.location = APPLY_CON;
					af.modifier = affectmodifier;
					af.owner_name = str_dup(ch->original_name);
					af.bitvector = 0;
					affect_to_char(ch, &af);
				}
			}
		}

		ch->next = char_list;
		char_list = ch;
		d->connected = CON_PLAYING;

		reset_char(ch);
		free_string(ch->pcdata->logon_time);
		ch->pcdata->logon_time = str_dup(ctime(&current_time));
		ch->pcdata->logon_time[strlen(ch->pcdata->logon_time) - 1] = '\0';
		do_unread(ch, "");
		loadCharmed(ch);
		updatePlayerAuth(ch);
		updatePlayerDb(ch);
		if (ch->cloaked)
		{
			AFFECT_DATA af;
			char cloakbuf[MSL];
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.aftype = AFT_POWER;
			af.type = gsn_cloak_form;
			af.level = ch->level;
			af.location = APPLY_NONE;
			af.modifier = 0;
			af.bitvector = AFF_SNEAK;
			af.duration = -1;
			af.affect_list_msg = str_dup("grants silent movement and a shrouded disguise");
			affect_to_char(ch, &af);
			af.affect_list_msg = NULL;
			af.location = APPLY_HIT;
			af.modifier = ch->level * 5;
			affect_to_char(ch, &af);
			af.location = APPLY_MOVE;
			af.modifier = ch->level * 2;
			affect_to_char(ch, &af);
			af.location = APPLY_MANA;
			af.modifier = ch->level * 2;
			affect_to_char(ch, &af);
			af.location = APPLY_DAMROLL;
			af.modifier = 5;
			affect_to_char(ch, &af);
			af.location = APPLY_HITROLL;
			af.modifier = 5;
			affect_to_char(ch, &af);
			send_to_char("You cloak your presence.\n\r", ch);
			free_string(ch->name);
			sprintf(cloakbuf, "cloaked figure");
			ch->name = str_dup(cloakbuf);
			ch->cloaked = 1;
			free_string(cloakbuf);
		}

		if (ch->level == 0)
		{
			/*
			Let's set up the basic auto-actions players usually set on logging in
			*/
			SET_BIT(ch->act, PLR_AUTOASSIST);
			SET_BIT(ch->act, PLR_AUTOEXIT);
			SET_BIT(ch->act, PLR_AUTOGOLD);
			SET_BIT(ch->act, PLR_COLOR);

			send_to_char("Autoassist, autoexit and autogold have been set.\n\r", ch);
			send_to_char("Type 'newbie off' to remove your NEWBIE PROTECTION.\n\r", ch);

			ch->level = 51;
			ch->exp = 0;
			ch->max_hit = ch->perm_stat[STAT_CON] * 100 + 250;
			ch->hit = ch->max_hit;
			ch->pcdata->perm_hit = ch->max_hit;
			ch->max_mana = ch->perm_stat[STAT_INT] * 50 + 250;
			ch->pcdata->perm_mana = ch->max_mana;
			ch->max_move = ch->perm_stat[STAT_DEX] * 50 + 150;
			ch->mana = ch->max_mana;
			ch->move = ch->max_move;
			ch->train = 30;
			ch->practice = 150;
			ch->exp = 550000;
			ch->ghost = 24;
			ch->pcdata->shifted = -1;
			ch->pcdata->newbie = TRUE;
			ch->original_name = str_dup(ch->name);
			ch->pcdata->roll_time = current_time;
			ch->gold = 5000;
			ch->base_enhancedDamMod = 100;
			ch->base_regen = 0;
			ch->regen_rate = 0;
			ch->base_numAttacks = 0;
			ch->base_dam_mod = 100;
			ch->enhancedDamMod = 100;
			ch->base_numAttacks = 0;
			ch->dam_mod = 100;
			ch->pcdata->dedication = 0;

			if (ch->class == CLASS_WARRIOR)
				ch->pcdata->special = 2;
			else
				ch->pcdata->special = 0;

			sprintf(buf,
					"the %s",
					title_table[ch->class][ch->level]
							   [ch->sex == SEX_FEMALE ? 1 : 0]);
			set_title(ch, buf);
			char_to_room(ch, get_room_index(TEMPLE_NEWBIE));
			send_to_char("\n\r", ch);
		}
		else if (ch->in_room != NULL)
		{
			char_to_room(ch, ch->in_room);
		}
		else if (IS_IMMORTAL(ch))
		{
			char_to_room(ch, get_room_index(ROOM_VNUM_CHAT));
		}
		else
		{
			char_to_room(ch, get_room_index(ROOM_VNUM_TEMPLE));
		}
		ch->pcdata->host = str_dup(ch->desc->host);
		log_string("Managed to get logged in..");
		act("$n has entered the game.", ch, NULL, NULL, TO_ROOM);

		group_add(ch, "class basics", FALSE);

		/* Debug age_mod on backward compatible pfiles...(Ceran) */
		if (ch->pcdata->age_mod == 0)
			get_age_mod(ch);

		/* Another real dodgy fix here...this time it has been left in. Problem in
		the dehydration and starving code if these lines aren't left in here...
		(Ceran)
		*/
		dh = ch->pcdata->condition[COND_DEHYDRATED];

		if (ch->pcdata->condition[COND_DEHYDRATED] > 80)
			ch->pcdata->condition[COND_DEHYDRATED] = 0;

		/* Set race specific abilities now...(Ceran) */
		if (ch->class != CLASS_WARRIOR)
		{
			sn = skill_lookup("staves");
			if (ch->pcdata->learned[sn] < 2)
				ch->pcdata->learned[sn] = 1;
			sn = skill_lookup("wands");
			if (ch->pcdata->learned[sn] < 2)
				ch->pcdata->learned[sn] = 1;
		}

		race = ch->race;

		/* This is messy...assigns each race special skills...(Ceran) */
		for (i = 0; i < 5; i++)
		{
			if (pc_race_table[race].skills[i] == NULL)
				break;
			sn = skill_lookup(pc_race_table[race].skills[i]);
			if ((sn == gsn_forest_blending || sn == gsn_awareness || sn == gsn_breath_fire || sn == skill_lookup("globe of darkness") || sn == skill_lookup("psionic blast") || sn == gsn_door_bash) && ch->pcdata->learned[sn] == 0)
			{
				ch->pcdata->learned[sn] = 75;
			}
			else if (sn == gsn_regeneration && ch->pcdata->learned[sn] <= 3)
			{
				ch->pcdata->learned[sn] = 50;
			}
			else if (sn == gsn_tertiary_wield && ch->pcdata->learned[sn] == 0)
			{
				ch->pcdata->learned[sn] = 75;
			}
			else
			{
				ch->pcdata->learned[sn] = 100;
			}
		}

		ch->pcdata->condition[COND_DEHYDRATED] = dh;

		/* Destroy all cabal items...make sure players don't relog with items, can
		occur if mud crashes, etc. Don't want cabal items being brought in while
		another one exists.
		*/
		OBJ_DATA *obj;
		OBJ_DATA *obj_next;

		for (obj = object_list; obj != NULL; obj = obj_next)
		{
			obj_next = obj->next;
			if (obj->carried_by == ch)
			{
				if (isCabalItem(obj))
				{
					extract_obj(obj);
				}
				else if (obj->pIndexData->limtotal != 0 && ch->level < 10)
					extract_obj(obj);
			}
		}
		do_look(ch, "auto");

		wiznet("$N has left real life behind.", ch, NULL,
			   WIZ_LOGINS, WIZ_SITES, get_trust(ch));

		if (ch->cabal != 0)
			announce_login(ch);

		/* Temporary for WizLock and NewLock Stats
		 * to be removed when open mud.
		 */
		if (IS_IMMORTAL(ch))
		{
			if (IS_SET(ch->wiznet, WIZ_SECURE))
			{
				REMOVE_BIT(ch->wiznet, WIZ_SECURE);
				do_lockstat(ch, "");
				SET_BIT(ch->wiznet, WIZ_SECURE);
			}
		}
		break;
	}
	return;
}
bool check_parse_name(char *name)
{
	log_string("Entered check_parse_name.");
	char *pc;
	int first = TRUE;

	if (is_name(name, "all auto immortal self zzz someone something the you demise balance circle loner honor outlaw rager arcana knight life enforcer ancient empire sylvan builder"))
		return FALSE;

	/* Length restrictions. */

	if (strlen(name) < 3)
		return FALSE;

	if (strlen(name) > 12)
		return FALSE;

	/* Alphanumerics only. */
	for (pc = name; *pc != '\0'; pc++)
	{
		if (!isalpha(*pc))
			return FALSE;
		if (isupper(*pc) && !first)
			*pc = LOWER(*pc);
		first = FALSE;
	}

	/* Prevent players from naming themselves after mobs.*/
	{
		extern MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
		MOB_INDEX_DATA *pMobIndex;
		int iHash;

		for (iHash = 0; iHash < MAX_KEY_HASH; iHash++)
		{
			for (pMobIndex = mob_index_hash[iHash]; pMobIndex != NULL; pMobIndex = pMobIndex->next)
			{
				if (is_name(name, pMobIndex->player_name))
					return FALSE;
			}
		}
	}

	if (descriptor_list)
	{
		int count = 0;
		DESCRIPTOR_DATA *d, *dnext;

		for (d = descriptor_list; d != NULL; d = dnext)
		{
			dnext = d->next;
			if (d->connected != CON_PLAYING && d->character && d->character->name && d->character->name[0] && !str_cmp(d->character->name, name))
			{
				count++;
				close_socket(d);
			}
		}
		if (count)
		{
			sprintf(log_buf, "Double newbie alert (%s)", name);
			wiznet(log_buf, NULL, NULL, WIZ_LOGINS, 0, 0);
			return FALSE;
		}
	}

	return TRUE;
}
bool check_reconnect(DESCRIPTOR_DATA *d, char *name, bool fConn)
{
	log_string("Entered check_reconnect.");
	CHAR_DATA *ch;
	OBJ_DATA *obj;

	for (ch = char_list; ch != NULL; ch = ch->next)
	{
		if (!IS_NPC(ch) && (!fConn || ch->desc == NULL) && !str_cmp((d->character->original_name ? d->character->original_name : d->character->name), (ch->original_name ? ch->original_name : ch->name)))
		{
			if (fConn == FALSE)
			{
				free_string(d->character->pcdata->pwd);
				d->character->pcdata->pwd = str_dup(ch->pcdata->pwd);
			}
			else
			{
				free_char(d->character);
				d->character = ch;
				ch->desc = d;
				ch->timer = 0;
				send_to_char("Reconnecting. Type replay to see missed tells.\n\r", ch);
				act("$n has reconnected.", ch, NULL, NULL, TO_ROOM);
				ch->pcdata->host = str_dup(ch->desc->host);
				/* Limit crap to balance reconnect objects from extracted link object */
				for (obj = ch->carrying; obj != NULL; obj = obj->next_content)
				{
					obj->pIndexData->limcount++;
				}

				sprintf(log_buf, "(%s) %s@%s reconnected.", timestamp(), ch->name, d->host);
				log_string(log_buf);
				free_string(log_buf);
				sprintf(log_buf, "(%s) $N recovers from link death.", timestamp());
				wiznet(log_buf, ch, NULL, WIZ_LINKS, 0, get_trust(ch));
				WAIT_STATE(ch, PULSE_VIOLENCE * 4);
				// Echo?
				d->connected = CON_PLAYING;
			}
			return TRUE;
		}
	}
	return FALSE;
}
bool check_playing(DESCRIPTOR_DATA *d, char *name)
{
	log_string("Entered check_playing.");
	DESCRIPTOR_DATA *dold;
	for (dold = descriptor_list; dold; dold = dold->next)
	{
		if (dold != d && dold->character != NULL && dold->connected != CON_GET_NAME && dold->connected != CON_GET_OLD_PASSWORD && !str_cmp(name, dold->original ? dold->original->name : (dold->character->original_name ? dold->character->original_name : dold->character->name)))
		{
			write_to_buffer(d, "That character is already playing.\n\r", 0);
			write_to_buffer(d, "Do you wish to connect anyway (Y/N)?", 0);
			d->connected = CON_BREAK_CONNECT;
			return TRUE;
		}
	}

	return FALSE;
}
void stop_idling(CHAR_DATA *ch)
{
	log_string("Entered stop_idling.");
	if (ch == NULL || ch->desc == NULL || ch->desc->connected != CON_PLAYING || ch->was_in_room == NULL || ch->in_room != get_room_index(ROOM_VNUM_LIMBO))
		return;

	ch->timer = 0;
	char_from_room(ch);
	char_to_room(ch, ch->was_in_room);
	ch->was_in_room = NULL;
	act("$n has returned from the void.", ch, NULL, NULL, TO_ROOM);
	return;
}
void send_to_char_bw(const char *txt, CHAR_DATA *ch)
{
	log_string("Entered send_to_char_bw.");
	if (txt != NULL && ch->desc != NULL)
		write_to_buffer(ch->desc, txt, strlen(txt));
	return;
}
void send_to_char(const char *txt, CHAR_DATA *ch)
{
	log_string("Entered send_to_char.");
	const char *point;
	char *point2;
	char buf[MAX_STRING_LENGTH * 4];
	int skip = 0;

	buf[0] = '\0';
	point2 = buf;
	if (txt && ch->desc)
	{
		if (IS_SET(ch->act, PLR_COLOR))
		{
			for (point = txt; *point; point++)
			{
				if (*point == '{')
				{
					point++;
					skip = color(*point, ch, point2);
					while (skip-- > 0)
						++point2;
					continue;
				}
				*point2 = *point;
				*++point2 = '\0';
			}
			*point2 = '\0';
			write_to_buffer(ch->desc, buf, point2 - buf);
		}
		else
		{
			for (point = txt; *point; point++)
			{
				if (*point == '{')
				{
					point++;
					continue;
				}
				*point2 = *point;
				*++point2 = '\0';
			}
			*point2 = '\0';
			write_to_buffer(ch->desc, buf, point2 - buf);
		}
	}
	return;
}
void send_to_chars(const char *txt, CHAR_DATA *ch, int min, ...)
{
	log_string("Entered send_to_chars.");
	if (txt != NULL && ch->desc != NULL)
		write_to_buffer(ch->desc, txt, strlen(txt));
	return;
}
void page_to_char_bw(const char *txt, CHAR_DATA *ch)
{
	log_string("Entered page_to_char_bw.");
	if (txt == NULL || ch->desc == NULL)
		return;

	if (ch->lines == 0)
	{
		send_to_char(txt, ch);
		return;
	}

#if defined(macintosh)
	send_to_char(txt, ch);
#else
	ch->desc->showstr_head = alloc_mem(strlen(txt) + 1);
	strcpy(ch->desc->showstr_head, txt);
	ch->desc->showstr_point = ch->desc->showstr_head;
	show_string(ch->desc, "");
#endif
}

void page_to_char(const char *txt, CHAR_DATA *ch)
{
	log_string("Entered page_to_char.");
	/*
	 * Page to one char, new color version, by Lope.
	 */
	const char *point;
	char *point2;
	char buf[MAX_STRING_LENGTH * 4];
	int skip = 0;

	buf[0] = '\0';
	point2 = buf;
	if (txt && ch->desc)
	{
		if (IS_SET(ch->act, PLR_COLOR))
		{
			for (point = txt; *point; point++)
			{
				if (*point == '{')
				{
					point++;
					skip = color(*point, ch, point2);
					while (skip-- > 0)
						++point2;
					continue;
				}
				*point2 = *point;
				*++point2 = '\0';
			}
			*point2 = '\0';
			ch->desc->showstr_head = alloc_mem(strlen(buf) + 1);
			strcpy(ch->desc->showstr_head, buf);
			ch->desc->showstr_point = ch->desc->showstr_head;
			show_string(ch->desc, "");
		}
		else
		{
			for (point = txt; *point; point++)
			{
				if (*point == '{')
				{
					point++;
					continue;
				}
				*point2 = *point;
				*++point2 = '\0';
			}
			*point2 = '\0';
			ch->desc->showstr_head = alloc_mem(strlen(buf) + 1);
			strcpy(ch->desc->showstr_head, buf);
			ch->desc->showstr_point = ch->desc->showstr_head;
			show_string(ch->desc, "");
		}
	}
	return;
}

/* string pager */
void show_string(struct descriptor_data *d, char *input)
{
	log_string("Entered show_string.");
	char buffer[4 * MAX_STRING_LENGTH];
	char buf[MAX_INPUT_LENGTH];
	register char *scan, *chk;
	int lines = 0, toggle = 1;
	int show_lines;

	one_argument(input, buf);
	if (buf[0] != '\0')
	{
		if (d->showstr_head)
		{
			free_mem(d->showstr_head, strlen(d->showstr_head));
			d->showstr_head = 0;
		}
		d->showstr_point = 0;
		return;
	}

	if (d->character)
		show_lines = d->character->lines;
	else
		show_lines = 0;

	for (scan = buffer;; scan++, d->showstr_point++)
	{
		if (((*scan = *d->showstr_point) == '\n' || *scan == '\r') && (toggle = -toggle) < 0)
			lines++;

		else if (!*scan || (show_lines > 0 && lines >= show_lines))
		{
			*scan = '\0';
			write_to_buffer(d, buffer, strlen(buffer));
			for (chk = d->showstr_point; isspace(*chk); chk++)
				;
			{
				if (!*chk)
				{
					if (d->showstr_head)
					{
						free_mem(d->showstr_head, strlen(d->showstr_head));
						d->showstr_head = 0;
					}
					d->showstr_point = 0;
				}
			}
			return;
		}
	}
	return;
}

/* quick sex fixer */
void fix_sex(CHAR_DATA *ch)
{
	log_string("Entered fix_sex.");
	if (ch->sex < 0 || ch->sex > 2)
		ch->sex = IS_NPC(ch) ? 0 : ch->pcdata->true_sex;
}

void act(const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type)
{
	log_string("Entered act.");
	/* to be compatible with older code */
	act_new(format, ch, arg1, arg2, type, POS_RESTING);
}

void act_new(const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type, int min_pos)
{
	log_string("Entered act_new.");
	CHAR_DATA *to;
	CHAR_DATA *vch = (CHAR_DATA *)arg2;
	OBJ_DATA *obj1 = (OBJ_DATA *)arg1;
	OBJ_DATA *obj2 = (OBJ_DATA *)arg2;
	const char *str;
	char *i = NULL;
	char *point;
	char *pbuff;
	char buffer[MAX_STRING_LENGTH * 2];
	char buf[MAX_STRING_LENGTH];
	char fname[MAX_INPUT_LENGTH];
	bool fColor = FALSE;

	/*
	 * Discard null and zero-length messages.
	 */
	if (!format || !*format)
		return;

	/* discard null rooms and chars */
	if (ch == NULL || ch->in_room == NULL)
		return;

	to = ch->in_room->people;
	if (type == TO_VICT)
	{
		if (!vch)
		{
			bug("Act: null vch with TO_VICT.", 0);
			n_logf("Act: null vch with TO_VICT where format=%s", format);
			return;
		}

		if (!vch->in_room)
			return;

		to = vch->in_room->people;
	}

	for (; to; to = to->next_in_room)
	{
		if ((!IS_NPC(to) && to->desc == NULL) // Maybe needs to be code for color?
			|| (IS_NPC(to) && !HAS_TRIGGER(to, TRIG_ACT)) || to->position < min_pos)
			continue;

		if ((type == TO_CHAR) && to != ch)
			continue;
		if (type == TO_VICT && (to != vch || to == ch))
			continue;
		if (type == TO_ROOM && to == ch)
			continue;
		if (type == TO_NOTVICT && (to == ch || to == vch))
			continue;
		if (type == TO_IMMINROOM && (!(IS_IMMORTAL(to)) || (to == ch) || !(can_see(to, ch))))
			continue;

		point = buf;
		str = format;
		while (*str != '\0')
		{
			if (*str != '$')
			{
				*point++ = *str++;
				continue;
			}
			fColor = TRUE;
			++str;
			i = " <@@@> ";
			if (!arg2 && *str >= 'A' && *str <= 'Z')

			{
				bug("Act: missing arg2 for code %d.", *str);
				i = " <@@@> ";
			}
			else
			{
				switch (*str)
				{
				default:
					bug("Act: bad code %d.", *str);
					i = " <@@@> ";
					break;
				/* Thx alex for 't' idea */
				case 't':
					i = (char *)arg1;
					break;
				case 'T':
					i = (char *)arg2;
					break;

				case 'n':
					if (ch && to)
						i = get_descr_form(ch, to, FALSE);
					else
						bug("Act: bad code $n for 'ch' or 'to'", 0);
					break;
				case 'N':
					if (vch && to)
						i = get_descr_form(vch, to, FALSE);
					else
						bug("Act: bad code $N for 'vch' or 'to'", 0);
					break;

				case 'f':
					i = (ch->original_name ? ch->original_name : ch->name);
					break;
				case 'F':
					i = (vch->original_name ? vch->original_name : vch->name);
					break;
				case 'e':
					i = he_she[URANGE(0, ch->sex, 2)];
					break;
				case 'E':
					i = he_she[URANGE(0, vch->sex, 2)];
					break;
				case 'm':
					i = him_her[URANGE(0, ch->sex, 2)];
					break;
				case 'M':
					i = him_her[URANGE(0, vch->sex, 2)];
					break;
				case 's':
					i = his_her[URANGE(0, ch->sex, 2)];
					break;
				case 'S':
					i = his_her[URANGE(0, vch->sex, 2)];
					break;

				case 'p':
					i = can_see_obj(to, obj1)
							? obj1->short_descr
							: "something";
					break;

				case 'P':
					i = can_see_obj(to, obj2)
							? obj2->short_descr
							: "something";
					break;

				case 'd':
					if (arg2 == NULL || ((char *)arg2)[0] == '\0')
					{
						i = "door";
					}
					else
					{
						one_argument((char *)arg2, fname);
						i = fname;
					}
					break;
				}
			}

			++str;
			while ((*point = *i) != '\0')
				++point, ++i;
		}

		*point++ = '\n';
		*point++ = '\r';
		*point = '\0';
		buf[0] = UPPER(buf[0]);
		pbuff = buffer;
		colorconv(pbuff, buf, to);
		if (to->desc != NULL)
			write_to_buffer(to->desc, buffer, 0);
	}

	return;
}

/*
 * Macintosh support functions.
 */
#if defined(macintosh)
int gofday(struct timeval *tp, void *tzp)
{
	log_string("Entered gofday.");
	tp->tv_sec = time(NULL);
	tp->tv_usec = 0;
}
#endif

void n_logf(char *fmt, ...)
{
	log_string("Entered n_logf.");
	char buf[2 * MSL];
	va_list args;
	va_start(args, fmt);
	log_string("Finished va_start.");
	vsprintf(buf, fmt, args);
	log_string("Finished vsprintf.");
	va_end(args);
	log_string("Finished va_end.");

	log_string(buf);
}

/* Messy hack for login announcements in cabals. */
void announce_login(CHAR_DATA *ch)
{
	log_string("Entered announce_logout.");
	CHAR_DATA *guardian;
	char *rstring, buf[MAX_STRING_LENGTH], rbuf[MAX_STRING_LENGTH];
	if (ch->cabal == 0 || ch->cabal > MAX_CABAL || IS_IMMORTAL(ch))
	{
		return;
	}
	guardian = get_cabal_guardian(ch->cabal);
	if (!guardian)
	{
		return;
	}
	rstring = str_dup(cabal_messages[ch->cabal].login);
	sprintf(buf, rstring, ch->original_name);
	sprintf(rbuf, "{c%s{x", buf);
	do_cb(guardian, buf);
}

void announce_logout(CHAR_DATA *ch)
{
	log_string("Entered announce_logout.");
	CHAR_DATA *guardian;
	char *rstring, buf[MAX_STRING_LENGTH], rbuf[MAX_STRING_LENGTH];
	if (ch->cabal == 0 || ch->cabal > MAX_CABAL || IS_IMMORTAL(ch))
	{
		return;
	}
	guardian = get_cabal_guardian(ch->cabal);
	if (!guardian)
	{
		return;
	}
	rstring = str_dup(cabal_messages[ch->cabal].logout);
	sprintf(buf, rstring, ch->original_name);
	sprintf(rbuf, "{c%s{x", buf);
	do_cb(guardian, buf);
}

/* Fun rename function to rename bad named chars on-line...(Ceran) */
void do_rename(CHAR_DATA *ch, char *argument)
{
	log_string("Entered do_rename.");
	char old_name[MAX_INPUT_LENGTH], new_name[MAX_INPUT_LENGTH], strsave[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	FILE *file;

#if defined(unix)
	argument = one_argument(argument, old_name); /* find new/old name */
	one_argument(argument, new_name);

	/* Trivial checks */
	if (!old_name[0])
	{
		send_to_char("Rename who?\n\r", ch);
		return;
	}

	victim = get_char_world(ch, old_name);

	if (!victim)
	{
		send_to_char("There is no such a person online.\n\r", ch);
		return;
	}

	if (IS_NPC(victim))
	{
		send_to_char("You cannot use Rename on NPCs.\n\r", ch);
		return;
	}

	/* allow rename "self new_name",but otherwise only lower level */
	if ((victim != ch) && (get_trust(victim) >= get_trust(ch)))
	{
		send_to_char("You failed.\n\r", ch);
		return;
	}

	if (!victim->desc || (victim->desc->connected != CON_PLAYING))
	{
		send_to_char("They are link-dead.\n\r", ch);
		return;
	}

	if (!new_name[0])
	{
		send_to_char("Rename to what new name?\n\r", ch);
		return;
	}

	if (!check_parse_name(new_name))
	{
		send_to_char("That new name is illegal..\n\r", ch);
		return;
	}

	/* First, check if the new name is already taken */
	sprintf(strsave, "%s%s.plr", PLAYER_DIR, capitalize(new_name));
	fclose(fpReserve);
	file = fopen(strsave, "r");
	if (file)
	{
		send_to_char("A player with that name already exists.\n\r", ch);
		fclose(file);
		fpReserve = fopen(NULL_FILE, "r");
		return;
	}

	fpReserve = fopen(NULL_FILE, "r");

	if (get_char_world(ch, new_name)) /* check for playing level-1 non-saved */
	{
		send_to_char("A player with the name you specified already exists.\n\r", ch);
		return;
	}
	/* Save char and then rename and move pfile */
	save_char_obj(victim);

	// set strsave before we lose what their original pfile is called
	sprintf(strsave, "%s%s.plr", PLAYER_DIR, capitalize(victim->original_name));

	free_string(victim->name);
	free_string(victim->original_name);
	victim->name = str_dup(capitalize(new_name));
	victim->original_name = str_dup(capitalize(new_name));

	save_char_obj(victim);
	remove(strsave);

	send_to_char("Character renamed.\n\r", ch);

	victim->position = POS_STANDING;
	act("$n has renamed you to $N!", ch, NULL, victim, TO_VICT);
#endif

	return;
}

void do_renam(CHAR_DATA *ch, char *argument)
{
	log_string("Entered do_renam.");
#if defined(unix)
	send_to_char("If you want to RENAME an existing player your must type rename in full.\n\r", ch);
	send_to_char("rename <current name> <new name>\n\r", ch);
#else
	send_to_char("Rename not supported on your platform.\n\r", ch);
#endif
	return;
}

int color(char type, CHAR_DATA *ch, char *string)
{
	log_string("Entered color.");
	char code[20];
	char *p = '\0';

	if (IS_NPC(ch))
		return (0);

	switch (type)
	{
	default:
		sprintf(code, CLEAR);
		break;
	case 'x':
		sprintf(code, CLEAR);
		break;
	case 'b':
		sprintf(code, C_BLUE);
		break;
	case 'c':
		sprintf(code, C_CYAN);
		break;
	case 'g':
		sprintf(code, C_GREEN);
		break;
	case 'm':
		sprintf(code, C_MAGENTA);
		break;
	case 'r':
		sprintf(code, C_RED);
		break;
	case 'w':
		sprintf(code, C_WHITE);
		break;
	case 'y':
		sprintf(code, C_YELLOW);
		break;
	case 'B':
		sprintf(code, C_B_BLUE);
		break;
	case 'C':
		sprintf(code, C_B_CYAN);
		break;
	case 'G':
		sprintf(code, C_B_GREEN);
		break;
	case 'M':
		sprintf(code, C_B_MAGENTA);
		break;
	case 'R':
		sprintf(code, C_B_RED);
		break;
	case 'W':
		sprintf(code, C_B_WHITE);
		break;
	case 'Y':
		sprintf(code, C_B_YELLOW);
		break;
	case 'D':
		sprintf(code, C_D_GREY);
		break;
	case '*':
		sprintf(code, "%c", 007);
		break;
	case '/':
		sprintf(code, "%c", 012);
		break;
	case '{':
		sprintf(code, "%c", '{');
		break;
	case '|':
		sprintf(code, "\n\r");
		break;
	case 'u':
		sprintf(code, C_UNDERLINE);
		break;
	case 'f':
		sprintf(code, C_FLASH);
		break;
	case 'v':
		sprintf(code, C_REVERSE);
		break;
	}

	p = code;
	while (*p != '\0')
	{
		*string = *p++;
		*++string = '\0';
	}

	return (strlen(code));
}

void colorconv(char *buffer, const char *txt, CHAR_DATA *ch)
{
	log_string("Entered colorconv.");
	const char *point;
	int skip = 0;

	if (ch->desc && txt)
	{
		if (IS_SET(ch->act, PLR_COLOR))
		{
			for (point = txt; *point; point++)
			{
				if (*point == '{')
				{
					point++;
					skip = color(*point, ch, buffer);
					while (skip-- > 0)
						++buffer;
					continue;
				}
				*buffer = *point;
				*++buffer = '\0';
			}
			*buffer = '\0';
		}
		else
		{
			for (point = txt; *point; point++)
			{
				if (*point == '{')
				{
					point++;
					continue;
				}
				*buffer = *point;
				*++buffer = '\0';
			}
			*buffer = '\0';
		}
	}
	return;
}

void process_text(CHAR_DATA *ch, char *text)
{
	log_string("Entered process_text.");
	char obuf[MSL * 2], tbuf[MSL], rbuf[MSL];
	bool found = FALSE;
	int len, rcount, i, iloc;
	if (!str_cmp(text, ""))
		return;
	if (!str_cmp(text, "exit"))
	{
		ch->pcdata->entering_text = FALSE;
		ch->pcdata->entered_text[0] = '\0';
		ch->pcdata->end_fun = NULL;
		send_to_char("Exiting without saving changes.\n\r", ch);
		return;
	}
	if (!str_cmp(text, "finish"))
	{
		send_to_char("Saving changes and exiting...\n\r", ch);
		if (ch->pcdata->end_fun)
			(*ch->pcdata->end_fun)(ch, ch->pcdata->entered_text);
		ch->pcdata->entering_text = FALSE;
		ch->pcdata->end_fun = NULL;
		return;
	}
	if (!str_cmp(text, "backline"))
	{
		if (ch->pcdata->entered_text == NULL || ch->pcdata->entered_text[0] == '\0')
			return send_to_char("No lines left to remove.", ch);
		strcpy(obuf, ch->pcdata->entered_text);
		for (len = strlen(obuf); len > 0; len--)
		{
			if (obuf[len] == '\r')
			{
				if (!found)
				{
					if (len > 0)
						len--;
					found = TRUE;
				}
				else
				{
					obuf[len + 1] = '\0';
					free_string(ch->pcdata->entered_text);
					ch->pcdata->entered_text = str_dup(obuf);
					return send_to_char("Line deleted.", ch);
				}
			}
		}
		obuf[0] = '\0';
		free_string(ch->pcdata->entered_text);
		ch->pcdata->entered_text = str_dup(obuf);
		send_to_char("Line deleted.", ch);
		return send_to_char(ch->pcdata->entered_text, ch);
	}
	if (!str_cmp(text, "show"))
		return send_to_char(ch->pcdata->entered_text, ch);
	if (text[0] != '\0')
	{
		obuf[0] = '\0';
		if (strlen(text) > 80)
		{
			sprintf(tbuf, "%s", text);
			for (i = 1; i <= strlen(text) / 80; i++)
			{
				for (rcount = 80 * i; rcount > (80 * i) - 15; rcount--)
					if (tbuf[rcount] == ' ')
						break;
				if (rcount <= (80 * i) - 15)
					rcount = 80 * i;
				for (iloc = rcount; tbuf[iloc + 1]; iloc++)
					rbuf[iloc - rcount] = tbuf[iloc + 1];
				rbuf[iloc - rcount] = '\0';
				tbuf[rcount] = '\n';
				tbuf[rcount + 1] = '\r';
				tbuf[rcount + 2] = '\0';
				strcat(tbuf, rbuf);
			}
			send_to_char("Over 80 characters, wrapping line.\n\r", ch);
		}
		if (ch->pcdata->entered_text)
			strcat(obuf, ch->pcdata->entered_text);
		if (strlen(text) < 80)
			strcat(obuf, text);
		else
			strcat(obuf, tbuf);
		strcat(obuf, "\n\r");
		if (strlen(obuf) >= MSL * 2)
			return send_to_char("String exceeds buffer.\n\r", ch);
		free_string(ch->pcdata->entered_text);
		ch->pcdata->entered_text = str_dup(obuf);
	}
	else if (text[0] == '\0')
		return send_to_char("Add what?", ch);
	return send_to_char("Line added.", ch);
}

const char *lowstring(const char *i)
{
	log_string("Entered lowstring.");
	static char buf[MAX_STRING_LENGTH];
	char *o;

	for (o = buf; *i && o - buf < sizeof(buf) - 1; i++, o++)
	{
		*o = LOWER(*i);
	}
	*o = '\0';
	return buf;
}

void reverse_txt(char *txt, int length)
{
	log_string("Entered reverse_txt.");
	int i, state, c;
	char *word_ptr = NULL;
#define WALKTHRU 0
#define INWORD 1
#define INESC 2
	/* walk thru line looking for words, ignore color codes */
	for (i = 0, state = WALKTHRU; i < length; i++, txt++)
	{
		c = *txt;
		switch (state)
		{
		case WALKTHRU:		/* just leave in place */
			if (isalnum(c)) /* start of word? */
			{
				word_ptr = txt;
				state = INWORD;
			}
			else if (c == '\033') /* start of color code escape seq? */
				state = INESC;
			break;
		case INESC:		  /* ignore contents of color code seq */
			if (c == 'm') /* 'm' marks end of code */
				state = WALKTHRU;
			break;

		case INWORD:										/* traversing a word */
			if (isalnum(c) || (c == '-' && isalnum(txt[1])) /* hypenated */
				|| (c == '\'' && isalnum(txt[1])))			/* contraction */
				break;										/* still in word */

			/* end of word, do the swap */
			reverse_word(word_ptr, txt - word_ptr);
			if (c == '\033')
				state = INESC;
			else
				state = WALKTHRU;
			word_ptr = NULL;
		}
	}
}

void reverse_word(char *w, int n)
{
	log_string("Entered reverse_word.");
	int i, hlen, left, right, up_left, up_right;
	hlen = n / 2;
	n--;
	for (i = 0; i < hlen; i++)
	{
		left = w[i];
		right = w[n - i];
		up_left = isupper(left) ? TRUE : FALSE;
		up_right = isupper(right) ? TRUE : FALSE;
		if (up_left != up_right)
		{
			if (isupper(left))
				left += 'a' - 'A';
			else
				left -= 'a' - 'A';
			if (isupper(right))
				right += 'a' - 'A';
			else
				right -= 'a' - 'A';
		}
		w[i] = right;
		w[n - i] = left;
	}
}

void printf_to_char(CHAR_DATA *ch, char *fmt, ...)
{
	log_string("Entered printf_to_char.");
	char buf[MAX_STRING_LENGTH];
	va_list args;
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	send_to_char(buf, ch);
}

void sprintf2(CHAR_DATA *ch, char *fmt, ...)
{
	log_string("Entered sprintf2.");
	char buf[MSL];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, MSL, fmt, args);
	va_end(args);
	send_to_char(buf, ch);
}

void bugf(char *fmt, ...)
{
	log_string("Entered bugf.");
	char buf[2 * MSL];
	va_list args;
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	bug(buf, 0);
}

void sendf(CHAR_DATA *ch, char *fmt, ...)
{
	log_string("Entered sendf.");
	char buf[MSL];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, MSL, fmt, args);
	va_end(args);
	send_to_char(buf, ch);
}

bool can_join_cabal(CHAR_DATA *ch, int cabal)
{
	log_string("Entered can_join_cabal.");
	int iAlign, iEthos;

	if (cabal == 0)
		return TRUE;

	if (ch->alignment < 0)
		iAlign = ALIGN_E;
	else if (ch->alignment == 0)
		iAlign = ALIGN_N;
	else
		iAlign = ALIGN_G;

	if (ch->pcdata->ethos < 0)
		iEthos = ETHOS_C;
	else if (ch->pcdata->ethos == 0)
		iEthos = ETHOS_N;
	else
		iEthos = ETHOS_L;

	if (!cabal_restr_table[cabal].acc_align & iAlign)
		return FALSE;

	if (!cabal_restr_table[cabal].acc_ethos & iEthos)
		return FALSE;

	if (cabal_restr_table[cabal].acc_class[ch->class] == 0)
		return FALSE;

	if (cabal_restr_table[cabal].acc_race[ch->race] == 0)
		return FALSE;

	if (!cabal_table[cabal].induct)
		return FALSE;

	if (cabal_members[cabal] >= cabal_table[cabal].max_members)
		return FALSE;

	return TRUE;
}

char *get_align_restr_name(int iAlign)
{
	log_string("Entered get_align_restr_name.");
	char buf[MSL];

	switch (iAlign)
	{
	case ALIGN_ANY:
		sprintf(buf, "Any");
		break;
	case ALIGN_GN:
		sprintf(buf, "G/N");
		break;
	case ALIGN_NE:
		sprintf(buf, "N/E");
		break;
	case ALIGN_GE:
		sprintf(buf, "G/E");
		break;
	case ALIGN_G:
		sprintf(buf, "G");
		break;
	case ALIGN_N:
		sprintf(buf, "N");
		break;
	case ALIGN_E:
		sprintf(buf, "E");
		break;
	default:
		sprintf(buf, "None");
		break;
	}

	return str_dup(buf);
}

char *get_ethos_restr_name(int iEthos)
{
	log_string("Entered get_ethos_restr_name.");
	char buf[MSL];

	switch (iEthos)
	{
	case ETHOS_ANY:
		sprintf(buf, "Any");
		break;
	case ETHOS_LN:
		sprintf(buf, "L/N");
		break;
	case ETHOS_NC:
		sprintf(buf, "N/C");
		break;
	case ETHOS_LC:
		sprintf(buf, "L/C");
		break;
	case ETHOS_L:
		sprintf(buf, "L");
		break;
	case ETHOS_N:
		sprintf(buf, "N");
		break;
	case ETHOS_C:
		sprintf(buf, "C");
		break;
	default:
		sprintf(buf, "None");
		break;
	}

	return str_dup(buf);
}
