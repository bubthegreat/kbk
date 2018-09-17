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
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
*       ROM 2.4 is copyright 1993-1996 Russ Taylor                         *
*       ROM has been brought to you by the ROM consortium                  *
*           Russ Taylor (rtaylor@pacinfo.com)                              *
*           Gabrielle Taylor (gtaylor@pacinfo.com)                         *
*           Brian Moore (rom@rom.efn.org)                                  *
*       By using this code, you have agreed to follow the terms of the     *
*       ROM license, in the file Tartarus/doc/rom.license                  *
***************************************************************************/

/***************************************************************************
*       Tartarus code is copyright (C) 1997-1998 by Daniel Graham          *
*       In using this code you agree to comply with the Tartarus license   *
*       found in the file /Tartarus/doc/tartarus.doc                       *
***************************************************************************/

#include "include.h"

DECLARE_RPROG_FUN_SPEECH (	speech_prog_realm_dead			);
DECLARE_RPROG_FUN_ENTRY	(	rprog_entry_esiraen_fall		);

const   struct  improg_type   rprog_table[] =
{
	{	"speech_prog",	"speech_prog_realm_dead",		speech_prog_realm_dead		},
	{	"entry_prog",	"rprog_entry_esiraen_fall",		rprog_entry_esiraen_fall	},
        {       NULL,           NULL,                                   NULL                            },
};

void rprog_set(ROOM_INDEX_DATA *pRoom, const char *progtype, const char *name)
{
    int i;

    for(i = 0; rprog_table[i].name != NULL; i++)
	if(!str_cmp(rprog_table[i].name, name))
	    break;

    if(!rprog_table[i].name)
	return bug("Load_improgs: 'R': Function not found for vnum %d", pRoom->vnum);

    if (!str_cmp(progtype, "speech_prog"))
    {
	pRoom->rprogs->speech_prog = rprog_table[i].function;
	free_string(pRoom->rprogs->speech_name);
	pRoom->rprogs->speech_name = str_dup(name);
	SET_BIT(pRoom->progtypes, RPROG_SPEECH);
	return;
    }
    if (!str_cmp(progtype, "entry_prog"))
    {
        pRoom->rprogs->entry_prog = rprog_table[i].function;
        free_string(pRoom->rprogs->entry_name);
        pRoom->rprogs->entry_name = str_dup(name);
        SET_BIT(pRoom->progtypes, RPROG_ENTRY);
        return;
    }

    bug( "Load_improgs: 'R': invalid program type for vnum %d", pRoom->vnum);
    exit(1);
}

void speech_prog_realm_dead(ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *speech)
{
	send_to_char("Room progs work!\n\r", ch);
	return;
}

void rprog_entry_esiraen_fall(ROOM_INDEX_DATA *room, CHAR_DATA *ch)
{
	if (isNewbie(ch))
		return;
	if (IS_IMMORTAL(ch))
		return;
	if (ch->ghost > 0)
		return;
	send_to_char("{bYou scream in absolute terror as you realize that you've fallen from impossible heights.{x\n\r", ch);
	send_to_char("{bThe last thing you hear is the sound of your body being slammed against the icy water.{x\n\r", ch);
	raw_kill_new(ch, ch, TRUE, FALSE);
	return;
}
