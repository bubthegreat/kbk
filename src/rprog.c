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

DECLARE_RPROG_FUN_SPEECH(speech_prog_realm_dead);
DECLARE_RPROG_FUN_ENTRY(rprog_entry_esiraen_fall);
DECLARE_RPROG_FUN_ENTRY(rprog_entry_noon_door);
DECLARE_RPROG_FUN_ENTRY(rprog_entry_secret_switch);
DECLARE_RPROG_FUN_ENTRY(rprog_entry_threshold_route);

const struct improg_type rprog_table[] =
	{
		{"speech_prog", "speech_prog_realm_dead", speech_prog_realm_dead},
		{"entry_prog", "rprog_entry_esiraen_fall", rprog_entry_esiraen_fall},
		{"entry_prog", "rprog_entry_noon_door", rprog_entry_noon_door},
		{"entry_prog", "rprog_entry_secret_switch", rprog_entry_secret_switch},
		{"entry_prog", "rprog_entry_threshold_route", rprog_entry_threshold_route},
		{NULL, NULL, NULL},
};

void rprog_set(ROOM_INDEX_DATA *pRoom, const char *progtype, const char *name)
{
	int i;

	for (i = 0; rprog_table[i].name != NULL; i++)
		if (!str_cmp(rprog_table[i].name, name))
			break;

	if (!rprog_table[i].name)
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

	bug("Load_improgs: 'R': invalid program type for vnum %d", pRoom->vnum);
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

/*
 * The Odd Metal Door (room 31050): its north exit yields only when the sun
 * stands at its highest.  Re-evaluated each time someone enters the room.
 */
void rprog_entry_noon_door(ROOM_INDEX_DATA *room, CHAR_DATA *ch)
{
	EXIT_DATA *pexit;
	ROOM_INDEX_DATA *to_room;

	if (IS_NPC(ch))
		return;

	pexit = room->exit[DIR_NORTH];
	if (pexit == NULL)
		return;

	if (time_info.hour == 12)
	{
		if (IS_SET(pexit->exit_info, EX_CLOSED))
		{
			REMOVE_BIT(pexit->exit_info, EX_CLOSED);
			REMOVE_BIT(pexit->exit_info, EX_LOCKED);
			if ((to_room = pexit->u1.to_room) != NULL && to_room->exit[DIR_SOUTH] != NULL)
			{
				REMOVE_BIT(to_room->exit[DIR_SOUTH]->exit_info, EX_CLOSED);
				REMOVE_BIT(to_room->exit[DIR_SOUTH]->exit_info, EX_LOCKED);
			}
			act("With a deep, grinding resonance the great metal door draws back, and the noon light pours through.", ch, NULL, NULL, TO_ROOM);
			send_to_char("With a deep, grinding resonance the great metal door draws back, and the noon light pours through.\n\r", ch);
		}
	}
	else
	{
		if (!IS_SET(pexit->exit_info, EX_CLOSED))
		{
			SET_BIT(pexit->exit_info, EX_CLOSED);
			if ((to_room = pexit->u1.to_room) != NULL && to_room->exit[DIR_SOUTH] != NULL)
				SET_BIT(to_room->exit[DIR_SOUTH]->exit_info, EX_CLOSED);
		}
		send_to_char("The great metal door stands sealed and cold; its markings promise it opens only when the sun stands at its highest.\n\r", ch);
	}
	return;
}

/*
 * The Secret Workshop (room 31039): reaching the hidden triangular switch
 * unseals the plugged passage from the natural cavern (31046, down) into the
 * smooth metal tunnel (31047, up) -- the tunnel that leads to the trap.
 */
void rprog_entry_secret_switch(ROOM_INDEX_DATA *room, CHAR_DATA *ch)
{
	ROOM_INDEX_DATA *cavern, *tunnel;
	EXIT_DATA *pexit;

	if (IS_NPC(ch))
		return;

	cavern = get_room_index(31046);
	tunnel = get_room_index(31047);
	if (cavern == NULL)
		return;

	pexit = cavern->exit[DIR_DOWN];
	if (pexit == NULL)
		return;

	if (IS_SET(pexit->exit_info, EX_CLOSED) || IS_SET(pexit->exit_info, EX_LOCKED))
	{
		REMOVE_BIT(pexit->exit_info, EX_CLOSED);
		REMOVE_BIT(pexit->exit_info, EX_LOCKED);
		if (tunnel != NULL && tunnel->exit[DIR_UP] != NULL)
		{
			REMOVE_BIT(tunnel->exit[DIR_UP]->exit_info, EX_CLOSED);
			REMOVE_BIT(tunnel->exit[DIR_UP]->exit_info, EX_LOCKED);
		}
		send_to_char("As your hand finds the triangular stone it sinks into the wall with a heavy click, and far off in the mountain stone grinds on stone -- a sealed way, opening at last.\n\r", ch);
		act("The triangular stone sinks into the wall with a click, and a distant grinding shudders up through the rock.", ch, NULL, NULL, TO_ROOM);
	}
	return;
}

/*
 * The Threshold (room 31068): the sundial reads the current hour and the
 * archway north opens onto a different era-wing of the lab depending on when
 * you cross it.  The in-game day (0-23) is split into seven chronological
 * bands, one per era, earliest to latest.
 */
void rprog_entry_threshold_route(ROOM_INDEX_DATA *room, CHAR_DATA *ch)
{
	EXIT_DATA *pexit;
	ROOM_INDEX_DATA *to_room;
	int hour, dest;
	const char *era;
	char buf[MAX_STRING_LENGTH];

	if (IS_NPC(ch))
		return;

	pexit = room->exit[DIR_NORTH];
	if (pexit == NULL)
		return;

	hour = time_info.hour;
	if (hour <= 2)
	{
		dest = 31069;
		era = "a cold vault of wheeling stars";
	}
	else if (hour <= 6)
	{
		dest = 31070;
		era = "a hall of endlessly falling sand";
	}
	else if (hour <= 9)
	{
		dest = 31071;
		era = "a workshop of swinging pendulums";
	}
	else if (hour <= 13)
	{
		dest = 31072;
		era = "a wing of hungry, roaring fire";
	}
	else if (hour <= 16)
	{
		dest = 31073;
		era = "a hall where loose lightning crawls";
	}
	else if (hour <= 20)
	{
		dest = 31074;
		era = "a place that loops upon itself";
	}
	else
	{
		dest = 31075;
		era = "a wing fraying out of the world";
	}

	if ((to_room = get_room_index(dest)) == NULL)
		return;

	pexit->u1.to_room = to_room;

	sprintf(buf, "The sundial's shadow settles, and the archway north opens onto %s.\n\r", era);
	send_to_char(buf, ch);
	return;
}
