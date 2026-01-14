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

void spell_self_immolation(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	if (is_affected(ch, sn))
	{
		send_to_char("You are already immolating.\n\r", ch);
		return;
	}

	init_affect(&af);
	af.type = sn;
	af.aftype = AFT_SPELL;
	af.duration = ch->level / 4;
	af.modifier = 0;
	af.bitvector = 0;
	af.where = TO_AFFECTS;
	af.affect_list_msg = str_dup("sears you and opponents who strike at you");
	af.location = 0;
	af.owner_name = str_dup(ch->original_name);
	affect_to_char(ch, &af);

	act("$n suddenly bursts into flames!", ch, NULL, NULL, TO_ROOM);
	act("You suddenly burst into flames!", ch, NULL, NULL, TO_CHAR);
}

void spell_shield_of_bubbles(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, gsn_shield_of_bubbles))
	{
		if (victim == ch)
			send_to_char("You are already surrounded in a shield of bubbles.\n\r", ch);
		else
			act("$N is already surrounded by a shield of bubbles.", ch, NULL, victim, TO_CHAR);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = level;
	af.duration = 18;
	af.location = APPLY_DAM_MOD;
	af.modifier = -3;
	af.affect_list_msg = str_dup("grants immunity to drowning attacks");
	af.bitvector = 0;
	affect_to_char(victim, &af);
	af.location = 0;
	af.modifier = 0;
	af.affect_list_msg = str_dup("reduces damage taken by 3%");
	affect_to_char(victim, &af);
	act("A protective shield of bubbles envelops $n.", victim, NULL, NULL, TO_ROOM);
	send_to_char("A protective shield of bubbles envelops you.\n\r", victim);
	return;
}

void spell_shield_of_flames(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, gsn_shield_of_flames))
	{
		if (victim == ch)
			send_to_char("You are already surrounded in a shield of flames.\n\r", ch);
		else
			act("$N is already surrounded by a shield of flames.", ch, NULL, victim, TO_CHAR);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = level;
	af.duration = 18;
	af.location = APPLY_DAM_MOD;
	af.modifier = -3;
	af.affect_list_msg = str_dup("grants immunity to fire and heat attacks");
	af.bitvector = 0;
	affect_to_char(victim, &af);
	af.location = 0;
	af.modifier = 0;
	af.affect_list_msg = str_dup("reduces damage taken by 3%");
	affect_to_char(victim, &af);
	act("A shield of flames flares into existence around $n.", victim, NULL, NULL, TO_ROOM);
	send_to_char("A shield of flames flares into existence around you.\n\r", victim);
	return;
}

void spell_shield_of_frost(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, gsn_shield_of_frost))
	{
		if (victim == ch)
			send_to_char("You are already surrounded in a shield of frost.\n\r", ch);
		else
			act("$N is already surrounded by a shield of frost.", ch, NULL, victim, TO_CHAR);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = level;
	af.duration = 18;
	af.location = APPLY_DAM_MOD;
	af.affect_list_msg = str_dup("grants immunity to cold attacks");
	af.modifier = -3;
	af.bitvector = 0;
	affect_to_char(victim, &af);
	af.location = 0;
	af.modifier = 0;
	af.affect_list_msg = str_dup("reduces damage taken by 3%");
	affect_to_char(victim, &af);
	act("The temperature around $n drops suddenly and forms a shield of frost about $m.", victim, NULL, NULL, TO_ROOM);
	send_to_char("The temperature around you drops suddenly and forms a shield of frost about you.\n\r", victim);
	return;
}

void spell_shield_of_lightning(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, gsn_shield_of_lightning))
	{
		if (victim == ch)
			send_to_char("You are already surrounded in a shield of lightning.\n\r", ch);
		else
			act("$N is already surrounded by a shield of lightning.", ch, NULL, victim, TO_CHAR);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = level;
	af.duration = 18;
	af.location = APPLY_DAM_MOD;
	af.modifier = -3;
	af.affect_list_msg = str_dup("grants immunity to lightning attacks");
	af.bitvector = 0;
	affect_to_char(victim, &af);
	af.location = 0;
	af.modifier = 0;
	af.affect_list_msg = str_dup("reduces damage taken by 3%");
	affect_to_char(victim, &af);
	act("A shield of lightning crackles into existence around $n.", victim, NULL, NULL, TO_ROOM);
	send_to_char("A shield of lightning crackles into existence around you.\n\r", victim);
	return;
}

void spell_shield_of_wind(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, gsn_shield_of_wind))
	{
		if (victim == ch)
			send_to_char("You are already surrounded in a shield of wind.\n\r", ch);
		else
			act("$N is already surrounded by a shield of wind.", ch, NULL, victim, TO_CHAR);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = level;
	af.duration = 18;
	af.location = APPLY_DAM_MOD;
	af.affect_list_msg = str_dup("grants immunity to wind attacks");
	af.modifier = -3;
	af.bitvector = 0;
	affect_to_char(victim, &af);
	af.location = 0;
	af.modifier = 0;
	af.affect_list_msg = str_dup("reduces damage taken by 3%");
	affect_to_char(victim, &af);
	act("Strong winds coalesce about $n in a shield of wind.", victim, NULL, NULL, TO_ROOM);
	send_to_char("Strong winds coalesce about you in a shield of wind.\n\r", victim);
	return;
}

void spell_shield_of_dust(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, gsn_shield_of_dust))
	{
		if (victim == ch)
			send_to_char("You are already surrounded in a shield of dust.\n\r", ch);
		else
			act("$N is already surrounded by a shield of dust.", ch, NULL, victim, TO_CHAR);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = level;
	af.duration = 18;
	af.location = APPLY_DAM_MOD;
	af.affect_list_msg = str_dup("grants immunity to dirt and dust attacks");
	af.modifier = -3;
	af.bitvector = 0;
	affect_to_char(victim, &af);
	af.location = 0;
	af.modifier = 0;
	af.affect_list_msg = str_dup("reduces damage taken by 3%");
	affect_to_char(victim, &af);
	act("The dust picks up from the earth, forming a shield of dust around $n.", victim, NULL, NULL, TO_ROOM);
	send_to_char("The dust picks up from the earth, forming a shield of dust around you.\n\r", victim);
	return;
}

void spell_hydrolic_ram(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim;
	int dam;
	int chance;

	victim = (CHAR_DATA *)vo;

	if (victim == NULL)
		victim = ch->fighting;

	dam = dice(level, 3) + level;

	act("You fire a concentrated jet of water at $N!", ch, NULL, victim, TO_CHAR);
	act("$n fires a concentrated jet of water at $N!", ch, NULL, victim, TO_NOTVICT);
	act("$n fires a concentrated jet of water at you!", ch, NULL, victim, TO_VICT);
	damage_old(ch, victim, dam, sn, DAM_DROWNING, TRUE);

	chance = get_skill(ch, sn);

	if (number_percent() > chance)
		return;

	act("Your hydrolic ram blasts $N off $S feet!", ch, NULL, victim, TO_CHAR);
	act("$n's hydrolic ram blasts you off your feet!", ch, NULL, victim, TO_VICT);
	act("$n's hydrolic ram knocks $N off $S feet!", ch, NULL, victim, TO_NOTVICT);

	WAIT_STATE(victim, PULSE_VIOLENCE);
}

void spell_spontanious_combustion(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int dam;

	act("$n screams in agony as flames engulf $s body.", victim, 0, 0, TO_ROOM);
	send_to_char("You feel a sudden intense pain throughout your body as you are engulfed in flames!\n\r", victim);
	dam = dice(level, 10) + 3 * ch->level;
	damage_old(ch, victim, saves_spell(level, victim, DAM_FIRE) ? dam / 2 : dam, sn, DAM_FIRE, TRUE);
	return;
}

void spell_shockwave(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	char buf[MAX_STRING_LENGTH];
	int lag;

	WAIT_STATE(ch, 2 * PULSE_VIOLENCE);

	act("$n stomps $s foot and the ground begins to shake!", ch, NULL, NULL, TO_ROOM);
	act("You stomp your foot and the ground begins to shake!", ch, NULL, NULL, TO_CHAR);

	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
	{
		vch_next = vch->next_in_room;
		if (is_same_group(vch, ch))
			continue;
		if (!IS_NPC(ch) && !IS_NPC(vch) && (ch->fighting == NULL || vch->fighting == NULL))
		{
			switch (number_range(0, 2))
			{
			case (0):
			case (1):
				sprintf(buf, "Die, %s you sorcerous dog!", PERS(ch, vch));
				break;
			case (2):
				sprintf(buf, "Help! %s is casting a spell on me!", PERS(ch, vch));
			}
			if (vch != ch)
				do_myell(vch, buf);
		}
		if (vch->position != POS_FIGHTING)
		{
			vch->position = POS_FIGHTING;
			vch->fighting = ch;
		}
		if (ch->position != POS_FIGHTING)
		{
			ch->position = POS_FIGHTING;
			ch->fighting = vch;
		}
		lag = number_range(0, 3);
		if (lag && !IS_AFFECTED(vch, AFF_FLYING))
		{
			act("$n is knocked of $s feet by the shockwave!", vch, NULL, NULL, TO_ROOM);
			act("You are knocked off your feet by the shockwave!", vch, NULL, NULL, TO_CHAR);
			WAIT_STATE(vch, lag * PULSE_VIOLENCE);
		}
	}
}

void spell_static_charge(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, gsn_static_charge))
	{
		send_to_char("You are already fully charged!\n\r", victim);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = gsn_static_charge;
	af.level = level;
	af.duration = number_range(1, 5);
	af.affect_list_msg = str_dup("charges you with static for one attack");
	af.modifier = 0;
	af.location = 0;
	af.bitvector = 0;
	affect_to_char(victim, &af);
	send_to_char("You charge yourself with static electricity!\n\r", victim);
	act("$n begins to glow softly as $s hair begins to stand on end.", victim, NULL, NULL, TO_ROOM);
	return;
}

void spell_flicker(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	ROOM_INDEX_DATA *pRoomIndex;
	int chance;
	int count;
	chance = get_skill(ch, sn);

	chance -= 10;
	if (ch->position == POS_FIGHTING)
	{
		chance -= 50;
	}

	if (number_percent() > chance)
	{
		send_to_char("You begin to flicker but nothing happens.\n\r", ch);
		act("$n seems to flicker for a moment but nothing happens.", ch, 0, 0, TO_ROOM);
		return;
	}
	if (ch->in_room->vnum == 23610 || ch->in_room->vnum == ROOM_FORCE_DUEL)
	{
		send_to_char("You fail.\n\r", ch);
		return;
	}
	count = 0; // keep the game from hanging if it never finds a room
	while (TRUE)
	{
		count++;
		if (count > 30000)
		{
			send_to_char("you fail.\n\r", ch);
			return;
		}
		pRoomIndex = get_room_index(number_range(0, 30000));
		if (pRoomIndex != NULL)
			if (pRoomIndex->area == ch->in_room->area && !IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE) && !IS_SET(pRoomIndex->room_flags, ROOM_NO_RECALL) && (pRoomIndex->guild == 0) && can_see_room(ch, pRoomIndex) && !IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY) && ch->in_room->vnum != 1212 && pRoomIndex->vnum != 1212)
				break;
	}

	act("$n begins to flicker and suddenly dissapears!", ch, 0, 0, TO_ROOM);
	send_to_char("You begin to flicker and suddenly find yourself somewhere else!\n\r", ch);
	char_from_room(ch);
	char_to_room(ch, pRoomIndex);
	act("The likeness of $n flickers into being.", ch, 0, 0, TO_ROOM);
	do_look(ch, "auto");
	return;
}

void spell_smoke(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (IS_AFFECTED(victim, AFF_BLIND))
	{
		send_to_char("They are already blind.\n\r", ch);
		return;
	}

	act("$n breathes a puff of smoke into $N's face!", ch, NULL, victim, TO_NOTVICT);
	act("$n breaths a puff of smoke into your face!", ch, NULL, victim, TO_VICT);
	act("You breath a puff of smoke into $N's face!", ch, NULL, victim, TO_CHAR);

	if (saves_spell(level, victim, DAM_OTHER))
	{
		act("$n manages to keep the smoke out of their eyes.", victim, NULL, NULL, TO_ROOM);
		act("You manage to keep the smoke out of your eyes.", victim, NULL, NULL, TO_CHAR);
		return;
	}

	if (victim->race == race_lookup("lich"))
	{
		act("$n doesn't seem to be affected by the smoke.", victim, NULL, NULL, TO_ROOM);
		act("The smoke doesn't seem to affect you.", victim, NULL, NULL, TO_CHAR);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = level;
	af.location = APPLY_HITROLL;
	af.modifier = -4;
	af.duration = 1;
	af.bitvector = AFF_BLIND;
	affect_to_char(victim, &af);
	send_to_char("The smoke gets in your eyes and blinds you!\n\r", victim);
	act("$n is blinded by the smoke.", victim, NULL, NULL, TO_ROOM);
	return;
}

void spell_nullify_sound(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (ch == victim)
	{
		send_to_char("That would not be wise.\n\r", ch);
		return;
	}

	if (is_affected(victim, sn))
	{
		act("$N cannot hear anything already.", ch, NULL, victim, TO_CHAR);
		return;
	}

	if (saves_spell(level, victim, DAM_NONE))
		return;

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = level;
	af.duration = 3;
	af.modifier = 0;
	af.location = 0;
	af.bitvector = 0;
	affect_to_char(victim, &af);

	act("You have destroyed all sound near $N!", ch, NULL, victim, TO_CHAR);
	send_to_char("The world suddenly grows very silent!\n\r", victim);
}

void spell_displacement(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (ch != victim)
	{
		send_to_char("You can only cast this spell on yourself.\n\r", ch);
		return;
	}

	if (is_affected(ch, sn))
	{
		act("Your true location is already masked.", ch, NULL, NULL, TO_CHAR);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = level;
	af.duration = 10;
	af.modifier = 0;
	af.location = 0;
	af.affect_list_msg = str_dup("grants a chance to avoid attacks completely");
	af.bitvector = 0;
	affect_to_char(victim, &af);

	act("Your body begins to flicker with translucency.", ch, NULL, victim, TO_CHAR);
}

void spell_detect_illusion(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;
	int dur;

	dur = ch->level / 3;
	if (victim != ch)
		dur /= 2;

	if (is_affected(ch, sn))
	{
		act("Your senses are already enhanced.", ch, NULL, NULL, TO_CHAR);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = level;
	af.duration = dur;
	af.modifier = 0;
	af.location = 0;
	af.bitvector = 0;
	affect_to_char(victim, &af);

	act("Your eyes gleam with a heightened awareness of illusion.", victim, NULL, NULL, TO_CHAR);
}

void spell_ice_slick(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	ROOM_AFFECT_DATA raf;

	if (is_affected(ch, sn))
	{
		send_to_char("You cannot make another ice slick so soon.\n\r", ch);
		return;
	}

	if (is_affected_room(ch->in_room, sn))
	{
		send_to_char("The ground is already frozen.\n\r", ch);
		return;
	}

	if (number_range(1, 100) > ch->pcdata->learned[sn])
	{
		send_to_char("You attempt to freeze the ground but fail.\n\r", ch);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = sn;
	af.aftype = AFT_SPELL;
	af.level = ch->level;
	af.affect_list_msg = str_dup("restricts casting of Ice Slick");
	af.duration = 48;
	af.modifier = 0;
	af.bitvector = 0;
	affect_to_char(ch, &af);

	init_affect_room(&raf);
	raf.where = TO_ROOM_AFFECTS;
	raf.aftype = AFT_SPELL;
	raf.type = sn;
	raf.level = ch->level;
	raf.duration = ch->level / 5;
	raf.modifier = 0;
	raf.bitvector = AFF_ROOM_ICE_SLICK;
	affect_to_room(ch->in_room, &raf);

	send_to_char("You freeze the ground around you!\n\r", ch);
	act("The ground becomes frozen and slick!", ch, NULL, NULL, TO_ROOM);
}

void spell_create_vacume(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	ROOM_AFFECT_DATA raf;

	if (is_affected(ch, sn))
	{
		send_to_char("You cannot create another vacume yet.\n\r", ch);
		return;
	}

	if (is_affected_room(ch->in_room, sn))
	{
		send_to_char("There already is a vacume here.\n\r", ch);
		return;
	}

	if (number_range(1, 100) > ch->pcdata->learned[sn])
	{
		send_to_char("You attempt to pull all the air out of the room but fail.\n\r", ch);
		WAIT_STATE(ch, PULSE_TICK / 2);
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = ch->level;
	af.duration = 60;
	af.modifier = 0;
	af.bitvector = 0;
	affect_to_char(ch, &af);

	init_affect_room(&raf);
	raf.where = TO_ROOM_AFFECTS;
	raf.aftype = AFT_SPELL;
	raf.type = sn;
	raf.level = ch->level;
	raf.duration = 4;
	raf.modifier = 0;
	raf.bitvector = AFF_ROOM_VACUME;
	affect_to_room(ch->in_room, &raf);

	send_to_char("You suck all the air out of this place and make a vacume!\n\r", ch);
	act("You feel a slight breeze and relize that there is no longer any air around you!", ch, NULL, NULL, TO_ROOM);
	WAIT_STATE(ch, PULSE_TICK);
}

void spell_hellfire(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;
	int dam;
	int dambase;
	dambase = 320;

	if (!IS_NPC(ch) && check_spellcraft(ch, sn))
	{
		dambase = 350;
		level += 2;
	}
	dam = dambase + number_percent();
	if (saves_spell(level + 1, victim, DAM_FIRE))
		dam -= 200;
	act("$N is consumed by hellish flames.", ch, 0, victim, TO_NOTVICT);
	act("You are consumed by hellish flames.", ch, 0, victim, TO_VICT);
	act("$N is consumed by hellish flames.", ch, 0, victim, TO_CHAR);
	damage_old(ch, victim, dam, sn, DAM_FIRE, TRUE);
	if (is_affected(victim, sn))
		return;
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.modifier = 0;
	af.location = 0;
	af.affect_list_msg = str_dup("induces major burns");
	af.bitvector = 0;
	af.duration = 4;
	af.level = level;
	af.owner_name = str_dup(ch->original_name);
	affect_to_char(victim, &af);
	return;
}

void spell_pyrotechnics(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	char buf[MAX_STRING_LENGTH];
	int dam;
	int dambase;
	dambase = 150;
	if (!IS_NPC(ch) && check_spellcraft(ch, sn))
	{
		dambase = 350;
		level += 2;
	}
	act("Sparks and flames begin to spew forth from $n's outstreched hands.", ch, 0, 0, TO_ROOM);
	act("The air fills with sparks and fire scorching everything in sight!", ch, 0, 0, TO_ROOM);
	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
	{
		vch_next = vch->next_in_room;
		if (is_same_group(vch, ch) || is_safe(ch, vch))
			continue;
		if (ch->cabal > 0 && vch->cabal > 0 && ch->cabal == vch->cabal)
			continue;
		if (!IS_NPC(vch) && (ch->fighting == NULL || vch->fighting == NULL))
		{
			sprintf(buf, "Die, %s, you sorcerous dog!", PERS(ch, vch));
			do_myell(vch, buf);
		}
		dam = dambase + number_percent();
		dam += number_percent() * 1.3;
		if (saves_spell(level + 1, vch, DAM_FIRE))
			dam *= .66;
		damage_old(ch, vch, dam, sn, DAM_FIRE, TRUE);
	}
	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
	return;
}

// New implementation since the old one was missing - Zornath
void spell_ball_lightning(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo, *rch, *rch_next;
	int target_dam = 225 + number_range(10, 20), offtarget_dam = 185 + number_range(5, 15), dam;

	if (victim == ch)
		return send_to_char("Logic prevails and prevents you from doing that.\n\r", ch);

	if (check_spellcraft(ch, skill_lookup("ball lightning")))
	{
		target_dam += spellcraft_dam(20, 5);
		offtarget_dam += spellcraft_dam(20, 7);
	}

	act("You release a ball of lightning towards $N!", ch, 0, victim, TO_CHAR);
	act("$n releases a ball of lightning towards $N!", ch, 0, victim, TO_NOTVICT);
	act("$n releases a ball of lightning towards you!", ch, 0, victim, TO_VICT);

	for (rch = victim->in_room->people; rch != NULL; rch = rch_next)
	{
		rch_next = rch->next_in_room;
		if (is_same_group(rch, victim) && rch != victim)
		{
			act("The ball of lightning explodes into a dazzling array of light near you!", ch, 0, rch, TO_VICT);
			act("Your ball of lightning explodes into a dazzling array of light near $N!", ch, 0, rch, TO_CHAR);
			act("$n's ball of lightning explodes into a dazzling array of light near $N!", ch, 0, rch, TO_NOTVICT);
			dam = offtarget_dam;
			if (saves_spell(level, rch, DAM_LIGHTNING))
				dam *= 0.66;
			damage_newer(ch, rch, dam, skill_lookup("ball lightning"), DAM_LIGHTNING, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "ball of lightning", PLURAL, DAM_POSSESSIVE, NULL, FALSE);
		}
		if (rch == victim)
		{
			act("The ball of lightning slams directly into you, exploding into an array of dazzling light!", ch, 0, rch, TO_VICT);
			act("Your ball of lightning slams directly into $N, exploding into an array of dazzling light!", ch, 0, rch, TO_CHAR);
			act("$n's ball of lightning slams directly into $N, exploding into an array of dazzling light!!", ch, 0, rch, TO_NOTVICT);
			dam = target_dam;
			if (saves_spell(level, rch, DAM_LIGHTNING))
				dam *= 0.66;
			damage_newer(ch, rch, dam, skill_lookup("ball lightning"), DAM_LIGHTNING, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "ball of lightning", PLURAL, DAM_POSSESSIVE, NULL, FALSE);
		}
	}

	return;
}

void spell_fatigue(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int dam = number_range(100, 150);
	int saved = FALSE;
	if (saves_spell(level, victim, DAM_MENTAL))
	{
		dam -= 40;
		saved = TRUE;
	}
	if (!saved)
	{
		act("$n looks fatigued.", victim, 0, 0, TO_ROOM);
		act("You feel very fatigued.", victim, 0, 0, TO_CHAR);
		victim->move -= dice(level, 3);
	}

	damage_old(ch, victim, dam, sn, DAM_MENTAL, TRUE);
}

void spell_cleanse(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	// Difficulty modifier: 1 easy 2 medium 3 hard 4 very hard
	sn = skill_lookup("blindness");

	if (number_percent() < number_range(40, 60))
	{
		if (is_affected(victim, sn) && cleansed(ch, victim, 1, sn))
		{
			if (victim != ch)
				act("$n is no longer blinded.", victim, 0, ch, TO_ROOM);
			send_to_char(skill_table[sn].msg_off, victim);
			send_to_char("\n\r", victim);
			affect_strip(victim, sn);
		}
		sn = skill_lookup("poison");
		if (is_affected(victim, sn) && cleansed(ch, victim, 2, sn))
		{
			if (victim != ch)
				act("$n looks less ill.", victim, 0, ch, TO_ROOM);
			send_to_char(skill_table[sn].msg_off, victim);
			send_to_char("\n\r", victim);
			affect_strip(victim, sn);
		}
		sn = skill_lookup("weaken");
		if (is_affected(victim, sn) && cleansed(ch, victim, 1, sn))
		{
			if (victim != ch)
				act("$n looks stronger.", victim, 0, ch, TO_ROOM);
			send_to_char(skill_table[sn].msg_off, victim);
			send_to_char("\n\r", victim);
			affect_strip(victim, sn);
		}
		sn = skill_lookup("plague");
		if (is_affected(victim, sn) && cleansed(ch, victim, 2, sn))
		{
			if (victim != ch)
				act("$n looks less diseased.", victim, 0, ch, TO_ROOM);
			send_to_char(skill_table[sn].msg_off, victim);
			send_to_char("\n\r", victim);
			affect_strip(victim, sn);
		}
		sn = skill_lookup("slow");
		if (is_affected(victim, sn) && cleansed(ch, victim, 2, sn))
		{
			if (victim != ch)
				act("$n starts moving at normal speed again.", victim, 0, ch, TO_ROOM);
			send_to_char(skill_table[sn].msg_off, victim);
			send_to_char("\n\r", victim);
			affect_strip(victim, sn);
		}
		sn = skill_lookup("forget");
		if (is_affected(victim, sn) && cleansed(ch, victim, 4, sn))
		{
			if (victim != ch)
				act("$n looks like $e's just had a rush of memory.", victim, 0, ch, TO_ROOM);
			send_to_char(skill_table[sn].msg_off, victim);
			send_to_char("\n\r", victim);
			affect_strip(victim, sn);
		}
		sn = skill_lookup("rot");
		if (is_affected(victim, sn) && cleansed(ch, victim, 3, sn))
		{
			if (victim != ch)
				act("$n's body stops wasting away.", victim, 0, ch, TO_ROOM);
			send_to_char(skill_table[sn].msg_off, victim);
			send_to_char("\n\r", victim);
			affect_strip(victim, sn);
		}
		sn = skill_lookup("garble");
		if (is_affected(victim, sn) && cleansed(ch, victim, 3, sn))
		{
			if (victim != ch)
				act("$n's tongue uncontorts.", victim, 0, ch, TO_ROOM);
			send_to_char(skill_table[sn].msg_off, victim);
			send_to_char("\n\r", victim);
			affect_strip(victim, sn);
		}
		sn = skill_lookup("confuse");
		if (is_affected(victim, sn) && cleansed(ch, victim, 3, sn))
		{
			if (victim != ch)
				act("$n looks less confused.", victim, 0, ch, TO_ROOM);
			send_to_char(skill_table[sn].msg_off, victim);
			send_to_char("\n\r", victim);
			affect_strip(victim, sn);
		}
		sn = skill_lookup("curse");
		if (is_affected(victim, sn) && cleansed(ch, victim, 2, sn))
		{
			if (victim != ch)
				act("$n's curse wears off.", victim, 0, ch, TO_ROOM);
			send_to_char(skill_table[sn].msg_off, victim);
			send_to_char("\n\r", victim);
			affect_strip(victim, sn);
		}
		sn = skill_lookup("incinerate");
		if (is_affected(victim, sn) && cleansed(ch, victim, 4, sn))
		{
			if (victim != ch)
				act("$n stops burning.", victim, 0, ch, TO_ROOM);
			send_to_char(skill_table[sn].msg_off, victim);
			send_to_char("\n\r", victim);
			affect_strip(victim, sn);
		}
		sn = skill_lookup("impale");
		if (is_affected(victim, sn) && cleansed(ch, victim, 4, sn))
		{
			if (victim != ch)
				act("$n stops bleeding.", victim, 0, ch, TO_ROOM);
			send_to_char(skill_table[sn].msg_off, victim);
			send_to_char("\n\r", victim);
			affect_strip(victim, sn);
		}
		sn = skill_lookup("hellfire");
		if (is_affected(victim, sn) && cleansed(ch, victim, 4, sn))
		{
			if (victim != ch)
				act("$n's helfire subsides.", victim, 0, ch, TO_ROOM);
			send_to_char(skill_table[sn].msg_off, victim);
			send_to_char("\n\r", victim);
			affect_strip(victim, sn);
		}
		sn = skill_lookup("haunting");
		if (is_affected(victim, sn) && cleansed(ch, victim, 4, sn))
		{
			if (victim != ch)
				act("$n looks less frightened.", victim, 0, ch, TO_ROOM);
			send_to_char(skill_table[sn].msg_off, victim);
			send_to_char("\n\r", victim);
			affect_strip(victim, sn);
		}
	}
	check_improve(ch, skill_lookup("cleanse"), TRUE, 1);
	return;
}

bool cleansed(CHAR_DATA *ch, CHAR_DATA *victim, int diffmodifier, int sn)
{
	int chance = 0, snlevel;
	chance = 100 / diffmodifier;
	snlevel = get_affect_level(victim, sn);
	if (snlevel < 0)
		return FALSE;
	chance -= (snlevel - ch->level) * 2;
	if (number_percent() > chance)
		return FALSE;
	return TRUE;
}
int get_affect_level(CHAR_DATA *ch, int sn)
{
	AFFECT_DATA *paf;
	// Go through all affects on ch and return the level of the one that matches sn.
	for (paf = ch->affected; paf != NULL; paf = paf->next)
		if (paf->type == sn)
			return paf->level;
	return -1;
}

void spell_rot(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		act("$N is already decomposing.", ch, NULL, victim, TO_CHAR);
		return;
	}

	if (saves_spell(level, victim, DAM_DISEASE) ||
		(IS_NPC(victim) && IS_SET(victim->act, ACT_UNDEAD)))
	{
		act("$N's body resists the decomposition.", ch, NULL, victim, TO_CHAR);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_MALADY;
	af.type = sn;
	af.level = level;
	af.duration = (get_curr_stat(victim, STAT_CON) * 6) / 5;
	af.location = APPLY_CON;
	af.modifier = -1;
	af.bitvector = 0;
	af.owner_name = str_dup(ch->original_name);
	affect_to_char(victim, &af);

	send_to_char("You feel your body begin to decompose.\n\r", victim);
	act("$n looks like $e is decomposing.", victim, NULL, NULL, TO_ROOM);
	return;
}

void spell_cure_deafness(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;

	if (!is_affected(victim, gsn_deafen))
	{
		if (victim == ch)
			send_to_char("You aren't deaf.\n\r", ch);
		else
			act("$N doesn't appear to be deafened.", ch, NULL, victim, TO_CHAR);
		return;
	}

	if (check_dispel(level, victim, gsn_deafen))
	{
		send_to_char("Your hearing returns!\n\r", victim);
		act("$n is no longer deafened.", victim, NULL, NULL, TO_ROOM);
	}
	else
	{
		send_to_char("You failed.\n\r", ch);
	}
	return;
}

void spell_remove_paralysis(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;

	if (!is_affected(victim, skill_lookup("hold person")))
	{
		if (victim == ch)
			send_to_char("You aren't paralyzed.\n\r", ch);
		else
			act("$N doesn't appear to be paralyzed.", ch, NULL, victim, TO_CHAR);
		return;
	}

	if (check_dispel(level, victim, skill_lookup("hold person")))
	{
		send_to_char("The feeling in your body returns!\n\r", victim);
		act("$n is no longer paraylzed.", victim, NULL, NULL, TO_ROOM);
	}
	else
	{
		send_to_char("You failed.\n\r", ch);
	}
	return;
}

void spell_awaken(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;

	if (IS_AWAKE(victim))
	{
		act("$N is already awake.", ch, NULL, victim, TO_CHAR);
		return;
	}

	if (is_affected(victim, skill_lookup("sleep")))
	{
		if (check_dispel(level, victim, skill_lookup("sleep")))
		{
			send_to_char("You suddenly awaken.\n\r", victim);
			act("$n suddenly awakens.", victim, NULL, NULL, TO_ROOM);
			do_stand(victim, "");
		}
		else
		{
			send_to_char("You failed.\n\r", ch);
		}
		return;
	}
	else if (is_affected(victim, gsn_garrotte))
	{
		if (check_dispel(level, victim, gsn_garrotte))
		{
			send_to_char("You suddenly awaken.\n\r", victim);
			act("$n suddenly awakens.", victim, NULL, NULL, TO_ROOM);
			do_stand(victim, "");
		}
		else
		{
			send_to_char("You failed.\n\r", ch);
		}
		return;
	}
	else if (is_affected(victim, gsn_blackjack))
	{
		if (check_dispel(level, victim, gsn_blackjack))
		{
			send_to_char("You suddenly awaken.\n\r", victim);
			act("$n suddenly awakens.", victim, NULL, NULL, TO_ROOM);
			do_stand(victim, "");
		}
		else
		{
			send_to_char("You failed.\n\r", ch);
		}
		return;
	}
	else if (is_affected(victim, gsn_choke))
	{
		if (check_dispel(level, victim, gsn_choke))
		{
			send_to_char("You suddenly awaken.\n\r", victim);
			act("$n suddenly awakens.", victim, NULL, NULL, TO_ROOM);
			do_stand(victim, "");
		}
		else
		{
			send_to_char("You failed.\n\r", ch);
		}
		return;
	}
	else if (is_affected(victim, gsn_choke))
	{
		if (check_dispel(level, victim, gsn_choke))
		{
			send_to_char("You suddenly awaken.\n\r", victim);
			act("$n suddenly awakens.", victim, NULL, NULL, TO_ROOM);
			do_stand(victim, "");
		}
		else
		{
			send_to_char("You failed.\n\r", ch);
		}
		return;
	}
	else if (is_affected(victim, gsn_stun))
	{
		if (check_dispel(level, victim, gsn_stun))
		{
			send_to_char("You suddenly awaken.\n\r", victim);
			act("$n suddenly awakens.", victim, NULL, NULL, TO_ROOM);
			do_stand(victim, "");
		}
		else
		{
			send_to_char("You failed.\n\r", ch);
		}
		return;
	}
	else
	{
		send_to_char("They don't seem to be in deep sleep.\n\r", ch);
		return;
	}
	return;
}

void spell_resist_heat(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("You are already resisting heat.\n\r", ch);
		else
			act("$E is already resisting heat.", ch, NULL, victim, TO_CHAR);
		return;
	}

	send_to_char("You feel resistant to heat.\n\r", victim);
	act("$n is resistant to heat.", victim, NULL, NULL, TO_ROOM);
	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.modifier = 0;
	af.level = level;
	af.duration = 4 + ch->level / 5;
	af.bitvector = RES_FIRE;
	af.location = 0;
	affect_to_char(victim, &af);
	return;
}

void spell_resist_cold(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("You are already resisting cold.\n\r", ch);
		else
			act("$E is already resisting cold.", ch, NULL, victim, TO_CHAR);
		return;
	}

	send_to_char("You feel resistant to cold.\n\r", victim);
	act("$n is resistant to cold.", victim, NULL, NULL, TO_ROOM);
	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.modifier = 0;
	af.level = level;
	af.duration = 4 + ch->level / 5;
	af.bitvector = RES_COLD;
	af.location = 0;
	affect_to_char(victim, &af);
	return;
}

void spell_resist_lightning(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("You are already resisting lightning.\n\r", ch);
		else
			act("$E is already resisting lightning.", ch, NULL, victim, TO_CHAR);
		return;
	}

	send_to_char("You feel resistant to lightning.\n\r", victim);
	act("$n is resistant to lightning.", victim, NULL, NULL, TO_ROOM);
	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.modifier = 0;
	af.level = level;
	af.duration = 4 + ch->level / 5;
	af.bitvector = RES_LIGHTNING;
	af.location = 0;
	affect_to_char(victim, &af);
	return;
}

void spell_resist_mental(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("You are already resisting mental attacks.\n\r", ch);
		else
			act("$E is already resisting mental attacks.", ch, NULL, victim, TO_CHAR);
		return;
	}

	send_to_char("You feel resistant to mental attacks.\n\r", victim);
	act("$n is resistant to mental attacks.", victim, NULL, NULL, TO_ROOM);
	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.modifier = 0;
	af.level = level;
	af.duration = 4 + ch->level / 5;
	af.bitvector = RES_MENTAL;
	af.location = 0;
	affect_to_char(victim, &af);
	return;
}

void spell_resist_acid(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("You are already resisting acid.\n\r", ch);
		else
			act("$E is already resisting acid.", ch, NULL, victim, TO_CHAR);
		return;
	}

	send_to_char("You feel resistant to acid.\n\r", victim);
	act("$n is resistant to acid.", victim, NULL, NULL, TO_ROOM);
	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.modifier = 0;
	af.level = level;
	af.duration = 4 + ch->level / 5;
	af.bitvector = RES_ACID;
	af.location = 0;
	affect_to_char(victim, &af);
	return;
}

void spell_resist_negative(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("You are already resisting negative damage.\n\r", ch);
		else
			act("$E is already resisting negative damage.", ch, NULL, victim, TO_CHAR);
		return;
	}

	send_to_char("You feel resistant to negative damage.\n\r", victim);
	act("$n is resistant to negative damage.", victim, NULL, NULL, TO_ROOM);
	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.modifier = 0;
	af.level = level;
	af.duration = 4 + ch->level / 5;
	af.bitvector = RES_NEGATIVE;
	af.location = 0;
	affect_to_char(victim, &af);
	return;
}

void spell_resist_paralysis(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("You are already resisting paralysis.\n\r", ch);
		else
			act("$E is already resisting paralysis.", ch, NULL, victim, TO_CHAR);
		return;
	}

	send_to_char("You feel resistant to paralysis.\n\r", victim);
	act("$n is resistant to paralysis.", victim, NULL, NULL, TO_ROOM);
	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.modifier = 0;
	af.level = level;
	af.duration = 4 + ch->level / 5;
	af.bitvector = RES_MAGIC;
	af.location = 0;
	affect_to_char(victim, &af);
	return;
}

void spell_resist_weapon(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("You are already resisting weapons.\n\r", ch);
		else
			act("$E is already resisting weapons.", ch, NULL, victim, TO_CHAR);
		return;
	}

	send_to_char("You feel resistant to weapons.\n\r", victim);
	act("$n is resistant to weapons.", victim, NULL, NULL, TO_ROOM);
	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.type = sn;
	af.modifier = 0;
	af.level = level;
	af.duration = 4 + ch->level / 5;
	af.bitvector = RES_WEAPON | RES_BASH | RES_SLASH | RES_PIERCE;
	af.location = 0;
	affect_to_char(victim, &af);
	return;
}

/* Psi Blast for Illithids -- Ceial */

void spell_psi_blast(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;
	int dam;

	if (is_affected(ch, gsn_psi_blast))
	{
		send_to_char("Your mind hasn't regained strength enough to assault minds.", ch);
		return;
	}

	if (saves_spell(level - 4, victim, DAM_NEGATIVE))
	{
		act("$n reels but is not over come by your mind.", victim, NULL, NULL, TO_ROOM);
		act("Your mind is brutally assaulted, but you resist.", victim, NULL, NULL, TO_CHAR);
		WAIT_STATE(ch, PULSE_VIOLENCE * 1);
		return;
	}

	if (check_spellcraft(ch, sn))
		dam = spellcraft_dam(6 + level / 1.5, 9);
	else
		dam = dice(6 + level / 2, 8);
	if (saves_spell(level, victim, DAM_MENTAL))
		dam /= 2;
	damage_old(ch, victim, dam, sn, DAM_MENTAL, TRUE);
	act("$N reels as their mind is brutally assaulted!", ch, NULL, victim, TO_NOTVICT);
	act("$N reels from your mental blast!", ch, NULL, victim, TO_CHAR);
	send_to_char("Your reels from the brutal assault.", victim);
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = gsn_psi_blast;
	af.bitvector = 0;
	af.level = level;
	af.duration = 10;
	affect_to_char(ch, &af);
	WAIT_STATE(victim, PULSE_VIOLENCE * 3);
	return;
}

void spell_group_teleport(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	ROOM_INDEX_DATA *old_room;
	CHAR_DATA *group;
	CHAR_DATA *g_next;

	old_room = ch->in_room;

	spell_teleport(skill_lookup("teleport"), level, ch, ch, (int)NULL);

	if (ch->in_room != old_room)
	{
		for (group = old_room->people; group != NULL; group = g_next)
		{
			g_next = group->next_in_room;
			if (!is_same_group(group, ch) || (group->fighting != NULL) || group == ch)
				continue;
			if (group == ch)
				continue;
			send_to_char("You have been teleported!\n\r", group);
			act("$n vanishes!", group, NULL, NULL, TO_ROOM);
			char_from_room(group);
			char_to_room(group, ch->in_room);
			act("$n slowly fades into existence.", group, NULL, NULL, TO_ROOM);
			do_look(group, "auto");
		}
	}
	return;
}

void spell_strength(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	if (is_affected(ch, sn))
	{
		send_to_char("You are as sure of your faith as you will ever get.\n\r", ch);
		return;
	}
	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.duration = ch->level * 2;
	af.location = 0;
	af.modifier = 0;
	af.bitvector = RES_HOLY;
	af.level = level;
	af.type = sn;
	affect_to_char(ch, &af);
	send_to_char("You fast for a period of time, building up your absolute faith in the strength of your Deity.\n\r", ch);
	act("$n meditates for a period of time, building up $s faith in $s Deity.", ch, 0, 0, TO_ROOM);
}
/*
void spell_soften( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	if(is_affected(victim,sn))
	{
	send_to_char("They are already softened.\n\r",ch);
	ch->mana-=30;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
	}
	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.type		= sn;
	af.level	= level;
	af.location	= APPLY_NONE;
	af.aftype	= AFT_SPELL;
	af.affect_list_msg = str_dup("increases damage received by 30%");
	af.modifier	= 0;
	af.duration	= 1;
	af.bitvector	= 0;
	affect_to_char(victim,&af);
	act("$n looks frail.",victim,0,0,TO_ROOM);
	send_to_char("You feel more frail.\n\r",victim);
}
*/
void spell_duo_dimension(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	int sn_fog, sn_fire, sn_dew;
	sn_fog = skill_lookup("faerie fog");
	sn_fire = skill_lookup("faerie fire");
	sn_dew = skill_lookup("dew");

	if (is_affected(ch, sn_fog) || is_affected(ch, sn_fire))
	{
		send_to_char("You are glowing too much for duo dimension.\n\r", ch);
		return;
	}

	if (is_affected(ch, sn_dew))
	{
		send_to_char("You cannot step out of the dimension while covered in dew.\n\r", ch);
		return;
	}

	act("You blink and $n is gone.", ch, 0, 0, TO_ROOM);
	send_to_char("You lose a dimension and disappear from view.\n\r", ch);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = gsn_duo;
	af.level = level;
	af.modifier = 0;
	af.bitvector = 0;
	af.affect_list_msg = str_dup("grants improved invisibility");
	af.duration = (level / 5);
	af.location = 0;
	affect_to_char(ch, &af);
	return;
}

bool fumble_obj(CHAR_DATA *victim, OBJ_DATA *obj_drop, int level, bool drop)
{
	if (drop)
	{
		if (!can_drop_obj(victim, obj_drop))
			return FALSE;
	}
	else
	{
		if (IS_OBJ_STAT(obj_drop, ITEM_NOREMOVE))
			return FALSE;
	}

	if (saves_spell(level, victim, DAM_NONE))
	{
		act("You nearly $T $p, but manage to keep your grip.",
			victim, obj_drop, drop ? "drop" : "lose hold of", TO_CHAR);
		act("$n nearly $T $p, but manages to keep $s grip.",
			victim, obj_drop, drop ? "drops" : "loses hold of", TO_ROOM);
		return FALSE;
	}

	if (drop)
	{
		obj_from_char(obj_drop);
		obj_to_room(obj_drop, victim->in_room);
	}
	else
	{
		unequip_char(victim, obj_drop);
	}

	act("You fumble and $T $p!",
		victim, obj_drop, drop ? "drop" : "lose hold of", TO_CHAR);
	act("$n fumbles and $T $p!",
		victim, obj_drop, drop ? "drops" : "loses hold of", TO_ROOM);
	return TRUE;
}

void spell_fumble(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	OBJ_DATA *obj;
	OBJ_DATA *obj_drop;
	int carry;
	int check;
	int count;
	int drop;
	bool fumbled = FALSE;

	if (!IS_AWAKE(victim))
		return;

	carry = 0;
	for (obj = victim->carrying; obj; obj = obj->next_content)
		carry++;

	drop = carry - can_carry_n(victim) + 5;

	for (check = 0; check < drop; check++)
	{
		obj_drop = NULL;
		count = 0;

		for (obj = victim->carrying; obj; obj = obj->next_content)
		{
			if (obj->wear_loc == WEAR_NONE && number_range(0, count++) == 0)
				obj_drop = obj;
		}

		if (!obj_drop)
			break;

		fumbled = fumble_obj(victim, obj_drop, level, TRUE) || fumbled;
	}

	if ((obj_drop = get_eq_char(victim, WEAR_HOLD)))
		fumbled = fumble_obj(victim, obj_drop, level, FALSE) || fumbled;

	if ((obj_drop = get_eq_char(victim, WEAR_LIGHT)))
		fumbled = fumble_obj(victim, obj_drop, level, FALSE) || fumbled;

	if ((obj_drop = get_eq_char(victim, WEAR_WIELD)))
		fumbled = fumble_obj(victim, obj_drop, level, FALSE) || fumbled;

	if (!fumbled)
	{
		send_to_char("You stumble momentarily, but quickly recover.\n\r",
					 victim);
		act("$n stumbles momentarily, but quickly recovers.",
			victim, NULL, NULL, TO_ROOM);
	}

	return;
}

void spell_knock(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{

	char pick_obj[MAX_INPUT_LENGTH];
	OBJ_DATA *obj;
	int door;

	obj = NULL;

	target_name = one_argument(target_name, pick_obj);

	if (pick_obj[0] == '\0')
	{
		send_to_char("What would you like to try to unlock?\n\r", ch);
		return;
	}

	if ((obj = get_obj_here(ch, pick_obj)) != NULL)
	{
		if (obj->item_type != ITEM_CONTAINER)
		{
			send_to_char("That's not a container.\n\r", ch);
			return;
		}
		if (!IS_SET(obj->value[1], CONT_CLOSED))
		{
			send_to_char("It's not closed.\n\r", ch);
			return;
		}
		if (obj->value[2] < 0)
		{
			send_to_char("It can't be unlocked.\n\r", ch);
			return;
		}
		if (!IS_SET(obj->value[1], CONT_LOCKED))
		{
			send_to_char("It's already unlocked.\n\r", ch);
			return;
		}
		if (IS_SET(obj->value[1], CONT_PICKPROOF))
		{
			send_to_char("Spell failed.\n\r", ch);
			return;
		}
		REMOVE_BIT(obj->value[1], CONT_LOCKED);
		send_to_char("*Click*\n\r", ch);
		return;
	}

	if ((door = find_door(ch, pick_obj)) >= 0)
	{
		ROOM_INDEX_DATA *to_room;
		EXIT_DATA *pexit;
		EXIT_DATA *pexit_rev;
		pexit = ch->in_room->exit[door];
		if (!IS_SET(pexit->exit_info, EX_CLOSED))
		{
			send_to_char("It's not closed.\n\r", ch);
			return;
		}
		if (pexit->key < 0 && !IS_IMMORTAL(ch))
		{
			send_to_char("That lock can't be opened.\n\r", ch);
			return;
		}
		if (!IS_SET(pexit->exit_info, EX_LOCKED))
		{
			send_to_char("It's already unlocked.\n\r", ch);
			return;
		}
		if (IS_SET(pexit->exit_info, EX_PICKPROOF) && !IS_IMMORTAL(ch))
		{
			send_to_char("Spell failed.\n\r", ch);
			return;
		}
		REMOVE_BIT(pexit->exit_info, EX_LOCKED);
		send_to_char("*Click*\n\r", ch);

		/* pick lock on other side of door as well */
		if ((to_room = pexit->u1.to_room) != NULL && (pexit_rev = to_room->exit[rev_dir[door]]) != NULL && pexit_rev->u1.to_room == ch->in_room)
		{
			REMOVE_BIT(pexit_rev->exit_info, EX_LOCKED);
		}
	}
	return;
}

void spell_protection(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	if (is_affected(ch, gsn_protection))
		return send_to_char("You are already protected.\n\r", ch);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = get_spell_aftype(ch);
	af.type = gsn_protection;
	af.level = level;
	af.duration = 24;
	af.location = 0;
	af.modifier = 0;
	af.bitvector = 0;
	affect_to_char(ch, &af);
	send_to_char("You feel protected.\n\r", ch);
	return;
}

void spell_prismatic_spray(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int i;
	int type = 0;

	if (ch->fighting)
	{
		int dam = dice(level, 6 + (ch->level > 30) + (ch->level > 40));
		victim = ch->fighting;

		if (check_spellcraft(ch, sn))
			dam = spellcraft_dam(level, 13);
		else
			dam = dice(level, 13);

		if (saves_spell(level, victim, DAM_OTHER))
			dam /= 2;
		i = number_range(1, 8);
		switch (i)
		{
		case 1:
			type = DAM_OTHER;
			send_to_char("A rainbow of colors shoots out from your hand.\n\r", ch);
			act("A rainbow of colors shoots out from $n's hand.", ch, NULL, victim, TO_ROOM);
			act("A red beam of light hits $N!", ch, NULL, victim, TO_ROOM);
			act("A red beam of light hits you!", ch, NULL, victim, TO_VICT);
			act("A red beam of light hits $N!", ch, NULL, victim, TO_CHAR);
			damage_old(ch, victim, number_range(100, 135), sn, type, TRUE);
			break;
		case 2:
			type = DAM_OTHER;
			send_to_char("A rainbow of colors shoots out from your hand.\n\r", ch);
			act("A rainbow of colors shoots out from $n's hand.", ch, NULL, victim, TO_ROOM);
			act("A orange beam of light hits $N!", ch, NULL, victim, TO_ROOM);
			act("A orange beam of light hits you!", ch, NULL, victim, TO_VICT);
			act("A orange beam of light hits $N!", ch, NULL, victim, TO_CHAR);
			damage_old(ch, victim, number_range(160, 250), sn, type, TRUE);
			break;
		case 3:
			type = DAM_OTHER;
			send_to_char("A rainbow of colors shoots out from your hand.\n\r", ch);
			act("A rainbow of colors shoots out from $n's hand.", ch, NULL, victim, TO_ROOM);
			act("A yellow beam of light hits $N!", ch, NULL, victim, TO_ROOM);
			act("A yellow beam of light hits you!", ch, NULL, victim, TO_VICT);
			act("A yellow beam of light hits $N!", ch, NULL, victim, TO_CHAR);
			damage_old(ch, victim, number_range(250, 330), sn, type, TRUE);
			break;
		case 4:
			type = DAM_OTHER;
			send_to_char("A rainbow of colors shoots out from your hand.\n\r", ch);
			act("A rainbow of colors shoots out from $n's hand.", ch, NULL, victim, TO_ROOM);
			act("A green beam of light hits $N!", ch, NULL, victim, TO_ROOM);
			act("A green beam of light hits you!", ch, NULL, victim, TO_VICT);
			act("A green beam of light hits $N!", ch, NULL, victim, TO_CHAR);
			spell_poison(gsn_poison, ch->level, ch, victim, TARGET_CHAR);
			damage_old(ch, victim, dam, sn, type, TRUE);
			break;
		case 5:
			type = DAM_OTHER;
			send_to_char("A rainbow of colors shoots out from your hand.\n\r", ch);
			act("A rainbow of colors shoots out from $n's hand.", ch, NULL, victim, TO_ROOM);
			act("A white beam of light hits $N!", ch, NULL, victim, TO_ROOM);
			act("A white beam of light hits you!", ch, NULL, victim, TO_VICT);
			act("A white beam of light hits $N!", ch, NULL, victim, TO_CHAR);
			spell_blindness(gsn_blindness, ch->level, ch, victim, TARGET_CHAR);
			damage_old(ch, victim, dam, sn, type, TRUE);
			break;
		case 6:
			type = DAM_OTHER;
			send_to_char("A rainbow of colors shoots out from your hand.\n\r", ch);
			act("A rainbow of colors shoots out from $n's hand.", ch, NULL, victim, TO_ROOM);
			act("A blue beam of light hits $N!", ch, NULL, victim, TO_ROOM);
			act("A blue beam of light hits you!", ch, NULL, victim, TO_VICT);
			act("A blue beam of light hits $N!", ch, NULL, victim, TO_CHAR);

			init_affect(&af);
			af.where = TO_AFFECTS;
			af.type = sn;
			af.aftype = AFT_SPELL;
			af.level = ch->level;
			af.location = APPLY_SAVING_PETRI;
			af.modifier = 60;
			affect_to_char(victim, &af);
			damage_old(ch, victim, dam, sn, type, TRUE);
			break;
		case 7:
			type = DAM_OTHER;
			send_to_char("A rainbow of colors shoots out from your hand.\n\r", ch);
			act("A rainbow of colors shoots out from $n's hand.", ch, NULL, victim, TO_ROOM);
			act("A indigo beam of light hits $N!", ch, NULL, victim, TO_ROOM);
			act("A indigo beam of light hits you!", ch, NULL, victim, TO_VICT);
			act("A indigo beam of light hits $N!", ch, NULL, victim, TO_CHAR);
			damage_old(ch, victim, number_range(330, 380), sn, type, TRUE);
			break;
		case 8:
			type = DAM_OTHER;
			send_to_char("A rainbow of colors shoots out from your hand.\n\r", ch);
			act("A rainbow of colors shoots out from $n's hand.", ch, NULL, victim, TO_ROOM);
			act("A violet beam of light hits $N!", ch, NULL, victim, TO_ROOM);
			act("A violet beam of light hits you!", ch, NULL, victim, TO_VICT);
			act("A violet beam of light hits $N!", ch, NULL, victim, TO_CHAR);
			damage_old(ch, victim, number_range(400, 600), sn, type, TRUE);
			break;
		}
		return;
	}
}

void spell_haunting(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;
	int dam;

	if (victim == ch)
	{
		send_to_char("Now thats just stupid talk.\n\r", ch);
		return;
	}

	if (saves_spell(level + 15, victim, DAM_NEGATIVE))
	{
		send_to_char("You failed to invoke haunting thoughts.\n\r", ch);
		return;
	}

	dam = dice(level / 3, 7);

	damage_old(ch, victim, dam, gsn_haunting, DAM_MENTAL, TRUE);
	act("$n narrows $s eyes and plagues $N's mind with haunting images!", ch, NULL, victim, TO_NOTVICT);
	act("$n plagues your mind with haunting images!", ch, NULL, victim, TO_VICT);
	act("You plague $N's mind with haunting images!", ch, NULL, victim, TO_CHAR);

	if (!is_affected(victim, gsn_haunting))
	{
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_MALADY;
		af.type = gsn_haunting;
		af.level = level;
		af.duration = 10;
		af.location = 0;
		af.modifier = 0;
		af.owner_name = str_dup(ch->original_name);
		af.affect_list_msg = str_dup("induces painful, haunting thoughts");
		af.bitvector = 0;
		affect_to_char(victim, &af);
		af.location = APPLY_SAVES;
		af.modifier = 10 + number_range(5, 12);
		affect_to_char(victim, &af);
	}
	return;
}

void spell_spire_of_flames(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int dam = level * 2 + dice(10, 40);

	if (saves_spell(level, victim, DAM_FIRE))
		dam /= 2;

	if (get_skill(ch, gsn_spire_of_flames) == 100)
		dam *= 1.2;

	act("You create a swirling column of incinerating flames and unleash it flying towards $N.", ch, NULL, victim, TO_CHAR);
	act("$n creates a swirling column of incinerating flames and unleash it flying towards you.", ch, NULL, NULL, TO_ROOM);
	damage_new(ch, victim, dam, TYPE_UNDEFINED, DAM_FIRE, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "spire of flames");
}

void spell_worldbind(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, sn))
	{
		send_to_char("They are already bound to this world.\n\r", ch);
		return;
	}

	if (saves_spell(level - 2, victim, DAM_NEGATIVE))
	{
		act("You failed to bind $N to this world.", ch, 0, victim, TO_CHAR);
		return;
	}

	act("You sever $N's ties to the spiritual world!", ch, 0, victim, TO_CHAR);
	act("$n severs $N's ties to the spiritual world!", ch, 0, victim, TO_NOTVICT);
	act_new("$n severs your ties to the spiritual world!", ch, 0, victim, TO_VICT, POS_DEAD);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = sn;
	af.aftype = AFT_SPELL;
	af.level = level;
	af.duration = 3;
	af.affect_list_msg = str_dup("restricts the ability to commune");
	af.location = 0;
	af.modifier = 0;
	af.bitvector = 0;
	affect_to_char(victim, &af);
}

void spell_icy_shards(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int dam;
	int num;

	if (check_spellcraft(ch, sn))
		dam = spellcraft_dam(level / 2, 11);
	else
		dam = dice(level / 2, 10);

	if (saves_spell(level, victim, DAM_COLD))
		dam /= 2;

	act("$n releases an orb of icy cold shards towards $N!", ch, NULL, victim, TO_ROOM);
	act("You release an orb of icy cold shards towards $N!", ch, NULL, victim, TO_CHAR);
	act("$n releases an orb of icy cold shards towards you!", ch, NULL, victim, TO_VICT);

	for (num = 0; num < 6; num++)
	{
		if (is_safe(victim, ch))
			return;
		if (number_percent() < 100 - num * 5)
			damage_new(ch, victim, dam, TYPE_UNDEFINED, DAM_COLD, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "icy shards");
		dam = dam - dam / 5;
	}

	return;
}

void spell_sunray(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int dam = 100;
	AFFECT_DATA af;
	if (ch->in_room->sector_type == SECT_INSIDE)
	{
		send_to_char("You can't see the sun to use it to burn people.\n\r", ch);
		return;
	}
	if (weather_info.sunlight == SUN_DARK)
		dam = 65;
	if (weather_info.sunlight == SUN_RISE)
		dam = 110;
	if (weather_info.sunlight == SUN_LIGHT)
		dam = 150;
	if (weather_info.sunlight == SUN_SET)
		dam = 100;
	if (weather_info.sky == SKY_CLOUDLESS)
		dam *= 1.7;
	if (weather_info.sky == SKY_CLOUDY)
		dam *= 1;
	if (weather_info.sky == SKY_RAINING)
		dam *= .8;
	if (weather_info.sky == SKY_LIGHTNING)
		dam *= 1;
	// Fuzz it up.
	act("$n burns $N with the light of the sun!", ch, 0, victim, TO_NOTVICT);
	act("$n burns you with the light of the sun!", ch, 0, victim, TO_VICT);
	act("You burn $N with the light of the sun!", ch, 0, victim, TO_CHAR);
	if (dam > 120 && number_percent() > 80)
	{
		act("$n appears to be blinded by the sun in $s eyes!", victim, 0, 0, TO_ROOM);
		act("You are blinded by the sun in your eyes!", victim, 0, 0, TO_CHAR);
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.type = sn;
		af.aftype = AFT_COMMUNE;
		af.duration = level / 12;
		af.location = APPLY_HITROLL;
		af.modifier = -6;
		af.level = level;
		af.bitvector = AFF_BLIND;
		affect_to_char(victim, &af);
		af.bitvector = 0;
		af.location = 0;
		af.modifier = 0;
		af.affect_list_msg = str_dup("induces blindness");
	}
	damage_old(ch, victim, dam, sn, DAM_LIGHT, TRUE);
}

void spell_vampiric_touch(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	int dam;

	if (victim == ch)
		return;

	dam = number_range(level * 3, level * 6);

	if (saves_spell(level, victim, DAM_NEGATIVE))
		dam *= .6;

	if (!IS_SET(victim->imm_flags, IMM_NEGATIVE))
	{
		if (ch->race == race_lookup("lich"))
		{
			act("Your unholy power feeds on $N's lifeforce.", ch, 0, victim, TO_CHAR);
			ch->hit += dam;
			ch->mana += dam;
		}
		else
		{
			act("You strengthen your life force at $N's expense.", ch, 0, victim, TO_CHAR);
			ch->hit += (dam * 2);
		}
	}

	damage(ch, victim, dam, gsn_vampiric_touch, DAM_NEGATIVE, TRUE);
	return;
}

void spell_create_fire_elemental(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = get_char_world(ch, target_name);
	CHAR_DATA *elemental;

	n_logf("Spell_create_fire_elemental: target name '%s'", target_name);

	if (!str_cmp(target_name, ""))
		return send_to_char("You must provide a target for the fire elemental.\n\r", ch);
	if (!victim)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}
	elemental = create_mobile(get_mob_index(28025));
	if (!elemental)
		return;
	char_to_room(elemental, victim->in_room);
	act("You conjure forth a greater fire elemental to destroy $N!", ch, 0, victim, TO_CHAR);
	act("A greater fire elemental arrives in a pillar of searing flames!", victim, 0, 0, TO_ALL);
	elemental->hunting = victim;
	multi_hit(elemental, victim, TYPE_UNDEFINED);
}

void spell_enlarge(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, gsn_enlarge) || victim->size >= SIZE_GIANT)
	{
		if (victim != ch)
			send_to_char("They are already enlarged.\n\r", ch);
		if (victim == ch)
			send_to_char("You are already enlarged.\n\r", ch);
		return;
	}

	if (!is_same_group(ch, victim))
		return send_to_char("You can only use this spell on groupmates.\n\r", ch);

	if (is_affected(victim, gsn_reduce))
	{
		send_to_char("You grow to your normal size.\n\r", ch);
		act("$n grows to $s normal size!", victim, 0, ch, TO_ROOM);
		affect_strip(victim, gsn_reduce);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = gsn_enlarge;
	af.level = level;
	af.duration = level / 8;
	af.location = APPLY_SIZE;
	af.modifier = 1;
	af.affect_list_msg = str_dup("increases size by 1");
	af.bitvector = 0;
	affect_to_char(victim, &af);
	af.aftype = AFT_INVIS;
	af.location = APPLY_DAM_MOD;
	af.modifier = 20;
	affect_to_char(victim, &af);

	send_to_char("Your entire body and gear rapidly grow in size, but you feel somewhat more frail afterward.\n\r", victim);
	act("$n suddenly swells in all directions, expanding to half $s normal size!", victim, 0, 0, TO_ROOM);
	return;
}

void spell_reduce(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, gsn_reduce) || victim->size <= SIZE_TINY)
	{
		if (victim != ch)
			send_to_char("They are already reduced.\n\r", ch);
		if (victim == ch)
			send_to_char("You are already reduced.\n\r", ch);
		return;
	}

	if (!is_same_group(ch, victim))
		return send_to_char("You can only use this spell on groupmates.\n\r", ch);

	if (is_affected(victim, gsn_enlarge))
	{
		send_to_char("You shrink down to your normal size.\n\r", ch);
		act("$n shrinks down to $s normal size.", victim, 0, ch, TO_ROOM);
		affect_strip(victim, gsn_enlarge);
		return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = gsn_reduce;
	af.level = level;
	af.duration = level / 8;
	af.location = APPLY_SIZE;
	af.modifier = -1;
	af.affect_list_msg = str_dup("decreases size by 1");
	af.bitvector = 0;
	affect_to_char(victim, &af);
	af.aftype = AFT_INVIS;
	af.location = APPLY_DAM_MOD;
	af.modifier = -20;
	affect_to_char(victim, &af);

	send_to_char("Your entire body suddenly contracts, leaving you significantly smaller but somehow sturdier.\n\r", victim);
	act("$n rapidly shrinks to two-thirds of $s original size!", victim, 0, 0, TO_ROOM);
	return;
}

void spell_shield_of_purity(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	if (is_affected(ch, gsn_shield_of_purity) || is_affected(ch, gsn_shield_of_redemption) || is_affected(ch, gsn_shield_of_justice) || is_affected(ch, gsn_shield_of_truth))
		return send_to_char("Your god already guides your shield.\n\r", ch);

	if (get_eq_char(ch, WEAR_SHIELD) == NULL)
		return send_to_char("You must be wearing a shield to call upon the power of your god.\n\r", ch);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.duration = 6;
	af.level = level;
	af.aftype = AFT_COMMUNE;
	af.type = gsn_shield_of_purity;
	af.affect_list_msg = str_dup("channels the power of Purity through your shield");
	affect_to_char(ch, &af);
	return send_to_char("Your god guides your shield, protecting you from harm.\n\r", ch);
}

void spell_shield_of_redemption(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	if (is_affected(ch, gsn_shield_of_purity) || is_affected(ch, gsn_shield_of_redemption) || is_affected(ch, gsn_shield_of_justice) || is_affected(ch, gsn_shield_of_truth))
		return send_to_char("Your god already guides your shield.\n\r", ch);

	if (get_eq_char(ch, WEAR_SHIELD) == NULL)
		return send_to_char("You must be wearing a shield to call upon the power of your god.\n\r", ch);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.duration = 6;
	af.level = level;
	af.aftype = AFT_COMMUNE;
	af.affect_list_msg = str_dup("channels the power of Redemption through your shield");
	af.type = gsn_shield_of_redemption;
	affect_to_char(ch, &af);
	return send_to_char("Your god channels his energy through your shield to strike down the unfaithful.\n\r", ch);
}

void spell_shield_of_justice(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	if (is_affected(ch, gsn_shield_of_purity) || is_affected(ch, gsn_shield_of_redemption) || is_affected(ch, gsn_shield_of_justice) || is_affected(ch, gsn_shield_of_truth))
		return send_to_char("Your god already guides your shield.\n\r", ch);

	if (get_eq_char(ch, WEAR_SHIELD) == NULL)
		return send_to_char("You must be wearing a shield to call upon the power of your god.\n\r", ch);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.duration = 6;
	af.level = level;
	af.aftype = AFT_COMMUNE;
	af.affect_list_msg = str_dup("channels the power of Justice through your shield");
	af.type = gsn_shield_of_justice;
	affect_to_char(ch, &af);
	return send_to_char("Your god channels his energy through your shield to crush the unholy.\n\r", ch);
}

void spell_shield_of_truth(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;

	if (is_affected(ch, gsn_shield_of_purity) || is_affected(ch, gsn_shield_of_redemption) || is_affected(ch, gsn_shield_of_justice) || is_affected(ch, gsn_shield_of_truth))
		return send_to_char("Your god already guides your shield.\n\r", ch);

	if (get_eq_char(ch, WEAR_SHIELD) == NULL)
		return send_to_char("You must be wearing a shield to call upon the power of your god.\n\r", ch);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.duration = 6;
	af.level = level;
	af.affect_list_msg = str_dup("channels the power of Truth through your shield");
	af.aftype = AFT_COMMUNE;
	af.type = gsn_shield_of_truth;
	affect_to_char(ch, &af);
	return send_to_char("Your god channels his energy through your shield to protect you.\n\r", ch);
}

void spell_water_breathing(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	if (is_affected(ch, gsn_water_breathing))
		return send_to_char("You can already breathe underwater.\n\r", ch);
	if (ch->class == class_lookup("druid") && victim != ch)
		return send_to_char("You cannot commune this supplication on another.\n\r", ch);
	if (ch->class == class_lookup("elementalist") && victim != ch)
		return send_to_char("You cannot cast this spell on another.\n\r", ch);

	init_affect(&af);
	if (ch->class == class_lookup("druid") || ch->class == class_lookup("healer"))
		af.aftype = AFT_COMMUNE;
	else
		af.aftype = AFT_SPELL;
	af.duration = 9;
	af.level = level;
	af.affect_list_msg = str_dup("allows you to breath underwater");
	af.type = gsn_water_breathing;
	af.where = TO_AFFECTS;
	affect_to_char(victim, &af);

	if (is_affected(ch, gsn_drowning))
		affect_strip(ch, gsn_drowning);

	if (ch != victim)
		act("$N is able to breathe water as $E would air.\n\r", ch, 0, victim, TO_CHAR);

	return send_to_char("You feel able to breathe water as you would air.\n\r", victim);
}

void spell_mana_disruption(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim;
	char arg1[MIL], arg2[MIL];

	target_name = one_argument(target_name, arg1);
	one_argument(target_name, arg2);

	if ((victim = get_char_room(ch, arg1)) == NULL)
		return send_to_char("They aren't here.\n\r", ch);

	if (arg2[0] == '\0')
		return send_to_char("How much mana will you channel?\n\r", ch);

	int manaChannel = atoi(arg2);
	if (manaChannel <= 0)
		return send_to_char("You must channel more than 0 mana!\n\r", ch);

	if (ch->mana < manaChannel)
		return send_to_char("You do not have that much mana to channel.\n\r", ch);

	act("You channel your mana through $N, disrupting his mind!", ch, 0, victim, TO_CHAR);
	act("$n channels $s mana through you, disrupting your mind!", ch, 0, victim, TO_VICT);
	act("$n channels $s mana through $N, disrupting $S mind!", ch, 0, victim, TO_NOTVICT);
	act("You lose control of your mental faculties and the mana runs rampant through your body!", ch, 0, victim, TO_VICT);
	act("$N loses control of $S mental faculties and the mana runs rampant through $S body!", ch, 0, victim, TO_CHAR);
	act("$N loses control of $S mental faculties and the mana runs rampant through $S body!", ch, 0, victim, TO_NOTVICT);
	victim->mana -= manaChannel * 3 / 4;
	ch->mana -= manaChannel;
	if (ch->mana < 0)
		ch->mana = 0;
	if (victim->mana < 0)
		victim->mana = 0;
	damage_new(ch, victim, manaChannel * 3 / 4, TYPE_UNDEFINED, DAM_TRUESTRIKE, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "mana disruption");
	return;
}

void spell_mana_shield(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	if (ch != victim)
		return send_to_char("You cannot cast that on another.\n\r", ch);

	if (is_affected(ch, skill_lookup("mental exhaustion")))
		return send_to_char("Your mind is not yet ready to conjure another mana shield.\n\r", ch);

	AFFECT_DATA af;
	init_affect(&af);
	af.aftype = AFT_SPELL;
	af.type = skill_lookup("mana shield");
	af.duration = ch->level / 5;
	af.level = ch->level;
	af.affect_list_msg = str_dup("substitutes mana damage for hp damage");
	affect_to_char(ch, &af);
	act("You use your mana as a shield from attacks.", ch, 0, 0, TO_CHAR);
	act("$n concentrates and the air around $m quivers slightly.", ch, 0, 0, TO_ROOM);
	return;
}

void spell_doublecast(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	char spell1[MIL], spell2[MIL], target1[MIL], target2[MIL], spellOnePass[MSL], spellTwoPass[MSL];
	char *spellOne, *spellTwo, *targetOne, *targetTwo;

	spellOne = one_argument(target_name, spell1);
	targetOne = one_argument(spellOne, target1);
	spellTwo = one_argument(targetOne, spell2);
	targetTwo = one_argument(spellTwo, target2);

	act("Drawing an arcane rune in the air, you channel your mana to cast two spells!", ch, 0, 0, TO_CHAR);
	act("Drawing an arcane rune in the air, $n channels $s mana to cast two spells!", ch, 0, 0, TO_ROOM);

	if (spell1[0] == '\0')
		return send_to_char("You need to specify a first spell to doublecast.\n\r", ch);
	if (target1[0] == '\0')
		return send_to_char("Cast the first spell on whom?\n\r", ch);
	if (spell2[0] == '\0')
		return send_to_char("You need to specify a second spell to doublecast.\n\r", ch);
	if (target2[0] == '\0')
		return send_to_char("Cast the second spell on whom?\n\r", ch);

	sprintf(spellOnePass, "'%s' %s", spell1, target1);
	do_cast(ch, spellOnePass);
	sprintf(spellTwoPass, "'%s' %s", spell2, target2);
	do_cast(ch, spellTwoPass);
	return;
}

void spell_group_sanctuary(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *gch;

	for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
	{
		if (((IS_NPC(ch) && IS_NPC(gch)) ||
			 (!IS_NPC(ch) && !IS_NPC(gch))) &&
			is_same_group(ch, gch) && !IS_AFFECTED(gch, AFF_SANCTUARY) && !is_affected(gch, skill_lookup("sanctuary")))
		{
			spell_sanctuary(skill_lookup("sanctuary"), level, ch, (void *)gch, TARGET_CHAR);
		}
	}
	return;
}

void spell_group_recall(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *gch;

	for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
	{
		if (is_same_group(ch, gch) && gch != ch)
		{
			spell_word_of_recall(skill_lookup("word of recall"), level, ch, (void *)gch, TARGET_CHAR);
		}
	}
	spell_word_of_recall(skill_lookup("word of recall"), level, ch, (void *)ch, TARGET_CHAR);
	return;
}

/*Pretty much total copy and paste of venueport spell.*/
void spell_group_gate(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim;
	CHAR_DATA *group;
	CHAR_DATA *arrival;
	CHAR_DATA *g_next;
	ROOM_INDEX_DATA *VENUEPORT;
	CHAR_DATA *last_to_venue;
	int numb;
	char buf[MSL];

	if (affect_find(ch->affected, gsn_insect_swarm) != NULL)
	{
		send_to_char("You attempt to transport yourself to safety but lose your concentration as you are bitten by yet another insect.\n\r", ch);
		return;
	}

	last_to_venue = ch;
	victim = get_char_world(ch, target_name);

	if (victim == NULL || victim == ch || victim->in_room == NULL || IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL) || !can_see_room(ch, victim->in_room) || (victim->in_room->cabal != 0 && victim->in_room->cabal != CABAL_ARCANA) || ch->in_room->guild != 0 || victim->in_room->guild != 0 || (IS_NPC(victim) && IS_SET(victim->imm_flags, IMM_SUMMON)) || IS_SET(victim->in_room->room_flags, ROOM_SAFE) || IS_SET(victim->in_room->room_flags, ROOM_PRIVATE) || IS_SET(victim->in_room->room_flags, ROOM_SOLITARY) || IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL) || victim->level >= level + 9 || (saves_spell(level, victim, DAM_OTHER)) || (!IS_NPC(victim) && !can_pk(ch, victim) && IS_SET(victim->act, PLR_NOSUMMON)))
	{
		send_to_char("You failed.\n\r", ch);
		return;
	}

	if (victim->in_room == ch->in_room)
	{
		send_to_char("You are already there.\n\r", ch);
		return;
	}

	VENUEPORT = get_room_index(ROOM_VNUM_VENUEPORT);
	numb = 1;
	for (group = ch->in_room->people; group != NULL; group = g_next)
	{
		g_next = group->next_in_room;
		if (!is_same_group(group, ch) || (group->fighting != NULL))
			continue;
		if (group == ch)
			continue;
		numb++;
		affect_strip(group, gsn_ambush);
		sprintf(buf, "%s closes his eyes momentarily and a massive tear in space explodes into the room and consumes you!\n\r", !IS_NPC(ch) ? ch->name : ch->short_descr);
		send_to_char(buf, group);
		send_to_char("Your stomach lurches as the gate disappears, leaving you somewhere else.\n\r", group);
		char_from_room(group);
		char_to_room(group, victim->in_room);
		act("$n arrives through a gate in the fabric of reality.", group, NULL, NULL, TO_ROOM);
		char_from_room(group);
		char_to_room(group, VENUEPORT);
		last_to_venue = group;
		if (is_affected(group, gsn_earthfade))
		{
			affect_strip(group, gsn_earthfade);
		}
	}

	act("$n closes his eyes and a massive tear in space explodes into the room and consumes $m!", ch, NULL, NULL, TO_ROOM);
	send_to_char("You close your eyes momentarily and a massive tear in space explodes into the room and consumes you!\n\r", ch);
	send_to_char("Your stomach lurches as the gate disappears, leaving you somewhere else.\n\r", ch);
	char_from_room(ch);
	char_to_room(ch, victim->in_room);
	act("$n arrives through a gate in the fabric of reality.", ch, NULL, NULL, TO_ROOM);
	un_earthfade(ch, NULL);

	if (last_to_venue == ch)
	{
		do_look(ch, "auto");
		return;
	}

	for (arrival = last_to_venue->in_room->people; arrival != NULL; arrival = g_next)
	{
		g_next = arrival->next_in_room;
		if (arrival == ch)
			continue;
		if ((is_same_group(arrival, ch)) && (arrival != ch))
		{
			char_from_room(arrival);
			char_to_room(arrival, ch->in_room);
		}
	}

	for (arrival = ch->in_room->people; arrival != NULL; arrival = arrival->next_in_room)
	{
		do_look(arrival, "auto");
		if (--numb == 0)
			break;
	}
	return;
}

void spell_coalesce_resistance(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, skill_lookup("coalesce resistance")))
		return send_to_char("They are already resistant.\n\r", ch);

	init_affect(&af);
	af.where = TO_RESIST;
	af.aftype = AFT_COMMUNE;
	af.type = skill_lookup("coalesce resistance");
	af.duration = ch->level / 13;
	af.bitvector = RES_FIRE;
	affect_to_char(victim, &af);
	af.bitvector = RES_COLD;
	affect_to_char(victim, &af);
	af.bitvector = RES_LIGHTNING;
	affect_to_char(victim, &af);
	af.bitvector = RES_MENTAL;
	affect_to_char(victim, &af);
	af.bitvector = RES_ACID;
	affect_to_char(victim, &af);
	af.bitvector = RES_NEGATIVE;
	affect_to_char(victim, &af);
	af.bitvector = RES_MAGIC;
	affect_to_char(victim, &af);
	af.bitvector = RES_WEAPON | RES_BASH | RES_PIERCE | RES_SLASH;
	affect_to_char(victim, &af);

	if (ch != victim)
	{
		act("You feel much more resistant as $n's god grants you their favor.", ch, 0, victim, TO_VICT);
		act("Your god smiles upon $N and grants $M their favor.", ch, 0, victim, TO_CHAR);
	}
	else
		act("You feel much more resistant as your god grants you their favor.", ch, 0, victim, TO_CHAR);

	act("$N looks much more resistant to damage.", ch, 0, victim, TO_ROOM);
	return;
}

void spell_bulwark_of_blades(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (ch != victim)
		return send_to_char("You cannot cast that on another.\n\r", ch);

	if (is_affected(ch, skill_lookup("bulwark of blades")))
		return send_to_char("You are already surrounding by a blade barrier.\n\r", ch);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_COMMUNE;
	af.type = skill_lookup("bulwark of blades");
	af.duration = ch->level / 8;
	af.affect_list_msg = str_dup("causes considerable harm to close-range attackers");
	affect_to_char(ch, &af);

	act("Blades fade into existence and form a protective barrier around you.", ch, 0, 0, TO_CHAR);
	act("A barrier of blades fades into existence around $n.", ch, 0, 0, TO_ROOM);
	return;
}

void spell_resurrect(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *search;
	OBJ_DATA *corpse;
	char *obj_name, *dead_name;
	AFFECT_DATA res;
	char buf[MSL];

	if (is_affected(ch, skill_lookup("resurrect timer")))
		return send_to_char("You are not yet able to resurrect the dead.\n\r", ch);

	obj_name = str_dup(target_name);
	corpse = get_obj_here(ch, obj_name);

	if (!corpse)
		return send_to_char("That corpse is not here.\n\r", ch);

	dead_name = corpse->owner;

	if (!dead_name)
		return send_to_char("They don't seem to exist.\n\r", ch);

	if (corpse->timer < 2)
		return send_to_char("That corpse will decay long before the ritual is complete.\n\r", ch);

	for (search = char_list; search != NULL; search = search->next)
	{
		if (!IS_NPC(search) && !str_cmp(dead_name, search->original_name))
		{

			if (search == ch)
				return send_to_char("Resurrect yourself?\n\r", ch);
			if (search->ghost == 0)
				return send_to_char("They have no connection to the afterlife.\n\r", ch);
			if (search->ghost < 2)
				return send_to_char("Their connection to the afterlife grows too tenuous.\n\r", ch);

			send_to_char("You feel a gentle tug at your soul...\n\r", search);
			act("You begin a ritual to bring $N back from the dead.", ch, 0, search, TO_CHAR);
			act("$n begins a ritual to bring $N back from the dead.", ch, 0, search, TO_ROOM);

			sprintf(buf, "resurrecting %s", search->original_name);
			init_affect(&res);
			res.aftype = AFT_COMMUNE;
			res.level = ch->level;
			res.type = skill_lookup("resurrect");
			res.owner_name = str_dup(search->original_name);
			res.affect_list_msg = str_dup(buf);
			res.duration = 1;
			affect_to_char(ch, &res);
			res.type = skill_lookup("resurrect timer");
			res.duration = 60;
			res.affect_list_msg = str_dup("prevents usage of resurrect");
			affect_to_char(ch, &res);
			return;
		}
	}

	send_to_char("They don't seem to exist.\n\r", ch);
	return;
}

void spell_divine_transferrance(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;
	char buf[MSL];

	if (ch == victim)
		return send_to_char("You can only cast that on others.\n\r", ch);

	if (is_affected(ch, skill_lookup("divine transferrance timer")))
		return send_to_char("You are unable to form a bond yet.\n\r", ch);

	if (is_affected(victim, skill_lookup("divine transferrer")))
		return send_to_char("They are already protected.\n\r", ch);

	if (is_affected(ch, skill_lookup("divine transferree")))
		return send_to_char("You are already using your faith to protect one individual.\n\r", ch);

	sprintf(buf, "transfers half of all damage received to %s", ch->original_name);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = skill_lookup("divine transferrer");
	af.aftype = AFT_COMMUNE;
	af.duration = ch->level / 10;
	af.affect_list_msg = str_dup(buf);
	af.name = str_dup("divine transferrance");
	af.owner_name = str_dup(ch->original_name);
	affect_to_char(victim, &af);

	free_string(buf);
	sprintf(buf, "receiving half of all damage taken by %s", victim->original_name);

	af.type = skill_lookup("divine transferree");
	af.affect_list_msg = str_dup(buf);
	af.owner_name = str_dup(victim->original_name);
	affect_to_char(ch, &af);
	af.type = skill_lookup("divine transferrance timer");
	af.name = str_dup("divine transferrance");
	af.affect_list_msg = str_dup("prevents usage of divine transferrance");
	af.duration = ch->level / 5;
	affect_to_char(ch, &af);

	act("Using your faith, you produce a bond between yourself and $N to share pain!", ch, 0, victim, TO_CHAR);
	act("Using $s faith, $n produces a bond between you to share pain!", ch, 0, victim, TO_VICT);
	act("Using $s faith, $n produces a bond between $mself and $N to share pain!", ch, 0, victim, TO_NOTVICT);

	return;
}

void spell_bastion_of_elements(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;

	if (is_affected(victim, skill_lookup("bastion of elements")))
		return send_to_char("They are already protected by a bastion of elements.\n\r", ch);

	AFFECT_DATA af;
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = skill_lookup("bastion of elements");
	af.name = str_dup("bastion of elements");
	af.duration = 8;
	af.location = APPLY_DAM_MOD;
	af.modifier = -18;
	affect_to_char(victim, &af);
	if (ch != victim)
		act("$N is now protected by a bastion of elements.", ch, 0, victim, TO_CHAR);
	act("You are now protected by a bastion of elements.", ch, 0, victim, TO_VICT);
	act("$N is now protected by a bastion of elements.", ch, 0, victim, TO_NOTVICT);
	af.location = APPLY_NONE;
	af.modifier = 0;

	if (!is_affected(victim, skill_lookup("shield of flames")))
	{
		af.type = skill_lookup("shield of flames");
		af.affect_list_msg = str_dup("grants immunity to fire and heat attacks");
		affect_to_char(victim, &af);
		if (ch != victim)
			act("$N is surrounded by a shield of flames.", ch, 0, victim, TO_CHAR);
		act("You are surrounded by a shield of flames.", ch, 0, victim, TO_VICT);
	}
	else
	{
		if (ch != victim)
			act("$N is already surrounded by a shield of flames.", ch, 0, victim, TO_CHAR);
		act("You are already surrounded by a shield of flames.", ch, 0, victim, TO_VICT);
	}
	if (!is_affected(victim, skill_lookup("shield of wind")))
	{
		af.type = skill_lookup("shield of wind");
		af.affect_list_msg = str_dup("grants immunity to wind attacks");
		affect_to_char(victim, &af);
		if (ch != victim)
			act("$N is surrounded by a shield of wind.", ch, 0, victim, TO_CHAR);
		act("You are surrounded by a shield of wind.", ch, 0, victim, TO_VICT);
	}
	else
	{
		if (ch != victim)
			act("$N is already surrounded by a shield of wind.", ch, 0, victim, TO_CHAR);
		act("You are already surrounded by a shield of wind.", ch, 0, victim, TO_VICT);
	}
	if (!is_affected(victim, skill_lookup("shield of frost")))
	{
		af.type = skill_lookup("shield of frost");
		af.affect_list_msg = str_dup("grants immunity to cold attacks");
		affect_to_char(victim, &af);
		if (ch != victim)
			act("$N is surrounded by a shield of frost.", ch, 0, victim, TO_CHAR);
		act("You are surrounded by a shield of frost.", ch, 0, victim, TO_VICT);
	}
	else
	{
		if (ch != victim)
			act("$N is already surrounded by a shield of frost.", ch, 0, victim, TO_CHAR);
		act("You are already surrounded by a shield of frost.", ch, 0, victim, TO_VICT);
	}
	if (!is_affected(victim, skill_lookup("shield of lightning")))
	{
		af.type = skill_lookup("shield of lightning");
		af.affect_list_msg = str_dup("grants immunity to lightning attacks");
		affect_to_char(victim, &af);
		if (ch != victim)
			act("$N is surrounded by a shield of lightning.", ch, 0, victim, TO_CHAR);
		act("You are surrounded by a shield of lightning.", ch, 0, victim, TO_VICT);
	}
	else
	{
		if (ch != victim)
			act("$N is already surrounded by a shield of lightning.", ch, 0, victim, TO_CHAR);
		act("You are already surrounded by a shield of lightning.", 0, 0, victim, TO_VICT);
	}
	if (!is_affected(victim, skill_lookup("shield of dust")))
	{
		af.type = skill_lookup("shield of dust");
		af.affect_list_msg = str_dup("grants immunity to dust and dirt attacks");
		affect_to_char(victim, &af);
		if (ch != victim)
			act("$N is surrounded by a shield of dust.", ch, 0, victim, TO_CHAR);
		act("You are surrounded by a shield of dust.", ch, 0, victim, TO_VICT);
	}
	else
	{
		if (ch != victim)
			act("$N is already surrounded by a shield of dust.", ch, 0, victim, TO_CHAR);
		act("You are already surrounded by a shield of dust.", ch, 0, victim, TO_VICT);
	}
	if (!is_affected(victim, skill_lookup("shield of bubbles")))
	{
		af.type = skill_lookup("shield of bubbles");
		af.affect_list_msg = str_dup("grants immunity to drowning attacks");
		affect_to_char(victim, &af);
		if (ch != victim)
			act("$N is surrounded by a shield of bubbles.", ch, 0, victim, TO_CHAR);
		act("You are surrounded by a shield of bubbles.", ch, 0, victim, TO_VICT);
	}
	else
	{
		if (ch != victim)
			act("$N is already surrounded by a shield of bubbles.", ch, 0, victim, TO_CHAR);
		act("You are already surrounded by a shield of bubbles.", ch, 0, victim, TO_VICT);
	}
	return;
}

void spell_maelstrom(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;

	if (is_affected(victim, skill_lookup("maelstrom")))
		return send_to_char("They are already affected by the elemental maelstrom.\n\r", ch);

	if (saves_spell(ch->level, victim, DAM_OTHER))
	{
		act("You resist the effects of $n's elemental maelstrom.", ch, 0, victim, TO_VICT);
		return send_to_char("They resisted the effects of your maelstrom.\n\r", ch);
	}

	act("You unleash an elemental maelstrom upon $N, leaving $M defenseless to the elements!", ch, 0, victim, TO_CHAR);
	act("$n unleashes an elemental maelstrom upon $N, leaving $M defenseless to the elements!", ch, 0, victim, TO_NOTVICT);
	act("$n unleashes an elemental maelstrom upon you, leaving you defenseless to the elements!", ch, 0, victim, TO_VICT);
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_MALADY;
	af.type = skill_lookup("maelstrom");
	af.duration = 10;
	af.affect_list_msg = str_dup("afflicted with a vulnerability to all elemental damage, bypassing immunities and resistances");
	affect_to_char(victim, &af);
	return;
}

void spell_shatter_armor(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;
	bool saved = FALSE;

	if (is_affected(victim, skill_lookup("shatter armor")))
		return act("$S armor is already shattered.", ch, 0, victim, TO_CHAR);

	if (saves_spell(ch->level, victim, DAM_PIERCE))
	{
		act("You partially resist $n's shatter armor spell!", ch, 0, victim, TO_VICT);
		act("$N partially resists your shatter armor spell!", ch, 0, victim, TO_CHAR);
		act("$N partially resists $n's shatter armor spell!", ch, 0, victim, TO_NOTVICT);
		saved = TRUE;
	}

	int modifier = (-9 * GET_AC(victim, AC_EXOTIC)) / 10;
	if (modifier < 0)
		modifier = -1 * modifier;

	if (!saved)
	{
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.duration = 5;
		af.type = skill_lookup("shatter armor");
		af.location = APPLY_AC;
		af.modifier = modifier;
		affect_to_char(victim, &af);
	}

	act("You clench your hand into a fist and will $N's armor to shatter around $M!", ch, 0, victim, TO_CHAR);
	act("$n clenches $s hand into a fist and wills your armor to shatter around you!", ch, 0, victim, TO_VICT);
	act("$n clenches $s hand into a fist and wills $N's armor to shatter around $M!", ch, 0, victim, TO_NOTVICT);

	damage_newer(ch, victim, modifier / 4, TYPE_UNDEFINED, DAM_PIERCE, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "the shattered armor fragments", SINGULAR, DAM_NOT_POSSESSIVE, NULL, FALSE);
	return;
}

void spell_hurricane(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	ROOM_AFFECT_DATA raf;
	CHAR_DATA *rch, *rch_next;
	EXIT_DATA *ex;
	int exits, throw_attempts_max = 10, throw_attempts = 0;
	bool exitsOpen = FALSE, thrownOut = FALSE;

	if (IS_NPC(victim))
		return send_to_char("Their movements are too erratic for you to summon a hurricane around.\n\r", ch);

	if (ch == victim)
		return send_to_char("Something tells you that would be a bad idea.\n\r", ch);

	if (is_affected(ch, skill_lookup("hurricane")))
		return send_to_char("You aren't ready to summon another hurricane.\n\r", ch);

	if (is_affected_room(ch->in_room, skill_lookup("hurricane")))
		return send_to_char("There is already a hurricane here.\n\r", ch);

	for (exits = 0; exits < MAX_DIR; exits++)
	{
		ex = ch->in_room->exit[exits];
		if (ex)
		{
			if (!IS_SET(ex->exit_info, EX_CLOSED))
			{
				exitsOpen = TRUE;
			}
		}
	}
	if (!exitsOpen)
		return send_to_char("You have no room to form a hurricane here.\n\r", ch);

	act("Focusing intently on the wind, $n conjures a powerful hurricane!", ch, 0, 0, TO_ROOM);
	act("Focusing intently on the wind, you conjure a powerful hurricane!", ch, 0, 0, TO_CHAR);
	damage_newer(ch, victim, ch->level * number_range(2, 3), TYPE_UNDEFINED, DAM_WIND, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "hurricane", PLURAL, DAM_POSSESSIVE, NULL, FALSE);

	for (rch = ch->in_room->people; rch != NULL; rch = rch_next)
	{
		thrownOut = FALSE;
		rch_next = rch->next_in_room;
		if (IS_NPC(rch))
			continue;
		if (rch == ch)
			continue;
		if (rch == victim)
			continue;
		if (IS_IMMORTAL(rch))
			continue;

		while (!thrownOut)
		{
			for (exits = number_range(0, 5); !thrownOut && throw_attempts < throw_attempts_max; exits = number_range(0, 5))
			{
				ex = ch->in_room->exit[exits];
				if (ex)
				{
					if (!IS_SET(ex->exit_info, EX_CLOSED))
					{
						int was_in = rch->in_room->vnum;
						switch (exits)
						{
						case DIR_NORTH:
							do_north(rch, "");
							if (rch->in_room->vnum != was_in)
								thrownOut = TRUE;
							else
								break;
							act("$N is violently blown out of the room by the billowing winds!", ch, 0, rch, TO_ROOM);
							act("$N is violently blown out of the room by the billowing winds!", ch, 0, rch, TO_CHAR);
							act("You are violently blown out of the room by the billowing winds!", rch, 0, 0, TO_CHAR);
							break;
						case DIR_EAST:
							do_east(rch, "");
							if (rch->in_room->vnum != was_in)
								thrownOut = TRUE;
							else
								break;
							act("$N is violently blown out of the room by the billowing winds!", ch, 0, rch, TO_ROOM);
							act("$N is violently blown out of the room by the billowing winds!", ch, 0, rch, TO_CHAR);
							act("You are violently blown out of the room by the billowing winds!", rch, 0, 0, TO_CHAR);
							break;
						case DIR_WEST:
							do_west(rch, "");
							if (rch->in_room->vnum != was_in)
								thrownOut = TRUE;
							else
								break;
							act("$N is violently blown out of the room by the billowing winds!", ch, 0, rch, TO_ROOM);
							act("$N is violently blown out of the room by the billowing winds!", ch, 0, rch, TO_CHAR);
							act("You are violently blown out of the room by the billowing winds!", rch, 0, 0, TO_CHAR);
							break;
						case DIR_SOUTH:
							do_south(rch, "");
							if (rch->in_room->vnum != was_in)
								thrownOut = TRUE;
							else
								break;
							act("$N is violently blown out of the room by the billowing winds!", ch, 0, rch, TO_ROOM);
							act("$N is violently blown out of the room by the billowing winds!", ch, 0, rch, TO_CHAR);
							act("You are violently blown out of the room by the billowing winds!", rch, 0, 0, TO_CHAR);
							break;
						case DIR_DOWN:
							do_down(rch, "");
							if (rch->in_room->vnum != was_in)
								thrownOut = TRUE;
							else
								break;
							act("$N is violently blown out of the room by the billowing winds!", ch, 0, rch, TO_ROOM);
							act("$N is violently blown out of the room by the billowing winds!", ch, 0, rch, TO_CHAR);
							act("You are violently blown out of the room by the billowing winds!", rch, 0, 0, TO_CHAR);
							break;
						case DIR_UP:
							do_up(rch, "");
							if (rch->in_room->vnum != was_in)
								thrownOut = TRUE;
							else
								break;
							act("$N is violently blown out of the room by the billowing winds!", ch, 0, rch, TO_ROOM);
							act("$N is violently blown out of the room by the billowing winds!", ch, 0, rch, TO_CHAR);
							act("You are violently blown out of the room by the billowing winds!", rch, 0, 0, TO_CHAR);
							break;
						}
					}
				}
			}
			if (throw_attempts >= throw_attempts_max)
			{
				act("$N is thrown against the ground by the billowing winds!", ch, 0, rch, TO_CHAR);
				act("$N is thrown against the ground by the billowing winds!", ch, 0, rch, TO_NOTVICT);
				act("You are thrown against the ground by the billowing winds!", ch, 0, rch, TO_VICT);
				damage_newer(ch, victim, number_range(212, 335), TYPE_UNDEFINED, DAM_WIND, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "the billowing winds", SINGULAR, DAM_NOT_POSSESSIVE, NULL, FALSE);
				thrownOut = TRUE;
			}
		}
	}

	init_affect_room(&raf);
	raf.duration = 3;
	raf.type = skill_lookup("hurricane");
	raf.level = ch->level;
	affect_to_room(ch->in_room, &raf);
}

void spell_vorpal_bolt(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;
	bool found = TRUE;
	char buf[MSL];
	int gsn;

	// type - 0 head - 1 legs - 2 arms - 3 chest
	int type = number_range(0, 3);

	act("$n brings $s arms together and thrusts them powerfully towards $N!", ch, 0, victim, TO_NOTVICT);
	act("In a flash of light, a bolt of black lightning streaks out of $n's hands and passes through $N!", ch, 0, victim, TO_NOTVICT);
	act("$n brings $s arms together and thrusts them powerfully towards you!", ch, 0, victim, TO_VICT);
	act("In a flash of light, a bolt of black lightning streaks out of $n's hands and passes through you!", ch, 0, victim, TO_VICT);
	act("You bring your arms together and thrust them powerfully towards $N!", ch, 0, victim, TO_CHAR);
	act("In a flash of light, a bolt of black lightning streaks out of your hands and passes through $N!", ch, 0, victim, TO_CHAR);

	damage_new(ch, victim, ch->level * number_range(4, 6), TYPE_UNDEFINED, DAM_LIGHTNING, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "vorpal bolt");

	switch (type)
	{
	case 0:
		if (!is_affected(victim, skill_lookup("vorpal bolt head")))
			found = FALSE;
		gsn = skill_lookup("vorpal bolt head");
		break;
	case 1:
		if (!is_affected(victim, skill_lookup("vorpal bolt leg")))
			found = FALSE;
		gsn = skill_lookup("vorpal bolt leg");
		break;
	case 2:
		if (!is_affected(victim, skill_lookup("vorpal bolt arm")))
			found = FALSE;
		gsn = skill_lookup("vorpal bolt arm");
		break;
	case 3:
		if (!is_affected(victim, skill_lookup("vorpal bolt chest")))
			found = FALSE;
		gsn = skill_lookup("vorpal bolt chest");
		break;
	}

	if (!found)
	{
		sprintf(buf, "$N yells out in agony as the bolt penetrates through $S %s!",
				type == 0 ? "head" : type == 1 ? "legs"
								 : type == 2   ? "arms"
											   : "chest");
		act(buf, ch, 0, victim, TO_NOTVICT);
		act(buf, ch, 0, victim, TO_CHAR);
		free_string(buf);
		sprintf(buf, "You yell out in agony as the bolt penetrates through your %s!",
				type == 0 ? "head" : type == 1 ? "legs"
								 : type == 2   ? "arms"
											   : "chest");
		act(buf, ch, 0, victim, TO_VICT);

		init_affect(&af);
		af.where = TO_AFFECTS;
		af.type = gsn;
		af.duration = 5;
		if (gsn == skill_lookup("vorpal bolt arm"))
		{
			af.location = APPLY_HITROLL;
			af.modifier = -1 * GET_HITROLL(victim);
			affect_to_char(victim, &af);
			af.location = APPLY_STR;
			af.modifier = (-1 * victim->perm_stat[STAT_STR]) / 3;
			affect_to_char(victim, &af);
		}
		else if (gsn == skill_lookup("vorpal bolt head"))
		{
			af.location = APPLY_MANA;
			af.modifier = (-1 * victim->mana) / 3;
			affect_to_char(victim, &af);
		}
		else if (gsn == skill_lookup("vorpal bolt leg"))
		{
			af.location = APPLY_HITROLL;
			af.modifier = -1 * GET_HITROLL(victim);
			affect_to_char(victim, &af);
			af.location = APPLY_DEX;
			af.modifier = (-1 * victim->perm_stat[STAT_DEX]) / 3;
			affect_to_char(victim, &af);
		}
		else
		{
			af.owner_name = str_dup(ch->original_name);
			af.affect_list_msg = str_dup("induces severe burns");
			affect_to_char(victim, &af);
		}
	}
	return;
}

void spell_ward_of_magnetism(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA af;
	OBJ_DATA *obj;
	int metal_obj_count = 0;
	int metal_obj_weight_total = 0;

	if (is_affected(ch, sn))
		return send_to_char("You are already surrounded by a field of electromagnetic energy.\n\r", ch);

	for (obj = ch->carrying; obj != NULL; obj = obj->next_content)
	{
		if (IS_METAL(obj))
		{
			metal_obj_weight_total += obj->weight;
			metal_obj_count++;
		}
	}

	if (metal_obj_count > 5 || metal_obj_weight_total > 20)
	{
		return send_to_char("It would be dangerous to cast this spell while handling so many metal objects.\n\r", ch);
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = level;
	af.duration = level / 5;
	af.modifier = 0;
	af.location = 0;
	af.affect_list_msg = str_dup("provides a ward of magnetism");
	af.bitvector = 0;
	affect_to_char(victim, &af);

	act("You feel a strange pull as an invisible field of electromagnetic energy forms around you.", ch, NULL, victim, TO_CHAR);
	return;
}
