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

void spell_hex(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *)vo;
	AFFECT_DATA hex;
	int severity = 0;
	if (is_affected(victim, sn))
		return send_to_char("They are already hexed.\n\r", ch);
	act("$n grins maniacally and spits out an archaic word, unleashing a hex upon $N!", ch, 0, victim, TO_NOTVICT);
	act("$n grins maniacally and spits out an archaic word, unleashing a hex upon you!", ch, 0, victim, TO_VICT);
	act("You grin maniacally and spit out an archaic word, unleashing a hex upon $N!", ch, 0, victim, TO_CHAR);
	if (saves_spell(level + 4, victim, DAM_NEGATIVE) && number_percent() > 35)
	{
		act("$n staggers for a moment, but resists the hex.", victim, 0, 0, TO_ROOM);
		act("You stagger as the power of the hex strikes you, but manage to resist it.", victim, 0, 0, TO_CHAR);
		return;
	}
	init_affect(&hex);
	hex.where = TO_AFFECTS;
	hex.type = sn;
	hex.level = level;
	hex.owner_name = str_dup(ch->original_name);
	hex.location = APPLY_SAVING_SPELL;
	hex.modifier = 10;
	hex.duration = level / 4;
	hex.bitvector = AFF_CURSE;
	severity++;
	if (number_percent() > 75)
	{
		hex.bitvector += AFF_BLIND;
		severity++;
	}
	affect_to_char(victim, &hex);
	if (number_percent() > 70)
	{
		// plague
		hex.location = APPLY_STR;
		hex.modifier = -5;
		hex.bitvector = AFF_PLAGUE;
		affect_to_char(victim, &hex);
		severity++;
	}
	if (number_percent() > 66)
	{
		// poison
		hex.location = APPLY_STR;
		hex.modifier = -6;
		hex.bitvector = AFF_POISON;
		affect_to_char(victim, &hex);
		severity++;
	}
	if (severity > 1 && severity < 4)
	{
		act("$n sags as the hex strikes him powerfully!", victim, 0, 0, TO_ROOM);
		act("You slump as the hex strikes you powerfully.", victim, 0, 0, TO_CHAR);
	}
	if (severity == 4)
	{
		act("$n screams in utter agony as the full power of the hex strangles $s soul!", victim, 0, 0, TO_ROOM);
		act("You scream in utter agony as the full power of the hex strangles your soul!", victim, 0, 0, TO_CHAR);
	}
	act("You feel drained from the power of your hex.", ch, 0, 0, TO_CHAR);
	ch->hit *= (98 - (severity * 3.0)) / 100.1;
}

void spell_black_circle(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *pet;
	AFFECT_DATA af;
	bool found = FALSE;
	if (is_affected(ch, sn))
		return send_to_char("You cannot summon your minions again yet.\n\r", ch);
	act("$n draws a black circle on the ground and falls into deep concentration.", ch, 0, 0, TO_ROOM);
	act("You draw a black circle on the ground and fall into deep concentration.", ch, 0, 0, TO_CHAR);
	for (pet = char_list; pet != NULL; pet = pet->next)
		if (IS_NPC(pet) && IS_AFFECTED(pet, AFF_CHARM) && (IS_SET(pet->act, ACT_UNDEAD) || (pet->pIndexData->vnum == MOB_VNUM_L_GOLEM || pet->pIndexData->vnum == MOB_VNUM_G_GOLEM)) && pet->master == ch)
		{
			stop_fighting(pet, TRUE);
			act("$n disappears suddenly.", pet, 0, 0, TO_ROOM);
			char_from_room(pet);
			char_to_room(pet, ch->in_room);
			act("$n arrives suddenly, kneeling before $N outside the circle.", pet, 0, ch, TO_ROOM);
			found = TRUE;
		}
	if (!found)
		return send_to_char("Your summonings went unanswered.\n\r", ch);
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.modifier = 0;
	af.location = 0;
	af.duration = 10;
	af.owner_name = str_dup(ch->original_name);
	af.affect_list_msg = str_dup("unable to perform the summoning ritual");
	af.level = ch->level;
	af.bitvector = 0;
	af.end_fun = NULL;
	affect_to_char(ch, &af);
}

void spell_banshee_call(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *zombie;
	CHAR_DATA *check;
	AFFECT_DATA af;
	char buf1[MSL], buf2[MSL];
	int count = 0;

	for (check = char_list; check != NULL; check = check->next)
	{
		if (IS_NPC(check) && IS_AFFECTED(check, AFF_CHARM) && IS_SET(check->act, ACT_UNDEAD) && check->master == ch)
			count++;
	}

	if (count >= 5)
		return send_to_char("You already control an army of banshees.\n\r", ch);
	if (is_affected(ch, skill_lookup("banshee call")))
		return send_to_char("You cannot yet summon an army of banshees.\n\r", ch);

	while (count < 5)
	{
		zombie = create_mobile(get_mob_index(MOB_VNUM_ZOMBIE));

		int z_level = number_range(47, 49);
		zombie->level = z_level;
		zombie->max_hit = (dice(z_level, 15));
		zombie->max_hit += (z_level * 20);
		zombie->hit = zombie->max_hit;
		zombie->damroll += (z_level * 7 / 10);
		zombie->alignment = -1000;
		zombie->name = str_dup("banshee spirit");

		sprintf(buf1, "a banshee spirit");
		sprintf(buf2, "A banshee spirit is standing here.\n\r");
		free_string(zombie->short_descr);
		free_string(zombie->long_descr);
		zombie->short_descr = str_dup(buf1);
		zombie->long_descr = str_dup(buf2);

		char_to_room(zombie, ch->in_room);
		SET_BIT(zombie->affected_by, AFF_CHARM);
		add_follower(zombie, ch);
		zombie->leader = ch;
		act("A banshee rises from the earth before $n.", ch, 0, 0, TO_ROOM);
		act("A banshee rises from the earth before you.", ch, 0, 0, TO_CHAR);
		count++;
	}

	init_affect(&af);
	af.type = skill_lookup("banshee call");
	af.affect_list_msg = str_dup("unable to summon an army of banshees");
	af.duration = 35;
	af.aftype = AFT_SPELL;
	affect_to_char(ch, &af);
}

void spell_minion_tactics(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *gch;

	for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
	{
		if (!is_same_group(gch, ch) || !(IS_NPC(gch)))
			continue;

		if ((IS_AFFECTED(gch, AFF_CHARM)) && (gch->pIndexData->vnum == MOB_VNUM_ZOMBIE) && (gch->master == ch))
		{
			act("$n's eyes widen with newly-acquired tactics.", gch, NULL, NULL, TO_ROOM);
			send_to_char("Your eyes widen with newly-acquired tactics.\n\r", gch);
			SET_BIT(gch->off_flags, OFF_BASH);
			SET_BIT(gch->off_flags, OFF_TRIP);
			SET_BIT(gch->off_flags, OFF_CRUSH);
		}
	}

	send_to_char("Ok.\n\r", ch);

	return;
}

void spell_minion_sanc(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *gch;

	sn = skill_lookup("sanctuary");

	for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
	{
		if (!is_same_group(gch, ch) || !(IS_NPC(gch)))
			continue;

		if ((IS_AFFECTED(gch, AFF_CHARM)) && (gch->pIndexData->vnum == MOB_VNUM_ZOMBIE) && (gch->master == ch))
		{
			spell_sanctuary(sn, ch->level, ch, gch, TARGET_CHAR);
		}
	}

	send_to_char("Ok.\n\r", ch);

	return;
}

void spell_minion_flight(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *gch;

	sn = skill_lookup("fly");

	for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
	{
		if (!is_same_group(gch, ch) || !(IS_NPC(gch)))
			continue;

		if ((IS_AFFECTED(gch, AFF_CHARM)) && (gch->pIndexData->vnum == MOB_VNUM_ZOMBIE) && (gch->master == ch))
		{
			spell_fly(sn, ch->level, ch, gch, TARGET_CHAR);
		}
	}

	send_to_char("Ok.\n\r", ch);

	return;
}

void spell_minion_sneak(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *gch;

	for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
	{
		if (!is_same_group(gch, ch) || !(IS_NPC(gch)))
			continue;

		if ((IS_AFFECTED(gch, AFF_CHARM)) && (gch->pIndexData->vnum == MOB_VNUM_ZOMBIE) && (gch->master == ch))
		{
			act("$n begins moving silently.", gch, NULL, NULL, TO_ROOM);
			send_to_char("You begin moving silently.\n\r", gch);
			SET_BIT(gch->affected_by, AFF_SNEAK);
		}
	}

	act("$n begins moving silently.", ch, NULL, NULL, TO_ROOM);

	return;
}

void spell_minion_recall(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *gch, *gch_next;
	ROOM_INDEX_DATA *room;

	sn = skill_lookup("word of recall");

	room = ch->in_room;
	spell_word_of_recall(sn, ch->level, ch, ch, TARGET_CHAR);

	for (gch = room->people; gch != NULL; gch = gch_next)
	{
		gch_next = gch->next_in_room;
		if (!is_same_group(gch, ch) || !(IS_NPC(gch)))
			continue;

		if ((IS_AFFECTED(gch, AFF_CHARM)) && (gch->pIndexData->vnum == MOB_VNUM_ZOMBIE || gch->pIndexData->vnum == MOB_VNUM_G_GOLEM || gch->pIndexData->vnum == MOB_VNUM_L_GOLEM) && (gch->master == ch))
		{
			char_from_room(gch);
			char_to_room(gch, ch->in_room);
			act("$n appears in the room.", gch, NULL, NULL, TO_ROOM);
		}
	}

	send_to_char("Ok.\n\r", ch);

	return;
}

void spell_minion_haste(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *gch;

	sn = skill_lookup("haste");

	for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
	{
		if (!is_same_group(gch, ch) || !(IS_NPC(gch)))
			continue;

		if ((IS_AFFECTED(gch, AFF_CHARM)) && (gch->pIndexData->vnum == MOB_VNUM_ZOMBIE) && (gch->master == ch))
		{
			spell_haste(sn, ch->level, ch, gch, TARGET_CHAR);
		}
	}

	send_to_char("Ok.\n\r", ch);

	return;
}

void spell_minion_trans(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *gch;

	sn = skill_lookup("pass door");

	for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
	{
		if (!is_same_group(gch, ch) || !(IS_NPC(gch)))
			continue;

		if ((IS_AFFECTED(gch, AFF_CHARM)) && (gch->pIndexData->vnum == MOB_VNUM_ZOMBIE) && (gch->master == ch))
		{
			spell_pass_door(sn, ch->level, gch, gch, TARGET_CHAR);
		}
	}

	send_to_char("Ok.\n\r", ch);

	return;
}

void do_lich(CHAR_DATA *ch, char *argument)
{
	char arg1[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;

	if (IS_NPC(ch))
		return;

	argument = one_argument(argument, arg1);

	if ((ch->level < 56 && ch->pcdata->induct != CABAL_LEADER) || IS_NPC(ch))
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (arg1[0] == '\0')
	{
		buf[0] = '\0';
		send_to_char("Syntax: lich <char>\n\r", ch);
		return;
	}

	if ((victim = get_char_world(ch, arg1)) == NULL)
	{
		send_to_char("They aren't playing.\n\r", ch);
		return;
	}

	if (victim->race == race_lookup("lich"))
	{
		send_to_char("They are lich already.\n\r", ch);
		return;
	}

	sprintf(buf, "%s is now a Lich.\n\r", victim->name);
	send_to_char(buf, ch);
	sprintf(buf, "Your mortal body begins to decompose as you gain unholy power!\n\r");
	send_to_char(buf, victim);

	victim->pcdata->learned[skill_lookup("minion tactics")] = 1;
	victim->pcdata->learned[skill_lookup("minion sanc")] = 1;
	victim->pcdata->learned[skill_lookup("minion flight")] = 1;
	victim->pcdata->learned[skill_lookup("minion sneak")] = 1;
	victim->pcdata->learned[skill_lookup("minion recall")] = 1;
	victim->pcdata->learned[skill_lookup("minion haste")] = 1;
	victim->pcdata->learned[skill_lookup("minion trans")] = 1;
	victim->pcdata->learned[skill_lookup("ritual of abominations")] = 1;
	victim->pcdata->learned[skill_lookup("vampiric touch")] = 1;

	victim->race = race_lookup("lich");

	victim->perm_stat[STAT_STR] = pc_race_table[ch->race].max_stats[STAT_STR];
	victim->perm_stat[STAT_INT] = pc_race_table[ch->race].max_stats[STAT_INT];
	victim->perm_stat[STAT_WIS] = pc_race_table[ch->race].max_stats[STAT_WIS];
	victim->perm_stat[STAT_DEX] = pc_race_table[ch->race].max_stats[STAT_DEX];
	victim->perm_stat[STAT_CON] = pc_race_table[ch->race].max_stats[STAT_CON];
	victim->mod_stat[STAT_STR] = 0;
	victim->mod_stat[STAT_INT] = 0;
	victim->mod_stat[STAT_WIS] = 0;
	victim->mod_stat[STAT_DEX] = 0;
	victim->mod_stat[STAT_CON] = 0;
	sprintf(buf, "Your stats are Str: %2d Int: %2d Wis: %2d Dex: %2d Con: %2d\n\r",
			ch->perm_stat[STAT_STR],
			ch->perm_stat[STAT_INT],
			ch->perm_stat[STAT_WIS],
			ch->perm_stat[STAT_DEX],
			ch->perm_stat[STAT_CON]);

	send_to_char(buf, victim);
}

void do_ritual_of_abominations(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *corpse, *phylactery;
	AFFECT_DATA af;
	char buf[MSL];
	int phylacRoll = number_percent();
	OBJ_INDEX_DATA *phylac;

	if (ch->mana < 300)
		return send_to_char("You don't have the 300 mana required to perform the Ritual of Abominations.\n\r", ch);
	if (argument[0] == '\0')
		return send_to_char("Which corpse do you wish to perform it on?\n\r", ch);
	corpse = get_obj_here(ch, argument);
	if (corpse == NULL)
		return send_to_char("That corpse is not here.\n\r", ch);
	if (corpse->pIndexData->vnum != OBJ_VNUM_CORPSE_PC || corpse->noPhylactery)
		return send_to_char("You cannot perform the Ritual of Abominations on that.\n\r", ch);
	if (is_affected(ch, skill_lookup("ritual of abominations")))
		return send_to_char("Your mind is still too weary from your previous Ritual.\n\r", ch);

	ch->mana -= 300;
	corpse->noPhylactery = TRUE;
	act("You kneel over $p and begin reciting unholy and forbidden incantations.", ch, corpse, 0, TO_CHAR);
	act("Resting your bony hand on its already-rotting chest, you begin to drain its life energy.", ch, corpse, 0, TO_CHAR);
	act("A phylactery formed from $p's own energy pulses into existence before you!", ch, corpse, 0, TO_CHAR);
	act("$n kneels over $p and begins reciting unholy and forbidden incantations.", ch, corpse, 0, TO_ROOM);
	act("Pulsing with energy, a phylactery appears before $n, surrounded in a hazy, dark aura.", ch, corpse, 0, TO_ROOM);
	act("$p grows charred and black, devoid of anything beyond rotting flesh and bones.", ch, corpse, 0, TO_CHAR);
	act("$p grows charred and black, devoid of anything beyond rotting flesh and bones.", ch, corpse, 0, TO_ROOM);
	sprintf(buf, "A corpse is charred and blackened beyond recognition here.");
	corpse->description = str_dup(buf);

	if (corpse->owner_class == class_lookup("necromancer"))
	{
		if (phylacRoll >= 90)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_DIAMOND);
		else if (phylacRoll >= 80)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_CATS_EYE_AGATE);
		else if (phylacRoll >= 65)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_ROUNDED_JADE);
		else if (phylacRoll >= 50)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_BLACK_SAPPHIRE);
		else if (phylacRoll >= 25)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_FACETED_JASPER);
		else
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_CATS_EYE);
	}
	else if (corpse->owner_class == class_lookup("healer"))
	{
		if (phylacRoll >= 90)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_BLACK_DIAMOND);
		else if (phylacRoll >= 80)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_AMETHYST);
		else if (phylacRoll >= 60)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_PALE_AQUAMARINE);
		else if (phylacRoll >= 40)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_STAR_DIOPSIDE);
		else
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_CATS_EYE);
	}
	else if (corpse->owner_class == class_lookup("elementalist"))
	{
		if (phylacRoll >= 90)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_RAINBOW_MOONSTONE);
		else if (phylacRoll >= 75)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_BASALT);
		else if (phylacRoll >= 60)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_STRIPED_GLASS);
		else if (phylacRoll >= 45)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_ZIRCON);
		else if (phylacRoll >= 35)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_MALACHITE_DISC);
		else if (phylacRoll >= 25)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_VIOLET_GARNET);
		else
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_CATS_EYE);
	}
	else if (corpse->owner_class == class_lookup("channeler"))
	{
		if (phylacRoll >= 90)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_SPHERICAL_MALACHITE);
		else if (phylacRoll >= 80)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_CATS_EYE_AGATE);
		else if (phylacRoll >= 65)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_SOMETHING_OILY);
		else if (phylacRoll >= 50)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_PALE_AQUAMARINE);
		else if (phylacRoll >= 40)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_SOAPSTONE);
		else if (phylacRoll >= 25)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_TAUROIDAL_BERYL);
		else
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_CATS_EYE);
	}
	else if (corpse->owner_class == class_lookup("ranger"))
	{
		if (phylacRoll >= 90)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_FIRE_OPAL);
		else if (phylacRoll >= 75)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_JADE);
		else if (phylacRoll >= 60)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_BASALT);
		else if (phylacRoll >= 45)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_STRIPED_GLASS);
		else if (phylacRoll >= 30)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_ZIRCON);
		else
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_CATS_EYE);
	}
	else if (corpse->owner_class == class_lookup("assassin"))
	{
		if (phylacRoll >= 95)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_FIRE_OPAL);
		else if (phylacRoll >= 85)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_MALACHITE_DISC);
		else if (phylacRoll >= 65)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_ANGLED_JASPER);
		else if (phylacRoll >= 45)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_MILKY_QUARTZ);
		else if (phylacRoll >= 25)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_BLACK_OPAL);
		else
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_CATS_EYE);
	}
	else if (corpse->owner_class == class_lookup("thief"))
	{
		if (phylacRoll >= 90)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_CATS_EYE_AGATE);
		else if (phylacRoll >= 80)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_ROUNDED_JADE);
		else if (phylacRoll >= 60)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_ANGLED_JASPER);
		else if (phylacRoll >= 40)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_MILKY_QUARTZ);
		else if (phylacRoll >= 25)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_PALE_AQUAMARINE);
		else
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_CATS_EYE);
	}
	else if (corpse->owner_class == class_lookup("warrior"))
	{
		if (phylacRoll >= 90)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_FIRE_OPAL);
		else if (phylacRoll >= 80)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_GRANITE);
		else if (phylacRoll >= 60)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_ROUNDED_JADE);
		else if (phylacRoll >= 40)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_BASALT);
		else if (phylacRoll >= 25)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_ZIRCON);
		else
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_CATS_EYE);
	}
	else if (corpse->owner_class == class_lookup("zealot"))
	{
		if (phylacRoll >= 90)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_SPHERICAL_MALACHITE);
		else if (phylacRoll >= 75)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_MALACHITE_DISC);
		else if (phylacRoll >= 60)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_PALE_JADE);
		else if (phylacRoll >= 45)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_BLOODSTONE);
		else if (phylacRoll >= 35)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_BLACK_OPAL);
		else if (phylacRoll >= 25)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_BLACK_SAPPHIRE);
		else
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_CATS_EYE);
	}
	else if (corpse->owner_class == class_lookup("anti-paladin"))
	{
		if (phylacRoll >= 90)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_BLACK_PEARL);
		else if (phylacRoll >= 70)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_PALE_JADE);
		else if (phylacRoll >= 55)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_SUNSTONE);
		else if (phylacRoll >= 35)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_STAR_DIOPSIDE);
		else if (phylacRoll >= 25)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_FIRE_OPAL);
		else
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_CATS_EYE);
	}
	else if (corpse->owner_class == class_lookup("paladin"))
	{
		if (phylacRoll >= 90)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_BLACK_PEARL);
		else if (phylacRoll >= 70)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_PALE_JADE);
		else if (phylacRoll >= 55)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_SUNSTONE);
		else if (phylacRoll >= 45)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_BASALT);
		else if (phylacRoll >= 35)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_ZIRCON);
		else if (phylacRoll >= 25)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_PALE_AQUAMARINE);
		else
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_CATS_EYE);
	}
	else if (corpse->owner_class == class_lookup("druid"))
	{
		if (phylacRoll >= 90)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_GRANITE);
		else if (phylacRoll >= 70)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_JADE);
		else if (phylacRoll >= 60)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_TAUROIDAL_BERYL);
		else if (phylacRoll >= 50)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_BASALT);
		else if (phylacRoll >= 40)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_STRIPED_GLASS);
		else if (phylacRoll >= 30)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_ZIRCON);
		else if (phylacRoll >= 25)
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_AMETHYST);
		else
			phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_CATS_EYE);
	}
	else
		phylac = get_obj_index(OBJ_VNUM_PHYLACTERY_CATS_EYE);

	phylactery = create_object(phylac, 0);
	obj_to_char(phylactery, ch);
	act("A sickening aura surrounds you as you stand, your hand wrapped around $p.", ch, phylactery, 0, TO_CHAR);
	act("A sickening aura surrounds $n as $e stands, $s hand wrapped around $p.", ch, phylactery, 0, TO_ROOM);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = skill_lookup("ritual of abominations");
	af.name = str_dup("ritual of abominatio");
	af.level = 60;
	af.duration = 80;
	af.aftype = AFT_POWER;
	af.affect_list_msg = str_dup("inability to perform the Ritual of Abominations");
	af.dispel_at_death = FALSE;
	affect_to_char(ch, &af);
}

void spell_maledict_corpse(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	OBJ_DATA *corpse;
	char *obj_name;

	if (target_name[0] == '\0')
	{
		send_to_char("Maledict which corpse?\n\r", ch);
		return;
	}

	obj_name = str_dup(target_name);

	corpse = get_obj_here(ch, obj_name);

	if (corpse == NULL)
	{
		send_to_char("You don't see that here.\n\r", ch);
		return;
	}

	if (corpse->pIndexData->vnum != OBJ_VNUM_CORPSE_PC)
	{
		send_to_char("You can only maledict player corpses.\n\r", ch);
		return;
	}

	act("You kneel down and pronounce a maledictive curse upon $p.", ch, corpse, 0, TO_CHAR);
	act("$n kneels down and pronounces a maledictive curse upon $p.", ch, corpse, 0, TO_ROOM);
	set_bit(&corpse->extra_flags, ITEM_MALEDICTED);
	return;
}

void spell_decay_corpse(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	char buf1[MSL], buf2[MSL];
	OBJ_DATA *obj, *obj_next, *corpse, *skeleton;
	char *name, *last_name, *obj_name;
	int chance;

	if (target_name[0] == '\0')
	{
		send_to_char("Decay which corpse?\n\r", ch);
		return;
	}

	obj_name = str_dup(target_name);
	corpse = get_obj_here(ch, obj_name);

	if (corpse == NULL)
	{
		send_to_char("You can't find that object.\n\r", ch);
		return;
	}

	if ((corpse->item_type != ITEM_CORPSE_NPC) && (corpse->item_type != ITEM_CORPSE_PC))
	{
		send_to_char("You can't decay that.\n\r", ch);
		return;
	}

	name = corpse->short_descr;

	for (obj = corpse->contains; obj != NULL; obj = obj_next)
	{
		obj_next = obj->next_content;
		obj_from_obj(obj);
		obj_to_room(obj, ch->in_room);
	}

	chance = get_skill(ch, sn);

	if (number_percent() > chance)
	{
		act("Your decaying becomes uncontrolled and you destroy $p.", ch, corpse, NULL, TO_CHAR);
		act("$n tries to decay $p but reduces it to a puddle of slime.", ch, corpse, NULL, TO_ROOM);
		extract_obj(corpse);
		return;
	}

	act("$n decays the flesh off $p.", ch, corpse, NULL, TO_ROOM);
	act("You decay the flesh off $p and reduce it to a skeleton.", ch, corpse, NULL, TO_CHAR);

	skeleton = create_object(get_obj_index(OBJ_VNUM_SKELETON), 1);
	obj_to_room(skeleton, ch->in_room);

	skeleton->level = corpse->level;

	last_name = name;
	last_name = one_argument(corpse->short_descr, name);
	last_name = one_argument(last_name, name);
	last_name = one_argument(last_name, name);
	name = last_name;
	extract_obj(corpse);

	sprintf(buf1, "the skeleton of %s", name);
	sprintf(buf2, "A skeleton of %s is lying here in a puddle of decayed flesh.", name);
	free_string(skeleton->short_descr);
	free_string(skeleton->description);
	skeleton->short_descr = str_dup(buf1);
	skeleton->description = str_dup(buf2);
	skeleton->item_type = ITEM_SKELETON;
	return;
}

void spell_preserve(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	OBJ_DATA *obj = (OBJ_DATA *)vo;
	long vnum, chance;

	vnum = obj->pIndexData->vnum;

	if (vnum != OBJ_VNUM_SEVERED_HEAD && vnum != OBJ_VNUM_TORN_HEART && vnum != OBJ_VNUM_SLICED_ARM && vnum != OBJ_VNUM_SLICED_LEG && vnum != OBJ_VNUM_GUTS && vnum != OBJ_VNUM_BRAINS)
	{
		send_to_char("You can't preserve that.\n\r", ch);
		return;
	}

	if (obj->timer > 10)
	{
		send_to_char("It's already in very well preserved condition.\n\r", ch);
		WAIT_STATE(ch, PULSE_VIOLENCE);
		return;
	}

	chance = get_skill(ch, sn);
	chance *= 90;

	if (number_percent() > chance)
	{
		act("$n destroys $p.", ch, obj, NULL, TO_ROOM);
		act("You fail and destroy $p.", ch, obj, NULL, TO_CHAR);
		extract_obj(obj);
		WAIT_STATE(ch, PULSE_VIOLENCE);
		return;
	}

	act("You surround $p with necromantic magic to slow it's decay.", ch, obj, NULL, TO_CHAR);
	WAIT_STATE(ch, PULSE_VIOLENCE);
	obj->timer += number_range(level / 2, level);
	return;
}

void spell_lesser_golem(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *golem;
	AFFECT_DATA af;
	CHAR_DATA *check;
	OBJ_DATA *part;
	OBJ_DATA *part_next;
	int parts, lvl;

	parts = 0;
	if (is_affected(ch, sn))
	{
		send_to_char("You aren't up to fashioning another golem yet.\n\r", ch);
		return;
	}

	for (check = char_list; check != NULL; check = check->next)
	{
		if (IS_NPC(check))
			if ((check->master == ch) && (check->pIndexData->vnum == MOB_VNUM_L_GOLEM))
			{
				send_to_char("You already have a golem under your command.\n\r", ch);
				return;
			}
	}

	for (part = ch->carrying; part != NULL; part = part_next)
	{
		part_next = part->next_content;

		if (part->pIndexData->vnum != OBJ_VNUM_SEVERED_HEAD && part->pIndexData->vnum != OBJ_VNUM_TORN_HEART && part->pIndexData->vnum != OBJ_VNUM_SLICED_ARM && part->pIndexData->vnum != OBJ_VNUM_SLICED_LEG && part->pIndexData->vnum != OBJ_VNUM_GUTS && part->pIndexData->vnum != OBJ_VNUM_BRAINS)
			continue;
		parts++;
	}

	if (parts == 0)
	{
		send_to_char("You don't have any body parts to animate.\n\r", ch);
		return;
	}
	else if (parts <= 1)
	{
		send_to_char("You don't have enough parts to build a flesh golem.\n\r", ch);
		return;
	}

	for (part = ch->carrying; part != NULL; part = part_next)
	{
		part_next = part->next_content;

		if (part->pIndexData->vnum != OBJ_VNUM_SEVERED_HEAD && part->pIndexData->vnum != OBJ_VNUM_TORN_HEART && part->pIndexData->vnum != OBJ_VNUM_SLICED_ARM && part->pIndexData->vnum != OBJ_VNUM_SLICED_LEG && part->pIndexData->vnum != OBJ_VNUM_GUTS && part->pIndexData->vnum != OBJ_VNUM_BRAINS)
			continue;
		extract_obj(part);
	}

	if (parts >= ch->level / 5)
		parts = ch->level / 5;

	lvl = (level - 3 + parts);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.level = level;
	af.location = 0;
	af.modifier = 0;
	af.duration = 35;
	af.bitvector = 0;
	af.type = sn;
	af.affect_list_msg = str_dup("unable to summon a lesser golem");
	af.aftype = AFT_SPELL;
	affect_to_char(ch, &af);
	act("$n fashions a flesh golem!", ch, 0, 0, TO_ROOM);
	send_to_char("You fashion a flesh golem to serve you!\n\r", ch);

	golem = create_mobile(get_mob_index(MOB_VNUM_L_GOLEM));
	golem->level = lvl;
	golem->damroll += level / 2 + parts;

	if (parts <= 3)
		golem->max_hit = ((lvl * lvl / 2) + dice(lvl, 10));
	else if (parts == 4)
		golem->max_hit = ((lvl * lvl / 2) + dice(lvl, 12));
	else if (parts == 5)
		golem->max_hit = ((lvl * lvl / 2) + dice(lvl, 13));
	else if (parts == 6)
		golem->max_hit = ((lvl * lvl / 2) + dice(lvl, 15));
	else if (parts == 7)
		golem->max_hit = ((lvl * lvl / 2) + dice(lvl, 18));
	else
		golem->max_hit = ((lvl * lvl / 2) + dice(lvl, 20));

	golem->hit = golem->max_hit;
	golem->max_move = ch->max_move;
	golem->move = golem->max_move;
	char_to_room(golem, ch->in_room);
	SET_BIT(golem->affected_by, AFF_CHARM);
	add_follower(golem, ch);
	golem->leader = ch;
	return;
}

void spell_greater_golem(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *golem, *check;
	AFFECT_DATA af;

	if (is_affected(ch, sn))
	{
		send_to_char("You aren't up to fashioning another golem yet.\n\r", ch);
		return;
	}

	for (check = char_list; check != NULL; check = check->next)
	{
		if (IS_NPC(check))
			if ((check->master == ch) && (check->pIndexData->vnum == MOB_VNUM_G_GOLEM))
			{
				send_to_char("You already have a golem under your command.\n\r", ch);
				return;
			}
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.level = level;
	af.location = 0;
	af.modifier = 0;
	af.duration = 35;
	af.bitvector = 0;
	af.type = sn;
	af.aftype = AFT_SPELL;
	af.affect_list_msg = str_dup("unable to summon a greater golem");
	affect_to_char(ch, &af);
	act("$n fashions an iron golem!", ch, 0, 0, TO_ROOM);
	send_to_char("You fashion an iron golem to serve you!\n\r", ch);

	golem = create_mobile(get_mob_index(MOB_VNUM_G_GOLEM));
	golem->level = ch->level;
	golem->damroll = (5 + 2 * level / 3);
	golem->max_hit = level * level;
	golem->hit = golem->max_hit;
	golem->max_move = ch->max_move;
	golem->move = golem->max_move;
	char_to_room(golem, ch->in_room);
	SET_BIT(golem->affected_by, AFF_CHARM);
	add_follower(golem, ch);
	golem->leader = ch;
	return;
}

void spell_animate_dead(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *zombie, *search;
	OBJ_DATA *corpse, *obj, *obj_next;
	AFFECT_DATA af;
	char *name, *last_name;
	char *obj_name;
	char buf1[MSL], buf2[MSL];
	int chance, z_level, control;

	if (level == 72)
		target_name = "corpse";

	if ((is_affected(ch, sn) || is_affected(ch, skill_lookup("mummify"))) && level < 70)
	{
		send_to_char("You have not yet regained your powers over the dead.\n\r", ch);
		return;
	}

	control = 0;

	for (search = char_list; search != NULL; search = search->next)
	{
		if (IS_NPC(search) && (search->master == ch) && search->pIndexData->vnum == MOB_VNUM_ZOMBIE)
			control += 6;
		else if (IS_NPC(search) && (search->master == ch) && search->pIndexData->vnum == MOB_VNUM_SKELETON)
			control += 4;
		else if (IS_NPC(search) && (search->master == ch) && search->pIndexData->vnum == MOB_VNUM_MUMMY)
			control += 12;
	}

	control += 4;

	if ((ch->level < 30 && control > 12) || (ch->level < 35 && control > 18) || (ch->level < 40 && control > 24) || (ch->level < 52 && control > 30) || control >= 36)
	{
		send_to_char("You already control as many undead as you can.\n\r", ch);
		return;
	}

	if (target_name[0] == '\0')
	{
		send_to_char("Animate which corpse?\n\r", ch);
		return;
	}

	obj_name = str_dup(target_name);

	corpse = get_obj_here(ch, obj_name);

	if (corpse == NULL)
	{
		send_to_char("You can't animate that.\n\r", ch);
		return;
	}

	if ((corpse->item_type != ITEM_CORPSE_NPC) && (corpse->item_type != ITEM_CORPSE_PC))
	{
		send_to_char("You can't animate that.\n\r", ch);
		return;
	}

	if (is_set(&corpse->extra_flags, CORPSE_NO_ANIMATE))
	{
		send_to_char("That corpse can not sustain further life beyond the grave.\n\r", ch);
		return;
	}

	name = corpse->short_descr;

	for (obj = corpse->contains; obj != NULL; obj = obj_next)
	{
		obj_next = obj->next_content;
		obj_from_obj(obj);
		obj_to_room(obj, ch->in_room);
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = sn;
	af.level = level;
	af.duration = 12;
	af.modifier = 0;
	af.location = 0;
	af.bitvector = 0;
	af.affect_list_msg = str_dup("unable to attempt animating the dead");
	af.aftype = AFT_SPELL;

	if (level < 70)
		affect_to_char(ch, &af);

	chance = get_skill(ch, sn);

	if (ch->level < corpse->level)
	{
		chance += (3 * ch->level);
		chance -= (3 * corpse->level);
	}

	chance = URANGE(20, chance, 90);

	if (number_percent() > chance)
	{
		act("You fail and destroy $p", ch, corpse, NULL, TO_CHAR);
		act("$n tries to animate a corpse but destroys it.", ch, NULL, NULL, TO_ROOM);
		extract_obj(corpse);
		return;
	}

	act("$n utters an incantation and a burning red glow flares into the eyes of $p.", ch, corpse, NULL, TO_ROOM);
	act("$p shudders and comes to life!", ch, corpse, NULL, TO_ROOM);
	act("You call upon the powers of the dark to give life to $p.", ch, corpse, NULL, TO_CHAR);

	zombie = create_mobile(get_mob_index(MOB_VNUM_ZOMBIE));

	z_level = UMAX(1, corpse->level - number_range(3, 6));
	zombie->level = z_level;
	zombie->max_hit = (dice(z_level, 15));
	zombie->max_hit += (z_level * 20);
	zombie->hit = zombie->max_hit;
	zombie->damroll += (z_level * 7 / 10);
	zombie->alignment = -1000;
	last_name = name;
	last_name = one_argument(corpse->short_descr, name);
	last_name = one_argument(last_name, name);
	last_name = one_argument(last_name, name);
	name = last_name;

	extract_obj(corpse);

	sprintf(buf1, "the zombie of %s", name);
	sprintf(buf2, "A zombie of %s is standing here.\n\r", name);
	free_string(zombie->short_descr);
	free_string(zombie->long_descr);
	zombie->short_descr = str_dup(buf1);
	zombie->long_descr = str_dup(buf2);

	char_to_room(zombie, ch->in_room);
	SET_BIT(zombie->affected_by, AFF_CHARM);
	add_follower(zombie, ch);
	zombie->leader = ch;
	return;
}

void spell_mummify(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *mummy, *search;
	OBJ_DATA *corpse, *obj, *obj_next;
	AFFECT_DATA af;
	char *name, *last_name, *obj_name;
	char buf1[MSL], buf2[MSL];
	int chance, z_level, control;

	if ((is_affected(ch, skill_lookup("animate dead")) || is_affected(ch, sn)) && level < 70)
	{
		send_to_char("You have not yet regained your powers to over the dead.\n\r", ch);
		return;
	}

	control = 0;

	for (search = char_list; search != NULL; search = search->next)
	{
		if (IS_NPC(search) && (search->master == ch) && search->pIndexData->vnum == MOB_VNUM_ZOMBIE)
			control += 6;
		else if (IS_NPC(search) && (search->master == ch) && search->pIndexData->vnum == MOB_VNUM_SKELETON)
			control += 4;
		else if (IS_NPC(search) && (search->master == ch) && search->pIndexData->vnum == MOB_VNUM_MUMMY)
			control += 12;
	}

	control += 4;

	if ((ch->level < 30 && control > 12) || (ch->level < 35 && control > 18) || (ch->level < 40 && control > 24) || (ch->level < 52 && control > 30) || control >= 36)
	{
		send_to_char("You already control as many undead as you can.\n\r", ch);
		return;
	}

	if (level >= 70)
		target_name = "corpse";

	if (target_name[0] == '\0')
	{
		send_to_char("Attempt to mummify which corpse?\n\r", ch);
		return;
	}
	obj_name = str_dup(target_name);

	corpse = get_obj_here(ch, obj_name);

	if (corpse == NULL)
	{
		send_to_char("You can't mummify that.\n\r", ch);
		return;
	}

	if ((corpse->item_type != ITEM_CORPSE_NPC) && (corpse->item_type != ITEM_CORPSE_PC))
	{
		send_to_char("You can't mummify and animate that.\n\r", ch);
		return;
	}

	if (is_set(&corpse->extra_flags, CORPSE_NO_ANIMATE))
	{
		send_to_char("That corpse does not have the stability to be animated anymore.\n\r", ch);
		return;
	}

	name = corpse->short_descr;

	for (obj = corpse->contains; obj != NULL; obj = obj_next)
	{
		obj_next = obj->next_content;
		obj_from_obj(obj);
		obj_to_room(obj, ch->in_room);
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = sn;
	af.level = level;
	af.duration = 20;
	af.modifier = 0;
	af.location = 0;
	af.bitvector = 0;
	af.aftype = AFT_SPELL;

	if (level < 70)
		affect_to_char(ch, &af);

	chance = get_skill(ch, sn);

	if (ch->level < corpse->level)
	{
		chance += (2 * ch->level);
		chance -= (2 * corpse->level);
	}

	chance = URANGE(10, chance, 90);

	if (number_percent() > chance)
	{
		act("You fail and destroy $p", ch, corpse, NULL, TO_CHAR);
		act("$n tries to mummify $p but destroys it.", ch, corpse, NULL, TO_ROOM);
		extract_obj(corpse);
		return;
	}

	act("$n utters an incantation and $p slowly stumbles to it's feet.", ch, corpse, NULL, TO_ROOM);
	act("$p shudders and slowly stumbles to it's feet!", ch, corpse, NULL, TO_ROOM);
	act("You invoke the powers of death and $p slowly rises to it's feet.", ch, corpse, NULL, TO_CHAR);

	mummy = create_mobile(get_mob_index(MOB_VNUM_MUMMY));

	z_level = UMAX(1, corpse->level - number_range(0, 2));
	mummy->level = z_level;
	mummy->max_hit = (dice(z_level, 25));
	mummy->max_hit += (z_level * 25);
	mummy->hit = mummy->max_hit;
	mummy->damroll += (z_level * 4 / 5);
	mummy->alignment = -1000;
	last_name = name;
	last_name = one_argument(corpse->short_descr, name);
	last_name = one_argument(last_name, name);
	last_name = one_argument(last_name, name);
	name = last_name;

	extract_obj(corpse);

	sprintf(buf1, "the mummy of %s", name);
	sprintf(buf2, "A torn and shredded mummy of %s is standing here.\n\r", name);
	free_string(mummy->short_descr);
	free_string(mummy->long_descr);
	mummy->short_descr = str_dup(buf1);
	mummy->long_descr = str_dup(buf2);

	char_to_room(mummy, ch->in_room);
	add_follower(mummy, ch);
	mummy->leader = ch;

	af.type = skill_lookup("animate dead");
	af.duration = -1;
	af.bitvector = AFF_CHARM;
	affect_to_char(mummy, &af);
	return;
}

void spell_animate_skeleton(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *skeleton, *search;
	OBJ_DATA *corpse, *obj, *obj_next;
	AFFECT_DATA af;
	char *name, *last_name, *obj_name;
	char buf1[MSL], buf2[MSL];
	int chance, z_level, control = 0, num_skeletons = 0;

	if (is_affected(ch, sn))
	{
		send_to_char("You have not yet regained your powers to animate bones.\n\r", ch);
		return;
	}

	for (search = char_list; search != NULL; search = search->next)
	{
		if (IS_NPC(search) && (search->master == ch) && search->pIndexData->vnum == MOB_VNUM_ZOMBIE)
			control += 6;
		else if (IS_NPC(search) && (search->master == ch) && search->pIndexData->vnum == MOB_VNUM_SKELETON)
		{
			control += 4;
			num_skeletons++;
		}
		else if (IS_NPC(search) && (search->master == ch) && search->pIndexData->vnum == MOB_VNUM_MUMMY)
			control += 12;
	}

	control += 4;

	if ((ch->level < 30 && control > 12) || (ch->level < 35 && control > 18) || (ch->level < 40 && control > 24) || (ch->level < 52 && control > 30) || control >= 36)
	{
		send_to_char("You already control as many undead as you can.\n\r", ch);
		return;
	}

	if (num_skeletons >= 3)
	{
		send_to_char("You cannot control any more skeletons.\n\r", ch);
		return;
	}

	if (target_name[0] == '\0')
	{
		send_to_char("Animate which skeleton?\n\r", ch);
		return;
	}

	obj_name = str_dup(target_name);

	corpse = get_obj_here(ch, obj_name);

	if (corpse == NULL)
	{
		send_to_char("You can't animate that.\n\r", ch);
		return;
	}

	if (corpse->item_type != ITEM_SKELETON)
	{
		send_to_char("You can't animate that.\n\r", ch);
		return;
	}

	if (is_set(&corpse->extra_flags, CORPSE_NO_ANIMATE))
	{
		send_to_char("That skeleton does not have the stability to be animated anymore.\n\r", ch);
		return;
	}

	name = corpse->short_descr;

	for (obj = corpse->contains; obj != NULL; obj = obj_next)
	{
		obj_next = obj->next_content;
		obj_from_obj(obj);
		obj_to_room(obj, ch->in_room);
	}

	chance = get_skill(ch, sn);

	if (ch->level < corpse->level)
	{
		chance += (4 * ch->level);
		chance -= (3 * corpse->level);
	}

	chance = URANGE(10, chance, 95);

	if (number_percent() > chance)
	{
		act("You fail and destroy $p", ch, corpse, NULL, TO_CHAR);
		act("$n tries to animate a skeleton but destroys it.", ch, NULL, NULL, TO_ROOM);
		extract_obj(corpse);
		return;
	}

	act("$n utters an incantation and $p slowly stumbles to it's feet.", ch, corpse, NULL, TO_ROOM);
	act("$p shudders and slowly stumbles to it's feet!", ch, corpse, NULL, TO_ROOM);
	act("You invoke the powers of death and $p slowly rises to it's feet.", ch, corpse, NULL, TO_CHAR);

	skeleton = create_mobile(get_mob_index(MOB_VNUM_SKELETON));

	z_level = UMAX(1, corpse->level - number_range(6, 11));
	skeleton->level = z_level;
	skeleton->max_hit = (dice(z_level, 12));
	skeleton->max_hit += (z_level * 15);
	skeleton->hit = skeleton->max_hit;
	skeleton->damroll += (z_level * 2 / 3);
	skeleton->alignment = -1000;
	last_name = name;

	last_name = one_argument(corpse->short_descr, name);
	last_name = one_argument(last_name, name);
	last_name = one_argument(last_name, name);
	name = last_name;

	extract_obj(corpse);

	sprintf(buf1, "the skeleton of %s", name);
	sprintf(buf2, "A skeleton of %s is standing here.\n\r", name);
	free_string(skeleton->short_descr);
	free_string(skeleton->long_descr);
	skeleton->short_descr = str_dup(buf1);
	skeleton->long_descr = str_dup(buf2);

	char_to_room(skeleton, ch->in_room);

	add_follower(skeleton, ch);
	skeleton->leader = ch;

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = sn;
	af.level = level;
	af.duration = 8;
	af.modifier = 0;
	af.location = 0;
	af.bitvector = 0;
	affect_to_char(ch, &af);

	af.type = skill_lookup("animate dead");
	af.duration = -1;
	af.bitvector = AFF_CHARM;
	affect_to_char(skeleton, &af);
	return;
}
