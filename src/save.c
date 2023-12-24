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

extern int _filbuf args((FILE *));

int rename(const char *oldfname, const char *newfname);

char *print_flags(int flag)
{
	int count, pos = 0;
	static char buf[52];

	for (count = 0; count < 32; count++)
	{
		if (IS_SET(flag, 1 << count))
		{
			if (count < 26)
				buf[pos] = 'A' + count;
			else
				buf[pos] = 'a' + (count - 26);
			pos++;
		}
	}

	if (pos == 0)
	{
		buf[pos] = '0';
		pos++;
	}

	buf[pos] = '\0';

	return buf;
}

/*
 * Array of containers read for proper re-nesting of objects.
 */
#define MAX_NEST 100
static OBJ_DATA *rgObjNest[MAX_NEST];

/*
 * Local functions.
 */
void fwrite_char args((CHAR_DATA * ch, FILE *fp));
void fwrite_obj args((CHAR_DATA * ch, OBJ_DATA *obj, FILE *fp, int iNest));
void fread_char args((CHAR_DATA * ch, FILE *fp));
void fread_obj args((CHAR_DATA * ch, FILE *fp));

/*
 * Save a character and inventory.
 * Would be cool to save NPC's too for quest purposes,
 *   some of the infrastructure is provided.
 */
void save_char_obj(CHAR_DATA *ch)
{
	char strsave[MAX_INPUT_LENGTH];
	FILE *fp;

	if (IS_NPC(ch))
		return;

	if (is_affected(ch, skill_lookup("age death")))
		return send_to_char("You are dead; you can't be saved.\n\r", ch);

	if (ch->desc != NULL && ch->desc->original != NULL)
		ch = ch->desc->original;

	fclose(fpReserve);
	sprintf(strsave, "%s%s.plr", PLAYER_DIR, capitalize(ch->original_name));

	if ((fp = fopen(TEMP_FILE, "w")) == NULL)
	{
		bug("Save_char_obj: fopen", 0);
		perror(strsave);
	}
	else
	{
		fwrite_char(ch, fp);
		if (ch->carrying != NULL)
			fwrite_obj(ch, ch->carrying, fp, 0);
		fprintf(fp, "#END\n");
	}
	fclose(fp);
	rename(TEMP_FILE, strsave);
	fpReserve = fopen(NULL_FILE, "r");

	return;
}

void fwrite_char(CHAR_DATA *ch, FILE *fp)
{
	AFFECT_DATA *paf;
	int sn, gn, pos, i;
	OBJ_DATA *obj;

	fprintf(fp, "#%s\n", IS_NPC(ch) ? "MOB" : "PLAYER");
	fprintf(fp, "Name %s~\n", ch->original_name);
	fprintf(fp, "Id   %ld\n", ch->id);
	fprintf(fp, "LogO %ld\n", current_time);
	fprintf(fp, "Vers %d\n", 14);
	if (!ch->pcdata->old && ch->short_descr[0] != '\0')
		fprintf(fp, "ShD  %s~\n", ch->short_descr);
	if (ch->pcdata->old && ch->pcdata->old->short_descr[0] != '\0')
		fprintf(fp, "ShD  %s~\n", ch->pcdata->old->short_descr);

	if (!ch->pcdata->old && ch->long_descr[0] != '\0')
		fprintf(fp, "LnD  %s~\n", ch->long_descr);
	if (ch->pcdata->old && ch->pcdata->old->long_descr[0] != '\0')
		fprintf(fp, "LnD  %s~\n", ch->pcdata->old->long_descr);

	if (!ch->pcdata->old && ch->description[0] != '\0')
		fprintf(fp, "Desc %s~\n", ch->description);
	if (ch->pcdata->old && ch->pcdata->old->description[0] != '\0')
		fprintf(fp, "Desc %s~\n", ch->pcdata->old->description);
	if (ch->prompt != NULL || !str_cmp(ch->prompt, "{c<%hhp %mm %vmv>{x "))
		fprintf(fp, "Prom %s~\n", ch->prompt);
	fprintf(fp, "Race %s~\n", pc_race_table[ch->race].name);
	if (ch->cabal)
		fprintf(fp, "Cabal %s~\n", cabal_table[ch->cabal].name);
	fprintf(fp, "Sex  %d\n", ch->sex);
	fprintf(fp, "Cla  %d\n", ch->class);
	fprintf(fp, "Levl %d\n", ch->level);
	if (ch->trust != 0)
		fprintf(fp, "Tru  %d\n", ch->trust);
	fprintf(fp, "Sec  %d\n", ch->pcdata->security); /* OLC */
	fprintf(fp, "Plyd %d\n",
			ch->played + (int)(current_time - ch->logon));
	if (!IS_NPC(ch))
	{
		fprintf(fp, "Agemod %d\n", ch->pcdata->age_mod);
		if (ch->pcdata->death_status == HAS_DIED)
		{
			fprintf(fp, "Died %d\n", ch->pcdata->death_status);
			fprintf(fp, "Dtime %d\n", ch->pcdata->death_timer);
		}
	}
	/* temples for hometowns */
	fprintf(fp, "Temple %d\n", (ch->temple == 0) ? TEMPLE_MIDGAARD : ch->temple);

	fprintf(fp, "Not  %ld %ld %ld %ld %ld\n",
			ch->pcdata->last_note, ch->pcdata->last_idea, ch->pcdata->last_penalty,
			ch->pcdata->last_news, ch->pcdata->last_changes);

	fprintf(fp, "Room %ld\n",
			(ch->in_room == get_room_index(ROOM_VNUM_LIMBO) && ch->was_in_room != NULL)
				? ch->was_in_room->vnum
			: ch->in_room == NULL ? 3001
								  : ch->in_room->vnum);

	fprintf(fp, "HomeTown %d\n", ch->hometown);

	fprintf(fp, "HMV  %d %d %d %d %d %d\n",
			ch->hit, ch->max_hit, ch->mana, ch->max_mana, ch->move, ch->max_move);
	fprintf(fp, "Noregen %d\n", ch->noregen_dam);
	if (!IS_NPC(ch))
	{
		if (ch->pcdata->death_count == 0)
			fprintf(fp, "Deaths %d\n", 0);
		else
			fprintf(fp, "Deaths %d\n", ch->pcdata->death_count);
		if (ch->pcdata->sp == 0)
			fprintf(fp, "Sp %d\n", 0);
		else
			fprintf(fp, "Sp %d\n", ch->pcdata->sp);
		if (ch->pcdata->special == 0)
			fprintf(fp, "Specialize %d\n", 0);
		else
			fprintf(fp, "Specialize %d\n", ch->pcdata->special);
	}

	fprintf(fp, "RollTime %ld\n", ch->pcdata->roll_time);
	if (ch->gold > 0)
		fprintf(fp, "Gold %ld\n", ch->gold);
	else
		fprintf(fp, "Gold %d\n", 0);
	if (ch->quest_credits > 0)
		fprintf(fp, "QuestCredits %ld\n", ch->quest_credits);
	else
		fprintf(fp, "QuestCredits %d\n", 0);
	if (ch->numAttacks > 0)
		fprintf(fp, "Attack_Mod %d\n", ch->base_numAttacks);
	else
		fprintf(fp, "Attack_Mod %d\n", 0);
	if (ch->enhancedDamMod > 0)
		fprintf(fp, "EnhancedDamMod %.0f\n", ch->base_enhancedDamMod);
	else
		fprintf(fp, "EnhancedDamMod %d\n", 100);
	fprintf(fp, "Regen %d\n", ch->base_regen);
	fprintf(fp, "Dam_mod %d\n", ch->base_dam_mod);
	if (ch->gold_bank > 0)
		fprintf(fp, "Bgold %ld\n", ch->gold_bank);
	else
		fprintf(fp, "Bgold %d\n", 0);
	fprintf(fp, "Exp  %d\n", ch->exp);
	fprintf(fp, "ExpT %d\n", ch->exp_total);
	if (ch->act != 0)
		fprintf(fp, "Act  %s\n", print_flags(ch->act));
	if (ch->affected_by != 0)
		fprintf(fp, "AfBy %s\n", print_flags(ch->affected_by));
	fprintf(fp, "Comm %s\n", print_flags(ch->comm));
	if (ch->wiznet)
		fprintf(fp, "Wizn %s\n", print_flags(ch->wiznet));
	if (ch->invis_level)
		fprintf(fp, "Invi %d\n", ch->invis_level);
	if (ch->incog_level)
		fprintf(fp, "Inco %d\n", ch->incog_level);
	fprintf(fp, "Pos  %d\n",
			ch->position == POS_FIGHTING ? POS_STANDING : ch->position);
	if (ch->practice != 0)
		fprintf(fp, "Prac %d\n", ch->practice);
	if (ch->train != 0)
		fprintf(fp, "Trai %d\n", ch->train);

	if (!ch->pcdata->old && ch->saving_throw != 0)
		fprintf(fp, "Save  %d\n", ch->saving_throw);
	if (ch->pcdata->old && ch->saving_throw != 0)
		fprintf(fp, "Save  %d\n", ch->pcdata->old->saving_throw);

	fprintf(fp, "Wantd %d\n", ch->pcdata->wanteds);
	fprintf(fp, "Lines %d\n", ch->lines);
	fprintf(fp, "Alig  %d\n", ch->alignment);
	fprintf(fp, "Etho  %d\n", ch->pcdata->ethos);
	fprintf(fp, "OAli  %d\n", ch->pcdata->oalign);
	fprintf(fp, "OEth  %d\n", ch->pcdata->oethos);
	fprintf(fp, "Indu  %d\n", ch->pcdata->induct);
	fprintf(fp, "Empr  %d\n", ch->pcdata->empire);
	fprintf(fp, "Bounty %ld\n", ch->pcdata->bounty);
	fprintf(fp, "BCredits %d\n", ch->pcdata->bounty_credits);
	if (ch->hitroll != 0)
		fprintf(fp, "Hit   %d\n", ch->hitroll);
	if (ch->damroll != 0)
		fprintf(fp, "Dam   %d\n", ch->damroll);
	if (!ch->pcdata->old)
		fprintf(fp, "ACs %d %d %d %d\n",
				ch->armor[0], ch->armor[1], ch->armor[2], ch->armor[3]);
	else
		fprintf(fp, "ACs %d %d %d %d\n",
				ch->pcdata->old->armor[0], ch->pcdata->old->armor[1], ch->pcdata->old->armor[2], ch->pcdata->old->armor[3]);
	fprintf(fp, "IMMUNE %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
			IS_SET(ch->imm_flags, IMM_SUMMON) ? 1 : 0,
			IS_SET(ch->imm_flags, IMM_CHARM) ? 1 : 0,
			IS_SET(ch->imm_flags, IMM_MAGIC) ? 1 : 0,
			IS_SET(ch->imm_flags, IMM_WEAPON) ? 1 : 0,
			IS_SET(ch->imm_flags, IMM_BASH) ? 1 : 0,
			IS_SET(ch->imm_flags, IMM_PIERCE) ? 1 : 0,
			IS_SET(ch->imm_flags, IMM_SLASH) ? 1 : 0,
			IS_SET(ch->imm_flags, IMM_FIRE) ? 1 : 0,
			IS_SET(ch->imm_flags, IMM_COLD) ? 1 : 0,
			IS_SET(ch->imm_flags, IMM_LIGHTNING) ? 1 : 0,
			IS_SET(ch->imm_flags, IMM_ACID) ? 1 : 0,
			IS_SET(ch->imm_flags, IMM_POISON) ? 1 : 0,
			IS_SET(ch->imm_flags, IMM_NEGATIVE) ? 1 : 0,
			IS_SET(ch->imm_flags, IMM_HOLY) ? 1 : 0,
			IS_SET(ch->imm_flags, IMM_ENERGY) ? 1 : 0,
			IS_SET(ch->imm_flags, IMM_MENTAL) ? 1 : 0,
			IS_SET(ch->imm_flags, IMM_DISEASE) ? 1 : 0,
			IS_SET(ch->imm_flags, IMM_DROWNING) ? 1 : 0,
			IS_SET(ch->imm_flags, IMM_LIGHT) ? 1 : 0,
			IS_SET(ch->imm_flags, IMM_SOUND) ? 1 : 0,
			IS_SET(ch->imm_flags, IMM_WOOD) ? 1 : 0,
			IS_SET(ch->imm_flags, IMM_SILVER) ? 1 : 0,
			IS_SET(ch->imm_flags, IMM_IRON) ? 1 : 0,
			IS_SET(ch->imm_flags, IMM_SLEEP) ? 1 : 0);

	fprintf(fp, "RESIST %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
			IS_SET(ch->res_flags, RES_SUMMON) ? 1 : 0,
			IS_SET(ch->res_flags, RES_CHARM) ? 1 : 0,
			IS_SET(ch->res_flags, RES_MAGIC) ? 1 : 0,
			IS_SET(ch->res_flags, RES_WEAPON) ? 1 : 0,
			IS_SET(ch->res_flags, RES_BASH) ? 1 : 0,
			IS_SET(ch->res_flags, RES_PIERCE) ? 1 : 0,
			IS_SET(ch->res_flags, RES_SLASH) ? 1 : 0,
			IS_SET(ch->res_flags, RES_FIRE) ? 1 : 0,
			IS_SET(ch->res_flags, RES_COLD) ? 1 : 0,
			IS_SET(ch->res_flags, RES_LIGHTNING) ? 1 : 0,
			IS_SET(ch->res_flags, RES_ACID) ? 1 : 0,
			IS_SET(ch->res_flags, RES_POISON) ? 1 : 0,
			IS_SET(ch->res_flags, RES_NEGATIVE) ? 1 : 0,
			IS_SET(ch->res_flags, RES_HOLY) ? 1 : 0,
			IS_SET(ch->res_flags, RES_ENERGY) ? 1 : 0,
			IS_SET(ch->res_flags, RES_MENTAL) ? 1 : 0,
			IS_SET(ch->res_flags, RES_DISEASE) ? 1 : 0,
			IS_SET(ch->res_flags, RES_DROWNING) ? 1 : 0,
			IS_SET(ch->res_flags, RES_LIGHT) ? 1 : 0,
			IS_SET(ch->res_flags, RES_SOUND) ? 1 : 0,
			IS_SET(ch->res_flags, RES_WOOD) ? 1 : 0,
			IS_SET(ch->res_flags, RES_SILVER) ? 1 : 0,
			IS_SET(ch->res_flags, RES_IRON) ? 1 : 0);

	fprintf(fp, "VULN %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
			IS_SET(ch->vuln_flags, VULN_SUMMON) ? 1 : 0,
			IS_SET(ch->vuln_flags, VULN_CHARM) ? 1 : 0,
			IS_SET(ch->vuln_flags, VULN_MAGIC) ? 1 : 0,
			IS_SET(ch->vuln_flags, VULN_WEAPON) ? 1 : 0,
			IS_SET(ch->vuln_flags, VULN_BASH) ? 1 : 0,
			IS_SET(ch->vuln_flags, VULN_PIERCE) ? 1 : 0,
			IS_SET(ch->vuln_flags, VULN_SLASH) ? 1 : 0,
			IS_SET(ch->vuln_flags, VULN_FIRE) ? 1 : 0,
			IS_SET(ch->vuln_flags, VULN_COLD) ? 1 : 0,
			IS_SET(ch->vuln_flags, VULN_LIGHTNING) ? 1 : 0,
			IS_SET(ch->vuln_flags, VULN_ACID) ? 1 : 0,
			IS_SET(ch->vuln_flags, VULN_POISON) ? 1 : 0,
			IS_SET(ch->vuln_flags, VULN_NEGATIVE) ? 1 : 0,
			IS_SET(ch->vuln_flags, VULN_HOLY) ? 1 : 0,
			IS_SET(ch->vuln_flags, VULN_ENERGY) ? 1 : 0,
			IS_SET(ch->vuln_flags, VULN_MENTAL) ? 1 : 0,
			IS_SET(ch->vuln_flags, VULN_DISEASE) ? 1 : 0,
			IS_SET(ch->vuln_flags, VULN_DROWNING) ? 1 : 0,
			IS_SET(ch->vuln_flags, VULN_LIGHT) ? 1 : 0,
			IS_SET(ch->vuln_flags, VULN_SOUND) ? 1 : 0,
			IS_SET(ch->vuln_flags, VULN_WOOD) ? 1 : 0,
			IS_SET(ch->vuln_flags, VULN_SILVER) ? 1 : 0,
			IS_SET(ch->vuln_flags, VULN_IRON) ? 1 : 0);

	fprintf(fp, "Newbie %d\n", ch->pcdata->newbie);

	for (i = 0; i <= MAX_SPECS; i++)
	{
		if (ch->pcdata->warrior_specs[i])
			fprintf(fp, "Spec%d %d~\n", i, ch->pcdata->warrior_specs[i]);
	}

	if (ch->pcdata->dedication > 0)
		fprintf(fp, "Dedication %d\n", ch->pcdata->dedication);
	else if (ch->pcdata->dedication == 0)
		fprintf(fp, "Dedication 0\n");

	if (ch->wimpy != 0)
		fprintf(fp, "Wimp  %d\n", ch->wimpy);

	if (!ch->pcdata->old)
		fprintf(fp, "Attr %d %d %d %d %d\n",
				ch->perm_stat[STAT_STR],
				ch->perm_stat[STAT_INT],
				ch->perm_stat[STAT_WIS],
				ch->perm_stat[STAT_DEX],
				ch->perm_stat[STAT_CON]);
	if (ch->pcdata->old)
		fprintf(fp, "AMod %d %d %d %d %d\n",
				ch->mod_stat[STAT_STR],
				ch->mod_stat[STAT_INT],
				ch->mod_stat[STAT_WIS],
				ch->mod_stat[STAT_DEX],
				ch->mod_stat[STAT_CON]);

	if (IS_NPC(ch))
	{
		fprintf(fp, "Vnum %ld\n", ch->pIndexData->vnum);
	}
	else
	{
		fprintf(fp, "Pass %s~\n", ch->pcdata->pwd);
		if (ch->pcdata->bamfin[0] != '\0')
			fprintf(fp, "Bin  %s~\n", ch->pcdata->bamfin);
		if (ch->pcdata->bamfout[0] != '\0')
			fprintf(fp, "Bout %s~\n", ch->pcdata->bamfout);
		if (ch->pcdata->smite[0] != '\0')
			fprintf(fp, "Smite %s~\n", ch->pcdata->smite);
		fprintf(fp, "Titl %s~\n", ch->pcdata->title);
		if (ch->pcdata->extitle)
			fprintf(fp, "EXTitl %s~\n", ch->pcdata->extitle);

		fprintf(fp, "TSex %d\n", ch->pcdata->true_sex);
		fprintf(fp, "LLev %d\n", ch->pcdata->last_level);
		fprintf(fp, "HMVP %d %d %d\n", ch->pcdata->perm_hit,
				ch->pcdata->perm_mana,
				ch->pcdata->perm_move);
		fprintf(fp, "Cnd  %d %d %d %d %d %d\n",
				ch->pcdata->condition[0],
				ch->pcdata->condition[1],
				ch->pcdata->condition[2],
				ch->pcdata->condition[3],
				ch->pcdata->condition[4],
				ch->pcdata->condition[5]);

		if (ch->cloaked == 1)
			fprintf(fp, "IsCloaked %d\n", ch->cloaked);
		else
			fprintf(fp, "IsCloaked 0\n");
		/* kills and killed scores */
		fprintf(fp, "kls %d %d %d %d\n",
				ch->pcdata->kills[PK_KILLS],
				ch->pcdata->kills[PK_GOOD],
				ch->pcdata->kills[PK_NEUTRAL],
				ch->pcdata->kills[PK_EVIL]);

		fprintf(fp, "kld %d %d\n",
				ch->pcdata->killed[PK_KILLED],
				ch->pcdata->killed[MOB_KILLED]);

		/* write alias */
		for (pos = 0; pos < MAX_ALIAS; pos++)
		{
			if (ch->pcdata->alias[pos] == NULL || ch->pcdata->alias_sub[pos] == NULL)
				break;

			fprintf(fp, "Alias %s %s~\n", ch->pcdata->alias[pos],
					ch->pcdata->alias_sub[pos]);
		}

		for (sn = 0; sn < MAX_SKILL; sn++)
		{
			if (skill_table[sn].name != NULL && (ch->pcdata->learned[sn] > 0 || ch->pcdata->learned[sn] == -2))
			{
				fprintf(fp, "Sk %d '%s'\n",
						ch->pcdata->learned[sn], skill_table[sn].name);
			}
		}

		for (gn = 0; gn < MAX_GROUP; gn++)
		{
			if (group_table[gn].name != NULL && ch->pcdata->group_known[gn])
			{
				fprintf(fp, "Gr '%s'\n", group_table[gn].name);
			}
		}
	}

	for (paf = ch->affected; paf != NULL; paf = paf->next)
	{
		if (paf->type < 0 || paf->type >= MAX_SKILL || paf->type == skill_lookup("disguise") || paf->type == skill_lookup("cloak"))
			continue;

		if (paf->name == NULL)
			paf->name = skill_table[paf->type].name;
		fprintf(fp, "Affc '%s' %s~ %s~ %s~ %c %c %3d %3d %3d %3d %3d %10d %3d\n",
				skill_table[paf->type].name,
				paf->owner_name ? paf->owner_name : str_dup("(null)"),
				paf->name,
				paf->affect_list_msg,
				paf->dispel_at_death ? 'T' : 'F',
				paf->strippable ? 'T' : 'F',
				paf->where,
				paf->level,
				paf->duration,
				paf->modifier,
				paf->location,
				paf->bitvector,
				paf->aftype);
	}

	if (ch->pcdata->sitetrack)
		fprintf(fp, "SiteTrack %s~\n", ch->pcdata->sitetrack);

	if (ch->pcdata->logon_time)
		fprintf(fp, "LogonTime %s~\n", ch->pcdata->logon_time);

	fprintf(fp, "TimePlayed %d~\n", (int)((current_time - ch->logon) / 60));

	if (ch->pcdata->history_buffer != NULL)
		fprintf(fp, "History %s~\n", ch->pcdata->history_buffer);

	fprintf(fp, "TrackAObj %d~\n", count_carried(ch, FALSE));
	fprintf(fp, "TrackLObj %d~\n", count_carried(ch, TRUE));

	fprintf(fp, "FingEQ");
	for (i = 0; i < MAX_WEAR; i++)
	{
		if ((obj = get_eq_char(ch, i)) == NULL)
		{
			fprintf(fp, " 0");
		}
		else
		{
			fprintf(fp, " %ld", obj->pIndexData->vnum);
		}
	}
	fprintf(fp, "\n");

	fprintf(fp, "End\n\n");
	return;
}

/*
 * Write an object and its contents.
 */
void fwrite_obj(CHAR_DATA *ch, OBJ_DATA *obj, FILE *fp, int iNest)
{
	EXTRA_DESCR_DATA *ed;
	AFFECT_DATA *paf;

	/*
	 * Slick recursion to write lists backwards,
	 *   so loading them will load in forwards order.
	 */
	if (obj->next_content != NULL)
		fwrite_obj(ch, obj->next_content, fp, iNest);

	/*
	 * Castrate storage characters.
	 */
	if (obj->item_type == ITEM_KEY || (obj->item_type == ITEM_MAP && !obj->value[0]))
		return;

	fprintf(fp, "#O\n");
	fprintf(fp, "Vnum %ld\n", obj->pIndexData->vnum);
	if (!obj->pIndexData->new_format)
		fprintf(fp, "Oldstyle\n");
	if (obj->enchanted)
		fprintf(fp, "Enchanted\n");
	fprintf(fp, "Nest %d\n", iNest);

	/* these data are only used if they do not match the defaults */

	if (obj->name != obj->pIndexData->name)
		fprintf(fp, "Name %s~\n", obj->name);
	if (obj->short_descr != obj->pIndexData->short_descr)
		fprintf(fp, "ShD  %s~\n", obj->short_descr);
	if (obj->description != obj->pIndexData->description)
		fprintf(fp, "Desc %s~\n", obj->description);
	if (obj->owner != NULL && str_cmp(obj->owner, "none"))
		fprintf(fp, "Owner %s~\n", obj->owner);
	fprintf(fp, "ExtF ");
	if (&obj->extra_flags)
	{
		save_bitmask(&obj->extra_flags, fp);
	}
	if (obj->wear_flags != obj->pIndexData->wear_flags)
		fprintf(fp, "WeaF %ld\n", obj->wear_flags);
	if (obj->item_type != obj->pIndexData->item_type)
		fprintf(fp, "Ityp %d\n", obj->item_type);
	if (obj->weight != obj->pIndexData->weight)
		fprintf(fp, "Wt   %d\n", obj->weight);
	if (obj->condition != obj->pIndexData->condition)
		fprintf(fp, "Cond %d\n", obj->condition);

	/* variable data */

	fprintf(fp, "Wear %d\n", obj->wear_loc);
	if (obj->level != obj->pIndexData->level)
		fprintf(fp, "Lev  %d\n", obj->level);
	if (obj->timer != 0)
		fprintf(fp, "Time %d\n", obj->timer);
	fprintf(fp, "Cost %d\n", obj->cost);
	if (obj->value[0] != obj->pIndexData->value[0] || obj->value[1] != obj->pIndexData->value[1] || obj->value[2] != obj->pIndexData->value[2] || obj->value[3] != obj->pIndexData->value[3] || obj->value[4] != obj->pIndexData->value[4])
		fprintf(fp, "Val  %ld %ld %ld %ld %ld\n",
				obj->value[0], obj->value[1], obj->value[2], obj->value[3],
				obj->value[4]);

	switch (obj->item_type)
	{
	case ITEM_POTION:
	case ITEM_SCROLL:
	case ITEM_PILL:
		if (obj->value[1] > 0)
		{
			fprintf(fp, "Spell 1 '%s'\n",
					skill_table[obj->value[1]].name);
		}

		if (obj->value[2] > 0)
		{
			fprintf(fp, "Spell 2 '%s'\n",
					skill_table[obj->value[2]].name);
		}

		if (obj->value[3] > 0)
		{
			fprintf(fp, "Spell 3 '%s'\n",
					skill_table[obj->value[3]].name);
		}

		break;

	case ITEM_STAFF:
	case ITEM_WAND:
		if (obj->value[3] > 0)
		{
			fprintf(fp, "Spell 3 '%s'\n",
					skill_table[obj->value[3]].name);
		}

		break;
	}

	for (paf = obj->affected; paf != NULL; paf = paf->next)
	{
		if (paf->type < 0 || paf->type >= MAX_SKILL)
			continue;

		fprintf(fp, "Affc '%s' %s~ %3d %3d %3d %3d %3d %10d %3d\n",
				skill_table[paf->type].name,
				paf->owner_name ? paf->owner_name : str_dup("(null)"),
				paf->where,
				paf->level,
				paf->duration,
				paf->modifier,
				paf->location,
				paf->bitvector,
				paf->aftype);
	}

	for (ed = obj->extra_descr; ed != NULL; ed = ed->next)
	{
		fprintf(fp, "ExDe %s~ %s~\n",
				ed->keyword, ed->description);
	}

	fprintf(fp, "End\n\n");

	if (obj->contains != NULL)
		fwrite_obj(ch, obj->contains, fp, iNest + 1);

	return;
}

/*
 * Load a char and inventory into a new ch structure.
 */
bool load_char_obj(DESCRIPTOR_DATA *d, char *name)
{
	char strsave[MAX_INPUT_LENGTH];
	CHAR_DATA *ch;
	FILE *fp;
	bool found;
	int stat, pos;

	ch = new_char();
	ch->pcdata = new_pcdata();

	d->character = ch;
	ch->desc = d;
	ch->name = str_dup(name);
	ch->original_name = str_dup(name);
	ch->id = get_pc_id();
	ch->race = race_lookup("human");
	ch->act = PLR_NOSUMMON;
	ch->comm = COMM_COMBINE | COMM_PROMPT;
	ch->extracted = FALSE;
	ch->prompt = str_dup("<%hhp %mm %vmv> ");
	ch->pcdata->confirm_delete = FALSE;
	ch->pcdata->pwd = str_dup("");
	ch->pcdata->bamfin = str_dup("");
	ch->pcdata->bamfout = str_dup("");
	ch->pcdata->smite = str_dup("");
	ch->pcdata->title = str_dup("");
	ch->pcdata->shifted = -1;
	for (stat = 0; stat < MAX_STATS; stat++)
		ch->perm_stat[stat] = 13;
	for (pos = 0; pos < MAX_FORM; pos++)
		ch->pcdata->forms[pos] = 0;
	ch->pcdata->condition[COND_THIRST] = 48;
	ch->pcdata->condition[COND_FULL] = 48;
	ch->pcdata->condition[COND_HUNGER] = 48;
	ch->pcdata->condition[COND_STARVING] = 0;
	ch->pcdata->condition[COND_DEHYDRATED] = 0;
	ch->pcdata->security = 0;
	ch->dam_mod = 100;
	ch->eavesdrop = 0;
	ch->enhancedDamMod = 100;
	ch->regen_rate = 0;
	ch->numAttacks = 0;
	ch->pcdata->bounty = 0;
	ch->pcdata->bounty_credits = 0;
	ch->pcdata->wanteds = 0;

	found = FALSE;
	fclose(fpReserve);

	sprintf(strsave, "%s%s.plr", PLAYER_DIR, capitalize(name));
	if ((fp = fopen(strsave, "r")) != NULL)
	{
		int iNest;

		for (iNest = 0; iNest < MAX_NEST; iNest++)
			rgObjNest[iNest] = NULL;

		found = TRUE;
		for (;;)
		{
			char letter;
			char *word;

			letter = fread_letter(fp);
			if (letter == '*')
			{
				fread_to_eol(fp);
				continue;
			}

			if (letter != '#')
			{
				bug("Load_char_obj: # not found.", 0);
				break;
			}

			word = fread_word(fp);
			if (!str_cmp(word, "PLAYER"))
				fread_char(ch, fp);
			else if (!str_cmp(word, "OBJECT"))
				fread_obj(ch, fp);
			else if (!str_cmp(word, "O"))
				fread_obj(ch, fp);
			else if (!str_cmp(word, "END"))
				break;
			else
			{
				bug("Load_char_obj: bad section.", 0);
				break;
			}
		}
		fclose(fp);
	}

	free_string(ch->original_name);
	ch->original_name = str_dup(capitalize(ch->name));

	fpReserve = fopen(NULL_FILE, "r");

	/* initialize race */
	if (found)
	{
		int i;

		if (ch->race == 0)
			ch->race = race_lookup("human");

		ch->size = pc_race_table[ch->race].size;
		ch->dam_type = 17; /*punch */

		for (i = 0; i < 5; i++)
		{
			if (pc_race_table[ch->race].skills[i] == NULL)
				break;
			group_add(ch, pc_race_table[ch->race].skills[i], FALSE);
		}
		ch->affected_by = ch->affected_by | race_table[ch->race].aff;
		if (ch->version < 12)
		{
			ch->imm_flags = ch->imm_flags | race_table[ch->race].imm;
			ch->res_flags = ch->res_flags | race_table[ch->race].res;
			ch->vuln_flags = ch->vuln_flags | race_table[ch->race].vuln;
		}
		ch->form = race_table[ch->race].form;
		ch->parts = race_table[ch->race].parts;
	}
	return found;
}

/*
 * Read in a char.
 */

#if defined(KEY)
#undef KEY
#endif

#define KEY(literal, field, value) \
	if (!str_cmp(word, literal))   \
	{                              \
		field = value;             \
		fMatch = TRUE;             \
		break;                     \
	}

/* provided to free strings */
#if defined(KEYS)
#undef KEYS
#endif

#define KEYS(literal, field, value) \
	if (!str_cmp(word, literal))    \
	{                               \
		free_string(field);         \
		field = value;              \
		fMatch = TRUE;              \
		break;                      \
	}

void fread_char(CHAR_DATA *ch, FILE *fp)
{
	char buf[MAX_STRING_LENGTH];
	char *word; //, bugMsg[80];
	bool fMatch;
	int count = 0;
	int lastlogoff = current_time;
	int percent;
	int i;

	sprintf(buf, "Loading %s.", ch->name);
	log_string(buf);

	ch->pcdata->newbie = FALSE;
	ch->pcdata->sitetrack = NULL;

	for (;;)
	{
		word = feof(fp) ? (char *)"End" : fread_word(fp);
		fMatch = FALSE;

		switch (UPPER(word[0]))
		{
		case '*':
			fMatch = TRUE;
			fread_to_eol(fp);
			break;
		case 'A':
			KEY("Act", ch->act, fread_flag(fp));
			KEY("Agemod", ch->pcdata->age_mod, fread_number(fp));
			KEY("Alignment", ch->alignment, fread_number(fp));
			KEY("Alig", ch->alignment, fread_number(fp));
			KEY("AfBy", ch->affected_by, fread_flag(fp));

			if (!str_cmp(word, "Alia"))
			{
				if (count >= MAX_ALIAS)
				{
					fread_to_eol(fp);
					fMatch = TRUE;
					break;
				}

				ch->pcdata->alias[count] = str_dup(fread_word(fp));
				ch->pcdata->alias_sub[count] = str_dup(fread_word(fp));
				count++;
				fMatch = TRUE;
				break;
			}

			if (!str_cmp(word, "Alias"))
			{
				if (count >= MAX_ALIAS)
				{
					fread_to_eol(fp);
					fMatch = TRUE;
					break;
				}

				ch->pcdata->alias[count] = str_dup(fread_word(fp));
				ch->pcdata->alias_sub[count] = fread_string(fp);
				count++;
				fMatch = TRUE;
				break;
			}

			if (!str_cmp(word, "AC") || !str_cmp(word, "Armor"))
			{
				fread_to_eol(fp);
				fMatch = TRUE;
				break;
			}

			if (!str_cmp(word, "ACs"))
			{
				int i;

				for (i = 0; i < 4; i++)
					ch->armor[i] = fread_number(fp);
				fMatch = TRUE;
				break;
			}

			if (!str_cmp(word, "AffD"))
			{
				AFFECT_DATA *paf;
				int sn;

				paf = new_affect();

				sn = skill_lookup(fread_word(fp));
				if (sn < 0)
					bug("Fread_char: unknown skill.", 0);
				else
					paf->type = sn;

				paf->level = fread_number(fp);
				paf->duration = fread_number(fp);
				paf->modifier = fread_number(fp);
				paf->location = fread_number(fp);
				paf->bitvector = fread_number(fp);
				paf->aftype = fread_number(fp);
				paf->next = ch->affected;
				ch->affected = paf;
				fMatch = TRUE;
				break;
			}

			if (!str_cmp(word, "Affc"))
			{
				AFFECT_DATA *paf;
				int sn;

				paf = new_affect();
				init_affect(paf);

				sn = skill_lookup(fread_word(fp));
				if (sn < 0)
					bug("Fread_char: unknown skill.", 0);
				else
					paf->type = sn;

				if (ch->version >= 13)
				{
					char owner_name[MSL];
					sprintf(owner_name, "%s", fread_string(fp));

					if (strcmp(owner_name, "(null)"))
					{
						CHAR_DATA *vch;

						// Check if any characters logged in match the name, assign them as owner. Flawed
						// in that player can delete and change their name and be assigned what is technically
						// another pfile's ownership of an affect
						if ((vch = find_char_by_name(owner_name)) != NULL)
							paf->owner_name = str_dup(vch->original_name);
					}
				}
				if (ch->version >= 11)
				{
					paf->name = fread_string(fp);
					paf->affect_list_msg = fread_string(fp);
					if (!str_cmp(paf->affect_list_msg, "(null)"))
						paf->affect_list_msg = NULL;
					paf->dispel_at_death = fread_letter(fp);
					if (fread_letter(fp) == 'F')
						paf->strippable = FALSE;
					else
						paf->strippable = TRUE;
				}

				if (ch->version == 10)
				{
					paf->name = fread_string(fp);
					paf->affect_list_msg = fread_string(fp);
					if (!str_cmp(paf->affect_list_msg, "(null)"))
						paf->affect_list_msg = NULL;
					paf->dispel_at_death = fread_letter(fp);
				}
				if (ch->version == 9)
				{
					paf->name = fread_string(fp);
					paf->affect_list_msg = fread_string(fp);
					if (!str_cmp(paf->affect_list_msg, "(null)"))
						paf->affect_list_msg = NULL;
				}
				if (ch->version == 8)
				{
					paf->name = fread_word(fp);
					paf->affect_list_msg = fread_word(fp);
				}
				paf->where = fread_number(fp);
				paf->level = fread_number(fp);
				paf->duration = fread_number(fp);
				paf->modifier = fread_number(fp);
				paf->location = fread_number(fp);
				paf->bitvector = fread_number(fp);
				paf->aftype = fread_number(fp);
				paf->next = ch->affected;
				ch->affected = paf;
				fMatch = TRUE;
				break;
			}

			if (!str_cmp(word, "AttrMod") || !str_cmp(word, "AMod"))
			{
				int stat;
				for (stat = 0; stat < MAX_STATS; stat++)
					ch->mod_stat[stat] = fread_number(fp);
				fMatch = TRUE;
				break;
			}
			if (!str_cmp(word, "Attack_Mod"))
			{
				ch->base_numAttacks = fread_number(fp);
				ch->numAttacks = ch->base_numAttacks;
				fMatch = TRUE;
			}

			if (!str_cmp(word, "AttrPerm") || !str_cmp(word, "Attr"))
			{
				int stat;
				for (stat = 0; stat < MAX_STATS; stat++)
				{
					ch->perm_stat[stat] = fread_number(fp);
				}
				fMatch = TRUE;
				break;
			}
			break;

		case 'B':
			KEY("Bgold", ch->gold_bank, fread_number(fp));
			KEY("Bamfin", ch->pcdata->bamfin, fread_string(fp));
			KEY("Bamfout", ch->pcdata->bamfout, fread_string(fp));
			KEY("Bin", ch->pcdata->bamfin, fread_string(fp));
			KEY("Bout", ch->pcdata->bamfout, fread_string(fp));
			KEY("Bounty", ch->pcdata->bounty, fread_number(fp));
			KEY("BCredits", ch->pcdata->bounty_credits, fread_number(fp));
			break;

		case 'C':
			KEY("Class", ch->class, fread_number(fp));
			KEY("Cla", ch->class, fread_number(fp));
			KEY("Cabal", ch->cabal, cabal_lookup(fread_string(fp)));
			if (!str_cmp(word, "Condition") || !str_cmp(word, "Cond"))
			{
				ch->pcdata->condition[0] = fread_number(fp);
				ch->pcdata->condition[1] = fread_number(fp);
				ch->pcdata->condition[2] = fread_number(fp);
				ch->pcdata->condition[3] = fread_number(fp);
				ch->pcdata->condition[4] = fread_number(fp);
				ch->pcdata->condition[5] = fread_number(fp);
				fMatch = TRUE;
				break;
			}
			if (!str_cmp(word, "Cnd"))
			{
				ch->pcdata->condition[0] = fread_number(fp);
				ch->pcdata->condition[1] = fread_number(fp);
				ch->pcdata->condition[2] = fread_number(fp);
				ch->pcdata->condition[3] = fread_number(fp);
				ch->pcdata->condition[4] = fread_number(fp);
				ch->pcdata->condition[5] = fread_number(fp);
				fMatch = TRUE;
				break;
			}
			KEY("Comm", ch->comm, fread_flag(fp));

			break;

		case 'D':
			KEY("Deaths", ch->pcdata->death_count, fread_number(fp));
			KEY("Died", ch->pcdata->death_status, fread_number(fp));
			KEY("Dtime", ch->pcdata->death_timer, fread_number(fp));
			KEY("Damroll", ch->damroll, fread_number(fp));
			KEY("Dam", ch->damroll, fread_number(fp));
			KEY("Description", ch->description, fread_string(fp));
			KEY("Desc", ch->description, fread_string(fp));
			KEY("Dedication", ch->pcdata->dedication, fread_number(fp));
			if (!str_cmp(word, "Dam_mod"))
			{
				ch->base_dam_mod = fread_number(fp);
				ch->dam_mod = ch->base_dam_mod;
			}
			break;

		case 'E':
			if (!str_cmp(word, "EXTitle") || !str_cmp(word, "EXTitl"))
			{
				ch->pcdata->extitle = fread_string(fp);
				if (ch->pcdata->extitle[0] != '.' && ch->pcdata->extitle[0] != ',' && ch->pcdata->extitle[0] != '!' && ch->pcdata->extitle[0] != '?')
				{
					sprintf(buf, " %s", ch->pcdata->extitle);
					free_string(ch->pcdata->extitle);
					ch->pcdata->extitle = str_dup(buf);
				}
				if (ch->pcdata->extitle[0] == '\0')
					free_string(ch->pcdata->extitle);
				fMatch = TRUE;
				break;
			}

			if (!str_cmp(word, "End"))
			{
				/* adjust hp mana move up  -- here for speed's sake */
				percent = (current_time - lastlogoff) * 25 / (2 * 60 * 60);

				percent = UMIN(percent, 100);

				if (percent > 0 && !IS_AFFECTED(ch, AFF_POISON) && !IS_AFFECTED(ch, AFF_PLAGUE))
				{
					ch->hit += (ch->max_hit - ch->hit) * percent / 100;
					ch->mana += (ch->max_mana - ch->mana) * percent / 100;
					ch->move += (ch->max_move - ch->move) * percent / 100;
				}
				return;
			}
			KEY("Empr", ch->pcdata->empire, fread_number(fp));
			KEY("Etho", ch->pcdata->ethos, fread_number(fp));
			KEY("Exp", ch->exp, fread_number(fp));
			KEY("ExpT", ch->exp_total, fread_number(fp));
			if (!str_cmp(word, "EnhancedDamMod"))
			{
				ch->base_enhancedDamMod = fread_number(fp);
				ch->enhancedDamMod = ch->base_enhancedDamMod;
				fMatch = TRUE;
			}
			break;

		case 'G':
			KEY("Gold", ch->gold, fread_number(fp));
			if (!str_cmp(word, "Group") || !str_cmp(word, "Gr"))
			{
				int gn;
				char *temp;

				temp = fread_word(fp);
				gn = group_lookup(temp);
				/* gn    = group_lookup( fread_word( fp ) ); */
				if (gn < 0)
				{
					fprintf(stderr, "%s", temp);
					bug("Fread_char: unknown group. ", 0);
				}
				else
					gn_add(ch, gn);
				fMatch = TRUE;
			}
			break;

		case 'H':
			KEY("History", ch->pcdata->history_buffer, fread_string(fp));
			KEY("Hitroll", ch->hitroll, fread_number(fp));
			KEY("Hit", ch->hitroll, fread_number(fp));
			KEY("HomeTown", ch->hometown, fread_number(fp));

			ch->pcdata->temp_history_buffer = NULL;
			if (!str_cmp(word, "HpManaMove") || !str_cmp(word, "HMV"))
			{
				ch->hit = fread_number(fp);
				ch->max_hit = fread_number(fp);
				ch->mana = fread_number(fp);
				ch->max_mana = fread_number(fp);
				ch->move = fread_number(fp);
				ch->max_move = fread_number(fp);
				fMatch = TRUE;
				break;
			}

			if (!str_cmp(word, "HpManaMovePerm") || !str_cmp(word, "HMVP"))
			{
				ch->pcdata->perm_hit = fread_number(fp);
				ch->pcdata->perm_mana = fread_number(fp);
				ch->pcdata->perm_move = fread_number(fp);
				fMatch = TRUE;
				break;
			}

			break;

		case 'I':
			KEY("Id", ch->id, fread_number(fp));
			KEY("Indu", ch->pcdata->induct, fread_number(fp));
			KEY("InvisLevel", ch->invis_level, fread_number(fp));
			KEY("Inco", ch->incog_level, fread_number(fp));
			KEY("Invi", ch->invis_level, fread_number(fp));

			if (!str_cmp(word, "IsCloaked"))
			{
				ch->cloaked = fread_number(fp);
				fMatch = TRUE;
				break;
			}

			if (!str_cmp(word, "IMMUNE") && ch->version >= 12)
			{
				int immLooper;
				for (immLooper = 1; immLooper <= 24; immLooper++)
				{
					if (fread_number(fp) == 1)
					{
						if (immLooper == 1)
							SET_BIT(ch->imm_flags, IMM_SUMMON);
						else if (immLooper == 2)
							SET_BIT(ch->imm_flags, IMM_CHARM);
						else if (immLooper == 3)
							SET_BIT(ch->imm_flags, IMM_MAGIC);
						else if (immLooper == 4)
							SET_BIT(ch->imm_flags, IMM_WEAPON);
						else if (immLooper == 5)
							SET_BIT(ch->imm_flags, IMM_BASH);
						else if (immLooper == 6)
							SET_BIT(ch->imm_flags, IMM_PIERCE);
						else if (immLooper == 7)
							SET_BIT(ch->imm_flags, IMM_SLASH);
						else if (immLooper == 8)
							SET_BIT(ch->imm_flags, IMM_FIRE);
						else if (immLooper == 9)
							SET_BIT(ch->imm_flags, IMM_COLD);
						else if (immLooper == 10)
							SET_BIT(ch->imm_flags, IMM_LIGHTNING);
						else if (immLooper == 11)
							SET_BIT(ch->imm_flags, IMM_ACID);
						else if (immLooper == 12)
							SET_BIT(ch->imm_flags, IMM_POISON);
						else if (immLooper == 13)
							SET_BIT(ch->imm_flags, IMM_NEGATIVE);
						else if (immLooper == 14)
							SET_BIT(ch->imm_flags, IMM_HOLY);
						else if (immLooper == 15)
							SET_BIT(ch->imm_flags, IMM_ENERGY);
						else if (immLooper == 16)
							SET_BIT(ch->imm_flags, IMM_MENTAL);
						else if (immLooper == 17)
							SET_BIT(ch->imm_flags, IMM_DISEASE);
						else if (immLooper == 18)
							SET_BIT(ch->imm_flags, IMM_DROWNING);
						else if (immLooper == 19)
							SET_BIT(ch->imm_flags, IMM_LIGHT);
						else if (immLooper == 20)
							SET_BIT(ch->imm_flags, IMM_SOUND);
						else if (immLooper == 21)
							SET_BIT(ch->imm_flags, IMM_WOOD);
						else if (immLooper == 22)
							SET_BIT(ch->imm_flags, IMM_SILVER);
						else if (immLooper == 23)
							SET_BIT(ch->imm_flags, IMM_IRON);
						else if (immLooper == 24)
							SET_BIT(ch->imm_flags, IMM_SLEEP);
					}
				}
				fMatch = TRUE;
			}

			break;

		case 'K':
			if (!str_cmp(word, "kls"))
			{
				ch->pcdata->kills[PK_KILLS] = fread_number(fp);
				ch->pcdata->kills[PK_GOOD] = fread_number(fp);
				ch->pcdata->kills[PK_NEUTRAL] = fread_number(fp);
				ch->pcdata->kills[PK_EVIL] = fread_number(fp);
				fMatch = TRUE;
			}
			if (!str_cmp(word, "kld"))
			{
				ch->pcdata->killed[PK_KILLED] = fread_number(fp);
				ch->pcdata->killed[MOB_KILLED] = fread_number(fp);
				fMatch = TRUE;
			}
			break;

		case 'L':
			KEY("Lines", ch->lines, fread_number(fp));
			KEY("LastLevel", ch->pcdata->last_level, fread_number(fp));
			KEY("LLev", ch->pcdata->last_level, fread_number(fp));
			KEY("Level", ch->level, fread_number(fp));
			KEY("Lev", ch->level, fread_number(fp));
			KEY("Levl", ch->level, fread_number(fp));
			KEY("LogO", lastlogoff, fread_number(fp));
			KEY("LogonTime", ch->pcdata->logon_time, fread_string(fp));
			KEY("LongDescr", ch->long_descr, fread_string(fp));
			KEY("LnD", ch->long_descr, fread_string(fp));
			break;

		case 'N':
			KEYS("Name", ch->name, fread_string(fp));
			KEY("Noregen", ch->noregen_dam, fread_number(fp));
			KEY("Newbie", ch->pcdata->newbie, fread_number(fp));
			if (!str_cmp(word, "Not"))
			{
				ch->pcdata->last_note = fread_number(fp);
				ch->pcdata->last_idea = fread_number(fp);
				ch->pcdata->last_penalty = fread_number(fp);
				ch->pcdata->last_news = fread_number(fp);
				ch->pcdata->last_changes = fread_number(fp);
				fMatch = TRUE;
				break;
			}
			break;

		case 'O':
			KEY("OAli", ch->pcdata->oalign, fread_number(fp));
			KEY("OEth", ch->pcdata->oethos, fread_number(fp));
			break;

		case 'P':
			KEY("Password", ch->pcdata->pwd, fread_string(fp));
			KEY("Pass", ch->pcdata->pwd, fread_string(fp));
			KEY("Played", ch->played, fread_number(fp));
			KEY("Plyd", ch->played, fread_number(fp));
			KEY("Position", ch->position, fread_number(fp));
			KEY("Pos", ch->position, fread_number(fp));
			KEY("Practice", ch->practice, fread_number(fp));
			KEY("Prac", ch->practice, fread_number(fp));
			KEYS("Prompt", ch->prompt, fread_string(fp));
			KEY("Prom", ch->prompt, fread_string(fp));
			break;

		case 'Q':
			KEY("QuestCredits", ch->quest_credits, fread_number(fp));

		case 'R':
			KEY("Race", ch->race,
				race_lookup(fread_string(fp)));

			if (!str_cmp(word, "Regen"))
			{
				ch->base_regen = fread_number(fp);
				ch->regen_rate = ch->base_regen;
				fMatch = TRUE;
				break;
			}

			if (!str_cmp(word, "Room"))
			{
				ch->in_room = get_room_index(fread_number(fp));
				if (ch->in_room == NULL)
					ch->in_room = get_room_index(ROOM_VNUM_LIMBO);
				fMatch = TRUE;
				break;
			}

			if (!str_cmp(word, "RESIST") && ch->version >= 12)
			{
				int resLooper;
				for (resLooper = 1; resLooper <= 23; resLooper++)
				{
					if (fread_number(fp) == 1)
					{
						if (resLooper == 1)
							SET_BIT(ch->res_flags, RES_SUMMON);
						else if (resLooper == 2)
							SET_BIT(ch->res_flags, RES_CHARM);
						else if (resLooper == 3)
							SET_BIT(ch->res_flags, RES_MAGIC);
						else if (resLooper == 4)
							SET_BIT(ch->res_flags, RES_WEAPON);
						else if (resLooper == 5)
							SET_BIT(ch->res_flags, RES_BASH);
						else if (resLooper == 6)
							SET_BIT(ch->res_flags, RES_PIERCE);
						else if (resLooper == 7)
							SET_BIT(ch->res_flags, RES_SLASH);
						else if (resLooper == 8)
							SET_BIT(ch->res_flags, RES_FIRE);
						else if (resLooper == 9)
							SET_BIT(ch->res_flags, RES_COLD);
						else if (resLooper == 10)
							SET_BIT(ch->res_flags, RES_LIGHTNING);
						else if (resLooper == 11)
							SET_BIT(ch->res_flags, RES_ACID);
						else if (resLooper == 12)
							SET_BIT(ch->res_flags, RES_POISON);
						else if (resLooper == 13)
							SET_BIT(ch->res_flags, RES_NEGATIVE);
						else if (resLooper == 14)
							SET_BIT(ch->res_flags, RES_HOLY);
						else if (resLooper == 15)
							SET_BIT(ch->res_flags, RES_ENERGY);
						else if (resLooper == 16)
							SET_BIT(ch->res_flags, RES_MENTAL);
						else if (resLooper == 17)
							SET_BIT(ch->res_flags, RES_DISEASE);
						else if (resLooper == 18)
							SET_BIT(ch->res_flags, RES_DROWNING);
						else if (resLooper == 19)
							SET_BIT(ch->res_flags, RES_LIGHT);
						else if (resLooper == 20)
							SET_BIT(ch->res_flags, RES_SOUND);
						else if (resLooper == 21)
							SET_BIT(ch->res_flags, RES_WOOD);
						else if (resLooper == 22)
							SET_BIT(ch->res_flags, RES_SILVER);
						else if (resLooper == 23)
							SET_BIT(ch->res_flags, RES_IRON);
					}
				}
				fMatch = TRUE;
			}

			break;

		case 'S':
			KEY("SavingThrow", ch->saving_throw, fread_number(fp));
			KEY("Save", ch->saving_throw, fread_number(fp));
			KEY("Scro", ch->lines, fread_number(fp));
			KEY("Sex", ch->sex, fread_number(fp));
			KEY("ShortDescr", ch->short_descr, fread_string(fp));
			KEY("ShD", ch->short_descr, fread_string(fp));
			KEY("Sec", ch->pcdata->security, fread_number(fp));
			KEY("SiteTrack", ch->pcdata->sitetrack, fread_string(fp));
			KEY("Smite", ch->pcdata->smite, fread_string(fp));
			KEY("Sp", ch->pcdata->sp, fread_number(fp));
			KEY("Specialize", ch->pcdata->special, fread_number(fp));

			for (i = 0; i <= MAX_SPECS; i++)
			{
				sprintf(buf, "Spec%d", i);
				KEY(buf, ch->pcdata->warrior_specs[i], fread_number(fp));
				if ((ch->pcdata->warrior_specs[i] < 0) || (ch->pcdata->warrior_specs[i] > MAX_WEAPON))
				{
					ch->pcdata->warrior_specs[i] = 0;
				}
			}

			if (!str_cmp(word, "Skill") || !str_cmp(word, "Sk"))
			{
				int sn;
				int value;
				char *temp;

				value = fread_number(fp);
				temp = fread_word(fp);
				sn = skill_lookup(temp);
				/* sn    = skill_lookup( fread_word( fp ) ); */
				if (sn < 0)
				{
					fprintf(stderr, "%s", temp);
					bug("Fread_char: unknown skill. ", 0);
				}
				else
					ch->pcdata->learned[sn] = value;
				fMatch = TRUE;
			}

			break;

		case 'T':
			KEY("Temple", ch->temple, fread_number(fp));
			KEY("TrueSex", ch->pcdata->true_sex, fread_number(fp));
			KEY("TSex", ch->pcdata->true_sex, fread_number(fp));
			KEY("Trai", ch->train, fread_number(fp));
			KEY("Trust", ch->trust, fread_number(fp));
			KEY("Tru", ch->trust, fread_number(fp));

			if (!str_cmp(word, "Title") || !str_cmp(word, "Titl"))
			{
				ch->pcdata->title = fread_string(fp);
				if (ch->pcdata->title[0] != '.' && ch->pcdata->title[0] != ',' && ch->pcdata->title[0] != '!' && ch->pcdata->title[0] != '?')
				{
					sprintf(buf, " %s", ch->pcdata->title);
					free_string(ch->pcdata->title);
					ch->pcdata->title = str_dup(buf);
				}
				fMatch = TRUE;
				break;
			}

			break;

		case 'V':
			KEY("Version", ch->version, fread_number(fp));
			KEY("Vers", ch->version, fread_number(fp));
			if (!str_cmp(word, "Vnum"))
			{
				ch->pIndexData = get_mob_index(fread_number(fp));
				fMatch = TRUE;
				break;
			}
			if (!str_cmp(word, "VULN") && ch->version >= 12)
			{
				int vulnLooper;
				for (vulnLooper = 1; vulnLooper <= 23; vulnLooper++)
				{
					if (fread_number(fp) == 1)
					{
						if (vulnLooper == 1)
							SET_BIT(ch->vuln_flags, VULN_SUMMON);
						else if (vulnLooper == 2)
							SET_BIT(ch->vuln_flags, VULN_CHARM);
						else if (vulnLooper == 3)
							SET_BIT(ch->vuln_flags, VULN_MAGIC);
						else if (vulnLooper == 4)
							SET_BIT(ch->vuln_flags, VULN_WEAPON);
						else if (vulnLooper == 5)
							SET_BIT(ch->vuln_flags, VULN_BASH);
						else if (vulnLooper == 6)
							SET_BIT(ch->vuln_flags, VULN_PIERCE);
						else if (vulnLooper == 7)
							SET_BIT(ch->vuln_flags, VULN_SLASH);
						else if (vulnLooper == 8)
							SET_BIT(ch->vuln_flags, VULN_FIRE);
						else if (vulnLooper == 9)
							SET_BIT(ch->vuln_flags, VULN_COLD);
						else if (vulnLooper == 10)
							SET_BIT(ch->vuln_flags, VULN_LIGHTNING);
						else if (vulnLooper == 11)
							SET_BIT(ch->vuln_flags, VULN_ACID);
						else if (vulnLooper == 12)
							SET_BIT(ch->vuln_flags, VULN_POISON);
						else if (vulnLooper == 13)
							SET_BIT(ch->vuln_flags, VULN_NEGATIVE);
						else if (vulnLooper == 14)
							SET_BIT(ch->vuln_flags, VULN_HOLY);
						else if (vulnLooper == 15)
							SET_BIT(ch->vuln_flags, VULN_ENERGY);
						else if (vulnLooper == 16)
							SET_BIT(ch->vuln_flags, VULN_MENTAL);
						else if (vulnLooper == 17)
							SET_BIT(ch->vuln_flags, VULN_DISEASE);
						else if (vulnLooper == 18)
							SET_BIT(ch->vuln_flags, VULN_DROWNING);
						else if (vulnLooper == 19)
							SET_BIT(ch->vuln_flags, VULN_LIGHT);
						else if (vulnLooper == 20)
							SET_BIT(ch->vuln_flags, VULN_SOUND);
						else if (vulnLooper == 21)
							SET_BIT(ch->vuln_flags, VULN_WOOD);
						else if (vulnLooper == 22)
							SET_BIT(ch->vuln_flags, VULN_SILVER);
						else if (vulnLooper == 23)
							SET_BIT(ch->vuln_flags, VULN_IRON);
					}
				}
				fMatch = TRUE;
			}

			break;

		case 'W':
			KEY("Wimpy", ch->wimpy, fread_number(fp));
			KEY("Wimp", ch->wimpy, fread_number(fp));
			KEY("Wizn", ch->wiznet, fread_flag(fp));
			KEY("Wantd", ch->pcdata->wanteds, fread_number(fp));
			break;
		}

		if (!fMatch)
		{
			// char temp[MSL];
			// sprintf(temp, "Fread_char: unable to match \"%s\"", word);
			// bug(temp, 0);
			fread_to_eol(fp);
		}
	}
}

extern OBJ_DATA *obj_free;

// Reads objects from pfiles
void fread_obj(CHAR_DATA *ch, FILE *fp)
{

	OBJ_DATA *obj;
	char *word;
	int iNest;
	bool fMatch;
	bool fNest;
	bool fVnum;
	bool first;
	bool new_format; /* to prevent errors */
	bool make_new;	 /* update object */

	fVnum = FALSE;
	obj = NULL;
	first = TRUE; /* used to counter fp offset */
	new_format = FALSE;
	make_new = FALSE;

	word = feof(fp) ? "End" : fread_word(fp);
	if (!str_cmp(word, "Vnum"))
	{
		long vnum;
		first = FALSE; /* fp will be in right place */

		vnum = fread_number(fp);
		if (get_obj_index(vnum) == NULL)
		{
			bug("Fread_obj: bad vnum %d.", vnum);
		}
		else
		{
			obj = create_object(get_obj_index(vnum), -1);
			obj->pIndexData->limcount -= 1;
			new_format = TRUE;
		}
	}

	if (obj == NULL) /* either not found or old style */
	{
		obj = new_obj();
		obj->name = str_dup("");
		obj->short_descr = str_dup("");
		obj->description = str_dup("");
	}

	fNest = FALSE;
	fVnum = TRUE;
	iNest = 0;

	for (;;)
	{
		if (first)
			first = FALSE;
		else
			word = feof(fp) ? "End" : fread_word(fp);
		fMatch = FALSE;

		switch (UPPER(word[0]))
		{
		case '*':
			fMatch = TRUE;
			fread_to_eol(fp);
			break;

		case 'A':
			if (!str_cmp(word, "AffD"))
			{
				AFFECT_DATA *paf;
				int sn;

				paf = new_affect();

				sn = skill_lookup(fread_word(fp));
				if (sn < 0)
					bug("Fread_obj: unknown skill.", 0);
				else
					paf->type = sn;

				paf->level = fread_number(fp);
				paf->duration = fread_number(fp);
				paf->modifier = fread_number(fp);
				paf->location = fread_number(fp);
				paf->bitvector = fread_number(fp);
				paf->next = obj->affected;
				obj->affected = paf;
				fMatch = TRUE;
				break;
			}
			if (!str_cmp(word, "Affc"))
			{
				AFFECT_DATA *paf;
				int sn;

				paf = new_affect();

				sn = skill_lookup(fread_word(fp));
				if (sn < 0)
					bug("Fread_obj: unknown skill.", 0);
				else
					paf->type = sn;

				if (ch->version >= 14)
					paf->owner_name = fread_string(fp);
				paf->where = fread_number(fp);
				paf->level = fread_number(fp);
				paf->duration = fread_number(fp);
				paf->modifier = fread_number(fp);
				paf->location = fread_number(fp);
				paf->bitvector = fread_number(fp);
				paf->aftype = fread_number(fp);
				paf->next = obj->affected;
				obj->affected = paf;
				fMatch = TRUE;
				break;
			}
			break;

		case 'C':
			KEY("Cond", obj->condition, fread_number(fp));
			KEY("Cost", obj->cost, fread_number(fp));
			break;

		case 'D':
			KEY("Description", obj->description, fread_string(fp));
			KEY("Desc", obj->description, fread_string(fp));
			break;

		case 'E':
			if (!str_cmp(word, "Enchanted"))
			{
				obj->enchanted = TRUE;
				fMatch = TRUE;
				break;
			}

			if (!str_cmp(word, "ExtF"))
			{
				fMatch = TRUE;
				load_bitmask(&obj->extra_flags, fp);
				break;
			}

			if (!str_cmp(word, "ExtraDescr") || !str_cmp(word, "ExDe"))
			{
				EXTRA_DESCR_DATA *ed;

				ed = new_extra_descr();

				ed->keyword = fread_string(fp);
				ed->description = fread_string(fp);
				ed->next = obj->extra_descr;
				obj->extra_descr = ed;
				fMatch = TRUE;
			}

			if (!str_cmp(word, "End"))
			{
				if (!fNest || (fVnum && obj->pIndexData == NULL))
				{
					bug("Fread_obj: incomplete object.", 0);
					free_obj(obj);
					return;
				}
				else
				{
					if (!fVnum)
					{
						free_obj(obj);
						obj = create_object(get_obj_index(OBJ_VNUM_DUMMY), 0);
					}

					if (!new_format)
					{
						obj->next = object_list;
						object_list = obj;
						obj->pIndexData->count++;
					}

					if (!obj->pIndexData->new_format && obj->item_type == ITEM_ARMOR && obj->value[1] == 0)
					{
						obj->value[1] = obj->value[0];
						obj->value[2] = obj->value[0];
					}
					if (make_new)
					{
						int wear;

						wear = obj->wear_loc;
						extract_obj(obj);
						obj->pIndexData->limcount += 1;
						obj = create_object(obj->pIndexData, 0);
						obj->pIndexData->limcount -= 1;
						obj->wear_loc = wear;
					}
					if (iNest == 0 || rgObjNest[iNest] == NULL)
						obj_to_char(obj, ch);
					else
						obj_to_obj(obj, rgObjNest[iNest - 1]);
					return;
				}
			}
			break;

		case 'I':
			KEY("ItemType", obj->item_type, fread_number(fp));
			KEY("Ityp", obj->item_type, fread_number(fp));
			break;

		case 'L':
			KEY("Level", obj->level, fread_number(fp));
			KEY("Lev", obj->level, fread_number(fp));
			break;

		case 'N':
			KEY("Name", obj->name, fread_string(fp));

			if (!str_cmp(word, "Nest"))
			{
				iNest = fread_number(fp);
				if (iNest < 0 || iNest >= MAX_NEST)
				{
					bug("Fread_obj: bad nest %d.", iNest);
				}
				else
				{
					rgObjNest[iNest] = obj;
					fNest = TRUE;
				}
				fMatch = TRUE;
			}
			break;

		case 'O':
			KEY("Owner", obj->owner, fread_string(fp));

			if (!str_cmp(word, "Oldstyle"))
			{
				if (obj->pIndexData != NULL && obj->pIndexData->new_format)
					make_new = TRUE;
				fMatch = TRUE;
			}
			break;

		case 'S':
			KEY("ShortDescr", obj->short_descr, fread_string(fp));
			KEY("ShD", obj->short_descr, fread_string(fp));

			if (!str_cmp(word, "Spell"))
			{
				int iValue;
				int sn;

				iValue = fread_number(fp);
				sn = skill_lookup(fread_word(fp));
				if (iValue < 0 || iValue > 3)
				{
					bug("Fread_obj: bad iValue %d.", iValue);
				}
				else if (sn < 0)
				{
					bug("Fread_obj: unknown skill.", 0);
				}
				else
				{
					obj->value[iValue] = sn;
				}
				fMatch = TRUE;
				break;
			}

			break;

		case 'T':
			KEY("Timer", obj->timer, fread_number(fp));
			KEY("Time", obj->timer, fread_number(fp));
			break;

		case 'V':
			if (!str_cmp(word, "Values") || !str_cmp(word, "Vals"))
			{
				obj->value[0] = fread_number(fp);
				obj->value[1] = fread_number(fp);
				obj->value[2] = fread_number(fp);
				obj->value[3] = fread_number(fp);
				if (obj->item_type == ITEM_WEAPON && obj->value[0] == 0)
					obj->value[0] = obj->pIndexData->value[0];
				fMatch = TRUE;
				break;
			}

			if (!str_cmp(word, "Val"))
			{
				obj->value[0] = fread_number(fp);
				obj->value[1] = fread_number(fp);
				obj->value[2] = fread_number(fp);
				obj->value[3] = fread_number(fp);
				obj->value[4] = fread_number(fp);
				fMatch = TRUE;
				break;
			}

			if (!str_cmp(word, "Vnum"))
			{
				long vnum;

				vnum = fread_number(fp);
				if ((obj->pIndexData = get_obj_index(vnum)) == NULL)
					bug("Fread_obj: bad vnum %d.", vnum);
				else
					fVnum = TRUE;
				fMatch = TRUE;
				break;
			}
			break;

		case 'W':
			KEY("WearFlags", obj->wear_flags, fread_number(fp));
			KEY("WeaF", obj->wear_flags, fread_number(fp));
			KEY("WearLoc", obj->wear_loc, fread_number(fp));
			KEY("Wear", obj->wear_loc, fread_number(fp));
			KEY("Weight", obj->weight, fread_number(fp));
			KEY("Wt", obj->weight, fread_number(fp));
			break;
		}

		if (!fMatch)
		{
			n_logf("Fread_obj: no match %s (%ld)", word, obj ? obj->pIndexData->vnum : -1);
			// bug("Fread_obj: no match.", 0);
			fread_to_eol(fp);
		}
	}
}
