/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc	   *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.						   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
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

/* command procedures needed */
DECLARE_DO_FUN(do_groups);
DECLARE_DO_FUN(do_help);
DECLARE_DO_FUN(do_say);

int is_spec_skill args((int sn));
int is_mob_spec args((int sn));
bool is_specced args((CHAR_DATA * ch, int spec));

int weapon_num_lookup args((const char *name));
char *weapon_name_lookup args((int type));

bool checkScrollsAcumen args((CHAR_DATA * ch));

/* used to get new skills */
void do_gain(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *trainer;
	if (IS_NPC(ch))
		return;

	/* find a trainer */
	for (trainer = ch->in_room->people;
		 trainer != NULL;
		 trainer = trainer->next_in_room)
		if (IS_NPC(trainer) && (IS_SET(trainer->act, ACT_GAIN) || IS_SET(trainer->act, ACT_TRAIN)))
			break;

	if (trainer == NULL || !can_see(ch, trainer))
	{
		send_to_char("You can't do that here.\n\r", ch);
		return;
	}

	one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		do_say(trainer, "Use gain convert to convert 10 practices into 1 train.");
		do_say(trainer, "Or use gain revert to revert 1 train into 10 practices.");
		return;
	}

	if (!str_prefix(arg, "convert"))
	{
		if (ch->practice < 10)
		{
			act("$N tells you 'You are not yet ready.'",
				ch, NULL, trainer, TO_CHAR);
			return;
		}

		act("$N helps you apply your practice to training",
			ch, NULL, trainer, TO_CHAR);
		ch->practice -= 10;
		ch->train += 1;
		return;
	}

	/* 'gain revert' converts one train into 10 pracs */
	if (!str_prefix(arg, "revert"))
	{
		if (ch->train < 1)
		{
			act("$N tells you 'You do not have the training to apply to your skill practices yet.'",
				ch, NULL, trainer, TO_CHAR);
			return;
		}

		act("$N helps you apply your training to skills practices.",
			ch, NULL, trainer, TO_CHAR);
		ch->practice += 10;
		ch->train -= 1;
		return;
	}

	act("$N tells you 'I do not understand...'", ch, NULL, trainer, TO_CHAR);
	return;
}

/* RT spells and skills show the players spells (or skills) */

void do_spells(CHAR_DATA *ch, char *argument)
{
	BUFFER *buffer;
	char arg[MAX_INPUT_LENGTH];
	char spell_list[LEVEL_HERO + 1][MAX_STRING_LENGTH];
	char spell_columns[LEVEL_HERO + 1];
	int sn, level, min_lev = 1, max_lev = LEVEL_HERO, mana;
	bool fAll = FALSE, found = FALSE;
	char buf[MAX_STRING_LENGTH];

	if (IS_NPC(ch))
		return;
	if (class_table[ch->class].ctype != CLASS_CASTER)
	{
		send_to_char("Your class knows no spells.\n\r", ch);
		return;
	}
	if (argument[0] != '\0')
	{
		fAll = TRUE;

		if (str_prefix(argument, "all"))
		{
			argument = one_argument(argument, arg);
			if (!is_number(arg))
			{
				send_to_char("Arguments must be numerical or all.\n\r", ch);
				return;
			}
			max_lev = atoi(arg);

			if (max_lev < 1 || max_lev > LEVEL_HERO)
			{
				sprintf(buf, "Levels must be between 1 and %d.\n\r", LEVEL_HERO);
				send_to_char(buf, ch);
				return;
			}

			if (argument[0] != '\0')
			{
				argument = one_argument(argument, arg);
				if (!is_number(arg))
				{
					send_to_char("Arguments must be numerical or all.\n\r", ch);
					return;
				}
				min_lev = max_lev;
				max_lev = atoi(arg);

				if (max_lev < 1 || max_lev > LEVEL_HERO)
				{
					sprintf(buf,
							"Levels must be between 1 and %d.\n\r", LEVEL_HERO);
					send_to_char(buf, ch);
					return;
				}

				if (min_lev > max_lev)
				{
					send_to_char("That would be silly.\n\r", ch);
					return;
				}
			}
		}
	}

	/* initialize data */
	for (level = 0; level < LEVEL_HERO + 1; level++)
	{
		spell_columns[level] = 0;
		spell_list[level][0] = '\0';
	}

	for (sn = 0; sn < MAX_SKILL; sn++)
	{
		if (skill_table[sn].name == NULL)
			break;

		if ((level = skill_table[sn].skill_level[ch->class]) < LEVEL_HERO + 1 && level >= min_lev && level <= max_lev && skill_table[sn].spell_fun != spell_null && ch->pcdata->learned[sn] > 0 && (skill_table[sn].ctype == CMD_SPELL || skill_table[sn].ctype == CMD_BOTH))
		{
			found = TRUE;
			level = skill_table[sn].skill_level[ch->class];
			if (ch->level < level)
				sprintf(buf, "%-18s n/a      ", skill_table[sn].name);
			else
			{
				mana = UMAX(skill_table[sn].min_mana,
							100 / (2 + ch->level - level));

				switch (get_curr_stat(ch, STAT_INT))
				{
				case 25:
					mana = 43 * mana / 50;
					break;
				case 24:
					mana = 22 * mana / 25;
					break;
				case 23:
					mana = 45 * mana / 50;
					break;
				case 22:
					mana = 23 * mana / 25;
					break;
				case 21:
					mana = 47 * mana / 50;
					break;
				case 20:
					mana = 24 * mana / 25;
					break;
				case 19:
					mana = 49 * mana / 50;
					break;
				}
				sprintf(buf, "%-18s  %3d mana  ", skill_table[sn].name, mana);
			}

			if (spell_list[level][0] == '\0')
				sprintf(spell_list[level], "\n\rLevel %2d: %s", level, buf);
			else /* append */
			{
				if (++spell_columns[level] % 2 == 0)
					strcat(spell_list[level], "\n\r          ");
				strcat(spell_list[level], buf);
			}
		}
	}

	/* return results */

	if (!found)
	{
		send_to_char("No spells found.\n\r", ch);
		return;
	}

	buffer = new_buf();
	for (level = 0; level < LEVEL_HERO + 1; level++)
		if (spell_list[level][0] != '\0')
			add_buf(buffer, spell_list[level]);
	add_buf(buffer, "\n\r");
	page_to_char(buf_string(buffer), ch);
	free_buf(buffer);
}

void do_skills(CHAR_DATA *ch, char *argument)
{
	BUFFER *buffer;
	char arg[MAX_INPUT_LENGTH];
	char skill_list[LEVEL_HERO + 1][MAX_STRING_LENGTH];
	char skill_columns[LEVEL_HERO + 1];
	int sn, level, min_lev = 1, max_lev = LEVEL_HERO;
	bool fAll = FALSE, found = FALSE;
	char buf[MAX_STRING_LENGTH];
	char skill_filter[MAX_INPUT_LENGTH];
	bool fFilter = FALSE;

	if (IS_NPC(ch))
		return;

	skill_filter[0] = '\0';

	if (argument[0] != '\0')
	{
		fAll = TRUE;

		if (str_prefix(argument, "all"))
		{
			argument = one_argument(argument, arg);
			if (!is_number(arg))
			{
				/* Not a number, treat as skill name filter */
				strcpy(skill_filter, arg);
				fFilter = TRUE;
			}
			else
			{
				max_lev = atoi(arg);

				if (max_lev < 1 || max_lev > LEVEL_HERO)
				{
					sprintf(buf, "Levels must be between 1 and %d.\n\r", LEVEL_HERO);
					send_to_char(buf, ch);
					return;
				}

				if (argument[0] != '\0')
				{
					argument = one_argument(argument, arg);
					if (!is_number(arg))
					{
						send_to_char("Arguments must be numerical or all.\n\r", ch);
						return;
					}
					min_lev = max_lev;
					max_lev = atoi(arg);

					if (max_lev < 1 || max_lev > LEVEL_HERO)
					{
						sprintf(buf,
								"Levels must be between 1 and %d.\n\r", LEVEL_HERO);
						send_to_char(buf, ch);
						return;
					}

					if (min_lev > max_lev)
					{
						send_to_char("That would be silly.\n\r", ch);
						return;
					}
				}
			}
		}
	}

	/* initialize data */
	for (level = 0; level < LEVEL_HERO + 1; level++)
	{
		skill_columns[level] = 0;
		skill_list[level][0] = '\0';
	}

	for (sn = 0; sn < MAX_SKILL; sn++)
	{
		if (skill_table[sn].name == NULL)
			break;

		/*if ((level = skill_table[sn].skill_level[ch->class]) < LEVEL_HERO + 1
	&&  (fAll || level <= ch->level)
	&&  level >= min_lev && level <= max_lev
	&&  skill_table[sn].spell_fun == spell_null
		&&  ch->pcdata->learned[sn] > 0)*/
		if ((level = skill_table[sn].skill_level[ch->class]) < LEVEL_HERO + 1 && level >= min_lev && level <= max_lev && skill_table[sn].spell_fun == spell_null && ch->pcdata->learned[sn] > 0)
		{
			/* If filtering by skill name, check if the filter matches */
			if (fFilter && str_infix(skill_filter, skill_table[sn].name))
				continue;

			found = TRUE;
			level = skill_table[sn].skill_level[ch->class];
			if (ch->level < level)
				sprintf(buf, "%-18s n/a      ", skill_table[sn].name);
			else
				sprintf(buf, "%-18s %3d%%      ", skill_table[sn].name,
						ch->pcdata->learned[sn]);

			if (skill_list[level][0] == '\0')
				sprintf(skill_list[level], "\n\rLevel %2d: %s", level, buf);
			else /* append */
			{
				if (++skill_columns[level] % 2 == 0)
					strcat(skill_list[level], "\n\r          ");
				strcat(skill_list[level], buf);
			}
		}
	}

	/* return results */

	if (!found)
	{
		if (fFilter)
		{
			sprintf(buf, "You don't know any skill matching '%s'.\n\r", skill_filter);
			send_to_char(buf, ch);
		}
		else
			send_to_char("No skills found.\n\r", ch);
		return;
	}

	buffer = new_buf();
	for (level = 0; level < LEVEL_HERO + 1; level++)
		if (skill_list[level][0] != '\0')
			add_buf(buffer, skill_list[level]);
	add_buf(buffer, "\n\r");
	page_to_char(buf_string(buffer), ch);
	free_buf(buffer);
}

/* shows skills, groups and costs (only if not bought) */
void list_group_costs(CHAR_DATA *ch)
{
	return;
}

void list_group_chosen(CHAR_DATA *ch)
{
	if (IS_NPC(ch))
		return;
	return;
}

int exp_per_level(CHAR_DATA *ch)
{
	int epl;

	if (IS_NPC(ch))
		return 1500;

	epl = pc_race_table[ch->race].xpadd + class_table[ch->class].xpadd + 1500;
	epl += (ch->level - 1) * epl * .08;
	return epl;
}

/* this procedure handles the input parsing for the skill generator */
bool parse_gen_groups(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	char buf[100];
	int gn, sn, i;

	if (argument[0] == '\0')
		return FALSE;

	argument = one_argument(argument, arg);

	if (!str_prefix(arg, "help"))
	{
		if (argument[0] == '\0')
		{
			do_help(ch, "group help");
			return TRUE;
		}

		do_help(ch, argument);
		return TRUE;
	}

	if (!str_prefix(arg, "add"))
	{
		if (argument[0] == '\0')
		{
			send_to_char("You must provide a skill name.\n\r", ch);
			return TRUE;
		}

		gn = group_lookup(argument);
		if (gn != -1)
		{
			if (ch->gen_data->group_chosen[gn] || ch->pcdata->group_known[gn])
			{
				send_to_char("You already know that group!\n\r", ch);
				return TRUE;
			}

			if (group_table[gn].rating[ch->class] < 1)
			{
				send_to_char("That group is not available.\n\r", ch);
				return TRUE;
			}

			sprintf(buf, "%s group added\n\r", group_table[gn].name);
			send_to_char(buf, ch);
			ch->gen_data->group_chosen[gn] = TRUE;
			ch->gen_data->points_chosen += group_table[gn].rating[ch->class];
			gn_add(ch, gn);
			return TRUE;
		}

		sn = skill_lookup(argument);
		if (sn != -1)
		{
			if (ch->gen_data->skill_chosen[sn] || ch->pcdata->learned[sn] > 0)
			{
				send_to_char("You already know that skill!\n\r", ch);
				return TRUE;
			}

			if (skill_table[sn].rating[ch->class] < 1 || skill_table[sn].spell_fun != spell_null)
			{
				send_to_char("That skill is not available.\n\r", ch);
				return TRUE;
			}
			sprintf(buf, "%s skill added\n\r", skill_table[sn].name);
			send_to_char(buf, ch);
			ch->gen_data->skill_chosen[sn] = TRUE;
			ch->gen_data->points_chosen += skill_table[sn].rating[ch->class];
			ch->pcdata->learned[sn] = 1;
			return TRUE;
		}

		send_to_char("No skills or groups by that name...\n\r", ch);
		return TRUE;
	}

	if (!strcmp(arg, "drop"))
	{
		if (argument[0] == '\0')
		{
			send_to_char("You must provide a skill to drop.\n\r", ch);
			return TRUE;
		}

		gn = group_lookup(argument);
		if (gn != -1 && ch->gen_data->group_chosen[gn])
		{
			send_to_char("Group dropped.\n\r", ch);
			ch->gen_data->group_chosen[gn] = FALSE;
			ch->gen_data->points_chosen -= group_table[gn].rating[ch->class];
			gn_remove(ch, gn);
			for (i = 0; i < MAX_GROUP; i++)
			{
				if (ch->gen_data->group_chosen[gn])
					gn_add(ch, gn);
			}
			return TRUE;
		}

		sn = skill_lookup(argument);
		if (sn != -1 && ch->gen_data->skill_chosen[sn])
		{
			send_to_char("Skill dropped.\n\r", ch);
			ch->gen_data->skill_chosen[sn] = FALSE;
			ch->gen_data->points_chosen -= skill_table[sn].rating[ch->class];
			ch->pcdata->learned[sn] = 0;
			return TRUE;
		}

		send_to_char("You haven't bought any such skill or group.\n\r", ch);
		return TRUE;
	}

	if (!str_prefix(arg, "premise"))
	{
		do_help(ch, "premise");
		return TRUE;
	}

	if (!str_prefix(arg, "list"))
	{
		list_group_costs(ch);
		return TRUE;
	}

	if (!str_prefix(arg, "learned"))
	{
		list_group_chosen(ch);
		return TRUE;
	}

	if (!str_prefix(arg, "info"))
	{
		do_groups(ch, argument);
		return TRUE;
	}

	return FALSE;
}

/* shows all groups, or the sub-members of a group */
void do_groups(CHAR_DATA *ch, char *argument)
{
	if (IS_NPC(ch))
		return;
	return;
}

/* checks for skill improvement */
void check_improve(CHAR_DATA *ch, int sn, bool success, int multiplier)
{
	int chance;
	char buf[MSL];

	if (IS_NPC(ch))
		return;

	if (ch->level < skill_table[sn].skill_level[ch->class] || skill_table[sn].rating[ch->class] == 0 || ch->pcdata->learned[sn] == 0 || ch->pcdata->learned[sn] == 100)
		return;

	/* check to see if the character has a chance to learn */
	chance = 45 * int_app[get_curr_stat(ch, STAT_INT)].learn;

	chance /= (multiplier * skill_table[sn].rating[ch->class] * 4);

	if (ch->pcdata->newbie == TRUE && (sn == gsn_assassinate || sn == gsn_impale || sn == gsn_cleave || sn == skill_lookup("power word kill")))
	{
		return;
	}

	if (IS_SET(ch->act, PLR_MORON))
		chance /= 6;

	if (checkScrollsAcumen(ch))
		chance *= 6;

	if (number_range(1, 500) > chance)
		return;

	/* now that the character has a CHANCE to learn, see if they really have */

	if (success)
	{
		chance = URANGE(30, 100 - ch->pcdata->learned[sn], 95);
		if (number_percent() < chance)
		{
			ch->pcdata->learned[sn]++;
			if (get_skill(ch, sn) == 100)
				sprintf(buf, "{GYou have perfected %s!{x\n\r", skill_table[sn].name);
			else
				sprintf(buf, "{YYou have become better at %s!{x\n\r", skill_table[sn].name);
			send_to_char(buf, ch);
		}
	}
	else
	{
		chance = URANGE(25, ch->pcdata->learned[sn] / 2, 60);
		if (number_percent() < chance)
		{
			ch->pcdata->learned[sn] += number_range(1, 3);
			ch->pcdata->learned[sn] = UMIN(ch->pcdata->learned[sn], 100);
			if (get_skill(ch, sn) == 100)
				sprintf(buf, "{GYou learn from your mistakes, and manage to perfect %s!{x\n\r", skill_table[sn].name);
			else
				sprintf(buf, "{YYou learn from your mistakes, and your %s skill improves!{x\n\r", skill_table[sn].name);
			send_to_char(buf, ch);
		}
	}
}

/* returns a group index number given the name */
int group_lookup(const char *name)
{
	int gn;

	for (gn = 0; gn < MAX_GROUP; gn++)
	{
		if (group_table[gn].name == NULL)
			break;
		if (LOWER(name[0]) == LOWER(group_table[gn].name[0]) && !str_prefix(name, group_table[gn].name))
			return gn;
	}

	return -1;
}

/* recursively adds a group given its number -- uses group_add */
void gn_add(CHAR_DATA *ch, int gn)
{
	int i;

	ch->pcdata->group_known[gn] = TRUE;
	for (i = 0; i < MAX_IN_GROUP; i++)
	{
		if (group_table[gn].spells[i] == NULL)
			break;
		group_add(ch, group_table[gn].spells[i], FALSE);
	}
}

/* recusively removes a group given its number -- uses group_remove */
void gn_remove(CHAR_DATA *ch, int gn)
{
	int i;

	ch->pcdata->group_known[gn] = FALSE;

	for (i = 0; i < MAX_IN_GROUP; i++)
	{
		if (group_table[gn].spells[i] == NULL)
			break;
		group_remove(ch, group_table[gn].spells[i]);
	}
}

/* use for processing a skill or group for addition  */
void group_add(CHAR_DATA *ch, const char *name, bool deduct)
{
	int sn, gn;

	if (IS_NPC(ch)) /* NPCs do not have skills */
		return;

	sn = skill_lookup(name);

	if (sn != -1)
	{
		if (ch->pcdata->learned[sn] == 0) /* i.e. not known */
		{
			ch->pcdata->learned[sn] = 1;
		}
		return;
	}

	/* now check groups */

	gn = group_lookup(name);

	if (gn != -1)
	{
		if (ch->pcdata->group_known[gn] == FALSE)
		{
			ch->pcdata->group_known[gn] = TRUE;
		}
		gn_add(ch, gn); /* make sure all skills in the group are known */
	}
}

/* used for processing a skill or group for deletion -- no points back! */

void group_remove(CHAR_DATA *ch, const char *name)
{
	int sn, gn;

	sn = skill_lookup(name);

	if (sn != -1)
	{
		ch->pcdata->learned[sn] = 0;
		return;
	}

	/* now check groups */

	gn = group_lookup(name);

	if (gn != -1 && ch->pcdata->group_known[gn] == TRUE)
	{
		ch->pcdata->group_known[gn] = FALSE;
		gn_remove(ch, gn); /* be sure to call gn_add on all remaining groups */
	}
}

void do_specialize(CHAR_DATA *ch, char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int i;
	bool already = FALSE;
	bool anyspecs = FALSE;

	if (IS_NPC(ch))
		return;

	if (IS_SET(ch->act, PLR_BETRAYER))
	{
		send_to_char("You don't feel ready to specialize.\n\r", ch);
		return;
	}

	if (ch->pcdata->special == 0)
	{
		send_to_char("You have no specializations available.\n\r", ch);
		return;
	}

	one_argument(argument, arg);
	if (arg[0] == '\0')
	{
		send_to_char("You can currently specialize in: ", ch);
		for (i = 1; i <= MAX_WEAPON; i++)
		{
			already = FALSE;

			already = is_specced(ch, i);

			if (already == FALSE)
			{
				anyspecs = TRUE;
				send_to_char(weapon_name_lookup(i), ch);
				send_to_char(" ", ch);
			}
		}
		if (anyspecs)
			send_to_char("\n\r", ch);
		else
			send_to_char("nothing.\n\r", ch);

		return;
	}

	if (weapon_num_lookup(arg) > 0)
	{
		for (i = 0; i <= MAX_SPECS; i++)
		{
			if (get_skill(ch, skill_lookup(arg)) < 80)
			{
				send_to_char("You do not feel knowledgable enough in that weapon to specialize in it.\n\r", ch);
				return;
			}
			if (!ch->pcdata->warrior_specs[i])
			{
				ch->pcdata->warrior_specs[i] = weapon_num_lookup(arg);
				already = TRUE;
				break;
			}
			else
			{
				if ((ch->pcdata->warrior_specs[i] == weapon_num_lookup(arg)) ||
					(ch->pcdata->warrior_specs[i] == WEAPON_STAFF && weapon_num_lookup(arg) == WEAPON_SPEAR) ||
					(ch->pcdata->warrior_specs[i] == WEAPON_SPEAR && weapon_num_lookup(arg) == WEAPON_STAFF) ||
					(ch->pcdata->warrior_specs[i] == WEAPON_WHIP && weapon_num_lookup(arg) == WEAPON_FLAIL) ||
					(ch->pcdata->warrior_specs[i] == WEAPON_FLAIL && weapon_num_lookup(arg) == WEAPON_WHIP))
				{
					send_to_char("You are already specialized in that.\n\r", ch);
					return;
				}
			}
		}
		if (!already)
		{
			send_to_char("You cannot specialize anymore!\n\r", ch);
			return;
		}
		sprintf(buf, "ws %s", weapon_name_lookup(weapon_num_lookup(arg)));
		group_add(ch, buf, FALSE);
		send_to_char("You are now specialized in ", ch);
		send_to_char(weapon_name_lookup(weapon_num_lookup(arg)), ch);
		send_to_char(".\n\r", ch);
		ch->pcdata->special--;
	}
	else
	{
		send_to_char("You can't specialize in that.\n\r", ch);
		return;
	}

	return;
}

int is_spec_skill(int sn)
{
	int i, gn, gns, gsn;
	char buf[MAX_STRING_LENGTH];

	for (i = 1; i <= MAX_WEAPON; i++)
	{
		sprintf(buf, "ws %s", weapon_name_lookup(i));
		gn = group_lookup(buf);
		for (gns = 0; gns <= MAX_SPEC_SKILLS; gns++)
		{
			gsn = skill_lookup(group_table[gn].spells[gns]);
			if (gsn == sn)
			{
				return i;
			}
		}
	}
	return -1;
}
int is_mob_spec(int sn)
{
	int i, gns, gsn;
	for (i = GROUP_SWORD; i <= GROUP_POLEARM; i++)
	{
		for (gns = 0; gns <= 4; gns++)
		{
			if (!group_table[i].spells[gns])
				break;
			gsn = skill_lookup(group_table[i].spells[gns]);
			if (gsn == sn)
				return i;
		}
	}
	return -1;
}
bool is_specced(CHAR_DATA *ch, int spec)
{
	int j;

	if (IS_IMMORTAL(ch))
		return TRUE;

	if (IS_NPC(ch))
	{
		if (spec == GROUP_SWORD && IS_SET(ch->extended_flags, OFF_SWORD_SPEC))
			return TRUE;
		if (spec == GROUP_AXE && IS_SET(ch->extended_flags, OFF_AXE_SPEC))
			return TRUE;
		if (spec == GROUP_HTH && IS_SET(ch->extended_flags, OFF_HTH_SPEC))
			return TRUE;
		if (spec == GROUP_DAGGER && IS_SET(ch->extended_flags, OFF_DAGGER_SPEC))
			return TRUE;
		if (spec == GROUP_SPEAR && IS_SET(ch->extended_flags, OFF_SPEAR_SPEC))
			return TRUE;
		if (spec == GROUP_MACE && IS_SET(ch->extended_flags, OFF_MACE_SPEC))
			return TRUE;
		if (spec == GROUP_WHIP && IS_SET(ch->extended_flags, OFF_WHIP_SPEC))
			return TRUE;
		if (spec == GROUP_POLEARM && IS_SET(ch->extended_flags, OFF_POLEARM_SPEC))
			return TRUE;
		return FALSE;
	}
	for (j = 0; j <= MAX_SPECS; j++)
	{
		if ((ch->pcdata->warrior_specs[j] == spec) ||
			(ch->pcdata->warrior_specs[j] == WEAPON_STAFF && spec == WEAPON_SPEAR) ||
			(ch->pcdata->warrior_specs[j] == WEAPON_SPEAR && spec == WEAPON_STAFF) ||
			(ch->pcdata->warrior_specs[j] == WEAPON_WHIP && spec == WEAPON_FLAIL) ||
			(ch->pcdata->warrior_specs[j] == WEAPON_FLAIL && spec == WEAPON_WHIP))
		{
			return TRUE;
		}
	}
	return FALSE;
}

void do_gag(CHAR_DATA *ch, char *argument)
{
	char arg[MIL], buf[MSL];
	CHAR_DATA *victim;
	AFFECT_DATA af;
	int percent, chance;
	one_argument(argument, arg);

	if ((chance = get_skill(ch, gsn_gag)) == 0)
	{
		send_to_char("You do not know where to start.\n\r", ch);
		return;
	}
	if (arg[0] == '\0')
	{
		send_to_char("Gag who?\n\r", ch);
		return;
	}

	if (ch->fighting != NULL)
	{
		send_to_char("You can't do that while fighting.\n\r", ch);
		return;
	}

	if ((victim = get_char_room(ch, arg)) == NULL)
	{
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (victim == ch)
	{
		send_to_char("Kinky.\n\r", ch);
		return;
	}

	if (is_safe(ch, victim))
		return;

	if (victim->position > POS_SLEEPING)
		return send_to_char("But they are still awake!\n\r", ch);

	chance += get_curr_stat(ch, STAT_DEX) - get_curr_stat(victim, STAT_DEX);

	chance += URANGE(-10, (ch->level - victim->level), 10);

	WAIT_STATE(ch, skill_table[gsn_gag].beats);

	percent = number_percent();

	if (percent > chance || percent > 90)
	{
		sprintf(buf, "Help!  %s just tried to gag me!", PERS(ch, victim));
		do_myell(victim, buf);
		check_improve(ch, gsn_gag, FALSE, 1);
		damage(ch, victim, 1, gsn_gag, DAM_NONE, TRUE);
		act("You tie a gag around $N's mouth, but it falls off.", ch, NULL, victim, TO_CHAR);
		act("$n ties a gag around $N's mouth, but it falls off.", ch, NULL, victim, TO_NOTVICT);
	}
	else if (percent < 3 * chance / 4)
	{
		if (!IS_AWAKE(victim))
			do_wake(victim, "");
		if (IS_NPC(victim) && IS_AWAKE(victim))
			multi_hit(victim, ch, TYPE_UNDEFINED);
		init_affect(&af);
		af.type = gsn_gag;
		af.aftype = AFT_SKILL;
		af.level = ch->level;
		af.duration = number_range(0, 1);
		af.location = APPLY_NONE;
		af.modifier = 0;
		af.bitvector = 0;
		affect_to_char(victim, &af);
		check_improve(ch, gsn_gag, TRUE, 1);
		act("You deftly slide a gag into $N's mouth.", ch, NULL, victim, TO_CHAR);
		act("$n slides a gag into $N's mouth.", ch, NULL, victim, TO_NOTVICT);
	}
}

void do_call(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	void *vo;
	int mana;
	int sn;

	int target;
	if (IS_NPC(ch) && ch->desc == NULL)
		return;
	target_name = one_argument(argument, arg1);
	one_argument(target_name, arg2);
	if (arg1[0] == '\0')
		return send_to_char("Call which what where?\n\r", ch);

	if ((sn = find_spell(ch, arg1)) < 1 || (!IS_NPC(ch) && get_skill(ch, sn) < 5) || (!IS_NPC(ch) && ch->pcdata->learned[sn] == 0))
		return send_to_char("You don't know any powers of that name.\n\r", ch);

	if (ch->position < skill_table[sn].minimum_position && get_trust(ch) < MAX_LEVEL - 1)
		return send_to_char("You can't concentrate enough.\n\r", ch);

	if (skill_table[sn].ctype != CMD_POWER && !IS_IMMORTAL(ch))
		return send_to_char("You can't call that.\n\r", ch);

	if (ch->level + 2 == skill_table[sn].skill_level[ch->class])
		mana = 50;
	else
		mana = UMAX(
			skill_table[sn].min_mana,
			100 / (2 + ch->level - skill_table[sn].skill_level[ch->class]));

	switch (get_curr_stat(ch, STAT_INT))
	{
	case 25:
		mana = 43 * mana / 50;
		break;
	case 24:
		mana = 22 * mana / 25;
		break;
	case 23:
		mana = 45 * mana / 50;
		break;
	case 22:
		mana = 23 * mana / 25;
		break;
	case 21:
		mana = 47 * mana / 50;
		break;
	case 20:
		mana = 24 * mana / 25;
		break;
	case 19:
		mana = 49 * mana / 50;
		break;
	}
	{
	}
	/*
	 * Locate targets.
	 */
	victim = NULL;
	obj = NULL;
	vo = NULL;
	target = TARGET_NONE;
	switch (skill_table[sn].target)
	{
	default:
		bug("Do_cast: bad target for sn %d.", sn);
		return;
	case TAR_IGNORE:
		break;
	case TAR_CHAR_OFFENSIVE:
		if (arg2[0] == '\0')
		{
			if ((victim = ch->fighting) == NULL)
				return send_to_char("Call the power on whom?\n\r", ch);
		}
		else
		{
			if ((victim = get_char_room(ch, target_name)) == NULL)
				return send_to_char("They aren't here.\n\r", ch);
		}
		if (!IS_NPC(ch))
			check_killer(ch, victim);

		if (IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim)
			return send_to_char("You can't do that on your own master.\n\r", ch);

		vo = (void *)victim;
		target = TARGET_CHAR;
		break;
	case TAR_CHAR_DEFENSIVE:
		if (arg2[0] == '\0')
			victim = ch;
		else
		{
			if ((victim = get_char_room(ch, target_name)) == NULL)
				return send_to_char("They aren't here.\n\r", ch);
		}
		vo = (void *)victim;
		target = TARGET_CHAR;
		break;
	case TAR_CHAR_SELF:
		if (arg2[0] != '\0' && !is_name(target_name, ch->name))
			return send_to_char("You cannot call this power on another.\n\r", ch);

		vo = (void *)ch;
		target = TARGET_CHAR;
		break;
	case TAR_OBJ_INV:
		if (arg2[0] == '\0')
			return send_to_char("What should the power be called upon?\n\r", ch);

		if ((obj = get_obj_carry(ch, target_name, ch)) == NULL)
			return send_to_char("You are not carrying that.\n\r", ch);

		vo = (void *)obj;
		target = TARGET_OBJ;
		break;
	case TAR_OBJ_CHAR_OFF:
		if (arg2[0] == '\0')
		{
			if ((victim = ch->fighting) == NULL)
				return send_to_char("Call the power on who or what?\n\r", ch);
			target = TARGET_CHAR;
		}
		else if ((victim = get_char_room(ch, target_name)) != NULL)
		{
			target = TARGET_CHAR;
		}
		if (target == TARGET_CHAR) /* check the sanity of the attack */
		{
			if (IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim)
				return send_to_char("You can't do that on your own follower.\n\r", ch);
			if (!IS_NPC(ch))
				check_killer(ch, victim);

			vo = (void *)victim;
		}
		else if ((obj = get_obj_here(ch, target_name)) != NULL)
		{
			vo = (void *)obj;
			target = TARGET_OBJ;
		}
		else
			return send_to_char("You don't see that here.\n\r", ch);
		break;
	case TAR_OBJ_CHAR_DEF:
		if (arg2[0] == '\0')
		{
			vo = (void *)ch;
			target = TARGET_CHAR;
		}
		else if ((victim = get_char_room(ch, target_name)) != NULL)
		{
			vo = (void *)victim;
			target = TARGET_CHAR;
		}
		else if ((obj = get_obj_carry(ch, target_name, ch)) != NULL)
		{
			vo = (void *)obj;
			target = TARGET_OBJ;
		}
		else
			return send_to_char("You don't see that here.\n\r", ch);
		break;
	}
	if (!IS_NPC(ch) && ch->mana < mana)
		return send_to_char("You don't have enough mana.\n\r", ch);

	WAIT_STATE(ch, skill_table[sn].beats);
	if (!IS_NPC(ch) && (number_percent() > get_skill(ch, sn)))
	{
		send_to_char("You lost your concentration.\n\r", ch);
		check_improve(ch, sn, FALSE, 1);
		ch->mana -= mana / 2;
	}
	else
	{
		ch->mana -= mana;
		if (skill_table[sn].target == TAR_CHAR_OFFENSIVE && is_safe(ch, victim))
			return;

		if (skill_table[sn].target == TAR_CHAR_OFFENSIVE)
		{
			if (!IS_NPC(ch) && !IS_NPC(victim) && (ch->fighting == NULL || victim->fighting == NULL))
			{
				switch (number_range(0, 2))
				{
				case (0):
				case (1):
					sprintf(buf, "Die, %s you sorcerous dog!", PERS(ch, victim));
					break;
				case (2):
					sprintf(buf, "Help! %s is casting a spell on me!", PERS(ch, victim));
				}
				if (victim != ch && !IS_NPC(ch))
					do_myell(victim, buf);
			}
		}
		(*skill_table[sn].spell_fun)(sn, ch->level, ch, vo, target);
		check_improve(ch, sn, TRUE, 1);
	}
	if ((skill_table[sn].target == TAR_CHAR_OFFENSIVE || (skill_table[sn].target == TAR_OBJ_CHAR_OFF && target == TARGET_CHAR)) && victim != ch && victim->master != ch)
	{
		CHAR_DATA *vch;
		CHAR_DATA *vch_next;
		for (vch = ch->in_room->people; vch; vch = vch_next)
		{
			vch_next = vch->next_in_room;
			if (victim == vch && victim->fighting == NULL)
			{
				check_killer(victim, ch);
				multi_hit(victim, ch, TYPE_UNDEFINED);
				break;
			}
		}
	}
	return;
}

bool checkScrollsAcumen(CHAR_DATA *ch)
{
	OBJ_DATA *scrolls;

	scrolls = get_eq_char(ch, WEAR_HOLD);
	if (scrolls == NULL)
		return FALSE;

	if (scrolls->pIndexData->vnum != 1231)
		return FALSE;

	return TRUE;
}

void do_plant_explosive(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	char arg1[MIL], arg2[MIL], *target;

	if (get_skill(ch, skill_lookup("plant explosive")) <= 0)
		return send_to_char("You don't know how to plant explosives.\n\r", ch);

	if (is_affected(ch, skill_lookup("plant explosive")))
		return send_to_char("You have not yet crafted another explosive to plant.\n\r", ch);

	target = one_argument(argument, arg1);
	one_argument(target, arg2);

	if (arg1[0] == '\0')
		return send_to_char("You must specify a target.\n\r", ch);

	if ((victim = get_char_room(ch, arg1)) == NULL)
		return send_to_char("They aren't here.\n\r", ch);

	if (victim == ch)
		return send_to_char("You wouldn't do that to yourself.\n\r", ch);
	if (is_safe(ch, victim))
		return;

	if (IS_NPC(victim))
		return send_to_char("They are too wary for you to plant an explosive on.\n\r", ch);

	if (isNewbie(victim) || victim->ghost != 0)
		return send_to_char("You cannot plant explosives on those who cannot defend themselves.\n\r", ch);

	if (arg2[0] == '\0')
		return send_to_char("You must specify a timer for the explosive.\n\r", ch);

	if (atoi(arg2) <= 0)
		return send_to_char("You can't set a timer that is less than at least an hour!\n\r", ch);

	int bombDuration = atoi(arg2);
	int chance = number_percent();

	AFFECT_DATA af;
	init_affect(&af);
	af.duration = bombDuration;
	af.where = TO_AFFECTS;
	af.aftype = AFT_POWER;
	af.level = 60;
	af.type = skill_lookup("plant explosive");
	af.affect_list_msg = str_dup("prevents planting of explosives");

	if (chance <= 80)
	{
		char buf[MSL];
		OBJ_DATA *explosive = create_object(get_obj_index(OBJ_VNUM_THIEF_PLANT_EXPLOSIVE), 55);
		explosive->timer = bombDuration;
		explosive->level = bombDuration;
		char *wordOne = NULL, *wordTwo = NULL, *wordThree = NULL;
		switch (number_range(1, 29))
		{
		case 1:
			wordOne = str_dup("a black");
			break;
		case 2:
			wordOne = str_dup("a ruby-laden");
			break;
		case 3:
			wordOne = str_dup("a jade-encrusted");
			break;
		case 4:
			wordOne = str_dup("an ornate");
			break;
		case 5:
			wordOne = str_dup("an exquisite");
			break;
		case 6:
			wordOne = str_dup("a gleaming white");
			break;
		case 7:
			wordOne = str_dup("a glinting");
			break;
		case 8:
			wordOne = str_dup("a heavy");
			break;
		case 9:
			wordOne = str_dup("an ancient");
			break;
		case 10:
			wordOne = str_dup("a dusty");
			break;
		case 11:
			wordOne = str_dup("an intricately-decorated");
			break;
		case 12:
			wordOne = str_dup("a large");
			break;
		case 13:
			wordOne = str_dup("an enormous");
			break;
		case 14:
			wordOne = str_dup("a dull");
			break;
		case 15:
			wordOne = str_dup("a red");
			break;
		case 16:
			wordOne = str_dup("a blue-hued");
			break;
		case 17:
			wordOne = str_dup("a chromatic");
			break;
		case 18:
			wordOne = str_dup("a shining");
			break;
		case 19:
			wordOne = str_dup("a blackened");
			break;
		case 20:
			wordOne = str_dup("a slightly charred");
			break;
		case 21:
			wordOne = str_dup("a finely-crafted");
			break;
		case 22:
			wordOne = str_dup("a grey-tinted");
			break;
		case 23:
			wordOne = str_dup("a glimmering");
			break;
		case 24:
			wordOne = str_dup("a long-forgotten");
			break;
		case 25:
			wordOne = str_dup("a polished");
			break;
		case 26:
			wordOne = str_dup("a battered");
			break;
		case 27:
			wordOne = str_dup("a fiery");
			break;
		case 28:
			wordOne = str_dup("an ice-encrusted");
			break;
		case 29:
			wordOne = str_dup("a pulsing");
			break;
		}
		switch (number_range(1, 23))
		{
		case 1:
			wordTwo = str_dup("steel");
			break;
		case 2:
			wordTwo = str_dup("iron");
			break;
		case 3:
			wordTwo = str_dup("wooden");
			break;
		case 4:
			wordTwo = str_dup("mithril");
			break;
		case 5:
			wordTwo = str_dup("glass");
			break;
		case 6:
			wordTwo = str_dup("stone");
			break;
		case 7:
			wordTwo = str_dup("silver");
			break;
		case 8:
			wordTwo = str_dup("gold");
			break;
		case 9:
			wordTwo = str_dup("adamantite");
			break;
		case 10:
			wordTwo = str_dup("brass");
			break;
		case 11:
			wordTwo = str_dup("bronze");
			break;
		case 12:
			wordTwo = str_dup("crystal");
			break;
		case 13:
			wordTwo = str_dup("ivory");
			break;
		case 14:
			wordTwo = str_dup("diamond");
			break;
		case 15:
			wordTwo = str_dup("golden");
			break;
		case 16:
			wordTwo = str_dup("wood");
			break;
		case 17:
			wordTwo = str_dup("onyx");
			break;
		case 18:
			wordTwo = str_dup("bone");
			break;
		case 19:
			wordTwo = str_dup("metallic");
			break;
		case 20:
			wordTwo = str_dup("metal");
			break;
		case 22:
			wordTwo = str_dup("obsidian");
			break;
		case 23:
			wordTwo = str_dup("dragonscale");
			break;
		}
		switch (number_range(1, 37))
		{
		case 1:
			wordThree = str_dup("sword");
			break;
		case 2:
			wordThree = str_dup("axe");
			break;
		case 3:
			wordThree = str_dup("whip");
			break;
		case 4:
			wordThree = str_dup("flail");
			break;
		case 5:
			wordThree = str_dup("pike");
			break;
		case 6:
			wordThree = str_dup("spear");
			break;
		case 7:
			wordThree = str_dup("polearm");
			break;
		case 8:
			wordThree = str_dup("blade");
			break;
		case 9:
			wordThree = str_dup("dagger");
			break;
		case 10:
			wordThree = str_dup("skewer");
			break;
		case 11:
			wordThree = str_dup("double-bladed axe");
			break;
		case 12:
			wordThree = str_dup("cleaver");
			break;
		case 13:
			wordThree = str_dup("bastard sword");
			break;
		case 14:
			wordThree = str_dup("great axe");
			break;
		case 15:
			wordThree = str_dup("poleaxe");
			break;
		case 16:
			wordThree = str_dup("halberd");
			break;
		case 17:
			wordThree = str_dup("mace");
			break;
		case 18:
			wordThree = str_dup("hammer");
			break;
		case 19:
			wordThree = str_dup("double-bladed sword");
			break;
		case 20:
			wordThree = str_dup("breastplate");
			break;
		case 21:
			wordThree = str_dup("helmet");
			break;
		case 22:
			wordThree = str_dup("helm");
			break;
		case 23:
			wordThree = str_dup("shortsword");
			break;
		case 24:
			wordThree = str_dup("longsword");
			break;
		case 25:
			wordThree = str_dup("morning-star");
			break;
		case 26:
			wordThree = str_dup("skull cap");
			break;
		case 27:
			wordThree = str_dup("shortspear");
			break;
		case 28:
			wordThree = str_dup("long spear");
			break;
		case 29:
			wordThree = str_dup("chest harness");
			break;
		case 30:
			wordThree = str_dup("chest-protector");
			break;
		case 31:
			wordThree = str_dup("torso");
			break;
		case 32:
			wordThree = str_dup("cap");
			break;
		case 33:
			wordThree = str_dup("full helm");
			break;
		case 34:
			wordThree = str_dup("dirk");
			break;
		case 35:
			wordThree = str_dup("hand-axe");
			break;
		case 36:
			wordThree = str_dup("large axe");
			break;
		case 37:
			wordThree = str_dup("claymore");
			break;
		}

		sprintf(buf, "%s %s %s", wordOne, wordTwo, wordThree);
		char bufname[MSL];
		sprintf(bufname, "%s %s", wordTwo, wordThree);
		explosive->name = str_dup(bufname);
		explosive->short_descr = str_dup(buf);
		sprintf(buf, "%s %s %s is lying here.", wordOne, wordTwo, wordThree);
		explosive->description = str_dup(buf);
		explosive->owner = str_dup(ch->original_name);
		obj_to_char(explosive, victim);
		affect_to_char(ch, &af);
		sprintf(buf, "You have successfully planted an explosive among %s's belongings!\n\r", victim->name);
		return send_to_char(buf, ch);
	}

	else if (chance <= 95)
	{
		af.duration = 0;
		affect_to_char(ch, &af);
		return send_to_char("You failed to plant the explosive, but remained uncaught.\n\r", ch);
	}

	else
	{
		char buf[MSL];
		sprintf(buf, "%s is attempting to plant something within my possessions!", !IS_NPC(ch) ? ch->name : ch->short_descr);
		do_yell(victim, buf);
		multi_hit(victim, ch, TYPE_UNDEFINED);
		af.duration = bombDuration / 3;
		affect_to_char(ch, &af);
		return send_to_char("You failed to plant the explosive and revealed your intentions!\n\r", ch);
	}
}

void do_undisguise(CHAR_DATA *ch, char *argument)
{
	if (get_skill(ch, skill_lookup("disguise")) <= 0)
		return send_to_char("Huh?\n\r", ch);

	if (is_affected(ch, skill_lookup("disguise")))
	{
		send_to_char("You toss your disguise aside.\n\r", ch);
		affect_strip(ch, skill_lookup("disguise"));
	}
	else
		send_to_char("You aren't wearing a disguise!\n\r", ch);
	return;
}

void do_disguise(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *corpse;
	char af_buf[MSL], buf[MSL];

	if (is_affected(ch, skill_lookup("disguise timer")))
		return send_to_char("You have not yet readied yourself to craft a disguise.\n\r", ch);

	if (is_affected(ch, skill_lookup("disguise")))
		return send_to_char("You are already disguised!\n\r", ch);

	if (argument[0] == '\0')
		return send_to_char("You must specify a target.\n\r", ch);

	if ((corpse = get_obj_here(ch, argument)) == NULL)
		return send_to_char("That isn't here.\n\r", ch);

	sprintf(af_buf, "disguised as %s", corpse->corpse_of);

	AFFECT_DATA af;
	init_affect(&af);
	af.duration = ch->level;
	af.where = TO_AFFECTS;
	af.aftype = AFT_SKILL;
	af.level = ch->level;
	af.type = skill_lookup("disguise");
	af.affect_list_msg = str_dup(af_buf);
	affect_to_char(ch, &af);
	af.type = skill_lookup("disguise timer");
	af.duration = ch->level / 10;
	af.affect_list_msg = str_dup("prevents usage of disguise");
	affect_to_char(ch, &af);
	ch->name = str_dup(corpse->corpse_of);

	sprintf(buf, "Using the corpse and its belongings, you cleverly disguise yourself as %s!\n\r", corpse->corpse_of);
	send_to_char(buf, ch);
	return;
}

int get_rand_skill(CHAR_DATA *ch, bool ignore_perfected)
{
	int sn = 0;
	while (1)
	{
		sn = number_range(0, MAX_SKILL);

		if (!ignore_perfected)
		{
			if (ch->pcdata->learned[sn] > 1)
				return sn;
		}
		else
		{
			if (ch->pcdata->learned[sn] > 1 && ch->pcdata->learned[sn] < 100)
				return sn;
		}
	}
}
