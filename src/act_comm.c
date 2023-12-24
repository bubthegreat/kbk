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
 **************************************************************************/

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

char *garble(CHAR_DATA *ch, char *i);
const char *upstring(const char *i);
bool arena;

/* RT code to delete yourself */

void do_delet(CHAR_DATA *ch, char *argument)
{
	send_to_char("You must type the full command to delete yourself.\n\r", ch);
}

void do_delete(CHAR_DATA *ch, char *argument)
{
	char strsave[MAX_INPUT_LENGTH];
	char *cname;
	if (IS_NPC(ch))
		return;

	if (ch->pcdata->confirm_delete)
	{
		if (argument[0] != '\0')
		{
			send_to_char("Delete status removed.\n\r", ch);
			ch->pcdata->confirm_delete = FALSE;
			return;
		}
		else
		{
			perm_death_log(ch, 1);
			sprintf(strsave, "%s%s.plr", PLAYER_DIR, capitalize(ch->original_name));
			wiznet("$N turns $Mself into line noise.", ch, NULL, 0, 0, 0);
			ch->pause = 0;
			while (ch->affected)
			{
				affect_remove(ch, ch->affected);
			}
			stop_fighting(ch, TRUE);
			cname = str_dup(ch->original_name);
			do_quit_new(ch, "", TRUE);
			if (((ch->played + (current_time - ch->logon)) / 3600) >= 15)
				delete_char(cname, TRUE); // >= 15 hours. Make name unusable.
			else
				delete_char(cname, FALSE);
			return;
		}
	}

	if (argument[0] != '\0')
	{
		send_to_char("Just type delete. No argument.\n\r", ch);
		return;
	}

	send_to_char("Type delete again to confirm this command.\n\r", ch);
	send_to_char("{f{RWARNING{x: this command is irreversible.\n\r", ch);
	send_to_char("Typing delete with an argument will undo delete status.\n\r",
				 ch);
	ch->pcdata->confirm_delete = TRUE;

	if (((ch->played + (current_time - ch->logon)) / 3600) >= 15)
	{
		send_to_char("{f{RALERT{x: Deletion will render this name unusable.\n\r", ch);
	}
	wiznet("$N is contemplating deletion.", ch, NULL, 0, 0, get_trust(ch));
	WAIT_STATE(ch, PULSE_VIOLENCE * 3);
}

/* RT code to display channel status */

void do_channels(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];

	/* lists all channels and their status */
	send_to_char("CHANNEL        STATUS\n\r", ch);
	send_to_char("---------------------\n\r", ch);

	send_to_char("auction        ", ch);
	if (!IS_SET(ch->comm, COMM_NOAUCTION))
		send_to_char("ON\n\r", ch);
	else
		send_to_char("OFF\n\r", ch);

	send_to_char("Q/A            ", ch);
	if (!IS_SET(ch->comm, COMM_NOQUESTION))
		send_to_char("ON\n\r", ch);
	else
		send_to_char("OFF\n\r", ch);

	if (IS_IMMORTAL(ch))
	{
		send_to_char("god channel    ", ch);
		if (!IS_SET(ch->comm, COMM_NOWIZ))
			send_to_char("ON\n\r", ch);
		else
			send_to_char("OFF\n\r", ch);
	}

	send_to_char("tells          ", ch);
	if (!IS_SET(ch->comm, COMM_DEAF))
		send_to_char("ON\n\r", ch);
	else
		send_to_char("OFF\n\r", ch);

	if (ch->cabal)
	{
		send_to_char("cabal channel  ", ch);
		if (!IS_SET(ch->comm, COMM_NOCABAL))
			send_to_char("ON\n\r", ch);
		else
			send_to_char("OFF\n\r", ch);
	}

	send_to_char("global talk    ", ch);
	if (!IS_SET(ch->comm, COMM_NOGLOBAL))
		send_to_char("ON\n\r", ch);
	else
		send_to_char("OFF\n\r", ch);

	send_to_char("quiet mode     ", ch);
	if (IS_SET(ch->comm, COMM_QUIET))
		send_to_char("ON\n\r", ch);
	else
		send_to_char("OFF\n\r", ch);

	if (IS_SET(ch->comm, COMM_SNOOP_PROOF))
		send_to_char("You are immune to snooping.\n\r", ch);

	if (ch->lines != PAGELEN)
	{
		if (ch->lines)
		{
			sprintf(buf, "You display %d lines of scroll.\n\r", ch->lines + 2);
			send_to_char(buf, ch);
		}
		else
			send_to_char("Scroll buffering is off.\n\r", ch);
	}

	if (IS_SET(ch->comm, COMM_NOTELL))
		send_to_char("You cannot use tell.\n\r", ch);

	if (IS_SET(ch->comm, COMM_NOCHANNELS))
		send_to_char("You cannot use channels.\n\r", ch);

	if (IS_SET(ch->comm, COMM_NOEMOTE))
		send_to_char("You cannot show emotions.\n\r", ch);
}

void do_globaltalk(CHAR_DATA *ch, char *argument)
{
	DESCRIPTOR_DATA *d;
	char buf[MSL];

	if (argument[0] == '\0')
	{
		if (IS_SET(ch->comm, COMM_NOGLOBAL))
		{
			send_to_char("Global talk channel is now ON.\n\r", ch);
			REMOVE_BIT(ch->comm, COMM_NOGLOBAL);
		}
		else
		{
			send_to_char("Global talk channel is now OFF.\n\r", ch);
			SET_BIT(ch->comm, COMM_NOGLOBAL);
		}
		return;
	}

	if (IS_SET(ch->comm, COMM_NOCHANNELS))
	{
		send_to_char("The gods have revoked your channel priviliges. Haha.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_silence))
	{
		send_to_char("You cannot find the words within you.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_tigerclaw))
	{
		send_to_char("Your throat constricts too painfully.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_gag))
	{
		send_to_char("That won't work with a gag in your mouth.\n\r", ch);
		return;
	}

	argument = garble(ch, argument);

	REMOVE_BIT(ch->comm, COMM_NOGLOBAL);

	act_new("({CGLOBAL{x) $n: $t", ch, argument, NULL, TO_CHAR, POS_DEAD);

	for (d = descriptor_list; d != NULL; d = d->next)
	{
		if (d->connected == CON_PLAYING && d->character != ch && !IS_SET(d->character->comm, COMM_NOGLOBAL))
		{
			if (IS_IMMORTAL(d->character) && str_cmp(ch->name, ch->original_name))
			{
				sprintf(buf, "({CGLOBAL{x) %s (%s): %s\n\r", ch->name, ch->original_name, argument);
				send_to_char(buf, d->character);
			}
			else
				act_new("({CGLOBAL{x) $n: $t", ch, argument, d->character, TO_VICT, POS_DEAD);
		}
	}
	return;
}

void do_deaf(CHAR_DATA *ch, char *argument)
{

	if (IS_SET(ch->comm, COMM_DEAF))
	{
		send_to_char("You can now hear tells again.\n\r", ch);
		REMOVE_BIT(ch->comm, COMM_DEAF);
	}
	else
	{
		send_to_char("From now on, you won't hear tells.\n\r", ch);
		SET_BIT(ch->comm, COMM_DEAF);
	}
}

/* RT quiet blocks out all communication */

void do_quiet(CHAR_DATA *ch, char *argument)
{
	if (IS_SET(ch->comm, COMM_QUIET))
	{
		send_to_char("Quiet mode removed.\n\r", ch);
		REMOVE_BIT(ch->comm, COMM_QUIET);
	}
	else
	{
		send_to_char("From now on, you will only hear says and emotes.\n\r", ch);
		SET_BIT(ch->comm, COMM_QUIET);
	}
}

void do_replay(CHAR_DATA *ch, char *argument)
{
	if (IS_NPC(ch))
	{
		send_to_char("You can't replay.\n\r", ch);
		return;
	}

	if (buf_string(ch->pcdata->buffer)[0] == '\0')
	{
		send_to_char("You have no tells to replay.\n\r", ch);
		return;
	}

	page_to_char(buf_string(ch->pcdata->buffer), ch);
	clear_buf(ch->pcdata->buffer);
}

/* RT auction rewritten in ROM style */
void do_auction(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	DESCRIPTOR_DATA *d;

	if (argument[0] == '\0')
	{
		if (IS_SET(ch->comm, COMM_NOAUCTION))
		{
			send_to_char("Auction channel is now ON.\n\r", ch);
			REMOVE_BIT(ch->comm, COMM_NOAUCTION);
		}
		else
		{
			send_to_char("Auction channel is now OFF.\n\r", ch);
			SET_BIT(ch->comm, COMM_NOAUCTION);
		}
	}
	else /* auction message sent, turn auction on if it is off */
	{
		if (IS_SET(ch->comm, COMM_QUIET))
		{
			send_to_char("You must turn off quiet mode first.\n\r", ch);
			return;
		}

		if (IS_SET(ch->comm, COMM_NOCHANNELS))
		{
			send_to_char("The gods have revoked your channel priviliges.\n\r", ch);
			return;
		}

		if (is_affected(ch, gsn_silence))
		{
			send_to_char("You cannot find the words within you.\n\r", ch);
			return;
		}

		if (is_affected(ch, gsn_gag))
		{
			send_to_char("You find difficultly talking.\n\r", ch);
			return;
		}

		argument = garble(ch, argument);

		REMOVE_BIT(ch->comm, COMM_NOAUCTION);
		if (IS_SET(ch->act, PLR_COLOR))
			sprintf(buf, "You auction '\x01B[1;36m%s\x01B[0;37m'\n\r", argument);
		else
			sprintf(buf, "You auction '%s'\n\r", argument);
		send_to_char(buf, ch);
		for (d = descriptor_list; d != NULL; d = d->next)
		{
			CHAR_DATA *victim;

			victim = d->original ? d->original : d->character;

			if (d->connected == CON_PLAYING &&
				d->character != ch &&
				ch->in_room->area == victim->in_room->area &&
				!IS_SET(victim->comm, COMM_NOAUCTION) &&
				!IS_SET(victim->comm, COMM_QUIET))
			{
				if (IS_SET(d->character->act, PLR_COLOR))
					act_new("$n auctions '\x01B[1;36m$t\x01B[0;37m'", ch, argument, d->character, TO_VICT, POS_DEAD);
				else
					act_new("$n auctions '$t'", ch, argument, d->character, TO_VICT, POS_DEAD);
			}
		}
	}
}

/* RT question channel */
void do_question(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	DESCRIPTOR_DATA *d;

	if (argument[0] == '\0')
	{
		if (IS_SET(ch->comm, COMM_NOQUESTION))
		{
			send_to_char("Q/A channel is now ON.\n\r", ch);
			REMOVE_BIT(ch->comm, COMM_NOQUESTION);
		}
		else
		{
			send_to_char("Q/A channel is now OFF.\n\r", ch);
			SET_BIT(ch->comm, COMM_NOQUESTION);
		}
	}
	else /* question sent, turn Q/A on if it isn't already */
	{
		if (IS_SET(ch->comm, COMM_QUIET))
		{
			send_to_char("You must turn off quiet mode first.\n\r", ch);
			return;
		}

		if (IS_SET(ch->comm, COMM_NOCHANNELS))
		{
			send_to_char("The gods have revoked your channel priviliges.\n\r", ch);
			return;
		}

		if (is_affected(ch, gsn_tigerclaw))
		{
			send_to_char("Your throat constricts too painfully.\n\r", ch);
			return;
		}

		if (is_affected(ch, gsn_gag))
		{
			send_to_char("You find difficultly talking.\n\r", ch);
			return;
		}

		argument = garble(ch, argument);

		REMOVE_BIT(ch->comm, COMM_NOQUESTION);
		if (IS_SET(ch->act, PLR_COLOR))
			sprintf(buf, "You question '\x01B[1;36m%s\x01B[0;37m'\n\r", argument);
		else
			sprintf(buf, "You question '%s'\n\r", argument);
		send_to_char(buf, ch);
		for (d = descriptor_list; d != NULL; d = d->next)
		{
			CHAR_DATA *victim;

			victim = d->original ? d->original : d->character;

			if (d->connected == CON_PLAYING &&
				d->character != ch &&
				d->character->in_room->area == ch->in_room->area &&
				!IS_SET(victim->comm, COMM_NOQUESTION) &&
				!IS_SET(victim->comm, COMM_QUIET))
			{
				if (IS_SET(d->character->act, PLR_COLOR))
					act_new("$n questions '\x01B[1;36m$t\x01B[0;37m'",
							ch, argument, d->character, TO_VICT, POS_SLEEPING);
				else
					act_new("$n questions '$t'",
							ch, argument, d->character, TO_VICT, POS_SLEEPING);
			}
		}
	}
}

/* RT answer channel - uses same line as questions */
void do_answer(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	DESCRIPTOR_DATA *d;

	if (argument[0] == '\0')
	{
		if (IS_SET(ch->comm, COMM_NOQUESTION))
		{
			send_to_char("Q/A channel is now ON.\n\r", ch);
			REMOVE_BIT(ch->comm, COMM_NOQUESTION);
		}
		else
		{
			send_to_char("Q/A channel is now OFF.\n\r", ch);
			SET_BIT(ch->comm, COMM_NOQUESTION);
		}
	}
	else /* answer sent, turn Q/A on if it isn't already */
	{
		if (IS_SET(ch->comm, COMM_QUIET))
		{
			send_to_char("You must turn off quiet mode first.\n\r", ch);
			return;
		}

		if (IS_SET(ch->comm, COMM_NOCHANNELS))
		{
			send_to_char("The gods have revoked your channel priviliges.\n\r", ch);
			return;
		}

		if (is_affected(ch, gsn_tigerclaw))
		{
			send_to_char("Your throat constricts too painfully.\n\r", ch);
			return;
		}

		if (is_affected(ch, gsn_gag))
		{
			send_to_char("You find difficultly talking.\n\r", ch);
			return;
		}

		argument = garble(ch, argument);

		REMOVE_BIT(ch->comm, COMM_NOQUESTION);
		if (IS_SET(ch->act, PLR_COLOR))
			sprintf(buf, "You answer '\x01B[1;36m%s\x01B[0;37m'\n\r", argument);
		else
			sprintf(buf, "You answer '%s'\n\r", argument);
		send_to_char(buf, ch);
		for (d = descriptor_list; d != NULL; d = d->next)
		{
			CHAR_DATA *victim;

			victim = d->original ? d->original : d->character;

			if (d->connected == CON_PLAYING &&
				d->character != ch &&
				d->character->in_room->area == ch->in_room->area &&
				!IS_SET(victim->comm, COMM_NOQUESTION) &&
				!IS_SET(victim->comm, COMM_QUIET))
			{
				if (IS_SET(d->character->act, PLR_COLOR))
					act_new("$n answers '\x01B[1;36m$t\x01B[0;37m'",
							ch, argument, d->character, TO_VICT, POS_SLEEPING);
				else
					act_new("$n answers '$t'",
							ch, argument, d->character, TO_VICT, POS_SLEEPING);
			}
		}
	}
}

void do_cb(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	DESCRIPTOR_DATA *d;

	if (!is_cabal(ch) || cabal_table[ch->cabal].independent)
	{
		send_to_char("You aren't in a cabal.\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		if (IS_SET(ch->comm, COMM_NOCABAL))
		{
			send_to_char("Cabal channel is now ON\n\r", ch);
			REMOVE_BIT(ch->comm, COMM_NOCABAL);
		}
		else
		{
			send_to_char("Cabal channel is now OFF\n\r", ch);
			SET_BIT(ch->comm, COMM_NOCABAL);
		}
		return;
	}

	if (is_affected(ch, gsn_silence))
	{
		send_to_char("You cannot find the words within you.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_tigerclaw))
	{
		send_to_char("Your throat constricts too painfully.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_gag))
	{
		send_to_char("You find difficultly talking.\n\r", ch);
		return;
	}

	if (IS_SET(ch->comm, COMM_NOCHANNELS))
	{
		send_to_char("The gods have revoked your channel priviliges.\n\r", ch);
		return;
	}

	argument = garble(ch, argument);

	if (is_affected(ch, gsn_deafen))
	{
		sprintf(argument, "%s", upstring(argument));
	}

	REMOVE_BIT(ch->comm, COMM_NOCABAL);

	if (!IS_IMMORTAL(ch))
		sprintf(buf, "%s%s: {c%s{x\n\r", cabal_table[ch->cabal].who_name, IS_NPC(ch) ? ch->short_descr : ch->original_name, argument);
	else
		sprintf(buf, "%s%s (%s): {c%s{x\n\r", cabal_table[ch->cabal].who_name, IS_NPC(ch) ? ch->short_descr : ch->original_name, timestamp(), argument);
	send_to_char(buf, ch);

	for (d = descriptor_list; d != NULL; d = d->next)
	{
		if (d->connected == CON_PLAYING && d->character != ch && ((is_same_cabal(ch, d->character) && !IS_SET(d->character->comm, COMM_NOCABAL)) || IS_SET(d->character->comm, COMM_ALL_CABALS) || (d->character->eavesdrop != 0 && d->character->eavesdrop == ch->cabal)))
		{
			if (!IS_IMMORTAL(d->character))
				sprintf(buf, "%s%s: {c%s{x\n\r", cabal_table[ch->cabal].who_name,
						!IS_NPC(ch) && can_see(d->character, ch) ? ch->original_name : PERS(ch, d->character), argument);
			else
				sprintf(buf, "%s%s (%s): {c%s{x\n\r", cabal_table[ch->cabal].who_name,
						!IS_NPC(ch) && can_see(d->character, ch) ? ch->original_name : PERS(ch, d->character), timestamp(), argument);
			send_to_char(buf, d->character);
		}
	}
	return;
}

void do_global(CHAR_DATA *ch, char *argument)
{
	if (IS_SET(ch->comm, COMM_ALL_CABALS))
	{
		send_to_char("You will no longer hear all Cabal channels.\n\r", ch);
		REMOVE_BIT(ch->comm, COMM_ALL_CABALS);
	}
	else
	{
		send_to_char("You will now hear all Cabal channels.\n\r", ch);
		SET_BIT(ch->comm, COMM_ALL_CABALS);
	}
	return;
}

void do_immtalk(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *wch;
	int trust;
	char arg1[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];
	char buf2[MAX_STRING_LENGTH];
	int level;

	trust = get_trust(ch);

	if (trust < 52 && (!IS_SET(ch->comm, COMM_IMMORTAL) && (!IS_HEROIMM(ch))))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		if (IS_SET(ch->comm, COMM_NOWIZ))
		{
			send_to_char("Immortal channel is now ON\n\r", ch);
			REMOVE_BIT(ch->comm, COMM_NOWIZ);
		}
		else
		{
			send_to_char("Immortal channel is now OFF\n\r", ch);
			SET_BIT(ch->comm, COMM_NOWIZ);
		}
		return;
	}

	sprintf(buf2, "%s", argument);
	argument = one_argument(argument, arg1);

	if (arg1[0] == '\0')
	{
		send_to_char("Syntax: immtalk <level?> <message>\n\r", ch);
		return;
	}

	if (!is_number(arg1))
	{
		level = LEVEL_HERO;
	}
	else
	{
		level = atoi(arg1);
		if (level > trust || level < LEVEL_HERO)
		{
			level = LEVEL_HERO;
		}
		else
		{
			free_string(buf2);
			sprintf(buf2, "%s", argument);
		}
	}

	if (buf2[0] == '\0')
	{
		send_to_char("Syntax: immtalk <level?> <message>\n\r", ch);
		return;
	}

	if (level > LEVEL_HERO)
		sprintf(buf, " (%s) [%d]: %s", timestamp(), level, buf2);
	else
		sprintf(buf, " (%s): %s", timestamp(), buf2);

	REMOVE_BIT(ch->comm, COMM_NOWIZ);

	sprintf(buf2, "{g[IMM] %s%s{x\n\r", ch->original_name, buf);
	send_to_char(buf2, ch);

	for (wch = char_list; wch != NULL; wch = wch->next)
	{
		if (IS_NPC(wch) && wch->desc == NULL)
			continue;

		if (ch == wch)
			continue;

		if ((IS_IMMORTAL(wch)) || IS_SET(wch->comm, COMM_IMMORTAL) || IS_HEROIMM(wch) ||
			(IS_NPC(wch) && (wch->desc != NULL) && IS_IMMORTAL(wch->desc->original)))
		{
			if (level > get_trust(wch))
				continue;

			sprintf(buf2, "{g[IMM] %s%s{x\n\r", ch->original_name, buf);
			send_to_char(buf2, wch);
		}
	}
	return;
}

/* This is a real messy hack for a builder channel where people making
areas, but who aren't immortals can talk and discuss the matter...(Ceran)
*/
void do_builder(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *wch;

	if (!IS_SET(ch->comm, COMM_BUILDER))
	{

		send_to_char("Huh?\n\r", ch);
		return;
	}
	if (argument[0] == '\0')
		return;

	if (IS_SET(ch->act, PLR_COLOR))
		act_new("\x01B[1;34m[BUILDER] $n: $t\x01B[0;37m", ch, argument, NULL, TO_CHAR, POS_DEAD);
	else
		act_new("[BUILDER] $n: $t", ch, argument, NULL, TO_CHAR, POS_DEAD);

	for (wch = char_list; wch != NULL; wch = wch->next)
	{
		if (IS_NPC(wch))
			continue;
		if (IS_SET(wch->comm, COMM_BUILDER))
		{
			if (IS_SET(wch->act, PLR_COLOR))
				act_new("\x01B[1;34m[BUILDER] $n: $t\x01B[0;37m", ch, argument, wch, TO_VICT, POS_DEAD);
			else
				act_new("[BUILDER] $n: $t", ch, argument, wch, TO_VICT, POS_DEAD);
		}
	}

	return;
}

/* Used for an object...take this out if you don't want it */
void obj_say_heavenly_sceptre(CHAR_DATA *ch, OBJ_DATA *obj, char *argument)
{
	int sn;
	sn = -1;

	if (strstr(argument, "i am the wrath of god") || strstr(argument, "I am the wrath of god"))
		sn = skill_lookup("heavenly wrath");

	if (strstr(argument, "feel the force of god") || strstr(argument, "Feel the force of god"))
		sn = skill_lookup("heavenly fire");

	if (sn == -1)
		return;

	(*skill_table[sn].spell_fun)(sn, obj->level, ch, NULL, TAR_IGNORE);
	return;
}

/*static const char *garble(CHAR_DATA *ch, const char *i)
{
	static char buf[MAX_STRING_LENGTH];
	char *o;

	if (!is_affected(ch, gsn_garble))
		return i;

	for (o = buf; *i && o-buf < sizeof(buf)-1; i++, o++) {
		if (*i >= 65 && *i <= 90)
		{
			*o = number_range(65, 90);
		}
		else if (*i >= 97 && *i <= 122)
		{
			*o = number_range(97,122);
		}
		else
		{
			*o = *i;
		}
	}
	*o = '\0';
	return buf;
}
*/

char *garble(CHAR_DATA *ch, char *i)
{
	static char buf[MAX_STRING_LENGTH];
	char *o;

	if (!is_affected(ch, gsn_garble))
		return i;

	for (o = buf; *i && o - buf < sizeof(buf) - 1; i++, o++)
	{
		if (*i >= 65 && *i <= 90)
		{
			*o = number_range(65, 90);
		}
		else if (*i >= 97 && *i <= 122)
		{
			*o = number_range(97, 122);
		}
		else
		{
			*o = *i;
		}
	}
	*o = '\0';
	return buf;
}

/*static const char *upstring(CHAR_DATA *ch, const char *i)
{
	static char buf[MAX_STRING_LENGTH];
	char *o;

	for (o = buf; *i && o-buf < sizeof(buf)-1; i++, o++) {
		if (*i >= 97 && *i <= 122)
		{
			*o = UPPER(*i);
		}
		else
		{
			*o = *i;
		}
	}
	*o = '\0';
	return buf;
}
*/

const char *upstring(const char *i)
{
	static char buf[MAX_STRING_LENGTH];
	char *o;
	const char * or ;
	or = i;

	for (o = buf; *i && o - buf < sizeof(buf) - 1; i++, o++)
	{
		*o = UPPER(*i);
	}
	*o = '\0';
	return buf;
}

void do_say(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	CHAR_DATA *room_char;
	OBJ_DATA *char_obj;
	char buf[MAX_STRING_LENGTH];

	if (argument[0] == '\0')
	{
		send_to_char("Say what?\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_silence))
	{
		send_to_char("You cannot find the words within you.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_tigerclaw))
	{
		send_to_char("Your throat constricts too painfully.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_gag))
	{
		send_to_char("You find difficultly talking.\n\r", ch);
		return;
	}

	argument = garble(ch, argument);

	if (is_affected(ch, gsn_deafen))
	{
		do_yell(ch, argument);
		return;
	}

	for (victim = ch->in_room->people; victim != NULL; victim = victim->next_in_room)
	{

		if (IS_AWAKE(victim))
		{
			if (is_affected(victim, gsn_shock_sphere))
			{
				act("$n says something you can't quite hear.", ch, 0, victim, TO_VICT);
			}
			else if (!(IS_IMMORTAL(ch)) && is_affected(victim, gsn_deafen))
			{
				act("$n says something you can't quite hear.", ch, 0, victim, TO_VICT);
			}
			else
			{
				if ((IS_IMMORTAL(victim) || victim->cabal == CABAL_ANCIENT) && victim != ch && (is_affected(ch, gsn_cloak_form)))
				{
					sprintf(buf, "A %s [%s] says '{Y%s{x'\n\r", ch->name, ch->original_name, argument);
					send_to_char(buf, victim);
				}
				else
					act("$n says '{Y$t{x'", ch, argument, victim, TO_VICT);
			}
		}
	}

	act("You say '{Y$T{x'", ch, NULL, argument, TO_CHAR);

	for (room_char = ch->in_room->people; room_char != NULL; room_char = room_char->next_in_room)
	{
		if (IS_SET(room_char->progtypes, MPROG_SPEECH) && room_char != ch)
		{
			(room_char->pIndexData->mprogs->speech_prog)(room_char, ch, argument);
		}
	}

	report_cabal_items(ch, argument);
	check_bloodoath(ch, argument);

	for (char_obj = ch->carrying; char_obj != NULL; char_obj = char_obj->next_content)
	{
		if (IS_SET(char_obj->progtypes, IPROG_SPEECH))
			(char_obj->pIndexData->iprogs->speech_prog)(char_obj, ch, argument);
	}

	for (char_obj = ch->in_room->contents; char_obj != NULL; char_obj = char_obj->next_content)
	{
		if (IS_SET(char_obj->progtypes, IPROG_SPEECH))
			(char_obj->pIndexData->iprogs->speech_prog)(char_obj, ch, argument);
	}

	if (IS_SET(ch->in_room->progtypes, RPROG_SPEECH))
		(ch->in_room->rprogs->speech_prog)(ch->in_room, ch, argument);

	return;
}

void do_whisper(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	char buf[MAX_STRING_LENGTH];

	if (!IS_NPC(ch))

		if (argument[0] == '\0')
		{
			send_to_char("Whisper what?\n\r", ch);
			return;
		}

	if (is_affected(ch, gsn_silence))
	{
		send_to_char("You cannot find the words within you.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_tigerclaw))
	{
		send_to_char("Your throat constricts too painfully.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_gag))
	{
		send_to_char("You find difficultly talking.\n\r", ch);
		return;
	}

	argument = garble(ch, argument);

	if (is_affected(ch, gsn_deafen))
	{
		do_yell(ch, argument);
		return;
	}

	for (victim = ch->in_room->people; victim != NULL; victim = victim->next_in_room)
	{
		if (IS_AWAKE(victim))
		{
			if (is_affected(victim, gsn_shock_sphere))
			{
				act("$n says something you can't quite hear.", ch, 0, victim, TO_VICT);
			}
			else if (!(IS_IMMORTAL(ch)) && is_affected(victim, gsn_deafen))
			{
				act("$n says something you can't quite hear.", ch, 0, victim, TO_VICT);
			}
			else
			{
				if (IS_IMMORTAL(victim) && is_affected(ch, gsn_cloak_form))
				{
					sprintf(buf, "A %s [%s] whispers '{r%s{x'\n\r", ch->name, ch->original_name, argument);
					send_to_char(buf, victim);
				}
				else
					act("$n whispers '{r$t{x'", ch, argument, victim, TO_VICT);
			}
		}
	}

	act("You whisper '{r$T{x'", ch, NULL, argument, TO_CHAR);

	return;
}

void do_pray(CHAR_DATA *ch, char *argument)
{
	DESCRIPTOR_DATA *d;
	char *check, buf[MSL];

	if (argument[0] == '\0')
	{
		send_to_char("Pray what?\n\r", ch);
		return;
	}

	if (IS_SET(ch->comm, COMM_NOCHANNELS))
	{
		send_to_char("You can't pray.\n\r", ch);
		return;
	}

	check = str_dup(argument);

	WAIT_STATE(ch, 10);
	send_to_char("You pray to the heavens!\n\r", ch);

	for (d = descriptor_list; d != NULL; d = d->next)
	{
		CHAR_DATA *victim;

		victim = d->original ? d->original : d->character;

		if (d->connected == CON_PLAYING &&
			d->character != ch &&
			!IS_SET(victim->comm, COMM_SHOUTSOFF) &&
			!IS_SET(victim->comm, COMM_QUIET) &&
			victim->level >= 53)
		{
			sprintf(buf, "{R%s : %s [%ld] is PRAYing for: %s{x\n\r",
					timestamp(), ch->original_name, ch->in_room->vnum, argument);
			send_to_char(buf, d->character);
		}
	}

	return;
}

void do_tell(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
	int count, number;
	char arg2[MAX_STRING_LENGTH];

	if (is_affected(ch, gsn_silence))
	{
		send_to_char("You cannot find the words within you.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_tigerclaw))
	{
		send_to_char("Your throat constricts too painfully.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_gag))
	{
		send_to_char("You find difficultly talking.\n\r", ch);
		return;
	}

	if (IS_SET(ch->comm, COMM_NOTELL) || IS_SET(ch->comm, COMM_DEAF))
	{
		send_to_char("Your message didn't get through.\n\r", ch);
		return;
	}

	if (IS_SET(ch->comm, COMM_QUIET))
	{
		send_to_char("You must turn off quiet mode first.\n\r", ch);
		return;
	}

	if (IS_SET(ch->comm, COMM_DEAF))
	{
		send_to_char("You must turn off deaf mode first.\n\r", ch);
		return;
	}

	argument = one_argument(argument, arg);

	if (arg[0] == '\0' || argument[0] == '\0')
	{
		send_to_char("Tell whom what?\n\r", ch);
		return;
	}

	arg2[0] = '\0';
	sprintf(arg2, "%s", arg);

	/*
	 * Can tell to PC's anywhere, but NPC's only in same room.
	 * -- Furey
	 */

	number = number_argument(arg2, arg);
	count = 0;

	for (victim = char_list; victim != NULL; victim = victim->next)
	{
		if (is_name(victim->name, arg) && (IS_NPC(victim) && victim->pIndexData->vnum == MOB_VNUM_DECOY))
			continue;
		if (victim->in_room == NULL || (IS_IMMORTAL(ch) ? !is_name(arg, (victim->original_name ? victim->original_name : victim->name)) : !is_name(arg, victim->name)) || !can_see(ch, victim))
			continue;
		if (++count == number)
			break;
	}

	if (victim == NULL || (IS_NPC(victim) && victim->in_room != ch->in_room))
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (victim->desc == NULL && !IS_NPC(victim))
	{
		act("$N seems to have lost consciousness...try again later.", ch, NULL, victim, TO_CHAR);
		sprintf(buf, "%s tells you '%s'\n\r", PERS(ch, victim), argument);
		buf[0] = UPPER(buf[0]);
		add_buf(victim->pcdata->buffer, buf);
		return;
	}

	if (!(IS_IMMORTAL(ch)) && is_affected(victim, gsn_deafen))
	{
		act("$E can't hear you.", ch, 0, victim, TO_CHAR);
		return;
	}

	if (!(IS_IMMORTAL(ch) && ch->level > LEVEL_IMMORTAL) && !IS_AWAKE(victim))
	{
		act("$E can't hear you.", ch, 0, victim, TO_CHAR);
		return;
	}
	if (is_affected(victim, gsn_shock_sphere))
	{
		act("$E can't hear you.", ch, 0, victim, TO_CHAR);
		return;
	}

	if ((IS_SET(victim->comm, COMM_QUIET) || IS_SET(victim->comm, COMM_DEAF)) && !IS_IMMORTAL(ch))
	{
		act("$E is not receiving tells.", ch, 0, victim, TO_CHAR);
		return;
	}

	argument = garble(ch, argument);

	if (is_affected(ch, gsn_deafen))
	{
		argument = str_dup(upstring(argument));
	}

	if (IS_IMMORTAL(ch))
		act("You tell $F '{g$t{x'", ch, argument, victim, TO_CHAR);
	else
		act("You tell $N '{g$t{x'", ch, argument, victim, TO_CHAR);

	if (IS_IMMORTAL(victim))
		act_new("$f tells you '{g$t{x'", ch, argument, victim, TO_VICT, POS_DEAD);
	else
		act_new("$n tells you '{g$t{x'", ch, argument, victim, TO_VICT, POS_DEAD);

	victim->reply = ch;

	return;
}

void do_reply(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	char buf[MAX_STRING_LENGTH];

	if (IS_SET(ch->comm, COMM_NOTELL))
	{
		send_to_char("Your message didn't get through.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_tigerclaw))
	{
		send_to_char("Your throat constricts too painfully.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_gag))
	{
		send_to_char("You find difficultly talking.\n\r", ch);
		return;
	}

	if ((victim = ch->reply) == NULL)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (victim->desc == NULL && !IS_NPC(victim))
	{
		act("$N seems to have misplaced $S link...try again later.", ch, NULL, victim, TO_CHAR);
		sprintf(buf, "%s tells you '%s'\n\r", PERS(ch, victim), argument);
		buf[0] = UPPER(buf[0]);
		add_buf(victim->pcdata->buffer, buf);
		return;
	}

	if (!IS_IMMORTAL(ch) && !IS_AWAKE(victim))
	{
		act("$E can't hear you.", ch, 0, victim, TO_CHAR);
		return;
	}

	if (!(IS_IMMORTAL(ch)) && is_affected(victim, gsn_deafen))
	{
		act("$E can't hear you.", ch, 0, victim, TO_CHAR);
		return;
	}

	if ((IS_SET(victim->comm, COMM_QUIET) || IS_SET(victim->comm, COMM_DEAF)) && !IS_IMMORTAL(ch) && !IS_IMMORTAL(victim))
	{
		act_new("$E is not receiving tells.", ch, 0, victim, TO_CHAR, POS_DEAD);
		return;
	}

	if (!IS_IMMORTAL(victim) && !IS_AWAKE(ch))
	{
		send_to_char("In your dreams, or what?\n\r", ch);
		return;
	}

	if (IS_SET(victim->comm, COMM_AFK))
	{
		if (IS_NPC(victim))
		{
			act_new("$E is AFK, and not receiving tells.", ch, NULL, victim, TO_CHAR, POS_DEAD);
			return;
		}

		act_new("$E is AFK, but your tell will go through when $E returns.", ch, NULL, victim, TO_CHAR, POS_DEAD);
		sprintf(buf, "%s tells you '%s'\n\r", PERS(ch, victim), argument);
		buf[0] = UPPER(buf[0]);
		add_buf(victim->pcdata->buffer, buf);
		return;
	}

	argument = garble(ch, argument);

	if (is_affected(ch, gsn_deafen))
	{
		argument = str_dup(upstring(argument));
	}

	if (IS_IMMORTAL(ch))
		act("You tell $F '{g$t{x'", ch, argument, victim, TO_CHAR);
	else
		act("You tell $N '{g$t{x'", ch, argument, victim, TO_CHAR);

	if (IS_IMMORTAL(victim))
		act_new("$f tells you '{g$t{x'", ch, argument, victim, TO_VICT, POS_DEAD);
	else
		act_new("$n tells you '{g$t{x'", ch, argument, victim, TO_VICT, POS_DEAD);

	victim->reply = ch;

	return;
}

void do_yell(CHAR_DATA *ch, char *argument)
{
	DESCRIPTOR_DATA *d;
	char buf[MAX_STRING_LENGTH];

	if (is_affected(ch, gsn_tigerclaw))
	{
		send_to_char("Your throat constricts too painfully.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_silence))
	{
		send_to_char("You cannot find the words within you.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_gag))
	{
		send_to_char("You find difficultly talking.\n\r", ch);
		return;
	}

	if (IS_SET(ch->comm, COMM_NOSHOUT))
	{
		send_to_char("You can't yell.\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char("Yell what?\n\r", ch);
		return;
	}

	argument = garble(ch, argument);

	if (is_affected(ch, gsn_deafen))
	{
		argument = str_dup(upstring(argument));
	}

	act("You yell '{c$t{x'", ch, argument, NULL, TO_CHAR);

	for (d = descriptor_list; d != NULL; d = d->next)
	{
		if (d->connected == CON_PLAYING && d->character != ch && d->character->in_room != NULL && d->character->in_room->area == ch->in_room->area && !IS_SET(d->character->comm, COMM_QUIET))
		{
			if (is_affected(d->character, gsn_shock_sphere))
				continue;
			if (IS_IMMORTAL(d->character) && is_affected(ch, gsn_cloak_form))
			{
				sprintf(buf, "A %s [%s] yells '{c%s{x'\n\r", ch->name, ch->original_name, argument);
				send_to_char(buf, d->character);
			}
			else
				act("$n yells '{c$t{x'", ch, argument, d->character, TO_VICT);
		}
	}
	return;
}

void do_myell(CHAR_DATA *ch, char *argument)
{
	DESCRIPTOR_DATA *d;

	if (ch->invis_level >= LEVEL_HERO)
		return;

	if (IS_SET(ch->comm, COMM_NOSHOUT))
	{
		send_to_char("You can't yell.\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char("Yell what?\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_tigerclaw))
	{
		send_to_char("Your throat constricts too painfully.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_gag))
	{
		send_to_char("You find difficultly talking.\n\r", ch);
		return;
	}

	argument = garble(ch, argument);

	if (is_affected(ch, gsn_deafen))
	{
		argument = str_dup(upstring(argument));
	}

	act("You yell '{c$t{x'", ch, argument, NULL, TO_CHAR);

	for (d = descriptor_list; d != NULL; d = d->next)
	{
		if (d->connected == CON_PLAYING && d->character != ch && d->character->in_room != NULL && d->character->in_room->area == ch->in_room->area && !IS_SET(d->character->comm, COMM_QUIET))
		{
			if (d->character->cabal != CABAL_ENFORCER)
			{
				argument = garble(ch, argument);
				act("$n yells '{c$t{x'", ch, argument, d->character, TO_VICT);
			}
			else
			{
				act("$n yells in panic '{c$t{x'", ch, argument, d->character, TO_VICT);
			}
		}
	}
	return;
}

void do_emote(CHAR_DATA *ch, char *argument)
{
	char buffer[MAX_STRING_LENGTH * 2];

	if (!IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE))
	{
		send_to_char("You can't show your emotions.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_gag))
	{
		send_to_char("You find difficultly talking.\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char("Emote what?\n\r", ch);
		return;
	}

	argument = garble(ch, argument);
	if (IS_IMMORTAL(ch))
	{
		colorconv(buffer, argument, ch);
	}
	else
	{
		strcpy(buffer, argument);
	}

	MOBtrigger = FALSE;
	act("$n $T", ch, NULL, buffer, TO_ROOM);
	act("$n $T", ch, NULL, buffer, TO_CHAR);
	MOBtrigger = TRUE;
	return;
}

void do_pmote(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *vch;
	char *letter, *name;
	char last[MAX_INPUT_LENGTH], temp[MAX_STRING_LENGTH];
	int matches = 0;

	if (!IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE))
	{
		send_to_char("You can't show your emotions.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_gag))
	{
		send_to_char("You find difficultly talking.\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char("Emote what?\n\r", ch);
		return;
	}

	argument = garble(ch, argument);
	act("$n $t", ch, argument, NULL, TO_CHAR);

	for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
	{
		if (vch->desc == NULL || vch == ch)
			continue;

		argument = garble(ch, argument);
		if ((letter = strstr(argument, vch->name)) == NULL)
		{
			MOBtrigger = FALSE;
			act("$N $t", vch, argument, ch, TO_CHAR);
			MOBtrigger = TRUE;
			continue;
		}

		strcpy(temp, argument);
		temp[strlen(argument) - strlen(letter)] = '\0';
		last[0] = '\0';
		name = vch->name;

		for (; *letter != '\0'; letter++)
		{
			if (*letter == '\'' && matches == strlen(vch->name))
			{
				strcat(temp, "r");
				continue;
			}

			if (*letter == 's' && matches == strlen(vch->name))
			{
				matches = 0;
				continue;
			}

			if (matches == strlen(vch->name))
			{
				matches = 0;
			}

			if (*letter == *name)
			{
				matches++;
				name++;
				if (matches == strlen(vch->name))
				{
					strcat(temp, "you");
					last[0] = '\0';
					name = vch->name;
					continue;
				}
				strncat(last, letter, 1);
				continue;
			}

			matches = 0;
			strcat(temp, last);
			strncat(temp, letter, 1);
			last[0] = '\0';
			name = vch->name;
		}

		argument = garble(ch, argument);
		MOBtrigger = FALSE;
		act("$N $t", vch, temp, ch, TO_CHAR);
		MOBtrigger = TRUE;
	}

	return;
}

/*
 * All the posing stuff.
 */
struct pose_table_type
{
	char *message[2 * MAX_CLASS];
};

const struct pose_table_type pose_table[] =
	{
		{{"You sizzle with energy.",
		  "$n sizzles with energy.",
		  "You feel very holy.",
		  "$n looks very holy.",
		  "You perform a small card trick.",
		  "$n performs a small card trick.",
		  "You show your bulging muscles.",
		  "$n shows $s bulging muscles."}},

		{{"You turn into a butterfly, then return to your normal shape.",
		  "$n turns into a butterfly, then returns to $s normal shape.",
		  "You nonchalantly turn wine into water.",
		  "$n nonchalantly turns wine into water.",
		  "You wiggle your ears alternately.",
		  "$n wiggles $s ears alternately.",
		  "You crack nuts between your fingers.",
		  "$n cracks nuts between $s fingers."}},

		{{"Blue sparks fly from your fingers.",
		  "Blue sparks fly from $n's fingers.",
		  "A halo appears over your head.",
		  "A halo appears over $n's head.",
		  "You nimbly tie yourself into a knot.",
		  "$n nimbly ties $mself into a knot.",
		  "You grizzle your teeth and look mean.",
		  "$n grizzles $s teeth and looks mean."}},

		{{"Little red lights dance in your eyes.",
		  "Little red lights dance in $n's eyes.",
		  "You recite words of wisdom.",
		  "$n recites words of wisdom.",
		  "You juggle with daggers, apples, and eyeballs.",
		  "$n juggles with daggers, apples, and eyeballs.",
		  "You hit your head, and your eyes roll.",
		  "$n hits $s head, and $s eyes roll."}},

		{{"A slimy green monster appears before you and bows.",
		  "A slimy green monster appears before $n and bows.",
		  "Deep in prayer, you levitate.",
		  "Deep in prayer, $n levitates.",
		  "You steal the underwear off every person in the room.",
		  "Your underwear is gone!  $n stole it!",
		  "Crunch, crunch -- you munch a bottle.",
		  "Crunch, crunch -- $n munches a bottle."}},

		{{"You turn everybody into a little pink elephant.",
		  "You are turned into a little pink elephant by $n.",
		  "An angel consults you.",
		  "An angel consults $n.",
		  "The dice roll ... and you win again.",
		  "The dice roll ... and $n wins again.",
		  "... 98, 99, 100 ... you do pushups.",
		  "... 98, 99, 100 ... $n does pushups."}},

		{{"A small ball of light dances on your fingertips.",
		  "A small ball of light dances on $n's fingertips.",
		  "Your body glows with an unearthly light.",
		  "$n's body glows with an unearthly light.",
		  "You count the money in everyone's pockets.",
		  "Check your money, $n is counting it.",
		  "The gods admire your physique.",
		  "The gods admire $n's physique."}},

		{{"Smoke and fumes leak from your nostrils.",
		  "Smoke and fumes leak from $n's nostrils.",
		  "A spot light hits you.",
		  "A spot light hits $n.",
		  "You balance a pocket knife on your tongue.",
		  "$n balances a pocket knife on your tongue.",
		  "Watch your feet, you are juggling granite boulders.",
		  "Watch your feet, $n is juggling granite boulders."}},

		{{"The light flickers as you rap in magical languages.",
		  "The light flickers as $n raps in magical languages.",
		  "Everyone levitates as you pray.",
		  "You levitate as $n prays.",
		  "You produce a coin from everyone's ear.",
		  "$n produces a coin from your ear.",
		  "Oomph!  You squeeze water out of a granite boulder.",
		  "Oomph!  $n squeezes water out of a granite boulder."}},

		{{"Your head disappears.",
		  "$n's head disappears.",
		  "A cool breeze refreshes you.",
		  "A cool breeze refreshes $n.",
		  "You step behind your shadow.",
		  "$n steps behind $s shadow.",
		  "You pick your teeth with a spear.",
		  "$n picks $s teeth with a spear."}},

		{{"A fire elemental singes your hair.",
		  "A fire elemental singes $n's hair.",
		  "The sun pierces through the clouds to illuminate you.",
		  "The sun pierces through the clouds to illuminate $n.",
		  "Your eyes dance with greed.",
		  "$n's eyes dance with greed.",
		  "Everyone is swept off their foot by your hug.",
		  "You are swept off your feet by $n's hug."}},

		{{"The sky changes color to match your eyes.",
		  "The sky changes color to match $n's eyes.",
		  "The ocean parts before you.",
		  "The ocean parts before $n.",
		  "You deftly steal everyone's weapon.",
		  "$n deftly steals your weapon.",
		  "Your karate chop splits a tree.",
		  "$n's karate chop splits a tree."}},

		{{"The stones dance to your command.",
		  "The stones dance to $n's command.",
		  "A thunder cloud kneels to you.",
		  "A thunder cloud kneels to $n.",
		  "The Grey Mouser buys you a beer.",
		  "The Grey Mouser buys $n a beer.",
		  "A strap of your armor breaks over your mighty thews.",
		  "A strap of $n's armor breaks over $s mighty thews."}},

		{{"The heavens and grass change colour as you smile.",
		  "The heavens and grass change colour as $n smiles.",
		  "The Burning Man speaks to you.",
		  "The Burning Man speaks to $n.",
		  "Everyone's pocket explodes with your fireworks.",
		  "Your pocket explodes with $n's fireworks.",
		  "A boulder cracks at your frown.",
		  "A boulder cracks at $n's frown."}},

		{{"Everyone's clothes are transparent, and you are laughing.",
		  "Your clothes are transparent, and $n is laughing.",
		  "An eye in a pyramid winks at you.",
		  "An eye in a pyramid winks at $n.",
		  "Everyone discovers your dagger a centimeter from their eye.",
		  "You discover $n's dagger a centimeter from your eye.",
		  "Mercenaries arrive to do your bidding.",
		  "Mercenaries arrive to do $n's bidding."}},

		{{"A black hole swallows you.",
		  "A black hole swallows $n.",
		  "Valentine Michael Smith offers you a glass of water.",
		  "Valentine Michael Smith offers $n a glass of water.",
		  "Where did you go?",
		  "Where did $n go?",
		  "Four matched Percherons bring in your chariot.",
		  "Four matched Percherons bring in $n's chariot."}},

		{{"The world shimmers in time with your whistling.",
		  "The world shimmers in time with $n's whistling.",
		  "The great god Mota gives you a staff.",
		  "The great god Mota gives $n a staff.",
		  "Click.",
		  "Click.",
		  "Atlas asks you to relieve him.",
		  "Atlas asks $n to relieve him."}}};

void do_pose(CHAR_DATA *ch, char *argument)
{
	int level;
	int pose;

	if (IS_NPC(ch))
		return;

	level = UMIN(ch->level, sizeof(pose_table) / sizeof(pose_table[0]) - 1);
	pose = number_range(0, level);

	act(pose_table[pose].message[2 * ch->class + 0], ch, NULL, NULL, TO_CHAR);
	act(pose_table[pose].message[2 * ch->class + 1], ch, NULL, NULL, TO_ROOM);

	return;
}

void do_bug(CHAR_DATA *ch, char *argument)
{
	append_file(ch, BUG_FILE, argument);
	send_to_char("Bug logged.\n\r", ch);
	return;
}

void do_typo(CHAR_DATA *ch, char *argument)
{
	append_file(ch, TYPO_FILE, argument);
	send_to_char("Typo logged.\n\r", ch);
	return;
}

void do_rent(CHAR_DATA *ch, char *argument)
{
	send_to_char("There is no rent here.  Just save and quit.\n\r", ch);
	return;
}

void do_qui(CHAR_DATA *ch, char *argument)
{
	send_to_char("If you want to QUIT, you have to spell it out.\n\r", ch);
	return;
}

void do_quit(CHAR_DATA *ch, char *argument)
{
	do_quit_new(ch, argument, FALSE);
	return;
}

void do_quit_new(CHAR_DATA *ch, char *argument, bool autoq)
{
	DESCRIPTOR_DATA *d, *d_next;
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;
	int id;
	CHAR_DATA *wch;
	CHAR_DATA *wch_next;

	if (IS_NPC(ch))
		return;

	if (!autoq && IS_AFFECTED(ch, AFF_CHARM))
	{
		send_to_char("Now why would you want to leave your master?\n\r", ch);
		return;
	}

	if (!autoq && arena)
	{
		send_to_char("You can't quit now, the Arena is running!\n\r", ch);
		return;
	}

	if (!autoq && ch->position == POS_FIGHTING)
	{
		send_to_char("No way! You are fighting.\n\r", ch);
		return;
	}

	if (!autoq && ch->position < POS_STUNNED)
	{
		send_to_char("You're not DEAD yet.\n\r", ch);
		return;
	}

	if (!autoq && ch->pause > 0)
	{
		send_to_char("Your adrenaline is gushing too much to quit!\n\r", ch);
		if (!IS_IMMORTAL(ch))
			return;
	}

	if (!autoq && (is_affected(ch, gsn_blackjack) || IS_AFFECTED(ch, AFF_SLEEP)))
	{
		send_to_char("You can't quit!\n\r", ch);
		return;
	}

	if (is_affected(ch, skill_lookup("channel")))
	{
		affect_strip(ch, skill_lookup("channel"));
	}

	if (is_affected(ch, skill_lookup("fortitude")))
	{
		affect_strip(ch, skill_lookup("fortitude"));
	}

	if (!autoq && !IS_SET(ch->in_room->room_flags, ROOM_LAW) && ch->in_room->cabal == 0 && !IS_IMMORTAL(ch))
	{
		send_to_char("You don't feel safe enough to quit here.\n\r", ch);
		return;
	}

	if (!autoq && ch->in_room->cabal != 0 && !IS_IMMORTAL(ch))
	{
		if (ch->in_room->cabal != ch->cabal)
		{
			send_to_char("You can't quit out in an enemy Cabal!\n\r", ch);
			return;
		}
	}

	for (obj = object_list; obj != NULL; obj = obj_next)
	{
		obj_next = obj->next;
		if (isCabalItem(obj))
		{
			if (!autoq && obj->carried_by != NULL && obj->carried_by == ch)
			{
				act("You cannot quit with cabal items in your inventory!", ch, obj, 0, TO_CHAR);
				return;
			}
		}
	}

	if (ch->cabal != 0 && ch->pcdata->host)
		announce_logout(ch);

	send_to_char("Alas, all good things must come to an end.\n\r", ch);
	act("$n has left the realm.", ch, NULL, NULL, TO_ROOM);

	sprintf(log_buf, "%s has %squit. [%ld minutes played, %d objects %d limited objects, Room %ld]",
			ch->original_name,
			autoq ? "auto" : "",
			((current_time - ch->logon) / 60),
			count_carried(ch, FALSE),
			count_carried(ch, TRUE),
			ch->in_room->vnum);

	log_string(log_buf);
	wiznet(log_buf, ch, NULL, WIZ_LOGINS, 0, get_trust(ch));

	if (autoq)
		login_log(ch, LTYPE_AUTO);
	else
		login_log(ch, LTYPE_OUT);

	/*
	 * After extract_char the ch is no longer valid!
	 */

	for (obj = object_list; obj != NULL; obj = obj_next)
	{
		obj_next = obj->next;
		if (obj->pIndexData->vnum == 1231)
		{
			if (obj->carried_by != NULL && obj->carried_by == ch)
				extract_obj(obj);
		}
		if (isCabalItem(obj))
		{
			if (obj->carried_by != NULL && obj->carried_by == ch)
			{
				extract_obj(obj);
				n_logf("Extracting (cabal item) object %ld from %s.", obj->pIndexData->vnum, ch->original_name);
			}
		}
		else if (obj->pIndexData->limtotal != 0 && ch->level < 10)
		{
			if (obj->carried_by == ch)
			{
				extract_obj(obj);
				n_logf("Extracting limited object %ld from %s, player level under 10.", obj->pIndexData->vnum, ch->original_name);
			}
		}
		/* this was taking any unholy blessed weapon away.  removed --daed*/
	}

	for (wch = char_list; wch != NULL; wch = wch_next)
	{
		wch_next = wch->next;
		if (wch->defending != NULL && wch->defending == ch)
			wch->defending = NULL;

		if (!IS_NPC(wch))
			continue;
		if ((wch->pIndexData->vnum == MOB_VNUM_DECOY || wch->pIndexData->vnum == MOB_VNUM_SHADOW) && is_name(ch->name, wch->name))
		{
			act("$n crumbles to dust.", wch, 0, 0, TO_ROOM);
			extract_char(wch, TRUE);
		}
	}

	save_char_obj(ch);
	id = ch->id;
	d = ch->desc;
	extract_char(ch, TRUE);
	if (d != NULL)
		close_socket(d);

	/* toast evil cheating bastards */
	for (d = descriptor_list; d != NULL; d = d_next)
	{
		CHAR_DATA *tch;

		d_next = d->next;
		tch = d->original ? d->original : d->character;
		if (tch && tch->id == id)
		{
			extract_char(tch, TRUE);
			close_socket(d);
		}
	}

	return;
}

void do_save(CHAR_DATA *ch, char *argument)
{
	if (IS_NPC(ch))
		return;

	if (is_affected(ch, skill_lookup("age death")))
		return send_to_char("You are dead; you cannot be saved.\n\r", ch);

	send_to_char("Saving. Remember that KBK has automatic saving.\n\r", ch);
	WAIT_STATE(ch, 1 * PULSE_VIOLENCE);
	if (IS_NPC(ch) || ch->pcdata->save_timer)
		return;

	saveCharmed(ch);
	save_char_obj(ch);

	ch->pcdata->save_timer = 2;
	return;
}

void do_follow(CHAR_DATA *ch, char *argument)
{
	/* RT changed to allow unlimited following and follow the NOFOLLOW rules */
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;

	one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		send_to_char("Follow whom?\n\r", ch);
		return;
	}

	if ((victim = get_char_room(ch, arg)) == NULL)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (IS_AFFECTED(ch, AFF_CHARM) && ch->master != NULL)
	{
		act("But you'd rather follow $N!", ch, NULL, ch->master, TO_CHAR);
		return;
	}

	if (victim == ch)
	{
		if (ch->master == NULL)
		{
			send_to_char("You already follow yourself.\n\r", ch);
			return;
		}
		stop_follower(ch);
		return;
	}

	if (!IS_NPC(victim) && IS_SET(victim->act, PLR_NOFOLLOW) && !IS_IMMORTAL(ch))
	{
		act("$N doesn't seem to want any followers.\n\r",
			ch, NULL, victim, TO_CHAR);
		return;
	}

	REMOVE_BIT(ch->act, PLR_NOFOLLOW);

	if (ch->master != NULL)
		stop_follower(ch);

	add_follower(ch, victim);
	return;
}

void add_follower(CHAR_DATA *ch, CHAR_DATA *master)
{
	if (ch->master != NULL)
	{
		bug("Add_follower: non-null master.", 0);
		return;
	}

	ch->master = master;
	ch->leader = NULL;

	if (can_see(master, ch))
		act("$n now follows you.", ch, NULL, master, TO_VICT);

	act("You now follow $N.", ch, NULL, master, TO_CHAR);

	return;
}

void stop_follower(CHAR_DATA *ch)
{
	if (ch->master == NULL)
	{
		bug("Stop_follower: null master.", 0);
		return;
	}

	if (IS_AFFECTED(ch, AFF_CHARM))
	{
		REMOVE_BIT(ch->affected_by, AFF_CHARM);
		affect_strip(ch, gsn_charm_person);
	}

	if (can_see(ch->master, ch) && ch->in_room != NULL)
	{
		act("$n stops following you.", ch, NULL, ch->master, TO_VICT);
		act("You stop following $N.", ch, NULL, ch->master, TO_CHAR);
	}

	ch->master = NULL;
	ch->leader = NULL;
	return;
}

void die_follower(CHAR_DATA *ch)
{
	CHAR_DATA *fch;
	CHAR_DATA *fch_next;

	if (ch->master != NULL)
	{
		stop_follower(ch);
	}

	ch->leader = NULL;

	for (fch = char_list; fch != NULL; fch = fch_next)
	{
		/* NPC fading added by Ceran...you'll have to keep this in cos tracking
		crashes if you don't clear all trackers and followers.
		*/
		fch_next = fch->next;
		if ((IS_NPC(fch)) && ((is_affected(fch, gsn_animate_dead)) || (IS_AFFECTED(fch, AFF_CHARM))))
		{
			if (fch->master == ch)
			{
				REMOVE_BIT(fch->affected_by, AFF_CHARM);
				affect_strip(fch, gsn_animate_dead);
				act("$n slowly fades away.", fch, NULL, NULL, TO_ROOM);
				extract_char(fch, TRUE);
			}
		}
		else
		{
			if (fch->master == ch)
				stop_follower(fch);
			if (fch->leader == ch)
				fch->leader = fch;
		}
	}

	return;
}

void do_order(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char arg[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	CHAR_DATA *och;
	CHAR_DATA *och_next;
	bool found;
	bool fAll;

	argument = one_argument(argument, arg);
	one_argument(argument, arg2);

	if (!str_cmp(arg2, "remove") || !str_cmp(arg, "rem"))
	{
		send_to_char("Don't do that!\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_tigerclaw))
	{
		send_to_char("Your throat constricts too painfully.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_gag))
	{
		send_to_char("You find difficultly talking.\n\r", ch);
		return;
	}

	if (!str_cmp(arg2, "delete") || !str_cmp(arg2, "mob"))
	{
		send_to_char("That will NOT be done.\n\r", ch);
		return;
	}

	if (arg[0] == '\0' || argument[0] == '\0')
	{
		send_to_char("Order whom to do what?\n\r", ch);
		return;
	}

	if (IS_AFFECTED(ch, AFF_CHARM))
	{
		send_to_char("You feel like taking, not giving, orders.\n\r", ch);
		return;
	}
	if (!str_cmp(arg2, "trip") || !str_cmp(arg2, "bash") || !str_cmp(arg2, "palm") || !str_cmp(arg2, "disarm") || !str_cmp(arg2, "crush") || !str_cmp(arg2, "throw") || !str_cmp(arg2, "throw"))
		return;

	if (!str_cmp(arg, "all"))
	{
		fAll = TRUE;
		victim = NULL;
	}
	else
	{
		fAll = FALSE;
		if ((victim = get_char_room(ch, arg)) == NULL)
		{
			send_to_char("They aren't here.\n\r", ch);
			return;
		}

		if (victim == ch)
		{
			send_to_char("Aye aye, right away!\n\r", ch);
			return;
		}

		if (!IS_AFFECTED(victim, AFF_CHARM) || victim->master != ch || (IS_IMMORTAL(victim) && victim->trust >= ch->trust))
		{
			send_to_char("Do it yourself!\n\r", ch);
			return;
		}
	}

	found = FALSE;

	for (och = ch->in_room->people; och != NULL; och = och_next)
	{
		och_next = och->next_in_room;

		if (IS_AFFECTED(och, AFF_CHARM) && och->master == ch && (fAll || och == victim))
		{
			found = TRUE;
			sprintf(buf, "$n orders you to '%s'.", argument);
			act(buf, ch, NULL, och, TO_VICT);
			interpret(och, argument);
		}
	}

	if (found)
	{
		WAIT_STATE(ch, PULSE_VIOLENCE);
		send_to_char("Ok.\n\r", ch);
	}
	else
		send_to_char("You have no followers here.\n\r", ch);
	return;
}

void do_group(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;

	one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		CHAR_DATA *gch;
		CHAR_DATA *leader;

		leader = (ch->leader != NULL) ? ch->leader : ch;
		sprintf(buf, "%s's group:\n\r", PERS(leader, ch));
		send_to_char(buf, ch);

		for (gch = char_list; gch != NULL; gch = gch->next)
		{
			if (is_same_group(gch, ch))
			{
				sprintf(buf,
						"[%2d %s] %-16s %3d%%hp %3d%%mana %3d%%mv %5d xp\n\r",
						gch->level,
						IS_NPC(gch) ? "Mob" : class_table[gch->class].who_name,
						capitalize(PERS(gch, ch)),
						(gch->max_hit == 0) ? 0 : (gch->hit * 100) / gch->max_hit,
						(gch->max_mana == 0) ? 0 : (gch->mana * 100) / gch->max_mana,
						(gch->max_move == 0) ? 0 : (gch->move * 100) / gch->max_move,
						gch->exp);
				send_to_char(buf, ch);
			}
		}
		return;
	}

	if ((victim = get_char_room(ch, arg)) == NULL)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (ch->master != NULL || (ch->leader != NULL && ch->leader != ch))
	{
		send_to_char("But you are following someone else!\n\r", ch);
		return;
	}

	if (victim->master != ch && ch != victim)
	{
		act_new("$N isn't following you.", ch, NULL, victim, TO_CHAR, POS_SLEEPING);
		return;
	}

	if (IS_AFFECTED(victim, AFF_CHARM))
	{
		send_to_char("You can't remove charmed mobs from your group.\n\r", ch);
		return;
	}

	if (IS_AFFECTED(ch, AFF_CHARM))
	{
		act_new("You like your master too much to leave $m!",
				ch, NULL, victim, TO_VICT, POS_SLEEPING);
		return;
	}

	if (is_same_group(victim, ch) && ch != victim)
	{
		victim->leader = NULL;
		act_new("$n removes $N from $s group.",
				ch, NULL, victim, TO_NOTVICT, POS_RESTING);
		act_new("$n removes you from $s group.",
				ch, NULL, victim, TO_VICT, POS_SLEEPING);
		act_new("You remove $N from your group.",
				ch, NULL, victim, TO_CHAR, POS_SLEEPING);
		return;
	}

	if (IS_GOOD(victim) && IS_EVIL(ch))
	{
		send_to_char("That person is too pure to group with you.\n\r", ch);
		return;
	}

	if (IS_EVIL(victim) && IS_GOOD(ch))
	{
		send_to_char("That person is too evil to group with you.\n\r", ch);
		return;
	}

	if ((ch->cabal == cabal_lookup("rager") && class_table[victim->class].ctype == CLASS_CASTER) || (victim->cabal == cabal_lookup("rager") && class_table[ch->class].ctype == CLASS_CASTER))
	{
		return send_to_char("Ragers and mages are incompatible as groupmates.\n\r", ch);
	}

	victim->leader = ch;
	act_new("$N joins $n's group.", ch, NULL, victim, TO_NOTVICT, POS_RESTING);
	act_new("You join $n's group.", ch, NULL, victim, TO_VICT, POS_SLEEPING);
	act_new("$N joins your group.", ch, NULL, victim, TO_CHAR, POS_SLEEPING);
	return;
}

void do_split(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char arg1[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH];
	CHAR_DATA *gch;
	int members;
	int amount_gold = 0;
	int share_gold;
	int extra_gold;

	argument = one_argument(argument, arg1);
	one_argument(argument, arg2);

	if (arg1[0] == '\0')
	{
		send_to_char("Split how much?\n\r", ch);
		return;
	}

	amount_gold = atoi(arg2);

	if (amount_gold < 0)
	{
		send_to_char("Your group wouldn't like that.\n\r", ch);
		return;
	}

	if (amount_gold == 0)
	{
		send_to_char("You hand out zero coins, but no one notices.\n\r", ch);
		return;
	}

	if (ch->gold < amount_gold)
	{
		send_to_char("You don't have that much to split.\n\r", ch);
		return;
	}

	members = 0;
	for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
	{
		if (is_same_group(gch, ch) && !IS_AFFECTED(gch, AFF_CHARM))
			members++;
	}

	if (members < 2)
	{
		send_to_char("Just keep it all.\n\r", ch);
		return;
	}

	share_gold = amount_gold / members;
	extra_gold = amount_gold % members;

	if (share_gold == 0)
	{
		send_to_char("Don't even bother, cheapskate.\n\r", ch);
		return;
	}

	ch->gold -= amount_gold;
	ch->gold += share_gold + extra_gold;

	if (share_gold > 0)
	{
		sprintf(buf,
				"You split %d gold coins. Your share is %d gold.\n\r",
				amount_gold, share_gold + extra_gold);
		send_to_char(buf, ch);
	}

	sprintf(buf, "$n splits %d gold coins. Your share is %d gold.",
			amount_gold, share_gold);

	for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
	{
		if (gch != ch && is_same_group(gch, ch) && !IS_AFFECTED(gch, AFF_CHARM))
		{
			act(buf, ch, NULL, gch, TO_VICT);
			gch->gold += share_gold;
		}
	}

	return;
}

void do_gtell(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *gch;

	if (is_affected(ch, gsn_silence))
	{
		send_to_char("You cannot find the words within you.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_gag))
	{
		send_to_char("You find difficultly talking.\n\r", ch);
		return;
	}

	if (argument[0] == '\0')
	{
		send_to_char("Tell your group what?\n\r", ch);
		return;
	}

	if (IS_SET(ch->comm, COMM_NOTELL))
	{
		send_to_char("Your message didn't get through!\n\r", ch);
		return;
	}

	argument = garble(ch, argument);

	if (is_affected(ch, gsn_deafen))
	{
		//		argument = upstring(ch,argument);
		argument = str_dup(upstring(argument));
	}

	for (gch = char_list; gch != NULL; gch = gch->next)
	{
		if (is_same_group(gch, ch))
		{
			act_new("$n tells the group '{m$t{x'",
					ch, argument, gch, TO_VICT, POS_SLEEPING);
		}
	}

	act_new("You tell the group '{m$t{x'", ch, argument, NULL, TO_CHAR, POS_SLEEPING);
	return;
}

/*
 * It is very important that this be an equivalence relation:
 * (1) A ~ A
 * (2) if A ~ B then B ~ A
 * (3) if A ~ B  and B ~ C, then A ~ C
 */
bool is_same_group(CHAR_DATA *ach, CHAR_DATA *bch)
{
	if (ach == NULL || bch == NULL)
		return FALSE;
	/*    if ( ( ach->level - bch->level > 8 || ach->level - bch->level < -8 ) && !IS_NPC(ach) )
		return FALSE;*/
	if (ach->leader != NULL)
	{
		if (IS_NPC(ach) && ach->leader->leader)
			ach = ach->leader->leader;
		else
			ach = ach->leader;
	}
	if (bch->leader != NULL)
	{
		if (IS_NPC(bch) && bch->leader->leader)
			bch = bch->leader->leader;
		else
			bch = bch->leader;
	}
	return ach == bch;
}

void do_release(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	char arg[MAX_INPUT_LENGTH];

	one_argument(argument, arg);
	if (arg[0] == '\0')
	{
		send_to_char("Release which follower?\n\r", ch);
		return;
	}

	victim = get_char_room(ch, arg);
	if (victim == NULL)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (!IS_AFFECTED(victim, AFF_CHARM) || victim->master != ch)
	{
		send_to_char("They aren't under your control.\n\r", ch);
		return;
	}

	if (IS_NPC(victim))
	{
		act("$n slowly fades away.", victim, 0, 0, TO_ROOM);
		extract_char(victim, TRUE);
	}
	else
	{
		send_to_char("You can't release a player into the void..maybe nofollow them?\n\r", ch);
		return;
	}
	return;
}

/* This is to introduce a basic random value in the ageing process. The
amount varies for each race since they live different lifespans .
*/
void get_age_mod(CHAR_DATA *ch)
{
	int mod;
	char *race;

	if (IS_NPC(ch))
		return;

	race = pc_race_table[ch->race].name;
	if (!str_cmp(race, "human"))
		mod = dice(2, 6);
	else if (!str_cmp(race, "elf") || !str_cmp(race, "svirfnebli") || !str_cmp(race, "dark-elf"))
	{
		mod = dice(6, 8);
		mod += dice(6, 10);
	}
	else if (!str_cmp(race, "cloud") || !str_cmp(race, "centaur") || !str_cmp(race, "troll"))
		mod = dice(4, 5);
	else if (!str_cmp(race, "gnome"))
	{
		mod = dice(6, 8);
		mod += dice(10, 11);
	}
	else if (!str_cmp(race, "dwarf"))
		mod = dice(5, 6);
	else
		mod = dice(4, 5);

	ch->pcdata->age_mod = mod;
	return;
}

/* A new auto setting so players can indicate whether they want to accept
objects sent via the transfer item spell.
*/
void do_notransfer(CHAR_DATA *ch, char *argument)
{
	if (IS_NPC(ch))
		return;

	if (IS_SET(ch->act, PLR_NO_TRANSFER))
	{
		REMOVE_BIT(ch->act, PLR_NO_TRANSFER);
		send_to_char("You now accept transfered objects.\n\r", ch);
		return;
	}

	SET_BIT(ch->act, PLR_NO_TRANSFER);
	send_to_char("You no longer accept transfered objects.\n\r", ch);
	return;
}

void do_evaluation(CHAR_DATA *ch, char *argument)
{
	if (IS_NPC(ch))
		return;

	if (IS_SET(ch->act, PLR_EVALUATION))
	{
		send_to_char("You no longer attempt to evaluate your opponent's wounds.\n\r", ch);
		REMOVE_BIT(ch->act, PLR_EVALUATION);
		return;
	}
	send_to_char("You now attempt to evaluate your opponent's wounds.\n\r", ch);
	SET_BIT(ch->act, PLR_EVALUATION);
	return;
}

void perm_death_log(CHAR_DATA *ch, int deltype)
{
	FILE *fp;
	int hours;

	hours = (int)(ch->played + current_time - ch->logon) / 3600;
	if (hours < 5)
		return;

	if ((fp = fopen(PDEATH_LOG_FILE, "a")) != NULL)
	{
		fprintf(fp, "%s||[%2d %5s %s]||%s%s%s%s%s||%d||%d||%d||%d||%d||%d||%d||%s",
				deltype == 1 ? "DELETED" : deltype == 2 ? "CON-DIE"
									   : deltype == 3	? "OLD-AGE"
														: "DELETED",
				ch->level,
				pc_race_table[ch->race].who_name,
				class_table[ch->class].who_name,
				cabal_table[ch->cabal].who_name,
				empire_table[ch->pcdata->empire + 1].who_name,
				(ch->original_name) ? ch->original_name : ch->name,
				IS_NPC(ch) ? "" : ch->pcdata->title,
				IS_NPC(ch) ? "" : (ch->pcdata->extitle) ? ch->pcdata->extitle
														: "",
				ch->pcdata->kills[PK_KILLS],
				ch->pcdata->kills[PK_GOOD],
				ch->pcdata->kills[PK_NEUTRAL],
				ch->pcdata->kills[PK_EVIL],
				ch->pcdata->killed[PK_KILLED],
				ch->pcdata->killed[MOB_KILLED],
				(int)(ch->played + current_time - ch->logon) / 3600,
				(char *)ctime(&current_time));

		fclose(fp);
	}
}

void temp_death_log(CHAR_DATA *killer, CHAR_DATA *dead)
{
	char wizbuf[MSL];

	if (IS_IMMORTAL(killer))
	{
		return;
	}

	if (IS_NPC(killer))
	{
		if (!IS_NPC(dead))
			dead->pcdata->killed[MOB_KILLED]++;
	}

	if (IS_NPC(killer) && (killer->master != NULL))
	{

		killer = killer->master;
	}

	if (IS_NPC(killer) || (IS_NPC(dead)))
	{
		return;
	}

	if (isNewbie(dead))
	{
		return;
	}

	if (killer->in_room == NULL)
		return;

	if (dead == killer)
		return;

	mysql_safe_query("INSERT INTO pklogs VALUES('%s', '%s', '%s', '%s')",
					 dead->original_name, killer->original_name, killer->in_room->name, (char *)ctime(&current_time));

	killer->pcdata->kills[PK_KILLS]++;
	dead->pcdata->killed[PK_KILLED]++;

	if (dead->alignment > 0)
	{
		killer->pcdata->kills[PK_GOOD]++;
	}
	else if (dead->alignment == 0)
	{
		killer->pcdata->kills[PK_NEUTRAL]++;
	}
	else if (dead->alignment < 0)
	{
		killer->pcdata->kills[PK_EVIL]++;
	}

	sprintf(wizbuf, "Recording PK Death [killer: %s victim: %s]", killer->original_name, dead->original_name);
	wiznet(wizbuf, NULL, NULL, WIZ_DEATHS, 0, 0);

	return;
}
