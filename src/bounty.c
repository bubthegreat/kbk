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
// Thanks to Dev for original Bounty Cabal code.

#include "include.h"

void bounty_log(char *string)
{
	FILE *fp;
	char *strtime;

	strtime = ctime(&current_time);
	strtime[strlen(strtime) - 1] = '\0';

	if ((fp = fopen(BOUNTY_LOG_FILE, "a")) != NULL)
	{
		fprintf(fp, "%s :: %s\n", strtime, string);
		fclose(fp);
	}
	return;
}

void do_credits(CHAR_DATA *ch, char *argument)
{
	char buf[500], buf2[500], buf3[500];
	CHAR_DATA *victim;
	int i = 0;
	if (IS_NPC(ch))
		return;
	if (!ch->pcdata->bounty_credits && ch->cabal != CABAL_BOUNTY && !IS_IMMORTAL(ch))
		return send_to_char("You are not in that cabal.\n\r", ch);
	if (IS_IMMORTAL(ch) && str_cmp(argument, ""))
	{
		argument = one_argument(argument, buf);
		argument = one_argument(argument, buf2);
		victim = get_char_world(ch, buf);
		if (!victim)
			return send_to_char("They aren't here.\n\r", ch);
		if (IS_NPC(victim))
			return send_to_char("Mobs have no bounty credits.\n\r", ch);
		if (buf != NULL && buf2[0] == '\0')
		{
			sprintf(buf3, "%s has %i bounty credits.\n\r", victim->name, victim->pcdata->bounty_credits);
			send_to_char(buf3, ch);
			return;
		}
		if (is_number(buf2))
		{
			victim->pcdata->bounty_credits = atoi(buf2);
			sprintf(buf3, "%s now has %i bounty credits.\n\r", victim->name, victim->pcdata->bounty_credits);
			send_to_char(buf3, ch);
			return;
		}
	}
	for (i = 0; i < MAX_BOUNTY; i++)
	{
		sprintf(buf, "%-15s - %i\n\r", capitalize(bounty_table[i].rank_name), bounty_table[i].credits_required);
		send_to_char(buf, ch);
	}
	sprintf(buf2, "You have %i bounty credits.\n\r", ch->pcdata->bounty_credits);
	send_to_char(buf2, ch);
}

void bounty_credit(CHAR_DATA *ch, int credit)
{
	char buf[300], buf2[300], blog[500];
	int i, old_credit = ch->pcdata->bounty_credits, gn, gns, sn;

	if (IS_NPC(ch))
		return;

	sprintf(buf, "You receive %i bounty credits for your kill!\n\r", credit);
	send_to_char(buf, ch);
	ch->pcdata->bounty_credits += credit;
	for (i = 1; i < MAX_BOUNTY; i++)
	{
		if (ch->pcdata->bounty_credits >= bounty_table[i].credits_required && old_credit < bounty_table[i].credits_required)
		{
			sprintf(buf, "You gain a title within the Bounty Guild, advancing to %s!\n\r", bounty_table[i].rank_extitle);
			send_to_char(buf, ch);
			sprintf(buf2, ", %s", bounty_table[i].rank_extitle);
			set_extitle(ch, buf2);
			group_add(ch, bounty_table[i].rank_name, FALSE);
			sprintf(buf2, "%s has advanced to the position of %s.", ch->name, bounty_table[i].rank_extitle);
			bounty_cb(buf2);
			sprintf(blog, "%s advances to %s.", ch->name, bounty_table[i].rank_extitle);
			bounty_log(blog);
			gn = group_lookup(bounty_table[i].rank_name);
			for (gns = 0; gns < MAX_SKILL; gns++)
			{
				if (group_table[gn].spells[gns] == NULL)
					break;
				sn = skill_lookup(group_table[gn].spells[gns]);
				if (skill_table[sn].skill_level[ch->class] >= LEVEL_HERO + 1)
					continue;
				ch->pcdata->learned[sn] = 70;
				sprintf(buf, "The bounty guildmaster teaches you %s.\n\r", skill_table[sn].name);
				send_to_char(buf, ch);
			}
		}
	}
}

void bounty_cb(char *string)
{
	CHAR_DATA *guardian;
	guardian = get_cabal_guardian(CABAL_BOUNTY);
	if (guardian)
		do_cb(guardian, string);
}

void do_topbounties(CHAR_DATA *ch, char *argument)
{
	int pnum = 0;
	SQL_ROW row;
	SQL_RES *res;

	send_to_char("      The Top Ten Most Wanted:\n\r", ch);

	res = sqlite_safe_query_with_result("SELECT * FROM bounties ORDER BY amount DESC LIMIT 10");
	if (res == NULL)
	{
		return send_to_char("Database error.\n\r", ch);
	}
	else if (sqlite_num_rows(res) == 0)
	{
		sqlite_free_result(res);
		return send_to_char("No bounties have been recorded.\n\r", ch);
	}
	else
	{
		while ((row = sqlite_fetch_row(res)) != NULL)
		{
			printf_to_char(ch, "%2i) %-15s %d gold\n\r", ++pnum, row[2], atoi(row[0]));
		}
		sqlite_free_result(res);
	}
	return;
}

void do_bounty(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	char arg1[MAX_STRING_LENGTH], arg2[MAX_STRING_LENGTH], tempstr[50], buf[200], buf2[200];
	int amount;
	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);
	if (arg1[0] == '\0' || arg2[0] == '\0' || (!is_number(arg2) && !IS_IMMORTAL(ch)))
	{
		send_to_char("Syntax:   bounty <character> <amount>\n\r", ch);
		send_to_char("Places a bounty of the specified amount on the life of the specified character.\n\r", ch);
		if (IS_IMMORTAL(ch))
		{
			send_to_char("		bounty <character> <clear>\n\r", ch);
			send_to_char("Clears the bounty on the specified character.\n\r", ch);
		}
		return;
	}
	if (!(victim = get_char_world(ch, arg1)))
		return send_to_char("They aren't here.\n\r", ch);
	if (IS_NPC(victim) || ch->cabal == CABAL_BOUNTY || ((IS_IMMORTAL(victim) || IS_HEROIMM(victim)) && !IS_IMMORTAL(ch)))
		return send_to_char("You can't do that.\n\r", ch);
	if (!is_number(arg2) && IS_IMMORTAL(ch) && !str_cmp(arg2, "clear"))
	{
		sprintf(tempstr, "%ld", victim->pcdata->bounty);
		act("The bounty on $N's head has been cleared. (formerly $t gold)", ch, tempstr, victim, TO_CHAR);
		victim->pcdata->bounty = 0;
		sqlite_safe_query("DELETE FROM bounties WHERE victim='%s'", victim->original_name);
		return;
	}
	if (!is_number(arg2))
		return send_to_char("Invalid amount.\n\r", ch);
	;
	amount = atoi(arg2);
	if (amount < 1000)
		return send_to_char("You must place a bounty of at least 1000 gold coins.\n\r", ch);
	if (amount > ch->gold)
		return send_to_char("You don't have enough money.\n\r", ch);
	if (victim == ch && !IS_IMMORTAL(ch))
		return send_to_char("Desperate for attention?\n\r", ch);
	victim->pcdata->bounty += amount;
	ch->gold -= amount;
	act("You have placed a bounty of $t gold on $N's head.", ch, arg2, victim, TO_CHAR);
	sprintf(buf, "Someone has placed a bounty of %i gold on the life of %s.", amount, victim->name);
	// process_bounty(victim->name,victim->pcdata->bounty);
	record_bounty(ch, victim, amount);
	bounty_cb(buf);
	sprintf(buf2, "%s places a bounty of %i gold on the life of %s.", ch->original_name, amount, victim->name);
	bounty_log(buf2);
}

void record_bounty(CHAR_DATA *ch, CHAR_DATA *victim, int amount)
{
	SQL_RES *res;

	if (!ch)
		return;
	if (!victim)
		return;

	n_logf("Recording bounty on %s for %d gold.", victim->original_name, amount);

	res = sqlite_safe_query_with_result("SELECT * FROM bounties WHERE victim='%s'", victim->original_name);

	if (res == NULL)
	{
		log_string("Record_bounty: NULL SQL_RES");
		return;
	}

	int f = sqlite_num_rows(res);

	n_logf("Rows: %d", f);

	if (f == 0)
	{
		sqlite_safe_query("INSERT INTO bounties VALUES(%d, '%s', '%s', %d)",
						 amount,
						 ch->original_name,
						 victim->original_name,
						 current_time);
	}
	else
	{
		sqlite_safe_query("UPDATE bounties SET amount=amount+%d WHERE victim='%s'", amount, victim->original_name);
	}
	sqlite_free_result(res);
	return;
}

void pay_bounty(CHAR_DATA *ch, CHAR_DATA *victim)
{
	/* CH is killer, VICTIM is deadguy */
	char sbounty[200];
	char buf[200];
	int credit, lostbounty;
	if (!victim->pcdata->bounty || ch->cabal != CABAL_BOUNTY)
		return;
	sprintf(sbounty, "You claim the bounty on %s's life, receiving %ld gold!\n\r", victim->name, victim->pcdata->bounty);
	send_to_char(sbounty, ch);
	ch->gold += victim->pcdata->bounty;
	credit = URANGE(3, victim->pcdata->bounty / 5000, 10);
	if (victim->cabal > 0)
		credit *= 1.6;
	if (victim->pcdata->induct || victim->pcdata->empire > EMPIRE_SLEADER)
		credit *= 2;
	if (victim->pcdata->bounty > 9999)
	{
		send_to_char("Wow, what a kill you bagged!\n\r", ch);
		credit = UMIN(credit, 25);
	}
	credit = URANGE(3, credit, 30);
	sprintf(sbounty, "%s collects the %ld gold bounty on %s.", ch->name, victim->pcdata->bounty, victim->name);
	wiznet(sbounty, NULL, NULL, WIZ_CABAL, 0, 0);
	sprintf(buf, "%s has collected the %ld gold bounty on the life of %s.", ch->name, victim->pcdata->bounty, victim->original_name);
	bounty_cb(buf);
	bounty_credit(ch, credit);
	bounty_log(sbounty);
	victim->pcdata->bounty = 0;
	if (victim->cabal == CABAL_BOUNTY && victim->pcdata->bounty_credits)
	{
		lostbounty = URANGE(2, credit, 5);
		sprintf(sbounty, "The guild takes %i of your credits to pay %s.\n\r", lostbounty, ch->name);
		send_to_char(sbounty, victim);
		victim->pcdata->bounty_credits -= URANGE(0, lostbounty, 5);
	}
	// process_bounty(victim->name,victim->pcdata->bounty);
	sqlite_safe_query("DELETE FROM bounties WHERE victim='%s'", victim->original_name);
	return;
}

void spell_hire_mercenary(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	CHAR_DATA *merc;
	char buf[350];
	int merc_vnum = -1, amount;
	if (cabal_down(ch, CABAL_BOUNTY) || IS_NPC(ch))
		return;
	if (is_affected(ch, sn))
		return send_to_char("You cannot hire another mercenary yet.\n\r", ch);
	if (!str_cmp(target_name, ""))
	{
		send_to_char("You can call the following mercenaries:\n\r", ch);
		if (ch->pcdata->bounty_credits >= bounty_table[0].credits_required)
			send_to_char("warrior       - 40 gold\n\r", ch);
		if (ch->pcdata->bounty_credits >= bounty_table[1].credits_required)
			send_to_char("thief         - 60 gold\n\r", ch);
		if (ch->pcdata->bounty_credits >= bounty_table[2].credits_required)
			send_to_char("assassin      - 80 gold\n\r", ch);
		if (ch->pcdata->bounty_credits >= bounty_table[3].credits_required)
			send_to_char("healer        - 100 gold\n\r", ch);
		return;
	}
	for (merc = char_list; merc != NULL; merc = merc->next)
		if (IS_NPC(merc) && merc->pIndexData->vnum >= MOB_VNUM_WARRIOR_MERCENARY &&
			merc->pIndexData->vnum <= MOB_VNUM_SHAMAN_MERCENARY && merc->master == ch)
			break;
	if (merc)
		return send_to_char("You already have a mercenary.\n\r", ch);
	if (!str_cmp(capitalize(target_name), "Warrior") && ch->pcdata->bounty_credits >= bounty_table[0].credits_required)
		merc_vnum = MOB_VNUM_WARRIOR_MERCENARY;
	if (!str_cmp(capitalize(target_name), "Thief") && ch->pcdata->bounty_credits >= bounty_table[1].credits_required)
		merc_vnum = MOB_VNUM_THIEF_MERCENARY;
	if (!str_cmp(capitalize(target_name), "Assassin") && ch->pcdata->bounty_credits >= bounty_table[2].credits_required)
		merc_vnum = MOB_VNUM_ASSASSIN_MERCENARY;
	if (!str_cmp(capitalize(target_name), "Healer") && ch->pcdata->bounty_credits >= bounty_table[3].credits_required)
		merc_vnum = MOB_VNUM_SHAMAN_MERCENARY;
	if (merc_vnum < 0)
		return send_to_char("Invalid mercenary choice.\n\r", ch);
	merc = create_mobile(get_mob_index(merc_vnum));
	amount = 20 + (1 + (merc_vnum - MOB_VNUM_WARRIOR_MERCENARY)) * 20;
	if (amount > ch->gold)
		return send_to_char("You don't have enough gold to hire that mercenary.\n\r", ch);
	sprintf(buf, "The mercenary's guild charges you %i gold for your hired mercenary.\n\r", amount);
	act("$n hires $N.", ch, 0, merc, TO_ROOM);
	send_to_char(buf, ch);
	ch->gold -= amount;
	char_to_room(merc, ch->in_room);
	add_follower(merc, ch);
	merc->alignment = ch->alignment;
	if (merc_vnum != MOB_VNUM_SHAMAN_MERCENARY)
		merc->leader = ch;
	SET_BIT(merc->affected_by, AFF_CHARM);
	do_gtell(merc, "Reporting for duty.");
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_POWER;
	af.type = sn;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.duration = 50;
	af.level = level;
	af.affect_list_msg = str_dup("unable to hire another mercenary");
	af.bitvector = 0;
	affect_to_char(ch, &af);
}

void spell_hunters_strength(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	if (is_affected(ch, sn) || IS_NPC(ch))
		return send_to_char("You already have the enhanced strength of a hunter.\n\r", ch);
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_POWER;
	af.type = sn;
	af.location = APPLY_STR;
	af.modifier = 5;
	af.level = level;
	af.duration = (get_skill(ch, sn) - 60);
	af.bitvector = 0;
	affect_to_char(ch, &af);
	af.location = APPLY_DAMROLL;
	af.modifier = 10 + (ch->pcdata->bounty_credits / 40);
	affect_to_char(ch, &af);
	send_to_char("You feel your body surge with the newfound strength of a hunter seeking his prey!\n\r", ch);
	act("$n seems to move with a newfound strength and agility.", ch, 0, 0, TO_ROOM);
}

void spell_web(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;
	if (is_safe(ch, victim))
		return;
	if (saves_spell(level + 5, victim, DAM_OTHER) || number_range(20, 35) < get_curr_stat(victim, STAT_DEX))
	{
		act("$n tries to entangle you in a sticky web, but you avoid it.", ch, 0, victim, TO_VICT);
		act("$n tries to entangle $N in a sticky web, but $E avoids it.", ch, 0, victim, TO_NOTVICT);
		act("You try to entangle $N, but $E avoids it.", ch, 0, victim, TO_CHAR);
		return;
	}
	act("$n releases a sticky web at you, impeding your mobility!", ch, 0, victim, TO_VICT);
	act("$n releases a sticky web at $N!", ch, 0, victim, TO_NOTVICT);
	act("You release a sticky web at $N!", ch, 0, victim, TO_CHAR);
	victim->move /= 2;
	damage(ch, victim, number_range(30, 50), sn, DAM_ACID, TRUE);
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = sn;
	af.aftype = AFT_MALADY;
	af.level = level;
	af.location = APPLY_DEX;
	af.modifier = -(number_range(7, 15));
	af.duration = 3;
	af.bitvector = 0;
	if (!is_affected(victim, sn))
		affect_to_char(victim, &af);
}

void spell_informant(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim;
	int amount;
	char info[500];
	if (cabal_down(ch, CABAL_BOUNTY))
		return;
	if (!str_cmp(target_name, "") || !(victim = get_char_world(ch, target_name)))
		return send_to_char("Your informants are unable to find them.\n\r", ch);
	if (saves_spell(level + 8, victim, DAM_OTHER) || IS_NPC(victim) || IS_IMMORTAL(victim))
		return act("Your spies are unable to locate $N.", ch, 0, victim, TO_CHAR);
	amount = number_range(25, 50);
	if (victim->pcdata->bounty)
		amount /= 2;
	if (ch->gold < amount)
		return send_to_char("An informant scoffs at your meager payment.\n\r", ch);
	ch->gold -= amount;
	sprintf(info, "An informant whispers to you '{rI saw %s in %s.{x'\n\r", victim->name, victim->in_room->name);
	send_to_char(info, ch);
	sprintf(info, "An informant requires %i gold for his report.\n\r", amount);
	send_to_char(info, ch);
}

void spell_hunters_vision(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	if (is_affected(ch, sn))
		return send_to_char("You already see with the guile of a predator.\n\r", ch);
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_POWER;
	af.type = sn;
	af.modifier = 0;
	af.location = 0;
	af.bitvector = AFF_DETECT_INVIS | AFF_DETECT_HIDDEN;
	af.level = level;
	af.duration = 10;
	af.affect_list_msg = str_dup("grants the ability to detect the hidden and the invisible");
	affect_to_char(ch, &af);
	send_to_char("You begin to see the world through the undeceiving eyes of a hunter.\n\r", ch);
}
