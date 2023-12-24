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

void save_cabal_items args((void));

DECLARE_MPROG_FUN_GREET(greet_prog_outer_guardian);
DECLARE_MPROG_FUN_GREET(greet_prog_inner_guardian);
DECLARE_MPROG_FUN_DEATH(death_prog_outer_guardian);
DECLARE_MPROG_FUN_DEATH(death_prog_inner_guardian);
DECLARE_MPROG_FUN_ATTACK(attack_prog_outer_guardian);
DECLARE_MPROG_FUN_MOVE(move_prog_outer_guardian);
DECLARE_MPROG_FUN_GREET(greet_prog_centurion);
DECLARE_MPROG_FUN_GREET(greet_prog_battlefield_soldier);
DECLARE_MPROG_FUN_GREET(greet_prog_battlefield_paladin);
DECLARE_MPROG_FUN_GREET(greet_prog_battlefield_troll);
DECLARE_MPROG_FUN_GREET(greet_prog_battlefield_goblin);
DECLARE_MPROG_FUN_SPEECH(speech_prog_elthian);
DECLARE_MPROG_FUN_GREET(greet_prog_elthian);
DECLARE_MPROG_FUN_DEATH(death_prog_obsidian_prince);
DECLARE_MPROG_FUN_FIGHT(fight_prog_warrior_mercenary);
DECLARE_MPROG_FUN_FIGHT(fight_prog_thief_mercenary);
DECLARE_MPROG_FUN_FIGHT(fight_prog_assassin_mercenary);
DECLARE_MPROG_FUN_PULSE(pulse_prog_sequestered);
DECLARE_MPROG_FUN_FIGHT(fight_prog_sequestered);
DECLARE_MPROG_FUN_SPEECH(speech_prog_enforcer);
DECLARE_MPROG_FUN_FIGHT(fight_prog_animate_weapon);
DECLARE_MPROG_FUN_FIGHT(fight_prog_animate_armor);

const struct improg_type mprog_table[] =
	{
		{"greet_prog", "greet_prog_outer_guardian", greet_prog_outer_guardian},
		{"greet_prog", "greet_prog_inner_guardian", greet_prog_inner_guardian},
		{"death_prog", "death_prog_inner_guardian", death_prog_inner_guardian},
		{"death_prog", "death_prog_outer_guardian", death_prog_outer_guardian},
		{"attack_prog", "attack_prog_outer_guardian", attack_prog_outer_guardian},
		{"move_prog", "move_prog_outer_guardian", move_prog_outer_guardian},
		{"greet_prog", "greet_prog_centurion", greet_prog_centurion},
		{"greet_prog", "greet_prog_battlefield_soldier", greet_prog_battlefield_soldier},
		{"greet_prog", "greet_prog_battlefield_paladin", greet_prog_battlefield_paladin},
		{"greet_prog", "greet_prog_battlefield_troll", greet_prog_battlefield_troll},
		{"greet_prog", "greet_prog_battlefield_goblin", greet_prog_battlefield_goblin},
		{"speech_prog", "speech_prog_elthian", speech_prog_elthian},
		{"greet_prog", "greet_prog_elthian", greet_prog_elthian},
		{"death_prog", "death_prog_obsidian_prince", death_prog_obsidian_prince},
		{"fight_prog", "fight_prog_warrior_mercenary", fight_prog_warrior_mercenary},
		{"fight_prog", "fight_prog_thief_mercenary", fight_prog_thief_mercenary},
		{"fight_prog", "fight_prog_assassin_mercenary", fight_prog_assassin_mercenary},
		{"pulse_prog", "pulse_prog_sequestered", pulse_prog_sequestered},
		{"fight_prog", "fight_prog_sequestered", fight_prog_sequestered},
		{"speech_prog", "speech_prog_enforcer", speech_prog_enforcer},
		{"fight_prog", "fight_prog_animate_weapon", fight_prog_animate_weapon},
		{"fight_prog", "fight_prog_animate_armor", fight_prog_animate_armor},
		{NULL, NULL, NULL},
};

void mprog_set(MOB_INDEX_DATA *mobindex, const char *progtype, const char *name)
{
	int i;
	for (i = 0; mprog_table[i].name != NULL; i++)
		if (!str_cmp(mprog_table[i].name, name))
			break;
	if (!mprog_table[i].name)
		return bug("Load_improgs: 'M': Function not found for vnum %d", mobindex->vnum);

	// IF YOU ADD A NEW TYPE ALSO PUT IT IN DB.C DO_ADUMP!
	if (!str_cmp(progtype, "bribe_prog"))
	{
		mobindex->mprogs->bribe_prog = mprog_table[i].function;
		free_string(mobindex->mprogs->bribe_name);
		mobindex->mprogs->bribe_name = str_dup(name);
		SET_BIT(mobindex->progtypes, MPROG_BRIBE);
		return;
	}

	if (!str_cmp(progtype, "entry_prog"))
	{
		mobindex->mprogs->entry_prog = mprog_table[i].function;
		free_string(mobindex->mprogs->entry_name);
		mobindex->mprogs->entry_name = str_dup(name);
		SET_BIT(mobindex->progtypes, MPROG_ENTRY);
		return;
	}
	if (!str_cmp(progtype, "greet_prog"))
	{
		mobindex->mprogs->greet_prog = mprog_table[i].function;
		free_string(mobindex->mprogs->greet_name);
		mobindex->mprogs->greet_name = str_dup(name);
		SET_BIT(mobindex->progtypes, MPROG_GREET);
		return;
	}
	if (!str_cmp(progtype, "fight_prog"))
	{
		mobindex->mprogs->fight_prog = mprog_table[i].function;
		free_string(mobindex->mprogs->fight_name);
		mobindex->mprogs->fight_name = str_dup(name);
		SET_BIT(mobindex->progtypes, MPROG_FIGHT);
		return;
	}
	if (!str_cmp(progtype, "death_prog")) /* returning TRUE prevents death */
	{
		mobindex->mprogs->death_prog = mprog_table[i].function;
		free_string(mobindex->mprogs->death_name);
		mobindex->mprogs->death_name = str_dup(name);
		SET_BIT(mobindex->progtypes, MPROG_DEATH);
		return;
	}
	if (!str_cmp(progtype, "pulse_prog"))
	{
		mobindex->mprogs->pulse_prog = mprog_table[i].function;
		free_string(mobindex->mprogs->pulse_name);
		mobindex->mprogs->pulse_name = str_dup(name);
		SET_BIT(mobindex->progtypes, MPROG_PULSE);
		return;
	}
	if (!str_cmp(progtype, "speech_prog"))
	{
		mobindex->mprogs->speech_prog = mprog_table[i].function;
		free_string(mobindex->mprogs->speech_name);
		mobindex->mprogs->speech_name = str_dup(name);
		SET_BIT(mobindex->progtypes, MPROG_SPEECH);
		return;
	}
	if (!str_cmp(progtype, "give_prog"))
	{
		mobindex->mprogs->give_prog = mprog_table[i].function;
		free_string(mobindex->mprogs->give_name);
		mobindex->mprogs->give_name = str_dup(name);
		SET_BIT(mobindex->progtypes, MPROG_GIVE);
		return;
	}
	if (!str_cmp(progtype, "attack_prog"))
	{
		mobindex->mprogs->attack_prog = mprog_table[i].function;
		free_string(mobindex->mprogs->attack_name);
		mobindex->mprogs->attack_name = str_dup(name);
		SET_BIT(mobindex->progtypes, MPROG_ATTACK);
		return;
	}
	if (!str_cmp(progtype, "move_prog"))
	{
		mobindex->mprogs->move_prog = mprog_table[i].function;
		free_string(mobindex->mprogs->move_name);
		mobindex->mprogs->move_name = str_dup(name);
		SET_BIT(mobindex->progtypes, MPROG_MOVE);
		return;
	}
	if (!str_cmp(progtype, "request_prog"))
	{
		mobindex->mprogs->request_prog = mprog_table[i].function;
		free_string(mobindex->mprogs->request_name);
		mobindex->mprogs->request_name = str_dup(name);
		SET_BIT(mobindex->progtypes, MPROG_REQUEST);
		return;
	}
	bug("Load_improgs: 'M': invalid program type for vnum %d", mobindex->vnum);
	exit(1);
}

void greet_prog_outer_guardian(CHAR_DATA *mob, CHAR_DATA *ch)
{
	if (IS_NPC(ch) || ch->invis_level > LEVEL_HERO - 1)
		return;
	if (mob->cabal == ch->cabal)
		do_say(mob, cabal_messages[ch->cabal].entrygreeting);
}

void attack_prog_outer_guardian(CHAR_DATA *mob, CHAR_DATA *attacker)
{
	char buf[MSL];
	if (IS_NPC(attacker))
		return;
	sprintf(buf, "Intruder! Intruder! %s is raiding the cabal!", attacker->original_name);
	if (attacker->ghost > 0)
	{
		do_say(mob, "Ghosts aren't allowed in here!");
		attacker->hit = attacker->max_hit * .75;
		attacker->ghost = 0;
	}
	do_cb(mob, buf);
}

void greet_prog_inner_guardian(CHAR_DATA *mob, CHAR_DATA *ch)
{
	char buf[MSL];
	if (IS_NPC(ch) || IS_IMMORTAL(ch))
		return;
	if (ch->cabal == mob->cabal)
		return;
	sprintf(buf, "Intruder! Intruder! %s is raiding the cabal!", ch->original_name);
	do_cb(mob, buf);
	multi_hit(mob, ch, TYPE_UNDEFINED);
	if (ch->fighting != mob)
		set_fighting(ch, mob);
}

bool death_prog_outer_guardian(CHAR_DATA *mob, CHAR_DATA *killer)
{
	CHAR_DATA *innerguardian;
	CHAR_DATA *cabalguardian;
	OBJ_DATA *obj, *obj_next;
	char buf[MSL];
	bool found = FALSE;

	innerguardian = get_cabal_guardian(mob->cabal);

	do_cb(mob, "Alas, my life has come to an end.");

	if (innerguardian)
	{
		for (obj = innerguardian->carrying; obj != NULL; obj = obj_next)
		{
			found = FALSE;
			obj_next = obj->next_content;
			if (!isCabalItem(obj))
				continue;

			if (IS_NPC(killer) && killer->leader != NULL)
				killer = killer->leader;

			if ((obj->pIndexData->cabal == killer->cabal) && (mob->cabal != killer->cabal))
			{
				cabalguardian = get_cabal_guardian(obj->pIndexData->cabal);
				if (cabalguardian)
				{
					sprintf(buf, "As %s falls, %s returns to %s!",
							mob->short_descr, obj->short_descr, cabalguardian->short_descr);
					CHAR_DATA *wch;
					for (wch = char_list; wch != NULL; wch = wch->next)
					{
						if (!IS_NPC(wch) && is_same_group(wch, killer))
						{
							send_to_char("{YYou are rewarded 3 quest credits for this retrieval!{x\n\r", wch);
							wch->quest_credits += 3;
						}
					}
					obj_from_char(obj);
					obj_to_char(obj, cabalguardian);
					found = TRUE;
					act(buf, mob, 0, 0, TO_ALL);
					cabal_shudder(obj->pIndexData->cabal, FALSE);
				}
			}
			save_cabal_items();
		}
	}
	return FALSE;
}

bool death_prog_inner_guardian(CHAR_DATA *mob, CHAR_DATA *killer)
{
	CHAR_DATA *cabalguardian;
	OBJ_DATA *obj, *obj_next;
	char buf[MSL];
	bool found = FALSE;
	do_cb(mob, "Alas, my life has come to an end.");
	for (obj = mob->carrying; obj != NULL; obj = obj_next)
	{
		found = FALSE;
		obj_next = obj->next_content;
		if (!isCabalItem(obj))
			continue;

		if (obj->pIndexData->cabal != mob->cabal)
		{
			cabalguardian = get_cabal_guardian(obj->pIndexData->cabal);
			if (cabalguardian)
			{
				sprintf(buf, "As %s falls, %s returns to %s!",
						mob->short_descr, obj->short_descr, cabalguardian->short_descr);
				obj_from_char(obj);
				obj_to_char(obj, cabalguardian);
				found = TRUE;
				act(buf, mob, 0, 0, TO_ALL);
				cabal_shudder(obj->pIndexData->cabal, FALSE);
			}
		}
		save_cabal_items();
		if (obj->pIndexData->cabal == mob->cabal || !found)
		{
			if (IS_NPC(killer) && killer->leader != NULL)
				killer = killer->leader;
			act("As $N falls, $p appears in $n's hands!", killer, obj, mob, TO_ROOM);
			act("As $N falls, $p appears in your hands!", killer, obj, mob, TO_CHAR);
			obj->timer = 21;
			obj_from_char(obj);
			obj_to_char(obj, killer);
		}
	}
	return FALSE;
}

bool move_prog_outer_guardian(CHAR_DATA *ch, CHAR_DATA *mob, ROOM_INDEX_DATA *from, int direction)
{
	int blockdir;

	if (IS_IMMORTAL(ch))
		return TRUE;

	switch (mob->cabal)
	{
	default:
		blockdir = DIR_WEST;
		break;
	case (CABAL_RAGER):
		blockdir = DIR_WEST;
		break;
	case (CABAL_KNIGHT):
		blockdir = DIR_NORTH;
		break;
	case (CABAL_ARCANA):
		blockdir = DIR_NORTH;
		break;
	case (CABAL_OUTLAW):
		blockdir = DIR_NORTH;
		break;
	case (CABAL_EMPIRE):
		blockdir = DIR_NORTH;
		break;
	case (CABAL_ANCIENT):
		blockdir = DIR_SOUTH;
		break;
	case (CABAL_BOUNTY):
		blockdir = DIR_SOUTH;
		break;
	case (CABAL_SYLVAN):
		blockdir = DIR_NORTH;
		break;
	case (CABAL_ENFORCER):
		blockdir = DIR_WEST;
		break;
	}

	if (direction != blockdir)
		return TRUE;

	if (IS_NPC(ch))
		return FALSE;

	if (mob->cabal != ch->cabal)
	{
		act("$n moves to block $N.", mob, 0, ch, TO_NOTVICT);
		act("$n moves to block you.", mob, 0, ch, TO_VICT);
		return FALSE;
	}

	act("$n steps aside to allow $N to pass.", mob, 0, ch, TO_NOTVICT);
	act("$n steps aside to allow you to pass.", mob, 0, ch, TO_VICT);
	return TRUE;
}

void greet_prog_centurion(CHAR_DATA *mob, CHAR_DATA *ch)
{
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
	CHAR_DATA *v_next;
	bool found = FALSE;

	if (!IS_AWAKE(mob) || mob->fighting != NULL)
		return;

	for (victim = mob->in_room->people; victim != NULL; victim = v_next)
	{
		v_next = victim->next_in_room;

		if (!IS_NPC(victim) && victim->pcdata->empire == EMPIRE_ANATH && can_see(mob, victim))
		{
			found = TRUE;
			break;
		}
	}

	if (victim != NULL && found)
	{
		act("An Imperial Centurion is infuriated at the sight of you and attacks!", victim, NULL, NULL, TO_CHAR);
		act("An Imperial Centurion is infuriated at the sight of $n and attacks!", victim, NULL, NULL, TO_ROOM);
		sprintf(buf, "empire The anathema %s is here!", victim->name);
		do_ccb(mob, buf);
		do_murder(mob, victim->name);
	}
	return;
}

void greet_prog_battlefield_soldier(CHAR_DATA *mob, CHAR_DATA *ch)
{
	if (ch->alignment < 0)
	{
		do_murder(mob, ch->name);
	}
	return;
}

void greet_prog_battlefield_paladin(CHAR_DATA *mob, CHAR_DATA *ch)
{
	if (ch->alignment < 0)
	{
		do_murder(mob, ch->name);
	}
	return;
}

void greet_prog_battlefield_troll(CHAR_DATA *mob, CHAR_DATA *ch)
{
	if (ch->alignment > 0)
	{
		do_murder(mob, ch->name);
	}
	return;
}

void greet_prog_battlefield_goblin(CHAR_DATA *mob, CHAR_DATA *ch)
{
	if (ch->alignment > -1)
	{
		do_murder(mob, ch->name);
	}
	return;
}

void greet_prog_elthian(CHAR_DATA *mob, CHAR_DATA *ch)
{
	char buf[350];

	if (IS_NPC(ch))
		return;

	sprintf(buf, "Greetings, %s. The Obsidian Prince has summoned the Arch-Dukes of Hell to stop you. I can only send you on your path, should you so choose. Do you wish to proceed?", ch->name);
	do_say(mob, buf);
}

void speech_prog_elthian(CHAR_DATA *mob, CHAR_DATA *ch, char *speech)
{
	CHAR_DATA *bel;
	if (mob->in_room->vnum == 28008 && (strstr(speech, "Yes") || strstr(speech, "yes")) && !IS_NPC(ch))
	{
		act("Elthian steps forward, muttering something under his breath about brave fools.", ch, 0, 0, TO_ALL);
		act("Elthian the Wizard utters the words, 'xvhahatq'.", ch, 0, 0, TO_ALL);
		act("A shimmering white platform suddenly appears, speeding $n away to an unknown destination.", ch, 0, 0, TO_ROOM);
		act("A shimmering white platform suddenly appears, and as you step on it, it speeds you away over the fire below you to..", ch, 0, 0, TO_CHAR);
		char_from_room(ch);
		ch->ghost = 0;
		char_to_room(ch, get_room_index(28009));
		do_look(ch, "auto");
		for (bel = ch->in_room->people; bel != NULL; bel = bel->next_in_room)
		{
			if (!IS_NPC(bel) || bel->pIndexData->vnum != 28004)
				continue;
			act("The Devil Be'lal throws his horned head back, laughing mockingly, his voice booming with infernal majesty.", ch, 0, 0, TO_CHAR);
			act("The Devil Be'lal says '{YYou're the best they've got to send against me?{x'", ch, 0, 0, TO_CHAR);
		}
	}
}

bool death_prog_obsidian_prince(CHAR_DATA *mob, CHAR_DATA *killer)
{
	OBJ_DATA *obj, *obj_next;
	CHAR_DATA *rec, *looper, *looper_next;
	do_say(mob, "I cannot be defeated! I will rise again!");
	for (rec = mob->in_room->people; rec != NULL; rec = rec->next_in_room)
		if (!IS_NPC(rec))
			break;
	if (!rec)
		return FALSE;
	act("A cloud of crimson mist descends over the room, and when it disperses, $n is gone!", mob, 0, 0, TO_ALL);
	if (mob->carrying != NULL)
	{
		act("Upon the Throne of the Obsidian Prince $n finds $N's items of power!", rec, 0, mob, TO_ROOM);
		act("Upon the Throne of the Obsidian Prince you find $N's items of power!", rec, 0, mob, TO_CHAR);
		for (obj = mob->carrying; obj != NULL; obj = obj_next)
		{
			obj_next = obj->next_content;
			obj_from_char(obj);
			obj_to_char(obj, rec);
		}
	}
	extract_char(mob, TRUE);
	act("Behind the Throne, you find a shimmering portal that seems the only way out.", rec, 0, 0, TO_ALL);
	for (looper = rec->in_room->people; looper != NULL; looper = looper_next)
	{
		looper_next = looper->next_in_room;
		act("You step through the shimmering portal to find yourself back on your home plane.", looper, 0, 0, TO_CHAR);
		act("$n passes through the shimmering portal.", looper, 0, 0, TO_NOTVICT);
		char_from_room(looper);
		char_to_room(looper, get_room_index(3005));
		do_look(looper, "auto");
	}
	return TRUE;
}

void fight_prog_animate_armor(CHAR_DATA *mob, CHAR_DATA *ch)
{
	CHAR_DATA *master, *rch;
	ch = mob;

	if (ch->master == NULL)
		return;
	master = ch->master;

	if (master->in_room != ch->in_room)
		return;

	if (number_percent() < 50 && !IS_AFFECTED(ch, AFF_BLIND))
	{
		for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
		{
			if (rch->fighting)
			{
				if (rch->fighting == master)
				{
					int rescue = number_range(0, 4);
					switch (rescue)
					{
					case 0:
						do_say(ch, "You will not harm the master!");
						break;
					case 1:
						do_say(ch, "Infidel! The master will crush you!");
						break;
					case 2:
						do_say(ch, "I will never let the master fall!");
						break;
					case 3:
						do_say(ch, "Begone!");
						break;
					case 4:
						do_say(ch, "Fool, your quarrel is with me!");
						break;
					}
					do_rescue(ch, master->name);
					return;
				}
			}
		}
	}
}

void fight_prog_animate_weapon(CHAR_DATA *mob, CHAR_DATA *ch)
{
	CHAR_DATA *victim = ch;
	ch = mob;
	AFFECT_DATA af;
	char *weapon_type;

	if (mob->position != POS_FIGHTING)
		return;

	if (number_percent() < 75)
		return;

	if (!str_cmp(ch->material, "exotic"))
	{
		int number = number_range(0, 7);
		switch (number)
		{
		case 0:
			weapon_type = str_dup("sword");
			break;
		case 1:
			weapon_type = str_dup("axe");
			break;
		case 2:
			weapon_type = str_dup("spear");
			break;
		case 3:
			weapon_type = str_dup("mace");
			break;
		case 4:
			weapon_type = str_dup("polearm");
			break;
		case 5:
			weapon_type = str_dup("flail");
			break;
		case 6:
			weapon_type = str_dup("whip");
			break;
		case 7:
			weapon_type = str_dup("staff");
			break;
		}
	}
	else
		weapon_type = str_dup(ch->material);
	if (!str_cmp(weapon_type, "sword"))
	{
		act("$n begins spinning wildly and soars into you!", ch, 0, victim, TO_VICT);
		act("$n begins spinning wildly and soars into $N!", ch, 0, victim, TO_NOTVICT);
		act("$n begins spinning wildly and soars into $N!", ch, 0, victim, TO_CHAR);

		damage_new(ch, victim, dice(GET_DAMROLL(ch), 2), TYPE_UNDEFINED, DAM_SLASH, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "whirling slash");
		if (number_percent() < 70 && !is_affected(victim, skill_lookup("critical strike")))
		{
			act("$n's whirling slash tears a large wound in $N!", ch, 0, victim, TO_NOTVICT);
			act("$n's whirling slash tears a large wound in you!", ch, 0, victim, TO_VICT);
			act("$n's whirling slash tears a large wound in $N!", ch, 0, victim, TO_CHAR);
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.type = skill_lookup("critical strike");
			af.name = str_dup("whirling slash");
			af.level = 4;
			af.duration = 4;
			af.affect_list_msg = str_dup("bleeding uncontrollably");
			af.owner_name = ch->master ? str_dup(ch->master->original_name) : NULL;
			affect_to_char(victim, &af);
		}
	}
	else if (!str_cmp(weapon_type, "axe"))
	{
		act("$n cleaves you with a wide arcing slash!", ch, 0, victim, TO_VICT);
		act("$n cleaves $N with a wide arcing slash!", ch, 0, victim, TO_NOTVICT);
		act("$n cleaves $N with a wide arcing slash!", ch, 0, victim, TO_CHAR);

		damage_new(ch, victim, dice(GET_DAMROLL(ch), 3), TYPE_UNDEFINED, DAM_SLASH, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "arcing slash");
		if (number_percent() < 40 && !is_affected(victim, skill_lookup("critical strike")))
		{
			act("$n's arcing slash tears a large wound in $N!", ch, 0, victim, TO_NOTVICT);
			act("$n's arcing slash tears a large wound in you!", ch, 0, victim, TO_VICT);
			act("$n's arcing slash tears a large wound in $N!", ch, 0, victim, TO_CHAR);
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.type = skill_lookup("critical strike");
			af.name = str_dup("arcing slash");
			af.level = 5;
			af.duration = 4;
			af.affect_list_msg = str_dup("bleeding uncontrollably");
			af.owner_name = ch->master ? str_dup(ch->master->original_name) : NULL;
			affect_to_char(victim, &af);
		}
	}
	else if (!str_cmp(weapon_type, "mace"))
	{
		act("$n begins spinning wildly and soars into you!", ch, 0, victim, TO_VICT);
		act("$n begins spinning wildly and soars into $N!", ch, 0, victim, TO_NOTVICT);
		act("$n begins spinning wildly and soars into $N!", ch, 0, victim, TO_CHAR);

		damage_new(ch, victim, dice(GET_DAMROLL(ch), 2), TYPE_UNDEFINED, DAM_BASH, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "boneshattering blow");
		if (number_percent() < 70 && !is_affected(victim, skill_lookup("boneshatter")))
		{
			act("You hear the sound of bones crunching as $N is struck by $n's boneshattering blow!", ch, 0, victim, TO_NOTVICT);
			act("You hear the sound of your bones crunching as you are struck by $n's boneshattering blow!", ch, 0, victim, TO_VICT);
			act("You hear the sound of bones crunching as $N is struck by $n's boneshattering blow!", ch, 0, victim, TO_CHAR);
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.type = skill_lookup("boneshatter");
			af.name = str_dup("boneshatter");
			af.location = APPLY_STR;
			af.duration = 6;
			af.modifier = -4;
			affect_to_char(victim, &af);
			af.location = APPLY_DEX;
			af.modifier = -4;
			affect_to_char(victim, &af);
		}
	}
	else if (!str_cmp(weapon_type, "spear"))
	{
		act("$n dives into you and impales you!", ch, 0, victim, TO_VICT);
		act("$n dives into $N and impales $M!", ch, 0, victim, TO_NOTVICT);
		act("$n dives into $N and impales $M!", ch, 0, victim, TO_CHAR);

		damage_new(ch, victim, dice(GET_DAMROLL(ch), 4), TYPE_UNDEFINED, DAM_PIERCE, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "impalement");
		if (number_percent() < 20 && !is_affected(victim, skill_lookup("critical strike")))
		{
			act("$n's impalement drives deep into $N, spilling blood!", ch, 0, victim, TO_NOTVICT);
			act("$n's impalement drives deep into you, spilling blood!", ch, 0, victim, TO_VICT);
			act("$n's impalement drives deep into $N, spilling blood!", ch, 0, victim, TO_CHAR);
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.type = skill_lookup("critical strike");
			af.name = str_dup("impalement");
			af.level = 6;
			af.duration = 4;
			af.affect_list_msg = str_dup("bleeding uncontrollably");
			af.owner_name = ch->master ? str_dup(ch->master->original_name) : NULL;
			affect_to_char(victim, &af);
		}
	}
	else if (!str_cmp(weapon_type, "flail"))
	{
		act("$n repeatedly smashes you mercilessly!", ch, 0, victim, TO_VICT);
		act("$n repeatedly smashes $N mercilessly!", ch, 0, victim, TO_NOTVICT);
		act("$n repeatedly smashes $N mercilessly!", ch, 0, victim, TO_CHAR);

		damage_new(ch, victim, dice(GET_DAMROLL(ch), 1), TYPE_UNDEFINED, DAM_BASH, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "smash");
		damage_new(ch, victim, dice(GET_DAMROLL(ch), 2), TYPE_UNDEFINED, DAM_BASH, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "smash");
		if (number_percent() < 50)
			damage_new(ch, victim, dice(GET_DAMROLL(ch), 3), TYPE_UNDEFINED, DAM_BASH, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "smash");
		if (number_percent() < 25)
			damage_new(ch, victim, dice(GET_DAMROLL(ch), 4), TYPE_UNDEFINED, DAM_BASH, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "smash");
		if (number_percent() < 10)
			damage_new(ch, victim, dice(GET_DAMROLL(ch), 5), TYPE_UNDEFINED, DAM_BASH, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "smash");
	}
	else if (!str_cmp(weapon_type, "whip"))
	{
		act("$n repeatedly lashes you mercilessly!", ch, 0, victim, TO_VICT);
		act("$n repeatedly lashes $N mercilessly!", ch, 0, victim, TO_NOTVICT);
		act("$n repeatedly lashes $N mercilessly!", ch, 0, victim, TO_CHAR);

		damage_new(ch, victim, dice(GET_DAMROLL(ch), 1), TYPE_UNDEFINED, DAM_BASH, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "lash");
		damage_new(ch, victim, dice(GET_DAMROLL(ch), 1), TYPE_UNDEFINED, DAM_BASH, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "lash");
		if (number_percent() < 80)
			damage_new(ch, victim, dice(GET_DAMROLL(ch), 1), TYPE_UNDEFINED, DAM_BASH, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "lash");
		if (number_percent() < 65)
			damage_new(ch, victim, dice(GET_DAMROLL(ch), 1), TYPE_UNDEFINED, DAM_BASH, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "lash");
		if (number_percent() < 50)
			damage_new(ch, victim, dice(GET_DAMROLL(ch), 2), TYPE_UNDEFINED, DAM_BASH, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "lash");
		if (number_percent() < 35)
			damage_new(ch, victim, dice(GET_DAMROLL(ch), 3), TYPE_UNDEFINED, DAM_BASH, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "lash");
	}
	else if (!str_cmp(weapon_type, "polearm"))
	{
		act("$n cleaves you with a wide arcing slash!", ch, 0, victim, TO_VICT);
		act("$n cleaves $N with a wide arcing slash!", ch, 0, victim, TO_NOTVICT);
		act("$n cleaves $N with a wide arcing slash!", ch, 0, victim, TO_CHAR);

		damage_new(ch, victim, dice(GET_DAMROLL(ch), 4), TYPE_UNDEFINED, DAM_SLASH, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "arcing slash");
		if (number_percent() < 20 && !is_affected(victim, skill_lookup("critical strike")))
		{
			act("$n's arcing slash tears a large wound in $N!", ch, 0, victim, TO_NOTVICT);
			act("$n's arcing slash tears a large wound in you!", ch, 0, victim, TO_VICT);
			act("$n's arcing slash tears a large wound in $N!", ch, 0, victim, TO_CHAR);
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.type = skill_lookup("critical strike");
			af.name = str_dup("arcing slash");
			af.level = 6;
			af.duration = 4;
			af.affect_list_msg = str_dup("bleeding uncontrollably");
			af.owner_name = ch->master ? str_dup(ch->master->original_name) : NULL;
			affect_to_char(victim, &af);
		}
	}
	else if (!str_cmp(weapon_type, "staff"))
	{
		act("$n begins spinning wildly and soars into you!", ch, 0, victim, TO_VICT);
		act("$n begins spinning wildly and soars into $N!", ch, 0, victim, TO_NOTVICT);
		act("$n begins spinning wildly and soars into $N!", ch, 0, victim, TO_CHAR);

		damage_new(ch, victim, dice(GET_DAMROLL(ch), 4), TYPE_UNDEFINED, DAM_BASH, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "boneshattering blow");
		if (number_percent() < 30 && !is_affected(victim, skill_lookup("boneshatter")))
		{
			act("You hear the sound of bones crunching as $N is struck by $n's boneshattering blow!", ch, 0, victim, TO_NOTVICT);
			act("You hear the sound of your bones crunching as you are struck by $n's boneshattering blow!", ch, 0, victim, TO_VICT);
			act("You hear the sound of bones crunching as $N is struck by $n's boneshattering blow!", ch, 0, victim, TO_CHAR);
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.type = skill_lookup("boneshatter");
			af.name = str_dup("boneshatter");
			af.location = APPLY_STR;
			af.duration = 6;
			af.modifier = -7;
			affect_to_char(victim, &af);
			af.location = APPLY_DEX;
			af.modifier = -7;
			affect_to_char(victim, &af);
		}
	}
	else if (!str_cmp(weapon_type, "dagger"))
	{
		act("$n darts through the air and plunges into you!", ch, 0, victim, TO_VICT);
		act("$n darts through the air and plunges into $N!", ch, 0, victim, TO_NOTVICT);
		act("$n darts through the air and plunges into $N!", ch, 0, victim, TO_CHAR);

		damage_new(ch, victim, dice(GET_DAMROLL(ch), 1), TYPE_UNDEFINED, DAM_PIERCE, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "plunge");
		int type = number_range(1, 3);
		switch (type)
		{
		case 1:
			if (number_percent() < 30 && !is_affected(victim, skill_lookup("hobble")))
			{
				act("$n plunges into $N's leg, shredding $S muscles!", ch, 0, victim, TO_NOTVICT);
				act("$n plunges into your leg, shredding your muscles!", ch, 0, victim, TO_VICT);
				act("$n plunges into $N's leg, shredding $S muscles!", ch, 0, victim, TO_CHAR);
				init_affect(&af);
				af.where = TO_AFFECTS;
				af.type = skill_lookup("hobble");
				af.name = str_dup("hobble");
				af.duration = 6;
				af.location = APPLY_DEX;
				af.modifier = -10;
				affect_to_char(victim, &af);
			}
			break;
		case 2:
			if (number_percent() < 30 && !is_affected(victim, skill_lookup("critical strike")))
			{
				act("$n plunges into $N's chest, spraying blood!", ch, 0, victim, TO_NOTVICT);
				act("$n plunges into your chest, spraying blood!", ch, 0, victim, TO_VICT);
				act("$n plunges into $N's chest, spraying blood!", ch, 0, victim, TO_CHAR);
				init_affect(&af);
				af.where = TO_AFFECTS;
				af.type = skill_lookup("critical strike");
				af.name = str_dup("stab wound");
				af.level = 3;
				af.duration = 5;
				af.affect_list_msg = str_dup("bleeding uncontrollably");
				af.owner_name = ch->master ? str_dup(ch->master->original_name) : NULL;
				affect_to_char(victim, &af);
			}
			break;
		case 3:
			act("$n twists inside $N!", ch, 0, victim, TO_NOTVICT);
			act("$n twists inside you!", ch, 0, victim, TO_VICT);
			act("$n twists inside $N!", ch, 0, victim, TO_CHAR);
			damage_new(ch, victim, dice(GET_DAMROLL(ch), 3), TYPE_UNDEFINED, DAM_PIERCE, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "twist");
			break;
		}
	}
	else if (!str_cmp(weapon_type, "hands"))
	{
		act("$n begins spinning wildly and soars into you!", ch, 0, victim, TO_VICT);
		act("$n begins spinning wildly and soars into $N!", ch, 0, victim, TO_NOTVICT);
		act("$n begins spinning wildly and soars into $N!", ch, 0, victim, TO_CHAR);

		damage_new(ch, victim, dice(GET_DAMROLL(ch), 2), TYPE_UNDEFINED, DAM_BASH, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "boneshattering blow");
		if (number_percent() < 70 && !is_affected(victim, skill_lookup("boneshatter")))
		{
			act("You hear the sound of bones crunching as $N is struck by $n's boneshattering blow!", ch, 0, victim, TO_NOTVICT);
			act("You hear the sound of your bones crunching as you are struck by $n's boneshattering blow!", ch, 0, victim, TO_VICT);
			act("You hear the sound of bones crunching as $N is struck by $n's boneshattering blow!", ch, 0, victim, TO_CHAR);
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.type = skill_lookup("boneshatter");
			af.name = str_dup("boneshatter");
			af.location = APPLY_STR;
			af.duration = 6;
			af.modifier = -4;
			affect_to_char(victim, &af);
			af.location = APPLY_DEX;
			af.modifier = -4;
			affect_to_char(victim, &af);
		}
	}
	else
		return;
}

void fight_prog_warrior_mercenary(CHAR_DATA *mob, CHAR_DATA *ch)
{
	int i;
	AFFECT_DATA af;

	if (number_percent() < 15)
	{
		act("$n drums at $N with $s maces.", mob, 0, ch, TO_NOTVICT);
		act("$n drums at you with $s maces.", mob, 0, ch, TO_VICT);
		for (i = 0; i < number_range(1, 4); i++)
			one_hit_new(mob, ch, gsn_drum, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 130, NULL);
		one_hit_new(mob, ch, gsn_drum, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 175, NULL);
		return;
	}

	if (number_percent() < 8)
	{
		if (number_percent() < 45)
		{
			act("$n brings $s mace around in a powerful bone-shattering blow, but misses $N.", mob, 0, ch, TO_NOTVICT);
			act("$n brings $s mace around in a powerful bone-shattering blow, but misses you.", mob, 0, ch, TO_VICT);
			damage(mob, ch, 0, gsn_boneshatter, DAM_NONE, TRUE);
			return;
		}
		act("$n brings $s mace around in a powerful bone-shattering blow, striking $N.", mob, 0, ch, TO_NOTVICT);
		act("$n brings $s mace around in a powerful bone-shattering blow, striking you.", mob, 0, ch, TO_VICT);
		one_hit_new(mob, ch, gsn_boneshatter, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 200, NULL);
		af.where = TO_AFFECTS;
		af.aftype = AFT_MALADY;
		af.type = gsn_boneshatter;
		af.location = APPLY_STR;
		af.modifier = -8;
		af.level = mob->level;
		af.duration = 5;
		af.bitvector = 0;
		if (is_affected(ch, gsn_boneshatter))
			affect_to_char(ch, &af);
		af.location = APPLY_DEX;
		if (is_affected(ch, gsn_boneshatter))
			affect_to_char(ch, &af);
		return;
	}
}

void fight_prog_thief_mercenary(CHAR_DATA *mob, CHAR_DATA *ch)
{
	if (number_percent() < 35)
	{
		act("$n circles around $N to land a deadly strike!", mob, 0, ch, TO_NOTVICT);
		act("$n circles around you to land a deadly strike!", mob, 0, ch, TO_VICT);
		one_hit_new(mob, ch, gsn_circle, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 200, NULL);
		return;
	}
	if (number_percent() < 30)
	{
		do_trip(mob, "");
		return;
	}
}

void fight_prog_assassin_mercenary(CHAR_DATA *mob, CHAR_DATA *ch)
{
	int dam = 0;

	if (number_percent() > 90)
		return;

	ch = mob->fighting;

	if (ch == NULL)
		return;

	if ((ch->fighting != mob) && (mob->fighting != ch))
		return;

	if (is_affected(ch, gsn_protective_shield))
	{
		act("$N's throw seems to slide around you.", ch, 0, mob, TO_CHAR);
		act("$N's throw seems to slide around $n.", ch, 0, mob, TO_NOTVICT);
		act("Your throw seems to slide around $n.", ch, 0, mob, TO_VICT);
		return;
	}

	dam = 40 + number_range(10, 20) * 2 / 3;
	dam += str_app[get_curr_stat(mob, STAT_STR)].todam;

	act("$n grabs $N and throws $M to the ground with stunning force!", mob, 0, ch, TO_NOTVICT);
	act("You grab $N and throw $M to the ground with stunning force!", mob, 0, ch, TO_CHAR);
	act("$n grabs you and throws you to the ground with stunning force!", mob, 0, ch, TO_VICT);

	damage_old(mob, ch, dam, gsn_throw, DAM_BASH, TRUE);
	WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
	return;
}

void pulse_prog_sequestered(CHAR_DATA *mob)
{
	CHAR_DATA *victim = mob->hunting;

	//	if (!victim || !mob->in_room || !victim->in_room || weather_info.sunlight != SUN_DARK)
	if (!victim || !mob->in_room || !victim->in_room)
	{
		act("$n fades away into the gloom.", mob, 0, 0, TO_ALL);
		extract_char(mob, TRUE);
		return;
	}

	if (mob->position < POS_STANDING)
		mob->position = POS_STANDING;

	if (victim->ghost != 0)
	{
		act("Having finished it's task, $n fades into the gloom.", mob, 0, 0, TO_ALL);
		extract_char(mob, TRUE);
		return;
	}

	if (number_percent() > 50)
		return;

	if (mob->fighting)
		if (mob->fighting != victim)
			stop_fighting(mob, TRUE);

	if (victim->in_room->vnum != mob->in_room->vnum)
	{
		act("$n disappears in a flash of black light.", mob, 0, 0, TO_ALL);
		char_from_room(mob);
		char_to_room(mob, victim->in_room);
		act("$n appears in a flash of black light.", mob, 0, 0, TO_ALL);
	}
	multi_hit(mob, victim, TYPE_UNDEFINED);
	return;
}

void fight_prog_sequestered(CHAR_DATA *mob, CHAR_DATA *ch)
{
	AFFECT_DATA af;

	if (number_percent() > 50)
		return;

	ch = mob->fighting;

	if (ch == NULL)
		return;

	if ((ch->fighting != mob) && (mob->fighting != ch))
		return;

	switch (mob->pIndexData->vnum)
	{
	case 200:
		act("$n rakes its bony claw across your face!", mob, 0, ch, TO_VICT);
		act("$n rakes its bony claw across $N's face!", mob, 0, ch, TO_NOTVICT);
		one_hit_new(mob, ch, TYPE_UNDEFINED, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 300, "savage clawing");
		break;
	case 201:
		(*skill_table[skill_lookup("vampiric touch")].spell_fun)(skill_lookup("vampiric touch"), 70, mob, ch, TAR_CHAR_OFFENSIVE);
		break;
	case 202:
		if (!is_affected(ch, skill_lookup("hold person")) && !saves_spell(60, ch, DAM_OTHER))
		{
			init_affect(&af);
			af.where = TO_AFFECTS;
			af.aftype = AFT_MALADY;
			af.type = gsn_hold_person;
			af.location = 0;
			af.modifier = 0;
			af.duration = 0;
			af.bitvector = 0;
			af.level = ch->level;
			affect_to_char(ch, &af);
			send_to_char("Your muscles stop responding as the terror of the Abyss strikes you!\n\r", ch);
			act("$n suddenly stiffens up, $s face the only sign of the terrors $e faces.", ch, NULL, NULL, TO_ROOM);
		}
		break;
	default:
		return;
	}
	return;
}

void speech_prog_enforcer(CHAR_DATA *mob, CHAR_DATA *ch, char *speech)
{
	int vnum = 0;

	if (IS_NPC(ch))
		return;

	if (ch->cabal != CABAL_ENFORCER)
		return;

	if (ch->cabal != mob->cabal)
		return;

	if (ch->pause > 5)
		return;

	if (ch->position != POS_STANDING)
		return;

	speech = str_dup(lowstring(speech));

	if (strstr(speech, "take me to") && strstr(speech, "midgaard"))
		vnum = 3054;
	if (strstr(speech, "take me to") && strstr(speech, "arkham"))
		vnum = 768;
	if (strstr(speech, "take me to") && strstr(speech, "tar valon"))
		vnum = 1856;
	if (strstr(speech, "take me to") && strstr(speech, "new thalos"))
		vnum = 9506;
	if (strstr(speech, "take me to") && strstr(speech, "ofcol"))
		vnum = 601;

	if (vnum == 0)
		return;

	act("A portal of pure gray energy opens on the western wall, and $n steps through.", ch, 0, 0, TO_ROOM);
	act("A portal of pure gray energy opens on the western wall, and you step through it.", ch, 0, 0, TO_CHAR);
	char_from_room(ch);
	char_to_room(ch, get_room_index(vnum));
	act("$n appears.", ch, 0, 0, TO_ROOM);
	WAIT_STATE(ch, PULSE_VIOLENCE);
	do_look(ch, "auto");
}
