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

/*
 * Local functions.
 */
int hit_gain args((CHAR_DATA * ch));
int mana_gain args((CHAR_DATA * ch));
int move_gain args((CHAR_DATA * ch));
void prelag_skills args((void));
void mobile_update args((void));
void weather_update args((void));
void char_update args((void));
void obj_update args((void));
void aggr_update args((void));
void riot_update args((void));
void age_update args((void));
void room_update args((void));
void room_affect_update args((void));
void iprog_pulse_update args((bool isTick));
void healing_sleep_wakeup args((CHAR_DATA * ch));
void updateRaceAff args((CHAR_DATA * ch));

/* used for saving */
int reverse_d args((int dir));
int save_number = 0;

CHAR_DATA *affectowner;
CHAR_DATA *affect_owner;

/*
 * Advancement stuff.
 */
void advance_level(CHAR_DATA *ch, bool hide)
{
	char buf[MAX_STRING_LENGTH];

	ch->pcdata->last_level = (ch->played + (int)(current_time - ch->logon)) / 3600;

	sprintf(buf, "the %s",
			title_table[ch->class][ch->level][ch->sex == SEX_FEMALE ? 1 : 0]);
	set_title(ch, buf);

	return;
}

void gain_quest_credits(CHAR_DATA *ch, int gain)
{
	if (IS_NPC(ch))
		return;

	ch->quest_credits += gain;
	if (ch->quest_credits < 0)
		ch->quest_credits = 0;
	save_char_obj(ch);

	return;
}

void gain_exp(CHAR_DATA *ch, int gain)
{
	char buf[MAX_STRING_LENGTH];

	if (IS_NPC(ch))
		return;

	if (ch->level < LEVEL_HERO)
		ch->exp += gain;

	if (ch->exp > ch->exp_total)
		ch->exp_total = ch->exp;

	while (ch->level < LEVEL_HERO && ch->exp >=
										 exp_per_level(ch) * (ch->level))
	{
		send_to_char("You raise a level!!  ", ch);
		ch->level += 1;
		sprintf(buf, "%s gained level %d", ch->name, ch->level);
		log_string(buf);
		sprintf(buf, "$N has attained level %d!", ch->level);
		wiznet(buf, ch, NULL, WIZ_LEVELS, 0, 0);
		advance_level(ch, FALSE);
		save_char_obj(ch);
	}

	return;
}

/*
 * Regeneration stuff.
 */
int hit_gain(CHAR_DATA *ch)
{
	int gain;
	int number;

	if (ch->in_room == NULL)
		return 0;

	if (ch->in_room->vnum == ROOM_VNUM_NIGHTWALK || ch->in_room->vnum == 2901)
	{
		number = gsn_shadowplane;
		damage_old(ch, ch, 40, number, DAM_NEGATIVE, TRUE);
		return 0;
	}

	if (is_affected(ch, gsn_prevent_healing))
		return 0;

	if (IS_NPC(ch))
	{
		gain = 5 + (ch->level);
		if (IS_AFFECTED(ch, AFF_REGENERATION))
			gain *= 2;

		switch (ch->position)
		{
		default:
			gain /= 2;
			break;
		case POS_SLEEPING:
			gain = 3 * gain / 2;
			break;
		case POS_RESTING:
			break;
		case POS_FIGHTING:
			gain /= 3;
			break;
		}
	}
	else
	{
		gain = UMAX(3, get_curr_stat(ch, STAT_CON) - 3 + (ch->level * (9.5 / 10)));
		gain += class_table[ch->class].hp_max;
		number = number_percent();
		if (number < get_skill(ch, gsn_fast_healing))
		{
			gain += number * gain / 100;
			if (ch->hit < ch->max_hit)
				check_improve(ch, gsn_fast_healing, TRUE, 8);
		}

		switch (ch->position)
		{
		default:
			gain /= 4;
			break;
		case POS_SLEEPING:
			break;
		case POS_RESTING:
			gain /= 2;
			break;
		case POS_FIGHTING:
			gain /= 6;
			break;
		}
	}

	if (ch->on != NULL && ch->on->item_type == ITEM_FURNITURE)
		gain = (gain * 7 / 5);

	/*if ( IS_AFFECTED(ch, AFF_f ( IS_A)
	gain /= 4;*/

	if (IS_AFFECTED(ch, AFF_PLAGUE))
		gain /= 2;

	if (ch->position == POS_SLEEPING && get_skill(ch, gsn_dark_dream) > 5)
	{
		if (number_percent() < get_skill(ch, gsn_dark_dream))
		{
			check_improve(ch, gsn_dark_dream, TRUE, 7);
			gain *= 3;
			gain /= 2;
		}
	}

	if (IS_AFFECTED(ch, AFF_HASTE))
		gain /= 2;
	if (IS_AFFECTED(ch, AFF_SLOW))
	{
		gain *= 17;
		gain /= 10;
	}

	if (is_affected(ch, gsn_camp))
	{
		if (number_percent() < ch->pcdata->learned[gsn_camp])
			gain *= 2;
	}
	gain *= 4;
	return UMIN(gain, ch->max_hit - ch->hit);
}

int mana_gain(CHAR_DATA *ch)
{
	int gain;
	int number;

	if (ch->in_room == NULL)
		return 0;

	if (is_affected(ch, gsn_prevent_healing))
		return 0;

	if (IS_NPC(ch))
	{

		gain = 5 + ch->level;

		switch (get_curr_stat(ch, STAT_WIS))
		{
		case 25:
			gain += 7 * gain / 20;
			break;
		case 24:
			gain += 6 * gain / 20;
			break;
		case 23:
			gain += 5 * gain / 20;
			break;
		case 22:
			gain += 4 * gain / 20;
			break;
		case 21:
			gain += 3 * gain / 20;
			break;
		case 20:
			gain += 2 * gain / 20;
			break;
		case 19:
			gain += 1 * gain / 20;
			break;
		default:
			gain += 0;
			break;
		}

		switch (ch->position)
		{
		default:
			gain /= 2;
			break;
		case POS_SLEEPING:
			gain = 3 * gain / 2;
			break;
		case POS_RESTING:
			break;
		case POS_FIGHTING:
			gain /= 3;
			break;
		}
	}
	else
	{
		gain = (get_curr_stat(ch, STAT_WIS) / 2 - 9 + get_curr_stat(ch, STAT_INT) * 2 + ch->level);
		number = number_percent();
		if (number < get_skill(ch, gsn_meditation))
		{
			gain += number * gain / 100;
			if (ch->mana < ch->max_mana)
				check_improve(ch, gsn_meditation, TRUE, 4);
		}
		number = number_percent();
		if (number < get_skill(ch, gsn_trance))
		{
			gain += number * gain / 100;
			if (ch->mana < ch->max_mana)
				check_improve(ch, gsn_trance, TRUE, 4);
		}

		switch (ch->position)
		{
		default:
			gain /= 4;
			break;
		case POS_SLEEPING:
			break;
		case POS_RESTING:
			gain /= 2;
			break;
		case POS_FIGHTING:
			gain /= 6;
			break;
		}
	}

	if (ch->on != NULL && ch->on->item_type == ITEM_FURNITURE)
		gain = gain * 7 / 5;

	if (IS_AFFECTED(ch, AFF_POISON))
		gain /= 2;

	if (ch->position == POS_SLEEPING && get_skill(ch, gsn_dark_dream) > 5)
	{
		if (number_percent() < get_skill(ch, gsn_dark_dream))
		{
			check_improve(ch, gsn_dark_dream, TRUE, 5);
			gain *= 3;
			gain /= 2;
		}
	}

	if (IS_AFFECTED(ch, AFF_PLAGUE))
		gain /= 2;

	if (IS_AFFECTED(ch, AFF_HASTE))
		gain /= 2;
	if (IS_AFFECTED(ch, AFF_SLOW))
		gain += (7 * gain / 10);

	if (is_affected(ch, gsn_camp))
	{
		if (number_percent() < ch->pcdata->learned[gsn_camp])
			gain *= 2;
	}
	gain *= 2;

	if (get_skill(ch, skill_lookup("mana conduit")) > 75)
		gain *= 1.75;

	gain = gain * ch->in_room->mana_rate / 100;
	gain *= 2;
	return UMIN(gain, ch->max_mana - ch->mana);
}

int move_gain(CHAR_DATA *ch)
{
	int gain;

	if (ch->in_room == NULL)
		return 0;
	if (is_affected(ch, gsn_atrophy) || is_affected(ch, gsn_prevent_healing))
		return 0;
	/*
		if (!IS_NPC(ch))
		{
		if (ch->pcdata->condition[COND_STARVING] > 6)
			return 0;
		if (ch->pcdata->condition[COND_DEHYDRATED] > 4)
			return 0;
		}
	*/
	if (IS_NPC(ch))
	{
		gain = ch->level;
	}
	else
	{
		gain = UMAX(15, ch->level);

		switch (ch->position)
		{
		case POS_SLEEPING:
			gain += get_curr_stat(ch, STAT_DEX);
			break;
		case POS_RESTING:
			gain += get_curr_stat(ch, STAT_DEX) / 2;
			break;
		}
	}

	gain = gain * ch->in_room->heal_rate / 100;

	if (ch->on != NULL && ch->on->item_type == ITEM_FURNITURE)
		gain = gain * 6 / 5;

	if (ch->position == POS_SLEEPING && get_skill(ch, gsn_dark_dream) > 5)
	{
		if (number_percent() < get_skill(ch, gsn_dark_dream))
		{
			check_improve(ch, gsn_dark_dream, TRUE, 8);
			gain *= 3;
			gain /= 2;
		}
	}

	if (IS_AFFECTED(ch, AFF_POISON))
		gain /= 2;

	if (IS_AFFECTED(ch, AFF_PLAGUE))
		gain /= 2;

	if (IS_AFFECTED(ch, AFF_HASTE) || IS_AFFECTED(ch, AFF_SLOW))
		gain *= 2;

	if (is_affected(ch, gsn_camp))
	{
		if (number_percent() < ch->pcdata->learned[gsn_camp])
			gain *= 2;
	}
	gain *= 4;
	return UMIN(gain, ch->max_move - ch->move);
}

/* If you don't want starvation/dehydration in your mud then simply return
from function before the if (ch->level > 10) check...(Ceran)
*/
void gain_condition(CHAR_DATA *ch, int iCond, int value)
{
	int condition;
	int counter;

	if (value == 0 || IS_NPC(ch) || ch->level >= LEVEL_IMMORTAL)
		return;

	condition = ch->pcdata->condition[iCond];
	if (condition == -1)
		return;
	ch->pcdata->condition[iCond] = URANGE(0, condition + value, 48);
	return;
	if (ch->level > 10)
	{
		if (ch->pcdata->condition[iCond] == 0 && iCond == COND_HUNGER)
			ch->pcdata->condition[COND_STARVING]++;
		if (ch->pcdata->condition[iCond] == 0 && iCond == COND_THIRST)
			ch->pcdata->condition[COND_DEHYDRATED]++;
	}
	else
	{
		ch->pcdata->condition[COND_STARVING] = 0;
		ch->pcdata->condition[COND_DEHYDRATED] = 0;
	}

	if (iCond == COND_HUNGER && value > 0 && ch->pcdata->condition[COND_STARVING] > 0)
	{
		counter = ch->pcdata->condition[COND_STARVING];
		if (counter <= 4)
			send_to_char("You are no longer famished.\n\r", ch);
		else
			send_to_char("You are no longer starving.\n\r", ch);
		ch->pcdata->condition[COND_STARVING] = 0;
		ch->pcdata->condition[COND_HUNGER] = 2;
	}

	if (iCond == COND_THIRST && value > 0 && ch->pcdata->condition[COND_DEHYDRATED] > 0)
	{
		counter = ch->pcdata->condition[COND_DEHYDRATED];
		if (counter <= 5)
			send_to_char("You are no longer dehydrated.\n\r", ch);
		else
			send_to_char("You are no longer dying of thirst.\n\r", ch);
		ch->pcdata->condition[COND_DEHYDRATED] = 0;
		ch->pcdata->condition[COND_THIRST] = 2;
	}

	if (ch->pcdata->condition[iCond] <= 4)
	{
		switch (iCond)
		{
		case COND_HUNGER:
			if (ch->pcdata->condition[COND_STARVING] < 2)
				send_to_char("You are hungry.\n\r", ch);
			break;

		case COND_THIRST:
			if (ch->pcdata->condition[COND_DEHYDRATED] < 2)
				send_to_char("You are thirsty.\n\r", ch);
			break;

		case COND_DRUNK:
			if (condition != 0)
				send_to_char("You are sober.\n\r", ch);
			break;
		}
	}

	if (ch->pcdata->condition[COND_STARVING] > 1 && iCond == COND_HUNGER)
	{
		counter = ch->pcdata->condition[COND_STARVING];
		if (counter <= 5)
			send_to_char("You are famished!\n\r", ch);
		else if (counter <= 8)
			send_to_char("You are beginning to starve!\n\r", ch);
		else
		{
			send_to_char("You are starving!\n\r", ch);
			if (ch->level > 10)
				damage_old(ch, ch, number_range(counter - 3, 2 * (counter - 3)), gsn_starvation, DAM_OTHER, TRUE);
		}
	}
	if (ch->pcdata->condition[COND_DEHYDRATED] > 1 && iCond == COND_THIRST)
	{
		counter = ch->pcdata->condition[COND_DEHYDRATED];
		if (counter <= 2)
			send_to_char("Your mouth is parched!\n\r", ch);
		else if (counter <= 5)
			send_to_char("You are beginning to dehydrate!\n\r", ch);
		else
		{
			send_to_char("You are dying of thirst!\n\r", ch);
			if (ch->level > 10)
				damage_old(ch, ch, number_range(counter, 2 * counter), gsn_dehydrated, DAM_OTHER, TRUE);
		}
	}

	return;
}

void spec_update(CHAR_DATA *ch)
{
	CHAR_DATA *mob;
	CHAR_DATA *mob_next;

	for (mob = ch->in_room->people; mob != NULL; mob = mob_next)
	{
		mob_next = mob->next;

		if (IS_NPC(mob) && mob->spec_fun != 0)
		{
			if ((*mob->spec_fun)(mob))
				return;
		}
	}
	return;
}

/*
 * Mob autonomous action.
 * This function takes 25% to 35% of ALL Merc cpu time.
 * -- Furey
 */

void mobile_update(void)
{
	CHAR_DATA *ch;
	CHAR_DATA *ch_next;
	EXIT_DATA *pexit;
	int door;

	/* Examine all mobs. */
	for (ch = char_list; ch != NULL; ch = ch_next)
	{
		ch_next = ch->next;

		if (!IS_NPC(ch) || ch->in_room == NULL)
			continue;

		if (ch->position == POS_FIGHTING && ch->extended_flags > 3 && number_percent() > 50 && ch->wait <= 0)
		{
			do_mob_spec(ch);
		}

		if (ch->in_room->area->empty && !IS_SET(ch->act, ACT_UPDATE_ALWAYS))
			continue;

		/* Examine call for special procedure */
		if (ch->spec_fun != 0 && (ch->wait <= 0))
		{
			if ((*ch->spec_fun)(ch))
				continue;
		}
		/*
		 * Check triggers only if mobile still in default position
		 */

		if (ch->pIndexData->pShop != NULL) /* give him some gold */
			if (ch->gold < ch->pIndexData->wealth)
			{
				ch->gold += ch->pIndexData->wealth * number_range(1, 20) / 5000000;
			}

		/* That's all for sleeping / busy monster, and empty zones */
		if (ch->position != POS_STANDING)
			continue;

		if (IS_SET(ch->progtypes, MPROG_PULSE) && (ch->in_room->area->nplayer > 0 || IS_SET(ch->act, ACT_UPDATE_ALWAYS)))
			(ch->pIndexData->mprogs->pulse_prog)(ch);

		/* Scavenge */
		if (IS_SET(ch->act, ACT_SCAVENGER) && ch->in_room->contents != NULL && number_bits(6) == 0)
		{
			OBJ_DATA *obj;
			OBJ_DATA *obj_best;
			int max;

			max = 1;
			obj_best = 0;
			for (obj = ch->in_room->contents; obj; obj = obj->next_content)
			{
				if (CAN_WEAR(obj, ITEM_TAKE) && can_loot(ch, obj) && obj->cost > max && obj->cost > 0)
				{
					obj_best = obj;
					max = obj->cost;
				}
			}

			if (obj_best)
			{
				/*		get_obj(ch,obj,NULL);*/
				obj_from_room(obj_best);
				obj_to_char(obj_best, ch);
				act("$n gets $p.", ch, obj_best, NULL, TO_ROOM);
			}
		}

		/* Wander */
		if (!IS_SET(ch->act, ACT_SENTINEL) && number_bits(3) == 0 && (door = number_bits(5)) <= 5 && (pexit = ch->in_room->exit[door]) != NULL && pexit->u1.to_room != NULL && !IS_SET(pexit->exit_info, EX_CLOSED) && !IS_SET(pexit->u1.to_room->room_flags, ROOM_NO_MOB) && (!IS_SET(ch->act, ACT_STAY_AREA) || pexit->u1.to_room->area == ch->in_room->area) && (!IS_SET(ch->act, ACT_OUTDOORS) || !IS_SET(pexit->u1.to_room->room_flags, ROOM_INDOORS)) && (!IS_SET(ch->act, ACT_INDOORS) || IS_SET(pexit->u1.to_room->room_flags, ROOM_INDOORS)))
		{
			move_char(ch, door, FALSE);
		}
	}

	return;
}

void weather_update(void)
{
	char buf[MAX_STRING_LENGTH];
	DESCRIPTOR_DATA *d;
	int diff;

	buf[0] = '\0';

	switch (++time_info.hour)
	{
	case 5:
		weather_info.sunlight = SUN_LIGHT;
		strcat(buf, "The day has begun.\n\r");
		break;

	case 6:
		weather_info.sunlight = SUN_RISE;
		strcat(buf, "The sun rises in the east.\n\r");
		break;

	case 18:
		weather_info.sunlight = SUN_SET;
		strcat(buf, "The sun slowly disappears in the west.\n\r");
		break;

	case 19:
		weather_info.sunlight = SUN_DARK;
		strcat(buf, "The night has begun.\n\r");
		break;

	case 20:
		if ((time_info.day + 1) == 1)
			strcat(buf, "A new moon rises in the night sky.\n\r");
		else if ((time_info.day + 1) >= 2 && (time_info.day + 1) <= 7)
			strcat(buf, "A waxing half moon rises in the night sky.\n\r");
		else if ((time_info.day + 1) == 8)
			strcat(buf, "A half moon rises in the night sky.\n\r");
		else if ((time_info.day + 1) >= 9 && (time_info.day + 1) <= 14)
			strcat(buf, "A waxing full moon rises in the night sky.\n\r");
		else if ((time_info.day + 1) == 15)
			strcat(buf, "A full moon rises in the night sky.\n\r");
		else if ((time_info.day + 1) >= 16 && (time_info.day + 1) <= 21)
			strcat(buf, "A waning full moon rises in the night sky.\n\r");
		else if ((time_info.day + 1) == 22)
			strcat(buf, "A half moon rises in the night sky.\n\r");
		else if ((time_info.day + 1) >= 23 && (time_info.day + 1) <= 28)
			strcat(buf, "A waning half moon rises in the night sky.\n\r");
		break;

	case 24:
		time_info.hour = 0;
		time_info.day++;
		break;
	}

	/* Time/Date counters */

	if (time_info.day >= 28)
	{
		time_info.day = 0;
		time_info.month++;
	}

	if (time_info.month >= 17)
	{
		time_info.month = 0;
		time_info.year++;
	}

	/*
	 * Weather change.
	 */
	if (time_info.month >= 9 && time_info.month <= 16)
		diff = weather_info.mmhg > 985 ? -2 : 2;
	else
		diff = weather_info.mmhg > 1015 ? -2 : 2;

	weather_info.change += diff * dice(1, 4) + dice(2, 6) - dice(2, 6);
	weather_info.change = UMAX(weather_info.change, -12);
	weather_info.change = UMIN(weather_info.change, 12);

	weather_info.mmhg += weather_info.change;
	weather_info.mmhg = UMAX(weather_info.mmhg, 960);
	weather_info.mmhg = UMIN(weather_info.mmhg, 1040);

	switch (weather_info.sky)
	{
	default:
		bug("Weather_update: bad sky %d.", weather_info.sky);
		weather_info.sky = SKY_CLOUDLESS;
		break;

	case SKY_CLOUDLESS:
		if (weather_info.mmhg < 990 || (weather_info.mmhg < 1010 && number_bits(2) == 0))
		{
			strcat(buf, "The sky is getting cloudy.\n\r");
			weather_info.sky = SKY_CLOUDY;
		}
		break;

	case SKY_CLOUDY:
		if (weather_info.mmhg < 970 || (weather_info.mmhg < 990 && number_bits(2) == 0))
		{
			strcat(buf, "It starts to rain.\n\r");
			weather_info.sky = SKY_RAINING;
		}

		if (weather_info.mmhg > 1030 && number_bits(2) == 0)
		{
			strcat(buf, "The clouds disappear.\n\r");
			weather_info.sky = SKY_CLOUDLESS;
		}
		break;

	case SKY_RAINING:
		if (weather_info.mmhg < 970 && number_bits(2) == 0)
		{
			strcat(buf, "Lightning flashes in the sky.\n\r");
			weather_info.sky = SKY_LIGHTNING;
		}

		if (weather_info.mmhg > 1030 || (weather_info.mmhg > 1010 && number_bits(2) == 0))
		{
			strcat(buf, "The rain stopped.\n\r");
			weather_info.sky = SKY_CLOUDY;
		}
		break;

	case SKY_LIGHTNING:
		if (weather_info.mmhg > 1010 || (weather_info.mmhg > 990 && number_bits(2) == 0))
		{
			strcat(buf, "The lightning has stopped.\n\r");
			weather_info.sky = SKY_RAINING;
			break;
		}
		break;
	}

	if (buf[0] != '\0')
	{
		for (d = descriptor_list; d != NULL; d = d->next)
		{
			if (d->connected == CON_PLAYING && IS_OUTSIDE(d->character))
			{
				if (IS_AWAKE(d->character))
					send_to_char(buf, d->character);
			}
		}
	}

	return;
}

/*
 * Update all chars, including mobs.
 */
void char_update(void)
{
	CHAR_DATA *ch;
	CHAR_DATA *ch_next;
	CHAR_DATA *ch_quit;
	int noregen, hgain;
	char chbuf[MAX_STRING_LENGTH];

	ch_quit = NULL;

	/* update save counter */
	save_number++;

	if (save_number > 29)
		save_number = 0;

	for (ch = char_list; ch != NULL; ch = ch_next)
	{
		CHAR_DATA *master;
		AFFECT_DATA *paf;
		AFFECT_DATA *paf_next;
		bool charm_gone;

		ch_next = ch->next;
		if (ch->timer > 30)
			ch_quit = ch;
		master = NULL;
		if (ch->pause > 0)
			ch->pause--;

		if (ch->ghost > 0)
		{
			if (ch->ghost == 5)
				send_to_char("{RYou start to regain your material form.{x\n\r", ch);
			ch->ghost--;
			if (ch->ghost == 0)
				send_to_char("{RYour body regains it's material form.{x\n\r", ch);
		}

		updateRaceAff(ch);

		AFFECT_DATA applyAffect;
		int drownLevel = 0;
		bool deadFromDrown = FALSE;

		if (ch->in_room->sector_type != SECT_UNDERWATER && is_affected(ch, gsn_drowning))
			affect_strip(ch, gsn_drowning);
		if (ch->in_room->sector_type == SECT_UNDERWATER && !IS_AFFECTED(ch, AFF_WATERBREATHING) && !IS_IMMORTAL(ch) && ch->ghost == 0 && !IS_NPC(ch))
		{
			for (paf = ch->affected; paf != NULL; paf = paf_next)
			{
				paf_next = paf->next;
				if (paf->type == gsn_drowning)
				{
					drownLevel = paf->level;
					if (paf->level == 3)
					{
						act("$n finds $mself lacking air!", ch, 0, 0, TO_ROOM);
						send_to_char("You find yourself lacking air!\n\r", ch);
					}
					else if (paf->level == 4)
					{
						act("$n struggles to keep $s air under the water!", ch, 0, 0, TO_ROOM);
						send_to_char("You struggle to keep your air under the water!\n\r", ch);
					}
					else if (paf->level == 5)
					{
						act("$n clutches $s chest as $s lungs begin filling with water!", ch, 0, 0, TO_ROOM);
						send_to_char("Your lungs begin filling with water!\n\r", ch);
					}
					else if (paf->level > 5 && paf->level < 9)
					{
						act("$n chokes and coughs as $s lungs fill with water!", ch, 0, 0, TO_ROOM);
						send_to_char("You choke as your lungs fill with water!\n\r", ch);
					}
					else if (paf->level == 9)
					{
						act("$n appears severely weakened as the water floods $s lungs!", ch, 0, 0, TO_ROOM);
						send_to_char("Your heartbeat slows as your remaining oxygen is used up.\n\r", ch);
					}
					else if (paf->level == 10)
					{
						act("$n's body floats limply in the water, devoid of life.", ch, 0, 0, TO_ROOM);
						send_to_char("Peace fills your mind as the struggle to breathe ends...\n\r", ch);
						raw_kill_new(ch, ch, FALSE, TRUE);
						affect_strip(ch, gsn_drowning);
						deadFromDrown = TRUE;
					}
				}
			}

			if (drownLevel != 0 && drownLevel < 11 && is_affected(ch, gsn_drowning))
			{
				if (drownLevel > 3)
					damage_new(ch, ch, drownLevel * 40, TYPE_UNDEFINED, DAM_DROWNING, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "drowning");
				affect_strip(ch, gsn_drowning);
				init_affect(&applyAffect);
				applyAffect.aftype = AFT_INVIS;
				applyAffect.where = TO_AFFECTS;
				applyAffect.type = gsn_drowning;
				applyAffect.level = drownLevel + 1;
				applyAffect.duration = -1;
				affect_to_char(ch, &applyAffect);
			}

			if (drownLevel == 0 && !deadFromDrown)
			{
				init_affect(&applyAffect);
				applyAffect.aftype = AFT_INVIS;
				applyAffect.where = TO_AFFECTS;
				applyAffect.type = gsn_drowning;
				applyAffect.level = 1;
				applyAffect.duration = -1;
				affect_to_char(ch, &applyAffect);
			}
		}

		if (ch->position >= POS_STUNNED)
		{
			if (ch->hit < ch->max_hit) /* Some regen stuff */
			{
				noregen = ch->noregen_dam;
				hgain = hit_gain(ch);
				if (is_affected(ch, gsn_bloodlust))
				{
					hgain += number_range(90, 120);
					hgain = UMIN(hgain, ch->max_hit - ch->hit);
				}
				ch->hit += hgain;
				if (noregen > hgain)
					ch->noregen_dam -= hgain;
				else
					ch->noregen_dam = 0;
			}
			else
				ch->hit = ch->max_hit;

			if (ch->mana < ch->max_mana)
				ch->mana += mana_gain(ch);
			else
				ch->mana = ch->max_mana;

			if (ch->move < ch->max_move)
				ch->move += move_gain(ch);
			else
				ch->move = ch->max_move;
		}

		if (ch->position == POS_STUNNED)
			update_pos(ch);

		if (!IS_NPC(ch) && ch->pcdata->save_timer)
			ch->pcdata->save_timer--;

		if (!IS_NPC(ch))
		{
			OBJ_DATA *obj;

			if ((obj = get_eq_char(ch, WEAR_LIGHT)) != NULL && obj->item_type == ITEM_LIGHT && obj->value[2] > 0)
			{
				if (--obj->value[2] == 0 && ch->in_room != NULL)
				{
					ch->in_room->light -= 3;
					act("$p goes out.", ch, obj, NULL, TO_ROOM);
					act("$p flickers and goes out.", ch, obj, NULL, TO_CHAR);
					extract_obj(obj);
				}
				else if (obj->value[2] <= 5 && ch->in_room != NULL)
					act("$p flickers.", ch, obj, NULL, TO_CHAR);
			}

			if (IS_IMMORTAL(ch))
				ch->timer = 0;

			if (++ch->timer >= 12)
			{
				if (ch->was_in_room == NULL && ch->in_room != NULL)
				{
					ch->was_in_room = ch->in_room;
					if (ch->fighting != NULL)
						stop_fighting(ch, TRUE);
					act("$n disappears into the void.",
						ch, NULL, NULL, TO_ROOM);
					send_to_char("You disappear into the void.\n\r", ch);
					if (ch->level > 1)
						save_char_obj(ch);
					ch->pause = 0;
					do_quit_new(ch, "", TRUE);
				}
			}
		}

		for (paf = ch->affected; paf != NULL; paf = paf_next)
		{
			paf_next = paf->next;
			charm_gone = FALSE;
			if (paf->duration > 0)
			{
				paf->duration--;
			}
			else if (paf->duration < 0)
				;
			else

			{
				if (paf_next == NULL || paf_next->type != paf->type || paf_next->duration > 0)
				{
					if (paf->type > 0 && skill_table[paf->type].msg_off)
					{
						send_to_char(skill_table[paf->type].msg_off, ch);
						send_to_char("\n\r", ch);
					}
				}

				if (paf->type == skill_lookup("charm") && IS_NPC(ch))
					charm_gone = TRUE;

				if (paf->type == skill_lookup("healing sleep"))
					healing_sleep_wakeup(ch);

				if (paf->type == skill_lookup("resurrect"))
				{
					resurrect(ch, paf);
				}

				affect_remove(ch, paf);
			}

			if (charm_gone)
				master = ch->master;
			if (master == NULL)
				continue;
			if ((charm_gone) && ch->position != POS_FIGHTING && (number_percent() >= master->level) && (ch->in_room == master->in_room))
			{
				sprintf(chbuf, "Help! I'm being attacked by %s!",
						IS_NPC(ch) ? ch->short_descr : ch->name);

				do_myell(master, chbuf);
				multi_hit(ch, master, TYPE_UNDEFINED);
			}
		}

		/*
		 * Careful with the damages here,
		 *   MUST NOT refer to ch after damage taken,
		 *   as it may be lethal damage (on NPC).
		 */
		CHAR_DATA *af_owner;
		if (is_affected(ch, gsn_plague) && ch != NULL)
		{
			AFFECT_DATA *af, plague;
			CHAR_DATA *vch;
			int dam;

			if (ch->race == race_lookup("lich"))
				act("You ignore the plague completely.", ch, 0, 0, TO_CHAR);
			else
			{
				if (ch->in_room == NULL)
					return;

				act("$n writhes in agony as plague sores erupt from $s skin.", ch, NULL, NULL, TO_ROOM);
				send_to_char("You writhe in agony from the plague.\n\r", ch);
				for (af = ch->affected; af != NULL; af = af->next)
				{
					if (af->type == gsn_plague)
						break;
				}

				if (af == NULL)
				{
					REMOVE_BIT(ch->affected_by, AFF_PLAGUE);
					return;
				}

				if (af->level == 1)
					return;

				init_affect(&plague);
				plague.where = TO_AFFECTS;
				plague.aftype = AFT_MALADY;
				plague.type = gsn_plague;
				plague.level = af->level - 1;
				plague.duration = number_range(1, 2 * plague.level);
				plague.location = APPLY_STR;
				plague.modifier = -3;
				plague.owner_name = af->owner_name ? str_dup(af->owner_name) : NULL;
				plague.bitvector = AFF_PLAGUE;

				for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
				{
					if (!saves_spell(plague.level - 2, vch, DAM_DISEASE) && !IS_IMMORTAL(vch) && !IS_AFFECTED(vch, AFF_PLAGUE) && number_bits(4) == 0)
					{
						send_to_char("You feel hot and feverish.\n\r", vch);
						act("$n shivers and looks very ill.", vch, NULL, NULL, TO_ROOM);
						affect_join(vch, &plague);
					}
				}

				dam = UMIN(ch->level, af->level);

				if ((af_owner = find_char_by_name(af->owner_name)) != NULL)
					damage_newer(af_owner, ch, dam, gsn_plague, DAM_DISEASE, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "sickness", PLURAL, DAM_POSSESSIVE, af, FALSE);
				else
					damage_newer(ch, ch, dam, gsn_plague, DAM_DISEASE, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "sickness", PLURAL, DAM_POSSESSIVE, NULL, FALSE);
				ch->mana = (ch->mana - dice(af->level, 1));
				ch->move = (ch->move - dice(af->level, 1));
			}
		}
		if (IS_AFFECTED(ch, AFF_POISON) && ch != NULL && !IS_AFFECTED(ch, AFF_SLOW))

		{
			AFFECT_DATA *poison;

			poison = affect_find(ch->affected, gsn_poison);

			if (poison != NULL && ch->race != race_lookup("lich"))
			{
				act("$n shivers and suffers.", ch, NULL, NULL, TO_ROOM);
				send_to_char("You shiver and suffer.\n\r", ch);

				if ((af_owner = find_char_by_name(poison->owner_name)) != NULL)
					damage_newer(af_owner, ch, poison->level, gsn_poison, DAM_POISON, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "poison", PLURAL, DAM_POSSESSIVE, poison, FALSE);
				else
					damage_newer(ch, ch, poison->level, gsn_poison, DAM_POISON, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "poison", PLURAL, DAM_POSSESSIVE, NULL, FALSE);
			}
			else if (poison != NULL && ch->race == race_lookup("lich"))
				send_to_char("Your undead body ignores the poison.\n\r", ch);
		}

		if (is_affected(ch, gsn_haunting) && ch != NULL)
		{
			AFFECT_DATA *haunting = affect_find(ch->affected, gsn_haunting);

			act("$n flinches as haunting images flash through their mind!", ch, NULL, NULL, TO_ROOM);
			send_to_char("You flinch as haunting images flash through your mind!\n\r", ch);

			if ((af_owner = find_char_by_name(haunting->owner_name)) != NULL)
				damage_newer(af_owner, ch, number_range(50, 150), gsn_haunting, DAM_MENTAL, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "haunting thoughts", SINGULAR, DAM_POSSESSIVE, haunting, FALSE);
			else
				damage_newer(ch, ch, number_range(50, 150), gsn_haunting, DAM_MENTAL, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "haunting thoughts", SINGULAR, DAM_POSSESSIVE, NULL, FALSE);
			ch->mana -= 50;
		}

		if (is_affected(ch, skill_lookup("critical strike")))
		{
			AFFECT_DATA *criticalStrike = affect_find(ch->affected, skill_lookup("critical strike"));

			if (criticalStrike->level > 1)
			{
				act("You cough as blood pours from your gaping wound.", ch, 0, 0, TO_CHAR);
				act("$n coughs as blood pours from $s gaping wound.", ch, 0, 0, TO_ROOM);
				if ((af_owner = find_char_by_name(criticalStrike->owner_name)) != NULL)
					damage_newer(af_owner, ch, criticalStrike->level * number_range(17, 23), TYPE_UNDEFINED, DAM_OTHER, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "the uncontrollable bleeding", PLURAL, DAM_NOT_POSSESSIVE, criticalStrike, FALSE);
				else
					damage_newer(ch, ch, criticalStrike->level * number_range(17, 23), TYPE_UNDEFINED, DAM_OTHER, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "the uncontrollable bleeding", PLURAL, DAM_NOT_POSSESSIVE, NULL, FALSE);
			}
		}

		if (is_affected(ch, skill_lookup("assassinate")))
		{
			AFFECT_DATA *assassinate = affect_find(ch->affected, skill_lookup("assassinate"));

			act("You cough as blood pours from your assassination wounds.", ch, 0, 0, TO_CHAR);
			act("$n coughs as blood pours from $s assassination wounds.", ch, 0, 0, TO_ROOM);
			if ((af_owner = find_char_by_name(assassinate->owner_name)) != NULL)
				damage_newer(af_owner, ch, 2 * number_range(17, 23), TYPE_UNDEFINED, DAM_OTHER, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "assassination wounds", SINGULAR, DAM_POSSESSIVE, assassinate, FALSE);
			else
				damage_newer(ch, ch, 2 * number_range(17, 23), TYPE_UNDEFINED, DAM_OTHER, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "assassination wounds", SINGULAR, DAM_POSSESSIVE, NULL, FALSE);
		}

		if (is_affected(ch, skill_lookup("dicing wounds")))
		{
			AFFECT_DATA *dicing_wounds = affect_find(ch->affected, skill_lookup("dicing wounds"));

			act("Blood continues to ooze from your diced flesh.", ch, 0, 0, TO_CHAR);
			act("Blood continues to ooze from $n's diced flesh.", ch, 0, 0, TO_ROOM);
			if ((af_owner = find_char_by_name(dicing_wounds->owner_name)) != NULL)
				damage_newer(af_owner, ch, dicing_wounds->level + number_range(1, 6), TYPE_UNDEFINED, DAM_OTHER, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "diced, bleeding flesh", PLURAL, DAM_POSSESSIVE, dicing_wounds, FALSE);
			else
				damage_newer(ch, ch, dicing_wounds->level + number_range(1, 6), TYPE_UNDEFINED, DAM_OTHER, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "diced, bleeding flesh", PLURAL, DAM_POSSESSIVE, NULL, FALSE);
		}

		if (is_affected(ch, gsn_icelance) && ch != NULL)
		{
			AFFECT_DATA *icelance = affect_find(ch->affected, skill_lookup("icelance wound"));

			if (check_immune(ch, DAM_COLD) != IS_IMMUNE)
			{
				act("$n shivers as the melting icelance mingles with his blood.", ch, NULL, NULL, TO_ROOM);
				send_to_char("A chill runs through your body as the icelance freezes your blood.\n\r", ch);

				if ((af_owner = find_char_by_name(icelance->owner_name)) != NULL)
					damage_newer(af_owner, ch, ch->level * (number_range(3, 5) / 3), TYPE_UNDEFINED, DAM_COLD, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "icelance wound", PLURAL, DAM_POSSESSIVE, icelance, FALSE);
				else
					damage_newer(ch, ch, ch->level * (number_range(3, 5) / 3), TYPE_UNDEFINED, DAM_COLD, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "icelance wound", PLURAL, DAM_POSSESSIVE, NULL, FALSE);
			}
		}

		if (is_affected(ch, gsn_bobbit) && ch != NULL)

		{
			AFFECT_DATA *bobbit = affect_find(ch->affected, skill_lookup("bobbit"));

			act("$n screams in agony as $s wound continues to bleed!", ch, NULL, NULL, TO_ROOM);
			send_to_char("You scream in agony as your wound continues to bleed!\n\r", ch);

			if ((af_owner = find_char_by_name(bobbit->owner_name)) != NULL)
				damage_newer(af_owner, ch, 51, TYPE_UNDEFINED, DAM_OTHER, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "wound", PLURAL, DAM_NOT_POSSESSIVE, bobbit, FALSE);
			else
				damage_newer(ch, ch, 51, TYPE_UNDEFINED, DAM_OTHER, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "wound", PLURAL, DAM_NOT_POSSESSIVE, NULL, FALSE);
		}

		if (is_affected(ch, skill_lookup("vorpal bolt chest")) && ch != NULL)

		{
			AFFECT_DATA *vorpal = affect_find(ch->affected, skill_lookup("vorpal bolt chest"));

			act("The burns on $n's chest pulse painfully.", ch, NULL, NULL, TO_ROOM);
			send_to_char("The electrical burns on your chest pulse painfully.\n\r", ch);

			if ((af_owner = find_char_by_name(vorpal->owner_name)) != NULL)
				damage_newer(af_owner, ch, ch->level + number_range(5, 15), TYPE_UNDEFINED, DAM_FIRE, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "severe burns", SINGULAR, DAM_POSSESSIVE, vorpal, FALSE);
			else
				damage_newer(ch, ch, ch->level + number_range(5, 15), TYPE_UNDEFINED, DAM_FIRE, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "severe burns", SINGULAR, DAM_POSSESSIVE, NULL, FALSE);
		}

		if (is_affected(ch, gsn_impale) && ch != NULL)
		{
			AFFECT_DATA *impale = affect_find(ch->affected, gsn_impale);
			act("$n chokes out blood as $s wound continues to bleed.", ch, NULL, NULL, TO_ROOM);
			send_to_char("You choke blood as your wound continues to bleed.\n\r", ch);
			if ((af_owner = find_char_by_name(impale->owner_name)) != NULL)
				damage_newer(af_owner, ch, ch->level * number_range(1, 3), gsn_impale, DAM_OTHER, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "impalement wound", PLURAL, DAM_POSSESSIVE, impale, FALSE);
			else
				damage_newer(ch, ch, ch->level * number_range(1, 3), gsn_impale, DAM_OTHER, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "impalement wound", PLURAL, DAM_POSSESSIVE, NULL, FALSE);
		}

		if (is_affected(ch, gsn_cross_slice) && ch != NULL)
		{
			AFFECT_DATA *bleed = affect_find(ch->affected, gsn_cross_slice);

			send_to_char("Your gashed stomach continues to bleed.\n\r", ch);

			if ((af_owner = find_char_by_name(bleed->owner_name)) != NULL)
				damage_newer(af_owner, ch, number_range(bleed->level * 2, bleed->level), gsn_bleed, DAM_OTHER, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "bleeding", PLURAL, DAM_POSSESSIVE, bleed, FALSE);
			else
				damage_newer(ch, ch, number_range(bleed->level * 2, bleed->level), gsn_bleed, DAM_OTHER, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "bleeding", PLURAL, DAM_POSSESSIVE, NULL, FALSE);
		}

		if (is_affected(ch, gsn_pathfinding))
		{
			check_improve(ch, gsn_pathfinding, TRUE, 4);
		}

		// Ends bloodthirst and adds fatigue at 0 hours left on thirst. - Ceial

		if (is_affected(ch, gsn_bloodthirst))
		{
			AFFECT_DATA af;
			AFFECT_DATA *paf;
			AFFECT_DATA *paf_next;
			int sn = skill_lookup("fatigue");

			for (paf = ch->affected; paf != NULL; paf = paf_next)
			{
				paf_next = paf->next;
				if ((paf->type == gsn_bloodthirst) && (paf->duration == 0))
				{
					if (!is_affected(ch, sn))
					{
						send_to_char("Your thirst for blood has ended. You feel extremely tired.\n\r", ch);
						affect_strip(ch, gsn_bloodthirst);
						init_affect(&af);
						af.where = TO_AFFECTS;
						af.aftype = AFT_SKILL;
						af.type = sn;
						af.level = 51;
						af.duration = 5;
						af.location = APPLY_DEX;
						af.modifier = -4;
						af.bitvector = AFF_SLOW;
						affect_to_char(ch, &af);
					}
				}
			}
		}

		// rot code
		if (!ch->in_room)
			continue;

		if (is_affected(ch, gsn_rot) && ch)
		{
			AFFECT_DATA *rot = affect_find(ch->affected, gsn_rot);

			if (!saves_spell(URANGE(52, rot->level + 2, 56), ch, DAM_DISEASE))
			{
				rot->modifier--;
				ch->mod_stat[STAT_CON]--;
			}

			if ((af_owner = find_char_by_name(rot->owner_name)) != NULL)
				damage_newer(af_owner, ch, URANGE(1, (-rot->modifier) * 10, 110), gsn_rot, DAM_DISEASE, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "decomposition", PLURAL, DAM_POSSESSIVE, rot, FALSE);
			else
			{
				af_owner = ch;
				damage_newer(ch, ch, URANGE(1, (-rot->modifier) * 10, 110), gsn_rot, DAM_DISEASE, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "decomposition", PLURAL, DAM_POSSESSIVE, NULL, FALSE);
			}

			if (get_curr_stat(ch, STAT_CON) <= 3)
			{
				act("$n's decomposing body gives out on $m.", ch, 0, 0, TO_ROOM);
				send_to_char("Your decomposing body gives out on you.\n\r", ch);
				if ((af_owner = find_char_by_name(rot->owner_name)) != NULL)
					raw_kill(af_owner, ch);
				else
					raw_kill(ch, ch);
				continue;
			}
		}

		if (affect_find(ch->affected, gsn_insect_swarm) != NULL)
		{
			AFFECT_DATA *insect_swarm = affect_find(ch->affected, gsn_insect_swarm);

			act("$n waves his arms trying to swat away insects.", ch, NULL, NULL, TO_ROOM);
			send_to_char("You wave your arms frantically trying to kill all the stinging insects.\n\r", ch);

			if ((af_owner = find_char_by_name(insect_swarm->owner_name)) != NULL)
				damage_newer(af_owner, ch, number_range(25, 70), gsn_insect_swarm, DAM_PIERCE, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "the insect swarm", PLURAL, DAM_NOT_POSSESSIVE, insect_swarm, FALSE);
			else
				damage_newer(ch, ch, number_range(25, 70), gsn_insect_swarm, DAM_PIERCE, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "the insect swarm", PLURAL, DAM_NOT_POSSESSIVE, NULL, FALSE);
		}
		if (is_affected(ch, gsn_self_immolation) && ch != NULL && !IS_SET(ch->off_flags, IMM_FIRE))
		{
			AFFECT_DATA *self_immo = affect_find(ch->affected, gsn_self_immolation);

			act("$n screams in agony as he continues to burn.", ch, NULL, NULL, TO_ROOM);
			send_to_char("You scream in agony as you continue to burn.\n\r", ch);

			if ((af_owner = find_char_by_name(self_immo->owner_name)) != NULL)
				damage_newer(af_owner, ch, number_range(50, 150), gsn_self_immolation, DAM_FIRE, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "self-immolation", PLURAL, DAM_POSSESSIVE, self_immo, FALSE);
			else
				damage_newer(ch, ch, number_range(50, 150), gsn_self_immolation, DAM_FIRE, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "self-immolation", PLURAL, DAM_POSSESSIVE, NULL, FALSE);
		}
		if (is_affected(ch, skill_lookup("hellfire")) && ch != NULL)
		{
			AFFECT_DATA *hellfire = affect_find(ch->affected, skill_lookup("hellfire"));

			act("$n screams in agony as flames consumes $s body!", ch, NULL, NULL, TO_ROOM);
			send_to_char("You scream in agony as the hellfire continues to burn you!\n\r", ch);

			if ((af_owner = find_char_by_name(hellfire->owner_name)) != NULL)
				damage_newer(af_owner, ch, number_range(25, 75), skill_lookup("hellfire"), DAM_FIRE, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "the hellfire", PLURAL, DAM_NOT_POSSESSIVE, hellfire, FALSE);
			else
				damage_newer(ch, ch, number_range(25, 75), skill_lookup("hellfire"), DAM_FIRE, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "the hellfire", PLURAL, DAM_NOT_POSSESSIVE, NULL, FALSE);
		}
		else if (ch->position == POS_INCAP && number_range(0, 1) == 0)
		{
			damage(ch, ch, 5, TYPE_UNDEFINED, DAM_NONE, FALSE);
		}
		else if (ch->position == POS_MORTAL)
		{
			damage(ch, ch, 5, TYPE_UNDEFINED, DAM_NONE, FALSE);
		}

		if (ch != NULL && !IS_NPC(ch) && ch->pcdata->newbie == TRUE)
		{
			if (((ch->played + (current_time - ch->logon)) / 3600) > 2 || ch->cabal != 0 || ch->pcdata->empire >= EMPIRE_BLOOD)
			{
				ch->pcdata->newbie = FALSE;
				send_to_char("You are no longer a newbie.\n\r", ch);
				ch->ghost = 0;
			}
		}

		if (ch->mana < -400)
			ch->mana = -400;
		if (ch->move < -400)
			ch->move = -400;
	}

	/*
	 * Autosave and autoquit.
	 * Check that these chars still exist.
	 */
	for (ch = char_list; ch != NULL; ch = ch_next)
	{
		ch_next = ch->next;

		if (ch->desc != NULL && ch->desc->descriptor % 30 == save_number)
			save_char_obj(ch);

		if (ch == ch_quit)
			do_quit_new(ch, "", TRUE);
	}

	return;
}

/*
 * Update all objs.
 * This function is performance sensitive.
 */
void obj_update(void)
{
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;
	AFFECT_DATA *paf, *paf_next;
	CHAR_DATA *cguard, *owner;

	for (obj = object_list; obj != NULL; obj = obj_next)
	{
		CHAR_DATA *rch;
		char *message;

		obj_next = obj->next;

		/* go through affects and decrement */
		for (paf = obj->affected; paf != NULL; paf = paf_next)
		{
			paf_next = paf->next;
			if (paf->duration > 0)
			{
				paf->duration--;
				if (number_range(0, 4) == 0 && paf->level > 0)
					paf->level--; /* spell strength fades with time */
			}
			else if (paf->duration < 0)
				;
			else
			{
				if (paf_next == NULL || paf_next->type != paf->type || paf_next->duration > 0)
				{
					if (paf->type > 0 && skill_table[paf->type].msg_obj)
					{
						if (obj->carried_by != NULL)
						{
							rch = obj->carried_by;
							act(skill_table[paf->type].msg_obj,
								rch, obj, NULL, TO_CHAR);
						}
						if (obj->in_room != NULL && obj->in_room->people != NULL)
						{
							rch = obj->in_room->people;
							act(skill_table[paf->type].msg_obj,
								rch, obj, NULL, TO_ALL);
						}
					}
				}

				affect_remove_obj(obj, paf);
			}
		}

		if ((obj->pIndexData->vnum == 77) && (obj->carried_by->fighting == NULL))
		{
			obj->timer = 0;
		}

		if (obj->timer <= 0 || --obj->timer > 0)
			continue;

		if (obj->pIndexData->vnum == OBJ_VNUM_THIEF_PLANT_EXPLOSIVE && obj->carried_by != NULL)
		{
			char explosivebuf[MSL];
			sprintf(explosivebuf, "%s suddenly ignites and explodes into a million fragments!\n\r", obj->short_descr);
			send_to_char(explosivebuf, obj->carried_by);
			sprintf(explosivebuf, "%s's %s suddenly ignites and explodes into a million fragments!\n\r", obj->carried_by->name, obj->short_descr);
			act("$n's $p suddenly ignites and explodes into a million fragments!", obj->carried_by, obj, 0, TO_ROOM);
			if (!isNewbie(obj->carried_by) && obj->carried_by->ghost == 0)
			{
				CHAR_DATA *af_owner;
				if (number_percent() <= 20)
				{
					AFFECT_DATA af;
					init_affect(&af);
					af.type = skill_lookup("critical strike");
					af.name = str_dup("fragmentation wounds");
					af.affect_list_msg = str_dup("induces uncontrollable bleeding");
					af.duration = 5;
					af.level = 5;
					af.aftype = AFT_SKILL;
					affect_to_char(obj->carried_by, &af);
					send_to_char("Fragments of the explosive pierce deep into your flesh!\n\r", obj->carried_by);
				}
				if (number_percent() <= 20)
				{
					AFFECT_DATA af;
					init_affect(&af);
					af.type = skill_lookup("deafen");
					af.name = str_dup("explosive deafening");
					af.affect_list_msg = str_dup("prevents casting and induces a deafened state");
					af.duration = 2;
					af.level = 50;
					af.aftype = AFT_SKILL;
					affect_to_char(obj->carried_by, &af);
					send_to_char("Everything goes silent as the loud explosion consumes your hearing.\n\r", obj->carried_by);
				}
				if ((af_owner = find_char_by_name(obj->owner)) != NULL)
					damage_newer(af_owner, obj->carried_by, ((obj->level * (obj->level / 2) + 3) * 19), TYPE_UNDEFINED, DAM_NONE, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "A sudden explosion", PLURAL, DAM_NOT_POSSESSIVE, NULL, TRUE);
				else
					damage_newer(obj->carried_by, obj->carried_by, ((obj->level * (obj->level / 2) + 3) * 19), TYPE_UNDEFINED, DAM_NONE, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "A sudden explosion", PLURAL, DAM_NOT_POSSESSIVE, NULL, FALSE);
			}
			else
				send_to_char("The gods protect you from harm.\n\r", obj->carried_by);
		}
		else if (obj->pIndexData->vnum == OBJ_VNUM_THIEF_PLANT_EXPLOSIVE && obj->in_room != NULL)
		{
			act("$p suddenly ruptures and explodes, but harms no one.", 0, 0, obj, TO_ROOM);
		}

		switch (obj->item_type)
		{
		default:
			message = "$p crumbles into dust.";
			break;
		case ITEM_FOUNTAIN:
			message = "$p dries up.";
			break;
		case ITEM_CORPSE_NPC:
			message = "$p decays into dust.";
			break;
		case ITEM_CORPSE_PC:
			message = "$p decays into dust.";
			break;
		case ITEM_FOOD:
			message = "$p decomposes.";
			break;
		case ITEM_POTION:
			message = "$p has evaporated from disuse.";
			break;
		case ITEM_PORTAL:
			message = "$p fades out of existence.";
			break;
		case ITEM_CONTAINER:
			message = "$p crumbles into dust.";
			break;
		}

		if (isCabalItem(obj) && obj->carried_by != NULL && IS_NPC(obj->carried_by))
		{
			obj->timer = 0;
			continue;
		}
		if (isCabalItem(obj))
		{
			cguard = get_cabal_guardian(obj->pIndexData->cabal);
			if (!cguard)
			{
				obj->timer = 15;
				continue;
			}
			act(message, obj->carried_by, obj, NULL, TO_CHAR);
			obj_from_char(obj);
			obj_to_char(obj, cguard);
			obj->timer = 0;
			continue;
		}

		if (obj->carried_by != NULL)
		{
			if (IS_NPC(obj->carried_by) && obj->carried_by->pIndexData->pShop != NULL)
				obj->carried_by->gold += obj->cost / 5;
			else
			{
				act(message, obj->carried_by, obj, NULL, TO_CHAR);
			}
		}
		else if (obj->in_room != NULL && (rch = obj->in_room->people) != NULL)
		{
			if (!(obj->in_obj && obj->in_obj->pIndexData->vnum == OBJ_VNUM_PIT && !CAN_WEAR(obj->in_obj, ITEM_TAKE)))
			{
				act(message, rch, obj, NULL, TO_ROOM);
				act(message, rch, obj, NULL, TO_CHAR);
			}
		}

		if (obj->item_type == ITEM_CORPSE_NPC && obj->contains)
		{
			OBJ_DATA *t_obj, *next_obj;
			if (obj->contains)
			{
				if (obj->in_room)
				{
					CHAR_DATA *rch;
					for (rch = obj->in_room->people; rch != NULL; rch = rch->next_in_room)
					{
						act("The contents of $p spill onto the ground as it decays.", rch, obj, 0, TO_CHAR);
					}
				}
				else
					act("The contents of $p spill from your hands onto the ground as it decays.", obj->carried_by, obj, 0, TO_CHAR);

				for (t_obj = obj->contains; t_obj != NULL; t_obj = next_obj)
				{
					next_obj = t_obj->next_content;
					obj_from_obj(t_obj);
					if (obj->in_room)
						obj_to_room(t_obj, obj->in_room);
					else
					{
						obj_to_room(t_obj, obj->carried_by->in_room);
					}
				}
			}
		}
		if (obj->item_type == ITEM_CORPSE_PC && obj->contains)
		{ /* objects go to pit */
			OBJ_DATA *t_obj, *next_obj, *pit_obj;

			/* Put items into the pit that the player recalls to */
			if (IS_EXPLORE(obj->in_room))
			{ // Gear to char
				for (owner = char_list; owner != NULL; owner = owner->next)
				{
					if (!IS_NPC(owner) && (!str_cmp(owner->name, obj->owner) || (is_affected(owner, gsn_cloak_form) &&
																				 !str_cmp(owner->original_name, obj->owner))))
					{
						if (obj->contains)
						{
							send_to_char("Your corpse decays.\n\r", owner);
							// It's their corpse.
							for (t_obj = obj->contains; t_obj != NULL; t_obj = next_obj)
							{
								next_obj = t_obj->next_content;
								act_new("$p returns to you.", owner, t_obj, 0, TO_CHAR, POS_DEAD);
								if (t_obj->item_type == ITEM_MONEY)
								{
									owner->gold += t_obj->value[0];
									extract_obj(t_obj);
									continue;
								}
								obj_from_obj(t_obj);
								obj_to_char(t_obj, owner);
							}
						}
						extract_obj(obj);
						break;
					}
				}
				continue;
			}
			if (obj->value[4])
				pit_obj = get_obj_type(get_obj_index(hometown_table[obj->value[4]].pit));
			else
				pit_obj = get_obj_type(get_obj_index(OBJ_VNUM_PIT));

			for (t_obj = obj->contains; t_obj != NULL; t_obj = next_obj)
			{
				next_obj = t_obj->next_content;
				obj_from_obj(t_obj);
				obj_to_obj(t_obj, pit_obj);
			}
		}

		extract_obj(obj);
	}

	return;
}

/* Mobile tracking. I implemented the most stupid, basic mob tracking you
could possibly come up with, but it works ok from a player point of view.
If you want something more sophisticated just hack that into here
instead...(Ceran)
*/
void track_update(void)
{
	CHAR_DATA *tch;
	CHAR_DATA *tch_next;
	char buf[MAX_STRING_LENGTH];

	for (tch = char_list; tch != NULL; tch = tch_next)
	{
		tch_next = tch->next;
		if (!IS_NPC(tch) || (tch->last_fought == NULL))
			continue;
		if (tch->fighting != NULL || (tch->position != POS_STANDING) || (is_affected(tch, gsn_power_word_stun)))
			continue;

		track_char(tch, tch->last_fought, tch->in_room->vnum);

		if (tch->pIndexData->vnum == MOB_VNUM_STALKER && MOB_VNUM_PHANTOM)
		{
			if (tch->in_room->area != tch->last_fought->in_room->area)
			{
				if (number_percent() < 50)
				{
					do_goto(tch, tch->last_fought->name);
					char_from_room(tch);
					char_to_room(tch, tch->last_fought->in_room);
				}
			}
			else
			{
				track_char(tch, tch->last_fought, tch->in_room->vnum);
				track_char(tch, tch->last_fought, tch->in_room->vnum);
				track_char(tch, tch->last_fought, tch->in_room->vnum);
			}
		}

		if (tch->in_room == tch->last_fought->in_room)
		{
			if (can_see(tch, tch->last_fought))
			{
				sprintf(buf, "%s, now you die!", tch->last_fought->name);
				do_myell(tch, buf);
				multi_hit(tch, tch->last_fought, TYPE_UNDEFINED);
			}
		}
	}
	return;
}

/* This is for the riot skill used by outlaws. If you don't plan on using
riot in the mud it would be a good idea to juse remove this function to
speed up the update calls...(Ceran)
*/
void riot_update(void)
{
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *rioter;
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	CHAR_DATA *rioter_next;

	return;
	for (rioter = char_list; rioter != NULL; rioter = rioter_next)
	{
		rioter_next = rioter->next;
		if (!is_affected(rioter, gsn_riot) || rioter->position != POS_STANDING)
			continue;
		for (vch = rioter->in_room->people; vch != NULL; vch = vch_next)
		{
			vch_next = vch->next_in_room;
			if (is_same_group(vch, rioter))
				continue;
			sprintf(buf, "Help! %s is rioting!", (IS_NPC(rioter) ? rioter->short_descr : rioter->name));
			do_myell(vch, buf);
			multi_hit(rioter, vch, TYPE_UNDEFINED);
			break;
		}
	}
	return;
}

/*
 * Aggress.
 *
 * for each mortal PC
 *     for each mob in room
 *         aggress on some random PC
 *
 * This function takes 25% to 35% of ALL Merc cpu time.
 * Unfortunately, checking on each PC move is too tricky,
 *   because we don't the mob to just attack the first PC
 *   who leads the party into the room.
 *
 * -- Furey
 */

void aggr_update(void)
{
	CHAR_DATA *wch;
	CHAR_DATA *wch_next;
	CHAR_DATA *ch;
	CHAR_DATA *ch_next;
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;
	CHAR_DATA *victim;
	char buf[MAX_STRING_LENGTH];

	for (wch = char_list; wch != NULL; wch = wch_next)
	{
		wch_next = wch->next;

		if (IS_AWAKE(wch) &&
			is_affected(wch, gsn_bloodthirst) && wch->fighting == NULL)
		{
			for (vch = wch->in_room->people;
				 vch != NULL && wch->fighting == NULL; vch = vch_next)
			{
				vch_next = vch->next_in_room;
				if (wch != vch && can_see(wch, vch) && ((arena) || (!arena && vch->cabal != CABAL_RAGER)))
				{
					if (IS_NPC(vch))
					{
						if (vch->pIndexData->vnum == 5701 || vch->pIndexData->vnum == 5702 || vch->pIndexData->vnum == 2906 || vch->pIndexData->vnum == 2907 || vch->pIndexData->vnum == 2908 || vch->pIndexData->vnum == 2909)
						{
							continue;
						}
						if (is_same_group(vch, wch))
						{
							continue;
						}
					}
					if ((wch->ghost > 0) || (vch->ghost > 0))
					{
						continue;
					}
					if (IS_IMMORTAL(vch))
						continue;

					sprintf(buf, "{RMORE BLOOD! MORE BLOOD! MOOORE BLOOOOOOD!!!{x\n\r");
					send_to_char(buf, wch);
					do_murder(wch, vch->name);
				}
			}
		}

		if (IS_AWAKE(wch) && check_darkshout(wch) == DARKSHOUT_BAAL && wch->fighting == NULL)
		{
			for (vch = wch->in_room->people; vch != NULL && wch->fighting == NULL; vch = vch_next)
			{
				vch_next = vch->next_in_room;
				if (wch != vch && can_see(wch, vch))
				{
					if ((wch->ghost > 0) || (vch->ghost > 0))
					{
						continue;
					}

					if (IS_NPC(vch))
						continue;

					if (IS_IMMORTAL(vch))
						continue;

					if (!IS_GOOD(vch))
						continue;

					printf_to_char(wch, "The unholy will of Baal reaches out and attacks %s!\n\r", vch->name);
					do_murder(wch, vch->name);
				}
			}
		}

		if (IS_NPC(wch) || wch->level >= LEVEL_IMMORTAL || wch->in_room == NULL || wch->in_room->area->empty)
			continue;

		for (ch = wch->in_room->people; ch != NULL; ch = ch_next)
		{
			int count;

			ch_next = ch->next_in_room;

			if (!IS_NPC(ch) || !IS_SET(ch->act, ACT_AGGRESSIVE) || IS_SET(ch->in_room->room_flags, ROOM_SAFE) || IS_AFFECTED(ch, AFF_CALM) || ch->fighting != NULL || IS_AFFECTED(ch, AFF_CHARM) || !IS_AWAKE(ch) || (IS_SET(ch->act, ACT_WIMPY) && IS_AWAKE(wch)) || !can_see(ch, wch) || number_bits(1) == 0)
				continue;

			/*
			 * Ok we have a 'wch' player character and a 'ch' npc aggressor.
			 * Now make the aggressor fight a RANDOM pc victim in the room,
			 *   giving each 'vch' an equal chance of selection.
			 */

			/* Unless I'm misunderstanding, the logic below doesn't actually define a random
			 * target, it defines a pretty consistent target when number_range(0, 0) hits,
			 * that would be the first person in the 'next_in_room', and THEN we have a chance
			 * to hit someone else - second person has a 50/50 chance to get "selected", and then
			 * third person has a 30% chance to get selected - so...statistically you end up with
			 * a random chance to get selected, but the reality is that it's less and less likely
			 * to be the one hit the further into this you get - since your random range will always
			 * have a larger and larger number to choose from, it will always be less and less likely
			 * to be selected.
			 * 
			 * 1 person: count = 0, 100% selection for first person
			 * 2 person: count = 0, 100% selection, count = 1, 50% selection for person 2, left with 50/50 for two people.
			 * 3 person: count = 0, 100% selection, count = 2, 50% selection for person 2, count = 3, 33% for person 3 to be selected
			 * N person: count = 0, 100%, count = 2, 50% selection P2, 33% selection P3, 25% selection P4, (N/index)% chance for each
			 * which sounds good mathematically, except you don't reduce the selection of the person before by adding a lower % selection
			 * for each subsequent person.  
			 * 
			 * It looks like this was a workaround to the lack of index counting for playable characters in the room?  Maybe I'm just
			 * folling myself on how those probabilities play out, but it feels like 50% chance you've been selected for #2 and 33% chance
			 * next guy is picked doesn't make it random, it makes it more and more likely it's within the last x people who entered the room
			 * past the first ones.
			 */
			count = 0;
			victim = NULL;
			for (vch = wch->in_room->people; vch != NULL; vch = vch_next)
			{
				vch_next = vch->next_in_room;

				if (
					!IS_NPC(vch) && /* victim is not an npc*/
					vch->level < LEVEL_IMMORTAL && /* victim is not an immortal*/
					ch->level >= vch->level - 5 && /* If the aggro mob (ch) is 5 levels below or better than target */
					(!IS_SET(ch->act, ACT_WIMPY) || !IS_AWAKE(vch)) &&  /* If the mob isn't wimpy OR the victim is asleep*/
					can_see(ch, vch) /* If the mob can see the victim*/
				)
				{
					if (number_range(0, count) == 0)
						victim = vch;
					count++;
				}
			}

			if (victim == NULL)
				continue;

			check_chargeset(ch, victim);
			multi_hit(ch, victim, TYPE_UNDEFINED);
		}
	}

	return;
}

int get_age(CHAR_DATA *ch) /* returns age in years. */
{
	return get_age_new((ch->played + (int)(current_time - ch->logon)), ch->race);
}

int get_age_new(int age, int racenumber)
{

	char *race;

	race = pc_race_table[racenumber].name;

	/* convert to hours */
	age /= 3600;

	if (!str_cmp(race, "human"))
	{
		age /= 7;
		age += 18;
	}
	else if (!str_cmp(race, "elf") || !str_cmp(race, "dark-elf"))
	{
		age /= 1.25;
		age += 70;
	}
	else if (!str_cmp(race, "dwarf") || !str_cmp(race, "duergar"))
	{
		age /= 1.25;
		age += 40;
	}
	else if (!str_cmp(race, "cloud") || !str_cmp(race, "fire") || !str_cmp(race, "storm"))
	{
		age /= 4.75;
		age += 75;
	}
	else if (!str_cmp(race, "draconian"))
	{
		age /= 5.75;
		age += 20;
	}
	else if (!str_cmp(race, "ethereal"))
	{
		age /= 7;
		age += 20;
	}
	else if (!str_cmp(race, "troll"))
	{
		age /= 7;
		age += 18;
	}
	else if (!str_cmp(race, "lich"))
	{
		age /= 7;
		age += 18;
	}
	else
	{
		age /= 7;
		age += 18;
	}

	return age;
}

int get_death_age(CHAR_DATA *ch)
{
	if (!IS_NPC(ch))
	{
		return get_death_age_new(ch->race, ch->perm_stat[STAT_CON], ch->pcdata->age_mod);
	}
	else
	{
		return 1000;
	}
}

int get_death_age_new(int racenumber, int con, int age_mod)
{
	int age;
	char *race;

	race = pc_race_table[racenumber].name;
	age = 1000;

	if (!str_cmp(race, "human"))
	{
		age = (70 + con);
	}
	else if (!str_cmp(race, "elf") || !str_cmp(race, "dark-elf"))
	{
		age = (320 + 20 * con);
	}
	else if (!str_cmp(race, "dwarf") || !str_cmp(race, "duergar"))
	{
		age = (270 + 10 * con);
	}
	else if (!str_cmp(race, "cloud") || !str_cmp(race, "fire") || !str_cmp(race, "storm"))
	{
		age = (175 + 6 * con);
	}
	else if (!str_cmp(race, "draconian"))
	{
		age = (80 + 2 * con);
	}
	else if (!str_cmp(race, "ethereal"))
	{
		age = (80 + con);
	}
	else if (!str_cmp(race, "troll"))
	{
		age = (65 + con);
	}
	else if (!str_cmp(race, "lich"))
	{
		age = (1000 + 25 * con);
	}
	else
	{
		age = (60 + 3 * con);
	}

	age -= age_mod;

	return age;
}

int reverse_d(int dir)
{
	if (dir == DIR_NORTH)
		return DIR_SOUTH;
	if (dir == DIR_SOUTH)
		return DIR_NORTH;
	if (dir == DIR_WEST)
		return DIR_EAST;
	if (dir == DIR_EAST)
		return DIR_WEST;
	if (dir == DIR_UP)
		return DIR_DOWN;
	if (dir == DIR_DOWN)
		return DIR_UP;
	return 0;
}

/* returns the age name type */
char *get_age_name_new(int age, int racenumber)
{
	char *name;

	age /= 3600;

	/*
		0 - 20:	young
		21 - 120:	mature
		121 - 220:	middle aged
		221 - 300:	old
		301 - 350:	very old
		351 - 1000:	ancient
		1001 -  ~ :	worm-food
	*/

	if (age <= 20)
	{
		name = "young";
	}
	else if (age <= 120)
	{
		name = "mature";
	}
	else if (age <= 220)
	{
		name = "middle aged";
	}
	else if (age <= 300)
	{
		name = "old";
	}
	else if (age <= 350)
	{
		name = "very old";
	}
	else if (age <= 1000)
	{
		name = "ancient";
	}
	else
	{
		name = "worm-food";
	}

	return name;
}

char *get_age_name(CHAR_DATA *ch)
{
	if (IS_NPC(ch))
		return "young";

	return get_age_name_new((ch->played + (int)(current_time - ch->logon)), ch->race);
}

void age_update(void)
{
	return;
}

/*
 * Handle all kinds of updates.
 * Called once per pulse from game loop.
 * Random times to defeat tick-timing clients and players.
 */

void update_handler(void)
{
	static int pulse_area;
	static int pulse_mobile;
	static int pulse_violence;
	static int pulse_point;
	static int pulse_track;
	static int pulse_raffect;
	static int pulse_iprog_pulse;

	int pepe;
	if (--pulse_iprog_pulse <= 0)
	{
		pepe = pulse_point;
		pulse_iprog_pulse = PULSE_IPROG_PULSE;
		if (--pepe <= 0)
			iprog_pulse_update(TRUE);
		else
			iprog_pulse_update(FALSE);
	}

	if (--pulse_area <= 0)
	{
		pulse_area = PULSE_AREA;
		area_update();
		room_update();
	}

	if (--pulse_track <= 0)
	{
		pulse_track = PULSE_TRACK;
		track_update();
	}

	if (--pulse_raffect <= 0)
	{
		pulse_raffect = PULSE_RAFFECT;
		room_affect_update();
	}

	if (--pulse_mobile <= 0)
	{
		pulse_mobile = PULSE_MOBILE;
		mobile_update();
	}

	if (--pulse_violence <= 0)
	{
		pulse_violence = PULSE_VIOLENCE;
		prelag_skills();
		violence_update();
	}

	if (--pulse_point <= 0)
	{
		wiznet("TICK!", NULL, NULL, WIZ_TICKS, 0, 0);
		pulse_point = PULSE_TICK;
		weather_update();
		char_update();
		obj_update();
		age_update();
	}

	aggr_update();
	tail_chain();
	return;
}

void do_forcetick(CHAR_DATA *ch, char *argument)
{
	wiznet("TICK!", NULL, NULL, WIZ_TICKS, 0, 0);
	weather_update();
	char_update();
	obj_update();
	age_update();
	room_update();
	return;
}

void room_update(void)
{
	ROOM_INDEX_DATA *room, *room_next;

	for (room = top_affected_room; room; room = room_next)
	{
		ROOM_AFFECT_DATA *paf, *paf_next;

		room_next = room->aff_next;

		for (paf = room->affected; paf != NULL; paf = paf_next)
		{
			paf_next = paf->next;
			if (paf->duration > 0)
			{
				if (paf->tick_fun)
					(*paf->tick_fun)(room, paf);
				paf->duration--;
			}
			else if (paf->duration < 0)
			{
				//			bug("Room_update: paf->duration is negative!", 0);
				char bugbuf[MSL];
				sprintf(bugbuf, "Room_update: raf duration is negative: %s (%d)", paf->name, paf->type);
				bug(bugbuf, 0);
			}
			else
			{
				affect_remove_room(room, paf);
			}
		}
	}
	return;
}

void room_affect_update(void)
{
	return;
}

void iprog_pulse_update(bool isTick)
{
	OBJ_DATA *obj;
	OBJ_DATA *obj_next, *t_obj;

	for (obj = object_list; obj != NULL; obj = obj_next)
	{
		obj_next = obj->next;

		for (t_obj = obj; t_obj->in_obj; t_obj = t_obj->in_obj)
			;
		if (IS_SET(obj->progtypes, IPROG_PULSE))
			if ((t_obj->in_room != NULL && (t_obj->in_room->area->nplayer > 0)) || (t_obj->carried_by && t_obj->carried_by->in_room && t_obj->carried_by->in_room->area->nplayer > 0))
				(obj->pIndexData->iprogs->pulse_prog)(obj, isTick);
	}
}

void healing_sleep_wakeup(CHAR_DATA *ch)
{
	if (ch->position != POS_SLEEPING)
		return;

	if (is_affected(ch, skill_lookup("blindness")))
	{
		send_to_char(skill_table[skill_lookup("blindness")].msg_off, ch);
		affect_strip(ch, skill_lookup("blindness"));
	}

	if (is_affected(ch, skill_lookup("hex")))
	{
		send_to_char(skill_table[skill_lookup("hex")].msg_off, ch);
		affect_strip(ch, skill_lookup("hex"));
	}

	if (is_affected(ch, skill_lookup("rot")))
	{
		send_to_char(skill_table[skill_lookup("rot")].msg_off, ch);
		affect_strip(ch, skill_lookup("rot"));
	}

	if (is_affected(ch, skill_lookup("poison")))
	{
		send_to_char(skill_table[skill_lookup("poison")].msg_off, ch);
		affect_strip(ch, skill_lookup("poison"));
	}

	if (is_affected(ch, skill_lookup("plague")))
	{
		send_to_char(skill_table[skill_lookup("plague")].msg_off, ch);
		affect_strip(ch, skill_lookup("plague"));
	}

	if (is_affected(ch, skill_lookup("deafen")))
	{
		send_to_char(skill_table[skill_lookup("deafen")].msg_off, ch);
		affect_strip(ch, skill_lookup("deafen"));
	}

	if (is_affected(ch, skill_lookup("curse")))
	{
		send_to_char(skill_table[skill_lookup("curse")].msg_off, ch);
		affect_strip(ch, skill_lookup("curse"));
	}

	if (is_affected(ch, skill_lookup("wither")))
	{
		send_to_char(skill_table[skill_lookup("wither")].msg_off, ch);
		affect_strip(ch, skill_lookup("wither"));
	}

	if (is_affected(ch, skill_lookup("atrophy")))
	{
		send_to_char(skill_table[skill_lookup("atrophy")].msg_off, ch);
		affect_strip(ch, skill_lookup("atrophy"));
	}

	if (is_affected(ch, skill_lookup("weaken")))
	{
		send_to_char(skill_table[skill_lookup("weaken")].msg_off, ch);
		affect_strip(ch, skill_lookup("weaken"));
	}

	if (is_affected(ch, skill_lookup("impale")))
	{
		send_to_char(skill_table[skill_lookup("impale")].msg_off, ch);
		affect_strip(ch, skill_lookup("impale"));
	}

	if (is_affected(ch, skill_lookup("boneshatter")))
	{
		send_to_char(skill_table[skill_lookup("boneshatter")].msg_off, ch);
		affect_strip(ch, skill_lookup("boneshatter"));
	}

	if (is_affected(ch, skill_lookup("gouge")))
	{
		send_to_char(skill_table[skill_lookup("gouge")].msg_off, ch);
		affect_strip(ch, skill_lookup("gouge"));
	}

	if (is_affected(ch, skill_lookup("hellfire")))
		send_to_char(skill_table[skill_lookup("hellfire")].msg_off, ch);
	affect_strip(ch, skill_lookup("hellfire"));

	if (is_affected(ch, skill_lookup("confuse")))
	{
		send_to_char(skill_table[skill_lookup("confuse")].msg_off, ch);
		affect_strip(ch, skill_lookup("confuse"));
	}

	if (is_affected(ch, skill_lookup("crossslice")))
	{
		send_to_char(skill_table[skill_lookup("crossslice")].msg_off, ch);
		affect_strip(ch, skill_lookup("crossslice"));
	}

	ch->hit = ch->max_hit;
	ch->mana = ch->max_mana;
	ch->move = ch->max_move;
	ch->position = POS_STANDING;
	update_pos(ch);

	return;
}

void do_mob_spec(CHAR_DATA *ch)
{
	OBJ_DATA *wield = get_eq_char(ch, WEAR_WIELD);
	sh_int gn = 0, cmd = 0;
	sh_int rng;

	if (!IS_NPC(ch) || !ch->fighting)
		return;

	if (IS_SET(ch->extended_flags, OFF_SWORD_SPEC))
		gn = GROUP_SWORD;
	if (IS_SET(ch->extended_flags, OFF_AXE_SPEC))
		gn = GROUP_AXE;
	if (IS_SET(ch->extended_flags, OFF_HTH_SPEC))
		gn = GROUP_HTH;
	if (IS_SET(ch->extended_flags, OFF_DAGGER_SPEC))
		gn = GROUP_DAGGER;
	if (IS_SET(ch->extended_flags, OFF_SPEAR_SPEC))
		gn = GROUP_SPEAR;
	if (IS_SET(ch->extended_flags, OFF_MACE_SPEC))
		gn = GROUP_MACE;
	if (IS_SET(ch->extended_flags, OFF_POLEARM_SPEC))
		gn = GROUP_POLEARM;
	if (IS_SET(ch->extended_flags, OFF_WHIP_SPEC))
		gn = GROUP_WHIP;
	if (!gn)
	{
		log_string("gn not found");
		return;
	}
	if (!wield && gn != GROUP_HTH)
		return;
	for (;;)
	{
		rng = number_range(0, 4);
		if (str_cmp(group_table[gn].spells[rng], ""))
			break;
	}
	for (cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++)
	{
		if (!str_cmp(group_table[gn].spells[rng], cmd_table[cmd].name))
			break;
	}

	if (cmd_table[cmd].name[0] != '\0')
	{
		(*cmd_table[cmd].do_fun)(ch, "");
	}
}

void updateRaceAff(CHAR_DATA *ch)
{
	// Detect Invis
	if (ch->race == race_lookup("svirfnebli"))
		SET_BIT(ch->affected_by, AFF_DETECT_INVIS);

	// Flight
	if (ch->race == race_lookup("cloud") || ch->race == race_lookup("draconian") || ch->race == race_lookup("ethereal") || ch->race == race_lookup("arial") || ch->race == race_lookup("illithid"))
	{
		if (!is_affected(ch, gsn_earthbind))
			SET_BIT(ch->affected_by, AFF_FLYING);
	}

	// Pass Door
	if (ch->race == race_lookup("ethereal"))
		SET_BIT(ch->affected_by, AFF_PASS_DOOR);

	// Waterbreathing
	if (ch->race == race_lookup("storm"))
		SET_BIT(ch->affected_by, AFF_WATERBREATHING);

	return;
}

void resurrect(CHAR_DATA *ch, AFFECT_DATA *paf)
{
	OBJ_DATA *corpse = NULL, *obj, *content, *next_content;
	CHAR_DATA *victim;
	AFFECT_DATA *af, *af_next;
	char buf[MSL];

	if (ch->position < skill_table[paf->type].minimum_position)
		return send_to_char("Your resurrection has been interrupted!\n\r", ch);
	if (is_affected(ch, skill_lookup("power word stun")) || is_affected(ch, skill_lookup("timestop")) || is_affected(ch, skill_lookup("hold person")))
		return send_to_char("You are in no position to complete the resurrection ritual!\n\r", ch);

	// Find owner of corpse
	for (obj = ch->in_room->contents; obj != NULL; obj = obj->next_content)
	{
		if (obj->pIndexData->vnum == OBJ_VNUM_CORPSE_PC)
		{
			if (!str_cmp(obj->owner, paf->owner_name))
			{
				corpse = obj;
				break;
			}
		}
	}

	if (!corpse)
		return send_to_char("The corpse is no longer here. You cannot resurrect it.\n\r", ch);
	if ((victim = find_char_by_name(corpse->owner)) == NULL)
		return send_to_char("They no longer seem to exist.\n\r", ch);
	if (victim->ghost == 0)
		return send_to_char("They are already living, it seems.\n\r", ch);

	// Transfer player back
	if (victim->fighting != NULL)
		stop_fighting(victim, TRUE);

	// Pretty things.
	act("The ghost of $n slowly fades away into nothing.", victim, NULL, NULL, TO_ROOM);
	act("{WYou see the world fade around you. You are left standing alone in a black void.{x", victim, 0, 0, TO_CHAR);
	act("{WYou feel sudden sharp pains as the void around you grows brighter and more familiar.{x", victim, 0, 0, TO_CHAR);
	act("{WAs it blurs into view, you float above a familiar scene... your death. Your body, on the ground, calls longingly for your soul to return.{x", victim, 0, 0, TO_CHAR);
	act("{WSuddenly, you are violently pulled into your lifeless corpse. Pain fills every orifice of your existence as your body and soul are rejoined.{x", victim, 0, 0, TO_CHAR);
	act("{RIn a final burst of pain, you open your eyes and gasp deeply for air. You have been resurrected by $N!{x", victim, 0, ch, TO_CHAR);
	act("{YYou throw your arms to the sky and plead for divine assistance!{x", ch, NULL, NULL, TO_CHAR);
	act("{Y$n throws $s arms to the sky and pleads for divine assistance!{x", ch, NULL, NULL, TO_ROOM);
	sprintf(buf, "Gods, I beseech you, hear my plea! Return to us the life of %s!", victim->original_name);
	do_yell(ch, buf);
	act("{WColor returns to the corpse of $n as $e returns to life!{x", victim, NULL, NULL, TO_ROOM);
	char_from_room(victim);
	char_to_room(victim, ch->in_room);
	victim->ghost = 0;

	// Remove all affects acquired after death
	for (af = victim->affected; af != NULL; af = af_next)
	{
		af_next = af->next;
		if (!af->strippable)
			continue;
		affect_remove(victim, af);
	}

	// Return corpse contents
	for (content = corpse->contains; content != NULL; content = next_content)
	{
		next_content = content->next_content;
		obj_from_obj(content);
		obj_to_char(content, victim);
	}

	// Cripple em a little bit
	victim->hit = victim->max_hit / 4;
	victim->mana = victim->max_mana / 4;
	victim->move = victim->max_move / 4;

	// Remove corpse
	extract_obj(corpse);
	do_look(victim, "auto");
}
