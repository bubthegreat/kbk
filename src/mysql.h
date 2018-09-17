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
MYSQL                   	conn;

#define SQL_SERVER      	"localhost"
#define SQL_USER        	""
#define SQL_PWD         	""
#define SQL_DB          	""

void    init_mysql		args( ( void ) );
void    close_db                args( ( void ) );
int     mysql_safe_query        args( (char *fmt, ...) );
void    login_log               args( (CHAR_DATA *ch, int type) );
void 	saveCharmed		args( (CHAR_DATA *ch) );
void 	charmedToDatabase	args( (CHAR_DATA *charm) );
void 	loadCharmed		args( (CHAR_DATA *ch) );
void 	printCharmed		args( (CHAR_DATA *ch, char *name) );
void 	pruneDatabase		args( (void ) );
void 	updatePlayerAuth	args( (CHAR_DATA *ch) );
void	updatePlayerDb		args( (CHAR_DATA *ch) );

#define LTYPE_IN                0
#define LTYPE_OUT               1
#define LTYPE_NEW               2
#define LTYPE_AUTO		3

// test
