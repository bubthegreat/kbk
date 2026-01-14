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

char *target_name;
int empire_lookup args((const char *name));

DECLARE_DO_FUN(do_yell);
DECLARE_DO_FUN(do_myell);
DECLARE_DO_FUN(do_murder);
DECLARE_DO_FUN(do_look);
DECLARE_DO_FUN(easy_induct);

void easy_uninduct(CHAR_DATA *ch)
{
	int gn, gns, sn;

	if (!ch->cabal)
		return;

	gn = group_lookup(cabal_table[ch->cabal].name);

	for (gns = 0; gns < MAX_SKILL; gns++)
	{
		if (group_table[gn].spells[gns] == NULL)
			break;

		sn = skill_lookup(group_table[gn].spells[gns]);

		if (skill_table[sn].skill_level[ch->class] < LEVEL_HERO + 1 && ch->pcdata->learned[sn] > 0)
		{
			ch->pcdata->learned[sn] = -2;
		}
	}
	group_remove(ch, cabal_table[ch->cabal].name);
	ch->cabal = 0;
	return;
}

void spell_bloodthirst(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	int chance, hp_percent;
	int fatigue = skill_lookup("fatigue");

	if ((chance = get_skill(ch, gsn_bloodthirst)) == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_bloodthirst))
	{
		send_to_char("Your thirst for blood continues.\n\r", ch);
		return;
	}

	if (IS_AFFECTED(ch, AFF_CALM))
	{
		send_to_char("You're feeling too mellow to be bloodthirsty.\n\r", ch);
		return;
	}
	if (IS_AFFECTED(ch, AFF_SLOW))
	{
		send_to_char("You're moving too slowly to build up a bloodthirsty rage.\n\r", ch);
		return;
	}

	if (is_affected(ch, fatigue))
	{
		send_to_char("You're too tired to be bloodthirsty.\n\r", ch);
		return;
	}

	if (cabal_down(ch, CABAL_RAGER))
		return;
	/* modifiers */

	hp_percent = 100 * ch->hit / ch->max_hit;
	chance += 35 - hp_percent / 2;

	if (number_percent() < chance)
	{
		AFFECT_DATA af;

		WAIT_STATE(ch, PULSE_VIOLENCE);

		send_to_char("You hunger for blood!\n\r", ch);
		act("$n gets a bloodthirsty look in $s eyes.", ch, NULL, NULL, TO_ROOM);
		check_improve(ch, gsn_bloodthirst, TRUE, 2);

		init_affect(&af);
		af.where = TO_AFFECTS;
		af.type = gsn_bloodthirst;
		af.level = ch->level;
		af.duration = ch->level / 5;
		af.modifier = 20;
		af.bitvector = 0;
		af.aftype = AFT_POWER;
		af.location = APPLY_HITROLL;
		affect_to_char(ch, &af);

		af.location = APPLY_DAMROLL;
		affect_to_char(ch, &af);

		af.modifier = ch->level * 2;
		af.location = APPLY_AC;
		affect_to_char(ch, &af);
		af.location = APPLY_DEX;
		af.modifier = 4;
		af.bitvector = AFF_HASTE;
		affect_to_char(ch, &af);
	}

	else
	{

		send_to_char("You feel bloodthirsty for a moment, but it passes.\n\r",
					 ch);
		check_improve(ch, gsn_bloodthirst, FALSE, 2);
		WAIT_STATE(ch, PULSE_VIOLENCE);
	}
}

void do_shackles(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	AFFECT_DATA af;

	if (((get_skill(ch, gsn_shackles)) == 0) && !(IS_NPC(ch) && ch->pIndexData->vnum == MOB_VNUM_STALKER))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (cabal_down(ch, CABAL_ENFORCER))
		return;

	one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		send_to_char("Shackle who?\n\r", ch);
		return;
	}

	victim = get_char_room(ch, arg);
	if (victim == NULL)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (victim->ghost > 0)
		return send_to_char("But they're a ghost!\n\r", ch);

	if (is_affected(victim, gsn_bloody_shackles))
	{
		send_to_char("They are already bound with shackles.\n\r", ch);
		return;
	}

	if (!(IS_SET(victim->act, PLR_CRIMINAL)))
	{
		act("You can't do that.", ch, 0, 0, TO_CHAR);
		return;
	}

	if (is_affected(victim, gsn_fly))
	{
		affect_strip(victim, gsn_fly);
		send_to_char("You fall to the ground.\n\r", victim);
		act("Restrained by your shackles, $N falls to the ground.", ch, 0, victim, TO_CHAR);
		return;
	}

	init_affect(&af);
	af.aftype = AFT_POWER;
	af.where = TO_AFFECTS;
	af.type = gsn_bloody_shackles;
	af.level = ch->level;
	af.duration = 6;
	af.modifier = -10;
	af.bitvector = 0;
	af.location = APPLY_DEX;
	affect_to_char(victim, &af);
	af.bitvector = 0;
	af.modifier = -5;
	af.location = APPLY_DAMROLL;
	affect_to_char(victim, &af);
	af.location = APPLY_HITROLL;
	affect_to_char(victim, &af);
	af.modifier = -2;
	af.location = APPLY_ATTACK_MOD;
	affect_to_char(victim, &af);

	act("You clasp barbed shackles onto $N wrists.", ch, 0, victim, TO_CHAR);
	act("$n clasps barbed shackles onto $N wrists.", ch, 0, victim, TO_NOTVICT);
	act("Your hands involuntarily flinch from the stabbing pain as $n clasps barbed shackles onto your wrists.", ch, 0, victim, TO_VICT);
	damage_old(ch, victim, number_range(20, 30), gsn_bloody_shackles, DAM_NONE, TRUE);

	return;
}

void do_disperse(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	AFFECT_DATA af;
	ROOM_INDEX_DATA *pRoomIndex;
	int chance, goodroom;

	if ((get_skill(ch, gsn_disperse)) == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	if (cabal_down(ch, CABAL_OUTLAW))
		return;

	if (is_affected(ch, gsn_disperse))
	{
		send_to_char("This power was used too recently.\n\r", ch);
		return;
	}

	if (ch->in_room == NULL || (IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)))
	{
		send_to_char("You failed.\n\r", ch);
		return;
		check_improve(ch, gsn_disperse, FALSE, 1);
	}

	if (ch->in_room->cabal != 0 && ch->in_room->cabal != ch->cabal)
	{
		send_to_char("You failed.\n\r", ch);
		return;
		check_improve(ch, gsn_disperse, FALSE, 1);
	}

	chance = get_skill(ch, gsn_disperse) + 10;

	if (number_percent() > chance)
	{
		send_to_char("You failed.\n\r", ch);
		return;
		check_improve(ch, gsn_disperse, FALSE, 2);
	}

	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
	{
		vch_next = vch->next_in_room;
		if (IS_NPC(vch))
		{
			continue;
		}
		goodroom = 0;
		while (goodroom == 0)
		{
			pRoomIndex = get_random_room(vch);
			if (IS_SET(pRoomIndex->room_flags, ROOM_NO_RECALL))
			{
				continue;
			}
			goodroom = 1;
		}
		send_to_char("The world spins around you and you find yourself somewhere else!\n\r", vch);
		act("$n disappears!", vch, NULL, NULL, TO_ROOM);
		char_from_room(vch);
		char_to_room(vch, pRoomIndex);
		act("$n appears!", vch, NULL, NULL, TO_ROOM);
		do_look(vch, "auto");
	}
	ch->mana -= ch->level * 2;
	check_improve(ch, gsn_disperse, TRUE, 1);

	init_affect(&af);
	af.aftype = AFT_POWER;
	af.where = TO_AFFECTS;
	af.type = gsn_disperse;
	af.level = ch->level;
	af.duration = 15;
	af.modifier = 0;
	af.location = APPLY_NONE;
	af.bitvector = 0;
	af.affect_list_msg = str_dup("prevents usage of disperse");
	affect_to_char(ch, &af);

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
	return;
}
void do_dragonsword(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *dragonsword;
	int chance;
	AFFECT_DATA af;
	char buf[MAX_STRING_LENGTH];
	if ((chance = get_skill(ch, gsn_dragonsword)) == 0)
	{
		send_to_char("You don't know how to forge golden weapons.\n\r", ch);
		return;
	}
	if (is_affected(ch, gsn_dragonsword))
	{
		send_to_char("You're not up to forging another dragonsword yet.\n\r", ch);
		return;
	}
	dragonsword = create_object(get_obj_index(OBJ_VNUM_DRAGONSWORD), 0);
	obj_to_char(dragonsword, ch);
	switch (number_range(0, 6))
	{
	case 0:
		sprintf(buf, "an ornate longsword of pure gold named 'Honorbound'");
		break;
	case 1:
		sprintf(buf, "an intricate rapier of pure gold named 'Shadowbane'");
		break;
	case 2:
		sprintf(buf, "a heavy claymore of pure gold named 'Divine Wrath'");
		break;
	case 3:
		sprintf(buf, "a fine blade of pure gold named 'Vengeance'");
		break;
	case 4:
		sprintf(buf, "a runed scimitar of pure gold named 'Soulforge'");
		break;
	case 5:
		sprintf(buf, "a cutlass of pure gold named 'Storm Cleaver'");
		break;
	case 6:
		sprintf(buf, "a broadsword of pure gold named 'Glory'");
		break;
	}
	free_string(dragonsword->short_descr);
	dragonsword->short_descr = str_dup(buf);
	init_affect(&af);
	af.aftype = AFT_POWER;
	af.where = TO_AFFECTS;
	af.type = gsn_dragonsword;
	af.level = 51;
	af.duration = 40;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.bitvector = 0;
	af.affect_list_msg = str_dup("prevents usage of dragonsword");
	affect_to_char(ch, &af);
	act("$n forges a golden weapon!", ch, 0, 0, TO_ROOM);
	act("You forge a golden weapon!", ch, 0, 0, TO_CHAR);
	check_improve(ch, skill_lookup("dragonsword"), TRUE, 3);
	return;
}
void do_dragonplate(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *dragonplate;
	int chance;
	AFFECT_DATA af;

	if ((chance = get_skill(ch, gsn_dragonplate)) == 0)
	{
		send_to_char("You don't know how to forge a dragonplate.\n\r", ch);
		return;
	}
	if (is_affected(ch, gsn_dragonplate))
	{
		send_to_char("You're not up to forging another dragonplate yet.\n\r", ch);
		return;
	}
	dragonplate = create_object(get_obj_index(OBJ_VNUM_DRAGONPLATE), 0);
	obj_to_char(dragonplate, ch);
	init_affect(&af);
	af.aftype = AFT_POWER;
	af.where = TO_AFFECTS;
	af.type = gsn_dragonplate;
	af.level = 51;
	af.duration = 40;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.bitvector = 0;
	af.affect_list_msg = str_dup("prevents usage of dragonplate");
	affect_to_char(ch, &af);
	act("$n forges a golden dragonplate!", ch, 0, 0, TO_ROOM);
	act("You forge a golden dragonplate!", ch, 0, 0, TO_CHAR);
	check_improve(ch, skill_lookup("dragonplate"), TRUE, 4);
	return;
}
void do_honorable_combat(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_INPUT_LENGTH];
	char buf2[MAX_INPUT_LENGTH];
	ROOM_INDEX_DATA *arena_room;
	CHAR_DATA *vch, *victim, *zombie;
	int zombies = 0;
	AFFECT_DATA af;

	if ((get_skill(ch, gsn_honorable_combat)) == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	if (cabal_down(ch, CABAL_KNIGHT))
	{
		return;
	}
	if (is_affected(ch, gsn_honorable_combat))
	{
		send_to_char("You don't feel up to challenging someone to honorable combat yet.\n\r", ch);
		return;
	}
	one_argument(argument, arg);
	if (arg[0] == '\0')
	{
		send_to_char("Challenge who to honorable combat?\n\r", ch);
		return;
	}
	victim = get_char_room(ch, arg);
	if (victim == NULL)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}
	if (IS_NPC(victim) || ch->fighting != NULL || victim->fighting != NULL || ch == victim)
	{
		send_to_char("You can't do that.\n\r", ch);
		return;
	}
	if (is_affected(victim, gsn_honorable_combat))
	{
		send_to_char("They cannot yet be called away again to honorable combat.\n\r", ch);
		return;
	}
	arena_room = get_room_index(ROOM_FORCE_DUEL);
	for (vch = arena_room->people; vch != NULL; vch = vch->next_in_room)
	{
		if (!IS_NPC(vch) && !IS_IMMORTAL(vch))
			break;
	}
	if (vch != NULL)
	{
		send_to_char("The arena is in use!\n\r", ch);
		return;
	}
	if (number_percent() > get_skill(ch, gsn_honorable_combat) * .75)
	{
		act("$n tries to call upon the Light to take $N away to fight to the death, but fails!", ch, 0, victim, TO_NOTVICT);
		act("You failed to call $N away to honorable combat.\n\r", ch, 0, victim, TO_CHAR);
		act("$n tries to call upon the Light to take you away to fight to the death, but fails!\n\r", ch, 0, victim, TO_VICT);
		ch->mana -= 200;
		WAIT_STATE(ch, PULSE_VIOLENCE * 3);
		multi_hit(victim, ch, TYPE_UNDEFINED);
		return;
	}
	if (ch->sex < SEX_FEMALE)
		sprintf(buf2, "him");
	if (ch->sex == SEX_FEMALE)
		sprintf(buf2, "her");
	switch (number_range(0, 1))
	{
	case (0):
		sprintf(buf, "Help! %s is calling me away to duel %s!", PERS(ch, victim), buf2);
		break;
	case (1):
		sprintf(buf, "Help! %s is forcing me to duel %s!", PERS(ch, victim), buf2);
		break;
	}
	do_myell(victim, buf);
	act("$n calls upon the Light to take $N away to duel to the death!", ch, 0, victim, TO_NOTVICT);
	act("$n calls upon the Light to take you away to duel to the death!", ch, 0, victim, TO_VICT);
	act("You call upon the Light to take $N away to duel to the death!", ch, 0, victim, TO_CHAR);
	check_improve(ch, skill_lookup("honorable combat"), TRUE, 3);
	ch->pcdata->old_room = ch->in_room->vnum;
	victim->pcdata->old_room = ch->in_room->vnum;
	for (zombie = victim->in_room->people; zombie != NULL; zombie = zombie->next_in_room)
	{
		if (zombie != NULL && IS_NPC(zombie) && IS_AFFECTED(zombie, AFF_CHARM) && zombie->pIndexData->vnum == MOB_VNUM_ZOMBIE && zombies < 3)
		{
			char_from_room(zombie);
			char_to_room(zombie, arena_room);
			zombies++;
		}
	}
	char_from_room(ch);
	char_to_room(ch, arena_room);
	char_from_room(victim);
	char_to_room(victim, arena_room);
	ch->mana -= 200;
	victim->hit = victim->max_hit;
	do_look(victim, "auto");
	do_look(ch, "auto");
	multi_hit(victim, ch, TYPE_UNDEFINED);
	WAIT_STATE(ch, PULSE_VIOLENCE);
	init_affect(&af);
	af.aftype = AFT_POWER;
	af.where = TO_AFFECTS;
	af.type = gsn_honorable_combat;
	af.duration = 24;
	af.location = 0;
	af.modifier = 0;
	af.bitvector = 0;
	af.affect_list_msg = str_dup("prevents usage of honorable combat");
	af.level = ch->level;
	affect_to_char(ch, &af);
}

void spell_stalker(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim;
	CHAR_DATA *stalker;
	AFFECT_DATA af;
	int i;

	if (cabal_down(ch, CABAL_ENFORCER))
		return;

	if (is_affected(ch, sn))
	{
		send_to_char("This power was used too recently.\n\r", ch);
		return;
	}

	send_to_char("You attempt to summon a stalker.\n\r", ch);
	act("$n attempts to summon a stalker.", ch, NULL, NULL, TO_ROOM);

	if ((victim = get_char_world(ch, target_name)) == NULL || victim == ch || victim->in_room == NULL || IS_NPC(victim))
	{
		send_to_char("You failed.\n\r", ch);
		return;
	}

	if (!(IS_SET(victim->act, PLR_CRIMINAL)))
	{
		act("You can't do that.", ch, 0, 0, TO_CHAR);
		return;
	}

	stalker = create_mobile(get_mob_index(MOB_VNUM_STALKER));

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = sn;
	af.aftype = AFT_POWER;
	af.level = level;
	af.duration = 24;
	af.bitvector = 0;
	af.modifier = 0;
	af.affect_list_msg = str_dup("prevents usage of stalker");
	af.location = APPLY_NONE;
	affect_to_char(ch, &af);

	for (i = 0; i < MAX_STATS; i++)
	{
		stalker->perm_stat[i] = victim->perm_stat[i];
	}

	stalker->max_hit = UMIN(20000, victim->max_hit);
	stalker->hit = stalker->max_hit;
	stalker->max_mana = victim->max_mana;
	stalker->mana = stalker->max_mana;
	stalker->level = victim->level;
	stalker->damroll = victim->level * 1.5;
	stalker->hitroll = victim->level * 1.5;

	stalker->damage[DICE_NUMBER] =
		number_range(victim->level / 18, victim->level / 14);
	stalker->damage[DICE_TYPE] =
		number_range(victim->level / 4, victim->level / 3);
	stalker->damage[DICE_BONUS] =
		number_range(victim->level / 10, victim->level / 8);

	for (i = 0; i < 3; i++)
		stalker->armor[i] = interpolate(stalker->level, 100, -100);
	stalker->armor[3] = interpolate(stalker->level, 100, 0);
	stalker->gold = 0;
	stalker->invis_level = LEVEL_HERO;

	SET_BIT(stalker->act, PLR_HOLYLIGHT);
	char_to_room(stalker, victim->in_room);
	stalker->last_fought = victim;

	send_to_char("An invisible stalker arrives to stalk you!\n\r", victim);
	act("An invisible stalker arrives to stalk $n!\n\r", victim, NULL, NULL, TO_ROOM);
	send_to_char("An invisible stalker has been sent to destroy the criminal!\n\r", ch);
	do_shackles(stalker, victim->name);
	do_murder(stalker, victim->name);
	return;
}

/* outlaw hacksaw -- Dioxide */
void do_hacksaw(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	int skill;

	if ((skill = get_skill(ch, gsn_hacksaw)) == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		send_to_char("Hacksaw who?\n\r", ch);
		return;
	}

	victim = get_char_room(ch, arg);
	if (victim == NULL)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (!is_affected(victim, gsn_bloody_shackles))
	{
		send_to_char("They aren't bound with shackles!\n\r", ch);
		return;
	}

	if (number_percent() < (skill - 15))
	{
		act("You release $N's legs by sawing off the shackles.", ch, 0, victim, TO_CHAR);
		act("$n releases $N's legs by sawing off the shackles.", ch, 0, victim, TO_NOTVICT);
		act("$n releases your legs by sawing off the shackles.", ch, 0, victim, TO_VICT);
		affect_strip(victim, gsn_shackles);
		WAIT_STATE(ch, PULSE_VIOLENCE * 2);
		WAIT_STATE(victim, PULSE_VIOLENCE * 1);
		check_improve(ch, gsn_hacksaw, TRUE, 1);
		return;
	}
	else
	{
		act("You attempt to saw off $N's shackles, and fail.", ch, 0, victim, TO_CHAR);
		act("$n attempts to saw off $N's shackles, and fails.", ch, 0, victim, TO_NOTVICT);
		act("$n attempts to saw off your shackles, and fails.", ch, 0, victim, TO_VICT);
		WAIT_STATE(ch, PULSE_VIOLENCE * 2);
		WAIT_STATE(victim, PULSE_VIOLENCE * 2);
		check_improve(ch, gsn_hacksaw, FALSE, 4);
		return;
	}
	return;
}

void spell_divine_aura(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	// if (cabal_down(ch,CABAL_ENFORCER))
	// return;

	if (is_affected(ch, sn))
	{
		send_to_char("You already have an aura.\n\r", ch);
		return;
	}

	init_affect(&af);
	af.aftype = AFT_POWER;
	af.where = TO_AFFECTS;
	af.type = sn;
	af.level = level;
	af.duration = level;
	af.location = APPLY_AC;
	af.modifier = -150;
	af.bitvector = 0;
	affect_to_char(ch, &af);
	af.location = APPLY_HITROLL;
	af.modifier = 15;
	af.bitvector = 0;
	affect_to_char(ch, &af);
	af.location = APPLY_SAVING_SPELL;
	af.modifier = -15;
	af.bitvector = 0;
	affect_to_char(ch, &af);
	af.type = skill_lookup("fly");
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.affect_list_msg = str_dup("grants flight");
	af.bitvector = AFF_FLYING;
	affect_to_char(ch, &af);
	send_to_char("You feel an aura surround you and lift you from the ground.\n\r", ch);
	return;
}

void check_bloodoath(CHAR_DATA *victim, char *argument)
{
	if (str_cmp(argument, "My heart, my soul, my life for the Empire") || !str_cmp(argument, "My heart, my soul, my life for the Empire.") || IS_NPC(victim))
		return;

	if (victim->pcdata->empire != EMPIRE_PBLOOD || IS_AFFECTED(victim, AFF_CHARM))
		return;

	wiznet("$N has bloodoathed.", victim, 0, WIZ_CABAL, 0, get_trust(victim));
	act("The bloodoath is finalized.", victim, 0, 0, TO_ALL);
	victim->pcdata->empire = EMPIRE_BLOOD;
	easy_uninduct(victim);

	if (!IS_EVIL(victim))
	{
		SET_BIT(victim->act, PLR_BETRAYER);
	}

	victim->pcdata->oalign = victim->alignment + 2000;
	victim->alignment = -1000;

	victim->pcdata->oethos = victim->pcdata->ethos + 2000;
	victim->pcdata->ethos = 1000;

	return;
}

void do_empire(CHAR_DATA *ch, char *argument)
{
	char arg1[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
	int empire;

	if (IS_NPC(ch))
		return;

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	if ((ch->level < 56 && ch->pcdata->empire != EMPIRE_SLEADER && ch->pcdata->empire != EMPIRE_EMPEROR))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (arg1[0] == '\0' || arg2[0] == '\0')
	{
		send_to_char("Syntax: empire <char> <state>\n\r", ch);
		return;
	}

	if ((victim = get_char_world(ch, arg1)) == NULL)
	{
		send_to_char("They aren't playing.\n\r", ch);
		return;
	}
	if (IS_NPC(victim))
	{
		send_to_char("You can't do that.\n\r", ch);
		return;
	}
	if ((empire = empire_lookup(arg2)) == -2)
	{
		send_to_char("No such Empire state.\n\r", ch);
		return;
	}
	if (empire == EMPIRE_ANATH && (ch->pcdata->empire < EMPIRE_SLEADER ||
								   victim->pcdata->empire <= EMPIRE_PBLOOD || victim->pcdata->empire >= ch->pcdata->empire))
	{
		send_to_char("You can't make them anathema.\n\r", ch);
		return;
	}

	/*	if (empire == EMPIRE_IMM || (ch->pcdata->empire < EMPIRE_CITIZEN))
		{
			send_to_char("You can't do that!\n\r",ch);
			return;
		}
	*/

	if (empire == EMPIRE_IMM && ch->level != MAX_LEVEL)
	{
		send_to_char("You can't do that!\n\r", ch);
		return;
	}

	if (empire == EMPIRE_EMPEROR && ch->pcdata->empire < EMPIRE_IMM)
	{
		send_to_char("You can't make them the Emperor.\n\r", ch);
		return;
	}

	if (empire == EMPIRE_SLEADER && ch->pcdata->empire < EMPIRE_IMM)
	{
		send_to_char("You can't make them a leader.\n\r", ch);
		return;
	}

	if (empire == EMPIRE_CITIZEN && (ch->pcdata->empire < EMPIRE_SLEADER || victim->pcdata->empire != EMPIRE_BLOOD))
	{
		send_to_char("You can't make them a citizen.\n\r", ch);
		return;
	}

	if (empire == EMPIRE_PBLOOD && (ch->pcdata->empire < EMPIRE_CITIZEN || victim->pcdata->empire > EMPIRE_PBLOOD || (victim->pcdata->empire == EMPIRE_ANATH && ch->pcdata->empire < EMPIRE_IMM)))
	{
		send_to_char("You can't attempt to bloodoath them.\n\r", ch);
		return;
	}

	if (empire == EMPIRE_BLOOD)
	{
		send_to_char("No such Empire state.\n\r", ch);
		return;
	}

	if (empire == EMPIRE_NONE && !(IS_IMMORTAL(ch)))
	{
		send_to_char("Anathema them!\n\r", ch);
		return;
	}

	if (empire == EMPIRE_ANATH)
	{
		send_to_char("You have betrayed your Empire and are now anathema!\n\r", victim);
		send_to_char("You are now anathema to the Empire!\n\r", victim);
		easy_uninduct(victim);
		if (victim->pcdata->oalign)
		{
			if (victim->pcdata->oalign == 1)
			{
				victim->alignment = -1000;
			}
			else if (victim->pcdata->oalign == 2)
			{
				victim->alignment = 0;
			}
			else if (victim->pcdata->oalign == 3)
			{
				victim->alignment = 1000;
			}
			else
			{
				victim->alignment = victim->pcdata->oalign - 2000;
			}
		}
		if (victim->pcdata->oethos)
		{
			if (victim->pcdata->oethos == 1)
			{
				victim->pcdata->ethos = -1000;
			}
			else if (victim->pcdata->oethos == 2)
			{
				victim->pcdata->ethos = 0;
			}
			else if (victim->pcdata->oethos == 3)
			{
				victim->pcdata->ethos = 1000;
			}
			else
			{
				victim->pcdata->ethos = victim->pcdata->oethos - 2000;
			}
		}
		set_extitle(victim, ", Anathema to the Empire");
		sprintf(buf, "AUTO: Anathamaed by %s.\n\r", ch->name);
		add_history(NULL, victim, buf);
	}
	if (empire == EMPIRE_CITIZEN)
	{
		send_to_char("You have become a citizen of the Empire.\n\r", victim);
		easy_induct(victim, "empire");
		sprintf(buf, "AUTO: Inducted into %s by %s.\n\r", capitalize(cabal_table[victim->cabal].name), ch->name);
		set_extitle(victim, ", Citizen of the Empire");
		add_history(NULL, victim, buf);
	}
	if (empire == EMPIRE_PBLOOD)
	{
		send_to_char("Speak the words, and you shall be bloodoathed.\n\r", victim);
	}
	if (empire == EMPIRE_SLEADER)
	{
		send_to_char("You have become a Leader of the Empire.\n\r", victim);
	}
	if (empire == EMPIRE_EMPEROR)
	{
		send_to_char("You have become the Emperor of the Empire.\n\r", victim);
		set_extitle(victim, ", Emperor of Thera");
	}

	victim->pcdata->empire = empire;
	send_to_char("Ok.\n\r", ch);
	return;
}

void do_bloodoath(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char arg1[MAX_STRING_LENGTH];

	argument = one_argument(argument, arg1);

	if (arg1[0] == '\0')
	{
		send_to_char("Syntax: bloodoath <character>\n\r", ch);
		return;
	}

	sprintf(buf, "%s preblood", arg1);

	do_empire(ch, buf);
	return;
}

void do_citizen(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char arg1[MAX_STRING_LENGTH];

	argument = one_argument(argument, arg1);

	if (arg1[0] == '\0')
	{
		send_to_char("Syntax: citizen <character>\n\r", ch);
		return;
	}

	sprintf(buf, "%s citizen", arg1);

	do_empire(ch, buf);
	return;
}

void do_anathema(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char arg1[MAX_STRING_LENGTH];

	argument = one_argument(argument, arg1);

	if (arg1[0] == '\0')
	{
		send_to_char("Syntax: anathema <character>\n\r", ch);
		return;
	}

	sprintf(buf, "%s anathema", arg1);

	do_empire(ch, buf);
	return;
}

void do_nightfist(CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	int chance;
	int mod = 0;

	if ((chance = get_skill(ch, gsn_nightfist)) == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	if (is_affected(ch, gsn_nightfist))
	{
		send_to_char("You are already filled with unholy might.\n\r", ch);
		return;
	}
	if (ch->mana < 80)
	{
		send_to_char("You don't have the mana.\n\r", ch);
		return;
	}

	if (number_percent() > chance)
	{
		send_to_char("Your pulse races, then calms.\n\r", ch);
		check_improve(ch, gsn_nightfist, FALSE, 2);
		ch->mana -= 40;
		WAIT_STATE(ch, PULSE_VIOLENCE);
		return;
	}

	if (ch->pcdata->empire >= EMPIRE_SLEADER)
		mod += 3;

	if (ch->pcdata->empire >= EMPIRE_EMPEROR)
		mod += 3;

	act("$n's eyes fill with unholy anger.", ch, 0, 0, TO_ROOM);
	send_to_char("Your pulse races as you are filled with unholy might!\n\r", ch);
	check_improve(ch, gsn_nightfist, TRUE, 2);
	init_affect(&af);
	af.aftype = AFT_POWER;
	af.where = TO_AFFECTS;
	af.type = gsn_nightfist;
	af.modifier = 10;
	af.location = APPLY_HITROLL;
	af.duration = ch->level / 3;
	af.bitvector = 0;
	af.level = ch->level;
	affect_to_char(ch, &af);
	af.location = APPLY_DAMROLL;
	affect_to_char(ch, &af);
	af.location = APPLY_SAVES;
	af.modifier = -5;
	affect_to_char(ch, &af);
	ch->mana -= 80;
	WAIT_STATE(ch, PULSE_VIOLENCE);
	return;
}

bool is_centurion(CHAR_DATA *ch)
{
	if (IS_NPC(ch) && ch->pIndexData->vnum == MOB_VNUM_CENTURION)
		return TRUE;

	return FALSE;
}

int room_centurions(CHAR_DATA *ch)
{
	CHAR_DATA *gch;
	int number;

	number = 0;

	for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
	{
		if (is_centurion(gch))
		{
			number++;
		}
	}

	return number;
}

int pay_howmuch(CHAR_DATA *ch)
{
	int pay;

	pay = ch->level * 20;
	return pay;
}

void do_pay(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char arg1[MAX_STRING_LENGTH];
	int pay, paid;
	CHAR_DATA *imperial;

	argument = one_argument(argument, arg1);

	if (IS_NPC(ch) || room_centurions(ch) == 0 || ch->pcdata->empire >= EMPIRE_BLOOD)
	{
		send_to_char("You don't need to pay to pass through here.\n\r", ch);
		return;
	}

	if (ch->last_fight_name != NULL)
	{
		if ((imperial = get_char_world(ch, ch->last_fight_name)) != NULL)
		{
			if (imperial->pcdata->empire >= EMPIRE_BLOOD)
			{
				send_to_char("An Imperial Centurion refuses your pament.\n\r", ch);
				return;
			}
		}
	}

	pay = pay_howmuch(ch);

	if (arg1[0] == '\0')
	{
		sprintf(buf, "You must pay %d gold coins to pass.\n\r", pay);
		send_to_char(buf, ch);
		return;
	}
	if (is_number(arg1))
	{
		paid = atoi(arg1);
		if (ch->gold < paid)
		{
			send_to_char("You can't afford that.\n\r", ch);
			return;
		}
		if (ch->pcdata->empire == EMPIRE_ANATH)
		{
			sprintf(buf, "An Imperial Centurion snorts disdainfully at you.\n\r");
			send_to_char(buf, ch);
			return;
		}
		if (paid < pay)
		{
			sprintf(buf, "An Imperial Centurion scoffs at your offering.\n\r");
			sprintf(buf, "You must pay %d gold coins to pass.\n\r", pay);
			send_to_char(buf, ch);
			return;
		}
		deduct_cost(ch, paid);
		act("$n slips an Imperial Centurion something.", ch, NULL, NULL, TO_ROOM);
		act("An Imperial Centurion motions that you can leave.", ch, NULL, NULL, TO_CHAR);
		act("An Imperial Centurion motions that $n can leave.", ch, NULL, NULL, TO_ROOM);
		ch->pcdata->empire_pay = paid;
	}
	return;
}

void do_centurion_call(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *centurion;
	AFFECT_DATA af;
	CHAR_DATA *check;
	bool found = FALSE;
	int count, chance, max;
	CHAR_DATA *fch;
	CHAR_DATA *fch_next;

	if ((chance = get_skill(ch, gsn_centurion_call)) == 0)
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (!str_cmp(argument, "release"))
	{
		for (fch = char_list; fch != NULL; fch = fch_next)
		{
			fch_next = fch->next;
			if (IS_NPC(fch) && (fch->leader == ch) && fch->pIndexData->vnum == MOB_VNUM_CENTURION)
			{
				if (fch->in_room == NULL)
					return;
				printf_to_char(ch, "Your centurion at %s has been relieved of his duty.\n\r", fch->in_room->name);
				extract_char(fch, TRUE);
				found = TRUE;
			}
		}
		if (is_affected(ch, gsn_centurion_call) && found)
		{
			affect_strip(ch, gsn_centurion_call);
			send_to_char("You can call centurions again.\n\r", ch);
		}
		return;
	}

	if (is_affected(ch, gsn_centurion_call))
	{
		send_to_char("You can't call more Centurions yet.\n\r", ch);
		return;
	}

	if (ch->mana <= 100)
	{
		send_to_char("You do not have the mana.\n\r", ch);
		return;
	}

	for (check = char_list; check != NULL; check = check->next)
	{
		if (IS_NPC(check) && (check->leader == ch) && check->pIndexData->vnum == MOB_VNUM_CENTURION)
			found = TRUE;
	}

	if (found)
	{
		send_to_char("You can't call more Centurions yet.\n\r", ch);
		return;
	}

	if (isInWilderness(ch) || (ch->in_room->sector_type == SECT_INSIDE) || (IS_SET(ch->in_room->room_flags, ROOM_LAW)) || ((ch->in_room->cabal != 0 && ch->in_room->cabal != CABAL_EMPIRE)))
	{
		send_to_char("You can't call Centurions here.\n\r", ch);
		return;
	}
	if (room_centurions(ch) > 0)
	{
		send_to_char("There are already Centurions here.\n\r", ch);
		return;
	}
	act("$n calls for Centurions to block this road.", ch, 0, 0, TO_ROOM);
	send_to_char("You call for Centurions to block this road.\n\r", ch);

	if (number_percent() > chance)
	{
		send_to_char("Centurions do not respond to your call.\n\r", ch);
		check_improve(ch, gsn_centurion_call, FALSE, 1);
		ch->mana -= 50;
		WAIT_STATE(ch, PULSE_VIOLENCE * 2);
		return;
	}

	ch->mana -= 100;

	max = 2;
	if (ch->pcdata->empire >= EMPIRE_SLEADER)
	{
		max++;
	}
	if (ch->pcdata->empire >= EMPIRE_EMPEROR)
	{
		max++;
	}

	for (count = 0; count < max; count++)
	{
		centurion = create_mobile(get_mob_index(MOB_VNUM_CENTURION));
		centurion->level = ch->level + 3;
		centurion->max_hit = ch->max_hit * 8 / 5;
		centurion->damroll += ch->level * 9 / 5;
		centurion->hitroll += ch->level * 8 / 5;
		centurion->alignment = ch->alignment;
		char_to_room(centurion, ch->in_room);
		add_follower(centurion, ch);
		centurion->leader = ch;
		SET_BIT(centurion->affected_by, AFF_CHARM);
		act("$N arrives at $n's demand!", ch, NULL, centurion, TO_ROOM);
		act("$N arrives at your demand!", centurion, NULL, ch, TO_CHAR);
		centurion->hit = centurion->max_hit;
	}

	init_affect(&af);
	af.aftype = AFT_POWER;
	af.where = TO_AFFECTS;
	af.level = ch->level;
	af.modifier = 0;
	af.duration = ch->level / 2;
	af.bitvector = 0;
	af.location = 0;
	af.type = gsn_centurion_call;
	affect_to_char(ch, &af);
	check_improve(ch, gsn_centurion_call, TRUE, 1);
	return;
}

void do_devour(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *find_corpse;
	OBJ_DATA *corpse;
	char arg1[MAX_STRING_LENGTH];
	char *name;
	char *last_name;
	char *back_name;
	char buf[MAX_STRING_LENGTH];
	int number, count;

	corpse = NULL;

	if ((get_skill(ch, gsn_devour) == 0))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (cabal_down(ch, CABAL_EMPIRE))
		return;

	one_argument(argument, arg1);
	number = number_argument(argument, arg1);
	count = 0;

	for (find_corpse = ch->in_room->contents; find_corpse != NULL; find_corpse = find_corpse->next_content)
	{
		if ((can_see_obj(ch, find_corpse)) && (arg1[0] == '\0' || is_name(arg1, find_corpse->name)))
		{
			if (++count == number)
				corpse = find_corpse;
		}
	}

	if (corpse == NULL)
	{
		send_to_char("You don't see that here.\n\r", ch);
		return;
	}

	if ((corpse->item_type != ITEM_CORPSE_NPC) && (corpse->item_type != ITEM_CORPSE_PC))
	{
		send_to_char("You can only devour a corpse.\n\r", ch);
		return;
	}

	if (is_set(&corpse->extra_flags, CORPSE_NO_ANIMATE))
	{
		send_to_char("You cannot devour that corpse.\n\r", ch);
		return;
	}
	back_name = str_dup(corpse->short_descr);
	name = corpse->short_descr;
	last_name = name;

	last_name = one_argument(corpse->short_descr, name);
	last_name = one_argument(last_name, name);
	last_name = one_argument(last_name, name);

	corpse->short_descr = str_dup(back_name);

	sprintf(buf, "The bloodless, mutilated corpse of %s is here.", last_name);
	free_string(corpse->description);
	corpse->description = str_dup(buf);
	set_bit(&corpse->extra_flags, CORPSE_NO_ANIMATE);

	if (number_percent() > get_skill(ch, gsn_devour))
	{
		act("Your stomach churns and you destroy the corpse.", ch, corpse, NULL, TO_CHAR);
		act("$n tries to devour a corpse but destroys it.", ch, NULL, NULL, TO_ROOM);
		check_improve(ch, gsn_devour, FALSE, 2);
		WAIT_STATE(ch, PULSE_VIOLENCE / 2);
		return;
	}

	act("$n thrashes in a flurry of blood as $e devours $p.", ch, corpse, 0, TO_ROOM);
	act("You thrash in a flurry of blood as you devour $p.", ch, corpse, 0, TO_CHAR);

	ch->hit = UMIN(ch->hit + ch->level + corpse->level * 2, ch->max_hit);
	ch->mana = UMIN(ch->mana + ch->level + corpse->level * 2, ch->max_mana);
	ch->move = UMIN(ch->move + ch->level + corpse->level * 2, ch->max_move);

	WAIT_STATE(ch, PULSE_VIOLENCE / 2);
	check_improve(ch, gsn_devour, TRUE, 1);
	return;
}

void do_shove(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
	CHAR_DATA *fch;
	int skill;

	one_argument(argument, arg);

	if ((skill = get_skill(ch, gsn_shove)) == 0)
	{
		if (IS_NPC(ch) && (ch->pIndexData->vnum == MOB_VNUM_EMPIRE || ch->pIndexData->vnum == MOB_VNUM_CENTURION))
		{
			skill = 100;
		}
		else
		{
			send_to_char("Huh?\n\r", ch);
			return;
		}
	}
	if (cabal_down(ch, CABAL_EMPIRE))
		return;
	if (ch->fighting == NULL)
	{
		send_to_char("You aren't fighting anymore.\n\r", ch);
		return;
	}
	if (arg[0] == '\0')
	{
		send_to_char("Shove whom?\n\r", ch);
		return;
	}
	if ((victim = get_char_room(ch, arg)) == NULL)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}
	if (victim == ch)
	{
		send_to_char("You can't shove yourself.\n\r", ch);
		return;
	}
	if (!IS_NPC(ch) && IS_NPC(victim))
	{
		send_to_char("You can't do that!\n\r", ch);
		return;
	}
	if (is_safe(ch, victim))
		return;
	sprintf(buf, "Help! %s is shoving me into battle!", PERS(ch, victim));

	do_myell(victim, buf);
	if (number_percent() > skill)
	{
		send_to_char("You failed.\n\r", ch);
		check_improve(ch, gsn_shove, FALSE, 1);
		set_fighting(ch, victim);
		set_fighting(victim, ch);
		WAIT_STATE(ch, PULSE_VIOLENCE * 2);
		return;
	}
	stop_fighting(ch, FALSE);
	for (fch = char_list; fch != NULL; fch = fch->next)
	{
		if (IS_CABAL_GUARD(fch) || (IS_NPC(ch) && ch->pIndexData->vnum == 2925))
			continue;
		if (fch->fighting == ch && fch != victim)
		{
			stop_fighting(fch, FALSE);
			set_fighting(fch, victim);
		}
	}
	act("You shove $N in front of you to take your blows!", ch, NULL, victim, TO_CHAR);
	act("$n shoves you in front of $m to take $s blows!", ch, NULL, victim, TO_VICT);
	act("$n shoves $N in front of $m to take $s blows!", ch, NULL, victim, TO_NOTVICT);
	check_improve(ch, gsn_shove, TRUE, 1);
	WAIT_STATE(ch, PULSE_VIOLENCE);
	return;
}

int check_imperial_training(CHAR_DATA *ch)
{
	AFFECT_DATA *paf;
	int modifier = IMPERIAL_NONE;

	for (paf = ch->affected; paf != NULL; paf = paf->next)
	{
		if (paf->type == skill_lookup("imperial training"))
		{
			modifier = paf->modifier;
			break;
		}
	}

	return modifier;
}

void do_imperial_training(CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	int type;

	if ((get_skill(ch, gsn_imperial_training) == 0))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (cabal_down(ch, CABAL_EMPIRE))
		return;

	if (argument[0] == '\0')
	{
		send_to_char("Syntax: imperial none\n\r        imperial offense\n\r        imperial defense\n\r", ch);

		if (check_imperial_training(ch) == IMPERIAL_OFFENSE)
		{
			send_to_char("Your training is currently focused on offense.\n\r", ch);
		}
		else if (check_imperial_training(ch) == IMPERIAL_DEFENSE)
		{
			send_to_char("Your training is currently focused on defense.\n\r", ch);
		}
		else
		{
			send_to_char("Your training is currently focused on nothing.\n\r", ch);
		}
		return;
	}

	WAIT_STATE(ch, PULSE_VIOLENCE);

	if (!str_prefix(argument, "none"))
	{
		if (is_affected(ch, gsn_imperial_training))
		{
			send_to_char("You no longer focus on offense or defense.\n\r", ch);
			affect_strip(ch, gsn_imperial_training);
		}
		else
		{
			send_to_char("You aren't focused on offensive or defensive training.\n\r", ch);
		}
		return;
	}
	else if (!str_prefix(argument, "offense"))
	{
		if (is_affected(ch, gsn_imperial_training))
		{
			send_to_char("You are already focused in your training.\n\r", ch);
			return;
		}
		send_to_char("You now focus your training on offense.\n\r", ch);
		type = IMPERIAL_OFFENSE;
	}
	else if (!str_prefix(argument, "defense"))
	{
		if (is_affected(ch, gsn_imperial_training))
		{
			send_to_char("You are already focused in your training.\n\r", ch);
			return;
		}
		send_to_char("You now focus your training on defense.\n\r", ch);
		type = IMPERIAL_DEFENSE;
	}
	else
	{
		send_to_char("Syntax: imperial none\n\r        imperial offense\n\r        imperial defense\n\r", ch);
		return;
	}

	affect_strip(ch, gsn_imperial_training);

	init_affect(&af);
	af.aftype = AFT_POWER;
	af.where = TO_AFFECTS;
	af.type = gsn_imperial_training;
	af.level = ch->level;
	af.duration = ch->level / 2;
	af.modifier = type;
	switch (type)
	{
	case IMPERIAL_OFFENSE:
		af.affect_list_msg = str_dup("improves offensive combat ability");
		break;
	case IMPERIAL_DEFENSE:
		af.affect_list_msg = str_dup("improves defensive combat ability");
		break;
	}
	af.location = APPLY_NONE;
	af.bitvector = 0;
	affect_to_char(ch, &af);

	return;
}

void spell_shield_onyx(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	if (cabal_down(ch, CABAL_ANCIENT))
		return;

	if (is_affected(ch, sn))
	{
		send_to_char("The power of the vanguard already protects you.\n\r", ch);
		return;
	}

	init_affect(&af);
	af.aftype = AFT_POWER;
	af.where = TO_RESIST;
	af.type = sn;
	af.level = level;
	af.duration = level / 5;
	af.location = APPLY_NONE;
	af.modifier = 0;
	af.bitvector = RES_HOLY;
	affect_to_char(ch, &af);
	af.where = TO_AFFECTS;
	af.type = sn;
	af.location = APPLY_SAVES;
	af.modifier = -8;
	affect_to_char(ch, &af);

	send_to_char("You bow your head, uttering blasphemies under your breath as you draw your arms together.\n\r", ch);
	send_to_char("A putrid black ooze begins to seep from your pores, covering your body in a second, shadowy skin.\n\r", ch);
	return;
}

void spell_maehslin(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, gsn_maehslin))
	{
		send_to_char("You already have it.\n\r", ch);
		return;
	}

	init_affect(&af);
	af.aftype = AFT_POWER;
	af.where = TO_AFFECTS;
	af.type = gsn_maehslin;
	af.level = level;
	af.duration = level / 6;
	af.location = 0;
	af.modifier = 0;
	af.bitvector = 0;
	af.affect_list_msg = str_dup("grants a chance to deliver an additional attack");
	affect_to_char(victim, &af);

	act("A fitful violet glow begins to surround $n.", victim, NULL, NULL, TO_ROOM);
	send_to_char("A fitful violet glow begins to surround you, radiating a chill wind.\n\r", victim);
	return;
}

void spell_darkforge(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	OBJ_DATA *shadow;
	char arg1[MAX_INPUT_LENGTH];

	if (ch->mana < 25)
	{
		send_to_char("You don't have the mana.\n\r", ch);
		return;
	}

	target_name = one_argument(target_name, arg1);

	if (arg1[0] == '\0')
	{
		send_to_char("You must choose either a sword, dagger or whip.\n\r", ch);
		return;
	}

	if (!strcmp(arg1, "sword"))
	{
		if (is_affected(ch, gsn_darkforge_sword))
		{
			send_to_char("You cannot darkforge another sword yet.\n\r", ch);
			return;
		}

		send_to_char("You master the unholy and darkforge yourself a sword.\n\r", ch);
		act("$n conjures and darkforges a sword!", ch, NULL, NULL, TO_ROOM);
		ch->mana -= 30;
		shadow = create_object(get_obj_index(OBJ_VNUM_ANCIENT_SWORD), level);
		shadow->timer = 60;
		obj_to_char(shadow, ch);
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_POWER;
		af.type = gsn_darkforge_sword;
		af.duration = 60;
		af.location = 0;
		af.bitvector = 0;
		af.modifier = 0;
		af.affect_list_msg = str_dup("prevents usage of darkforge sword");
		af.level = ch->level;
		affect_to_char(ch, &af);
		check_improve(ch, gsn_darkforge, TRUE, 1);
		return;
	}
	else if (!strcmp(arg1, "dagger"))
	{
		if (is_affected(ch, gsn_darkforge_dagger))
		{
			send_to_char("You cannot darkforge another dagger yet.\n\r", ch);
			return;
		}

		send_to_char("You master the unholy and darkforge yourself a dagger.\n\r", ch);
		act("$n conjures and darkforges a dagger!", ch, NULL, NULL, TO_ROOM);
		ch->mana -= 30;
		shadow = create_object(get_obj_index(OBJ_VNUM_ANCIENT_DAGGER), level);
		shadow->timer = 60;
		obj_to_char(shadow, ch);
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_POWER;
		af.type = gsn_darkforge_dagger;
		af.duration = 60;
		af.location = 0;
		af.bitvector = 0;
		af.modifier = 0;
		af.affect_list_msg = str_dup("prevents usage of darkforge dagger");
		af.level = ch->level;
		affect_to_char(ch, &af);
		check_improve(ch, gsn_darkforge, TRUE, 1);
		return;
	}
	else if (!strcmp(arg1, "whip"))
	{
		if (is_affected(ch, gsn_darkforge_whip))
		{
			send_to_char("You cannot darkforge another whip yet.\n\r", ch);
			return;
		}

		send_to_char("You master the unholy and darkforge yourself a whip.\n\r", ch);
		act("$n conjures and darkforges a sword!", ch, NULL, NULL, TO_ROOM);
		ch->mana -= 30;
		shadow = create_object(get_obj_index(OBJ_VNUM_ANCIENT_WHIP), level);
		shadow->timer = 60;
		obj_to_char(shadow, ch);
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_POWER;
		af.type = gsn_darkforge_whip;
		af.duration = 60;
		af.location = 0;
		af.bitvector = 0;
		af.modifier = 0;
		af.affect_list_msg = str_dup("prevents usage of darkforge whip");
		af.level = ch->level;
		affect_to_char(ch, &af);
		check_improve(ch, gsn_darkforge, TRUE, 1);
		return;
	}
}

/*Pintose- a nasty spell to wreak total havoc, it disperses, blinds, cofuses, garbles, and change sex!*/
void spell_pandemonium(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	AFFECT_DATA af;

	if (is_affected(ch, sn))
	{
		send_to_char("You haven't regained enough energy to cause such great pandemonium.\n\r", ch);
		return;
	}
	if (cabal_down(ch, CABAL_OUTLAW))
		return;
	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
	{
		vch_next = vch->next_in_room;
		if (IS_NPC(vch))
			continue;
		if (vch == ch || vch->cabal == CABAL_OUTLAW)
			continue;

		init_affect(&af);
		af.aftype = AFT_POWER;
		af.where = TO_AFFECTS;
		af.type = sn;
		af.level = 30;
		af.duration = 48;
		af.modifier = 0;
		af.location = 0;
		af.bitvector = 0;
		af.affect_list_msg = str_dup("prevents usage of pandemonium");
		affect_to_char(ch, &af);
		send_to_char("Pandemonium has been perpetrated.\n\r", vch);

		/* confuse */
		if (!is_affected(vch, gsn_confuse) && !saves_spell(ch->level, vch, DAM_NONE))
		{
			init_affect(&af);
			af.aftype = AFT_SPELL;
			af.where = TO_AFFECTS;
			af.type = gsn_confuse;
			af.level = ch->level;
			af.duration = ch->level / 10;
			af.modifier = 0;
			af.location = 0;
			af.bitvector = 0;
			af.affect_list_msg = str_dup("inflicts confusion");
			affect_to_char(vch, &af);
		}
		/* garble */
		if (!is_affected(vch, gsn_garble) && !saves_spell(ch->level, vch, DAM_NONE))
		{
			init_affect(&af);
			af.aftype = AFT_SPELL;
			af.where = TO_AFFECTS;
			af.type = gsn_garble;
			af.level = ch->level;
			af.duration = ch->level / 5;
			af.modifier = 0;
			af.location = 0;
			af.bitvector = 0;
			af.affect_list_msg = str_dup("prevents casting and inflicts a deafened state");
			affect_to_char(vch, &af);
		}
		/* blindness */
		if (!is_affected(vch, gsn_blindness) && !saves_spell(ch->level, vch, DAM_ENERGY))
		{
			send_to_char("Swirling mass of chaotic energy blinds you", vch);
			init_affect(&af);
			af.aftype = AFT_SPELL;
			af.where = TO_AFFECTS;
			af.type = gsn_blindness;
			af.level = ch->level;
			af.duration = ch->level / 10;
			af.modifier = 0;
			af.location = 0;
			af.affect_list_msg = str_dup("inflicts blindness");
			af.bitvector = AFF_BLIND;
			affect_to_char(vch, &af);
		}
		/* change sex */
		if (!is_affected(vch, gsn_change_sex) && !saves_spell(ch->level, vch, DAM_NONE))
		{
			init_affect(&af);
			af.aftype = AFT_SPELL;
			af.where = TO_AFFECTS;
			af.type = gsn_change_sex;
			af.level = ch->level;
			af.duration = ch->level / 2;
			af.location = APPLY_SEX;
			af.modifier = -1;
			af.bitvector = 0;
			affect_to_char(vch, &af);
		}
		/* dispersal */
		send_to_char("The world spins around ou and you find yourself somewhere else!\n\r", vch);
		act("$n disappears!", vch, NULL, NULL, TO_ROOM);
		char_from_room(vch);
		char_to_room(vch, get_random_room(vch));
		act("$n appears!", vch, NULL, NULL, TO_ROOM);
		do_look(vch, "auto");
	}
}

void spell_dragonweapon(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	char buf[MSL], buf2[MSL], arg[MIL], arg2[MIL];
	OBJ_DATA *obj;
	AFFECT_DATA *paf;
	target_name = one_argument(target_name, arg);
	target_name = one_argument(target_name, arg2);

	buf[0] = '\0';
	buf2[0] = '\0';
	if (arg[0] == '\0')
	{
		send_to_char("Syntax: call 'dragonweapon' <type>\n\r", ch);
		return;
	}
	obj = create_object(get_obj_index(OBJ_VNUM_DRAGON_WEAPON), level);
	if (!str_cmp(arg, "dagger"))
	{
		free_string(obj->name);
		strcat(buf, "a gleaming dagger named 'Light'");
		obj->name = str_dup(buf);
		free_string(obj->short_descr);
		obj->short_descr = str_dup(buf);
		strcat(buf, " is here.");
		free_string(obj->description);
		obj->description = str_dup(capitalize(buf));
		obj->value[0] = WEAPON_DAGGER;
		obj->value[3] = attack_lookup("pierce");
		obj->weight = 10;
	}

	else if (!str_cmp(arg, "spear"))
	{
		free_string(obj->name);
		strcat(buf, "a finely-sharpened golden pike named 'Hope'");
		obj->name = str_dup(buf);
		free_string(obj->short_descr);
		obj->short_descr = str_dup(buf);
		strcat(buf, " is here.");
		free_string(obj->description);
		buf[0] = UPPER(buf[0]);
		obj->description = str_dup(buf);
		obj->value[0] = WEAPON_SPEAR;
		obj->value[3] = attack_lookup("pierce");
		obj->value[4] ^= (flag_value(weapon_type2, "twohands") != NO_FLAG
							  ? flag_value(weapon_type2, "twohands")
							  : 0);
		obj->weight = 30;
	}

	else if (!str_cmp(arg, "polearm"))
	{
		free_string(obj->name);
		strcat(buf, "a finely-sharpened golden poleaxe named 'Hope'");
		obj->name = str_dup(buf);
		free_string(obj->short_descr);
		obj->short_descr = str_dup(buf);
		strcat(buf, " is here.");
		free_string(obj->description);
		buf[0] = UPPER(buf[0]);
		obj->description = str_dup(buf);
		obj->value[0] = WEAPON_POLEARM;
		obj->value[3] = attack_lookup("slice");
		obj->value[4] ^= (flag_value(weapon_type2, "twohands") != NO_FLAG
							  ? flag_value(weapon_type2, "twohands")
							  : 0);
		obj->weight = 30;
	}

	else if (!str_cmp(arg, "mace"))
	{
		free_string(obj->name);
		strcat(buf, "a pulsing mace named 'Light'");
		obj->name = str_dup(buf);
		free_string(obj->short_descr);
		obj->short_descr = str_dup(buf);
		strcat(buf, " is here.");
		free_string(obj->description);
		buf[0] = UPPER(buf[0]);
		obj->description = str_dup(buf);
		obj->value[0] = WEAPON_MACE;
		obj->value[3] = attack_lookup("crush");
		obj->weight = 20;
	}
	else if (!str_cmp(arg, "flail"))
	{
		free_string(obj->name);
		strcat(buf, "a golden flail named 'Virtue'");
		obj->name = str_dup(buf);
		free_string(obj->short_descr);
		obj->short_descr = str_dup(buf);
		strcat(buf, " is here.");
		free_string(obj->description);
		buf[0] = UPPER(buf[0]);
		obj->description = str_dup(buf);
		obj->value[0] = WEAPON_FLAIL;
		obj->value[3] = attack_lookup("pound");
		obj->weight = 15;
	}
	else if (!str_cmp(arg, "whip"))
	{
		free_string(obj->name);
		strcat(buf, "a glowing whip named 'Honor'");
		obj->name = str_dup(buf);
		free_string(obj->short_descr);
		obj->short_descr = str_dup(buf);
		strcat(buf, " is here.");
		free_string(obj->description);
		buf[0] = UPPER(buf[0]);
		obj->description = str_dup(buf);
		obj->value[0] = WEAPON_WHIP;
		obj->value[3] = attack_lookup("whip");
		obj->weight = 15;
	}
	else if (!str_cmp(arg, "sword"))
	{
		free_string(obj->name);
		strcat(buf, "a finely-crafted sword named 'Virtue'");
		obj->name = str_dup(buf);
		free_string(obj->short_descr);
		obj->short_descr = str_dup(buf);
		strcat(buf, " is here.");
		free_string(obj->description);
		buf[0] = UPPER(buf[0]);
		obj->description = str_dup(buf);
		obj->value[0] = WEAPON_SWORD;
		obj->value[3] = attack_lookup("slash");
		obj->weight = 17;
	}
	else if (!str_cmp(arg, "two-handed sword"))
	{
		free_string(obj->name);
		strcat(buf, "a two-handed bastard sword named 'Virtue'");
		obj->name = str_dup(buf);
		free_string(obj->short_descr);
		obj->short_descr = str_dup(buf);
		strcat(buf, " is here.");
		free_string(obj->description);
		buf[0] = UPPER(buf[0]);
		obj->description = str_dup(buf);
		obj->value[0] = WEAPON_SWORD;
		obj->value[3] = attack_lookup("slash");
		obj->value[4] ^= (flag_value(weapon_type2, "twohands") != NO_FLAG
							  ? flag_value(weapon_type2, "twohands")
							  : 0);
		obj->weight = 17;
	}
	else if (!str_cmp(arg, "axe"))
	{
		free_string(obj->name);
		strcat(buf, "a fine battle axe named 'Honor'");
		obj->name = str_dup(buf);
		free_string(obj->short_descr);
		obj->short_descr = str_dup(buf);
		strcat(buf, " is here.");
		free_string(obj->description);
		buf[0] = UPPER(buf[0]);
		obj->description = str_dup(buf);
		obj->value[0] = WEAPON_AXE;
		obj->value[3] = attack_lookup("cleave");
		obj->weight = 23;
	}

	else if (!str_cmp(arg, "two-handed axe"))
	{
		free_string(obj->name);
		strcat(buf, "a two-handed battle axe named 'Honor'");
		obj->name = str_dup(buf);
		free_string(obj->short_descr);
		obj->short_descr = str_dup(buf);
		strcat(buf, " is here.");
		free_string(obj->description);
		buf[0] = UPPER(buf[0]);
		obj->description = str_dup(buf);
		obj->value[0] = WEAPON_AXE;
		obj->value[3] = attack_lookup("cleave");
		obj->value[4] ^= (flag_value(weapon_type2, "twohands") != NO_FLAG
							  ? flag_value(weapon_type2, "twohands")
							  : 0);
		obj->weight = 23;
	}
	else if (!str_cmp(arg, "staff"))
	{
		free_string(obj->name);
		strcat(buf, "a glowing staff named 'Honor'");
		obj->name = str_dup(buf);
		free_string(obj->short_descr);
		obj->short_descr = str_dup(buf);
		strcat(buf, " is here.");
		free_string(obj->description);
		buf[0] = UPPER(buf[0]);
		obj->description = str_dup(buf);
		obj->value[0] = WEAPON_STAFF;
		obj->value[3] = attack_lookup("smash");
		obj->value[4] ^= (flag_value(weapon_type2, "twohands") != NO_FLAG
							  ? flag_value(weapon_type2, "twohands")
							  : 0);
		obj->weight = 26;
	}
	else
	{
		send_to_char("You can't make a dragonweapon like that!\n\r", ch);
		send_to_char("Possible weapon choices: dagger, sword, axe, mace, staff, polearm, spear\n", ch);
		send_to_char("      whip,flail, two-handed axe, two-handed sword\n\r", ch);
		return;
	}
	obj->value[1] = ch->level / 10;
	obj->value[2] = ch->level / 4;
	obj->level = level;
	obj->timer = 24;
	paf = new_affect();
	paf->type = sn;
	paf->level = level;
	paf->duration = -1;
	paf->location = APPLY_HITROLL;
	paf->modifier = level / 10;
	paf->bitvector = 0;
	paf->next = obj->affected;
	obj->affected = paf;
	paf = new_affect();
	paf->type = sn;
	paf->level = level;
	paf->duration = -1;
	paf->location = APPLY_DAMROLL;
	paf->modifier = level / 10;
	paf->bitvector = 0;
	paf->next = obj->affected;
	obj->affected = paf;
	if (ch->pcdata->induct == CABAL_LEADER)
	{
		free_string(obj->name);
		strcat(buf2, "'Shadowbane' the Bringer of Light");
		obj->name = str_dup(buf2);
		free_string(obj->short_descr);
		obj->short_descr = str_dup(buf2);
		strcat(buf2, " is here.");
		free_string(obj->description);
		obj->description = str_dup(buf2);
		obj->timer = 0;
		obj->value[1] = 9;
		obj->value[2] = 5;
	}
	obj_to_char(obj, ch);
	act("You create $p.", ch, obj, NULL, TO_CHAR);
	act("$n creates $p.", ch, obj, NULL, TO_ROOM);
	WAIT_STATE(ch, PULSE_VIOLENCE);
}

void spell_dragonarmor(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	char buf[MSL], arg[MIL];
	OBJ_DATA *obj;
	AFFECT_DATA *paf;
	int ac_value;

	target_name = one_argument(target_name, arg);

	buf[0] = '\0';
	if (arg[0] == '\0')
	{
		send_to_char("Syntax: call 'dragonarmor' <type>\n\r", ch);
		send_to_char("Possible armor types: helm, legs, arms, hands, feet, shield, waist, wrists\n\r", ch);
		return;
	}

	obj = create_object(get_obj_index(OBJ_VNUM_DRAGONARMOR), level);
	ac_value = level / 10;

	if (!str_cmp(arg, "helm") || !str_cmp(arg, "head"))
	{
		free_string(obj->name);
		strcat(buf, "a gleaming dragonscale helm");
		obj->name = str_dup(buf);
		free_string(obj->short_descr);
		obj->short_descr = str_dup(buf);
		strcat(buf, " lies here.");
		free_string(obj->description);
		obj->description = str_dup(capitalize(buf));
		obj->wear_flags = ITEM_WEAR_HEAD;
		obj->weight = 15;
	}
	else if (!str_cmp(arg, "legs"))
	{
		free_string(obj->name);
		strcat(buf, "gleaming dragonscale leggings");
		obj->name = str_dup(buf);
		free_string(obj->short_descr);
		obj->short_descr = str_dup(buf);
		strcat(buf, " lie here.");
		free_string(obj->description);
		obj->description = str_dup(capitalize(buf));
		obj->wear_flags = ITEM_WEAR_LEGS;
		obj->weight = 20;
	}
	else if (!str_cmp(arg, "arms"))
	{
		free_string(obj->name);
		strcat(buf, "gleaming dragonscale sleeves");
		obj->name = str_dup(buf);
		free_string(obj->short_descr);
		obj->short_descr = str_dup(buf);
		strcat(buf, " lie here.");
		free_string(obj->description);
		obj->description = str_dup(capitalize(buf));
		obj->wear_flags = ITEM_WEAR_ARMS;
		obj->weight = 18;
	}
	else if (!str_cmp(arg, "hands") || !str_cmp(arg, "gloves"))
	{
		free_string(obj->name);
		strcat(buf, "gleaming dragonscale gauntlets");
		obj->name = str_dup(buf);
		free_string(obj->short_descr);
		obj->short_descr = str_dup(buf);
		strcat(buf, " lie here.");
		free_string(obj->description);
		obj->description = str_dup(capitalize(buf));
		obj->wear_flags = ITEM_WEAR_HANDS;
		obj->weight = 12;
	}
	else if (!str_cmp(arg, "feet") || !str_cmp(arg, "boots"))
	{
		free_string(obj->name);
		strcat(buf, "gleaming dragonscale boots");
		obj->name = str_dup(buf);
		free_string(obj->short_descr);
		obj->short_descr = str_dup(buf);
		strcat(buf, " lie here.");
		free_string(obj->description);
		obj->description = str_dup(capitalize(buf));
		obj->wear_flags = ITEM_WEAR_FEET;
		obj->weight = 14;
	}
	else if (!str_cmp(arg, "shield"))
	{
		free_string(obj->name);
		strcat(buf, "a gleaming dragonscale shield");
		obj->name = str_dup(buf);
		free_string(obj->short_descr);
		obj->short_descr = str_dup(buf);
		strcat(buf, " lies here.");
		free_string(obj->description);
		obj->description = str_dup(capitalize(buf));
		obj->wear_flags = ITEM_WEAR_SHIELD;
		obj->weight = 25;
		ac_value = (ac_value * 3) / 2; // Shields get better AC
	}
	else if (!str_cmp(arg, "waist") || !str_cmp(arg, "belt"))
	{
		free_string(obj->name);
		strcat(buf, "a gleaming dragonscale belt");
		obj->name = str_dup(buf);
		free_string(obj->short_descr);
		obj->short_descr = str_dup(buf);
		strcat(buf, " lies here.");
		free_string(obj->description);
		obj->description = str_dup(capitalize(buf));
		obj->wear_flags = ITEM_WEAR_WAIST;
		obj->weight = 8;
	}
	else if (!str_cmp(arg, "wrists") || !str_cmp(arg, "wrist"))
	{
		free_string(obj->name);
		strcat(buf, "gleaming dragonscale bracers");
		obj->name = str_dup(buf);
		free_string(obj->short_descr);
		obj->short_descr = str_dup(buf);
		strcat(buf, " lie here.");
		free_string(obj->description);
		obj->description = str_dup(capitalize(buf));
		obj->wear_flags = ITEM_WEAR_WRIST;
		obj->weight = 10;
	}
	else
	{
		send_to_char("You can't make dragonarmor like that!\n\r", ch);
		send_to_char("Possible armor types: helm, legs, arms, hands, feet, shield, waist, wrists\n\r", ch);
		extract_obj(obj);
		return;
	}

	// Set armor values (pierce, bash, slash, exotic)
	obj->value[0] = ac_value;
	obj->value[1] = ac_value;
	obj->value[2] = ac_value;
	obj->value[3] = ac_value;
	obj->level = level;
	obj->timer = 24;

	// Add hitroll bonus
	paf = new_affect();
	paf->type = sn;
	paf->level = level;
	paf->duration = -1;
	paf->location = APPLY_HITROLL;
	paf->modifier = level / 15;
	paf->bitvector = 0;
	paf->next = obj->affected;
	obj->affected = paf;

	// Add AC bonus
	paf = new_affect();
	paf->type = sn;
	paf->level = level;
	paf->duration = -1;
	paf->location = APPLY_AC;
	paf->modifier = -(level / 8);
	paf->bitvector = 0;
	paf->next = obj->affected;
	obj->affected = paf;

	obj_to_char(obj, ch);
	act("You forge $p from gleaming dragonscales!", ch, obj, NULL, TO_CHAR);
	act("$n forges $p from gleaming dragonscales!", ch, obj, NULL, TO_ROOM);
	WAIT_STATE(ch, PULSE_VIOLENCE);
}

void spell_scourge(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *vch;

	for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
	{
		if (is_same_group(vch, ch) || is_safe(ch, vch) || is_same_cabal(ch, vch))
			continue;
		if (number_percent() > 30)
			spell_blindness(skill_lookup("blindness"), level - 4, ch, vch, TAR_CHAR_OFFENSIVE);
		if (number_percent() > 30)
			spell_poison(skill_lookup("poison"), level - 4, ch, vch, TAR_CHAR_OFFENSIVE);
		if (number_percent() > 30)
			spell_weaken(skill_lookup("weaken"), level - 4, ch, vch, TAR_CHAR_OFFENSIVE);
	}
	spell_fireball(sn, level - 2, ch, NULL, TAR_CHAR_OFFENSIVE);
	return;
}

void do_chameleon(CHAR_DATA *ch, char *argument)
{
	int sn_fog, sn_fire, skill;

	sn_fog = skill_lookup("faerie fog");
	sn_fire = skill_lookup("faerie fire");

	if ((skill = get_skill(ch, gsn_chameleon) == 0))
		return send_to_char("Huh?\n\r", ch);

	if (is_affected(ch, sn_fog) || is_affected(ch, sn_fire))
	{
		send_to_char("You stand out too much to blend with your surroundings.\n\r", ch);
		return;
	}

	if ((ch->in_room->sector_type == SECT_CITY) || (ch->in_room->sector_type == SECT_INSIDE))
	{
		send_to_char("You would feel too dirty if you took on the colors of civilization.\n\r", ch);
		return;
	}

	if (IS_AFFECTED(ch, AFF_CAMOUFLAGE))
		return send_to_char("You are already camouflaged.\n\r", ch);
	{
		send_to_char("You change color to blend with your surroundings.\n\r", ch);
		SET_BIT(ch->affected_by, AFF_CAMOUFLAGE);
		check_improve(ch, gsn_chameleon, TRUE, 1);
		WAIT_STATE(ch, 12);
	}
	return;
}

void do_insectswarm(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	int chance;
	AFFECT_DATA af;
	char buf[MAX_STRING_LENGTH];

	one_argument(argument, arg);

	if ((chance = get_skill(ch, gsn_insect_swarm)) == 0 || (!IS_NPC(ch) && ch->level < skill_table[gsn_insect_swarm].skill_level[ch->class]))
	{
		send_to_char("You don't know how to do that.\n\r", ch);
		return;
	}

	if (arg[0] == '\0')
	{
		send_to_char("Call insects on who?\n\r", ch);
		return;
	}

	if ((victim = get_char_room(ch, arg)) == NULL)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (is_affected(victim, gsn_insect_swarm))
	{
		send_to_char("They are already being swarmed.\n\r", ch);
		return;
	}

	if (ch->mana < 50)
	{
		send_to_char("You can't find the energy.\n\r", ch);
		return;
	}

	if (victim == ch)
	{
		send_to_char("You can't call that on yourself!\n\r", ch);
		return;
	}
	if (is_safe(ch, victim))
		return;

	if (!isInWilderness(ch))
	{
		send_to_char("You must be in more of a wilderness area to call insects.\n\r", ch);
		return;
	}

	chance += (ch->drain_level + ch->level - victim->level - victim->drain_level) * 2;

	if (number_percent() < (get_skill(ch, gsn_insect_swarm) - 15))
	{
		sprintf(buf, "Help! %s is calling down insects on me!", PERS(ch, victim));
		do_myell(victim, buf);

		WAIT_STATE(ch, PULSE_VIOLENCE);
		ch->mana -= 50;
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_POWER;
		af.type = gsn_insect_swarm;
		af.level = ch->level;
		af.duration = ch->level / 6;
		af.location = APPLY_DEX;
		af.modifier = -3;
		af.bitvector = 0;
		af.owner_name = str_dup(ch->original_name);
		affect_to_char(victim, &af);
		send_to_char("You frantically wave your arms as a cloud of insects descends upon you.\n\r", victim);
		act("$n waves $s arms as a cloud of insects descends upon $m.", victim, NULL, NULL, TO_ROOM);
		check_improve(ch, gsn_insect_swarm, TRUE, 1);
		return;
	}
	else
	{
		WAIT_STATE(ch, PULSE_VIOLENCE);
		ch->mana -= 25;

		send_to_char("You fail to call down insects.\n\r", ch);
		check_improve(ch, gsn_insect_swarm, FALSE, 1);
	}
}

void spell_beast_call(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *gch;
	CHAR_DATA *mob, *victim = (CHAR_DATA *)vo, *wolf;
	char buf[MAX_STRING_LENGTH];
	AFFECT_DATA timer;

	if (cabal_down(ch, CABAL_SYLVAN) && !IS_IMMORTAL(ch))
		return;

	for (wolf = char_list; wolf != NULL; wolf = wolf->next)
	{
		if ((wolf->master == ch) && (wolf->pIndexData->vnum == MOB_VNUM_BEAST))
		{
			return send_to_char("You can't call for more than one beast at a time.\n\r", ch);
			break;
		}
		else
		{
			break;
		}
	}

	for (gch = char_list; gch != NULL; gch = gch->next)
	{
		if (IS_NPC(gch) && IS_AFFECTED(gch, AFF_CHARM) && gch->master == ch &&
			gch->pIndexData->vnum == MOB_VNUM_BEAST)
		{
			send_to_char("You cannot call upon more than one beast at a time!\n\r", ch);
			return;
		}
	}

	if (!(ch->in_room->sector_type == SECT_CITY || ch->in_room->sector_type == SECT_INSIDE))
	{
		if (!is_affected(ch, gsn_beast_timer))
		{
			mob = create_mobile(get_mob_index(MOB_VNUM_BEAST));
			mob->level = ch->level;
			mob->max_hit = dice(ch->level, 2 * ch->level);
			mob->hit = mob->max_hit;
			mob->damroll = dice(ch->level / 2, 3);
			char_to_room(mob, ch->in_room);
			add_follower(mob, ch);
			mob->leader = ch;
			SET_BIT(mob->affected_by, AFF_CHARM);
			act("You call out to nature and $N responds.", ch, 0, mob, TO_CHAR);
			act("$N responds to $n's call!", ch, 0, mob, TO_ROOM);

			if (!IS_NPC(victim))
			{
				sprintf(buf, "%s is calling the beasts of the wilds on me!", ch->name);
				do_myell(victim, buf);
			}
			multi_hit(mob, victim, TYPE_UNDEFINED);
			init_affect(&timer);
			timer.where = TO_AFFECTS;
			timer.aftype = AFT_POWER;
			timer.type = gsn_beast_timer;
			timer.level = ch->level;
			timer.duration = 12;
			timer.modifier = 0;
			timer.bitvector = 0;
			timer.affect_list_msg = str_dup("prevents usage of beast call");
			affect_to_char(ch, &timer);
			return;
		}

		else
		{
			return send_to_char("You cannot call upon the beasts of the wilds yet.\n\r", ch);
		}
	}

	else
	{
		return send_to_char("You must be in a wilderness area to call beasts.\n\r", ch);
	}
}

void spell_wall_of_thorns(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	ROOM_AFFECT_DATA raf;

	if (cabal_down(ch, CABAL_SYLVAN) && !IS_IMMORTAL(ch))
		return;

	if (is_affected(ch, sn))
	{
		send_to_char("It's too soon to make another wall of thorns.\n\r", ch);
		return;
	}

	if (!isInWilderness(ch))
	{
		send_to_char("There isn't enough foliage here to call a wall of thorns.\n\r", ch);
		return;
	}

	if (is_affected_room(ch->in_room, sn))
	{
		send_to_char("There is already a wall of thorns constructed here.\n\r", ch);
		return;
	}

	if (number_range(1, 100) > ch->pcdata->learned[sn])
	{
		send_to_char("You attempt to create a wall of thorns but fail.\n\r", ch);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_POWER;
	af.type = sn;
	af.level = ch->level;
	af.duration = 48;
	af.modifier = 0;
	af.bitvector = 0;
	af.affect_list_msg = str_dup("prevents usage of wall of thorns");
	affect_to_char(ch, &af);

	init_affect_room(&raf);
	raf.where = TO_ROOM_AFFECTS;
	raf.aftype = AFT_POWER;
	raf.type = sn;
	raf.level = ch->level;
	raf.duration = ch->level / 5;
	raf.bitvector = AFF_ROOM_WALL_THORNS;
	affect_to_room(ch->in_room, &raf);

	send_to_char("Using the nearby foliage, you craft a makeshift wall of thorns here to attack intruders.\n", ch);
	act("Using the nearby foliage, $n crafts a makeshift wall of thorns here to attack intruders.", ch, NULL, NULL, TO_ROOM);
}

void spell_defiance(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	if (is_affected(ch, sn))
	{
		send_to_char("You are already defiant.\n\r", ch);
		return;
	}

	//	if (ch->hit > ch->max_hit)
	//		return send_to_char("You are still feeling the effects of your defiance.\n\r",ch);

	ch->hit += 250;

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_POWER;
	af.type = sn;
	af.level = ch->level;
	af.duration = 20;
	af.modifier = 0;
	af.bitvector = 0;
	affect_to_char(ch, &af);
	/*
	 af.location  = APPLY_HIT;
	 af.modifier  = 250;
	 affect_to_char( ch, &af );
	*/
	send_to_char("You feel more defiant!\n\r", ch);
	return;
}

void spell_ward_diminution(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af, timer;

	if (is_affected(ch, sn))
		return send_to_char("You are already shielded by a ward of diminution.\n\r", ch);
	if (is_affected(ch, gsn_ward_timer))
		return send_to_char("You cannot yet gather the energies to summon another ward of diminution.\n\r", ch);

	if (cabal_down(ch, CABAL_ARCANA))
		return;

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_POWER;
	af.type = sn;
	af.level = level;
	af.duration = 6;
	af.location = 0;
	af.modifier = 0;
	af.bitvector = 0;
	affect_to_char(ch, &af);

	init_affect(&timer);
	timer.where = TO_AFFECTS;
	timer.aftype = AFT_POWER;
	timer.type = gsn_ward_timer;
	timer.level = level;
	timer.duration = 24;
	timer.location = 0;
	timer.modifier = 0;
	timer.bitvector = 0;
	timer.affect_list_msg = str_dup("prevents usage of ward of diminution");
	affect_to_char(ch, &timer);

	act("A shield of pure mana forms around you.", ch, 0, 0, TO_CHAR);
	act("A shield of pure mana forms around $n.", ch, 0, 0, TO_ROOM);
	WAIT_STATE(ch, PULSE_VIOLENCE);
	return;
}

void spell_eavesdrop(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	int cabal;

	if (cabal_down(ch, CABAL_ANCIENT))
		return;

	if (!str_cmp(target_name, "none"))
	{
		if (ch->eavesdrop == 0)
		{
			send_to_char("You are not currently eavesdropping on any cabal.\n\r", ch);
		}
		else
		{
			printf_to_char(ch, "You close your spy network to the %s cabal.\n\r", cabal_table[ch->eavesdrop].name);
			ch->eavesdrop = 0;
		}
		return;
	}

	if ((cabal = cabal_lookup(target_name)) == 0)
	{
		send_to_char("Syntax: call eavesdrop <cabal>\n\r", ch);
		return;
	}

	if (cabal == cabal_lookup("ancient"))
	{
		send_to_char("You cannot eavesdrop on yourself.\n\r", ch);
		return;
	}

	ch->eavesdrop = cabal;
	printf_to_char(ch, "You open your spy network with the %s cabal.\n\r", cabal_table[ch->eavesdrop].name);
	return;
}

void spell_hallow_cry(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	ROOM_AFFECT_DATA raf;
	AFFECT_DATA af;
	int dur = 0;

	if (cabal_down(ch, CABAL_RAGER))
		return;

	if (is_affected_room(ch->in_room, sn))
		return send_to_char("This ground is already halllowed.\n\r", ch);

	if (is_affected(ch, sn))
		return send_to_char("You aren't ready to call upon this sacred power again.\n\r", ch);

	dur = number_range(8, 16);

	init_affect_room(&raf);
	raf.where = TO_ROOM_AFFECTS;
	raf.type = sn;
	raf.level = level;
	raf.aftype = AFT_POWER;
	raf.duration = dur;
	raf.bitvector = 0;
	raf.owner_name = str_dup(ch->original_name);
	raf.location = APPLY_ROOM_MANA;
	raf.modifier = -100;
	affect_to_room(ch->in_room, &raf);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = sn;
	af.level = level;
	af.aftype = AFT_POWER;
	af.duration = dur + 2;
	af.bitvector = 0;
	af.location = 0;
	af.modifier = 0;
	affect_to_char(ch, &af);

	act("You cry out with an ancestral war cry, hallowing the ground you stand upon!", ch, 0, 0, TO_CHAR);
	act("$n cries out with an ancestral war cry, hallowing this ground!", ch, 0, 0, TO_ROOM);
	return;
}

void spell_guard_call(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *guard;
	AFFECT_DATA af;
	CHAR_DATA *check;
	int i, k;

	if (cabal_down(ch, CABAL_ENFORCER))
		return;

	if (is_affected(ch, sn))
	{
		send_to_char("You can't call more guard's yet.\n\r", ch);
		return;
	}

	for (check = char_list; check != NULL; check = check->next)
	{
		if (IS_NPC(check))
			if ((check->master == ch) && (check->pIndexData->vnum == MOB_VNUM_ENFORCER))
			{
				send_to_char("You still have guards under your command!\n\r", ch);
				return;
			}
	}

	if (ch->in_room->cabal == CABAL_OUTLAW)
	{
		send_to_char("There are no guards available within this place of outlaws!\n\r", ch);
		return;
	}

	if (!IS_SET(ch->in_room->room_flags, ROOM_LAW))
	{
		return send_to_char("You cannot call guards outside of protected areas!\n\r", ch);
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_POWER;
	af.level = level;
	af.location = 0;
	af.modifier = 0;
	af.duration = 6;
	af.affect_list_msg = str_dup("prohibits summoning of guards");
	af.bitvector = 0;
	af.type = sn;
	affect_to_char(ch, &af);

	do_yell(ch, "Guards! Guards!");
	send_to_char("You shout out for guards!\n\r", ch);
	send_to_char("A guard contingent arrives to aid you.\n\r", ch);

	for (i = 0; i < 3; i++)
	{
		guard = create_mobile(get_mob_index(MOB_VNUM_ENFORCER));
		for (k = 0; k < MAX_STATS; k++)
			guard->perm_stat[k] = UMIN(25, (ch->level / 10) + 17);
		guard->armor[3] = interpolate(guard->level, 100, 0);
		guard->level = ch->level;
		guard->hitroll = (ch->level / 10) * 2 + 3;
		guard->damroll = (ch->level / 5) * 2;
		guard->damage[DICE_NUMBER] = (ch->level / 5);
		guard->damage[DICE_TYPE] = 5;
		guard->alignment = ch->alignment;
		guard->max_hit = ch->max_hit / 2;
		guard->hit = guard->max_hit;
		guard->max_move = ch->max_move;
		guard->move = guard->max_move;
		char_to_room(guard, ch->in_room);
		add_follower(guard, ch);
		guard->leader = ch;
		SET_BIT(guard->affected_by, AFF_CHARM);
	}
	act("A guard contingent arrives to $n's call.", ch, 0, 0, TO_ROOM);
	return;
}

void do_wanted(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *criminal;
	char arg[MAX_INPUT_LENGTH], status[MAX_INPUT_LENGTH];
	char *next_arg;
	bool wanted;

	wanted = FALSE;
	next_arg = one_argument(argument, arg);
	one_argument(next_arg, status);

	if (IS_NPC(ch))
		return;

	if (ch->cabal != cabal_lookup("enforcer") && !IS_IMMORTAL(ch))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (!str_cmp(status, "yes"))
		wanted = TRUE;
	else if (!str_cmp(status, "no"))
		wanted = FALSE;
	else
	{
		send_to_char("Syntax: wanted <player> <yes/no>\n\r", ch);
		return;
	}

	if ((criminal = get_char_world(ch, arg)) == NULL)
	{
		send_to_char("That person isn't here.\n\r", ch);
		return;
	}

	if (IS_NPC(criminal))
	{
		send_to_char("You can only flag other players.\n\r", ch);
		return;
	}

	if (wanted)
	{
		if (IS_SET(criminal->act, PLR_CRIMINAL))
		{
			send_to_char("That person is already a criminal.\n\r", ch);
			return;
		}
		else
		{
			SET_BIT(criminal->act, PLR_CRIMINAL);
			send_to_char("{gYou are now WANTED!!{x\n\r", criminal);
			printf_to_char(ch, "{g%s is now a criminal.{x\n\r", criminal->original_name);
			criminal->pcdata->wanteds += 1;
			return;
		}
	}

	if (!IS_SET(criminal->act, PLR_CRIMINAL))
	{
		send_to_char("That person is not a criminal.\n\r", ch);
		return;
	}
	else
	{
		REMOVE_BIT(criminal->act, PLR_CRIMINAL);
		send_to_char("{gYou are no longer wanted.{x\n\r", criminal);
		printf_to_char(ch, "{g%s is no longer a criminal.{x\n\r", criminal->original_name);
		criminal->pcdata->wanteds -= 1;
	}
	return;
}

void spell_vigilance(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	if (cabal_down(ch, CABAL_ENFORCER))
		return;

	if (is_affected(ch, sn))
		return send_to_char("You are already vigilant.\n\r", ch);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_POWER;
	af.type = sn;
	af.modifier = 0;
	af.location = 0;
	af.bitvector = AFF_DETECT_INVIS | AFF_DETECT_HIDDEN;
	af.level = level;
	af.duration = ch->level / 2;
	af.affect_list_msg = str_dup("grants the ability to detect the hidden and the invisible");
	affect_to_char(ch, &af);
	send_to_char("You become more alert.\n\r", ch);
	return;
}

void spell_shield_order(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	OBJ_DATA *shield;
	AFFECT_DATA af;
	int timer = 0;

	if (is_affected(ch, sn))
		return send_to_char("You aren't ready to create another shield of order yet.\n\r", ch);

	shield = create_object(get_obj_index(OBJ_VNUM_SHIELD_ORDER), 0);
	if (!shield)
		return;

	timer = number_range(24, 36);
	shield->timer = timer;
	obj_to_char(shield, ch);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = sn;
	af.duration = timer + 2;
	af.affect_list_msg = str_dup("prevents creation of another shield of order");
	af.aftype = AFT_POWER;
	affect_to_char(ch, &af);

	free_string(shield->owner);
	shield->owner = str_dup(ch->original_name);

	act("The Immortal lords of Enforcer provide you with $p.", ch, shield, 0, TO_CHAR);
	return;
}

void spell_chromatic_fire(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	char *spell = NULL;
	AFFECT_DATA af;
	int newsn;

	act("A fiery mass appears at your bidding and flies a zig-zag course to $N.", ch, NULL, victim, TO_CHAR);
	act("A large ball of strange flame appears in front of $n and flies around before striking you!", ch, NULL, victim, TO_VICT);
	act("A fiery mass appears at $n's bidding and flies around the room, nearly hitting you, before striking $N.", ch, NULL, victim, TO_NOTVICT);

	switch (number_range(1, 35))
	{
	case (1):
		spell = str_dup("blind");
		break;
	case (2):
		spell = str_dup("curse");
		break;
	case (3):
		spell = str_dup("faerie fog");
		break;
	case (4):
		spell = str_dup("rejuvenate");
		break;
	case (5):
		spell = str_dup("resist weapon");
		break;
	case (6):
		spell = str_dup("dispel magic");
		break;
	case (7):
		spell = str_dup("acid blast");
		break;
	case (8):
		spell = str_dup("confuse");
		break;
	case (9):
		spell = str_dup("slow");
		break;
	case (10):
		spell = str_dup("haste");
		break;
	case (11):
		spell = str_dup("garble");
		break;
	case (12):
		spell = str_dup("icelance");
		break;
	case (13):
		spell = str_dup("enlarge");
		break;
	case (14):
		spell = str_dup("poison");
		break;
	case (15):
		spell = str_dup("power word kill");
		break;
	case (16):
		spell = str_dup("forget");
		break;
	case (17):
		spell = str_dup("garble");
		break;
	case (18):
		spell = str_dup("shield");
		break;
	case (19):
		spell = str_dup("armor");
		break;
	case (20):
		spell = str_dup("fireball");
		break;
	case (21):
		spell = str_dup("heal");
		break;
	case (22):
		spell = str_dup("cure critical");
		break;
	case (23):
		spell = str_dup("sunbolt");
		break;
	case (24):
		spell = str_dup("chill touch");
		break;
	case (25):
		spell = str_dup("armor");
		break;
	case (26):
		spell = str_dup("invisibility");
		break;
	case (27):
		spell = str_dup("stone skin");
		break;
	case (28):
		spell = str_dup("stone skin");
		break;
	case (29):
		spell = str_dup("icy needles");
		break;
	case (30):
		spell = str_dup("infravision");
		break;
	case (31):
		spell = str_dup("detect invis");
		break;
	case (32):
		spell = str_dup("plague");
		break;
	case (33):
		spell = str_dup("mental knife");
		break;
	case (34):
		spell = str_dup("resist acid");
		break;
	case (35):
		spell = str_dup("wither");
		break;
	}

	if ((newsn = skill_lookup(spell)) < 0)
		return;

	(*skill_table[newsn].spell_fun)(newsn, ch->level - 4, ch, victim, TARGET_CHAR);

	if (number_range(1, 4) == 1 && !is_affected(victim, skill_lookup("chromatic fire")))
	{
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_POWER;
		af.type = sn;
		af.level = ch->level;
		af.duration = ch->level / 15;
		af.modifier = 0;
		af.location = APPLY_NONE;
		af.bitvector = 0;
		affect_to_char(victim, &af);
		send_to_char("You are engulfed in the chromatic fire.", ch);
	}
	return;
}
