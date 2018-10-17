/****************************************************************************
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
*	In using this code you agree to comply with the Tartarus license   *
*       found in the file /Tartarus/doc/tartarus.doc                       *
***************************************************************************/
#include "include.h"

#define HITS (dt==gsn_kick||dt==gsn_lunge||dt==gsn_downstrike||dt==gsn_throw||dt==gsn_crush||dt==gsn_bash||dt==gsn_restrike)

/*
 * Local functions.
 */
void 	check_downstrike 	args( ( CHAR_DATA *ch, CHAR_DATA *victim) );
void 	check_assist 		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void	affect_join_obj		args(	(OBJ_DATA *obj, AFFECT_DATA *af)	);
bool 	check_dodge 		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
bool 	check_questdodge 	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
bool	check_fam		args( ( CHAR_DATA *ch, char *attack	)	);
bool 	check_block 		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
void 	check_killer 		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool 	check_parry 		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
bool 	check_spin 		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
bool 	check_displacement 	args( ( CHAR_DATA *ch, CHAR_DATA *victim) );
bool 	check_shield_block 	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
void 	dam_message 		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam,int dt, bool immune, char *dnoun, bool isPlural, bool showCharName) );
void    death_cry       	args( ( CHAR_DATA *ch ) );
void    group_gain      	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
long	quest_credit_compute	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int group_amount ) );
int     xp_compute      	args( ( CHAR_DATA *gch, CHAR_DATA *victim,int group_amount, int glevel ) );
bool    is_safe         	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void    make_corpse     	args( ( CHAR_DATA *killer, CHAR_DATA *ch ) );
void    one_hit         	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ));
void    mob_hit         	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
void    raw_kill        	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void    raw_kill_new		args( ( CHAR_DATA *ch, CHAR_DATA *victim, bool deathCry, bool fLook ) );
void    set_fighting    	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void    disarm          	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void 	check_ground_control 	args( ( CHAR_DATA *ch,CHAR_DATA *victim,int chance,int dam) );
void 	check_follow_through	args( ( CHAR_DATA *ch, CHAR_DATA *victim,int dam) );
bool 	check_roll		args( ( CHAR_DATA *ch,CHAR_DATA *victim,int dt) );
void 	do_strap 		args( ( CHAR_DATA *victim,OBJ_DATA *strap));
bool 	check_unholy_bless 	args( ( CHAR_DATA *ch, CHAR_DATA *victim));
void 	check_cheap_shot 	args( ( CHAR_DATA *ch, CHAR_DATA *victim));
void 	check_parting_blow 	args( ( CHAR_DATA *ch, CHAR_DATA *victim));
bool 	arena;
bool 	check_distance  	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ));
bool 	check_ironhands 	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ));
bool 	check_unarmed_defense 	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ));
char 	*get_dam_message 	args( ( CHAR_DATA *ch, int dt));
char 	* get_attack_noun 	args( ( CHAR_DATA *ch, int dt));
int 	get_attack_number 	args( ( CHAR_DATA *ch, int dt));
bool 	check_dispel		( int dis_level, CHAR_DATA *victim, int sn);
bool 	check_cutoff		( CHAR_DATA *ch, CHAR_DATA *victim);
void 	do_arena_echo		( const char *txt);
int 	check_imperial_training	( CHAR_DATA *ch);
bool 	check_wall_thorns 	args( ( CHAR_DATA *ch));
void 	one_hit_new 		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt, bool specials, bool blockable, int addition, int multiplier, char *dnoun));
bool 	damage_new 		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int dam_type, bool show, bool blockable, int addition, int multiplier, char *dnoun));
char *  weapon_name_lookup    args( ( int type) );

int 	con_multiplier		args( (int con, int dam) );
//Customizable dnoun, isPlural for singular or plural dverbs, showCharName (i.e. Zornath's blah or The blah), dealtByAffect for DoTs, keep_invisible to avoid revealing the ch
bool    damage_newer		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int dam_type, bool show, bool blockable, int addition, int multiplier, char *dnoun, bool isPlural, bool showCharName, AFFECT_DATA *dealtByAffect, bool keep_invisible ) );
int	damage_calculate	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt, int dam_type, bool show, bool blockable, int addition, int multiplier) );
bool    check_blademaster     	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
void 	check_restrike 		args( ( CHAR_DATA *ch, CHAR_DATA *victim));
bool 	check_ward_diminution	args( (CHAR_DATA *mage, CHAR_DATA *attacker, int dam, int type, int dt) );
bool	check_unholy_new	args( (CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj)	);
bool	is_affected_obj		args( (OBJ_DATA *obj, int sn)			);
void	check_paladin_combo	args( (CHAR_DATA *ch, CHAR_DATA *victim) );
float   hitroll_multiplier	args( (CHAR_DATA *ch, CHAR_DATA *victim, float max_multiplier) );
bool    check_shield_magnetism	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ));
void    check_critical_strike   args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ));

 /*
 * Control the fights going on.
 * Called periodically by update_handler.
 */
void violence_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *victim;
    int regen, to_heal;
    OBJ_DATA *obj;

    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
        ch_next = ch->next;

	/* regen */
	if (!IS_NPC(ch))
	{
	    regen = get_skill(ch,gsn_regeneration);
	    to_heal = (ch->max_hit - ch->hit);

	    if (number_percent() <=  ch->level*2)
	    {
		if ((ch->hit < ch->max_hit) && ch->noregen_dam < to_heal && number_percent() < regen)
		{
		    if (number_percent() < 3)
			check_improve(ch,gsn_regeneration,TRUE,4);
		    ch->hit += number_range(1,3);
		}
	    }
	}

	//////// Begin Lich Quaff Potion Time Interval Messages ////////
	if (is_affected(ch,skill_lookup("lich elixir affect")))
	{
		AFFECT_DATA *lichAffect = affect_find(ch->affected,skill_lookup("lich elixir affect"));
		char buf[MSL];

		if (lichAffect->level == 59)
		{
			if (lichAffect->modifier == 8)
			{
				act("{BYou feel the elixir eating at your internal organs.{x",ch,0,0,TO_CHAR);
				lichAffect->modifier--;
			}
			else if (lichAffect->modifier == 6)
			{
				act("{BThin stripes of blackened energy play around you.{x",ch,0,0,TO_CHAR);
				lichAffect->modifier--;
			}
			else if (lichAffect->modifier == 4)
			{
				act("{BYour limbs, muscles, and bones seem to grow, the pain of transformation crippling you.{x",ch,0,0,TO_CHAR);
				lichAffect->modifier--;
			}
			else if (lichAffect->modifier == 2)
			{
				act("{BChunks of skin fall from your body, and suddenly the pain is gone.",ch,0,0,TO_CHAR);
				affect_strip(ch,skill_lookup("lich elixir affect"));
				ch->pcdata->learned[skill_lookup("minion tactics")] = 1;
     				ch->pcdata->learned[skill_lookup("minion sanc")] =1;
     				ch->pcdata->learned[skill_lookup("minion flight")] =1;
     				ch->pcdata->learned[skill_lookup("minion sneak")] =1;
     				ch->pcdata->learned[skill_lookup("minion recall")] =1;
     				ch->pcdata->learned[skill_lookup("minion haste")] =1;
     				ch->pcdata->learned[skill_lookup("minion trans")] =1;
				ch->pcdata->learned[skill_lookup("ritual of abominations")] = 1;
				ch->pcdata->learned[skill_lookup("vampiric touch")] = 1;
    
     				ch->race = race_lookup("lich");

    				ch->perm_stat[STAT_STR]=pc_race_table[ch->race].max_stats[STAT_STR];
        			ch->perm_stat[STAT_INT]=pc_race_table[ch->race].max_stats[STAT_INT];
       				ch->perm_stat[STAT_WIS]=pc_race_table[ch->race].max_stats[STAT_WIS];
        			ch->perm_stat[STAT_DEX]=pc_race_table[ch->race].max_stats[STAT_DEX];
        			ch->perm_stat[STAT_CON]=pc_race_table[ch->race].max_stats[STAT_CON];
    				ch->mod_stat[STAT_STR]=0;
    				ch->mod_stat[STAT_INT]=0;
    				ch->mod_stat[STAT_WIS]=0;
    				ch->mod_stat[STAT_DEX]=0;
    				ch->mod_stat[STAT_CON]=0;
    				sprintf(buf, "{RThe power of the undead courses through your veins as your soul becomes one with the Prime Negative Plane.{x\n\r");
				send_to_char(buf,ch);
				ch->ghost += 10;
			}
			else
				lichAffect->modifier--;
		}

		else if (lichAffect->level == 60)
		{
			if (lichAffect->modifier == 8)
			{
				act("{BYou feel the elixir eating at your internal organs.{x",ch,0,0,TO_CHAR);
				lichAffect->modifier--;
			}
			else if (lichAffect->modifier == 6)
			{
				act("{BThin stripes of blackened energy play around you.{x",ch,0,0,TO_CHAR);
				lichAffect->modifier--;
			}
			else if (lichAffect->modifier == 4)
			{
				act("{BYour limbs, muscles, and bones seem to grow, the pain of transformation crippling you.{x",ch,0,0,TO_CHAR);
				lichAffect->modifier--;
			}
			else if (lichAffect->modifier == 2)
			{

				act("{RJust as quickly, your body falls to the ground, the power leaving you.{x",ch,0,0,TO_CHAR);
				affect_strip(ch,skill_lookup("lich elixir affect"));
				if (!IS_IMMORTAL(ch))
					age_death(ch);
			}
			else
				lichAffect->modifier--;
		}
	}
	//////// End Lich Quaff Potion Time Interval Messages ////////

	if (ch->regen_rate != 0)
	{
	    if (ch->regen_rate > 0)
	  	regen = number_range(1,ch->regen_rate);
	    else
		regen = number_range(ch->regen_rate, -1);
	
		if (ch->regen_rate > 0) {
		    ch->hit = UMIN(ch->hit + regen,ch->max_hit);
			ch->mana = UMIN(ch->mana + regen / 2, ch->max_mana);
			ch->move = UMIN(ch->move + regen / 2, ch->max_move);
		}
		else {
		    ch->hit = UMAX(ch->hit + regen,0);
			ch->mana = UMAX(ch->mana + regen / 2, 0);
			ch->move = UMAX(ch->move + regen / 2, 0);
	    }
	}

        if ( ( victim = ch->fighting ) == NULL || ch->in_room == NULL )
            continue;

	if (IS_NPC(ch) && (!IS_NPC(ch->fighting))
	&& !IS_SET(ch->act,ACT_NO_TRACK)
	&& !IS_AFFECTED(ch,AFF_CHARM)
	&& !IS_SET(ch->act,ACT_IS_HEALER)
	&& !IS_SET(ch->act,ACT_BANKER))
		ch->last_fought = ch->fighting;

	update_pc_last_fight(ch,ch->fighting);

        if ( IS_AWAKE(ch) && ch->in_room == victim->in_room )
		{
            multi_hit( ch, victim, TYPE_UNDEFINED );
		}
        else
		{
            stop_fighting( ch, FALSE );
		}

        if ( ( victim = ch->fighting ) == NULL )
            continue;

	for (obj = ch->carrying;obj != NULL; obj = obj->next_content)
 	   if (IS_SET(obj->progtypes,IPROG_FIGHT))
		(obj->pIndexData->iprogs->fight_prog) (obj,ch);

	if (IS_SET(ch->progtypes,MPROG_FIGHT) && (ch->wait <= 0))
	   	(ch->pIndexData->mprogs->fight_prog) (ch,victim);

        /*
         * Fun for the whole family!
         */
        check_assist(ch,victim);
    }

    return;
}

/* for auto assisting */
void check_assist(CHAR_DATA *ch,CHAR_DATA *victim)
{
    CHAR_DATA *rch, *rch_next;

    for (rch = ch->in_room->people; rch != NULL; rch = rch_next)
    {
        rch_next = rch->next_in_room;

	if (!IS_NPC(rch) && rch->ghost > 0)
		continue;

        if (IS_AWAKE(rch) && rch->fighting == NULL)
        {

	    if (IS_NPC(rch) && (IS_AFFECTED(rch,AFF_CHARM)) && is_same_group(rch,ch) )
 	    {
		multi_hit(rch,victim,TYPE_UNDEFINED);
		continue;
	    }


            /* quick check for ASSIST_PLAYER */
	    if (!IS_NPC(ch) && IS_NPC(rch) && IS_SET(rch->off_flags,ASSIST_PLAYERS) && rch->level + 6 > victim->level)
	    {
            	do_emote(rch,"screams and attacks!");
            	multi_hit(rch,victim,TYPE_UNDEFINED);
            	continue;
            }

	    /* PCs next */
            if ( !IS_NPC(ch) || IS_AFFECTED(ch,AFF_CHARM))
            {

            	if ( ( (!IS_NPC(rch) && IS_SET(rch->act,PLR_AUTOASSIST))
            	|| IS_AFFECTED(rch,AFF_CHARM))
            	&& is_same_group(ch,rch)
            	&& !is_safe(rch, victim))
            	    multi_hit (rch,victim,TYPE_UNDEFINED);
            	continue;
	    }

            /* now check the NPC cases */
            if (IS_NPC(ch) && !IS_AFFECTED(ch,AFF_CHARM))
	    {
            	if ( (IS_NPC(rch) && IS_SET(rch->off_flags,ASSIST_ALL) )
            	||   (IS_NPC(rch) && rch->group && rch->group == ch->group)
            	||   (IS_NPC(rch) && rch->race == ch->race && IS_SET(rch->off_flags,ASSIST_RACE))
            	||   (IS_NPC(rch) && IS_SET(rch->off_flags,ASSIST_ALIGN) 
            	&&   ((IS_GOOD(rch)    && IS_GOOD(ch))
            	||  (IS_EVIL(rch)    && IS_EVIL(ch))
            	||  (IS_NEUTRAL(rch) && IS_NEUTRAL(ch))))
            	||   (rch->pIndexData == ch->pIndexData && IS_SET(rch->off_flags,ASSIST_VNUM)))
            	{
         	    CHAR_DATA *vch;
                    CHAR_DATA *target;
		    int number;

                    if (number_bits(1) == 0)
               	    	continue;

               	    target = NULL;
                    number = 0;
                    for (vch = ch->in_room->people; vch; vch = vch->next)
                    {
            	    	if (can_see(rch,vch) && is_same_group(vch,victim) &&  number_range(0,number) == 0)
                    	{
                    	    target = vch;
                            number++;
                    	}
             	    }

                    if (target != NULL)
                    {
                    	do_emote(rch,"screams and attacks!");
                    	multi_hit(rch,target,TYPE_UNDEFINED);
                    }
            	}
            }
  	}
    }
}


/*
 * Do one group of attacks.
 */
void multi_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    int chance, dual_chance;
    char buf[MAX_STRING_LENGTH];
    bool candual = FALSE;
    
    // Can we get dual wield attacks?
    if ( get_eq_char(ch,WEAR_DUAL_WIELD) != NULL 
      || count_hands(ch) == 0
      || (get_eq_char(ch,WEAR_WIELD) != NULL && count_hands(ch) == 1) )
        candual = TRUE;

    if (is_affected(ch,gsn_timestop))
    {
	if (ch->fighting != NULL)
		ch->fighting = NULL;
	return;
    }

    if (ch->desc == NULL)
        ch->wait = UMAX(0,ch->wait - PULSE_VIOLENCE);

    if (ch->desc == NULL)
        ch->daze = UMAX(0,ch->daze - PULSE_VIOLENCE);

    if (ch->level == MAX_LEVEL)
	ch->daze = 0;

    if(ch->in_room!=victim->in_room)
    {
	ch->fighting		= NULL;
	victim->fighting	= NULL;
	ch->position		= POS_STANDING;
	victim->position	= POS_STANDING;
	update_pos(ch);
	update_pos(victim);
	return;
    }

    if(is_affected(victim,gsn_snare) && number_percent()<80 && victim->fighting!=NULL)
    {
	act("In the chaos of combat, the snare holding $n comes loose.",victim,0,0,TO_ROOM);
	act("In the chaos of combat, the snare holding you comes loose.",victim,0,0,TO_CHAR);
	affect_strip(victim,gsn_snare);
    }


    if (ch->position < POS_RESTING)
        return;

    if (IS_NPC(ch))
    {
        mob_hit(ch,victim,dt);
        return;
    }

    if (IS_NPC(victim) && IS_CABAL_GUARD(victim) && !IS_NPC(ch) && (ch->pcdata->newbie==TRUE || ch->ghost > 0))
    {
		int attempt, door;
    		for ( attempt = 0; attempt < 6; attempt++ )
    		{
			EXIT_DATA *pexit;
    			ROOM_INDEX_DATA *was_in;
			door = number_door( );
			was_in = ch->in_room;
			if ( ( pexit = was_in->exit[door] ) == 0 || pexit->u1.to_room == NULL || pexit->u1.to_room->cabal>0)
	    			continue;
			act("$N growls with rage and throws you out of the room!",ch,0,victim,TO_CHAR);
			act("$N growls with rage and throws $n out of the room!",ch,0,victim,TO_ROOM);
        		char_from_room(ch);
        		char_to_room(ch, pexit->u1.to_room);
			do_look(ch,"auto");
			ch->ghost = 1;
			stop_fighting( victim, TRUE );
			return;
		}
    }

    if (IS_NPC(victim) && IS_CABAL_GUARD(victim) && !IS_NPC(ch) && ch->cabal==victim->cabal)
    {
	sprintf(buf,"How DARE you attack me, %s!",ch->name);
	do_cb(victim,buf);
    }

    // First hit.
    one_hit( ch, victim, dt );

    if (ch->fighting != victim)
        return;

    if ( ch->fighting != victim || dt == gsn_backstab || (dt ==  gsn_ambush))
        return;

    if (IS_AFFECTED(ch,AFF_HASTE) && (IS_NPC(ch) || get_skill(ch,gsn_third_attack)>5))
        one_hit(ch,victim,dt);

    chance = get_skill(ch,gsn_second_attack)/2 + GET_HITROLL(ch) / 5;
    dual_chance = get_skill(ch,gsn_dual_wield)/2 + GET_HITROLL(ch) / 5;

    if (is_affected(ch,gsn_maehslin) && number_percent() < 20)
    {
	act("The amber aura around $n intenses and their movements become more fluid!",ch,0,victim,TO_VICT);
    	act("$n's amber aura intenses and their movements become more fluid!" ,ch,0,0,TO_NOTVICT);
    	act("Your amber aura intenses and your movements become more fluid!",ch,0,0,TO_CHAR);
	one_hit_new(ch,victim,dt,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
    }

    check_critical_strike(ch, victim, dt);

    if (check_darkshout(ch) == DARKSHOUT_MEPHISTOPHELES && number_percent() < 40)
    {
        damage_new(ch, victim, 250, TYPE_UNDEFINED, DAM_FIRE, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "infernal wrath");
    }

    if (is_affected(ch,gsn_shield_of_truth) && get_eq_char(ch,WEAR_SHIELD) != NULL)
    {
	int healingCheck = number_percent();
	
	if (healingCheck <= 65)
	{
		act("The power of your god flows through your body, revitalizing you.",ch,0,0,TO_CHAR);
		act("$n looks more at peace as his wounds seem to heal.",ch,0,0,TO_ROOM);
	}
	if (healingCheck < 45)
		(*skill_table[skill_lookup("cure serious")].spell_fun) (skill_lookup("cure serious"), 51, ch, ch, TAR_CHAR_DEFENSIVE);
	if (healingCheck >= 45 && healingCheck < 57)
		(*skill_table[skill_lookup("cure critical")].spell_fun) (skill_lookup("cure serious"), 51, ch, ch, TAR_CHAR_DEFENSIVE);
	if (healingCheck >= 57 && healingCheck <= 65)
		(*skill_table[skill_lookup("heal")].spell_fun) (skill_lookup("heal"), 51, ch, ch, TAR_CHAR_DEFENSIVE);
    }

    if (IS_AFFECTED(ch,AFF_HASTE))
    {
	chance*=1.25;
	dual_chance*=1.25;
    }

    if (IS_AFFECTED(ch,AFF_SLOW))
    {
      	chance /= 2;
	dual_chance /= 2;
    }

    if (check_imperial_training(ch) == IMPERIAL_DEFENSE)
    {
	chance /= 2.5;
	dual_chance /= 2.5;
    }

    if ( number_percent( ) < chance )
    {
        one_hit( ch, victim, dt );
        check_improve(ch,gsn_second_attack,TRUE,5);
        if ( ch->fighting != victim )
            return;
    }

    if (candual && ch->pcdata->learned[gsn_second_attack] > 10)
    {
	if (number_percent( ) < dual_chance)
	{
	    one_hit(ch,victim,gsn_dual_wield);
	    check_improve(ch,gsn_dual_wield,TRUE,3);
	    if (ch->fighting != victim)
		return;
	} else {
	    check_improve(ch,gsn_dual_wield,FALSE,3);
	}
    }

    chance = get_skill(ch,gsn_third_attack)/4 + GET_HITROLL(ch) / 10;

    if (IS_AFFECTED(ch,AFF_SLOW))
        chance = 0;
    if (IS_AFFECTED(ch,AFF_HASTE))
        chance*=1.3;
    if ( number_percent( ) < chance )
    {
        one_hit( ch, victim, dt );
        check_improve(ch,gsn_third_attack,TRUE,6);
        if ( ch->fighting != victim )
            return;
    }

    dual_chance = get_skill(ch,gsn_dual_wield)/4 + GET_HITROLL(ch) / 10;
    if (IS_AFFECTED(ch,AFF_HASTE))
        dual_chance*=1.2;

    if (candual && ch->pcdata->learned[gsn_third_attack] > 10)
    {
	if (number_percent( ) < dual_chance)
	{
	    one_hit(ch,victim,gsn_dual_wield);
	    check_improve(ch,gsn_dual_wield,TRUE,3);
	    if (ch->fighting != victim)
		return;
	} else {
	    check_improve(ch,gsn_dual_wield,FALSE,3);
	}
    }

    chance = get_skill(ch,gsn_fourth_attack)/5 + GET_HITROLL(ch) / 15;

    if (IS_AFFECTED(ch,AFF_SLOW))
	chance /= 2;
    if (IS_AFFECTED(ch,AFF_HASTE))
        chance*=1.3;
    if ( number_percent() < chance )
    {
	one_hit( ch, victim, dt );
	check_improve(ch,gsn_fourth_attack,TRUE,6);
	if (ch->fighting != victim )
	    return;
    }

    dual_chance = get_skill(ch,gsn_dual_wield)/4 + GET_HITROLL(ch) / 15;
    if (IS_AFFECTED(ch,AFF_HASTE))
        dual_chance*=1.2;
    if (candual && ch->pcdata->learned[gsn_fourth_attack] > 10)
    {
	if (number_percent( ) < dual_chance)
	{
	    one_hit(ch,victim,gsn_dual_wield);
	    check_improve(ch,gsn_dual_wield,TRUE,6);
	    if (ch->fighting != victim)
		return;
	} else {
	    check_improve(ch,gsn_dual_wield,FALSE,5);
	}
    }
/*  If we can already dual wield hands, this block makes no sense.
	
	int off_chance = 3 * get_skill(ch,skill_lookup("unarmed offense")) / 4;
	if (IS_AFFECTED(ch,AFF_HASTE)) off_chance = 3 * off_chance / 2;
	if (check_imperial_training(ch) == IMPERIAL_OFFENSE) off_chance = 5 * off_chance / 4;
	
	if (get_eq_char(ch,WEAR_WIELD) == NULL && get_eq_char(ch,WEAR_DUAL_WIELD) == NULL &&
		ch->pcdata->learned[skill_lookup("unarmed offense")] > 10) {
		if (number_percent() < off_chance) {
			one_hit(ch,victim,dt);
			check_improve(ch,skill_lookup("unarmed offense"),TRUE,3);
			if (number_percent() < off_chance / 2) {
				one_hit(ch,victim,dt);
				check_improve(ch,skill_lookup("unarmed offense"),TRUE,5);
			}
		}
		else check_improve(ch,skill_lookup("unarmed offense"),FALSE,3);
	}

    if (is_affected(victim,gsn_blackjack)) 
    {
	affect_strip(victim,gsn_blackjack);
	return;
    }

*/

    /* imperial training */
    if (check_imperial_training(ch) == IMPERIAL_OFFENSE && get_skill(ch,gsn_third_attack)>80)
	    one_hit(ch, victim, dt);

    int extraAttacks;
    if(ch->numAttacks > 0)
	for (extraAttacks = 1;extraAttacks<=ch->numAttacks;extraAttacks++)
		if (extraAttacks <= 20)
			one_hit(ch,victim,dt);

    return;
}

/* procedure for all mobile attacks */
void mob_hit (CHAR_DATA *ch, CHAR_DATA *victim, int dt)
{
    	int chance,number;
    	int dual_chance;
    	CHAR_DATA *vch, *vch_next;
	
	one_hit(ch,victim,dt);

    	if (ch->fighting != victim)
        	return;
    	
    	/* Area attack -- BALLS nasty! */
    	if (IS_SET(ch->off_flags,OFF_AREA_ATTACK))
    	{
        	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
        	{
            		vch_next = vch->next;
            		if ((vch != victim && vch->fighting == ch))
                		one_hit(ch,vch,dt);
       	 	}
    	}

    	if (IS_AFFECTED(ch,AFF_HASTE)
    	||  (IS_SET(ch->off_flags,OFF_FAST) && !IS_AFFECTED(ch,AFF_SLOW)))
        	one_hit(ch,victim,dt);

    	if (ch->fighting != victim || dt == gsn_backstab)
        	return;

    	chance = get_skill(ch,gsn_second_attack)/2;
    	dual_chance = ch->level*2/3;

    	if (IS_AFFECTED(ch,AFF_SLOW) && !IS_SET(ch->off_flags,OFF_FAST))
    	{
		dual_chance /= 2;
        	chance /= 2;
    	}

    	if (number_percent() < chance)
    	{
        	one_hit(ch,victim,dt);
        	if (ch->fighting != victim)
            		return;
    	}

    	if (IS_AFFECTED(ch,AFF_HASTE))
		dual_chance *= 6/4;

    	if (number_percent() < dual_chance && get_eq_char(ch,WEAR_DUAL_WIELD) != NULL)
    	{
		one_hit(ch,victim,gsn_dual_wield);
		if (ch->fighting != victim)
			return;
    	}

    	chance = get_skill(ch,gsn_third_attack)/4;
     	dual_chance *= 2/3;

    	if (IS_AFFECTED(ch,AFF_SLOW) && !IS_SET(ch->off_flags,OFF_FAST))
    	{
		dual_chance = 0;
        	chance = 0;
    	}

    	if (IS_AFFECTED(ch,AFF_HASTE))
		dual_chance *= 3/2;

    	if (number_percent() < chance)
    	{
        	one_hit(ch,victim,dt);
        	if (ch->fighting != victim)
           		return;
    	}

    	if (number_percent() < dual_chance && get_eq_char(ch,WEAR_DUAL_WIELD) != NULL)
    	{
		one_hit(ch,victim,gsn_dual_wield);
		if (ch->fighting != victim)
			return;
    	}

    	if (ch->pIndexData->vnum >= 28000 && ch->pIndexData->vnum <= 28011)
    	{
		int hits=number_range(1,2);
		int x;

		for (x=0; x<hits; x++)	
		{
	    		one_hit(ch,victim,dt);
	    		if (ch->fighting != victim)
	 			return;
		}
    	}

    	if (ch->wait > 0)
        	return;

    	/* now for the skills */
    	number = number_range(0,9);

    	switch(number)
    	{
    	case (0) :
        if (IS_SET(ch->off_flags,OFF_BASH))
            do_bash(ch,"");
	else if (IS_SET(ch->off_flags,OFF_TAIL))
	    do_tail(ch,"");
        break;

    	case (1) :
        if (IS_SET(ch->off_flags,OFF_BERSERK) && !IS_AFFECTED(ch,AFF_BERSERK))
            do_berserk(ch,"");
        break;

	case (2) :
        if (IS_SET(ch->off_flags,OFF_DISARM)
        || (get_weapon_sn(ch) != gsn_hand_to_hand
        && (IS_SET(ch->act,ACT_WARRIOR)
        ||  IS_SET(ch->act,ACT_THIEF))))
            do_disarm(ch,"");
        break;

    	case (3) :
        if (IS_SET(ch->off_flags,OFF_KICK))
            do_kick(ch,"");
        break;

    	case (4) :
        if (IS_SET(ch->off_flags,OFF_KICK_DIRT))
            do_dirt(ch,"");
        break;

    	case (5) :
        if (IS_SET(ch->off_flags,OFF_TAIL))
        {
               do_tail(ch,"");
        }
        break;

    	case (6) :
        if (IS_SET(ch->off_flags,OFF_TRIP))
            do_trip(ch,"");
        break;

    	case (7) :
        if (IS_SET(ch->off_flags,OFF_CRUSH))
        {
            do_crush(ch,"") ;
        }
        break;
    	case (8) :
        if (IS_SET(ch->off_flags,OFF_BACKSTAB))
        {
            do_backstab(ch,"");
        }
	else if (IS_SET(ch->off_flags,OFF_UNDEAD_DRAIN))
		do_undead_drain(ch,"");
	break;
    	case (9) :
	if ( IS_SET(ch->off_flags,OFF_UNDEAD_DRAIN))
	{
		do_undead_drain(ch,"");
	}
	break;
    	}

    int extraAttacks;
    if(ch->numAttacks > 0)
	for (extraAttacks = 1;extraAttacks<=ch->numAttacks;extraAttacks++)
		if(extraAttacks <= 20)
			one_hit(ch,victim,dt);
}


/*
 * Hit one guy once.
 */
void one_hit(CHAR_DATA *ch, CHAR_DATA *victim, int dt)
{
	one_hit_new(ch,victim,dt,HIT_SPECIALS,HIT_BLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
}

void one_hit_new( CHAR_DATA *ch, CHAR_DATA *victim, int dt, bool specials, bool blockable, int addition, int multiplier, char *dnoun)
{
    OBJ_DATA *wield;
    int dam, mdam, fchance, tchance, diceroll, diceroll2 = 0, sn, skill, dam_type, tmp_dt, tdt=dt;
    bool result, truestrike = FALSE, exeresult;
    const char *attack;
    char buf1[MAX_STRING_LENGTH], buf2[MAX_STRING_LENGTH], buf3[MAX_STRING_LENGTH];
	
    /* just in case */
    if (victim == ch || ch == NULL || victim == NULL)
        return;

    if(dt == TYPE_TRUESTRIKE)
    {
	truestrike = TRUE;
	dt = TYPE_UNDEFINED;
    }

    if ( victim->position == POS_DEAD || (ch->in_room != victim->in_room))
        return;

    if (is_safe(ch,victim))
	return;

    /*
     * Figure out the type of damage message.
     */
    wield = get_eq_char( ch, WEAR_WIELD );

    if ( dt == TYPE_UNDEFINED )
    {
        dt = TYPE_HIT;
        if ( wield != NULL && wield->item_type == ITEM_WEAPON )
            dt += wield->value[3];
        else 
            dt += ch->dam_type;

    }

    if(is_affected(ch,gsn_executioner) && number_percent()<5 && specials==TRUE && tdt==TYPE_UNDEFINED)
    {
      	act("$n darts forward and gracefully strikes you!",ch,0,victim,TO_VICT);
     	act("$n darts forward and gracefully strikes $N!",ch,0,victim,TO_NOTVICT);
      	act("Moving fluidly, you dart forward and gracefully strike $N!",ch,0,victim,TO_CHAR);
		one_hit_new( ch, victim, TYPE_UNDEFINED, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, NULL);
		return;
    }
    if(is_affected(victim,gsn_executioner) && number_percent()<7 && dt!=gsn_flurry && dt!=gsn_drum && specials==TRUE)
    {
        act("$n smoothly redirects $N's attack, unleashing a deadly riposte!",victim,0,ch,TO_NOTVICT);
        act("You smoothly redirect $N's attack, and unleash a deadly riposte!",victim,0,ch,TO_CHAR);
        act("$n smoothly redirects your attack, unleashing a deadly riposte!",victim,0,ch,TO_VICT);
		one_hit_new( victim, ch, TYPE_UNDEFINED, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, NULL);
		return;
    }
	
    if (IS_NPC(ch) && ch->pIndexData->vnum == 3011)
	truestrike = TRUE;

    if(get_skill(victim,gsn_backfist) !=0 && specials==TRUE)
    {
		if (number_percent() < (get_skill(victim,gsn_backfist)/4) && count_hands(victim) < 2)
		{
			act("$n steps in quickly to strike $N with a quick backfist!",victim,0,ch,TO_NOTVICT);
			act("You step in quickly to strike $N with a quick backfist!",victim,0,ch,TO_CHAR);
			act("$n steps in quickly to strike you with a quick backfist!",victim,0,ch,TO_VICT);
			exeresult = damage( victim, ch, GET_DAMROLL(victim), gsn_backfist, DAM_BASH, TRUE );
			check_improve(victim,gsn_backfist,TRUE,2);
		}
		else
		{
			if ( count_hands(victim) < 2)
                            check_improve(victim,gsn_backfist,FALSE,5);
		}
    }

    if(is_affected(victim,gsn_shield_of_redemption) && get_eq_char(victim,WEAR_SHIELD) != NULL && number_percent() > 40)
	damage_new(victim, ch, number_range(20,30), TYPE_UNDEFINED, DAM_HOLY, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "shield of redemption");

    if (wield != NULL)
	dam_type = attack_table[wield->value[3]].damage;
    else
	dam_type = attack_table[ch->dam_type].damage;

    if (dam_type == -1)
        dam_type = DAM_BASH;

    /* get the weapon skill */
    sn = get_weapon_sn(ch);
    skill = 20 + get_weapon_skill(ch,sn);

    if (dt == gsn_dual_wield)
    {
		wield = get_eq_char( ch, WEAR_DUAL_WIELD );
        tmp_dt = TYPE_HIT;
        if ( wield != NULL && wield->item_type == ITEM_WEAPON )
            tmp_dt += wield->value[3];
        else 
            tmp_dt += ch->dam_type;

    	if (tmp_dt < TYPE_HIT)
		{
            if (wield != NULL)
            	dam_type = attack_table[wield->value[3]].damage;
            else
            	dam_type = attack_table[ch->dam_type].damage;
		} else {
            dam_type = attack_table[dt - TYPE_HIT].damage;
		}

    	if (dam_type == -1)
            dam_type = DAM_BASH;

    	if (wield == NULL)		
			sn = -1;
	
		else switch (wield->value[0])
	    	{
		    default:			sn = -1;		break;
		    case (WEAPON_SWORD):	sn = gsn_sword;		break;
		    case (WEAPON_DAGGER):	sn = gsn_dagger;	break;
		    case (WEAPON_SPEAR):	sn = gsn_spear;		break;
		    case (WEAPON_MACE):		sn = gsn_mace;		break;
		    case (WEAPON_AXE):		sn = gsn_axe;		break;
		    case (WEAPON_FLAIL):	sn = gsn_flail;		break;
		    case (WEAPON_WHIP):		sn = gsn_whip;		break;
		    case (WEAPON_POLEARM):	sn = gsn_polearm;	break;
	    	}
	    	skill = 10 + get_weapon_skill(ch,sn);
    }

    if(truestrike)
    	dam_type = DAM_TRUESTRIKE;

    if(victim->cabal==CABAL_KNIGHT && get_skill(victim,skill_lookup("blademaster"))>0 && specials==TRUE)
    {
	tchance=3+((get_skill(victim,skill_lookup("blademaster"))-75)/3);
	if(is_affected(victim,AFF_HASTE))
		tchance+=2;
	if(is_affected(victim,AFF_SLOW))
		tchance-=5;
	if(number_percent()<tchance)
	{
		attack = get_attack_noun(ch,dt);
		if(cabal_down_new(victim,CABAL_KNIGHT,FALSE))
			return;
	  	sprintf(buf1,"$n parries $N's %s, and skillfully unleashes an attack of $s own!",attack);
        	act(buf1,victim,0,ch,TO_NOTVICT);
	  	sprintf(buf2,"You parry $N's %s and skillfully unleash an attack of your own!",attack);
	  	act(buf2,victim,0,ch,TO_CHAR);
        	sprintf(buf3,"$n parries your %s and skillfully unleashes an attack of $s own!",attack);
	  	act(buf3,victim,0,ch,TO_VICT);
		attack = get_attack_noun(victim,dt);
	  	one_hit_new( victim, ch, dt, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, str_dup(attack));
		check_improve(victim, skill_lookup("blademaster"),TRUE,5);
	  	return;
	}
    }

    if((IS_NPC(victim) && IS_SET(victim->off_flags,OFF_FADE)) || is_affected(victim,gsn_displacement))
    {
	fchance = 25 + (victim->level - ch->level);
	if(IS_AFFECTED(ch,AFF_HASTE))
		fchance-=8;
	if(IS_AFFECTED(victim,AFF_HASTE))
		fchance+=5;
	if(number_percent()<fchance)
	{
	    attack = get_attack_noun(ch,dt);
	    sprintf(buf1,"$N's %s lands on empty air near $n.",attack);
	    act(buf1,victim,0,ch,TO_NOTVICT);
	    sprintf(buf2,"$N's %s lands on empty air near you.",attack);
            act(buf2,victim,0,ch,TO_CHAR);
	    sprintf(buf3,"Your %s lands on empty air near $n.",attack);
            act(buf3,victim,0,ch,TO_VICT);

	    if(!ch->fighting && !victim->fighting)
		set_fighting(ch,victim);

	    return;
	}
    }


    /*
     * Hit.
     * Calc damage.
     */
    if ( IS_NPC(ch) && (!ch->pIndexData->new_format || wield == NULL))
    {
        if (!ch->pIndexData->new_format)
        {
	    dam = number_range( ( ch->level + ch->drain_level) / 2, (ch->drain_level + ch->level) * 3 / 2 );
            if ( wield != NULL )
                dam += dam / 2;
        }
        else
            dam = dice(ch->damage[DICE_NUMBER],ch->damage[DICE_TYPE]);
    }    
    else
    {
        if (sn != -1)
            check_improve(ch,sn,TRUE,5);
        if ( wield != NULL )
        {
            if (wield->pIndexData->new_format)
                dam = dice(wield->value[1],wield->value[2]) * skill/100;
            else
                dam = number_range( wield->value[1] * skill/100, wield->value[2] * skill/100);

            if (get_eq_char(ch,WEAR_SHIELD) == NULL)  /* no shield = more */
                dam = dam * 11/10;

	    if (wield->level - (ch->drain_level + ch->level) >= 35)
		dam = (dam*6)/10;
	    else if (wield->level - (ch->drain_level + ch->level) >= 25)
		dam = (dam*7)/10;
	    else if (wield->level - (ch->drain_level + ch->level) >= 15)
		dam = (dam*8)/10;
	    else if (wield->level - (ch->drain_level + ch->level) >= 5)
		dam = (dam*9)/10;

            /* sharpness! */
            if (IS_WEAPON_STAT(wield,WEAPON_SHARP))
            {
                int percent;

                if ((percent = number_percent()) <= (skill / 8))
                    dam *= 1.2;
            }
        }
        else
       	    dam = number_range( 1 + GET_DAMROLL(ch) / 12 + 7 * skill/100, (ch->level - 5) * skill/100);
    }

    /*
     * Bonuses.
     */

    dam += (ch->drain_level + ch->level)/8;

    if ( get_skill(ch,gsn_enhanced_damage) > 0 )
    {
        diceroll = number_percent();
	if (get_skill(ch,gsn_enhanced_damage_two) > 0)
	{
		diceroll2 = number_percent();
	
		if (diceroll2 < get_skill(ch,gsn_enhanced_damage_two) && diceroll2 < get_skill(ch,gsn_enhanced_damage) )
		{
			check_improve(ch,gsn_enhanced_damage_two,TRUE,5);
			diceroll2 = diceroll2 + 5;
			diceroll = diceroll2;
		}
	}
	if (get_skill(ch,gsn_enhanced_damage_three) > 0)
	{
            	check_improve(ch,gsn_enhanced_damage_three,TRUE,3);
		diceroll += get_skill(ch,gsn_enhanced_damage_three)/3;
	}
        if (diceroll <= get_skill(ch,gsn_enhanced_damage))
        {
            	check_improve(ch,gsn_enhanced_damage,TRUE,6);
            	dam += (dam * diceroll/125);
        }
    }
    else
    {
	dam *= 7;
	dam /= 10;
    } 

    if ( !IS_AWAKE(victim) )
        dam *= 2;
     else if (victim->position < POS_FIGHTING)
        dam = dam * 3 / 2;

    if ( dt == gsn_backstab && wield != NULL) 
    {
        if ( wield->value[0] != 2 )
 	    dam *= 2 + ((ch->level + ch->drain_level) / 10) ;
        else 
            dam *= 2 + ((ch->drain_level + ch->level) / 8);
    }

    if ( dt == gsn_ambush )
	dam *= 5/2 + ((ch->drain_level + ch->level) / 6 );

    mdam = GET_DAMROLL(ch);

    dam += mdam * UMIN(100,skill) /100;

    if (wield != NULL && IS_WEAPON_STAT(wield,WEAPON_VORPAL) && number_percent() < 20)
    {
	act("$p emits a fierce red glow as it lashes violently at $N!",ch,wield,victim,TO_CHAR);
	act("$p emits a fierce red glow as it lashes violently at You!",ch,wield,victim,TO_VICT);
	act("$p emits a fierce red glow as it lashes violently at $N!",ch,wield,victim,TO_NOTVICT);
	dam *= 1.2;
    }

    if (wield != NULL && wield->pIndexData->vnum == OBJ_VNUM_AVENGER)
    {
	if (IS_WEAPON_STAT(wield,WEAPON_SHADOWBANE) && number_percent() < 7)
	{
		act("$p flares and sears $N!",ch,wield,victim,TO_CHAR);
		act("$p flares and sears you!",ch,wield,victim,TO_VICT);
		act("$p flares and sears $N!",ch,wield,victim,TO_NOTVICT);
		dam *= 1.5;
	}
	if (IS_WEAPON_STAT(wield,WEAPON_LIGHTBRINGER) && number_percent() < 10)
	{
		act("$p flares and sears $N!",ch,wield,victim,TO_CHAR);
		act("$p flares and sears you!",ch,wield,victim,TO_VICT);
		act("$p flares and sears $N!",ch,wield,victim,TO_NOTVICT);
		dam *= 1.7;
	}

    }

    if(!IS_NPC(ch) && ch->cabal == CABAL_RAGER 
    && !cabal_down_new(ch,CABAL_RAGER,FALSE) && number_percent()<=5 && specials==TRUE)
    {
	send_to_char("You land an especially devastating blow!\n\r",ch);
	act("$n lands an especially devastating blow!",ch,0,0,TO_ROOM);
	dam *= 3.5;
    }

    // Same as above, except for rager mobs.
    if (IS_NPC(ch) && ch->cabal == CABAL_RAGER && number_percent() < 15 && specials == TRUE
	&& (IS_CABAL_OUTER_GUARD(ch) || IS_CABAL_GUARD(ch)))
    {
        send_to_char("You land an especially devastating blow!\n\r",ch);
        act("$n lands an especially devastating blow!",ch,0,0,TO_ROOM);
        dam *= 3;
    }

    if ( dam <= 0 )
        dam = 1;

	//Bulwark of blades damage reflection -- Zornath
	if(is_affected(victim,skill_lookup("bulwark of blades"))) {
		int chance;
		AFFECT_DATA bulwark_bleed;
		
		if (get_skill(victim,skill_lookup("blade mastery")) < 1) chance = 30;
		else chance = 50;
		
		if (number_percent() < chance) {
			act("The blade barrier around $N dices your flesh as your strike $M!",ch,0,victim,TO_CHAR);
			act("The blade barrier around $N dices into $n's flesh!",ch,0,victim,TO_NOTVICT);
			act("The blade barrier around you dices into $n's flesh as $e strikes you!",ch,0,victim,TO_VICT);
		
			if (get_skill(victim,skill_lookup("blade mastery")) < 1)
				damage_newer(victim,ch,dam / 6,TYPE_UNDEFINED,DAM_SLASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the dicing blades",SINGULAR,DAM_NOT_POSSESSIVE,NULL,FALSE);
			else {
				damage_newer(victim,ch,dam / 3,TYPE_UNDEFINED,DAM_SLASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the dicing blades",SINGULAR,DAM_NOT_POSSESSIVE,NULL,FALSE);	
				
				//10% chance to add a bleed effect
				if (number_percent() < 10 && !is_affected(ch,skill_lookup("dicing wounds"))) {
					act("The blades dig deep into $n's flesh, unleashing a mist of blood!",ch,0,victim,TO_VICT);
					act("The blades dig deep into $n's flesh, unleashing a mist of blood!",ch,0,victim,TO_NOTVICT);
					act("The blades dig deep into your flesh, unleashing a spray of blood!",ch,0,victim,TO_CHAR);
					init_affect(&bulwark_bleed); 
					bulwark_bleed.aftype = TO_AFFECTS;
					bulwark_bleed.type = skill_lookup("dicing wounds");
					bulwark_bleed.duration = 3;
					bulwark_bleed.level = victim->level;
					bulwark_bleed.owner_name = str_dup(victim->original_name);
					bulwark_bleed.affect_list_msg = str_dup("bleeding uncontrollably");
					affect_to_char(ch,&bulwark_bleed);
				}
			}
		}
	}
	
    if(ch->cabal==CABAL_KNIGHT && get_skill(ch,skill_lookup("truestrike")) 
	&& number_percent()<((get_skill(ch,skill_lookup("truestrike"))-40)/5)-3 && !cabal_down_new(ch,CABAL_KNIGHT,FALSE) 
	&& specials==TRUE)
    {
      	act("$n unleashes a blow of true accuracy, striking through your defenses!",ch,0,victim,TO_VICT);
      	act("$n unleashes a blow of true accuracy, striking through $N's defenses!",ch,0,victim,TO_NOTVICT);
     	act("You unleash a blow of true accuracy, striking through $N's defenses!",ch,0,victim,TO_CHAR);	
	dam*=1.5;
      	result = damage_new( ch, victim, dam, dt, DAM_TRUESTRIKE, TRUE, blockable, addition, multiplier,NULL);
      	check_improve(ch,skill_lookup("truestrike"),TRUE,1);
    }
    else
    	result = damage_new( ch, victim, dam, dt, dam_type, TRUE, blockable, addition, multiplier,dnoun);

    /* but do we have a funky weapon? */
    if (result && wield != NULL)
    { 
        int dam;

        if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_POISON))
        {
            int level = 10;
            AFFECT_DATA *poison, af;

            if ((poison = affect_find(wield->affected,gsn_poison)) == NULL)
                level = wield->level;
            else
                level = poison->level;
        
            if (!saves_spell(level / 2,victim,DAM_POISON) && !IS_AFFECTED(victim,AFF_POISON)) 
            {
                send_to_char("You feel poison coursing through your veins.\n\r", victim);
                act("$n is poisoned by the venom on $p.", victim,wield,NULL,TO_ROOM);
                init_affect(&af);
                af.where     = TO_AFFECTS;
		af.aftype    = AFT_MALADY;
                af.type      = gsn_poison;
                af.level     = level * 3/4;
                af.duration  = level / 10;
                af.location  = APPLY_STR;
                af.modifier  = -6;
                af.bitvector = AFF_POISON;
				af.owner_name	= str_dup(ch->original_name);
                affect_join( victim, &af );
            }

            /* weaken the poison if it's temporary */
            if (poison != NULL)
            {
                poison->level = UMAX(0,poison->level - 2);
                poison->duration = UMAX(0,poison->duration - 1);
        
                if (poison->duration < 0)
                    act("The poison on $p has worn off.",ch,wield,NULL,TO_CHAR);
            }
        }

        if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_VAMPIRIC) && number_percent() < 8)
        {
            	dam = number_range(wield->level/2, wield->level);
            	act("$p draws life from $n.",victim,wield,NULL,TO_ROOM);
            	act("You feel $p drawing your life away.", victim,wield,NULL,TO_CHAR);
            	damage_old(ch,victim,dam,dt,DAM_NEGATIVE,FALSE);
            	ch->hit = UMIN(ch->hit + dam, ch->max_hit);
        }

        if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_FLAMING) && number_percent() < 15)
        {
            	dam = number_range(1,wield->level / 4 + 1);
            	act("$n is burned by $p.",victim,wield,NULL,TO_ROOM);
            	act("$p sears your flesh.",victim,wield,NULL,TO_CHAR);
            	fire_effect( (void *) victim,wield->level/2,dam,TARGET_CHAR);
            	damage(ch,victim,dam,dt,DAM_FIRE,FALSE);
        }

        if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_FROST) && number_percent() < 15)
        {
            	dam = number_range(1,wield->level / 6 + 2);
            	act("$p freezes $n.",victim,wield,NULL,TO_ROOM);
            	act("The cold touch of $p surrounds you with ice.", victim,wield,NULL,TO_CHAR);
            	cold_effect(victim,wield->level/2,dam,TARGET_CHAR);
            	damage(ch,victim,dam,dt,DAM_COLD,FALSE);
        }

        if (ch->fighting == victim && IS_WEAPON_STAT(wield,WEAPON_SHOCKING) && number_percent() < 15)
	{
            	dam = number_range(1,wield->level/5 + 2);
            	act("$n is struck by lightning from $p.",victim,wield,NULL,TO_ROOM);
            	act("You are shocked by $p.",victim,wield,NULL,TO_CHAR);
            	shock_effect(victim,wield->level/2,dam,TARGET_CHAR);
		damage(ch,victim,dam,dt,DAM_LIGHTNING,FALSE);
        }
    } // End special weapon flag if.

    tail_chain( );
    return;
}

int avenger_multiplier(CHAR_DATA *ch,OBJ_DATA *wield)
{
    	int avenge;

    	avenge = 1;
    	if (IS_EVIL(ch->fighting) && wield != NULL)
	{
		if (number_percent() < 7 && IS_WEAPON_STAT(wield,WEAPON_SHADOWBANE))
			avenge = 2;
		if (number_percent() < 7 && IS_WEAPON_STAT(wield,WEAPON_LIGHTBRINGER))
			avenge = 3/2;
	}

    	return avenge;
}

/*
 * Inflict damage from a hit.
 */

bool damage(CHAR_DATA *ch,CHAR_DATA *victim,int dam,int dt,int dam_type, bool show) 
{
	return damage_new(ch, victim, dam, dt, dam_type, show, HIT_BLOCKABLE, HIT_NOADD, HIT_NOMULT, NULL);
}


bool damage_old( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int dam_type, bool show )
{
	return damage_new(ch, victim, dam, dt, dam_type, show, HIT_BLOCKABLE, HIT_NOADD, HIT_NOMULT, NULL);
}

int con_multiplier(int con, int dam) {
	float damage = dam;
	switch(con) {
		case 25: damage *= 0.955; break;
		case 24: damage *= 0.96; break;
		case 23: damage *= 0.965; break;
		case 22: damage *= 0.97; break;
		case 21: damage *= 0.975; break;
		case 20: damage *= 0.98; break;
		case 19: damage *= 0.985; break;
		case 18: damage *= 0.99; break;
		case 17: damage *= 0.995; break;
		case 16: damage *= 1; break;
		case 15: damage *= 1.02; break;
		case 14: damage *= 1.04; break;
		case 13: damage *= 1.06; break;
		case 12: damage *= 1.08; break;
		case 11: damage *= 1.1; break;
		case 10: damage *= 1.12; break;
		case 9: damage *= 1.14; break;
		case 8: damage *= 1.16; break;
		case 7: damage *= 1.18; break;
		case 6: damage *= 1.2; break;
		case 5: damage *= 1.22; break;
		case 4: damage *= 1.24; break;
		case 3: damage *= 1.26; break;
		case 2: damage *= 1.35; break;
		case 1: damage *= 1.45; break;
		default: damage *= 2; break;
	}
	return (int)damage;
}

float calculate_ac_redux(int ac) {
	float ac_mod = 0;
	//If <75% reduction
	if (ac > AC_PER_ONE_PERCENT_DECREASE_DAMAGE * 75) 
		//Damage modifier is AC divided by some predefined constant that can be tweaked to increase or decrease the effectiveness of AC. Zornath
		ac_mod = ((float)ac / (float)AC_PER_ONE_PERCENT_DECREASE_DAMAGE) / (float)100;
	//Otherwise asymptotically approach 100%
	else {
		//Finding a widening factor for (-1/x) + 100, so that redux asymptotically approaches 100 
		long widening_factor = -1 * AC_PER_ONE_PERCENT_DECREASE_DAMAGE * 75 * 25;
		long asymp_mod = widening_factor / ac + 100;
		ac_mod = (float)asymp_mod / (float)100;
	}
	return ac_mod;
}

bool damage_newer(CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int dam_type, bool show, bool blockable, int addition, int multiplier, char *dnoun, bool isPlural, bool showCharName, AFFECT_DATA *dealtByAffect, bool keep_invisible)
{
    OBJ_DATA *corpse;
    OBJ_DATA *obj;
    OBJ_DATA *wield;
    bool immune;
    int originaldam;
    CHAR_DATA *owner = NULL;
    bool nosafe = FALSE;
	int ac;
	
	dam = con_multiplier(get_curr_stat(victim,STAT_CON),dam);
	
	//Armor class affects damage taken now. Zornath
	switch(dt) {
		case DAM_PIERCE: ac = GET_AC(victim,AC_PIERCE); break;
		case DAM_SLASH: ac = GET_AC(victim,AC_SLASH); break;
		case DAM_BASH: ac = GET_AC(victim,AC_BASH); break;
		default: ac = GET_AC(victim,AC_EXOTIC); break;
	}
	
	dam -= dam * calculate_ac_redux(ac);
	
	//For damage dealt by affects such as poison, plague, etc., lookup the owner by name
	if (dealtByAffect) {
		owner = find_char_by_name(dealtByAffect->owner_name);
		ch = victim;
	}
	
    if ( victim->position == POS_DEAD )
        return FALSE;

    if (IS_NPC(ch) && spellbaned(ch,victim,dt))
	return FALSE;

	if (is_affected(ch,skill_lookup("valiant wrath")))
    {
		dnoun = str_dup("valiant wrath");
		dt += DAM_SOUND;
    }
	
	if (get_skill(ch,skill_lookup("elemental affinity")) > 75) {
		dam = (23 * dam) / 20;
	}

    if (!dnoun && !nosafe && is_safe(ch,victim))
	return FALSE;


    originaldam=dam;
    /*
     * Stop up any residual loopholes.
     */

    if ( dam > 3000 && dt >= TYPE_HIT)
    {
        dam = 3000;
    }

    dam+=addition;

    if (multiplier != HIT_NOMULT) 
    {
	dam *= multiplier/100.1;
    }

    if (!IS_NPC(ch) && IS_SET(ch->act,PLR_MORON))
 	dam *= .50;

    if ( victim != ch && ch->in_room == victim->in_room)
    {
        /*
         * Certain attacks are forbidden.
         * Most other attacks are returned.
         */
        check_killer( ch, victim );

        if ( victim->position > POS_STUNNED && victim != ch && !((is_same_group(ch,victim) || is_safe(ch,victim)) && dnoun && dnoun[strlen(dnoun)-1] == '*'))
        {
            if ( victim->fighting == NULL )
                set_fighting( victim, ch );
            if (victim->timer <= 4)
                victim->position = POS_FIGHTING;
        }

        if ( victim->position > POS_STUNNED && victim != ch)
        {
            if ( ch->fighting == NULL && !((is_same_group(ch,victim) || is_safe(ch,victim)) && dnoun && dnoun[strlen(dnoun)-1] == '*'))
                set_fighting( ch, victim );
        }

        /*
         * More charm stuff.
         */
        if ( victim->master == ch && !(dnoun && dnoun[strlen(dnoun)-1] == '*'))
            stop_follower( victim );
    }

    if ( (dt >= TYPE_HIT || dt == gsn_dual_wield) && ch != victim && blockable==HIT_BLOCKABLE && ch->in_room == victim->in_room && dam>0) 
    {

	if (check_shield_magnetism(ch, victim, dt)) return FALSE;

	if (check_spin(ch,victim, dt))			return FALSE;

    if (check_distance(ch,victim,dt))		return FALSE;

	if (check_ironhands(ch,victim,dt))		return FALSE;

	if (check_unarmed_defense(ch,victim,dt))	return FALSE;

	if (check_parry(ch,victim,dt))
	{
		(check_restrike(ch,victim));
		return FALSE;
	}		

	if (check_shield_block(ch,victim,dt))		return FALSE;

	if (check_dodge(ch,victim,dt))			return FALSE;

    if (check_questdodge(ch,victim,dt))		return FALSE;
	
   }

   if (ch->enhancedDamMod >= 0)
   {
	dam *= (ch->enhancedDamMod / 100);
   }

   if (is_affected(victim,gsn_static_charge))
   {
	affect_strip(victim,gsn_static_charge);
	damage_old(victim,ch,victim->level * number_range(6,8),gsn_static_charge,DAM_LIGHTNING,TRUE);
   }
   
	/* damage reduction */
    if (dam > 85)
    {
	dam = (dam - 85);
	dam *= 8;
	dam /= 10;
	dam += 85;
    }

   if (is_affected(victim,gsn_soul_channel) && dam_type != DAM_TRUESTRIKE)
   {
	AFFECT_DATA *af;
	int start, level = 0;
	af = affect_find(victim->affected,gsn_soul_channel);
	for (start = 1; start <= level; start++)
	{
		dam *= .8;
	}
    }
    if (is_affected(victim,gsn_stoneskin) && dam_type!=DAM_TRUESTRIKE)
	    dam *= .8;
	if (is_affected(victim,gsn_wraithform) && dam_type!=DAM_TRUESTRIKE)
	    dam *= .65;
    if (is_affected(victim,gsn_shield) && dam_type!=DAM_TRUESTRIKE)
	    dam *= .95;
    if (is_affected(victim,gsn_barrier) && dam_type!=DAM_TRUESTRIKE)
	    dam *= .5;
	if (is_affected(victim,skill_lookup("aura")) && dam_type != DAM_TRUESTRIKE)
		dam *= 0.7;
		
    if (is_affected(ch,gsn_rage) && dam_type!=DAM_TRUESTRIKE)
        dam *= 1.25;

    if ( dam > 1 && ((IS_AFFECTED(victim, AFF_PROTECT_EVIL) && IS_EVIL(ch) )
    ||               (IS_AFFECTED(victim, AFF_PROTECT_GOOD) && IS_GOOD(ch) )) && dam_type!=DAM_TRUESTRIKE)
        dam -= dam / 4;

    // Dragonplate
    obj = get_eq_char(ch,WEAR_BODY);
    if (obj != NULL && obj->pIndexData->vnum == OBJ_VNUM_DRAGONPLATE)
    {
	if(!IS_AFFECTED(victim, AFF_SANCTUARY) && dam_type!=DAM_TRUESTRIKE) {
		dam *= .75;
	}
    }

    // Armored carapace 
    obj = get_eq_char(victim,WEAR_ABOUT);
    if (obj!=NULL && obj->pIndexData->vnum==28015)
	dam *= .6;

    // Aura of Darkness
    obj = get_eq_char(victim,WEAR_BODY);
    if (obj!=NULL && obj->pIndexData->vnum==28017 && (dam_type==DAM_HOLY || dam_type==DAM_NEGATIVE))
	dam *= .4;

    /* Check for fire demon belt immunity */
    obj = get_eq_char(victim,WEAR_WAIST);
    if (obj!=NULL && obj->pIndexData->vnum==28016 && dam_type==DAM_FIRE)
	dam *= .4;

	if (victim->dam_mod < 20)
		dam *= 0.2;
	else dam *= (victim->dam_mod / 100.01);

    if (check_ward_diminution(victim, ch, dam, dam_type, dt))
	dam = 1;


	if (!keep_invisible) {
	    un_camouflage( ch, NULL);
	    un_earthfade(ch, NULL);
	    un_hide(ch, NULL);
	    un_invis(ch, NULL);
	    un_sneak(ch, NULL);
	    un_duo(ch, NULL);
	}

    /*
     * Damage modifiers.
     */
    if ( dam > 1 && IS_AFFECTED(victim, AFF_SANCTUARY) && dam_type!=DAM_TRUESTRIKE) {
		if (!is_affected(victim,skill_lookup("shroud")))
			dam /= 2;
		else dam *= 0.4;
	}

    immune = FALSE; 

    /* Check for absorb */
    if (check_absorb(ch,victim,dt))
	return FALSE;

    /*
     * Check for parry, and dodge.
     */

    switch(check_immune(victim,dam_type))
    {
	case(IS_IMMUNE):	immune = TRUE;	dam = 0;		break;
	case(IS_RESISTANT):	dam -= dam/3;	break;
	case(IS_VULNERABLE):	dam += dam/4;	break;
    }

    //ObsidianPalace
    if(IS_NPC(victim) && victim->pIndexData->vnum>28003 && victim->pIndexData->vnum<28012 && !IS_AFFECTED(victim,AFF_SANCTUARY))
	dam/=2;
    if(IS_NPC(victim) && victim->pIndexData->vnum==28006 && dam>10 && dam_type!=DAM_MENTAL)
	dam=number_range(5,25);

    if (dt == gsn_dual_wield)
	wield = get_eq_char(ch,WEAR_DUAL_WIELD);
    else wield = get_eq_char(ch,WEAR_WIELD);


    if(wield && IS_SET(victim->vuln_flags, VULN_IRON) && !str_cmp(wield->material->name,"iron"))
	dam += dam / 3;
    if(wield && IS_SET(victim->vuln_flags, VULN_SILVER) && !str_cmp(wield->material->name,"silver"))
        dam += dam / 3;
	
	if (is_affected(victim,skill_lookup("divine transferrer"))) {
		AFFECT_DATA *af;
		CHAR_DATA *transferree;
		
		af = affect_find(victim->affected,skill_lookup("divine transferrer"));
		if ((transferree = find_char_by_name(af->owner_name)) != NULL) {
			if (transferree->in_room == ch->in_room) {
				if (is_affected(transferree,skill_lookup("divine transferree"))) {
					damage_newer(ch,transferree,dam/2,dt,dam_type,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"the divine transferrance",PLURAL,DAM_NOT_POSSESSIVE,NULL,FALSE);
					dam /= 2;
				}
			}
		}
	}
	

    if (showCharName)
    {
    	if (show && isPlural)
       		dam_message( ch, victim, dam, dt, immune, dnoun, PLURAL, DAM_POSSESSIVE );
    	else if (show && !isPlural)
       		dam_message( ch, victim, dam, dt, immune, dnoun, SINGULAR, DAM_POSSESSIVE );
    }
    else
    {
    	if (show && isPlural)
       		dam_message( ch, victim, dam, dt, immune, dnoun, PLURAL, DAM_NOT_POSSESSIVE );
    	else if (show && !isPlural)
       		dam_message( ch, victim, dam, dt, immune, dnoun, SINGULAR, DAM_NOT_POSSESSIVE );
    }

    if ( (dt >= TYPE_HIT || dt == gsn_dual_wield) && ch != victim)
    {
      	if (is_affected(victim,gsn_self_immolation) && dam>0)
     	{
        	act("$N is burned by the flames engulfing $n!",victim,NULL,ch,TO_NOTVICT);
        	act("$N is burned by the flames engulfing you!",victim,NULL,ch,TO_CHAR);
        	act("You are burned by the flames engulfing $n!",victim,NULL,ch,TO_VICT);
        	damage_old(victim,ch,number_range(20,60),gsn_flames,DAM_FIRE, TRUE);
     	}

    }

    if (dam <= 0)
        return FALSE;

    /*
     * Hurt the victim.
     * Inform the victim of his new state.
     */
    if (is_affected(victim,skill_lookup("mana shield")) && victim->mana > dam)
    {
		victim->mana -= dam;
    }
    else if (is_affected(victim,skill_lookup("mana shield")) && victim->mana <= dam)
    {
		act("{rYour mana shield cannot withstand the force of $n's attack and shatters!{x",ch,0,victim,TO_VICT);
		act("{rThe mana shield around $N crumbles under your attack!{x",ch,0,victim,TO_CHAR);
		act("The mana shield around $N crumbles under $n's attack!",ch,0,victim,TO_NOTVICT);
		victim->hit -= (dam - victim->mana);
		victim->mana = 0;
		affect_strip(victim,skill_lookup("mana shield"));
		AFFECT_DATA shield_timer;
		init_affect(&shield_timer);
		shield_timer.aftype = AFT_MALADY;
		shield_timer.type = skill_lookup("mental exhaustion");
		shield_timer.duration = (ch->level / 5);
		shield_timer.affect_list_msg = str_dup("prevents usage of mana shield");
		affect_to_char(victim,&shield_timer);
    }
    else
    	victim->hit -= dam;
		
    if (dam_type == DAM_FIRE || dam_type == DAM_ACID)
	victim->noregen_dam += dam;
    if (victim->noregen_dam > victim->max_hit)
	victim->noregen_dam = victim->max_hit;

    if ( !IS_NPC(victim)
    &&   victim->level >= LEVEL_IMMORTAL
    &&   victim->hit < 1 
	&& !arena)
        victim->hit = 1;

		update_pos( victim );

    switch( victim->position )
    {
    case POS_MORTAL:
        act( "$n is mortally wounded, and will die soon, if not aided.", victim, NULL, NULL, TO_ROOM );
        send_to_char( "You are mortally wounded, and will die soon, if not aided.\n\r", victim );
        break;

    case POS_INCAP:
        act( "$n is incapacitated and will slowly die, if not aided.", victim, NULL, NULL, TO_ROOM );
        send_to_char( "You are incapacitated and will slowly die, if not aided.\n\r", victim );
        break;

    case POS_STUNNED:
        act( "$n is stunned, but will probably recover.", victim, NULL, NULL, TO_ROOM );
        send_to_char("You are stunned, but will probably recover.\n\r", victim );
        break;

    case POS_DEAD:
        act( "$n is DEAD!!", victim, 0, 0, TO_ROOM );
        send_to_char( "You have been KILLED!!\n\r\n\r", victim );
        break;

    default:
        if ( dam > victim->max_hit / 4 )
            send_to_char( "That really did HURT!\n\r", victim );
        if ( victim->hit < victim->max_hit / 4 )
            send_to_char( "You sure are BLEEDING!\n\r", victim );
        break;
    }

    /*
     * Sleep spells and extremely wounded folks.
     */
    if ( !IS_AWAKE(victim) && ch != victim)
        stop_fighting( victim, FALSE );

    /*
     * Payoff for killing things.
     */
    if ( victim->position == POS_DEAD )
    {
		if (owner)
			group_gain( owner, victim );
		else
			group_gain( ch, victim );

        if ( !IS_NPC(victim) )
        {
            sprintf( log_buf, "%s killed by %s at %ld", victim->name,
                (IS_NPC(ch) ? ch->short_descr : owner ? owner->name : ch->name), victim->in_room->vnum );
            log_string( log_buf );
		}

            /*
			* Dying penalty:
			*/

		if (owner)
	        raw_kill(owner, victim);
		else
	        raw_kill(ch, victim);

        /*
			* Death trigger
		*/
      if (!IS_NPC(ch)
        &&  (corpse = get_obj_list(ch,"corpse",ch->in_room->contents)) != NULL
        &&  corpse->item_type == ITEM_CORPSE_NPC && can_see_obj(ch,corpse))
        {
            OBJ_DATA *coins;

            corpse = get_obj_list( ch, "corpse", ch->in_room->contents ); 
            if ( IS_SET(ch->act, PLR_AUTOLOOT) &&
                 corpse && corpse->contains) /* exists and not empty */
                do_get( ch, "all corpse" );

            if (IS_SET(ch->act,PLR_AUTOGOLD) &&
                corpse && corpse->contains  && /* exists and not empty */
                !IS_SET(ch->act,PLR_AUTOLOOT))
                if ((coins = get_obj_list(ch,"gcash",corpse->contains)) != NULL)
                    do_get(ch, "all.gcash corpse");
            
            if ( IS_SET(ch->act, PLR_AUTOSAC) )
	    {
              if ( IS_SET(ch->act,PLR_AUTOLOOT) && corpse && corpse->contains)
                return TRUE;  /* leave if corpse has treasure */
              else
                do_sacrifice( ch, "corpse" );
	    }
        }
        return TRUE;
    }

    if ( victim == ch )
        return TRUE;

    /*
     * Take care of link dead people.
    if ( !IS_NPC(victim) && victim->desc == NULL )
    {
        if ( number_range( 0, victim->wait ) == 0 )
        {
            do_recall( victim, "" );
            return TRUE;
        }
    }
	*/

    /*
     * Wimp out?
     */

    if(ch->in_room != victim->in_room || dt == gsn_parting_blow)
	return TRUE;

    if ( IS_NPC(victim) && dam > 0 && victim->wait < PULSE_VIOLENCE / 2)
    {
        if ( ( IS_SET(victim->act, ACT_WIMPY) && number_bits( 2 ) == 0
        &&   victim->hit < victim->max_hit / 5)
        ||   ( IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL
        &&     victim->master->in_room != victim->in_room
	&& !is_centurion(victim) 
        && dt != gsn_parting_blow
	&& dt != gsn_cutoff))
	    do_flee( victim, "" );
    }

    if ( !IS_NPC(victim)
    &&   victim->hit > 0
    &&   victim->hit <= victim->wimpy
    &&   victim->wait < PULSE_VIOLENCE / 2 
    && dt != gsn_parting_blow)
        do_flee( victim, "" );

    tail_chain( );
    return TRUE;
}

bool damage_new(CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int dam_type, bool show, bool blockable, int addition, int multiplier, char *dnoun)
{
    return damage_newer(ch,victim,dam,dt,dam_type,show,blockable,addition,multiplier,dnoun,TRUE,TRUE, NULL, FALSE);
}

int damage_calculate(CHAR_DATA *ch, CHAR_DATA *victim, int dt, int dam_type, bool show, bool blockable, int addition, int multiplier)
{
    OBJ_DATA *obj;
    OBJ_DATA *wield;
    int diceroll, diceroll2 = 0;
    int sn, skill;
    int dam, mdam;
    bool immune;
    int originaldam;

    wield = get_eq_char( ch, WEAR_WIELD );
    sn = get_weapon_sn(ch);
    skill = 20 + get_weapon_skill(ch,sn);

    if ( IS_NPC(ch) && (!ch->pIndexData->new_format || wield == NULL))
    {
        if (!ch->pIndexData->new_format)
        {
	    dam = number_range( ( ch->level + ch->drain_level) / 2, (ch->drain_level + ch->level) * 3 / 2 );
            if ( wield != NULL )
                dam += dam / 2;
        }
        else
            dam = dice(ch->damage[DICE_NUMBER],ch->damage[DICE_TYPE]);
    }    
    else
    {
        if (sn != -1)
            check_improve(ch,sn,TRUE,5);
        if ( wield != NULL )
        {
            if (wield->pIndexData->new_format)
                dam = dice(wield->value[1],wield->value[2]) * skill/100;
            else
                dam = number_range( wield->value[1] * skill/100, wield->value[2] * skill/100);

            if (get_eq_char(ch,WEAR_SHIELD) == NULL)  /* no shield = more */
                dam = dam * 11/10;

	    if (wield->level - (ch->drain_level + ch->level) >= 35)
		dam = (dam*6)/10;
	    else if (wield->level - (ch->drain_level + ch->level) >= 25)
		dam = (dam*7)/10;
	    else if (wield->level - (ch->drain_level + ch->level) >= 15)
		dam = (dam*8)/10;
	    else if (wield->level - (ch->drain_level + ch->level) >= 5)
		dam = (dam*9)/10;

            /* sharpness! */
            if (IS_WEAPON_STAT(wield,WEAPON_SHARP))
            {
                int percent;

                if ((percent = number_percent()) <= (skill / 8))
                    dam *= 1.2;
            }
        }
        else {
       	    dam = number_range( 1 + 7 * skill/100, (ch->level - 5) * skill/100);
			dam += GET_DAMROLL(ch) / 15; 
		}
    }


    dam += (ch->drain_level + ch->level)/8;

    if ( get_skill(ch,gsn_enhanced_damage) > 0 )
    {
        diceroll = number_percent();
	if (get_skill(ch,gsn_enhanced_damage_two) > 0)
	{
		diceroll2 = number_percent();
	
		if (diceroll2 < get_skill(ch,gsn_enhanced_damage_two) && diceroll2 < get_skill(ch,gsn_enhanced_damage) )
		{
			check_improve(ch,gsn_enhanced_damage_two,TRUE,5);
			diceroll2 = diceroll2 + 5;
			diceroll = diceroll2;
		}
	}
	if (get_skill(ch,gsn_enhanced_damage_three) > 0)
	{
            	check_improve(ch,gsn_enhanced_damage_three,TRUE,3);
		diceroll += get_skill(ch,gsn_enhanced_damage_three)/3;
	}
        if (diceroll <= get_skill(ch,gsn_enhanced_damage))
        {
            	check_improve(ch,gsn_enhanced_damage,TRUE,6);
            	dam += (dam * diceroll/125);
        }
    }
    else
    {
	dam *= 7;
	dam /= 10;
    } 

    mdam = GET_DAMROLL(ch);
    dam += mdam * UMIN(100,skill) /100;

    originaldam=dam;
    if ( dam > 3000 && dt >= TYPE_HIT)
    {
        dam = 3000;
    }

    dam+=addition;

    if (multiplier != HIT_NOMULT) 
    {
	dam *= multiplier/100.1;
    }

    if (!IS_NPC(ch) && IS_SET(ch->act,PLR_MORON))
 	dam *= .50;

    if ( (dt >= TYPE_HIT || dt == gsn_dual_wield) && ch != victim && blockable==HIT_BLOCKABLE && ch->in_room == victim->in_room && dam>0) 
    {	
   }

   if (ch->enhancedDamMod >= 0)
   {
	dam *= (ch->enhancedDamMod / 100);
   }

	/* damage reduction */
    if (dam > 85)
    {
	dam = (dam - 85);
	dam *= 8;
	dam /= 10;
	dam += 85;
    }

    if (is_affected(victim,gsn_stoneskin) && dam_type!=DAM_TRUESTRIKE)
	dam *= .8;
    if (is_affected(victim,gsn_shield) && dam_type!=DAM_TRUESTRIKE)
	dam *= .95;
    if (is_affected(victim,gsn_barrier) && dam_type!=DAM_TRUESTRIKE)
	dam *= .5;
    if (is_affected(ch,gsn_rage) && dam_type!=DAM_TRUESTRIKE)
        dam *= 1.25;

    if ( dam > 1 && ((IS_AFFECTED(victim, AFF_PROTECT_EVIL) && IS_EVIL(ch) )
    ||               (IS_AFFECTED(victim, AFF_PROTECT_GOOD) && IS_GOOD(ch) )) && dam_type!=DAM_TRUESTRIKE)
        dam -= dam / 4;

    // Dragonplate
    obj = get_eq_char(ch,WEAR_BODY);
    if (obj != NULL && obj->pIndexData->vnum == OBJ_VNUM_DRAGONPLATE)
    {
	if(!IS_AFFECTED(victim, AFF_SANCTUARY) && dam_type!=DAM_TRUESTRIKE) {
		dam *= .75;
	}
    }

    // Armored carapace 
    obj = get_eq_char(victim,WEAR_ABOUT);
    if (obj!=NULL && obj->pIndexData->vnum==28015)
	dam *= .6;

    // Aura of Darkness
    obj = get_eq_char(victim,WEAR_BODY);
    if (obj!=NULL && obj->pIndexData->vnum==28017 && (dam_type==DAM_HOLY || dam_type==DAM_NEGATIVE))
	dam *= .4;

    /* Check for fire demon belt immunity */
    obj = get_eq_char(victim,WEAR_WAIST);
    if (obj!=NULL && obj->pIndexData->vnum==28016 && dam_type==DAM_FIRE)
	dam *= .4;

    if (dam_type!=DAM_TRUESTRIKE)
    	dam *= (victim->dam_mod / 100.01);

    if (check_ward_diminution(victim, ch, dam, dam_type, dt))
	dam = 1;


    /*
     * Damage modifiers.
     */
    if ( dam > 1 && IS_AFFECTED(victim, AFF_SANCTUARY) && dam_type!=DAM_TRUESTRIKE)
 	dam /= 2;

    immune = FALSE; 


    //ObsidianPalace
    if(IS_NPC(victim) && victim->pIndexData->vnum>28003 && victim->pIndexData->vnum<28012 && !IS_AFFECTED(victim,AFF_SANCTUARY))
	dam/=2;
    if(IS_NPC(victim) && victim->pIndexData->vnum==28006 && dam>10 && dam_type!=DAM_MENTAL)
	dam=number_range(5,25);

    if (dt == gsn_dual_wield)
	wield = get_eq_char(ch,WEAR_DUAL_WIELD);
    else wield = get_eq_char(ch,WEAR_WIELD);


    if(wield && IS_SET(victim->vuln_flags, VULN_IRON) && !str_cmp(wield->material->name,"iron"))
	dam -= dam / 3;
    if(wield && IS_SET(victim->vuln_flags, VULN_SILVER) && !str_cmp(wield->material->name,"silver"))
        dam -= dam / 3;


    return dam;
}

bool is_area_safe(CHAR_DATA *ch, CHAR_DATA *victim)
{
    if (victim->in_room == NULL || ch->in_room == NULL)
	return TRUE;
    if (victim == ch )
	return TRUE;
    if (victim->fighting == ch)
	return FALSE;
    if (IS_NPC(ch) && IS_GOOD(ch) && IS_GOOD(victim))
	return FALSE;
    if (IS_IMMORTAL(victim) && victim->invis_level > 2)
	return TRUE;
    if (IS_IMMORTAL(ch) && IS_IMMORTAL(victim) && victim->level > LEVEL_IMMORTAL && !is_same_group(ch,victim))
	return FALSE;
    if (IS_IMMORTAL(ch) && victim->level < ch->level && !is_same_group(ch,victim))
	return FALSE;

   /* killing mobiles */
    if (IS_NPC(victim))
    {
	if (IS_SET(victim->in_room->room_flags,ROOM_SAFE))
        {
	    send_to_char("Not in this room.\n\r",ch);
	    return TRUE;
	}
	if (is_same_group(ch,victim))
            return TRUE;
    }
    /* killing players */
    else
    {
        if (IS_SET(victim->in_room->room_flags,ROOM_SAFE))
        {
            send_to_char("Not in this room.\n\r",ch);
            return TRUE;
        }
        if (IS_NPC(ch) && ch->master != NULL && !IS_NPC(ch->master))
            return TRUE;
        if (IS_NPC(ch))
            return FALSE;
	if (is_same_group(ch,victim))
            return TRUE;

    }
    if (!IS_NPC(ch))
        ch->ghost = (time_t)NULL;
    return FALSE;
}

bool is_safe(CHAR_DATA *ch, CHAR_DATA *victim)
{
    if (victim->in_room == NULL || ch->in_room == NULL)
        return TRUE;

    if (IS_NPC(ch) && victim->invis_level >= LEVEL_HERO)
	return TRUE;

    if (!IS_NPC(ch))
	{
	if (ch->pcdata->death_status == HAS_DIED)
		return TRUE;
	}
    if (!IS_NPC(victim))
	{
	if (victim->pcdata->death_status == HAS_DIED)
		return TRUE;
	}

    if(IS_NPC(ch) && IS_NPC(victim) && !IS_AFFECTED(ch,AFF_CHARM) && !IS_AFFECTED(victim,AFF_CHARM) && !ch->desc &&
	!ch->hunting && !victim->hunting && victim!=ch && !ch->desc && !victim->desc)
	return TRUE;


/* Handle Link dead players , only saves from PKS, not mobs -Ceran */
    if (!IS_NPC(victim) && victim->desc == NULL && !IS_NPC(ch)
	&& victim->fighting != ch && ch->fighting != victim)
	{
	act("The gods protect $N from $n.",ch,0,victim,TO_NOTVICT);
	act("The gods protect $N from you.",ch,0,victim,TO_CHAR);
	return TRUE;
 	}

    if (!IS_NPC(victim) && victim->pcdata->newbie==TRUE && !IS_NPC(ch))
	{
	act("$N is too young for $n to harm.",ch,0,victim,TO_NOTVICT);
	act("$N is too young for you to harm.",ch,0,victim,TO_CHAR);
	act("You are too young for $n to harm.",ch,0,victim,TO_VICT);
	return TRUE;
 	}
    if (!IS_NPC(ch) && ch->pcdata->newbie==TRUE && !IS_NPC(victim))
	{
	act("$n is too young to harm $N.",ch,0,victim,TO_NOTVICT);
	act("You are too young to harm $N. Type newbie to change this.",ch,0,victim,TO_CHAR);
	act("$n is too young to harm you.",ch,0,victim,TO_VICT);
	return TRUE;
 	}

    if (is_affected(victim,gsn_timestop))
    {
	act("$n is stuck in time and the gods protect $m.",victim,0,0,TO_ROOM);
	act("$N tries to do something but you are stuck in time and unaffected.",victim,0,ch,TO_CHAR);
	if (victim->fighting != NULL)
		victim->fighting = NULL;
	return TRUE;
    }

    if (victim->fighting == ch || victim == ch)
        return FALSE;

    if (IS_NPC(ch) && ch->last_fought == victim)
	return FALSE;


    if (victim->ghost > 0)
    {
	act("The gods protect $N from $n.",ch,0,victim,TO_NOTVICT);
	act("The gods protect you from $n.",ch,0,victim,TO_VICT);
	act("The gods protect $N from you.",ch,0,victim,TO_CHAR);
	return TRUE;
    }

    if (ch->ghost > 0)
    {
	if (!IS_NPC(victim))
	{
	act("The gods protect $N from $n.",ch,0,victim,TO_NOTVICT);
	act("The gods protect you from $n.",ch,0,victim,TO_VICT);
	act("The gods protect $N from you.",ch,0,victim,TO_CHAR);
		return TRUE;
	}
            if (IS_AFFECTED(victim,AFF_CHARM) && victim->master != NULL
		&& !IS_NPC(victim->master))
            {
  	act("The gods protect $N from $n.",ch,0,victim,TO_NOTVICT);
	act("The gods protect you from $n.",ch,0,victim,TO_VICT);
	act("The gods protect $N from you.",ch,0,victim,TO_CHAR);
                return TRUE;
            }

    }

    if (IS_IMMORTAL(ch) && is_affected(ch,gsn_riot) && !IS_NPC(victim))
    {
	send_to_char("You can't riot on mortals.\n\r",ch);
	return TRUE;
    }

    if (IS_IMMORTAL(ch) && ch->level > LEVEL_IMMORTAL)
        return FALSE;


    if (IS_SET(victim->act,PLR_CRIMINAL))
    {
	if (IS_NPC(ch) && ch->pIndexData->vnum == MOB_VNUM_ENFORCER)
		return FALSE;
    }
    if(IS_NPC(victim)&&victim->pIndexData->vnum==MOB_VNUM_ENFORCER)
	return FALSE;

    /* killing mobiles */
    if (IS_NPC(victim))
    {
        if (!IS_NPC(ch))
        {
            /* no charmed creatures unless in pk of master */
            if (IS_AFFECTED(victim,AFF_CHARM) && !can_pk(ch,victim->master) && !is_centurion(victim))
            {
  	act("The gods protect $N from $n.",ch,0,victim,TO_NOTVICT);
	act("The gods protect you from $n.",ch,0,victim,TO_VICT);
	act("The gods protect $N from you.",ch,0,victim,TO_CHAR);
                return TRUE;
            }
        }
	else
	{
	if (IS_AFFECTED(ch,AFF_CHARM) && IS_AFFECTED(victim,AFF_CHARM)
	&& !can_pk(ch->master,victim->master))
                return TRUE;
	}
    }
    /* killing players */
    else
    {
        /* NPC doing the killing */
        if (IS_NPC(ch))
        {
            /* charmed mobs and pets cannot attack players while owned */
            if (IS_AFFECTED(ch,AFF_CHARM) && ch->master != NULL
            &&  ch->master->fighting != victim && !is_centurion(ch))
            {
                send_to_char("Players are your friends!\n\r",ch);
                return TRUE;
            }
        }
        /* player doing the killing */
        else
        {
	    if (!can_pk(ch, victim))
    	    {
	if (!IS_IMMORTAL(victim)) {
  	act("The gods protect $N from $n.",ch,0,victim,TO_NOTVICT);
	act("The gods protect you from $n.",ch,0,victim,TO_VICT);
	act("The gods protect $N from you.",ch,0,victim,TO_CHAR);
		}
        	return TRUE;
    	    }
        }
    }
    return FALSE;
}

bool is_safe_spell(CHAR_DATA *ch, CHAR_DATA *victim, bool area )
{
    if (victim->in_room == NULL || ch->in_room == NULL)
        return TRUE;

    if (victim == ch && area)
        return TRUE;

    if (victim->fighting == ch || victim == ch)
        return FALSE;

    if (IS_IMMORTAL(ch) && ch->level > LEVEL_IMMORTAL && !area)
        return FALSE;

    if (!IS_NPC(victim) && victim->pcdata->newbie==TRUE && !IS_NPC(ch))
	  return TRUE;

    /* killing mobiles */
    if (IS_NPC(victim))
    {
        /* safe room? */
        if (IS_SET(victim->in_room->room_flags,ROOM_SAFE))
            return TRUE;

        if (victim->pIndexData->pShop != NULL)
            return TRUE;

        /* no killing healers, trainers, etc */
        if (IS_SET(victim->act,ACT_TRAIN)
        ||  IS_SET(victim->act,ACT_PRACTICE)
        ||  IS_SET(victim->act,ACT_IS_HEALER)
        ||  IS_SET(victim->act,ACT_IS_CHANGER))
            return TRUE;

        if (!IS_NPC(ch))
        {
            /* no pets */
            if (IS_SET(victim->act,ACT_PET))
                return TRUE;

            /* no charmed creatures unless owner */
            if (IS_AFFECTED(victim,AFF_CHARM) && (area || ch != victim->master))
                return TRUE;

            /* legal kill? -- cannot hit mob fighting non-group member */
            if (victim->fighting != NULL && !is_same_group(ch,victim->fighting))
                return TRUE;
        }
        else
        {
            /* area effect spells do not hit other mobs */
            if (area && !is_same_group(victim,ch->fighting))
                return TRUE;
        }
    }
    /* killing players */
    else
    {
        if (area && IS_IMMORTAL(victim) && victim->level > LEVEL_IMMORTAL)
            return TRUE;

        /* NPC doing the killing */
        if (IS_NPC(ch))
        {
            /* charmed mobs and pets cannot attack players while owned */
            if (IS_AFFECTED(ch,AFF_CHARM) && ch->master != NULL
            &&  ch->master->fighting != victim)
                return TRUE;

            /* safe room? */
            if (IS_SET(victim->in_room->room_flags,ROOM_SAFE))
                return TRUE;

            /* legal kill? -- mobs only hit players grouped with opponent*/
            if (ch->fighting != NULL && !is_same_group(ch->fighting,victim))
                return TRUE;
        }

        /* player doing the killing */
        else
        {
            if (!is_cabal(ch))
                return TRUE;

            if (IS_SET(victim->act,PLR_KILLER) || IS_SET(victim->act,PLR_THIEF))
                return FALSE;

            if (!is_cabal(victim))
                return TRUE;

            if (ch->level > victim->level + 8)
                return TRUE;
        }

    }
    return FALSE;
}
/*
 * See if an attack justifies a KILLER flag.
 */
void check_killer( CHAR_DATA *ch, CHAR_DATA *victim )
{
    /*
     * Follow charm thread to responsible character.
     * Attacking someone's charmed char is hostile!
     */
    while ( IS_AFFECTED(victim, AFF_CHARM) && victim->master != NULL )
        victim = victim->master;

    /*
     * NPC's are fair game.
     * So are killers and thieves.
     */
    if ( IS_NPC(victim)
    ||   IS_SET(victim->act, PLR_KILLER)
    ||   IS_SET(victim->act, PLR_THIEF))
        return;

    /*
     * NPC's are cool of course (as long as not charmed).
     * Hitting yourself is cool too (bleeding).
     * So is being immortal (Alander's idea).
     * And current killers stay as they are.
     */
    if ( IS_NPC(ch)
    ||   ch == victim
    ||   ch->level >= LEVEL_IMMORTAL
    ||   !is_cabal(ch)
    ||   IS_SET(ch->act, PLR_KILLER)
    ||   ch->fighting  == victim)
        return;

    save_char_obj( ch );
    return;
}

bool check_blademaster( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    int chance;
    int tsn;
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char *attack;

    if ( !IS_AWAKE(victim) )
        return FALSE;
		

    tsn=skill_lookup("blademaster");
    chance = get_skill(victim,tsn) / 3;
    if(chance==0)
	  return FALSE;
    if ( get_eq_char( victim, WEAR_WIELD ) == NULL )
    {
        if (IS_NPC(victim))
            chance /= 2;
        else
            return FALSE;
    }
    if (IS_AFFECTED(victim,AFF_SLOW))
        chance*=.5;
    if (IS_AFFECTED(victim,AFF_HASTE))
	  chance+=8;

    if (check_imperial_training(ch) == IMPERIAL_OFFENSE)
		chance -= 20;
    if (check_imperial_training(ch) == IMPERIAL_DEFENSE)
		chance += 20;

    chance+=(get_curr_stat(victim,STAT_DEX))*.6;
    if(chance>50)
	chance=50;
    if ( number_percent( ) >= chance)
        return FALSE;
    attack=get_dam_message(ch,dt);
    sprintf(buf1,"You block $n's %s with an agile cross parry.",attack);
    sprintf(buf2,"$N blocks your %s with an agile cross parry.",attack);
    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);
    check_improve(victim,tsn,TRUE,1);
    return TRUE;
}

//Hitroll vs hitroll check that returns the ratio between two characters' hitrolls, up to a predefined max_multiplier.
float hitroll_multiplier(CHAR_DATA *ch, CHAR_DATA *victim, float max_multiplier) {
	int ch_hitroll = GET_HITROLL(ch);
	int victim_hitroll = GET_HITROLL(victim);
	float hit_ratio;
	
	//Hitroll vs hitroll multiplier based on ratio
	if (victim_hitroll >= ch_hitroll) {
		hit_ratio = ch_hitroll == 0 ? ch_hitroll = 1 : ch_hitroll > 0 ? victim_hitroll / ch_hitroll : ch_hitroll / victim_hitroll;
		if (hit_ratio > max_multiplier)
			return max_multiplier;
		else 
			return hit_ratio;
	}
	return 1;
}

bool check_displacement(CHAR_DATA *ch, CHAR_DATA *victim)
{
    float chance, hit_ratio;
	int ch_hitroll, victim_hitroll;

    if (!(is_affected(victim, gsn_displacement)))
	return FALSE;
    chance = get_skill(victim,gsn_displacement);
    chance /= 1.5;
	
	ch_hitroll = GET_HITROLL(ch);
	victim_hitroll = GET_HITROLL(victim);
	
	//For simplicity...
	if (victim_hitroll <= 0) victim_hitroll = 1;
	if (ch_hitroll <= 0) ch_hitroll = 1;

	if (victim_hitroll < ch_hitroll) {
		hit_ratio = (float)ch_hitroll / (float)victim_hitroll;
		
		if (hit_ratio > 1.5)
			chance *= 1.5;
		else 
			chance *= hit_ratio;
	}

    if (!can_see(victim,ch))
	chance /=2;

    if (number_percent() > chance + victim->level - ch->level)
	return FALSE;

    act("$n's attack passes harmlessly through $N.",ch,NULL,victim,TO_NOTVICT);
    act("$n's attack misses you completely.",ch,NULL,victim,TO_VICT);
    act("Your attack passes harmlessly through $N.",ch,NULL,victim,TO_CHAR);
    return TRUE;
}

/*
 * Check for parry.
 */

bool check_parry( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    OBJ_DATA *wield, *victimwield;
    char *attack, buf1[MSL], buf2[MSL];
    int flourintine=0, ch_hitroll, victim_hitroll, roll;
	float chance, hit_ratio;

    if ( !IS_AWAKE(victim) )
        return FALSE;

    chance = get_skill(victim,gsn_parry) / 2;
	
	ch_hitroll = GET_HITROLL(ch);
	victim_hitroll = GET_HITROLL(victim);
	
	//For simplicity...
	if (victim_hitroll <= 0) victim_hitroll = 1;
	if (ch_hitroll <= 0) ch_hitroll = 1;

	if (victim_hitroll < ch_hitroll) {
		hit_ratio = (float)ch_hitroll / (float)victim_hitroll;
		
		if (hit_ratio > 1.25)
			chance *= 1.25;
		else 
			chance *= hit_ratio;
	}

    if ( get_eq_char( victim, WEAR_WIELD ) == NULL )
    {
        if (IS_NPC(victim))
            chance /= 2;
        else
            return FALSE;
    }

    if (IS_AFFECTED(victim,AFF_BLIND))
        chance *= .75;

    if (IS_AFFECTED(victim,AFF_HASTE))
	  chance+=20;

    if (IS_AFFECTED(ch,AFF_HASTE))
	  chance-=15;

    chance-=5*abs((ch->size-victim->size));

    if (dt  == gsn_dual_wield)
    	wield = get_eq_char(ch,WEAR_DUAL_WIELD);
    else
	wield = get_eq_char(ch,WEAR_WIELD);

    if (wield != NULL)
    {
    	switch (wield->value[0])
    	{
		case (WEAPON_SWORD): 	chance+=5;		break;
		case (WEAPON_DAGGER): 	chance-=5;		break;
		case (WEAPON_SPEAR):	chance+=0;		break;
		case (WEAPON_STAFF):	chance+=0;		break;
		case (WEAPON_MACE):	chance-=5;		break;
		case (WEAPON_AXE):	chance-=5;		break;
		case (WEAPON_FLAIL):	chance+=10;		break;
		case (WEAPON_WHIP):	chance+=10;		break;
		case (WEAPON_POLEARM):	chance-=5;		break;
		default: break;
    	}
    }

    if((victimwield=get_eq_char(victim,WEAR_WIELD)) != NULL)
    {
    	switch (victimwield->value[0])
    	{
		case (WEAPON_SWORD): 	chance+=10; flourintine++;		break;
		case (WEAPON_DAGGER): 	chance-=20;		break;
		case (WEAPON_SPEAR):	chance+=15;		break;
		case (WEAPON_STAFF):	chance+=15;		break;
		case (WEAPON_MACE):	chance-=5;		break;
		case (WEAPON_AXE):	chance-=5;		break;
		case (WEAPON_FLAIL):	chance-=10;		break;
		case (WEAPON_WHIP):	chance-=10;		break;
		case (WEAPON_POLEARM):	chance+=10;		break;
		default: break;
    	}	
    }
	
    if ((get_skill(victim,gsn_flourintine) > 0) && flourintine == 1) 
    {
	check_improve(victim,gsn_flourintine,TRUE,1);
	chance += get_skill(victim,gsn_flourintine) / 7;
    }

    /* imperial training */
    if (check_imperial_training(ch) == IMPERIAL_OFFENSE)
	chance -= 20;
    if (check_imperial_training(victim) == IMPERIAL_DEFENSE)
	chance += 20;
    if (check_imperial_training(victim) == IMPERIAL_OFFENSE)
	chance -= 20;
    if (check_imperial_training(ch) == IMPERIAL_DEFENSE)
	chance += 20;
        
    if (!IS_NPC(victim) && IS_SET(victim->act,PLR_MORON))
 	chance /= 2;
	
	switch(get_curr_stat(victim,STAT_STR)) {
		case 25: chance += 10; break;
		case 24: chance += 8; break;
		case 23: chance += 6; break;
		case 22: chance += 4; break;
		case 21: chance += 3; break;
		case 20: chance += 2; break;
		case 19: chance += 1; break;
		default: chance += 0; break;
	}

    chance+=attack_table[get_attack_number(ch,dt)].modifier;

    if (chance > 95)	
	chance = 95;

	attack = get_dam_message(ch,dt);
	roll = number_percent();
    if ( roll >= chance)
       return FALSE;

    if (chance - roll >= 40) {
		sprintf(buf1,"You masterfully deflect $n's %s with your %s.",attack, victimwield ? weapon_name_lookup(victimwield->value[0]) : "hands");
		sprintf(buf2,"$N masterfully deflects your %s with $S %s.",attack, victimwield ? weapon_name_lookup(victimwield->value[0]) : "hands");
	}
	else if (chance - roll >= 30) {
		sprintf(buf1,"You gracefully parry $n's %s with your %s.",attack, victimwield ? weapon_name_lookup(victimwield->value[0]) : "hands");
		sprintf(buf2,"$N gracefully parries your %s with $S %s.",attack, victimwield ? weapon_name_lookup(victimwield->value[0]) : "hands");
	}
	else if (chance - roll >= 20) {
		sprintf(buf1,"You parry $n's %s with your %s.",attack, victimwield ? weapon_name_lookup(victimwield->value[0]) : "hands");
		sprintf(buf2,"$N parries your %s with $S %s.",attack, victimwield ? weapon_name_lookup(victimwield->value[0]) : "hands");
	}
	else if (chance - roll >= 10) {
		sprintf(buf1,"You manage to parry $n's %s with your %s.",attack, victimwield ? weapon_name_lookup(victimwield->value[0]) : "hands");
		sprintf(buf2,"$N manages to parry your %s with $S %s.",attack, victimwield ? weapon_name_lookup(victimwield->value[0]) : "hands");
	}
	else {
		sprintf(buf1,"You barely manage to parry $n's %s with your %s.",attack, victimwield ? weapon_name_lookup(victimwield->value[0]) : "hands");
		sprintf(buf2,"$N barely manages to parry your %s with $S %s.",attack, victimwield ? weapon_name_lookup(victimwield->value[0]) : "hands");
	}
	
    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);
    check_improve(victim,gsn_parry,TRUE,2);
    return TRUE;
}

bool check_block( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    float chance, hit_ratio;
    int tmp_dt, ch_hitroll, victim_hitroll, roll;
    OBJ_DATA *wield;
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char *attack;

    if ( !IS_AWAKE(victim) )
        return FALSE;
    if (IS_NPC(victim))
	return FALSE;

    chance = get_skill(victim,gsn_block);

    if ( victim->level <skill_table[gsn_block].skill_level[victim->class])
  	return FALSE;

    chance /= 2.25;
	
	ch_hitroll = GET_HITROLL(ch);
	victim_hitroll = GET_HITROLL(victim);
	
	//For simplicity...
	if (victim_hitroll <= 0) victim_hitroll = 1;
	if (ch_hitroll <= 0) ch_hitroll = 1;

	if (victim_hitroll < ch_hitroll) {
		hit_ratio = (float)ch_hitroll / (float)victim_hitroll;
		
		if (hit_ratio > 1.25)
			chance *= 1.25;
		else 
			chance *= hit_ratio;
	}

    if (get_eq_char(victim,WEAR_WIELD) != NULL)
	return FALSE;

    if (!can_see(ch,victim))
        chance /= 2;
    if (is_affected(victim,gsn_rage))
        chance *= .75;
    if ( number_percent( ) >= chance + (victim->drain_level +  victim->level) - (ch->drain_level + ch->level) )
    {
	check_improve(victim,gsn_block,FALSE,5);
        return FALSE;
    }

/* The rest is all new dual wield stuff, with dam types displayed for
clarity when dual wielding - Ceran */
    tmp_dt = TYPE_HIT;
    if (dt  == gsn_dual_wield)
    	wield = get_eq_char(ch,WEAR_DUAL_WIELD);
    else
	wield = get_eq_char(ch,WEAR_WIELD);


    if (wield != NULL)
	tmp_dt += wield->value[3];
    else
	tmp_dt += ch->dam_type;


    if (tmp_dt > TYPE_HIT && tmp_dt < TYPE_HIT + MAX_DAMAGE_MESSAGE)
	attack = attack_table[tmp_dt - TYPE_HIT].noun;
    else
    {
	bug("In check_block, bad dam message: dt %d.",tmp_dt);
	attack = attack_table[0].name;
    }

    if (chance - roll >= 40) {
		sprintf(buf1,"You masterfully deflect $n's %s with your hands.",attack);
		sprintf(buf2,"$N masterfully deflects your %s with $S hands.",attack);
	}
	else if (chance - roll >= 30) {
		sprintf(buf1,"You gracefully block $n's %s with your hands.",attack);
		sprintf(buf2,"$N gracefully blocks your %s with $S hands.",attack);
	}
	else if (chance - roll >= 20) {
		sprintf(buf1,"You block $n's %s with your hands.",attack);
		sprintf(buf2,"$N blocks your %s with $S hands.",attack);
	}
	else if (chance - roll >= 10) {
		sprintf(buf1,"You manage to block $n's %s with your hands.",attack);
		sprintf(buf2,"$N manages to block your %s with $S hands.",attack);
	}
	else {
		sprintf(buf1,"You barely manage to block $n's %s with your hands.",attack);
		sprintf(buf2,"$N barely manages to block your %s with $S hands.",attack);
	}
	
    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);
    check_improve(victim,gsn_block,TRUE,6);
    return TRUE;
}

/*
 * Check for shield block.
 */

bool check_shield_block( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    float chance, hit_ratio;
	int ch_hitroll, victim_hitroll, roll;
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char *attack;

    if ( !IS_AWAKE(victim) )
        return FALSE;

    if(victim->level < skill_table[gsn_shield_block].skill_level[victim->class])
	return FALSE;

    chance = (( get_skill(victim,gsn_shield_block) / 4) + 15);
	
	ch_hitroll = GET_HITROLL(ch);
	victim_hitroll = GET_HITROLL(victim);
	
	//For simplicity...
	if (victim_hitroll <= 0) victim_hitroll = 1;
	if (ch_hitroll <= 0) ch_hitroll = 1;

	if (victim_hitroll < ch_hitroll) {
		hit_ratio = (float)ch_hitroll / (float)victim_hitroll;
		
		if (hit_ratio > 1.35)
			chance *= 1.35;
		else 
			chance *= hit_ratio;
	}

	/* imperial training */
    if (check_imperial_training(ch) == IMPERIAL_OFFENSE)
	chance -= 10;
    if (check_imperial_training(victim) == IMPERIAL_DEFENSE)
	chance += 10;
    if (check_imperial_training(victim) == IMPERIAL_OFFENSE)
	chance -= 20;
    if (check_imperial_training(ch) == IMPERIAL_DEFENSE)
	chance += 20;

    if (is_affected(victim,gsn_shield_of_purity) && get_eq_char(victim,WEAR_SHIELD) != NULL)
	chance += 30;

    if (is_affected(victim,gsn_shield_of_justice) && get_eq_char(victim,WEAR_SHIELD) != NULL)
	chance -= 30;

	switch(get_curr_stat(victim,STAT_STR)) {
		case 25: chance += 13; break;
		case 24: chance += 10; break;
		case 23: chance += 9; break;
		case 22: chance += 7; break;
		case 20: chance += 4; break;
		case 19: chance += 2; break;
		default: chance += 0; break;
	}
	
	switch(get_curr_stat(victim,STAT_DEX)) {
		case 25: chance += 8; break;
		case 24: chance += 7; break;
		case 23: chance += 6; break;
		case 22: chance += 5; break;
		case 20: chance += 4; break;
		case 19: chance += 3; break;
		default: chance += 0; break;
	}
	
    if ( get_eq_char( victim, WEAR_SHIELD ) == NULL )
        return FALSE;
		

	roll = number_percent();
    if ( roll >= chance  )
        return FALSE;

    attack=get_dam_message(ch,dt);
	
    if (!is_affected(victim,gsn_shield_of_purity) && !is_affected(victim,gsn_shield_of_justice))
    {
    	sprintf(buf1,"You block $n's %s with your shield.",attack);
    	sprintf(buf2,"$N shield blocks your %s.",attack);
    }
    
    else if (is_affected(victim,gsn_shield_of_purity) && get_eq_char(victim,WEAR_SHIELD) != NULL)
    {
	sprintf(buf1,"Your god guides your shield into the path of $n's %s.",attack);
	sprintf(buf2,"$N gracefully deflects your %s with his divine shield.",attack);
    }

    else if (is_affected(victim,gsn_shield_of_justice) && get_eq_char(victim,WEAR_SHIELD) != NULL)
    {
	sprintf(buf1,"Blocking $n's %s with your divine shield, you execute a powerful counterattack!",attack);
	sprintf(buf2,"Blocking your %s with his divine shield, $N executes a powerful counterattack!",attack);
    }
	else {
		if (chance - roll >= 40) {
			sprintf(buf1,"You masterfully block $n's %s with your shield.",attack);
			sprintf(buf2,"$N masterfully blocks your %s with $S shield.",attack);
		}
		else if (chance - roll >= 30) {
			sprintf(buf1,"You gracefully block $n's %s with your shield.",attack);
			sprintf(buf2,"$N gracefully blocks your %s with $S shield.",attack);
		}
		else if (chance - roll >= 20) {
			sprintf(buf1,"You block $n's %s with your shield.",attack);
			sprintf(buf2,"$N blocks your %s with $S shield.",attack);
		}
		else if (chance - roll >= 10) {
			sprintf(buf1,"You manage to block $n's %s with your shield.",attack);
			sprintf(buf2,"$N manages to block your %s with $S shield.",attack);
		}
		else {
			sprintf(buf1,"You barely manage to block $n's %s with your shield.",attack);
			sprintf(buf2,"$N barely manages to block your %s with $S shield.",attack);
		}
	}

    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);
    if (is_affected(victim, gsn_shield_of_justice) && get_eq_char(victim,WEAR_SHIELD) != NULL)
one_hit_new(victim,ch,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,200,"strike of justice");
    check_improve(victim,gsn_shield_block,TRUE,4);
    return TRUE;
}

/*
 * Check for dodge.
 */

// attacker = ch, defender = victim.  Changed variable names for clarity.

bool check_dodge( CHAR_DATA *attacker, CHAR_DATA *defender, int dt )
{
    char buf1[MSL], buf2[MSL], *attack;
    int dex, dexa, fam, attacker_hitroll, defender_hitroll, roll;
	float hit_ratio, chance;

    if ( !IS_AWAKE(defender) )
        return FALSE;

    chance = ( 3 * get_skill(defender,gsn_dodge) / 10);

    dex = get_curr_stat(defender,STAT_DEX);
    dexa= get_curr_stat(attacker,STAT_DEX);
	
	attacker_hitroll = GET_HITROLL(attacker);
	defender_hitroll = GET_HITROLL(defender);
	
	//For simplicity...
	if (defender_hitroll <= 0) defender_hitroll = 1;
	if (attacker_hitroll <= 0) attacker_hitroll = 1;
	
	//Only increase the chance if the defender has more hitroll than the attacker. This is to prevent mage
	//classes from getting gibbed since they don't stack hitroll right now. Dodge will stay the same for
	//mages but increase for melee classes that utilize hitroll up to 1.5x the base chance, which can be at
	//most 30%. Zornath
	if (defender_hitroll > attacker_hitroll) {
		hit_ratio = (float)defender_hitroll / (float)attacker_hitroll;
		
		if (hit_ratio > 1.5)
			chance *= 1.5;
		else 
			chance *= hit_ratio;
	}

    if (dex <= 5)
	chance += 0;
    else if (dex <= 10)
	chance += dex/2;
    else if (dex <=15)
	chance += (2*dex/3);
    else if (dex <= 20)
	chance += dex;
    else
	chance += 3*dex/2;

    chance+=dex-dexa;
    chance+=(attacker->size-defender->size)*5;

    fam = get_skill(defender,gsn_wilderness_fam);

    if(( fam > 0) && isInWilderness(defender))
	chance += fam/2;

    /* imperial training */
    if (check_imperial_training(attacker) == IMPERIAL_OFFENSE)
	chance -= 20;
    if (check_imperial_training(attacker) == IMPERIAL_DEFENSE)
	chance += 20;
    if (check_imperial_training(defender) == IMPERIAL_OFFENSE)
	chance -= 20;
    if (check_imperial_training(defender) == IMPERIAL_DEFENSE)
	chance += 20;
    if (!can_see(defender,attacker))
        chance /= 2;
    if (!IS_NPC(defender) && IS_SET(defender->act,PLR_MORON))
	chance /= 2;

    // haste/slow
    if (IS_AFFECTED(defender,AFF_HASTE))
	chance += 20;
    if (IS_AFFECTED(defender,AFF_SLOW))
	chance -= 20;
    if (IS_AFFECTED(attacker,AFF_HASTE))
        chance -= 20;
    if (IS_AFFECTED(attacker,AFF_SLOW))
        chance += 20;

    if (chance > 95)    
	chance = 95; 
	
	roll = number_percent();
    if ( roll >= chance)
        return FALSE;

    attack=get_dam_message(attacker,dt);
    if (chance - roll >= 40) {
		sprintf(buf1,"You deftly dodge $n's %s.",attack);
		sprintf(buf2,"$N deftly dodges your %s.",attack);
	}
	else if (chance - roll >= 30) {
		sprintf(buf1,"You gracefully sidestep $n's %s.",attack);
		sprintf(buf2,"$N gracefully sidesteps your %s.",attack);
	}
	else if (chance - roll >= 20) {
		sprintf(buf1,"You dodge $n's %s.",attack);
		sprintf(buf2,"$N dodges your %s.",attack);
	}
	else if (chance - roll >= 10) {
		sprintf(buf1,"You manage to avoid $n's %s.",attack);
		sprintf(buf2,"$N manages to avoid your %s.",attack);
	}
	else {
		sprintf(buf1,"You barely escape $n's %s.",attack);
		sprintf(buf2,"$N barely escapes your %s.",attack);
	}
	

    if(isInWilderness(defender) && check_fam(defender,attack))
	return TRUE;

    act(buf1,attacker,0,defender,TO_VICT);
    act(buf2,attacker,0,defender,TO_CHAR);
    check_improve(defender,gsn_dodge,TRUE,5);
    return TRUE;
}


/*
 * Check for quest dodge.
 */

bool check_questdodge( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    float chance, hit_ratio;
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char *attack;
    int dex, dexa, ch_hitroll, victim_hitroll, roll;

    if ( !IS_AWAKE(victim) )
        return FALSE;

	if (get_skill(victim,gsn_questdodge) == 0)
		return FALSE;

	chance = ( 3 * get_skill(victim,gsn_questdodge) / 7);
	
	ch_hitroll = GET_HITROLL(ch);
	victim_hitroll = GET_HITROLL(victim);
	
	//For simplicity...
	if (victim_hitroll <= 0) victim_hitroll = 1;
	if (ch_hitroll <= 0) ch_hitroll = 1;

	if (victim_hitroll < ch_hitroll) {
		hit_ratio = (float)ch_hitroll / (float)victim_hitroll;
		
		if (hit_ratio > 1.3)
			chance *= 1.3;
		else 
			chance *= hit_ratio;
	}
    dex = get_curr_stat(victim,STAT_DEX);
    dexa= get_curr_stat(ch,STAT_DEX);
    if (dex <= 5)
	chance += 0;
    else if (dex <= 10)
	chance += dex/2;
    else if (dex <=15)
	chance += (2*dex/3);
    else if (dex <= 20)
	chance += (8*dex/10);
    else
	chance += dex;
    chance+=dex-dexa;
    chance+=(ch->size-victim->size)*5;
    if (is_affected(ch,gsn_executioner))
	  chance+=30;

	roll = number_percent();
    if ( roll >= chance)
      return FALSE;
    attack=get_dam_message(ch,dt);
	if (chance - roll >= 40) {
		sprintf(buf1,"You deftly evade $n's %s.",attack);
		sprintf(buf2,"$N deftly evades your %s.",attack);
	}
	else if (chance - roll >= 30) {
		sprintf(buf1,"You gracefully dart around $n's %s.",attack);
		sprintf(buf2,"$N gracefully darts around your %s.",attack);
	}
	else if (chance - roll >= 20) {
		sprintf(buf1,"You evade $n's %s.",attack);
		sprintf(buf2,"$N evades your %s.",attack);
	}
	else if (chance - roll >= 10) {
		sprintf(buf1,"You manage to evade $n's %s.",attack);
		sprintf(buf2,"$N manages to evade your %s.",attack);
	}
	else {
		sprintf(buf1,"You barely evade $n's %s.",attack);
		sprintf(buf2,"$N barely evades your %s.",attack);
	}
    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);
    check_improve(victim,gsn_questdodge,TRUE,5);
    return TRUE;
}

/*
 * Check for defensive spin.
 */

bool check_spin( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
	float chance, hit_ratio;
	int tmp_dt, victim_hitroll, ch_hitroll, roll;
	OBJ_DATA *wield;
	char buf1[MAX_STRING_LENGTH];
	char buf2[MAX_STRING_LENGTH];
	char *attack;
	
	if ((chance = get_skill(victim,gsn_spin) / 2) == 0) {
		return FALSE;
	}

	ch_hitroll = GET_HITROLL(ch);
	victim_hitroll = GET_HITROLL(victim);
	
	//For simplicity...
	if (victim_hitroll <= 0) victim_hitroll = 1;
	if (ch_hitroll <= 0) ch_hitroll = 1;

	if (victim_hitroll < ch_hitroll) {
		hit_ratio = (float)ch_hitroll / (float)victim_hitroll;
		
		if (hit_ratio > 1.25)
			chance *= 1.25;
		else 
			chance *= hit_ratio;
	}
	
	wield = get_eq_char(victim,WEAR_WIELD);

	if (wield == NULL) {
		return FALSE;
	}

	if (wield != NULL)
	{
		switch (wield->value[0])
		{
			case (WEAPON_STAFF):
			 	chance+= get_skill(victim,gsn_staff) / 10;
				break;
			case (WEAPON_SPEAR):
				chance+= get_skill(victim,gsn_spear) / 10;
				break;
			default:
				return FALSE;
				break;
		}
	}

	if ( !IS_AWAKE(victim) )
	return FALSE;

	if ( get_eq_char( victim, WEAR_WIELD ) == NULL )
	{
		if (IS_NPC(victim))
			chance /= 2;
		else
			return FALSE;
	}

	if (IS_AFFECTED(victim,AFF_BLIND))
		chance *= .75;
	if (IS_AFFECTED(victim,AFF_HASTE))
		chance+=20;
	if (IS_AFFECTED(ch,AFF_HASTE))
		chance-=15;

	switch(get_curr_stat(victim,STAT_STR)) {
		case 25: chance += 13; break;
		case 24: chance += 10; break;
		case 23: chance += 9; break;
		case 22: chance += 7; break;
		case 20: chance += 4; break;
		case 19: chance += 2; break;
		default: chance += 0; break;
	}
	
	switch(get_curr_stat(victim,STAT_DEX)) {
		case 25: chance += 6; break;
		case 24: chance += 5; break;
		case 23: chance += 4; break;
		case 22: chance += 3; break;
		case 20: chance += 2; break;
		case 19: chance += 1; break;
		default: chance += 0; break;
	}
	
	tmp_dt = TYPE_HIT;

	roll = number_percent();
	if ( roll >= chance)
	{
		check_improve(victim,gsn_spin,FALSE,2);
		return FALSE;
	}

    attack=get_dam_message(ch,dt);
    if (chance - roll >= 40) {
		sprintf(buf1,"You masterfully spin your %s, blocking $n's %s.", wield ? weapon_name_lookup(wield->value[0]) : "hands", attack);
		sprintf(buf2,"$N masterfully spins $S %s, blocking your %s.",wield ? weapon_name_lookup(wield->value[0]) : "hands",attack);
	}
	else if (chance - roll >= 30) {
		sprintf(buf1,"You gracefully deflect $n's %s with a defensive spin.",attack);
		sprintf(buf2,"$N gracefully deflects your %s with a defensive spin.",attack);
	}
	else if (chance - roll >= 20) {
		sprintf(buf1,"You spin your %s and fend off $n's %s.",wield ? weapon_name_lookup(wield->value[0]) : "hands",attack);
		sprintf(buf2,"$N spins $S %s, fending off your %s.",wield ? weapon_name_lookup(wield->value[0]) : "hands",attack);
	}
	else if (chance - roll >= 10) {
		sprintf(buf1,"You manage to spin and deflect $n's %s.",attack);
		sprintf(buf2,"$N manages to spin and deflect your %s.",attack);
	}
	else {
		sprintf(buf1,"You barely deflect $n's %s with a defensive spin.",attack);
		sprintf(buf2,"$N barely deflects your %s with a defensive spin.",attack);
	}
    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);
    check_improve(victim,gsn_spin,TRUE,2);
    return TRUE;
}

bool check_distance( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    int sn = skill_lookup("distance"), ch_hitroll, victim_hitroll;
	float chance, hit_ratio;
    OBJ_DATA *wield;
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char *attack;

    if ( !IS_AWAKE(victim) )
        return FALSE;

    if(get_skill(victim,sn)<5)
	return FALSE;

    chance = (( get_skill(victim,sn) / 4) + 15);
	
	ch_hitroll = GET_HITROLL(ch);
	victim_hitroll = GET_HITROLL(victim);
	
	//For simplicity...
	if (victim_hitroll <= 0) victim_hitroll = 1;
	if (ch_hitroll <= 0) ch_hitroll = 1;

	if (victim_hitroll < ch_hitroll) {
		hit_ratio = (float)ch_hitroll / (float)victim_hitroll;
		
		if (hit_ratio > 1.35)
			chance *= 1.35;
		else 
			chance *= hit_ratio;
	}

	/* imperial training */
	if (check_imperial_training(ch) == IMPERIAL_OFFENSE)
		chance -= 10;
	if (check_imperial_training(victim) == IMPERIAL_DEFENSE)
		chance += 10;
	if (check_imperial_training(victim) == IMPERIAL_OFFENSE)
		chance -= 20;
	if (check_imperial_training(ch) == IMPERIAL_DEFENSE)
		chance += 20;
    chance+=20*(victim->size-ch->size);
    chance+=10*(get_curr_stat(victim,STAT_STR)-get_curr_stat(ch,STAT_STR));
    if(IS_AFFECTED(victim,AFF_HASTE))
		chance+=15;
    if(IS_AFFECTED(ch,AFF_HASTE))
		chance-=15;
    if(((wield=get_eq_char(victim,WEAR_WIELD)) == NULL || wield->value[0]!=WEAPON_POLEARM))
		return FALSE;
    chance += (victim->drain_level + victim->level - (ch->drain_level + ch->level));
    chance=URANGE(5,chance,65);
    if ( number_percent( ) >= chance  )
        return FALSE;
    attack=get_dam_message(ch,dt);
    if(number_percent()<(chance/4))
    {
	 sprintf(buf1,"You keep $n at bay with your $p, knocking $m off balance with a counter strike.");
	 sprintf(buf2,"$N keeps you at bay with $S $p, knocking you off balance with a counter strike.");
    	 act(buf1,ch,wield,victim,TO_VICT);
    	 act(buf2,ch,wield,victim,TO_CHAR);
	 sprintf(buf1,"$N keeps $n at bay with $S $p, knocking $m off balance with a counter strike.");
	 act(buf1,ch,wield,victim,TO_NOTVICT);
	 one_hit_new(victim,ch,sn,HIT_SPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,75,NULL);
	 WAIT_STATE(ch,PULSE_VIOLENCE*1.5);
	 return TRUE;
    }
    sprintf(buf1,"You keep $n's %s at bay with your polearm.",attack);
    sprintf(buf2,"$N keeps your %s at bay with $s polearm.",attack);
    if(IS_SET(victim->wiznet,WIZ_PERCENT))
        sprintf(buf1,"You keep $n's %s at bay with your polearm. (%.0f%%)",attack,chance);
    if(IS_SET(ch->wiznet,WIZ_PERCENT))
        sprintf(buf2,"$N keeps your %s at bay with $S polearm. (%.0f%%)",attack,chance);
    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);
    check_improve(victim,sn,TRUE,2);
    return TRUE;
}


/*
 * Set position of a victim.
 */
void update_pos( CHAR_DATA *victim )
{
    if ( victim->hit > 0 )
    {
        if ( victim->position <= POS_STUNNED )
            victim->position = POS_STANDING;
        return;
    }

    if ( IS_NPC(victim) && victim->hit < 1 )
    {
        victim->position = POS_DEAD;
        return;
    }

    if ( victim->hit <= -11 )
    {
        victim->position = POS_DEAD;
        return;
    }

         if ( victim->hit <= -6 ) victim->position = POS_MORTAL;
    else if ( victim->hit <= -3 ) victim->position = POS_INCAP;
    else                          victim->position = POS_STUNNED;

    return;
}



/*
 * Start fights.
 */
void set_fighting( CHAR_DATA *ch, CHAR_DATA *victim )
{
    	char buf[MAX_STRING_LENGTH];

    	if ( ch->fighting != NULL )
        	return;

	if (ch == victim)
		return;

    	if ( IS_AFFECTED(ch, AFF_SLEEP) )
        	affect_strip( ch, gsn_sleep );
			
		if ( is_affected(victim, skill_lookup("resurrect"))) {
			send_to_char("{RYou lose your focus and the resurrection fails!{x\n\r",victim );
			affect_strip(victim,skill_lookup("resurrect"));
		}
		
		if ( is_affected(ch, skill_lookup("resurrect"))) {
			send_to_char("{RYou lose your focus and the resurrection fails!{x\n\r",ch );
			affect_strip(ch,skill_lookup("resurrect"));
		}

    	if (is_affected(victim,gsn_trip_wire))
    	{
    		send_to_char("You lost your trip wire in the frey.\n\r",victim);
		affect_strip(victim,gsn_trip_wire);
    	}

    	if (is_affected(ch,gsn_trip_wire))
    	{
    		send_to_char("You lost your trip wire in the frey.\n\r",ch);
    		affect_strip(ch,gsn_trip_wire);
    	}

    	ch->fighting = victim;
    	ch->position = POS_FIGHTING;
	ch->pause = UMAX(ch->pause, 10);
	victim->pause = UMAX(victim->pause, 10);

    	if (is_centurion(victim))
    	{
		sprintf(buf,"empire Help! %s is attacking me at %s!",PERS(ch,victim), victim->in_room->name);
		do_ccb(victim,buf);
    	}

    	if (IS_SET(victim->progtypes,MPROG_ATTACK))
       		(victim->pIndexData->mprogs->attack_prog) (victim,ch);

    	return;
}

/*
 * Stop fights.
 */
void stop_fighting( CHAR_DATA *ch, bool fBoth )
{
    CHAR_DATA *fch;

    for ( fch = char_list; fch != NULL; fch = fch->next )
    {
        if ( fch == ch || ( fBoth && fch->fighting == ch ) )
        {
            fch->fighting       = NULL;
            fch->position       = IS_NPC(fch) ? fch->default_pos : POS_STANDING;
            update_pos( fch );
        }
    }

    return;
}



/*
 * Make a corpse out of a character.
 */
void make_corpse( CHAR_DATA *killer,CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *corpse;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    OBJ_DATA *head;
    char *name, wizbuf[MAX_STRING_LENGTH], *objown;
	MOB_INDEX_DATA *pMobIndex;

	objown = NULL;

    if (IS_NPC(ch) && ch->pIndexData->vnum == 13724)
    {
	act("One of $n's massive heads falls decapitated onto the ground.",ch,0,0,TO_ROOM);
    	head = create_object(get_obj_index(OBJ_VNUM_HYDRA_HEAD),0);
	if (head != NULL)
	obj_to_room(head,ch->in_room);
    }

    if ( IS_NPC(ch) )
    {
        name            = str_dup(ch->short_descr);
        corpse          = create_object(get_obj_index(OBJ_VNUM_CORPSE_NPC), 0);
        corpse->corpse_of = str_dup(ch->short_descr);
		corpse->timer   = number_range( 5, 8 );

		free_string(corpse->talked);
		if (IS_NPC(killer))
			corpse->talked = str_dup(killer->short_descr);
		else
			corpse->talked = str_dup(killer->name);
			
			pMobIndex = ch->pIndexData;
			
		// Begin gold calculation
		float gold = 0;
		
		//presumably single kills
		if (ch->level < 30) gold = 10 * (ch->level) + number_range(1,10);
		else if (ch->level <= 53) { gold = 52.1*(ch->level-30) + number_range(275,300); gold -= pMobIndex->killed * 10; }
		else if (ch->level <= 60) { gold = 214.2 *(ch->level-53) + number_range(1300,1500); gold -= pMobIndex->killed * 30; }
		else if (ch->level <= 70) { gold = 500*(ch->level-60) + number_range(2700,3000); gold -= pMobIndex->killed * 100; }
		
		//now we're getting to possibly high-level group kills, so the gold scales much faster to compensate
		else if (ch->level <= 75) { gold = 1200*(ch->level-70) + number_range(7500,8000); gold -= pMobIndex->killed * 250; }
		else if (ch->level <= 80) { gold = 16000*(ch->level-75) + number_range(13300,14000); gold -= pMobIndex->killed * 450; }
		else { gold = 20000*(ch->level-80) + number_range(90000,94000); gold -= pMobIndex->killed * 1000; }
		// End gold calculation
		
		obj_to_obj( create_money((int)gold), corpse );
		corpse->cost = 0;
    }
    else
    {
        name            = str_dup(ch->name);
        corpse          = create_object(get_obj_index(OBJ_VNUM_CORPSE_PC), 0);
		corpse->corpse_of = str_dup(ch->name);
        corpse->timer   = number_range( 20, 35 );
    	if(ch->in_room && IS_EXPLORE(ch->in_room))
        	corpse->timer   = number_range(6,9);
	corpse->value[4] = ch->hometown;
        REMOVE_BIT(ch->act,PLR_CANLOOT);
	free_string(corpse->talked);
	if (IS_NPC(killer))
		corpse->talked = str_dup(killer->short_descr);
	else
		corpse->talked = str_dup(killer->name);
	
	corpse->owner = str_dup(ch->original_name);

            if (ch->gold > 1)
            {
                obj_to_obj(create_money(ch->gold), corpse);
		ch->gold = 0;
            }

        corpse->cost = 0;
    }

    corpse->level = ch->level;
    corpse->owner_class = ch->class;
     if (IS_SET(ch->act,ACT_UNDEAD) || IS_SET(ch->form,FORM_UNDEAD))
	set_bit(&corpse->extra_flags,CORPSE_NO_ANIMATE);


    sprintf( buf, corpse->short_descr, name );
    free_string( corpse->short_descr );
    corpse->short_descr = str_dup( buf );

    sprintf( buf, corpse->description, name );
    free_string( corpse->description );
    corpse->description = str_dup( buf );
    corpse->ohp = ch->max_hit;
    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
		bool takeItem = TRUE;
		bool putInCorpse = TRUE;
        obj_next = obj->next_content;

		if(!IS_NPC(ch))
			objown = str_dup(obj->carried_by->original_name);
	        
	    if (obj->wear_loc == WEAR_BRAND)
			takeItem = FALSE;
		
		if (obj->owner != NULL)
		{
			if (!str_cmp(obj->owner,ch->original_name)) {
				act("$p flashes brightly and returns to $n!",ch,obj,NULL,TO_ROOM);
				act("$p pulses brightly beside you as you collapse, following you through death itself.",ch,obj,NULL,TO_CHAR);
				takeItem = FALSE;
			}
		}

		if (takeItem)
			obj_from_char( obj );

	    if (obj->item_type == ITEM_POTION)
	        obj->timer = number_range(500,1000);
	    if (obj->item_type == ITEM_SCROLL)
	        obj->timer = number_range(1000,2500);

		if (is_set(&obj->extra_flags,ITEM_ROT_DEATH))
		{
			obj->timer = 2;
				remove_bit(&obj->extra_flags,ITEM_ROT_DEATH);
		}

		remove_bit(&obj->extra_flags,ITEM_VIS_DEATH);

		
		if (obj->pIndexData->limcount > obj->pIndexData->limtotal
			 && obj->pIndexData->limtotal > 0 && !IS_NPC(ch)
                         && (number_percent() < (abs( 1. - (obj->pIndexData->limtotal / obj->pIndexData->limcount )) * 100.) ))
		{
			act("$p flashes brightly and vaporizes.",ch,obj,NULL,TO_ROOM);
                        act("$p flashes brightly and vaporizes.",ch,obj,NULL,TO_CHAR);
			extract_obj( obj );
			putInCorpse = FALSE;
		}
		
	    if (obj->wear_loc == WEAR_BRAND)
			putInCorpse = FALSE;

		if (obj->owner != NULL)		
			if (!str_cmp(obj->owner,ch->original_name))
				putInCorpse = FALSE;
		
		if (putInCorpse) {
			obj_to_obj( obj, corpse );
			if(!IS_NPC(ch)) {
				free_string(wizbuf);
				free_string(objown);
			}
		}	
    }

    obj_to_room( corpse, ch->in_room );

    return;
}



/*
 * Improved Death_cry contributed by Diavolo.
 */
void death_cry( CHAR_DATA *ch )
{
    ROOM_INDEX_DATA *was_in_room;
    char *msg;
    int door;
    long vnum;

    vnum = 0;
    msg = "You hear $n's death cry.";

    switch ( number_bits(4))
    {
    case  0: msg  = "$n hits the ground ... DEAD.";                     break;
    case  1:
        if (ch->material == 0)
        {
            msg  = "$n splatters blood on your armor.";
            break;
        }
    case  2:
        if (IS_SET(ch->parts,PART_GUTS))
        {
            msg = "$n spills $s guts all over the floor.";
            vnum = OBJ_VNUM_GUTS;
        }
        break;
    case  3:
        if (IS_SET(ch->parts,PART_HEAD))
        {
            msg  = "$n's severed head plops on the ground.";
            vnum = OBJ_VNUM_SEVERED_HEAD;
        }
        break;
    case  4:
        if (IS_SET(ch->parts,PART_HEART))
        {
            msg  = "$n's heart is torn from $s chest.";
            vnum = OBJ_VNUM_TORN_HEART;
        }
        break;
    case  5:
        if (IS_SET(ch->parts,PART_ARMS))
        {
            msg  = "$n's arm is sliced from $s dead body.";
            vnum = OBJ_VNUM_SLICED_ARM;
        }
        break;
    case  6:
        if (IS_SET(ch->parts,PART_LEGS))
        {
            msg  = "$n's leg is sliced from $s dead body.";
            vnum = OBJ_VNUM_SLICED_LEG;
        }
        break;
    case 7:
        if (IS_SET(ch->parts,PART_BRAINS))
        {
            msg = "$n's head is shattered, and $s brains splash all over you.";
            vnum = OBJ_VNUM_BRAINS;
        }
    }

    if (IS_SET(ch->parts,PART_HEAD) && is_affected(ch,gsn_decapitate))
    {
       msg  = "$n's severed head plops on the ground.";
       vnum = OBJ_VNUM_SEVERED_HEAD;
    }

    act( msg, ch, NULL, NULL, TO_ROOM );

    if ( vnum != 0 )
    {
        char buf[MAX_STRING_LENGTH];
        OBJ_DATA *obj;
        char *name;

        name            = IS_NPC(ch) ? ch->short_descr : ch->name;
        obj             = create_object( get_obj_index( vnum ), 0 );
        obj->timer      = number_range( 4, 7 );

        sprintf( buf, obj->short_descr, name );
        free_string( obj->short_descr );
        obj->short_descr = str_dup( buf );

        sprintf( buf, obj->description, name );
        free_string( obj->description );
        obj->description = str_dup( buf );

        if (obj->item_type == ITEM_FOOD)
        {
            if (IS_SET(ch->form,FORM_POISON))
                obj->value[3] = 1;
            else if (!IS_SET(ch->form,FORM_EDIBLE))
                obj->item_type = ITEM_TRASH;
        }

        obj_to_room( obj, ch->in_room );
    }

    if ( IS_NPC(ch) )
        msg = "You hear something's death cry.";
    else
        msg = "You hear someone's death cry.";

    was_in_room = ch->in_room;
    for ( door = 0; door <= 5; door++ )
    {
        EXIT_DATA *pexit;

        if ( ( pexit = was_in_room->exit[door] ) != NULL
        &&   pexit->u1.to_room != NULL
        &&   pexit->u1.to_room != was_in_room )
        {
            ch->in_room = pexit->u1.to_room;
            act( msg, ch, NULL, NULL, TO_ROOM );
        }
    }
    ch->in_room = was_in_room;

    return;
}

void raw_kill_new( CHAR_DATA *ch,CHAR_DATA *victim, bool deathCry, bool fLook)
{
int i, cloak=0;
    ROOM_INDEX_DATA *location;
    AFFECT_DATA af;
    AFFECT_DATA *paf, *paf_next;
    CHAR_DATA *gch, *gch_next, *zombie;
    OBJ_DATA *trophy, *obj, *strap, *duelcorpse;
    ROOM_INDEX_DATA *arena_room, *old_room;
    char buf[MAX_STRING_LENGTH], *cname;
    char wizbuf[MAX_STRING_LENGTH];
    bool unholy=0;
    bool credit_timer=0;

    stop_fighting( victim, TRUE );

    if (check_darkshout(ch) == DARKSHOUT_NECRUVIAN)
    {
		if (IS_GOOD(victim))
		{
			printf_to_char(ch, "You invoke your bond with The Lich Necruvian and siphon life from the lifeless body of %s.\n\r", 
				victim->name);
			ch->hit = UMIN( ch->hit + number_fuzzy(victim->level), ch->max_hit );
		}
    }

    if (is_affected(victim,gsn_cloak_form))
        do_cloak(victim,"auto");

    if (IS_NPC(victim) && victim->pIndexData->vnum == 3059) // PRACTICE DUMMY
    {
	act("$n's skull glows brightly and $n rises from $s ashes!",victim,0,0,TO_ROOM);
	return;
    }

    if (IS_NPC(victim) && victim->pIndexData->vnum == MOB_VNUM_DECOY)
    {
	death_cry(victim);
	act("$n vanishes in a poof of smoke!",victim,0,0,TO_ROOM);
    }
    else
    {
	if (deathCry)
    		death_cry( victim );
    

    	if (!IS_NPC(victim) && IS_IMMORTAL(victim))
    	{
            if (victim->pcdata->imm_death)
            {
            	act("$t.",victim,NULL,NULL,TO_ROOM);
            	act("$t.",victim,NULL,NULL,TO_CHAR);
            }
            else
            {
            	act("$n's skull glows brightly and $n rises from $s ashes!",victim,0,0,TO_ROOM);
            	send_to_char("Your skull glows brightly and you rise from your ashes!\n\r",victim);
            }
            return;
    	}

    	for (obj = victim->carrying;obj != NULL;obj = obj->next_content)
    	{
    	    if (IS_SET(obj->progtypes,IPROG_DEATH))
      	    	if ((obj->pIndexData->iprogs->death_prog) (obj,victim))
            	{
          	    victim->position = POS_STANDING;
          	    return;
            	}
    	}

    	if ( IS_SET(victim->progtypes,MPROG_DEATH))
	{
    	    if ((victim->pIndexData->mprogs->death_prog) (victim,ch))
      	    {
            	victim->position = POS_STANDING;
            	return;
      	    }
	}

    	if(IS_NPC(ch) && IS_AFFECTED(ch,AFF_CHARM) && ch->master && ch->master->in_room && ch->master->in_room == ch->in_room)
	    ch = ch->master;

    	// If bard phantom kills, killer becomes phantom->owner 
    	if(IS_NPC(ch) && ch->pIndexData->vnum == MOB_VNUM_PHANTOM && ch->owner)
	    ch = ch->owner;

    	if (!IS_NPC(victim)) 
    	{
	    if (arena) 
	    {

	    	/* MAIN STUFF */
	    	sprintf(buf,"%s is killed by %s!",victim->name,ch->original_name);
	    	do_arena_echo(buf);
	    	char_from_room(victim);
	    	location = get_room_index(26000);
	    	char_to_room(victim,location);
	    	victim->ghost = 1;

	    	affect_strip(victim,gsn_plague);
            	affect_strip(victim,gsn_poison);
            	affect_strip(victim,gsn_blindness);
            	affect_strip(victim,gsn_sleep);
            	affect_strip(victim,gsn_curse);
            	affect_strip(victim,gsn_decapitate);
            	affect_strip(victim,gsn_shackles);
            	affect_strip(victim,gsn_boneshatter);
            	affect_strip(victim,gsn_gouge);
		affect_strip(victim,skill_lookup("hex"));
		affect_strip(victim,skill_lookup("weaken"));
		affect_strip(victim,skill_lookup("impale"));
		affect_strip(victim,skill_lookup("crossslice"));
		affect_strip(victim,skill_lookup("insectswarm"));

            	victim->hit		= victim->max_hit;
            	victim->mana   	= victim->max_mana;
            	victim->move	= victim->max_move;
            	update_pos( victim);

	    	send_to_char("You have died and turned into an invincible ghost a few seconds.\n\r",victim);
	    	act("$n suddenly appears as a ghost.",victim,0,0,TO_ROOM);

		/* TROPHY CODE */
	    	trophy = create_object(get_obj_index(OBJ_VNUM_ARENA_TROPHY),0);
	    	sprintf( buf, trophy->short_descr, victim->original_name);
	    	free_string( trophy->short_descr );
	    	trophy->short_descr = str_dup( buf );
	    	sprintf( buf, trophy->description, victim->original_name);
	    	free_string( trophy->description );
	    	trophy->description = str_dup( buf );
	    	obj_to_char(trophy,ch);
	    	act("You receive $p.",ch,trophy,0,TO_CHAR);
	    	act("$n receives $p.",ch,trophy,0,TO_ROOM);
  	    	return;
	    } // End arena
    	} // End npc check for arena

    	temp_death_log(ch,victim);

    	/* AP unholy bless check --Dioxide */
    	if (check_unholy_new(ch,victim,get_eq_char(ch,WEAR_WIELD)) || check_unholy_new(ch,victim,get_eq_char(ch,WEAR_DUAL_WIELD))) 
 	{
	    unholy = TRUE;
    	} else {
	    unholy = FALSE;
    	}

    	if (is_affected(victim,gsn_unholy_timer)) 
    	{
	    unholy = TRUE;
    	}
	
	if (is_affected(victim,gsn_credit_timer))
	{
	    credit_timer = TRUE;
	}

    	strap = get_eq_char(victim, WEAR_STRAPPED);

    	if (strap != NULL) 
	{
	    do_strap(victim,strap);
    	}

    	make_corpse( ch,victim );

    } // End npc/pc decoy check.

    if(!IS_NPC(ch) && !IS_NPC(victim) && victim!=ch)
    	pay_bounty(ch, victim);

    if (ch && victim) 
    {
        sprintf( wizbuf, "%s died to %s at %s [room %ld]",
            (IS_NPC(victim) ? victim->short_descr : victim->original_name),
            (IS_NPC(ch) ? ch->short_descr : ch->original_name),
            victim->in_room->name, victim->in_room->vnum);

        if (IS_NPC(victim))
            wiznet(wizbuf,NULL,NULL,WIZ_MOBDEATHS,0,0);
        else
            wiznet(wizbuf,NULL,NULL,WIZ_DEATHS,0,0);

	free_string(wizbuf);
    }

    for (gch = char_list; gch != NULL; gch = gch_next)
    {
	gch_next = gch->next;
  	if (IS_NPC(gch) && gch->master == victim)
		extract_char(gch,TRUE);
    }

    if ( IS_NPC(victim) )
    {
        victim->pIndexData->killed++;
        kill_table[URANGE(0, victim->level, MAX_LEVEL-1)].killed++;
        extract_char( victim, TRUE );
        return;
    }

    for (paf=victim->affected; paf!=NULL; paf=paf_next)
    {
	paf_next=paf->next;

        if (paf->type == gsn_unholy_bless && (IS_NPC(ch) || arena))
            continue;

	if (!paf->dispel_at_death)
	    continue;

       	affect_remove( victim, paf );
    }

    extract_char( victim, FALSE );
    if(is_affected(victim,gsn_cloak_form)) cloak=1;

    for (paf=victim->affected; paf!=NULL; paf=paf_next)  
    {
        paf_next=paf->next;

	if (paf->type == gsn_unholy_bless && (IS_NPC(ch) || arena))
	    continue;

	if (paf->dispel_at_death == FALSE)
	    continue;

        affect_remove( victim, paf );             
    }

    victim->affected_by = race_table[victim->race].aff;

    for (i = 0; i < 4; i++)
        victim->armor[i]= 100;
    victim->position    = POS_RESTING;
    victim->hit         = victim->pcdata->perm_hit/2;
    victim->mana        = victim->pcdata->perm_mana/2;
    victim->move        = victim->pcdata->perm_move/2;
    victim->pcdata->condition[COND_HUNGER] 	= 48;
    victim->pcdata->condition[COND_THIRST] 	= 48;
    victim->pcdata->condition[COND_DRUNK] 	= 0;
    victim->pcdata->condition[COND_FULL] 	= 0;
    victim->pcdata->condition[COND_DEHYDRATED]	= 0;
    victim->pcdata->condition[COND_STARVING] 	= 0;
    REMOVE_BIT(victim->act,PLR_CRIMINAL);
    victim->ghost = 8;
    send_to_char("You have died and turned into an invincible ghost for a few minutes.\n\r",victim);

    if (victim->hometown)
		location = get_room_index(hometown_table[victim->hometown].recall);
    else
		location = get_room_index(ROOM_VNUM_TEMPLE);

    char_from_room(victim);
    char_to_room(victim,location);

    init_affect(&af);
    af.where 		= TO_AFFECTS;
    af.aftype 		= AFT_SKILL;
    af.type 		= skill_lookup("infravision");
	af.name			= str_dup("ghost");
    af.duration 	= 20;
    af.location 	= 0;
    af.bitvector 	= AFF_INFRARED|AFF_PASS_DOOR|AFF_FLYING;
    af.modifier 	= 0;
	char msg_buf[MSL];
	sprintf(msg_buf,"turned into a ghostly spirit");
	af.affect_list_msg = str_dup(msg_buf);
    af.level		= victim->level;
    affect_to_char(victim,&af);

    if (unholy) 
    {
	if (!is_affected(victim, skill_lookup("unholy guard")))
	{
		init_affect(&af);
    		af.where 	= TO_AFFECTS;
		af.aftype 	= AFT_SPELL;
    		af.type 	= skill_lookup("unholy guard");
    		af.duration 	= 18;
    		af.location 	= 0;
    		af.modifier 	= 0;
			char msg_buf[MSL];
			sprintf(msg_buf,"protects your soul from unholy bless");
			af.affect_list_msg = str_dup(msg_buf);
    		af.level	= victim->level;
			af.dispel_at_death = FALSE;
    		affect_to_char(victim,&af);
	}
    }

    if (!credit_timer)
    {
		if (!IS_NPC(victim) && !IS_NPC(ch)) {
			char quest_buf[MSL];
			sprintf(quest_buf,"As $N collapses, you find yourself holding %d quest credits!",QUEST_CREDITS_PER_PK);
			act(quest_buf,ch,0,victim,TO_CHAR);
			gain_quest_credits(ch,QUEST_CREDITS_PER_PK);

			init_affect(&af);
			af.where 	= TO_AFFECTS;
			af.aftype 	= AFT_SKILL;
			af.type 	= gsn_credit_timer;
			af.duration 	= 30;
			af.location 	= 0;
			af.modifier 	= 0;
			af.name			= str_dup("quest guard");
			af.affect_list_msg	= str_dup("cannot be harvested for quest credits");
			af.level	= victim->level;
			af.dispel_at_death = FALSE;
			affect_to_char(victim,&af);
		}
    }
		
    victim->last_fight_name = NULL;
    victim->last_fight_time = 0;
	
    // con death
    if (!IS_NPC(victim))
    {
	if (ch->race == race_lookup("lich"))
		victim->pcdata->death_count=4;
	else
		victim->pcdata->death_count++;

	if ( (victim->pcdata->death_count >= 4) && !IS_IMMORTAL(victim) && !(victim->perm_stat[STAT_CON] < 3))
	{
		victim->perm_stat[STAT_CON] -= 1;
		victim->pcdata->death_count = 0;
		send_to_char("With this death you feel your vitality draining away from you.\n\r",victim);
	}
	if (victim->perm_stat[STAT_CON] < CON_DIE_BOUND) {
		SET_BIT(victim->act, PLR_DENY);
		perm_death_log(victim,2);
		send_to_char("You gasp your final breaths. Your vitality has faded.\n\r",victim);
		wiznet("$N dies of constitution loss.",victim,NULL,0,0,0);
		stop_fighting(victim,TRUE);
		victim->pause = 0;
		save_char_obj(victim);
		cname	= str_dup(victim->original_name);
		do_quit_new(victim,"",TRUE);
		delete_char(cname, TRUE);
		return;
	}
	if (victim->perm_stat[STAT_CON] < 5)
		send_to_char("The banshee wails as you feel closer to a permanent death.\n\r",victim);
    } // End con death

    if(!IS_NPC(ch) && ch->in_room->vnum==ROOM_FORCE_DUEL)
    {
	init_affect(&af);
	af.where 	= TO_AFFECTS;
	af.aftype 	= AFT_POWER;
        af.type 	= gsn_honorable_combat;
	af.duration 	= 50;
	af.location 	= 0;
	af.modifier 	= 0;
	af.bitvector	= 0;
	char msg_buf[MSL];
	sprintf(msg_buf,"safe from honorable combat");
	af.affect_list_msg = str_dup(msg_buf);
	af.level	= ch->level;
	affect_to_char(victim,&af);

	char_from_room(ch);
	old_room=get_room_index(ch->pcdata->old_room);
	arena_room=get_room_index(ROOM_FORCE_DUEL);
	char_to_room(ch,old_room);
	act("$n arrives in a blaze of white light, having triumphed honorably over $N!\n\r",ch,0,victim,TO_ROOM);
	for (duelcorpse = arena_room->contents; duelcorpse != NULL; duelcorpse = duelcorpse->next)
    	{
	    if(duelcorpse->in_room==arena_room)
	    {
	    	obj_from_room(duelcorpse);
		obj_to_room(duelcorpse,old_room);
	    }
    	}

        for (zombie = arena_room->people; zombie != NULL; zombie = zombie->next_in_room)
	{
	    if (zombie!=NULL && IS_NPC(zombie) && IS_AFFECTED(zombie,AFF_CHARM) && zombie->pIndexData->vnum==MOB_VNUM_ZOMBIE)
	    {
		char_from_room(zombie);
		char_to_room(zombie,old_room);
	    }
    	}
    } // End force duel

    if (fLook)
    	do_look(victim,"auto");
    return;
}

void raw_kill( CHAR_DATA *ch,CHAR_DATA *victim )
{
    raw_kill_new( ch, victim, TRUE, TRUE );
}

void group_gain( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *gch;
    CHAR_DATA *lch;
    int xp;
    int members;
    int group_levels;
    long quest_credit_gain;
	
    /*
     * Monsters don't get kill xp's or alignment changes.
     * P-killing doesn't help either.
     * Dying of mortal wounds or poison doesn't give xp to anyone!
     */
    if ( victim == ch )
        return;
    

    members = 0;
    group_levels = 0;
    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
        if ( is_same_group( gch, ch ) )
        {
            if(!IS_NPC(gch)) members++;
            group_levels+=gch->level;
            if(IS_NPC(gch)) group_levels+=gch->level;
        }
    }

    if ( members == 0 )
    {
        members = 1;
    }

    lch = (ch->leader != NULL) ? ch->leader : ch;

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
        OBJ_DATA *obj;
        OBJ_DATA *obj_next;
	bool owner = FALSE;

        if ( !is_same_group( gch, ch ) || IS_NPC(gch))
            continue;

        if ( gch->level - lch->level > 8 )
        {
            send_to_char( "You are too high for this group.\n\r", gch );
            continue;
        }

        if ( gch->level - lch->level < -8 )
        {
            send_to_char( "You are too low for this group.\n\r", gch );
            continue;
        }


        xp = xp_compute( gch, victim, members, group_levels );
	quest_credit_gain = quest_credit_compute( gch, victim, members );

        if (IS_NPC(victim))
	{
		if(victim->quest_credits <= 0)
		{
			sprintf( buf, "You receive %d experience points.\n\r", xp );
			send_to_char( buf, gch );
		}
		else
		{
			sprintf( buf, "You receive %d experience points.\n\r", xp);
			send_to_char( buf, gch );
			sprintf( buf, "{RAs %s collapses, you suddenly feel an unusually large weight of precious quest credits burden you.{x\n\r", victim->short_descr);
			send_to_char( buf, gch );
			sprintf( buf, "You find an extra %ld quest credits in your possession!\n\r", quest_credit_gain );
			send_to_char( buf, gch );
			gain_quest_credits( gch, quest_credit_gain );
		}
	}
        gain_exp( gch, xp );
	

        for ( obj = ch->carrying; obj != NULL; obj = obj_next )
        {
            obj_next = obj->next_content;
            if ( obj->wear_loc == WEAR_NONE )
                continue;

	ch->unholy_name = str_dup(ch->name);

	if (str_cmp(obj->owner,ch->unholy_name) )
	{
	owner = TRUE;
	}

            if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)    && IS_EVIL(ch)    )
            ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)    && IS_GOOD(ch)    )
            ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL) && IS_NEUTRAL(ch) )
	    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NEUT)    &&
	         (ch->pcdata->ethos == 0))
	    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_CHAOTIC) &&
		 (ch->pcdata->ethos < 0))
	    ||   ( IS_WEAPON_STAT(obj,WEAPON_UNHOLY)  && (owner)))
            {
                act( "You are zapped by $p.", ch, obj, NULL, TO_CHAR );
                act( "$n is zapped by $p.",   ch, obj, NULL, TO_ROOM );
                obj_from_char( obj );
                obj_to_room( obj, ch->in_room );
            }
        }
    }

    return;
}

long quest_credit_compute( CHAR_DATA *ch, CHAR_DATA *victim, int group_amount)
{
    int quest_credit_gain = 0;

    if (IS_NPC(victim) && victim->quest_credits > 0)
	quest_credit_gain = victim->quest_credits;
    
    else
    {
   	  if (group_amount == 1)
		{
		if (victim->level < 45)
			quest_credit_gain = 0;
		else 
			quest_credit_gain = (victim->level / 18);
		}

  	  else if (group_amount == 2)
		{
		if (victim->level < 45)
			quest_credit_gain = 0;
		else 
			quest_credit_gain = (victim->level / 18) / 3;
		}
    	  else if (group_amount == 3)
		{
		if (victim->level < 45)
			quest_credit_gain = 0;
		else 
			quest_credit_gain = (victim->level / 18) / 3;
		}
   	  else if (group_amount > 3)
		{
		if (victim->level < 45)
			quest_credit_gain = 0;
		else 
			quest_credit_gain = (victim->level / 18) / (group_amount -2);
		}
     }

	return quest_credit_gain;
}

/*
 * Compute xp for a kill.
 * Also adjust alignment of killer.
 * Edit this function to change xp computations.
 */
int xp_compute( CHAR_DATA *gch, CHAR_DATA *victim, int group_amount, int glevel )
{
    int xp,base_exp;
    int level_range;
    double mult;

    mult=((double)(gch->level)/glevel)*group_amount;
    if(mult>=1) { mult=(1+mult)/2;
    } else {
       mult=mult*mult;
    }
    mult = URANGE(.25,mult,1.1);

    level_range = victim->level - gch->level;

    /* compute the base exp */
    switch (level_range)
    {
        default :       base_exp =   0;         break;
        case -9 :       base_exp =   2;         break;
        case -8 :       base_exp =   4;         break;
        case -7 :       base_exp =   7;         break;
        case -6 :       base_exp =   12;         break;
        case -5 :       base_exp =  14;         break;
        case -4 :       base_exp =  25;         break;
        case -3 :       base_exp =  36;         break;
        case -2 :       base_exp =  55;         break;
        case -1 :       base_exp =  70;         break;
        case  0 :       base_exp =  88;         break;
        case  1 :       base_exp =  110;         break;
        case  2 :       base_exp = 131;         break;
        case  3 :       base_exp = 153;         break;
        case  4 :       base_exp = 165;         break;
    }

    if (level_range > 4)
        base_exp = 165 + 15 * (level_range - 4);
    if(mult<1&&level_range>4)
        base_exp = (2*base_exp+165)/3;

    if (IS_SET(victim->act,ACT_NOALIGN))
        xp = base_exp;

    else if (gch->alignment > 0)
    {
        if (victim->alignment < 0)
            xp = (base_exp *4)/3;

        else if (victim->alignment > 0)
            xp = -30;

        else
            xp = base_exp;
    }

    else if (gch->alignment < 0) /* for baddies */
    {
        if (victim->alignment > 0)
            xp = (base_exp * 4)/3;

        else if (victim->alignment < 0)
            xp = base_exp/2;

        else
            xp = base_exp;
    }


    else /* neutral */
    {
            xp = base_exp;
    }

    xp = (xp*2)/3;

    xp=xp*mult;
    xp = number_range (xp, xp * 5/4);



    /* adjust for grouping */
    if (group_amount == 2)
        xp = (xp * 5)/3;
    if (group_amount == 3)
        xp = (xp * 7)/3;
    if (group_amount > 3)
	xp /= (group_amount - 2);

    return xp;
}


void dam_message( CHAR_DATA *ch, CHAR_DATA *victim,int dam,int dt,bool immune, char *dnoun, bool isPlural, bool showCharName )
{
    char buf1[256], buf2[256], buf3[256];
    OBJ_DATA *wield;
    CHAR_DATA *gch;
    const char *vs;
    const char *vp;
    const char *attack;
    char punct;

    if (ch == NULL || victim == NULL)
       	return;

    if (is_affected(victim,skill_lookup("mana shield")) && victim->mana > dam)
    {
		vs = "{xis absorbed by the mana shield around"; vp = "{xis absorbed by the mana shield around";
    }
    else {
		if (is_affected(victim,skill_lookup("mana shield")) && victim->mana <= dam)
			dam = dam - victim->mana;
			
		if (dam < 0) { vs = "fail to harm";	vp = "fails to harm"; dam = 0;	}
		else if ( dam ==   0 ) { vs = "miss";       vp = "misses";          }
	    else if ( dam <=   2 ) { vs = "scratch";    vp = "scratches";       }
	    else if ( dam <=   4 ) { vs = "graze";      vp = "grazes";          }
	    else if ( dam <=   7 ) { vs = "hit";        vp = "hits";            }
	    else if ( dam <=  11 ) { vs = "injure";     vp = "injures";         }
	    else if ( dam <=  15 ) { vs = "wound";      vp = "wounds";          }
	    else if ( dam <=  20 ) { vs = "maul";       vp = "mauls";           }
	    else if ( dam <=  25 ) { vs = "decimate";   vp = "decimates";       }
	    else if ( dam <=  30 ) { vs = "devastate";  vp = "devastates";      }
	    else if ( dam <=  37 ) { vs = "maim";       vp = "maims";           }
	    else if ( dam <=  45 ) { vs = "MUTILATE";   vp = "MUTILATES";       }
	    else if ( dam <=  55 ) { vs = "EVISCERATE"; vp = "EVISCERATES";     }
	    else if ( dam <=  65 ) { vs = "DISMEMBER";  vp = "DISMEMBERS";   }
	    else if ( dam <=  85 ) { vs = "MASSACRE";   vp = "MASSACRES";       }
	    else if ( dam <=  100 ) { vs = "MANGLE";     vp = "MANGLES";         }
	    else if ( dam <= 135 ) { vs = "*** DEMOLISH ***";
	                             vp = "*** DEMOLISHES ***";                 }
	    else if ( dam <= 160 ) { vs = "*** DEVASTATE ***";
	                             vp = "*** DEVASTATES ***";                 }
	    else if ( dam <= 250)  { vs = "=== OBLITERATE ===";
	                             vp = "=== OBLITERATES ===";                }
	    else if ( dam <= 330)  { vs = ">>> ANNIHILATE <<<";
	                             vp = ">>> ANNIHILATES <<<";                }
	    else if ( dam <= 380)  { vs = "<<< ERADICATE >>>";
	                             vp = "<<< ERADICATES >>>";                 }
	    else                   { vs = "do UNSPEAKABLE things to";
	                             vp = "does UNSPEAKABLE things to";         }
	}
	
    punct   = (dam <= 25) ? '.' : '!';

    if (dt  == gsn_dual_wield) 
    {
    	wield = get_eq_char(ch,WEAR_DUAL_WIELD);
		dt = TYPE_HIT;
		if (wield != NULL)
			dt += wield->value[3];
		else
			dt += ch->dam_type;
    }

    if ( dt == TYPE_HIT )
    {
        if (ch == victim)
        {
            sprintf( buf1, "$n %s $melf%c [%d]",vp,punct,dam);
            sprintf( buf2, "You %s yourself%c [%d]",vs,punct,dam);
        }
        else
        {
			for (gch = victim->in_room->people; gch != NULL; gch = gch->next_in_room)
			{
			    if (gch != ch && gch != victim)
			    {
		            sprintf( buf1, "$n's {r%s{x %s%c [%d]",  vp, get_descr_form(victim,gch,FALSE), punct, dam);
					act(buf1,ch,NULL,gch,TO_VICT);
			    }
			}

            sprintf( buf2, "You {r%s{x $N%c [%d]", vs, punct, dam );
            sprintf( buf3, "$n {r%s{x you%c [%d]", vp, punct, dam );
        }
    }
    else
    {
        if ( dt >= 0 && dt < MAX_SKILL )
            attack      = skill_table[dt].noun_damage;
        else if ( dt >= TYPE_HIT && dt < TYPE_HIT + MAX_DAMAGE_MESSAGE)
            attack      = attack_table[dt - TYPE_HIT].noun;
        else
        {
            dt  = TYPE_HIT+17;
            attack  = attack_table[0].noun;
        }

		if (dnoun)
			attack = str_dup(dnoun);

        if (immune)
        {
            if (ch == victim)
            {
				if (!showCharName)
					sprintf(buf1,"$n is unaffected by $s own %s.",attack);
				else
					sprintf(buf1,"$n is unaffected by %s.",attack);
                sprintf(buf2,"Luckily, you are immune to that.");
            }
            else
            {
				if (showCharName) {
					sprintf(buf1,"$N is unaffected by $n's %s!",attack);
					sprintf(buf2,"$N is unaffected by your %s!",attack);
					sprintf(buf3,"$n's %s is powerless against you.",attack);
				}
				else {
					sprintf(buf1,"$N is unaffected by %s!",attack);
					sprintf(buf2,"$N is unaffected by %s!",attack);
					sprintf(buf3,"%s is powerless against you.",attack);
				}
                
            }
        }
        else
        {
            if (ch == victim)
            {
				for (gch = victim->in_room->people; gch != NULL; gch = gch->next_in_room)
				{
				    if (gch != ch && gch != victim)
				    {
						if (showCharName)
						{
							if (isPlural)
				                sprintf( buf1, "$n's %s {r%s{x $m%c [%d]",  attack, vp, punct, dam );
							else
								sprintf( buf1, "$n's %s {r%s{x $m%c [%d]",  attack, vs, punct, dam );
						}
						else
						{
							if (isPlural)
				                sprintf( buf1, "%s {r%s{x $n%c [%d]",  attack, vp, punct, dam );
							else
								sprintf( buf1, "%s {r%s{x $n%c [%d]",  attack, vs, punct, dam );
						}
						act(buf1,ch,NULL,gch,TO_VICT);
				    }
				}
				if (showCharName)
				{
					if (isPlural)
		                sprintf( buf2, "Your %s {r%s{x you%c [%d]",attack,vp,punct, dam);
					else
						sprintf( buf2, "Your %s {r%s{x you%c [%d]",attack,vs,punct, dam);
				}
				else
				{
					if (isPlural)
		                sprintf( buf2, "%s {r%s{x you%c [%d]",attack,vp,punct, dam);
					else
						sprintf( buf2, "%s {r%s{x you%c [%d]",attack,vs,punct, dam);
				}
            }
            else
            {
				for (gch = victim->in_room->people; gch != NULL; gch = gch->next_in_room)
				{
				    if (gch != ch && gch != victim)
				    {
						if (showCharName)
						{
							if (isPlural)
			 					sprintf(buf1, "$n's %s {r%s{x %s%c [%d]", attack, vp, get_descr_form(victim,gch,FALSE), punct, dam);
							else
								sprintf(buf1, "$n's %s {r%s{x %s%c [%d]", attack, vs, get_descr_form(victim,gch,FALSE), punct, dam);
						}
						else
						{
							if (isPlural)
								sprintf(buf1, "%s {r%s{x %s%c [%d]", attack, vp, get_descr_form(victim,gch,FALSE), punct, dam);
							else
								sprintf(buf1, "%s {r%s{x %s%c [%d]", attack, vs, get_descr_form(victim,gch,FALSE), punct, dam);
						}
						act(buf1,ch,NULL,gch,TO_VICT);
				    }
				}
		
				if (showCharName)
				{
					if (isPlural)
		                sprintf( buf2, "Your %s {r%s{x $N%c [%d]",  attack, vp, punct, dam );
					else
						sprintf( buf2, "Your %s {r%s{x $N%c [%d]",  attack, vs, punct, dam );
					if (isPlural)
		                sprintf( buf3, "$n's %s {r%s{x you%c [%d]", attack, vp, punct, dam );
					else
						sprintf( buf3, "$n's %s {r%s{x you%c [%d]", attack, vs, punct, dam );
				}
				else
				{
					if (isPlural)
		                sprintf( buf2, "%s {r%s{x $N%c [%d]",  attack, vp, punct, dam );
					else
						sprintf( buf2, "%s {r%s{x $N%c [%d]",  attack, vs, punct, dam );
					if (isPlural)
		                sprintf( buf3, "%s {r%s{x you%c [%d]", attack, vp, punct, dam );
					else
						sprintf( buf3, "%s {r%s{x you%c [%d]", attack, vs, punct, dam );
				}
            }
        }
    }

    if (ch == victim)
    {
	act_new(buf2,ch,NULL,NULL,TO_CHAR,POS_INCAP);
    }
    else
    {
        act( buf2, ch, NULL, victim, TO_CHAR );
        act( buf3, ch, NULL, victim, TO_VICT );
    }

    if (dt != gsn_poison && dt != gsn_plague && dt != gsn_starvation && dt != gsn_dehydrated)
    {
    	if (!IS_IMMORTAL(ch))
		{
		    if (!IS_NPC(victim))
		    {
				ch->pause = 10;
		    }
		    else
		    {
				ch->pause = 1;
		    }
		}

	   	if (!IS_IMMORTAL(victim))
		{
		    if (!IS_NPC(ch))
		    {
			victim->pause = 10;
		    }
		    else
		    {
			ch->pause = 1;
		    }
		}
    }
    return;
}



/*
 * Disarm a creature.
 * Caller must check for successful attack.
 */
void disarm( CHAR_DATA *ch, CHAR_DATA *victim )
{
    OBJ_DATA *obj,*obj2;
    OBJ_DATA *secondary;


    if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )
    {
        if ( ( obj = get_eq_char( victim, WEAR_DUAL_WIELD ) ) == NULL )
            return;
    }

    if (IS_AFFECTED(ch,AFF_BLIND))

    {
        act("You can't see the person to disarm them!",ch,NULL,victim,TO_CHAR);
        return;
    }

    if (IS_NPC(ch) && (IS_AFFECTED(ch,AFF_BLIND)))
       return;

    if ( IS_OBJ_STAT(obj,ITEM_NOREMOVE) || IS_OBJ_STAT(obj,ITEM_NODISARM))
    {
        act("$S weapon won't budge!",ch,NULL,victim,TO_CHAR);
        act("$n tries to disarm you, but your weapon won't budge!",
            ch,NULL,victim,TO_VICT);
        act("$n tries to disarm $N, but fails.",ch,NULL,victim,TO_NOTVICT);
        return;
    }

	if (is_affected(ch,gsn_unholy_affinity) && ((obj2 = get_eq_char(ch,WEAR_WIELD)) != NULL) && is_affected_obj(obj2,gsn_unholy_bless))
	{
		act("$n tries to disarm you, but you refuse to let go!",victim,0,ch,TO_CHAR);
		act("You try to disarm $n, but he won't let go of $s weapon!",victim,0,ch,TO_VICT);
		act("$N tries to disarm $n, but $n holds onto $s weapon!",victim,0,ch,TO_NOTVICT);
		return;
	}

    act( "$n DISARMS you and sends your weapon flying!",
         ch, NULL, victim, TO_VICT    );
    act( "You disarm $N!",  ch, NULL, victim, TO_CHAR    );
    act( "$n disarms $N!",  ch, NULL, victim, TO_NOTVICT );

    obj_from_char( obj );
    if ( IS_OBJ_STAT(obj,ITEM_NODROP) || IS_OBJ_STAT(obj,ITEM_INVENTORY) )
        obj_to_char( obj, victim );
    else
    {
        obj_to_room( obj, victim->in_room );
        if (IS_NPC(victim) && victim->wait == 0 && can_see_obj(victim,obj))
            get_obj(victim,obj,NULL);
		
		//Return to owner if owned item
		if (strcmp(obj->owner,victim->name) == 0) {
			act( "$N's $p twitches on the ground, then returns to $M!", ch, obj, victim, TO_NOTVICT);
			act( "$p twitches on the ground, then returns to yout!", ch, obj, victim, TO_VICT);
			obj_to_char( obj, victim );
		}
    }

    return;
}

void do_berserk( CHAR_DATA *ch, char *argument)
{
    	int chance, hp_percent;

    	if ((chance = get_skill(ch,gsn_berserk)) == 0
    	||  (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_BERSERK))
    	||  (!IS_NPC(ch)
    	&&   ch->level < skill_table[gsn_berserk].skill_level[ch->class]))
    	{
        	send_to_char("You turn red in the face, but nothing happens.\n\r",ch);
        	return;
    	}

    	if (IS_AFFECTED(ch,AFF_BERSERK) || is_affected(ch,gsn_berserk)
    	||  is_affected(ch,skill_lookup("frenzy")))
    	{
        	send_to_char("You get a little madder.\n\r",ch);
        	return;
    	}

    	if (IS_AFFECTED(ch,AFF_CALM))
    	{
        	send_to_char("You're feeling to mellow to berserk.\n\r",ch);
        	return;
    	}

	if (IS_AFFECTED(ch,AFF_SLOW))
	{
		send_to_char("You're feeling way too lethargic to berserk.\n\r", ch);
		return;
	}

    	if (ch->mana < 100)
    	{
        	send_to_char("You can't get up enough energy.\n\r",ch);
        	return;
    	}

    	if (ch->position == POS_FIGHTING)
        	chance += 10;

    	if (ch->max_hit == 0)
		hp_percent = 0;
     	else
    		hp_percent = 100 * ch->hit/ch->max_hit;

    	chance += 25 - hp_percent/2;

    	if (number_percent() < chance)
    	{
        	AFFECT_DATA af;

        	WAIT_STATE(ch,PULSE_VIOLENCE);
        	ch->mana -= 100;
        	ch->move /= 1.2;

        	ch->hit += (ch->drain_level + ch->level) * 4;
        	ch->hit = UMIN(ch->hit,ch->max_hit);

        	send_to_char("Your pulse races as you are consumed by rage!\n\r",ch);
        	act("$n gets a wild look in $s eyes.",ch,NULL,NULL,TO_ROOM);
        	check_improve(ch,gsn_berserk,TRUE,2);

		init_affect(&af);
        	af.where        = TO_AFFECTS;
        	af.type         = gsn_berserk;
        	af.level        = ch->level;
		af.duration 	= number_range(4,9);
        	af.modifier     = 10;
        	af.bitvector    = AFF_BERSERK;
		af.aftype	= AFT_SKILL;

        	af.location     = APPLY_HITROLL;
        	affect_to_char(ch,&af);

        	af.location     = APPLY_DAMROLL;
        	affect_to_char(ch,&af);

		af.modifier 	= -2 * AC_PER_ONE_PERCENT_DECREASE_DAMAGE;
        	af.location     = APPLY_AC;
        	affect_to_char(ch,&af);
    	}
    	else
    	{
        	WAIT_STATE(ch,PULSE_VIOLENCE);
        	ch->mana -= 25;

        	send_to_char("Your pulse speeds up, but nothing happens.\n\r",ch);
        	check_improve(ch,gsn_berserk,FALSE,2);
    	}
	return;
}

void do_bash( CHAR_DATA *ch, char *argument )
{
    	CHAR_DATA *victim;
    	int dam, chance;
	char buf[MSL], arg[MIL];
    
	one_argument(argument,arg);

    	if ( (chance = get_skill(ch,gsn_bash)) == 0
    	|| (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_BASH))
    	|| (!IS_NPC(ch)
    	&&  ch->level < skill_table[gsn_bash].skill_level[ch->class]))
    	{
        	send_to_char("Bashing? What's that?\n\r",ch);
        	return;
    	}

    	if (IS_NPC(ch) && IS_AFFECTED(ch,AFF_CHARM))
		return;

    	if (arg[0] == '\0')
    	{
        	victim = ch->fighting;
        	if (victim == NULL)
        	{
            		send_to_char("But you aren't fighting anyone!\n\r",ch);
            		return;
        	}
    	}	
    	else if ((victim = get_char_room(ch,arg)) == NULL)
    	{
        	send_to_char("They aren't here.\n\r",ch);
        	return;
    	}

    	if (victim->position < POS_FIGHTING)
    	{
        	act("You'll have to let $M get back up first.",ch,NULL,victim,TO_CHAR);
        	return;
    	}

    	if (victim == ch)
    	{
        	send_to_char("You try to bash your brains out, but fail.\n\r",ch);
        	return;
    	}

    	if (is_safe(ch,victim))
        	return;

    	if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    	{
        	act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
        	return;
    	}

    	if (is_affected(victim,gsn_protective_shield))
    	{
		act("$N's bash seems to slide around you.",victim,0,ch,TO_CHAR);
		act("$N's bash seems to slide around $n.",victim,0,ch,TO_NOTVICT);
		act("Your bash seems to slide around $n.",victim,0,ch,TO_VICT);
		WAIT_STATE(ch,PULSE_VIOLENCE*.5);
		return;
    	}

    	if (is_affected(victim,gsn_asscounter))
    	{
		act("You deftly sidestep $N's bash, throwing them to the ground.",victim,0,ch,TO_CHAR);
        	act("$n quickly dodges $N's bash, throwing $s down in the process.",victim,0,ch,TO_NOTVICT);
        	act("$n deftly dodges your bash, throwing you to the ground in the process.",victim,0,ch,TO_VICT);
        	WAIT_STATE(ch,2 * PULSE_VIOLENCE);
        	dam = victim->level*2 + number_percent()*2 - 50;
        	damage_old(victim,ch,dam,gsn_throw,DAM_BASH,TRUE);
        	affect_strip( victim, gsn_asscounter );
		return;
    	}

    	if (is_affected(victim, skill_lookup("wraithform")))
    	{
		act("$N's tries to bash you but stumbles through thin air.",victim,0,ch,TO_CHAR);
		act("$N's tries to bash $n but stumbles through thin air.",victim,0,ch,TO_NOTVICT);
		act("You stumble as you try to bash $n but stumble through thin air.",victim,0,ch,TO_VICT);
		WAIT_STATE(ch,PULSE_VIOLENCE*1);
		return;
    	}

    	chance += ch->carry_weight / 250;
    	chance -= victim->carry_weight / 200;

    	if (ch->size < victim->size)
        	chance += (ch->size - victim->size) * 20;
    	else
        	chance += (ch->size - victim->size) * 10;


    	/* stats */
    	chance += get_curr_stat(ch,STAT_STR);
    	chance -= (get_curr_stat(victim,STAT_DEX) * 4)/3;
    	chance -= GET_AC(victim,AC_BASH) /25;
    
	/* speed */
    	if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
        	chance += 10;
    	if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
        	chance -= 40;

    	/* level */
    	chance += (ch->drain_level + ch->level - victim->level - victim->drain_level);
	
	if (!IS_NPC(ch) && !IS_NPC(victim) && (victim->fighting == NULL || ch->fighting == NULL))
	{
		sprintf(buf,"Help! %s is bashing me!",PERS(ch,victim));
		do_myell(victim,buf);
	}


    	if (!IS_NPC(victim) && chance < get_skill(victim,gsn_dodge) )
        	chance -= 3 * (get_skill(victim,gsn_dodge) - chance);

    	/* now the attack */
    	if (number_percent() < chance )
    	{
        	act("$n sends you sprawling with a powerful bash!", ch,NULL,victim,TO_VICT);
        	act("You slam into $N, and send $M flying!",ch,NULL,victim,TO_CHAR);
        	act("$n sends $N sprawling with a powerful bash.", ch,NULL,victim,TO_NOTVICT);
        	check_improve(ch,gsn_bash,TRUE,1);
        	WAIT_STATE(victim, 2 * PULSE_VIOLENCE);
        	damage_old(ch,victim,number_range(2,2 + 2 * ch->size + chance/20),gsn_bash, DAM_BASH,TRUE);
        	WAIT_STATE(ch,skill_table[gsn_bash].beats);
        	victim->position = POS_RESTING;
	}
   	else
   	{
        	damage_old(ch,victim,0,gsn_bash,DAM_BASH,FALSE);
        	act("You fall flat on your face!", ch,NULL,victim,TO_CHAR);
        	act("$n falls flat on $s face.", ch,NULL,victim,TO_NOTVICT);
        	act("You evade $n's bash, causing $m to fall flat on $s face.", ch,NULL,victim,TO_VICT);
		check_cheap_shot(victim,ch);
        	check_improve(ch,gsn_bash,FALSE,1);
        	ch->position = POS_RESTING;
        	WAIT_STATE(ch,skill_table[gsn_bash].beats);
    	}
    	check_killer(ch,victim);
}

void do_dirt( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;
    char buf[MAX_STRING_LENGTH];

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_dirt)) == 0
    ||   (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_KICK_DIRT) && ch->pIndexData->vnum!=MOB_VNUM_ENFORCER)
    ||   (!IS_NPC(ch)
    &&    ch->level < skill_table[gsn_dirt].skill_level[ch->class]))
    {
        send_to_char("You get your feet dirty.\n\r",ch);
        return;
    }

    if (arg[0] == '\0')
    {
        victim = ch->fighting;
        if (victim == NULL)
        {
            send_to_char("But you aren't in combat!\n\r",ch);
            return;
        }
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }

    if (IS_AFFECTED(victim,AFF_BLIND))
    {
        act("$E's already been blinded.",ch,NULL,victim,TO_CHAR);
        return;
    }

    if(victim->position==POS_SLEEPING) {
	send_to_char("Their eyes are closed.\n\r",ch);
	return;
    }

    if (victim == ch)
    {
        send_to_char("Very funny.\n\r",ch);
        return;
    }

    if (is_safe(ch,victim))
        return;

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
        act("But $N is such a good friend!",ch,NULL,victim,TO_CHAR);
        return;
    }

    if (is_affected(victim, gsn_shield_of_dust))
    {
	act("Your dirt is blocked by $N's shield of dust!",ch,NULL,victim,TO_CHAR);
	act("$n kicks dirt at you but it is stoped by your shield of dust!", ch,NULL,victim,TO_VICT);
	act("$n's dirt is deflected by $N's shield of dust!",ch,NULL,victim,TO_NOTVICT);
	return;
    }

    /* dexterity */
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= 2 * get_curr_stat(victim,STAT_DEX);

    /* speed  */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
        chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
        chance -= 30;

    /* level */
    chance += (ch->drain_level + ch->level - victim->level - victim->drain_level) * 2;

    /* sloppy hack to prevent false zeroes */
    if (chance % 5 == 0)
        chance += 1;

    /* terrain */

    switch(ch->in_room->sector_type)
    {
        case(SECT_INSIDE):              chance -= 20;   break;
        case(SECT_CITY):                chance -= 10;   break;
        case(SECT_FIELD):               chance +=  5;   break;
        case(SECT_FOREST):                              break;
        case(SECT_HILLS):                               break;
        case(SECT_MOUNTAIN):            chance -= 10;   break;
        case(SECT_WATER_SWIM):          chance  =  0;   break;
        case(SECT_WATER_NOSWIM):        chance  =  0;   break;
        case(SECT_AIR):                 chance  =  0;   break;
        case(SECT_DESERT):              chance += 10;   break;
    }

    if (chance == 0)
    {
        send_to_char("There isn't any dirt to kick.\n\r",ch);
        return;
    }

    /* now the attack */
    if (number_percent() < chance)
    {
        AFFECT_DATA af;
        act("$n is blinded by the dirt in $s eyes!",victim,NULL,NULL,TO_ROOM);
        act("$n kicks dirt in your eyes!",ch,NULL,victim,TO_VICT);
	if (!IS_NPC(ch) && !IS_NPC(victim) && ( (victim->fighting == NULL) || (ch->fighting == NULL)))
		do_myell(victim,"Help! Someone just kicked dirt in my eyes!");

        damage(ch,victim,number_range(2,5),gsn_dirt,DAM_TRUESTRIKE,TRUE);
        send_to_char("You can't see a thing!\n\r",victim);
        check_improve(ch,gsn_dirt,TRUE,2);
        WAIT_STATE(ch,skill_table[gsn_dirt].beats);

	init_affect(&af);
        af.where        = TO_AFFECTS;
        af.type         = gsn_dirt;
	af.aftype 	= AFT_MALADY;
        af.level        = ch->level + ch->drain_level;
        af.duration     = 0;
        af.location     = APPLY_HITROLL;
        af.modifier     = -4;
        af.bitvector    = AFF_BLIND;

        affect_to_char(victim,&af);
    }
    else
    {
	if (!IS_NPC(ch) && !IS_NPC(victim) && (victim->fighting == NULL || ch->fighting == NULL))
	{
		switch(number_range(0,1))
		{
		case (0):
			sprintf(buf,"Help! %s just tried to kick dirt in my eyes!",PERS(ch,victim));
		break;
		case (1):
			sprintf(buf,"Die, %s you dirty fool!",PERS(ch,victim));
		break;
		}
		do_myell(victim,buf);
	}
        damage(ch,victim,0,gsn_dirt,DAM_NONE,TRUE);

        check_improve(ch,gsn_dirt,FALSE,2);
        WAIT_STATE(ch,skill_table[gsn_dirt].beats);
    }
        check_killer(ch,victim);
}

void do_trip( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;
    int dam;
    AFFECT_DATA af;
    char buf[MAX_STRING_LENGTH];

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_trip)) == 0
    ||   (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_TRIP) && ch->pIndexData->vnum!=MOB_VNUM_ENFORCER)
    ||   (!IS_NPC(ch)
          && ch->level < skill_table[gsn_trip].skill_level[ch->class]))
    {
        send_to_char("Tripping?  What's that?\n\r",ch);
        return;
    }


    if (arg[0] == '\0')
    {
        victim = ch->fighting;
        if (victim == NULL)
        {
            send_to_char("But you aren't fighting anyone!\n\r",ch);
            return;
        }
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }

    if (is_safe(ch,victim))
        return;

    if (IS_AFFECTED(victim,AFF_FLYING)
	&& !is_affected(victim,gsn_earthbind))
    {
        act("$S feet aren't on the ground.",ch,NULL,victim,TO_CHAR);
        return;
    }

    if (victim->position < POS_FIGHTING)
    {
        act("$N is already down.",ch,NULL,victim,TO_CHAR);
        return;
    }

    if (victim == ch)
    {
        send_to_char("You fall flat on your face!\n\r",ch);
        WAIT_STATE(ch,2 * skill_table[gsn_trip].beats);
        act("$n trips over $s own feet!",ch,NULL,NULL,TO_ROOM);
        return;
    }

    if (is_affected(victim,gsn_asscounter))
    {
        act("You dart to the side kicking the inside of $N's knee!",victim,0,ch,TO_CHAR);
        act("$n darts to the side kicking the inside of $N's knee!",victim,0,ch,TO_NOTVICT);
        act("$n dodges your trip, then kicks the inside of your knee!",victim,0,ch,TO_VICT);
        WAIT_STATE(victim,1 * PULSE_VIOLENCE);
        dam = victim->level*2 + number_percent()*2 - 50;
        damage_old(victim,ch,dam,gsn_legbreak,DAM_BASH,TRUE);

         if (!is_affected(ch,gsn_legbreak))
           {
		init_affect(&af);
                af.where     = TO_AFFECTS;
		af.aftype = AFT_MALADY;
                af.type      = gsn_legbreak;
                af.level     = victim->level;
                af.duration  = victim->level/5;
                af.bitvector = 0;
                af.location  = APPLY_DEX;
                af.modifier  = 1-(victim->level/5);
                affect_to_char(ch,&af);
            }
        affect_strip( victim, gsn_asscounter );
        return;
 }

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
        act("$N is your beloved master.",ch,NULL,victim,TO_CHAR);
        return;
    }

    /* modifiers */

    /* size */
    if (ch->size < victim->size)
        chance += (ch->size - victim->size) * 10;  /* bigger = harder to trip */

    /* dex */
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= get_curr_stat(victim,STAT_DEX) * 3 / 2;

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
        chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
        chance -= 40;

    /* level */
    chance += (ch->drain_level + ch->level - victim->level - victim->drain_level) * 2;


    /* now the attack */
    if (number_percent() < chance)
    {
        act("$n trips you and you go down!",ch,NULL,victim,TO_VICT);
        act("You trip $N and $N goes down!",ch,NULL,victim,TO_CHAR);
        act("$n trips $N, sending $M to the ground.",ch,NULL,victim,TO_NOTVICT);
        check_improve(ch,gsn_trip,TRUE,1);

	if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (ch->fighting == NULL || victim->fighting == NULL))
	{
	    	sprintf(buf,"Help! %s just tripped me!",PERS(ch,victim));
	    do_myell(victim,buf);
	}
        WAIT_STATE(victim,2 * PULSE_VIOLENCE);
        WAIT_STATE(ch,skill_table[gsn_trip].beats);
        victim->position = POS_RESTING;
        damage(ch,victim,number_range(2, 2 +  2 * victim->size),gsn_trip, DAM_BASH,TRUE);
	check_downstrike(ch,victim);
	check_cheap_shot(ch,victim);
    }
    else
    {
	if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (ch->fighting == NULL || victim->fighting == NULL))
	{
		sprintf(buf,"Help! %s just tried to trip me!",PERS(ch,victim));
	    do_myell(victim,buf);
	}
        damage(ch,victim,0,gsn_trip,DAM_BASH,TRUE);
        WAIT_STATE(ch,skill_table[gsn_trip].beats*2/3);
        check_improve(ch,gsn_trip,FALSE,1);
    }
        check_killer(ch,victim);
}

void do_hit( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( ch->position != POS_FIGHTING )
    {
	send_to_char( "You're not fighting anyone.\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Switch targets to whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( victim == ch )
    {
        send_to_char( "Are you crazy?\n\r", ch );
        return;
    }

    if ( victim->fighting != ch )
    {
	send_to_char( "That person isn't fighting you.\n\r", ch );
	return;
    }

    ch->fighting = victim;
    return;
}

void do_kill( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "Kill whom?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
/*  Allow player killing   */
    if ( !IS_NPC(victim) )
    {
        send_to_char( "You must MURDER a player.\n\r", ch );
        return;
    }
    if ( victim == ch )
    {
        send_to_char( "You hit yourself.  Ouch!\n\r", ch );
        multi_hit( ch, ch, TYPE_UNDEFINED );
        return;
    }

    if ( is_safe( ch, victim ) )
        return;

    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
    {
        act( "$N is your beloved master.", ch, NULL, victim, TO_CHAR );
        return;
    }

    if ( ch->position == POS_FIGHTING )
    {
        send_to_char( "You do the best you can!\n\r", ch );
        return;
    }

    WAIT_STATE( ch, 1 * PULSE_VIOLENCE );
    multi_hit( ch, victim, TYPE_UNDEFINED );
    return;
}



void do_murde( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to MURDER, spell it out.\n\r", ch );
    return;
}



void do_murder( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "Murder whom?\n\r", ch );
        return;
    }

    if ((IS_AFFECTED(ch,AFF_CHARM) || (IS_NPC(ch) && IS_SET(ch->act,ACT_PET)))
    && !is_centurion(ch))
        return;

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( victim == ch )
    {
        send_to_char( "Suicide is a mortal sin.\n\r", ch );
        return;
    }

    if ( is_safe( ch, victim ) )
        return;

	if (check_chargeset(ch,victim)) {return;}

    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
    {
        act( "$N is your beloved master.", ch, NULL, victim, TO_CHAR );
        return;
    }

    if ( ch->position == POS_FIGHTING )
    {
        send_to_char( "You do the best you can!\n\r", ch );
        return;
    }

    WAIT_STATE( ch, 1 * PULSE_VIOLENCE );
    if (IS_NPC(ch))
        sprintf(buf, "Help! I am being attacked by %s!",ch->short_descr);
    else
    {
        switch(number_range(1,6))
        {
	default:
            sprintf(buf, "Help!  I am being attacked by %s!",PERS(ch,victim));
            break;
        case (1):
            sprintf(buf, "Help!  I am being attacked by %s!",PERS(ch,victim));
            break;
        case (2):
            sprintf(buf, "Die %s, you dirty fool!", PERS(ch,victim));
            break;
        case (3): case (4):
            sprintf(buf, "Help!  %s is attacking me!", PERS(ch,victim));
            break;
        case (5):
            sprintf(buf, "How dare you attack me %s!", PERS(ch,victim));
            break;
        case (6):
            sprintf(buf, "Help!  %s is hurting me!", PERS(ch,victim));
            break;
        }
    }

/* Added a myell hack of yell so Enforcer cabal members can see if a PK
yell is faked or not (see myell in act_comm.c for this).
-Ceran
*/
    do_myell( victim, buf );
    multi_hit( ch, victim, TYPE_UNDEFINED );
    return;
}

void do_backstab( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int dam;
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );


    if ( (get_skill(ch,gsn_backstab) == 0 )
	|| ch->level < skill_table[gsn_backstab].skill_level[ch->class])
    {
	if (!IS_NPC(ch))
	{
	send_to_char("You don't know how to backstab.\n\r",ch);
	return;
	}
    }

    if (arg[0] == '\0')
    {
        send_to_char("Backstab whom?\n\r",ch);
        return;
    }

    if (ch->fighting != NULL)
    {
        send_to_char("You're facing the wrong end.\n\r",ch);
        return;
    }
 
    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }

    if ( victim == ch )
    {
        send_to_char( "How can you sneak up on yourself?\n\r", ch );
        return;
    }

        obj= get_eq_char(ch,WEAR_WIELD);

        if (obj == NULL)
        {
        send_to_char("You must be wielding a weapon to backstab.\n\r",ch);
        return;
        }

        if (obj->value[0] != WEAPON_DAGGER)
        {
        obj = get_eq_char(ch,WEAR_DUAL_WIELD);
        if (obj == NULL)
        {
        send_to_char("You must be wielding a weapon to backstab.\n\r",ch);
        return;
        }
        }

        if (obj->value[0] != WEAPON_DAGGER)
        {
        send_to_char("You need to be wielding a dagger to backstab.\n\r",ch);
        return;
        }

    if ( is_safe(ch, victim ) )
      return;

    if (check_chargeset(ch,victim)) {return;}

    if ( victim->fighting != NULL )
    {
	send_to_char( "That person is moving around too much to backstab.\n\r", ch );
	return;
    }

    if ((victim->hit < victim->max_hit*9/10))
    {
        act( "$N is hurt and suspicious ... you can't sneak up.", ch, NULL, victim, TO_CHAR );
        return;
    }

    WAIT_STATE( ch, skill_table[gsn_backstab].beats );
    if ( number_percent( ) < get_skill(ch,gsn_backstab)
    || ( get_skill(ch,gsn_backstab) >= 2 && !IS_AWAKE(victim) ) )
    {
        check_improve(ch,gsn_backstab,TRUE,1);
	if (!IS_NPC(ch) && !IS_NPC(victim) && victim->fighting == NULL)
	{
	switch(number_range(0,1))
	{
	case (0):
	sprintf(buf,"Die, %s you backstabbing fool!",PERS(ch,victim));
	break;
	case (1):
	sprintf(buf,"Help! I'm being backstabbed by %s!",PERS(ch,victim));
	break;
	}
	do_myell(victim,buf);
	}


	one_hit_new(ch,victim,gsn_backstab,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
    }
    else
    {
        check_improve(ch,gsn_backstab,FALSE,1);
	if (!IS_NPC(ch) && !IS_NPC(victim) && victim->fighting == NULL)
	{
	switch(number_range(0,1))
	{
	case (0):
	sprintf(buf,"Die, %s you backstabbing fool!",PERS(ch,victim));
	break;
	case (1):
	sprintf(buf,"Help! I'm being backstabbed by %s!",PERS(ch,victim));
	break;
	}
	do_myell(victim,buf);
	}


        damage( ch, victim, 0, gsn_backstab,DAM_NONE,TRUE);
    }


     if (ch->level < skill_table[gsn_dual_backstab].skill_level[ch->class])
	return;

    if (number_range(0,3) == 0)
    {
	if (get_skill(ch,gsn_dual_backstab) != 100)
		check_improve(ch,gsn_dual_backstab,FALSE,1);
	return;
    }

    if (number_percent() > (get_skill(ch,gsn_dual_backstab) + ch->level - victim->level) )
    {
	check_improve(ch,gsn_dual_backstab,FALSE,1);
	dam  = 0;
    }
    else
    {
	dam = dice(obj->value[1], obj->value[2]) * get_skill(ch,gsn_dual_backstab)/100;
        if ((ch->drain_level + ch->level) < 35)         dam *= number_range(2,4);
        else if ((ch->drain_level + ch->level) < 40)    dam *= number_range(2,5);
        else if ((ch->drain_level + ch->level) < 45)    dam *= number_range(3,5);
        else if ((ch->drain_level + ch->level) < 50)    dam *= number_range(10,25); /* 4,6 */
        else dam *= number_range(15,30); /* 4,7 */

        dam += (ch->damroll * get_skill(ch,gsn_dual_backstab)/100);
    }
    damage_old(ch,victim,dam,gsn_dual_backstab,DAM_NONE,TRUE);

    return;
}

void do_rescue( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *fch;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
        send_to_char( "Rescue whom?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( victim == ch )
    {
        send_to_char( "What about fleeing instead?\n\r", ch );
        return;
    }

    if ( !IS_NPC(ch) && IS_NPC(victim) )
    {
        send_to_char( "Doesn't need your help!\n\r", ch );
        return;
    }

    if ( ( fch = victim->fighting ) == NULL )
    {
        send_to_char( "That person is not fighting right now.\n\r", ch );
        return;
    }

    if (victim->fighting == ch)
	return send_to_char( "You cannot rescue someone who is attacking you!\n\r",ch);

    if (ch->fighting == victim)
	return send_to_char( "Rescue the person you're fighting?\n\r",ch);

    if (is_safe(ch,victim->fighting))
	return;

    WAIT_STATE( ch, skill_table[gsn_rescue].beats );
    if ( number_percent( ) > get_skill(ch,gsn_rescue))
    {
        send_to_char( "You fail the rescue.\n\r", ch );
        check_improve(ch,gsn_rescue,FALSE,1);
        return;
    }

    WAIT_STATE(victim,12);
    act( "You rescue $N!",  ch, NULL, victim, TO_CHAR    );
    act( "$n rescues you!", ch, NULL, victim, TO_VICT    );
    act( "$n rescues $N!",  ch, NULL, victim, TO_NOTVICT );
    check_improve(ch,gsn_rescue,TRUE,1);

    stop_fighting( fch, FALSE );
    stop_fighting( victim, FALSE );

    set_fighting( ch, fch );
    set_fighting( fch, ch );
    return;
}



void do_kick( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    int dam;

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_kick].skill_level[ch->class] )
    {
        send_to_char(
            "You better leave the martial arts to fighters.\n\r", ch );
        return;
    }

    if (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_KICK))
        return;

    if ( ( victim = ch->fighting ) == NULL )
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }

    WAIT_STATE( ch, skill_table[gsn_kick].beats );
    if ( get_skill(ch,gsn_kick) > number_percent())
    {
	dam = (ch->drain_level + ch->level)/2;
	dam += number_range(0,(ch->drain_level + ch->level)/6);
	dam += number_range(0,(ch->drain_level + ch->level)/6);
	dam += number_range(0,(ch->drain_level + ch->level)/6);
	dam += number_range(0,(ch->drain_level + ch->level)/6);
	dam += number_range(0,(ch->drain_level + ch->level)/6);
	dam += number_range(0,(ch->drain_level + ch->level)/6);
	dam += number_range(ch->level/5,ch->level/4);

	if (!IS_NPC(ch))
	{
		if (!str_cmp(class_table[ch->class].name,"channeler"))
		if (!str_cmp(class_table[ch->class].name,"necromancer"))
		if (!str_cmp(class_table[ch->class].name,"elementalist"))
		{
			dam *= 3;
			dam /= 4;
		}
	}

	if (spike_toed_boots(ch))
	    dam *= 3;

        damage(ch,victim,dam, gsn_kick,DAM_BASH,TRUE);
        check_improve(ch,gsn_kick,TRUE,1);
	check_follow_through(ch,victim,dam);
    }
    else
    {
        damage( ch, victim, 0, gsn_kick,DAM_BASH,TRUE);
        check_improve(ch,gsn_kick,FALSE,1);
    }
        check_killer(ch,victim);
    return;
}




void do_disarm( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim=0;
    OBJ_DATA *obj,*obj2;
    int dam;
    AFFECT_DATA af;
    int chance,hth,ch_weapon,vict_weapon,ch_vict_weapon;

    hth = 0;

    if ((chance = get_skill(ch,gsn_disarm)) == 0)
    {
        send_to_char( "You don't know how to disarm opponents.\n\r", ch );
        return;
    }

    if ( get_eq_char( ch, WEAR_WIELD ) == NULL
    &&   ((hth = get_skill(ch,gsn_hand_to_hand)) == 0
    ||    (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_DISARM))))
    {
        send_to_char( "You must wield a weapon to disarm.\n\r", ch );
        return;
    }

    if (IS_AFFECTED(ch,AFF_BLIND))

    {
        act("You can't see the person to disarm them!",ch,NULL,victim,TO_CHAR);
        return;
    }

    if (IS_NPC(ch) && (IS_AFFECTED(ch,AFF_BLIND)))
       return;

    if ( ( victim = ch->fighting ) == NULL )
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }

    if (is_affected(victim,gsn_asscounter))
    {
        act("With blinding speed you hit $N's arm, breaking it!",victim,0,ch,TO_CHAR);
        act("With blinding speed $n hits $N's arm, breaking it!",victim,0,ch,TO_NOTVICT);
        act("With blinding speed $n hits your arm, breaking it instantly!",victim,0,ch,TO_VICT);
        WAIT_STATE(victim,2 * PULSE_VIOLENCE);
        dam = victim->level*2 + number_percent()*2 - 50;
        damage_old(victim,ch,dam,gsn_armbreak,DAM_BASH,TRUE);

        if (!is_affected(ch,gsn_armbreak))
              {
		init_affect(&af);
                af.where     = TO_AFFECTS;
                af.type      = gsn_armbreak;
		af.aftype = AFT_MALADY;
                af.level     = victim->level;
                af.duration  = victim->level/5;
                af.bitvector = 0;
                af.location  = APPLY_DEX;
                af.modifier  = 1-(victim->level/5);
                affect_to_char(ch,&af);
                af.location  = APPLY_STR;
                af.modifier  = 1-(victim->level/5);
                af.bitvector = 0;
                affect_to_char(ch,&af);
          	}
        affect_strip( victim, gsn_asscounter );
        return;
    }

    if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )
    {
        if ( ( obj = get_eq_char( victim, WEAR_DUAL_WIELD ) ) == NULL )
        {
            send_to_char( "Your opponent is not wielding a weapon.\n\r", ch );
            return;
        }
    }

	if (is_affected(victim,gsn_spiderhands) )
	{
		act("You try to disarm $N, but $s hands stick to his weapon.",
			ch,NULL,victim,TO_CHAR);
		act("$n tries to disarm you, but your hands stick to your weapon.",
			ch,NULL,victim,TO_VICT);
		act("$n tries to disarm $N, but their hands stick to their weapon.",
			ch,NULL,victim,TO_NOTVICT);
		return;
	}

        if (is_affected(victim,gsn_unholy_affinity) && ((obj2 = get_eq_char(victim,WEAR_WIELD)) != NULL) && is_affected_obj(obj2,gsn_unholy_bless))
        {
                act("$N tries to disarm you, but you refuse to let go!",victim,0,ch,TO_CHAR);
                act("You try to disarm $n, but he won't let go of $s weapon!",victim,0,ch,TO_VICT);
                act("$N tries to disarm $n, but $n holds onto $s weapon!",victim,0,ch,TO_NOTVICT);
                return;
        }

    /* find weapon skills */
    ch_weapon = get_weapon_skill(ch,get_weapon_sn(ch));
    vict_weapon = get_weapon_skill(victim,get_weapon_sn(victim));
    ch_vict_weapon = get_weapon_skill(ch,get_weapon_sn(victim));

    /* modifiers */
    chance *= 8;
    chance /=10;

    /* skill */
    if ( get_eq_char(ch,WEAR_WIELD) == NULL)
        chance = chance * hth/150;
    else
        chance = chance * ch_weapon/100;

    chance += (ch_vict_weapon/2 - vict_weapon) / 2;

    /* dex vs. strength */
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= 1.25 * get_curr_stat(victim,STAT_STR);

    /* level */
    chance += (ch->drain_level + ch->level - victim->drain_level - victim->level) * 2;

    /* and now the attack */
    if (number_percent() < chance)
    {
        WAIT_STATE( ch, skill_table[gsn_disarm].beats );
        disarm( ch, victim );
        check_improve(ch,gsn_disarm,TRUE,1);
    }
    else
    {
        WAIT_STATE(ch,skill_table[gsn_disarm].beats);
        act("You fail to disarm $N.",ch,NULL,victim,TO_CHAR);
        act("$n tries to disarm you, but fails.",ch,NULL,victim,TO_VICT);
        act("$n tries to disarm $N, but fails.",ch,NULL,victim,TO_NOTVICT);
        check_improve(ch,gsn_disarm,FALSE,1);
    }
    check_killer(ch,victim);
    return;
}


void do_circle_stab( CHAR_DATA *ch, char *argument )
{
    bool attempt_dual;
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *v_check;
    CHAR_DATA *v_next;
    OBJ_DATA *obj;
    int chance;
    int dam;

    attempt_dual = FALSE;
    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_circle)) == 0
    ||  IS_NPC(ch)
    ||  (!IS_NPC(ch) && ch->level < skill_table[gsn_circle].skill_level[ch->class]) )
    {
	send_to_char("Circling? What's that?\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	send_to_char("But you aren't fighting anyone.\n\r",ch);
	return;
	}
    }
    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char("They aren't here.\n\r",ch);
	return;
    }

    if (ch->fighting == NULL)
	{
	send_to_char("You can't circle someone like that.\n\r",ch);
	return;
	}


    for (v_check = ch->in_room->people; v_check != NULL; v_check = v_next)
    {
	v_next = v_check->next_in_room;

	if (v_check->fighting == ch)
	{
	send_to_char("Not while you're defending yourself!\n\r",ch);
	return;
	}
    }

    if (victim == ch)
    {
	send_to_char("huh?\n\r", ch);
	return;
    }

   obj = get_eq_char(ch,WEAR_WIELD);
   if (obj == NULL || obj->value[0] != WEAPON_DAGGER)
	{
	attempt_dual = TRUE;
	obj = get_eq_char(ch,WEAR_DUAL_WIELD);
	}

	if (obj == NULL)
	{
	send_to_char("You must wield a dagger to circle stab.\n\r",ch);
	return;
	}

    if (obj->value[0] != 2)
    {
	send_to_char("You must wield a dagger to circle stab.\n\r",ch);
	return;
    }

    chance += (ch->drain_level + ch->level - victim->level - victim->drain_level);
    chance = URANGE(5, chance, 95);

    WAIT_STATE(ch,skill_table[gsn_circle].beats);

    if (number_percent() < chance)
    {
   	act("You circle around $N to land a critical strike.",ch,NULL,victim,TO_CHAR);
	act("$n cirlces around you to land a critical strike.",ch,NULL,victim,TO_VICT);
	act("$n circles $N to land a critical strike.",ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_circle,TRUE,1);
	dam = dice(obj->value[1],obj->value[2]);
	dam += 40;

	if ((ch->drain_level + ch->level) <= 15)
		dam *= 1;
	else if ((ch->drain_level + ch->level) <= 20)
		dam *= 3/2;
	else if ((ch->drain_level + ch->level) < 25)
		dam *= 2;
	else if ((ch->drain_level + ch->level) < 30)
		dam *= 7/3;
	else if ((ch->drain_level + ch->level) < 40)
		dam *= 5/2;
	else if ((ch->drain_level + ch->level) <= 49)
		dam *= 7/2;
	else if ((ch->drain_level + ch->level) <= 55)
		dam *= 10/3;
	else dam *= 10/3;


	damage_old(ch,victim,dam,gsn_circle, attack_table[obj->value[3]].damage, TRUE);
    }
    else
    {
	check_improve(ch,gsn_circle,FALSE,1);

	damage_old(ch,victim,0,gsn_circle,DAM_NONE,TRUE);
    }

    return;
}


void do_surrender( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *mob;
    if ( (mob = ch->fighting) == NULL )
    {
        send_to_char( "But you're not fighting!\n\r", ch );
        return;
    }
    act( "You surrender to $N!", ch, NULL, mob, TO_CHAR );
    act( "$n surrenders to you!", ch, NULL, mob, TO_VICT );
    act( "$n tries to surrender to $N!", ch, NULL, mob, TO_NOTVICT );
    stop_fighting( ch, TRUE );
}


void do_sla( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to SLAY, spell it out.\n\r", ch );
    return;
}



void do_slay( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
        send_to_char( "Slay whom?\n\r", ch );
        return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( ch == victim )
    {
        send_to_char( "Suicide is a mortal sin.\n\r", ch );
        return;
    }

    if ( !IS_NPC(victim) && victim->level >= get_trust(ch))
    {
        send_to_char( "You failed.\n\r", ch );
        return;
    }

    act( "You point at $M and $S heart stops!",  ch, NULL, victim, TO_CHAR    );
    act( "$n points at you and your heart suddenly stops!", ch, NULL, victim, TO_VICT    );
    act( "$n points at $N and $E falls over clenching his chest!",  ch, NULL, victim, TO_NOTVICT );
    raw_kill(ch, victim );
    n_logf("%s has slain %s", ch->name, victim->name);
    return;
}


void spell_evil_eye(int sn,int level, CHAR_DATA *ch,void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam, modify, saves;

    if (victim == ch)
    {
	send_to_char("You can't do such a thing.\n\r",ch);
	return;
    }

    act("$n's eyes turn pitch-black and a surging darkness erupts at $N.",ch,NULL,victim,TO_NOTVICT);
    act("Your eyes turn pitch-black and a stream of pure negative energy streams out at $N.",ch,NULL,victim,TO_CHAR);
    send_to_char("You feel a sudden intense agony burning into your skull.\n\r",victim);

    if (IS_SET(victim->affected_by,AFF_BLIND) )
    {
	dam = dice(level,8);
	if (saves_spell(level,victim, DAM_NEGATIVE) )
	dam /= 2;
	damage_old(ch,victim,dam,sn,DAM_NEGATIVE, TRUE);
	return;
    }

   saves = 1;
   for (modify = 0; modify < 4; modify ++)
    {
	if (saves_spell(level, victim, DAM_NEGATIVE))
		saves++;
    }

    if (saves == 0)
    {
	act("$n's surging darkness devestates $N's skull, killing $M instantly!",ch,NULL,victim,TO_NOTVICT);
	act("Your darkness slays $N!",ch,NULL,victim,TO_CHAR);
	send_to_char("With a violent burning sensation your mind vaporises.\n\r",victim);
	raw_kill(ch,victim);
    	return;
    }

    dam = dice(level, 16);
    dam /= (saves + 1);

    damage_old(ch,victim,dam,sn,DAM_NEGATIVE,TRUE);
    return;

}


void spell_power_word_kill(int sn,int level,CHAR_DATA *ch,void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam,saves,modify;

    saves = 0;

    act("$n points a finger at $N and utters the word, 'Die'.",ch,NULL,victim,TO_NOTVICT);
    act("$n points a finger at you and utters the word, 'Die'.",ch,NULL,victim,TO_VICT);
    send_to_char("You intone a word of unholy power.\n\r",ch);

    for ( modify = 0; modify < 5; modify++)
    {
	if (saves_spell(level-1,victim,DAM_NEGATIVE) )
		saves++;
    }

    if(IS_IMMORTAL(ch) && (victim->level<ch->level)) { saves=0; }
    if (saves == 0)
    {
	act("$N shudders in shock as $S heart explodes!", ch, NULL,victim,TO_NOTVICT);
	send_to_char("You feel your heart rupture in a violent explosion of pain!\n\r",victim);
	act("Your word of power vaporises $N's heart, killing $M instantly!",ch,NULL,victim,TO_CHAR);
	raw_kill(ch,victim);
    	return;
    }
    dam = dice(level,14);
    dam /= saves;
    damage_old(ch,victim,dam,sn,DAM_NEGATIVE,TRUE);

	return;
}

void do_lunge( CHAR_DATA *ch, char *argument)
{
        CHAR_DATA *victim;
        int chance, dam;
        char arg[MAX_INPUT_LENGTH];
        OBJ_DATA *wield;
        int multiplier;

	dam = 0;
        one_argument(argument,arg);
        if ((chance = get_skill(ch,gsn_lunge) ) == 0
        || (ch->level < skill_table[gsn_lunge].skill_level[ch->class]) )
        {
        send_to_char("Lunging? What's that?\n\r",ch);
        return;
        }
        if (arg[0] == '\0')
        {
        victim = ch->fighting;
        if (victim == NULL)
                {
                send_to_char("But you aren't fighting anyone!\n\r",ch);
                return;
                }
        }
        else if ( (victim = get_char_room(ch,arg)) == NULL )
        {
                send_to_char("They aren't here.\n\r",ch);
                return;
        }
        if (victim == ch)
        {
        send_to_char("You can't lunge at yourself!\n\r",ch);
        return;
        }

        wield = get_eq_char(ch,WEAR_WIELD);
        if ((wield == NULL)
        || (( wield->value[0] != WEAPON_SWORD)
        && ( wield->value[0] != WEAPON_SPEAR)
        && ( wield->value[0] != WEAPON_POLEARM) ))
        {
        send_to_char("You must be wielding a sword, spear or polearm to lunge.\n\r",ch);
        return;
        }

        if (is_safe(ch,victim) )
        return;

	if (check_chargeset(ch,victim)) {return;}

        chance += ch->carry_weight/25;
        chance -= victim->carry_weight/20;
        chance += (ch->size - victim->size)*20;
        chance -= get_curr_stat(victim,STAT_DEX);
        chance += get_curr_stat(ch,STAT_STR)/3;
        chance += get_curr_stat(ch,STAT_DEX)/2;
        if (IS_AFFECTED(ch,AFF_HASTE) )
                chance += 10;
        if (IS_AFFECTED(victim,AFF_HASTE) )
                chance = 20;
        chance += (ch->drain_level + ch->level - victim->level - victim->drain_level);

        act("$n attempts to impale $N with a quick lunge!",ch,0,victim,TO_NOTVICT);
        act("You attempt to impale $N with a quick lunge!",ch,0,victim,TO_CHAR);
        act("$n attempts to impale you with a quick lunge!",ch,0,victim,TO_VICT);

        if (number_percent() < chance)
        {
        check_improve(ch,gsn_lunge,TRUE,1);
        WAIT_STATE(ch,skill_table[gsn_lunge].beats);
        if (wield->pIndexData->new_format)
                dam = dice(wield->value[1],wield->value[2]);
        else
                dam = number_range(wield->value[1],wield->value[2]);

        if (get_skill(ch,gsn_enhanced_damage) > 0 )
        {
        if (number_percent() <= get_skill(ch,gsn_enhanced_damage) )
                {
                check_improve(ch,gsn_enhanced_damage,TRUE,1);
dam += dam*(number_range(60,100)/100) * ch->pcdata->learned[gsn_enhanced_damage]/100;
                }
        }

        dam += GET_DAMROLL(ch);
        dam *= ch->pcdata->learned[gsn_lunge];
	dam /= 100;
        multiplier = number_range((ch->drain_level + ch->level)/8,(ch->drain_level + ch->level)/4);
        multiplier /= 10;
	multiplier += 6/4;
        dam *= multiplier;

        if (dam <= 0)
                dam = 1;
        damage_old(ch,victim,dam,gsn_lunge,DAM_PIERCE,TRUE);
        }
        else
        {
        damage_old(ch,victim,dam,gsn_lunge,DAM_PIERCE,TRUE);
        check_improve(ch,gsn_lunge,FALSE,1);
        WAIT_STATE(ch,skill_table[gsn_lunge].beats);
        }
        return;
}

void do_bandage(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *victim;
        char arg[MAX_INPUT_LENGTH];
        AFFECT_DATA af;

        one_argument(argument,arg);

        if ( (get_skill(ch,gsn_bandage) == 0)
        || (ch->level < skill_table[gsn_bandage].skill_level[ch->class]) )
        {
        	send_to_char("Huh?\n\r",ch);
        	return;
        }

	if (cabal_down(ch,CABAL_RAGER))
		return;

        if (is_affected(ch,gsn_bandage))
        {
        	send_to_char("You can't apply more aid yet.\n\r",ch);
        	return;
        }

        if (ch->mana < 50)
        {
        	send_to_char("You don't have the mana.\n\r",ch);
        	return;
        }

    	if (arg[0] == '\0')
		victim = ch;
        else if ((victim = get_char_room(ch,arg)) == NULL)
        {
        	send_to_char("They aren't here.\n\r",ch);
        	return;
        }

        if (number_percent() > ch->pcdata->learned[gsn_bandage])
        {
        	act("You fail to apply battle dressing to $N's wounds.",ch,0,victim,TO_CHAR);
        	act("$n fumbles with $s bandages but fails to use them effectively.",ch,0,0,TO_ROOM);
        	ch->mana -= 35;
        	check_improve(ch,gsn_bandage,FALSE,3);
        	return;
        }

        ch->mana -= 50;

        if (victim != ch)
        {
        	act("$n applies bandages to $N's battle wounds.",ch,0,victim,TO_NOTVICT);
        	act("You apply bandages to $N's battle wounds.",ch,0,victim,TO_CHAR);
        	act("$n applies bandages to your battle wounds.",ch,0,victim,TO_VICT);
        }
        else
        {
        	act("$n applies bandages to $mself.",ch,0,0,TO_ROOM);
        	send_to_char("You apply battle dressing to yourself.\n\r",ch);
        }

        send_to_char("You feel better.\n\r",victim);

        victim->hit = UMIN(victim->hit + (9*ch->level), victim->max_hit);
	if (number_percent() < 60)
	{
		if (IS_AFFECTED(victim,AFF_PLAGUE))
		{
			affect_strip(victim,gsn_plague);
			act("The sores on $n's body vanish.\n\r",victim,0,0,TO_ROOM);
			send_to_char("The sores on your body vanish.\n\r",victim);
		}
	}

	if (number_percent() < 60)
	{
	 	if (is_affected(victim, gsn_blindness))
	 	{
	  		affect_strip(victim, gsn_blindness);
	  		send_to_char("Your vision returns!\n\r",victim);
	 	}
	}

	if (number_percent() < 60)
	{
		if (is_affected(victim,gsn_poison))
		{
			affect_strip(victim,gsn_poison);
			send_to_char("A warm feeling goes through your body.\n\r",victim);
			act("$n looks better.",victim,0,0,TO_ROOM);
		}

		if (is_affected(victim,gsn_poison_dust))
		{
			affect_strip(victim,gsn_poison_dust);
			send_to_char("The dust in your eyes is cleaned out.\n\r",victim);
			act("The dust in $n's eyes looks cleaned out.",victim,0,0,TO_ROOM);
		}
	}

        check_improve(ch,gsn_bandage,TRUE,3);
	init_affect(&af);
        af.where 	= TO_AFFECTS;
        af.type 	= gsn_bandage;
        af.location 	= 0;
        af.duration 	= number_range(5,8);
        af.modifier 	= 0;
        af.bitvector 	= 0;
		char msg_buf[MSL];
		sprintf(msg_buf,"unable to bandage");
		af.affect_list_msg = str_dup(msg_buf);
        af.level 	= ch->level;
	af.aftype 	= AFT_POWER;
        affect_to_char(ch,&af);
        return;
}

void do_herb(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *victim;
        char arg[MAX_INPUT_LENGTH];
        AFFECT_DATA af;

        one_argument(argument,arg);

        if ( (get_skill(ch,gsn_herb) == 0)
        || (ch->level < skill_table[gsn_herb].skill_level[ch->class]) )
        {
        send_to_char("Huh?\n\r",ch);
        return;
        }
        if (is_affected(ch,gsn_herb))
        {
        send_to_char("You can't find any more herbs.\n\r",ch);
        return;
        }
    if ( !isInWilderness(ch) )
        {
        send_to_char("You can't find any herbs here.\n\r",ch);
        return;
        }

	if (arg[0] == '\0')
		victim = ch;
       else   if ((victim = get_char_room(ch,arg)) == NULL)
        {
        send_to_char("They aren't here.\n\r",ch);
        return;
        }

        if (number_percent() > ch->pcdata->learned[gsn_herb])
        {
        send_to_char("You search for herbs but fail to find any.\n\r",ch);
        act("$n looks about in the bushes but finds nothing.",ch,0,0,TO_ROOM);
        check_improve(ch,gsn_herb,FALSE,4);
        return;
        }

        if (victim != ch)
        {
        act("$n applies herbs to $N.",ch,0,victim,TO_NOTVICT);
        act("You apply herbs to $N.",ch,0,victim,TO_CHAR);
        act("$n applies herbs to you.",ch,0,victim,TO_VICT);
        }

	if (victim == ch)
	{
        act("$n applies herbs to $mself.",ch,0,0,TO_ROOM);
        send_to_char("You find herbs and apply them to yourself.\n\r",ch);
        }

	send_to_char("You feel better.\n\r",victim);

	if (IS_AFFECTED(victim,AFF_PLAGUE) && number_percent() > 30)
	{
	affect_strip(victim,gsn_plague);
	act("The sores on $n's body vanish.\n\r",victim,0,0,TO_ROOM);
	send_to_char("The sores on your body vanish.\n\r",victim);
	}

        check_improve(ch,gsn_herb,TRUE,4);

        victim->hit = UMIN(victim->hit + (4*(ch->drain_level + ch->level)), victim->max_hit);
	init_affect(&af);
        af.where = TO_AFFECTS;
        af.type = gsn_herb;
        af.location = 0;
        af.duration = 4;
        af.modifier = 0;
        af.bitvector = 0;
		char msg_buf[MSL];
		sprintf(msg_buf,"unable to search for herbs");
		af.affect_list_msg = str_dup(msg_buf);
	af.aftype = AFT_SKILL;
        af.level = ch->level + ch->drain_level;
        affect_to_char(ch,&af);
        return;
}

void do_cleave(CHAR_DATA *ch,char *argument)
{
	OBJ_DATA *weapon;
        CHAR_DATA *victim;
        char arg[MAX_INPUT_LENGTH];
        int dam, chance, dam_type;
        char buf[MAX_STRING_LENGTH];
        int sn;
	int skill;

        if ( ( ( chance = get_skill(ch,gsn_cleave)) == 0)
        || (ch->level < skill_table[gsn_cleave].skill_level[ch->class]) )
        {
        send_to_char("You don't know how to cleave.\n\r",ch);
        return;
        }

        one_argument(argument,arg);
        weapon = get_eq_char(ch,WEAR_WIELD);

    	if (weapon == NULL)
	{
        send_to_char("You need to wield a sword or axe to cleave.\n\r",ch);
        return;
        }

        if ((weapon->value[0] != WEAPON_SWORD)
        && (weapon->value[0] != WEAPON_AXE) )
        {
        send_to_char("You need to wield a sword or axe to cleave.\n\r",ch);
        return;
        }
        if ( (victim = get_char_room(ch,arg)) == NULL )
        {
        send_to_char("They aren't here.\n\r",ch);
        return;
        }
        if (victim == ch)
        {
        send_to_char("You can't do such a thing.\n\r",ch);
        return;
        }
        if (victim->fighting != NULL)
        {
        send_to_char("They are moving too much to cleave.\n\r",ch);
        return;
        }
        if (victim->hit < victim->max_hit*9/10)
        {
        send_to_char("They are too hurt and watchful to cleave right now...\n\r",ch);
        return;
        }

        if (is_safe(ch,victim))
                return;

	if (check_chargeset(ch,victim)) {return;}

        chance /= 5;
        if ((ch->drain_level + ch->level - victim->drain_level + victim->level) < 0)
                chance -= (ch->drain_level + ch->level - victim->drain_level - victim->level)*3;
        else
                chance += (ch->drain_level + ch->level - victim->level - victim->drain_level);
        chance += get_curr_stat(ch,STAT_STR)/2;
        chance -= get_curr_stat(victim,STAT_DEX)/3;  /* Improve evasion */
        chance -= get_curr_stat(victim,STAT_STR)/4;  /* Improve repelling */
        chance -= get_curr_stat(victim,STAT_CON)/4;  /* Shock survival */
        dam_type = attack_table[weapon->value[3]].damage;


        if (!can_see(victim,ch))
                chance += 5;
        chance -= dice(2,6);
        chance = URANGE(2,chance,10);

	if (IS_IMMORTAL(victim))
	    chance = 0;

	if (IS_CABAL_OUTER_GUARD(victim))
	    chance = 0;

        if (number_percent() < 50)
                sprintf(buf,"Die, %s you butchering fool!",PERS(ch,victim));
        else
                sprintf(buf,"Help! %s just tried to cleave me in half!",PERS(ch,victim));

        sn = get_weapon_sn(ch);
        skill = get_weapon_skill(ch,sn) + ch->pcdata->learned[gsn_cleave] + 10;
        skill = URANGE(0,skill/2,100);

        if (IS_IMP(ch)) {
                chance += 100;
        }

        act("You make a brutal swing at $N in an attempt to cleave them in half.",ch,0,victim,TO_CHAR);
        act("$n attempts to cleave you in half with a brutal slice.",ch,0,victim,TO_VICT);
        act("$n makes an attempt to cleave $N in half.",ch,0,victim,TO_NOTVICT);

	if (IS_IMP(victim)) {
		act("You make a brutal swing at $N in an attempt to cleave them in half, but miss and chop your own head off.",ch,0,victim,TO_CHAR);
		act("$n attempts to cleave you in half with a brutal slice, but misses and chops thier own head off.",ch,0,victim,TO_VICT);
		act("$n makes an attempt to cleave $N in half but misses and chops their own head off!",ch,0,victim,TO_NOTVICT);
		raw_kill(victim,ch);
		return;
	}

    if (IS_NPC(victim))
	victim->last_fought = ch;

        if (number_percent() > chance)
        {
            check_improve(ch,gsn_cleave,FALSE,5);
            if (weapon->pIndexData->new_format)
                dam = dice(weapon->value[1],weapon->value[2])*skill/100;
            else
                dam = number_range(weapon->value[1]*skill/100,weapon->value[2]*skill/100);
        
	    dam *= number_range((ch->drain_level + ch->level)/10,(ch->drain_level + ch->level)/7);
            damage_old(ch,victim,dam,gsn_cleave,dam_type,TRUE);
            if (!IS_NPC(victim) && !IS_NPC(ch) && victim->hit > 1)
                do_myell(victim,buf);
	    WAIT_STATE(ch,PULSE_VIOLENCE*3);
	    WAIT_STATE(victim,PULSE_VIOLENCE*2);
            return;
        }

    act("Your cleave slices $S body in half with a clean cut!",ch,0,victim,TO_CHAR);
    act("$n cleaves you in half, tearing your body into two bloody bits!",ch,0,victim,TO_VICT);
    act("$n cleaves $N into to bits of bloody flesh!",ch,0,victim,TO_NOTVICT);
    check_improve(ch,gsn_cleave,TRUE,5);
    raw_kill(ch,victim);
    return;
}


void do_deathstrike(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *victim;
        int chance, dam;
        char arg[MAX_INPUT_LENGTH];
        char buf[MAX_INPUT_LENGTH];
/*	AFFECT_DATA af;	*/

        one_argument(argument,arg);
        if (IS_NPC(ch))
                return;

	if ((get_skill(ch,gsn_deathstrike) == 0)
	|| ch->level < skill_table[gsn_deathstrike].skill_level[ch->class])
	{
	send_to_char("Huh?\n\r",ch);
	return;
	}

	if (is_affected(ch,gsn_deathstrike))
	{
	send_to_char("You can't build up the intensity to deathstrike yet.\n\r",ch);
	return;
	}

        if (arg[0] == '\0')
        {
        send_to_char("Attempt to destroy who's skull?\n\r",ch);
        return;
        }
        if ( (victim = get_char_room(ch,arg)) == NULL)
        {
        send_to_char("They aren't here.\n\r",ch);
        return;
        }
        if (victim == ch)
        {
        send_to_char("You can't do that.\n\r",ch);
        return;
        }
        if (victim->fighting != NULL || victim->position == POS_FIGHTING)
        {
        send_to_char("They are moving around too much to get in close for the kill.\n\r",ch);
        return;
        }

        if (is_safe(ch,victim))
                return;

        if (victim->hit < victim->max_hit)
        {
        send_to_char("They have too much blood on them right now to locate their strike point.\n\r",ch);
        return;
        }
        chance = get_skill(ch,gsn_deathstrike)/2;
        chance += ch->level;
        chance -= victim->level * 3/2;
        chance -= number_range(0,15);
        if (!can_see(victim,ch))
                chance += 10;
        if (victim->position == POS_FIGHTING)
                chance -= 25;
        else if (victim->position == POS_SLEEPING)
                chance += 10;
        else chance -= 10;

        chance /= 2;
        chance = URANGE(2,chance,90);

        act("$n strikes out at $N with deadly intensity.",ch,0,victim,TO_NOTVICT);
        act("You strike out at $N with deadly intensity.",ch,0,victim,TO_CHAR);
        act("$n strikes at you with deadly intensity.",ch,0,victim,TO_VICT);

        if (number_percent() < chance)
        {
        send_to_char("With agonising pain your skull is smashed by the blow!\n\r",victim);
        act("Your blow shatters $N's skull into bloody fragments!",ch,0,victim,TO_CHAR);
        act("$N's skull is shattered into bits of mangled flesh and bone by $n's strike!",ch,0,victim,TO_NOTVICT);
        raw_kill(ch,victim);
        check_improve(ch,gsn_deathstrike,TRUE,3);
        return;
        }
        else
        {
        send_to_char("You feel a sharp pain searing your skull!\n\r",victim);
        act("Your deathstrike smashes $N's skull but fails to kill.",ch,0,victim,TO_CHAR);
        dam = ch->level * 2;
        dam += dice(ch->level, 4);
        damage_old(ch,victim,dam,gsn_deathstrike,DAM_PIERCE,TRUE);
        check_improve(ch,gsn_deathstrike,FALSE,3);
        if (!IS_NPC(victim) && !IS_NPC(ch) && (victim->hit > 1))
               {
                sprintf(buf,"Help! %s just tried to kill me!",PERS(ch,victim));
                do_myell(victim,buf);
                }
        multi_hit(victim,ch,TYPE_UNDEFINED);
        }

        return;
}

void do_crush(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *victim;
        int chance, dam;

    if (IS_NPC(ch) && IS_AFFECTED(ch,AFF_CHARM))
	return;

        if ( (chance = get_skill(ch,gsn_crush)) == 0
        || ch->level < skill_table[gsn_crush].skill_level[ch->class])
        if (!IS_NPC(ch))
	{
                send_to_char("Crushing, how do you do that again?\n\r",ch);
                return;
        }
	if ((victim = ch->fighting) == NULL)
	{
	send_to_char("You aren't fighting anyone.\n\r",ch);
	return;
	}
    if (is_affected(victim,gsn_protective_shield))
    {
	act("$N's crush attempt seems to slide around you.",victim,0,ch,TO_CHAR);
	act("$N's crush attempt seems to slide around $n.",victim,0,ch,TO_NOTVICT);
	act("Your crush attempt seems to slide around $n.",victim,0,ch,TO_VICT);
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
    }

 if (is_affected(victim, skill_lookup("wraithform")))
    {
        act("$N's crush attempt seems to slide around you.",victim,0,ch,TO_CHAR);
        act("$N's crush attempt seems to slide around $n.",victim,0,ch,TO_NOTVICT);
        act("Your crush attempt seems to slide around $n.",victim,0,ch,TO_VICT);
        WAIT_STATE(ch,PULSE_VIOLENCE*2);
        return;
    }

	chance -= chance/4;
        chance += (ch->drain_level + ch->level - victim->drain_level - victim->level) * 2;
        chance += get_curr_stat(ch,STAT_STR);
        chance -= get_curr_stat(victim,STAT_DEX)/3;
        chance -= get_curr_stat(victim,STAT_STR)/2;
        if (ch->size < victim->size)
                chance += (ch->size - victim->size)*25;
        else
                chance += (ch->size - victim->size)*10;
        if (number_percent() < chance)
	{
	if (check_roll(ch,victim, gsn_crush))
	{
	check_improve(ch,gsn_crush,TRUE,2);
	act("$N rolls free of $n's crush.",ch,0,victim,TO_NOTVICT);
	act("You roll out of $n's crush attempt.",ch,0,victim,TO_VICT);
	act("$N rolls free of your crush attempt.",ch,0,victim,TO_CHAR);
	return;
	}
        act("$n grabs you and slams you to the ground with bone crushing force!",ch,0,victim,TO_VICT);
        act("You grab $N and slam $M to the ground with bone crushing force!",ch,0,victim,TO_CHAR);
        act("$n slams $N to the ground with bone crushing force!",ch,0,victim,TO_NOTVICT);
        check_improve(ch,gsn_crush,TRUE,4);
        if ((ch->drain_level + ch->level) < 20)
                dam = 20;
        else if ((ch->drain_level + ch->level) < 25)
                dam = 30;
        else if ((ch->drain_level + ch->level) < 30)
                dam = 40;
        else if ((ch->drain_level + ch->level) < 35)
                dam = 50;
        else if ((ch->drain_level + ch->level) < 40)
                dam = 60;
        else if ((ch->drain_level + ch->level) < 52)
                dam = 70;
        else
                dam = 70;
        dam += str_app[get_curr_stat(ch,STAT_STR)].todam;
        WAIT_STATE(victim,2*PULSE_VIOLENCE);
        WAIT_STATE(ch,2*PULSE_VIOLENCE);
        damage_old(ch,victim,dam,gsn_crush,DAM_BASH,TRUE);
        victim->position = POS_RESTING;

        if (ch->fighting == victim)
                check_ground_control(ch,victim,chance,dam);
        return;

	}
        act("Your crush attempt misses $N.",ch,0,victim,TO_CHAR);
        act("$n lashes out wildly with $s arms but misses.",ch,0,0,TO_ROOM);
        WAIT_STATE(ch,2*PULSE_VIOLENCE);
        check_improve(ch,gsn_crush,FALSE,3);
        return;

}

void check_ground_control(CHAR_DATA *ch,CHAR_DATA *victim,int chance,int dam)
{
	if (chance < 10)
		return;
	if (dam == 0)
		return;

        chance += get_skill(ch,gsn_ground_control)/2;
        chance /= 3;

  	chance -= number_range(5,15);
	chance = URANGE(5,chance,60);

        if (ch->level < skill_table[gsn_ground_control].skill_level[ch->class])
                return;
        if (number_percent() > chance)
	{
                check_improve(ch,gsn_ground_control,FALSE,3);
                return;

	}
        act("With brutal skill you grind $N against the ground with your weight.",ch,0,victim,TO_CHAR);
        act("$n takes hold of $N and grinds $M against the ground.",ch,0,victim,TO_NOTVICT);
        act("$n grinds you against the ground with brutal skill.",ch,0,victim,TO_VICT);
        dam -= 10;
        check_improve(ch,gsn_ground_control,TRUE,2);
        damage_old(ch,victim,dam,gsn_ground_control,DAM_BASH,TRUE);
        WAIT_STATE(victim,2*PULSE_VIOLENCE);
        return;

}


void do_riot(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *gch;
    int bonus;
     AFFECT_DATA af;

    if ( (get_skill(ch,gsn_riot) == 0 )
	|| ch->level < skill_table[gsn_riot].skill_level[ch->class])
	{
	send_to_char("Huh?\n\r",ch);
	return;
 	}

    if (ch->mana < 40)
    {
	send_to_char("You don't have the mana.\n\r",ch);
	return;
    }

    if (is_affected(ch,gsn_riot))
    {
	send_to_char("You can't stir up another riot yet.\n\r",ch);
	return;
    }
    if (number_percent() > ch->pcdata->learned[gsn_riot])
    {
	send_to_char("You try to incite a riot amongst your group but fail.\n\r",ch);
	act("$n gestures around angrily, trying to build up anger and discord but fails.",ch,0,0,TO_ROOM);
	ch->mana -= 20;
	check_improve(ch,gsn_riot,FALSE,2);
	return;
    }

    send_to_char("You stir up a riot with your actions!\n\r",ch);
	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype = AFT_POWER;
    af.type = gsn_riot;
    af.level = ch->level + ch->drain_level;
    af.bitvector = 0;
    af.duration = ( 2 + (ch->drain_level + ch->level)/8);
    af.location = APPLY_HITROLL;

    if ((ch->drain_level + ch->level) < 20)
	bonus = 6;
    else if ((ch->drain_level + ch->level) < 25)
	bonus = 7;
    else if ((ch->drain_level + ch->level) < 30)
	bonus = 8;
    else if ((ch->drain_level + ch->level) < 35)
	bonus = 9;
    else if ((ch->drain_level + ch->level) < 38)
	bonus = 10;
    else
	bonus = 12;

    af.modifier = bonus;
    affect_to_char(ch,&af);
    af.location = APPLY_DAMROLL;
    affect_to_char(ch,&af);
    send_to_char("You are filled with terrible fury!\n\r",ch);

    for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room)
    {
	if (!is_same_group(ch,gch) || !IS_AWAKE(gch))
		continue;
	if (is_affected(gch,gsn_riot))
		continue;
	if (gch->trust > 51)
	        continue;
	if (gch == ch)
		continue;
	af.location = APPLY_DAMROLL;
	affect_to_char(gch,&af);
	af.location = APPLY_HITROLL;
	affect_to_char(gch,&af);
	send_to_char("You are filled with a terrible fury!\n\r",gch);
    	act("$n gets a terrible look of anger in $s eyes.",gch,0,0,TO_ROOM);
    }

    check_improve(ch,gsn_riot,TRUE,2);
    ch->mana -= 40;
    return;
}



void do_tail(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance, dam;
    int knock;

    one_argument(argument,arg);

    if (IS_NPC(ch) && IS_AFFECTED(ch,AFF_CHARM))
	return;

    if ( (chance = get_skill(ch,gsn_tail)) == 0
    ||   (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_TAIL))
    ||   (!IS_NPC(ch)
    &&    ch->level < skill_table[gsn_tail].skill_level[ch->class]))
    {
	send_to_char("You don't have a tail to strike with like that.\n\r",ch);
        return;
    }

    if (arg[0] == '\0')
    {
        victim = ch->fighting;
        if (victim == NULL)
        {
            send_to_char("But you aren't fighting anyone!\n\r",ch);
            return;
        }
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }

    chance = URANGE(5,chance,90);

    chance += (ch->level - victim->level);
    chance -= get_curr_stat(victim,STAT_DEX);
    chance += get_curr_stat(ch,STAT_DEX)/3;
    chance += get_curr_stat(ch,STAT_STR)/3;

	act("$n violently lashes out with $s tail.",ch,0,0,TO_ROOM);
	send_to_char("You violently lash out with your tail!\n\r",ch);

    if (number_percent() > chance)
    {
	damage_old(ch,victim,0,gsn_tail,DAM_BASH,TRUE);
	return;
    }

    dam = dice(ch->level, 3);
    knock = (ch->level + 15);

    damage_old(ch,victim,dam,gsn_tail,DAM_BASH,TRUE);
    if (number_percent() < knock)
    {
	act("$n is sent crashing to the ground by the force of the blow!",victim,0,0,TO_ROOM);
	send_to_char("The tail strike sends you crashing to the ground!\n\r",victim);
	WAIT_STATE(victim,( 3 * PULSE_VIOLENCE/2));
    }
    WAIT_STATE(ch,2*PULSE_VIOLENCE);
    return;
}


void do_spellbane(CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;

	if (get_skill(ch,gsn_spellbane) < 10
	|| ch->level < skill_table[gsn_spellbane].skill_level[ch->class])
	{
	send_to_char("Huh?\n\r",ch);
	return;
	}
	if (is_affected(ch,gsn_spellbane))
	{
	send_to_char("You are already turning spells.\n\r",ch);
	return;
	}

	if (ch->mana < 50)
	{
send_to_char("You don't have the mana.\n\r",ch);
	return;
	}

	if (number_percent() > get_skill(ch,gsn_spellbane))
	{
	send_to_char("You think of your hatred of magic but can not sustain it.\n\r",ch);
act("$n makes some angry gestures and goes red in the face, but nothing happens.",ch,0,0,TO_ROOM);
	ch->mana -= 25;
	check_improve(ch,gsn_spellbane,FALSE,1);
	return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = gsn_spellbane;
	af.level = ch->level + ch->drain_level;
	af.duration = (5 + (ch->drain_level + ch->level)/5);
	af.location = APPLY_SAVES;
	af.modifier = (-5 -ch->level/4);
	af.bitvector = 0;
	af.aftype = AFT_POWER;

	affect_to_char(ch,&af);
	char msg_buf[MSL];
	sprintf(msg_buf,"grants a chance to reprise against magic");
	af.affect_list_msg = str_dup(msg_buf);
	af.location = APPLY_NONE;
	af.modifier = 0;
	affect_to_char(ch,&af);

	act("$n fills the air with $s hatred of magic.",ch,0,0,TO_ROOM);
	send_to_char("You surround yourself with your hatred of magic.\n\r",ch);
    if (is_affected(ch,gsn_undead_drain))
    {
	if (saves_spell(50,ch,DAM_OTHER))
	{
	send_to_char("Your spellbane burns away the negative draining magic of the undead.\n\r",ch);

	affect_strip(ch,gsn_undead_drain);
	}
    }

check_improve(ch,gsn_spellbane,TRUE,1);
	ch->mana -= 50;
	return;

}

bool spellbaned(CHAR_DATA *caster, CHAR_DATA *victim, int sn)
{
	bool status = FALSE;
  	int chance;

    	if (cabal_down_new(victim,CABAL_RAGER,FALSE))
		return FALSE;

     	if (!is_affected(victim,gsn_spellbane))
    	{
		return FALSE;
    	}

	if(number_percent()<26) 
 	{
		return FALSE;
	}

    	if ( sn >= TYPE_HIT && sn < (TYPE_HIT + MAX_DAMAGE_MESSAGE) )
		return FALSE;

    	if (skill_table[sn].slot == 0)
	  	return FALSE;

    	if (victim->position==POS_SLEEPING)
		return FALSE;

	if (caster == victim)
		return FALSE;

    	if ( skill_table[sn].target == TAR_CHAR_DEFENSIVE)
		status = TRUE;
    	else
	{
		chance = 50 + (victim->level - caster->level) * 2;
		chance -= victim->saving_throw;
		chance = URANGE(10,chance,50);

		if (IS_NPC(victim) &&
		victim->pIndexData->vnum == MOB_VNUM_RAGER)
			chance += 25;

		if (number_percent() < chance)
			status = TRUE;
	}

	if (status)
	{
		act("$n's spellbane reflects $N's spell!",victim,0,caster,TO_NOTVICT);
		act("Your spellbane reflects $N's spell!",victim,0,caster,TO_CHAR);
		act("$n's spellbane reflects your spell!",victim,0,caster,TO_VICT);

    		if (skill_table[sn].target != TAR_CHAR_OFFENSIVE)
      			damage_old(victim,caster,dice(caster->level,2),gsn_spellbane,DAM_ENERGY,TRUE);	
	}
    	return status;
}

void do_throw(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *victim;
    int dam, chance;
    char arg[MAX_INPUT_LENGTH];

    if ( (chance = get_skill(ch,gsn_throw)) == 0
    || ch->level < skill_table[gsn_throw].skill_level[ch->class])
    {
        send_to_char("Throwing? What's that?\n\r",ch);
        return;
    }

    one_argument(argument,arg);
    if (arg[0] == '\0')
        victim = ch->fighting;
    else
        victim = get_char_room(ch,arg);

    if (victim == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }
    if (victim == ch)
    {
        send_to_char("Now that would be a bit stupid.\n\r",ch);
        return;
    }
    if ((victim->fighting != ch) && (ch->fighting != victim))
    {
        send_to_char("But you aren't engaged in combat with them.\n\r",ch);
        return;
    }

    if (hands_full(ch))
    {
        send_to_char("You can't throw, your hands are filled with items.\n\r",ch);
        return;
    }
/*
    if (is_affected(ch,gsn_palm))
    {
        send_to_char("You can't throw, you're fighting with open hand palms.\n\r",ch);
        return;
    }
*/
    if (is_affected(victim,gsn_protective_shield))
    {
	act("$N's throw seems to slide around you.",victim,0,ch,TO_CHAR);
	act("$N's throw seems to slide around $n.",victim,0,ch,TO_NOTVICT);
	act("Your throw seems to slide around $n.",victim,0,ch,TO_VICT);
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
    }
    chance += (ch->drain_level + ch->level - victim->level - victim->drain_level);
    chance -= get_curr_stat(victim,STAT_DEX);
    chance += get_curr_stat(ch,STAT_DEX)/2;
    chance *= 9;
    chance /= 10;
    chance = URANGE(5, chance, 90);

    if (number_percent() > chance)
    {
        dam = 0;
	check_improve(ch,gsn_throw,FALSE,3);
    }
    else
    {
	if (check_roll(ch,victim,gsn_throw))
	{
	check_improve(ch,gsn_throw,TRUE,2);
	act("$N rolls free of $n's throw.",ch,0,victim,TO_NOTVICT);
	act("You roll out of $n's throw attempt.",ch,0,victim,TO_VICT);
	act("$N rolls free of your throw attempt.",ch,0,victim,TO_CHAR);
	return;
	}

        if ((ch->drain_level + ch->level) <= 18)    dam = 30;
        else if ((ch->drain_level + ch->level) <= 22)   dam = 35;
        else if ((ch->drain_level + ch->level) <= 25)   dam = 40;
        else if ((ch->drain_level + ch->level) <= 28)   dam = 45;
        else if ((ch->drain_level + ch->level) <= 32)   dam = 48;
        else if ((ch->drain_level + ch->level) <= 35)   dam = 52;
        else if ((ch->drain_level + ch->level) <= 38)   dam = 56;
        else if ((ch->drain_level + ch->level) <= 40)   dam = 60;
        else if ((ch->drain_level + ch->level) <= 43)   dam = 64;
        else if ((ch->drain_level + ch->level) <= 45)   dam = 68;
        else if ((ch->drain_level + ch->level) <= 48)   dam = 70;
        else                        dam = 72;

	act("$n grabs $N and throws $M to the ground with stunning force!",ch,0,victim,TO_NOTVICT);
	act("You grab $N and throw $M to the ground with stunning force!",ch,0,victim,TO_CHAR);
	act("$n grabs you and throws you to the ground with stunning force!",ch,0,victim,TO_VICT);

        dam += str_app[get_curr_stat(ch,STAT_STR)].todam;
        check_improve(ch,gsn_throw,TRUE,3);
        WAIT_STATE(victim,2*PULSE_VIOLENCE);
    }

    damage_old(ch,victim,dam,gsn_throw,DAM_BASH,TRUE);
    WAIT_STATE(ch,2*PULSE_VIOLENCE);
    if (ch->fighting == victim)
        check_ground_control(ch,victim,chance,dam);

    return;
}

void do_palm(CHAR_DATA *ch,char *argument)
{
    AFFECT_DATA af;
    int chance;

    if ( (chance = get_skill(ch,gsn_palm)) == 0
    || ch->level < skill_table[gsn_palm].skill_level[ch->class])
    {
        send_to_char("You don't have the ability to use palm striking martial arts.\n\r",ch);
        return;
    }

    if (is_affected(ch,gsn_palm))
    {
        send_to_char("You're already fighting with concentrated open hand martial arts.\n\r",ch);
        return;
    }

    if (ch->mana <= 60)
    {
        send_to_char("You don't have the mana.\n\r",ch);
        return;
    }

    if (get_eq_char(ch,WEAR_WIELD) != NULL
	|| get_eq_char(ch,WEAR_DUAL_WIELD) != NULL)
	{
	send_to_char("You can't start using open palm striking while using weapons.\n\r",ch);
	return;
	}

    if (number_percent() >= chance)
    {
        send_to_char("You flex your hands but cannot grasp the concentration required.\n\r",ch);
        check_improve(ch,gsn_palm,FALSE,2);
        ch->mana -= 30;
        return;
    }

    act("$n slowly spreads $s hands and a look of deadly concentration clouds $s eyes.",ch,0,0,TO_ROOM);
    send_to_char("You feel your mind relax and pass power to your opened hands.\n\r",ch);
	init_affect(&af);
    af.where = TO_AFFECTS;
    af.type = gsn_palm;
    af.location = 0;
    af.modifier = 0;
    af.duration = ((ch->drain_level + ch->level)/3);
    af.level = ch->drain_level + ch->level;
    af.bitvector = 0;
	af.aftype = AFT_SKILL;
	char msg_buf[MSL];
	sprintf(msg_buf,"utilizing open palm martial arts");
	af.affect_list_msg = str_dup(msg_buf);
    affect_to_char(ch,&af);
    check_improve(ch,gsn_palm,TRUE,2);

    return;
}

void do_nerve(CHAR_DATA *ch,char *argument)
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    AFFECT_DATA af;
    int chance;

    if ( (chance = get_skill(ch,gsn_nerve)) == 0
    || ch->level < skill_table[gsn_nerve].skill_level[ch->class])
    {
        send_to_char("You don't know how to use nerve pressure tactics.\n\r",ch);
        return;
    }

    one_argument(argument,arg);
    if (arg[0] == '\0')
        victim = ch->fighting;
    else
        victim = get_char_room(ch,arg);

    if (victim == NULL)
    {
        send_to_char("Attempt to put pressure on who's nerves?\n\r",ch);
        return;
    }
    if (victim == ch)
    {
        send_to_char("You can't do that.\n\r",ch);
        return;
    }
    if (is_safe(ch,victim))
	return;

    if (is_affected(victim,gsn_nerve))
    {
	send_to_char("They have already been weakened using nerve pressure.\n\r",ch);
	return;
    }


    chance += (ch->drain_level + ch->level - victim->level  - victim->drain_level)*3;
    chance -= get_curr_stat(victim,STAT_DEX)/3;
    chance += get_curr_stat(ch,STAT_DEX)/2;
    chance -= get_curr_stat(victim,STAT_CON)/3;

    if (number_percent() > chance)
    {
        act("$n grasps $N's arm but fails to apply the right pressure point.",ch,0,victim,TO_NOTVICT);
        act("You grasp $N's arm but fail to apply the right pressure point.",ch,0,victim,TO_CHAR);
        act("$n grasps your arm but fails to apply the right pressure point.",ch,0,victim,TO_VICT);
        check_improve(ch,gsn_nerve,FALSE,3);
        WAIT_STATE(ch,PULSE_VIOLENCE);
        return;
    }
    else
    {
    act("$n grasps $N's arm and weakens $m with pressure points.",ch,0,victim,TO_NOTVICT);
    act("You grasp $N's arm and weaken $m with pressure points.",ch,0,victim,TO_CHAR);
    act("$n grasps your arm and weakens you with pressure point.",ch,0,victim,TO_VICT);
    check_improve(ch,gsn_nerve,TRUE,3);
	init_affect(&af);
    af.where = TO_AFFECTS;
    af.type = gsn_nerve;
    af.location = APPLY_STR;
    af.duration = ((ch->drain_level + ch->level)/5);
    af.bitvector = 0;
    af.modifier = -3;
	af.aftype = AFT_SKILL;
    af.level = ch->drain_level + ch->level;
    affect_to_char(victim,&af);
    WAIT_STATE(ch,PULSE_VIOLENCE);
    }

	if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (ch->fighting == NULL
	|| victim->fighting == NULL) )
	{
sprintf(buf,"Help, %s is attacking me!",PERS(ch,victim));
do_myell(victim,buf);
	}
    if (victim->fighting == NULL)
    {
	multi_hit(victim,ch,TYPE_UNDEFINED);
    }

    return;
}

void check_follow_through(CHAR_DATA *ch, CHAR_DATA *victim, int dam)
{
    int chance;

    chance = get_skill(ch,gsn_follow_through);

    if (ch->level <skill_table[gsn_follow_through].skill_level[ch->class])
        return;

    chance /= 2;
    chance -= number_range(0,15);

    if (IS_AFFECTED(ch, AFF_HASTE))
	chance += 15;

    if (chance <= 0)
	return;

    if (number_percent() < chance)
    {
        act("$n's leg sweeps around and strikes with a follow through!",ch,0,0,TO_ROOM);
        act("Your leg sweeps around to land a follow through kick!",ch,0,0,TO_CHAR);
        damage_old(ch,victim, dam, gsn_follow_through, DAM_BASH,TRUE);
        check_improve(ch,gsn_follow_through,TRUE,2);
    }
    else
        check_improve(ch,gsn_follow_through,FALSE,3);

    return;
}



void do_blindness_dust(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;
        AFFECT_DATA af;
        int chance;
        bool fighting = FALSE;
        char buf[MAX_STRING_LENGTH];

        if ( (chance = get_skill(ch,gsn_blindness_dust)) == 0
        || ch->level < skill_table[gsn_blindness_dust].skill_level[ch->class])
        {
        send_to_char("You don't know how to make blindness dust to throw.\n\r",ch);
        return;
        }
        if (ch->mana < 18)
        {
        send_to_char("You don't have the mana.\n\r",ch);
        return;
        }
        if (number_percent() > chance)
        {
        act("$n hurls some dust into the air but it is blown away.",ch,0,0,TO_ROOM);
        send_to_char("You throw out some dust but it is blown away.\n\r",ch);
        ch->mana -= 9;
        check_improve(ch,gsn_blindness_dust,FALSE,2);
        WAIT_STATE(ch,12);
        return;
        }
        act("$n hurls a handful of dust into the room!",ch,0,0,TO_ROOM);
        send_to_char("You throw a handful of blindness dust into the room!\n\r",ch);
        check_improve(ch,gsn_blindness_dust,TRUE,2);
        if (ch->fighting != NULL)
                fighting = TRUE;

	init_affect(&af);
        af.where = TO_AFFECTS;
        af.type = gsn_blindness_dust;
        af.level = ch->drain_level + ch->level;
        af.duration = 1;
        af.bitvector = AFF_BLIND;
        af.location = APPLY_HIT;
        af.modifier = -3;
	af.aftype = AFT_SKILL;
        ch->mana -= 18;
        WAIT_STATE(ch,12);
        for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
        {
        vch_next = vch->next_in_room;
        if (is_safe(ch,vch))
		 continue;
        if (is_same_group(ch,vch))
		continue;
        if (!IS_AFFECTED(vch,AFF_BLIND)
        && !saves_spell(ch->drain_level + ch->level,vch,DAM_OTHER))
	{
	act("$n appears blinded.",vch,0,0,TO_ROOM);
	send_to_char("You get dust in your eyes.\n\r",vch);
                affect_to_char(vch,&af);
	}
        if (!IS_NPC(vch) && !IS_NPC(ch)
        && (vch->fighting == NULL || (!fighting)))
                {
        sprintf(buf,"Help! %s just threw dust in my eyes!",PERS(ch,vch));
                do_myell(vch,buf);
                }

                if (vch->fighting == NULL)
                        multi_hit(vch,ch,TYPE_UNDEFINED);

        }

        return;
}

void do_poison_dust(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;
        AFFECT_DATA af;
        int chance;
        bool fighting = FALSE;
        char buf[MAX_STRING_LENGTH];

        if ( (chance = get_skill(ch,gsn_poison_dust)) == 0
        || ch->level < skill_table[gsn_poison_dust].skill_level[ch->class])
        {
        send_to_char("You don't know how to make poison dust to throw.\n\r",ch);
        return;
        }
        if (ch->mana < 20)
        {
        send_to_char("You don't have the mana.\n\r",ch);
        return;
        }
        if (number_percent() > chance)
        {
        act("$n hurls some dust into the air but it is blown away.",ch,0,0,TO_ROOM);
        send_to_char("You throw out some dust but it is blown away.\n\r",ch);
        ch->mana -= 10;
        WAIT_STATE(ch,12);
        check_improve(ch,gsn_poison_dust,FALSE,2);
        return;
        }
        act("$n hurls a handful of dust into the room!",ch,0,0,TO_ROOM);
        send_to_char("You throw a handful of poison dust into the room!\n\r",ch);
        check_improve(ch,gsn_poison_dust,TRUE,2);
          if (ch->fighting != NULL)
                fighting = TRUE;

	init_affect(&af);
        af.where = TO_AFFECTS;
        af.type = gsn_poison_dust;
        af.level = ch->drain_level + ch->level;
        af.duration = (ch->drain_level + ch->level)/5;
        af.bitvector = AFF_POISON;
        af.location = APPLY_STR;
        af.modifier = -3;
	af.aftype = AFT_SKILL;
        ch->mana -= 20;
        WAIT_STATE(ch,12);
        for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
        {
        vch_next = vch->next_in_room;
        if (is_safe(ch,vch))    continue;
        if (is_same_group(ch,vch))      continue;
        if (!IS_AFFECTED(vch,AFF_POISON)
        && !saves_spell(ch->drain_level + ch->level,vch,DAM_OTHER))
	{
	act("$n turns green and looks sick.",vch,0,0,TO_ROOM);
	send_to_char("You suddenly feel very sick.\n\r",vch);
                affect_to_char(vch,&af);
	}

        if (!IS_NPC(vch) && !IS_NPC(ch)
        && (vch->fighting == NULL || (!fighting)))
                {
        sprintf(buf,"Help! %s just threw dust in my eyes!",PERS(ch,vch));
                do_myell(vch,buf);
                }
                if (vch->fighting == NULL)
                        multi_hit(vch,ch,TYPE_UNDEFINED);

        }
        return;
}

bool check_roll(CHAR_DATA *ch, CHAR_DATA *victim, int dt)
{
        int chance;

        if ( (chance = get_skill(victim,gsn_roll)) == 0
        || victim->level < skill_table[gsn_roll].skill_level[victim->class])
                return FALSE;

        chance *= 4;
        chance /= 5;
        chance += get_curr_stat(victim,STAT_DEX);
        chance -= victim->carry_weight/100;
        if (number_percent() > chance)
                {
                check_improve(victim,gsn_roll,FALSE,2);
                return FALSE;
                }

        check_improve(victim,gsn_roll,TRUE,2);
        return TRUE;
}

void do_warcry(CHAR_DATA *ch,char *argument)
{
        AFFECT_DATA af;
        int chance;
        if ( (chance = get_skill(ch,gsn_warcry)) == 0
        || ch->level < skill_table[gsn_warcry].skill_level[ch->class])
        {
        send_to_char("You don't know how to warcry properly.\n\r",ch);
        return;
        }

        if (is_affected(ch,gsn_warcry))
        {
        send_to_char("You are already affected by a warcry.\n\r",ch);
        return;
        }
        if (ch->mana < 20)
        {
        send_to_char("You don't have the mana.\n\r",ch);
        return;
        }

        if (number_percent() > chance)
        {
        act("$n makes some soft grunting noises.",ch,0,0,TO_ROOM);
        send_to_char("You make soft grunting sounds but nothing happens.\n\r",ch);
        check_improve(ch,gsn_warcry,FALSE,2);
        ch->mana -= 10;
        WAIT_STATE(ch,12);
        return;
        }

        act("$n lets out a blood freezing warcry!",ch,0,0,TO_ROOM);
        send_to_char("You let out a fierce warcry!\n\r",ch);
        check_improve(ch,gsn_warcry,TRUE,2);
	init_affect(&af);
        af.where = TO_AFFECTS;
	af.aftype = AFT_SKILL;
        af.type = gsn_warcry;
        af.modifier = (ch->drain_level + ch->level)/7;
        af.location = APPLY_HITROLL;
        af.duration = ch->drain_level + ch->level;
        af.bitvector = 0;
        af.level = ch->drain_level + ch->level;
        affect_to_char(ch,&af);
	af.location = APPLY_SAVES;
        af.modifier = -(ch->level/10);
        affect_to_char(ch,&af);
        ch->mana -= 20;
        WAIT_STATE(ch,12);
        return;
}

void check_downstrike(CHAR_DATA *ch,CHAR_DATA *victim)
{
    OBJ_DATA *wield;
    int dam;
    int chance;

    chance = get_skill(ch,gsn_downstrike);

    if (chance <= 10)	
	return;

    if (ch->level < skill_table[gsn_downstrike].skill_level[ch->class])
	return;

    chance /= 4.5;
    chance += (ch->drain_level + ch->level - victim->level - victim->drain_level);
    chance -= get_curr_stat(victim,STAT_DEX)/3;
    chance += get_curr_stat(ch,STAT_DEX)/3;

    chance = URANGE(2, chance, 35);

    if (number_percent() > chance)
    {
    	check_improve(ch,gsn_downstrike,FALSE,1);
	return;
    }

    wield = get_eq_char(ch,WEAR_WIELD);
    if (wield == NULL || wield->value[0]!=2)
	wield = get_eq_char(ch,WEAR_DUAL_WIELD);
    if (wield == NULL || wield->value[0]!=2)
	return;

    dam = dice(wield->value[1],wield->value[2]);
    dam += ch->damroll;

    act("You strike at $N with a downward thrust of your dagger!",ch,0,victim,TO_CHAR);
    act("$n strikes at you with a downward thrust of $s dagger!",ch,0,victim,TO_VICT);
    act("$n strikes at $N with a downward thrust of $s dagger!",ch,0,victim,TO_NOTVICT);
    check_improve(ch,gsn_downstrike,TRUE,1);
    damage_old(ch,victim,dam,gsn_downstrike,attack_table[wield->value[3]].damage, TRUE);
    return;
}




void do_enlist(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    int level;
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA af;
    int chance;
    CHAR_DATA *check;

    chance = get_skill(ch,gsn_enlist);
    if (chance <= 10
    || ch->level < skill_table[gsn_enlist].skill_level[ch->class])
    {
        send_to_char("You do not have the skills required to enlist mercenary aid.\n\r",ch);
        return;
    }
    one_argument(argument,arg);
    if (is_affected(ch,gsn_enlist))
    {
	send_to_char("You haven't built up the nerve to recruit anyone else yet.\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
        send_to_char("Who do you wish to enlist as a follower?\n\r",ch);
        return;
    }
    if ((victim = get_char_room(ch,arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }
    if (victim == ch)
    {
        send_to_char("You can't recruit yourself.\n\r",ch);
        return;
    }
    if (!IS_NPC(victim))
    {
        send_to_char("Why not just ask them to join you?\n\r",ch);
        return;
    }
    for (check = char_list; check != NULL; check = check->next)
    {
	if (is_affected(check,gsn_enlist) && check->master == ch)
	{
	send_to_char("You already have a devoted recruit following you.\n\r",ch);
	return;
	}
    }

    if (!IS_SET(victim->act,ACT_WARRIOR))
    {
	send_to_char("You may only enlist the aid of those trained as warriors.\n\r",ch);
        return;
    }

    level = ch->drain_level + ch->level;

    if ((IS_GOOD(ch) && IS_EVIL(victim))
       || (IS_EVIL(ch) && IS_GOOD(victim)))
       level = 0;

    if (IS_NEUTRAL(victim) || IS_NEUTRAL(ch))
        level -= 4;

    if (IS_AFFECTED(victim,AFF_CHARM))
        level = 0;
    if ( (victim->level + 8) > level)
	level = 0;

    if (saves_spell(level,victim,DAM_OTHER) || level == 0)
    {
        if (number_percent() < 50)
            do_say(victim,"I don't follow scum such as you!");
        else
            do_say(victim,"You couldn't recruit a mangy dog to your company!");
        sprintf(buf,"Help! I'm being attacked by %s!",PERS(victim,ch));
        do_myell(ch,buf);
        multi_hit(victim,ch,TYPE_UNDEFINED);
        return;
    }

    victim->leader = ch;
    victim->master = ch;
	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype = AFT_SKILL;
    af.type = gsn_enlist;
    af.modifier = 0;
    af.level = ch->drain_level + ch->level;
    af.bitvector = AFF_CHARM;
    af.duration = ch->drain_level + ch->level;
    af.location = 0;
    affect_to_char(victim,&af);
    af.bitvector = 0;
    af.duration = 15;
	af.affect_list_msg = str_dup("unable to enlist aid");
    affect_to_char(ch,&af);
    act("$N salutes you and prepares to follow into combat.",ch,0,victim,TO_CHAR);
    act("$N salutes $n and falls into line with $s.",ch,0,victim,TO_NOTVICT);
    return;
}



void do_tame(CHAR_DATA *ch, char *argument)
{
        CHAR_DATA *victim;
        char arg[MAX_INPUT_LENGTH];
        int chance;

        chance = get_skill(ch,gsn_tame);
        one_argument(argument,arg);
        if (arg[0] == '\0')
                victim = ch->fighting;
        else
                victim = get_char_room(ch,arg);
        if (victim == NULL)
        {
        send_to_char("They aren't here to tame though.\n\r",ch);
        return;
        }
        if (!IS_NPC(victim))
        {
        send_to_char("Why not just talk to them about your problems?\n\r",ch);
        return;
        }
        else if (!IS_SET(victim->act,ACT_AGGRESSIVE)
        && !IS_SET(victim->off_flags,SPAM_MURDER))
        {
        send_to_char("They are as tame as they can be.\n\r",ch);
        return;
        }

        chance += (ch->drain_level + ch->level - victim->level - victim->drain_level)*3;
        if (chance > 90)        chance = 90;
        WAIT_STATE(ch,12);
        if (number_percent() > chance)
        {
        act("$n tries to calm down $N but fails.",ch,0,victim,TO_NOTVICT);
        act("$n tries to calm you down but fails.",ch,0,victim,TO_VICT);
        act("You try to calm $N down but fail.",ch,0,victim,TO_CHAR);
        check_improve(ch,gsn_tame,FALSE,2);
        return;
        }
        act("$n calms $N down.",ch,0,victim,TO_NOTVICT);
        act("You calm $N down.",ch,0,victim,TO_CHAR);
        act("$n calms you down.",ch,0,victim,TO_VICT);
        check_improve(ch,gsn_tame,TRUE,2);
        stop_fighting(victim,TRUE);
     victim->last_fought = NULL;
        REMOVE_BIT(victim->act,ACT_AGGRESSIVE);
        REMOVE_BIT(victim->off_flags,SPAM_MURDER);
        return;
}

void do_find_water(CHAR_DATA *ch,char *argument)
{
        OBJ_DATA *spring;
        int chance;
        chance = get_skill(ch,gsn_find_water);
        if (chance > 95)        chance = 95;
        if (chance == 0
        || ch->level < skill_table[gsn_find_water].skill_level[ch->class])
        {
        send_to_char("You poke the ground with a stick but find no water that way.\n\r",ch);
        return;
        }
        if (ch->mana < 15)
        {
        send_to_char("You don't have the mana.\n\r",ch);
        return;
        }
        if (ch->in_room->sector_type == SECT_WATER_SWIM
        || ch->in_room->sector_type == SECT_WATER_NOSWIM)
        {
        send_to_char("Water water all around but not a drop to drink..\n\r",ch);
        return;
        }
        if (number_percent() > chance)
        {
        act("$n pokes the ground with a stick then scratches $s head.",ch,0,0,TO_ROOM);
        send_to_char("You poke about on the ground but fail to find any water.\n\r",ch);
        check_improve(ch,gsn_find_water,FALSE,1);
        ch->mana -= 7;
        WAIT_STATE(ch,18);
        return;
        }
        act("$n pokes at the ground and digs up a spring of natural water!",ch,0,0,TO_ROOM);
        send_to_char("You poke about for a bit and eventually dig up a spring of water.\n\r",ch);
        WAIT_STATE(ch,18);
        ch->mana -= 15;
        spring = create_object(get_obj_index(OBJ_VNUM_SPRING),0);
        check_improve(ch,gsn_find_water,TRUE,1);
        obj_to_room(spring,ch->in_room);
        return;
}

void do_track(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *victim;
        int chance, dir;
        char arg[MAX_INPUT_LENGTH];
        char buf[MAX_STRING_LENGTH];

        dir = 0;
	chance = get_skill(ch,gsn_track);

        if (chance == 0
        || ch->level < skill_table[gsn_track].skill_level[ch->class])
        {
        send_to_char("You peer intently at the ground but discern nothing.\n\r",ch);
        return;
        }
        one_argument(argument,arg);
        victim = get_char_world(ch,arg);
        chance *= 2;
        chance /= 3;
        if (ch->in_room->sector_type == SECT_INSIDE)
                chance -= 40;
        else if (ch->in_room->sector_type == SECT_CITY)
                chance -= 35;
        else if (ch->in_room->sector_type >= SECT_WATER_SWIM
        && ch->in_room->sector_type < SECT_DESERT)
                chance = 0;
        else if (ch->in_room->sector_type == SECT_FOREST)
                chance += 25;

        if (victim != NULL && !IS_NPC(victim))
                dir = victim->pcdata->track_dir[ch->in_room->vnum];

        if (number_percent() > chance
        || victim == NULL
        || !IS_NPC(victim)
        || dir == 0)
        {
        act("$n peers intently at the ground but doesn't seem to find anything.",ch,0,0,TO_ROOM);
        send_to_char("You peer intently at the ground but uncover nothing new.\n\r",ch);
        check_improve(ch,gsn_track,FALSE,2);
        WAIT_STATE(ch,18);
        return;
        }
        act("$n peers at the ground and seems more enlightened.",ch,0,0,TO_ROOM);

sprintf(buf,"You peer at the surroundings and find that %s left %s last time they were here.\n\r",
victim->name,
(dir == 1) ? "north" : (dir == 2) ? "east" : (dir == 3) ? "south" :
(dir == 4) ? "west" : (dir == 5) ? "up" : "down");
        send_to_char(buf,ch);
        WAIT_STATE(ch,18);
        check_improve(ch,gsn_track,TRUE,2);
        return;
}

void do_shield_cleave(CHAR_DATA *ch,char *argument)
{
	char buf[MAX_STRING_LENGTH];
        char arg[MAX_INPUT_LENGTH];
        int chance;
        OBJ_DATA *weapon;
        OBJ_DATA *shield;
        bool using_primary = TRUE;
  	CHAR_DATA *victim;

        one_argument(argument,arg);
        if (arg[0] == '\0')
                victim = ch->fighting;
        else
                victim = get_char_room(ch,arg);

        chance = get_skill(ch,gsn_shield_cleave);
        if (chance == 0
        || ch->level < skill_table[gsn_shield_cleave].skill_level[ch->class])
        {
        send_to_char("You don't know the methods to cleave a shield in two.\n\r",ch);
        return;
        }
        weapon = get_eq_char(ch,WEAR_WIELD);
        if (weapon == NULL)
        {
        weapon = get_eq_char(ch,WEAR_DUAL_WIELD);
        using_primary = FALSE;
        }
        if (weapon == NULL
        || (weapon->value[0] != WEAPON_SWORD
        && weapon->value[0] != WEAPON_AXE) )
        {
        send_to_char("You must be wielding an axe or sword to shield cleave.\n\r",ch);
        return;
        }
        if (victim == NULL)
        {
        send_to_char("But they aren't here.\n\r",ch);
        return;
        }
    if (victim == ch)
    {
send_to_char("That isn't possible.\n\r",ch);
return;
}
        if ((shield = get_eq_char(victim,WEAR_SHIELD)) == NULL)
        {
        send_to_char("But they aren't using a shield.\n\r",ch);
        return;
        }

        chance *= 9;
        chance /= 10;
        chance += (ch->level - victim->level)*3;
        chance -= shield->level;
	if(IS_AFFECTED(ch,AFF_BLIND))
		chance -= 50;
        if (!using_primary)
                chance -= 15;
    if (!IS_NPC(victim) && ch->fighting != victim)
    {
sprintf(buf,"Help! %s is trying to cleave my shield!",PERS(ch,victim));
	do_myell(victim,buf);
    }

	if (is_affected(victim,gsn_shield_of_purity))
	{
		act("$n attempts to strike a mighty blow to $N's shield!",ch,0,victim,TO_NOTVICT);
       		act("$n attempts to strike a mighty blow to your shield!",ch,0,victim,TO_VICT);
        	act("You attempt to strike a mighty blow to $N's shield!",ch,0,victim,TO_CHAR);
		act("$N skillfully deflects your shield-cleave attempt and counters with a powerful strike!",ch,0,victim,TO_CHAR);
		act("You gracefully deflect $n's shield-cleave attempt and counter with a powerful strike!",ch,0,victim,TO_VICT);
		act("$N skillfully deflects $n's shield-cleave attempt and counters with a powerful strike!",ch,0,victim,TO_NOTVICT);
		one_hit_new(victim,ch,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,225,"strike of purity");
		check_improve(ch,gsn_shield_cleave,FALSE,1);
		WAIT_STATE(ch,12);
		return;
	}

	if (is_affected(victim,gsn_shield_of_justice))
	{
		act("$n attempts to strike a mighty blow to $N's shield!",ch,0,victim,TO_NOTVICT);
       		act("$n attempts to strike a mighty blow to your shield!",ch,0,victim,TO_VICT);
        	act("You attempt to strike a mighty blow to $N's shield!",ch,0,victim,TO_CHAR);
		act("$N meets your shield-cleave attempt with incredible force using $S divine shield, reflecting your attack!",ch,0,victim,TO_CHAR);
		act("$N meets $n's shield-cleave attempt with incredible force using $S divine shield, reflecting $S attack!",ch,0,victim,TO_NOTVICT);
		act("You meet $n's shield-cleave with incredible force using your divine shield, reflecting $S attack!",ch,0,victim,TO_VICT);
one_hit_new(victim,ch,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,150,"reflected attack");		
		check_improve(ch,gsn_shield_cleave,FALSE,1);
		WAIT_STATE(ch,12);
		return;
	}

	if (is_affected(victim,gsn_shield_of_redemption))
	{
		act("$N positions $Mself to minimize the effectiveness of your shield-cleave.",ch,0,victim,TO_CHAR);
		act("$N positions $Mself to minimize the effectiveness of $n's shield-cleave.",ch,0,victim,TO_NOTVICT);
		act("You position yourself to minimize the effectiveness of $n's shield-cleave.",ch,0,victim,TO_VICT);
		chance -= 25;
	}

	if (is_affected(victim,gsn_shield_of_truth) && victim->pcdata->learned[skill_lookup("shieldbash")] > 75)
	{
		act("Striking preemptively, $N catches you offguard during your shield-cleave attempt and drives at you with $S shield!",ch,0,victim,TO_CHAR);
		act("$N's slams into you with his shield and sends you flying!",ch,0,victim,TO_CHAR);
		act("Striking preemptively, $N catches $n offguard during $s shield-cleave attempt and drives at $n with $S shield!",ch,0,victim,TO_NOTVICT);
		act("$N's slams into $n with his shield and sends $M flying!",ch,0,victim,TO_NOTVICT);
		act("Striking preemptively, you catch $n offguard during $s shield-cleave attempt and drive at $M with your shield!",ch,0,victim,TO_VICT);
		act("You slam into $n with your shield and send $M flying!",ch,0,victim,TO_VICT);
		WAIT_STATE(victim,skill_table[gsn_shieldbash].beats+PULSE_VIOLENCE);
		ch->position = POS_RESTING;
		check_improve(victim,gsn_shieldbash,TRUE,1);
		act("Your god senses the proximity of $n through your shield!",ch,0,victim,TO_VICT);
		int sapHealth = GET_DAMROLL(victim) * .75;
		act("The shield's energy reaches out and saps $n's health, using it to fuel your own!",ch,0,victim,TO_VICT);
		act("The energy surrounding $N's divine shield reaches out and saps your health!",ch,0,victim,TO_CHAR);
		act("The energy surrounding $N's divine shield reaches out and saps $n's health!",ch,0,victim,TO_NOTVICT);		
		damage_new(victim,ch,sapHealth,TYPE_UNDEFINED,DAM_HOLY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,100,"strike of truth");
		act("$N absorbs your lost health!",ch,0,victim,TO_CHAR);
		victim->hit += sapHealth;
		if (victim->hit > victim->max_hit)
			victim->hit = victim->max_hit;
		check_improve(ch,gsn_shield_cleave,FALSE,1);
		return;
	}

        if (number_percent() > chance)
        {
        act("$n makes a mighty blow at $N's shield but fails to cleave it.",ch,0,victim,TO_NOTVICT);
        act("$n lands a mighty blow to your shield but fails to cleave it.",ch,0,victim,TO_VICT);
        act("You strike a mighty blow to $N's shield but fail to cleave it.",ch,0,victim,TO_CHAR);
        check_improve(ch,gsn_shield_cleave,FALSE,1);
        WAIT_STATE(ch,12);

multi_hit(victim,ch,-1);
        return;
        }
        act("$n's mighty blow cleaves $N's shield in half!",ch,0,victim,TO_NOTVICT);
        act("Your mighty blow cleaves $N's shield in half!",ch,0,victim,TO_CHAR);
        act("$n strikes your shield with powerful force, cleaving it in two!",ch,0,victim,TO_VICT);
	if (is_affected(victim,gsn_shield_of_redemption))
	{
		act("$N's divine shield shatters, consuming you in a torrent of holy energy!",ch,0,victim,TO_CHAR);
		act("Your divine shield shatters, consuming $n in a torrent of holy energy!",ch,0,victim,TO_VICT);
		act("$N's divine shield shatters, consuming $n in a torrent of holy energy!",ch,0,victim,TO_NOTVICT);damage_new(victim,ch,victim->level*2 + number_range(1,20),TYPE_UNDEFINED,DAM_HOLY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"holy fire"); 
damage_new(victim,ch,victim->level*2 + number_range(1,20),TYPE_UNDEFINED,DAM_HOLY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"holy fire"); 
damage_new(victim,ch,victim->level*2 + number_range(1,20),TYPE_UNDEFINED,DAM_HOLY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"holy fire"); 
damage_new(victim,ch,victim->level*2 + number_range(1,20),TYPE_UNDEFINED,DAM_HOLY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"holy fire"); 
damage_new(victim,ch,victim->level*2 + number_range(1,20),TYPE_UNDEFINED,DAM_HOLY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"holy fire"); 
	}
        extract_obj(shield);
        WAIT_STATE(ch,12);
	check_improve(ch,gsn_shield_cleave,TRUE,1);
    multi_hit(victim,ch,-1);

        return;
}

/* more race skills...Ceran */

void do_breath_fire(CHAR_DATA *ch,char *argument)
{
    AFFECT_DATA af;
    CHAR_DATA *victim;
    OBJ_DATA *obj_lose;
    OBJ_DATA *obj_next;
    char arg[MAX_INPUT_LENGTH];
    int dam, chance;
    int hpch, level;

    level = ch->drain_level + ch->level;
    chance = get_skill(ch,gsn_breath_fire);
    if (chance == 0
    || ch->level < skill_table[gsn_breath_fire].skill_level[ch->class])
    {
    send_to_char("You aren't able to breath fire.\n\r",ch);
    return;
    }

    if (is_affected(ch,gsn_breath_fire))
    {
    send_to_char("You haven't yet regained the heat in your body to create more flames.\n\r",ch);
    return;
    }

    one_argument(argument,arg);
    if (arg[0] == '\0')
        victim = ch->fighting;
    else
        victim = get_char_room(ch,arg);

    if (victim == NULL)
    {
    send_to_char("They aren't here.\n\r",ch);
    return;
    }
    if (ch->mana < 20)
    {
    send_to_char("You don't have the mana.\n\r",ch);
    return;
    }
    if (number_percent() > chance)
    {
    send_to_char("You try to breath forth fire but only smokes comes out.\n\r",ch);
    act("$n opens $s mouth but only a thick plume of black smoke comes forth.",ch,0,0,TO_ROOM);
    ch->mana -= 10;
    check_improve(ch,gsn_breath_fire,FALSE,1);
    return;
    }
	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype = AFT_SKILL;
    af.type = gsn_breath_fire;
    af.location = 0;
    af.modifier = 0;
    af.duration = 4;
	af.affect_list_msg = str_dup("unable to breath fire");
    af.bitvector = 0;
    af.level = ch->drain_level + ch->level;
    affect_to_char(ch,&af);

    act("$n breaths forth a cone of searing flames!",ch,0,0,TO_ROOM);
    send_to_char("You breath forth a cone of searing flames!\n\r",ch);

    if ( number_percent( ) < level && !saves_spell( level - 2, victim,DAM_FIRE ) )
    {
	for ( obj_lose = victim->carrying; obj_lose != NULL;
	obj_lose = obj_next )
	{
	    char *msg;

	    obj_next = obj_lose->next_content;
	    if ( number_bits( 2 ) != 0 )
		continue;

	    switch ( obj_lose->item_type )
	    {
	    default:             continue;
	    case ITEM_CONTAINER: msg = "$p ignites and burns!";   break;
	    case ITEM_POTION:    msg = "$p bubbles and boils!";   break;
	    case ITEM_SCROLL:    msg = "$p crackles and burns!";  break;
	    case ITEM_STAFF:     msg = "$p smokes and chars!";    break;
	    case ITEM_WAND:      msg = "$p sparks and sputters!"; break;
	    case ITEM_FOOD:      msg = "$p blackens and crisps!"; break;
	    case ITEM_PILL:      msg = "$p melts and drips!";     break;
	    }

	    act( msg, victim, obj_lose, NULL, TO_CHAR );
	    extract_obj( obj_lose );
	}
    }

    hpch = UMAX( 10, ch->hit );
    dam  = number_range( (hpch/7) + 1, hpch/3 );
    if ( saves_spell( level - 2, victim, DAM_FIRE ) )
	dam /= 2;
    damage_old( ch, victim, dam, gsn_breath_fire, DAM_FIRE,TRUE);
    check_improve(ch,gsn_breath_fire,TRUE,1);
    return;
}


void do_rear_kick(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *victim;
    int dam;
/*
    AFFECT_DATA af;
*/
    if (str_cmp(race_table[ch->race].name,"centaur"))
    {
    send_to_char("You don't have a centaur's rear legs to kick like that.\n\r",ch);
    return;
    }

    if ( !IS_NPC(ch)
    &&   ch->level < skill_table[gsn_rear_kick].skill_level[ch->class] )
    {
	send_to_char("You do not have the ability to rear kick yet.\n\r",ch );
	return;
    }

    if (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_KICK))
	return;

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You are not fighting anyone.\n\r", ch );
	return;
    }

    WAIT_STATE( ch, 24 );
    act("$n whips about and kicks out with $s hind legs!",ch,0,0,TO_ROOM);
    act("You whip about and kick out with your hind legs!",ch,0,0,TO_CHAR);

    if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_rear_kick] )
    {
        dam = number_range(ch->level/2,ch->level);
        dam += get_curr_stat(ch,STAT_STR);
        dam += number_range(1,ch->drain_level + ch->level);
        dam += number_range((ch->drain_level + ch->level)/4, (ch->drain_level + ch->level)/2);

        if (dam < 1)
                dam = 1;
        if (dam > 140)
                dam = 140;

        damage_old( ch, victim, dam, gsn_rear_kick,DAM_BASH, TRUE );
        check_improve(ch,gsn_rear_kick,TRUE,1);
    }
    else
    {
        damage_old( ch, victim, 0, gsn_rear_kick,DAM_BASH,TRUE );
        check_improve(ch,gsn_rear_kick,FALSE,1);
    }

    return;
}

void do_strap(CHAR_DATA *victim,OBJ_DATA *strap) {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	if (strap->pIndexData->vnum == 5750) {
	act("\x01B[1;31m$n's $p explodes in a blinding flash of light!\x01B[0;37m",victim,strap,0,TO_ROOM);
	act("\x01B[1;31mYour $p explodes in a blinding flash of light!\x01B[0;37m",victim,strap,0,TO_CHAR);
	for (vch = victim->in_room->people; vch != NULL; vch = vch_next)
	{
	vch_next = vch->next_in_room;
	if (vch == victim) {
		continue;
	}
	if (IS_NPC(vch)) {
		continue;
	}
	if (vch->level > LEVEL_HERO) {
		continue;
	}
	act("$n detonates in an eruption of blood and fluids!",vch,strap,0,TO_ROOM);
	act("You detonate in an eruption of blood and fluids!",vch,strap,0,TO_CHAR);
	act( "$n is DEAD!!", vch, 0, 0, TO_ROOM );
	send_to_char( "You have been KILLED!!\n\r\n\r", vch );
	raw_kill(victim,vch);
	}
	unequip_char(victim,strap);
	obj_from_char(strap);
	extract_obj(strap);
	}
	if (strap->pIndexData->vnum == 5751) {
	unequip_char(victim,strap);
	obj_from_char(strap);
	extract_obj(strap);
	}
	return;
}

/* dying of old age */
void age_death(CHAR_DATA *ch)
{
    ROOM_INDEX_DATA *pRoomIndex;
    char *cname;
    AFFECT_DATA af;


    if (IS_NPC(ch))
        return;


	pRoomIndex = get_room_index(1212);
 
    if (pRoomIndex == NULL)
        return;

/* For now default to the players recall, or midgaard */
        pRoomIndex = get_room_index(hometown_table[ch->hometown].recall);
	if (pRoomIndex == NULL)
		pRoomIndex = get_room_index(ROOM_VNUM_TEMPLE);

    stop_fighting(ch,TRUE);
    make_corpse(ch,ch);
    char_from_room(ch);
    char_to_room(ch,pRoomIndex);
    do_look(ch,"auto");
    send_to_char("You have died and become a permanent ghost, awaiting your final departure.\n\r",ch);
    cname   = str_dup(ch->original_name);
    if( ((ch->played + (current_time - ch->logon)) / 3600) >= 15)
                delete_char(cname,TRUE);        // >= 15 hours. Make name unusable.
        else
                delete_char(cname,FALSE);
    init_affect(&af);
    af.aftype = AFT_INVIS;
    af.type   = skill_lookup("age death");
    af.duration = -1;
    af.strippable = FALSE;
    affect_to_char(ch,&af);
    return;
}



void do_forage(CHAR_DATA *ch,char *argument)
{
        OBJ_DATA *berry_1;
        OBJ_DATA *berry_2;
        int chance, found;

    if ( (chance = get_skill(ch,gsn_forage)) == 0
    || (ch->level < skill_table[gsn_forage].skill_level[ch->class]) )
    {
        send_to_char("You aren't able to decide on which plants are edible.\n\r",ch);
	return;
    }
    if (ch->in_room->sector_type != SECT_FOREST)
    {
    send_to_char("You aren't in a suitable forest region where you can apply your plant lore.\n\r",ch);
    return;
    }

    if (chance > 90)
        chance = 90;

    if (number_percent() > chance)
    {
    act("$n messes about in the undergrowth but comes up looking perplexed.",ch,0,0,TO_ROOM);
    send_to_char("You search around but find nothing you can recognise as edible.\n\r",ch);
    check_improve(ch,gsn_forage,FALSE,2);
    WAIT_STATE(ch,12);
    return;
    }

    act("$n messes about in the nearby bushes and comes out with some berries.",ch,0,0,TO_ROOM);
    send_to_char("You search around and find some edible berries in the bushes.\n\r",ch);
    check_improve(ch,gsn_forage,TRUE,2);

    found = number_range(1,3);

    berry_1 = create_object(get_obj_index(OBJ_VNUM_BERRY),1);
    if (berry_1 == NULL)
        return;
    obj_to_char(berry_1,ch);

    if (found >= 2)
    {
        berry_2 = create_object(get_obj_index(OBJ_VNUM_BERRY),1);
        obj_to_char(berry_2,ch);
    }
/*
    if (found >= 3)
    {
        berry_3 = create_object(get_obj_index(OBJ_VNUM_BERRY),1);
        obj_to_char(berry_3,ch);
    }
    if (found >= 4)
    {
        berry_4 = create_object(get_obj_index(OBJ_VNUM_BERRY),1);
        obj_to_char(berry_4,ch);
    }
*/

    return;
}

void do_defend(CHAR_DATA *ch,char *argument)
{
    CHAR_DATA *victim;
    char arg[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];

    one_argument(argument,arg);
        if ( (get_skill(ch,gsn_defend) == 0)
        || (ch->level < skill_table[gsn_defend].skill_level[ch->class]) )
    {
        send_to_char("You aren't able to defend other people.\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
    if (ch->defending == NULL)
    sprintf(buf,"You aren't defending anyone right now.\n\r");
    else
    sprintf(buf,"You are defending %s.\n\r",ch->defending->name);
    send_to_char(buf,ch);
    return;
    }
    victim = get_char_room(ch,arg);
    if (victim == NULL)
    {
    send_to_char("They aren't here.\n\r",ch);
    return;
    }
    if (victim == ch)
    {
    send_to_char("You no longer defend anyone.\n\r",ch);
    ch->defending = NULL;
    return;
    }
    if (IS_NPC(victim))
    {
    send_to_char("They don't need to be defended.\n\r",ch);
    return;
    }
    if (ch->defending != NULL)
    {
        act("You stop defending $N.",ch,0,ch->defending,TO_CHAR);
        act("$n stops defending you.",ch,0,ch->defending,TO_VICT);
    }
    sprintf(buf,"You start defending %s.",victim->name);
	act(buf,ch,0,victim,TO_CHAR);

    ch->defending = victim;
    act("$n is now defending you.",ch,0,victim,TO_VICT);
    return;
}

/* New flee...this allows 'flee <direction>' for the thief skill escape.
-Ceran
*/
void do_flee( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *was_in;
    ROOM_INDEX_DATA *now_in;
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    int attempt, chance, dir;

	if ( ( victim = ch->fighting ) == NULL )
    {
        if ( ch->position == POS_FIGHTING )
            ch->position = POS_STANDING;
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }

    if (IS_ROOM_AFFECTED(ch->in_room,AFF_ROOM_WALL_THORNS))
    {
	if (check_wall_thorns(ch))
		return;
    }

    if (is_affected(ch,gsn_aristaeia))
    {
	send_to_char("You are too filled with righteous wrath to flee!\n\r",ch);
	check_parting_blow(ch,victim);
	return;
    }

    if (is_affected(ch,gsn_shadowstrike) && number_percent()<20)
    {
		act("$n tries to flee but the shadowy arms stop his movements.",ch,0,0,TO_ROOM);
		act("You try to flee but the shadow arms block you.",ch,0,0,TO_CHAR);
		WAIT_STATE(ch,2*PULSE_VIOLENCE);
		return;
	}

    if (is_affected(ch,gsn_shackles) && number_percent()<20)
    {
		act("$n tries to flee but trips over his shackles.",ch,0,0,TO_ROOM);
		act("You try to flee but trip over your shackles.",ch,0,0,TO_CHAR);
		WAIT_STATE(ch,PULSE_VIOLENCE);
		check_cheap_shot(ch->fighting,ch);
		return;
    }
    if (is_affected(ch,gsn_entwine))
    {
        act("$n tries to flee with $s legs entwined, but trips.",ch,NULL,NULL,TO_ROOM);
        act("You try to flee with your legs entwined, but trip.",ch,NULL,NULL,TO_CHAR);
        check_downstrike(victim,ch);
        check_cheap_shot(victim,ch);
          WAIT_STATE(ch,PULSE_VIOLENCE);
          return;
    }
    if (is_affected(ch,gsn_bloodthirst) && number_percent()<70 )
    {
      send_to_char("You gasp for breath as you attempt to snap out of your insanity!\n\r",ch);
      act("$n gasps for breath, attempting to snap out of $s insanity!",ch,NULL,NULL,TO_ROOM);
      WAIT_STATE(ch,PULSE_VIOLENCE);
    }

    if (is_affected(ch,gsn_bind) )
    {
	act("$n attempts to flee but his bindings cause $m to trip up and fall!",ch,0,0,TO_ROOM);
	act("You attempt to flee but your bindings cause you to trip up and fall!",ch,0,0,TO_CHAR);
	WAIT_STATE(ch,PULSE_VIOLENCE);
	check_cheap_shot(ch->fighting,ch);
	check_parting_blow(ch,victim);
	return;
    }

	if (check_cutoff(ch,victim)) {
		return;
	}

    one_argument(argument,arg);
    chance = get_skill(ch,gsn_escape);
    if (chance > 90) chance=90;

    dir = 10;

    if (arg[0] == '\0')             dir=10;
    else if (!str_cmp(arg,"north")) dir=0;
    else if (!str_cmp(arg,"east"))  dir=1;
    else if (!str_cmp(arg,"south")) dir=2;
    else if (!str_cmp(arg,"west"))  dir=3;
    else if (!str_cmp(arg,"up"))    dir=4;
    else if (!str_cmp(arg,"down"))  dir=5;
    else                            dir=10;

    was_in = ch->in_room;

    if (was_in==NULL)
    {
      bug("In do_flee: ch->in_room is NULL.",0);
      return;
    }

    if (chance>0 && number_percent()>chance && dir!=10)
    {
      send_to_char("Trying to escape...\n\r",ch);
    }

    for ( attempt = 0; attempt < 6; attempt++ )
    {
        EXIT_DATA *pexit;
        int door;

        if (dir>5 || dir<0) door=number_door()%6;
        else door = dir;

        pexit = was_in->exit[door];

        if ( pexit == 0) continue;

        if (pexit->u1.to_room == NULL
            ||   ( IS_SET(pexit->exit_info, EX_CLOSED)
	        && (!IS_AFFECTED(ch,AFF_PASS_DOOR) ))
            ||   number_range(0,ch->daze) != 0
            || ( IS_NPC(ch)
            &&   IS_SET(pexit->u1.to_room->room_flags, ROOM_NO_MOB) ) )
          continue;

	
	if (attempt == 0)
		check_parting_blow(ch,victim);

        move_char( ch, door, FALSE );

        if ( ( now_in = ch->in_room ) == was_in )
            continue;

	stop_fighting(ch,TRUE);
	ch->last_fought = NULL;
        ch->in_room = was_in;
	if (is_affected(ch, gsn_owaza))
	{
		send_to_char("Your attempt at owaza fails as you flee.\n\r",ch);
		affect_strip(ch, gsn_owaza);
	}
        act( "$n has fled!", ch, NULL, NULL, TO_ROOM );

        ch->in_room = now_in;

        if ( !IS_NPC(ch) )
        {
            send_to_char( "You flee from combat!\n\r", ch );

	if (dir != 10 && get_skill(ch,gsn_escape) > 2)
	check_improve(ch,gsn_escape,2,TRUE);
        }

        return;
    }

    send_to_char( "PANIC! You couldn't escape!\n\r", ch );
    return;
}

void do_pugil(CHAR_DATA *ch,char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int chance;
    int dam;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_pugil)) == 0
    ||  IS_NPC(ch)
    ||  (!IS_NPC(ch) && ch->level < skill_table[gsn_pugil].skill_level[ch->class]) )
    {
        send_to_char("You're not trained in the art of pugiling.\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	send_to_char("But you aren't fighting anyone.\n\r",ch);
	return;
	}
    }
    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char("They aren't here.\n\r",ch);
	return;
    }

    if (ch->fighting == NULL)
	{
        send_to_char("You can't pugil someone like that.\n\r",ch);
	return;
	}

    if (victim == ch)
    {
        send_to_char("That would be a bit stupid.\n\r", ch);
	return;
    }

   obj = get_eq_char(ch,WEAR_WIELD);
   if (obj == NULL || obj->value[0] != WEAPON_STAFF)
    {
    send_to_char("You must be wielding a staff to pugil.\n\r",ch);
    return;
    }

    chance += (ch->level - victim->level);
    chance = URANGE(5, chance, 90);

    WAIT_STATE(ch,skill_table[gsn_pugil].beats);

    if (number_percent() < chance)
    {
        act("You smash $N with a bone crushing pugil!",ch,NULL,victim,TO_CHAR);
        act("$n smashes you with a bone crushing pugil!",ch,NULL,victim,TO_VICT);
        act("$n pugils $N with a bone crushing pugil!",ch,NULL,victim,TO_NOTVICT);
        check_improve(ch,gsn_pugil,TRUE,1);
	dam = dice(obj->value[1],obj->value[2]);
        dam += (get_skill(ch,gsn_enhanced_damage) * dam/100);
        if (ch->level <= 20)
            {
            dam *= number_range(10,13);
            dam /= 10;
            }
        else if (ch->level <= 25)
            {
            dam *= number_range(11,14);
            dam /= 10;
            }
        else if (ch->level <= 30)
            {
            dam *= number_range(12,15);
            dam /= 10;
            }
        else if (ch->level <= 35)
            {
            dam *= number_range(12,17);
            dam /= 10;
            }
        else if (ch->level <= 40)
            {
            dam *= number_range(13,18);
            dam /= 10;
            }
        else
            {
            dam *= number_range(14,20);
            dam /= 10;
            }

        damage_old(ch,victim,dam,gsn_pugil, attack_table[obj->value[3]].damage, TRUE);
    }
    else
    {
        check_improve(ch,gsn_pugil,FALSE,1);

        damage_old(ch,victim,0,gsn_pugil,DAM_NONE,TRUE);
    }

    return;
}


void do_protection_heat_cold(CHAR_DATA *ch,char *argument)
{
    AFFECT_DATA af;
    int chance;
    chance = get_skill(ch,gsn_protection_heat_cold);

    if (chance == 0
	|| ch->level <skill_table[gsn_protection_heat_cold].skill_level[ch->class])
    {
send_to_char("You don't know how to protect yourself from the elements.\n\r",ch);
	return;
    }

    if (is_affected(ch,gsn_protection_heat_cold))
    {
    send_to_char("You are already protected from the elements.\n\r",ch);
    return;
    }

    if (number_percent() > chance)
    {
	send_to_char("You try to protect yourself from the elements but fail.\n\r",ch);
	check_improve(ch,gsn_protection_heat_cold,FALSE,1);
	return;
    }

	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype = AFT_SKILL;
    af.type = gsn_protection_heat_cold;
    af.level = ch->level;
    af.modifier = 0;
    af.duration = ch->level;
    af.bitvector = 0;
    af.location = 0;
	af.affect_list_msg = str_dup("grants protection against heat and cold-elemental attacks");
    affect_to_char(ch,&af);
    send_to_char("You are protected from the elements.\n\r",ch);
	check_improve(ch,gsn_protection_heat_cold,TRUE,1);
    return;
}

void do_undead_drain(CHAR_DATA *ch,char *argument)
{
	CHAR_DATA *vch;
	int sn_drain;
	if (ch->level < 40) return;

	for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
	{
	if (vch->fighting == ch
	&& number_percent() > 70)
			break;
	}

if (vch == NULL)
		vch = ch->fighting;
	sn_drain = skill_lookup("undead drain");
	if (sn_drain == -1)
	return;

	(*skill_table[sn_drain].spell_fun) (sn_drain,ch->level,ch,vch,TAR_CHAR_OFFENSIVE);
	return;
}

char * check_evaluation_fight(CHAR_DATA *ch,CHAR_DATA *victim)
{
    int percent;
    char wound[100];
    char buf[MAX_STRING_LENGTH];

    if ((victim = ch->fighting) != NULL && can_see(ch,victim))
    {
        if (victim->max_hit > 0)
            percent = victim->hit * 100 / victim->max_hit;
        else
            percent = -1;

        if (percent >= 100)
            sprintf(wound,"is in perfect condition.");
        else if (percent >= 95)
            sprintf(wound,"has a few scratches.");
        else if (percent >= 90)
            sprintf(wound,"has a few bruises.");
        else if (percent >= 80)
             sprintf(wound,"has some small wounds.");
        else if (percent >= 70)
            sprintf(wound,"has some big nasty cuts.");
        else if (percent >= 60)
            sprintf(wound,"has quite a few wounds.");
        else if (percent >= 50)
            sprintf(wound,"is bleeding profusely.");
        else if (percent >= 40)
            sprintf(wound,"is gushing blood.");
        else if (percent >= 30)
            sprintf(wound,"is screaming in pain.");
        else if (percent >= 20)
            sprintf(wound,"is spasming in shock.");
        else if (percent >= 10)
            sprintf(wound,"is writhing on the ground.");
        else if (percent >= 1)
            sprintf(wound,"is convulsing near death.");
        else
            sprintf(wound,"is nearly dead.");
	}
	return str_dup(buf);
}

void do_iron_resolve(CHAR_DATA *ch,char *argument)
{
    AFFECT_DATA af;

    if (is_affected(ch,gsn_iron_resolve))
    {
        send_to_char("Your mind is already firmly set on your inner being.\n\r",ch);
        return;
    }
    if (ch->mana < 100)
    {
        send_to_char("You don't have the mental concentration right now.\n\r",ch);
        return;
    }

    if (number_percent() > get_skill(ch,gsn_iron_resolve))
    {
        send_to_char("You try to concentrate on your inner being but fail to maintain it.\n\r",ch);
        ch->mana -= 50;
        WAIT_STATE(ch,12);
        return;
    }

    send_to_char("You feel yourself come together as mind and body.\n\r",ch);

	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype = AFT_SKILL;
    af.type = gsn_iron_resolve;
    af.level = ch->level;
    af.location = 0;
    af.bitvector = 0;
    af.modifier = 0;
    af.duration = ch->level/3;
    ch->mana -= 100;
    WAIT_STATE(ch,12);
}

void do_quiet_movement(CHAR_DATA *ch, char *argument)
{
    AFFECT_DATA af;
    int chance;

    chance = get_skill(ch,gsn_quiet_movement);
    if (chance == 0
    || ch->level < skill_table[gsn_quiet_movement].skill_level[ch->class])
    {
        send_to_char("You don't know how to move with silent stealth through the wilderness.\n\r",ch);
        return;
    }

    if (is_affected(ch,gsn_quiet_movement))
    {
        send_to_char("You are already attempting to move silently through the wilderness.\n\r",ch);
        return;
    }
    if (ch->in_room->sector_type == SECT_FOREST)
    {
        send_to_char("You attempt to move quietly through the forest.\n\r",ch);
    }
    else if (ch->in_room->sector_type == SECT_MOUNTAIN)
    {
        send_to_char("You attempt to move quietly through the mountains.\n\r",ch);
    }
    else if (ch->in_room->sector_type == SECT_HILLS)
    {
        send_to_char("You attempt to move quietly through the hills.\n\r",ch);
    }
    else
    {
        send_to_char("You aren't in the right kind of wilderness to movee quietly.\n\r",ch);
        return;
    }

    if (number_percent() > chance)
    {
        check_improve(ch,gsn_quiet_movement,FALSE,1);
        return;
    }
    check_improve(ch,gsn_quiet_movement,TRUE,1);

	init_affect(&af);
    af.where = TO_AFFECTS;
    af.type = gsn_quiet_movement;
	af.aftype = AFT_SKILL;
    af.location = 0;
    af.modifier = 0;
    af.duration = ch->level;
    af.level = ch->level;
	char msg_buf[MSL];
	sprintf(msg_buf,"moving silently");
	af.affect_list_msg = str_dup(msg_buf);
    af.bitvector = 0;
    affect_to_char(ch,&af);
    return;
}

void do_executioners_grace(CHAR_DATA *ch,char *argument)
{
	AFFECT_DATA af;
	int chance = get_skill(ch,gsn_executioner);
	bool night=FALSE;

	if(chance==0)
	{
		send_to_char("You don't know how to move with the deadly grace of an executioner.\n\r",ch);
		return;
	}

	if(number_percent() > chance)
	{
		send_to_char("You fail to move with the deadly grace of an executioner.\n\r",ch);
		WAIT_STATE(ch,PULSE_VIOLENCE*4);
		return;
	}

	if(is_affected(ch,gsn_executioner))
	{
		send_to_char("You are already moving with the deadly grace of an executioner.\n\r",ch);
		WAIT_STATE(ch,PULSE_VIOLENCE*4);
		return;
	}

	send_to_char("You begin to flow with the deadly grace of an executioner.\n\r",ch);

	night = (weather_info.sunlight == SUN_DARK ? TRUE : FALSE);

	init_affect(&af);
	af.where 	= TO_AFFECTS;
	af.type 	= gsn_executioner;
	af.location 	= APPLY_DAMROLL;
	af.modifier 	= night ? 25 : 15;
	af.level 	= ch->level;
	af.duration 	= night ? ch->level/2 : ch->level/4;
	af.bitvector 	= AFF_BERSERK;
	af.aftype 	= AFT_SKILL;
	ch->mana-=300;
	affect_to_char(ch,&af);
	return;
}
void do_spike(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *victim;
        int chance, dam;
        char arg[MAX_INPUT_LENGTH];

        one_argument(argument,arg);
        if (IS_NPC(ch))
                return;
	if (ch->position == POS_FIGHTING)
	{
        send_to_char("You can't spike them in the heat of battle.\n\r",ch);
        return;
        }


	if ((get_skill(ch,gsn_spike) == 0)
	|| ch->level < skill_table[gsn_spike].skill_level[ch->class])
	{
	send_to_char("Huh?\n\r",ch);
	return;
	}

        if (arg[0] == '\0')
        {
        send_to_char("Attempt to spike which person?\n\r",ch);
        return;
        }
        if ( (victim = get_char_room(ch,arg)) == NULL)
        {
        send_to_char("They aren't here.\n\r",ch);
        return;
        }
        if (victim == ch)
        {
        send_to_char("You can't do that.\n\r",ch);
        return;
        }
        if (victim->fighting != NULL || victim->position == POS_FIGHTING)
        {
        send_to_char("They are moving around too much to get in close for the kill.\n\r",ch);
        return;
        }

        if (is_safe(ch,victim))
                return;

        chance = get_skill(ch,gsn_spike)/2;
        chance += ch->level;
        chance -= victim->level * 3/2;
        chance -= number_range(0,15);
        if (!can_see(victim,ch))
                chance += 10;
        if (victim->position == POS_FIGHTING)
                chance -= 25;
        else if (victim->position == POS_SLEEPING)
                chance += 10;
        else chance -= 10;

        chance /= 2;
        chance = URANGE(2,chance,90);

        act("$n strikes out at $N with deadly intensity.",ch,0,victim,TO_NOTVICT);
        act("You strike out at $N with deadly intensity.",ch,0,victim,TO_CHAR);
        act("$n strikes at you with deadly intensity.",ch,0,victim,TO_VICT);

        if (number_percent() < chance)
        {
        send_to_char("With agonising pain your skull is smashed by the blow!\n\r",victim);
        act("Your blow shatters $N's skull into bloody fragments!",ch,0,victim,TO_CHAR);
        act("$N's skull is shattered into bits of mangled flesh and bone by $n's strike!",ch,0,victim,TO_NOTVICT);
        raw_kill(ch,victim);
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
        check_improve(ch,gsn_spike,TRUE,2);
        return;
        }
        else
        {
        send_to_char("You feel a sharp pain searing your skull!\n\r",victim);
        act("Your deathstrike smashes $N's skull but fails to kill.",ch,0,victim,TO_CHAR);
        dam = ch->level * 2;
        dam += dice(ch->level, 4);
        damage_old(ch,victim,dam,gsn_spike,DAM_PIERCE,TRUE);
        WAIT_STATE(ch,PULSE_VIOLENCE);
	check_improve(ch,gsn_spike,FALSE,1);
	}

	return;
}


bool check_unholy_bless(CHAR_DATA *ch, CHAR_DATA *victim) {

	AFFECT_DATA af;
	AFFECT_DATA *paf;
	AFFECT_DATA *paf_next;
        int sn, hitmod = 0, manamod = 0, hrollmod = 0, drollmod = 0, vdrollmod = 0, i;

	if (IS_NPC(victim))
		return FALSE;

	if (!is_affected(ch,gsn_unholy_bless)) {
		return FALSE;
	}

	if (is_affected(victim,gsn_unholy_timer)) {
		return FALSE;
	}

	sn = skill_lookup("unholy bless");

	/* check if victim is AP and take his charges */
        if (is_affected(victim,gsn_unholy_bless) && victim != ch) {
		for ( paf = victim->affected; paf != NULL; paf = paf_next )
		{
			paf_next = paf->next;
			if ( paf->type == sn ) {
				switch (paf->location)
				{
				default: break;
				case APPLY_DAMROLL: vdrollmod = paf->modifier; break;
				}
			}
		}
		vdrollmod = (vdrollmod - 1) / 6;
		if (vdrollmod < 1) {
			vdrollmod = 0;
		}
	} else {
		vdrollmod = 0;
	}

	for (i=0; i < vdrollmod+1; i++) {
	for ( paf = ch->affected; paf != NULL; paf = paf_next )
	{
		paf_next = paf->next;
		if ( paf->type == sn ) {
			switch (paf->location)
			{
			default: break;
			case APPLY_HIT: hitmod = paf->modifier; affect_remove( ch, paf ); break;
			case APPLY_MANA: manamod = paf->modifier; affect_remove( ch, paf ); break;
			case APPLY_HITROLL: hrollmod = paf->modifier; affect_remove( ch, paf ); break;
			case APPLY_DAMROLL: drollmod = paf->modifier; affect_remove( ch, paf ); break;
			}
		}
	}

	hitmod += 10;
	manamod += 10;
	hrollmod += 3;
	drollmod += 3;

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = sn;
	af.level = ch->level;
	af.duration = -1;
	af.bitvector = 0;
	af.modifier = hitmod;
	af.location = APPLY_HIT;
	affect_to_char(ch,&af);
	af.modifier = manamod;
	af.location = APPLY_MANA;
	affect_to_char(ch,&af);
	af.modifier = hrollmod;
	af.location = APPLY_HITROLL;
	affect_to_char(ch,&af);
	af.modifier = drollmod;
	af.location = APPLY_DAMROLL;
	affect_to_char(ch,&af);
	act("\x01B[1;31m$n's body pulses as power drains from $N's corpse!\x01B[0;37m",ch,0,victim,TO_NOTVICT);
	act("\x01B[1;31m$n's body pulses as power drains from your corpse!\x01B[0;37m",ch,0,victim,TO_VICT);
	act("\x01B[1;31mYour body pulses as you drain power from $N's corpse!\x01B[0;37m",ch,0,victim,TO_CHAR);
	}
	return TRUE;
}

bool check_unholy_new(CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj)
{
	AFFECT_DATA *af,*af_next,paf;
	int hmod=0, hpmod=0, dmod=0, mmod=0;
	bool dual = FALSE;
	
	if (obj == get_eq_char(ch,WEAR_DUAL_WIELD)) dual = TRUE;

	if(obj == NULL)
		return FALSE;
	int sn = skill_lookup("unholy bless");
	if((af = affect_find_obj(obj,sn)) == NULL)
		return FALSE;
	if(IS_NPC(victim) || is_affected(victim,skill_lookup("unholy guard")))
		return FALSE;

	unequip_char(ch,obj);

	for (af = obj->affected; af != NULL; af = af_next )
       	 	{
                af_next = af->next;
                if ( af->type == sn ) {
                        switch (af->location)
                        {
                        default: break;
                        case APPLY_HIT: hpmod = af->modifier; affect_remove_obj( obj, af ); break;
                        case APPLY_MANA: mmod = af->modifier; affect_remove_obj( obj, af ); break;
                        case APPLY_HITROLL: hmod = af->modifier; affect_remove_obj( obj, af ); break;
                        case APPLY_DAMROLL: dmod = af->modifier; affect_remove_obj( obj, af ); break;
                        }
                }
		}

	hmod += 1;
	hpmod += 50;
	mmod += 50;
	dmod += 1;

	init_affect(&paf);
	paf.where = TO_OBJECT;
	paf.duration = -1;
	paf.level = ch->level;
	paf.type = sn;
	paf.owner_name = str_dup(ch->original_name);
	paf.location = APPLY_HIT;
	paf.modifier = hpmod;
	affect_to_obj(obj,&paf);
	paf.location = APPLY_MANA;
	paf.modifier = mmod;
	affect_to_obj(obj,&paf);
	paf.location = APPLY_HITROLL;
	paf.modifier = hmod;
	affect_to_obj(obj,&paf);
	paf.location = APPLY_DAMROLL;
	paf.modifier = dmod;
	affect_to_obj(obj,&paf);

	if (dual)
		equip_char(ch,obj,WEAR_DUAL_WIELD);
	else
		equip_char(ch,obj,WEAR_WIELD);
	act("{R$p writhes in your grip and you feel a new energy enter your body as you steal $N's soul.{x",ch,obj,victim,TO_CHAR);
	act("{R$p seems to twist and writhe in $n's grip as a chill runs down your spine.{x",ch,obj,0,TO_ROOM);
	return TRUE;
}

void do_drive(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim = ch->fighting;
	AFFECT_DATA af;
	char buf[MSL];
	char arg[MSL];
	int direction = -1;

	one_argument(argument,arg);
	if (get_skill(ch, skill_lookup("drive")) < 5)
		return send_to_char("You do not know how to drive others.\n\r",ch);
	if (victim == NULL)
		return send_to_char("But you aren't fighting anyone!\n\r",ch);
	if (arg[0] != '\0' && IS_NPC(victim))
		return send_to_char("You cannot seem to budge them.\n\r",ch);
	if (arg[0] == '\0')
		return send_to_char("Drive in which direction?\n\r",ch);

	if (!str_prefix(arg,"east") || !str_prefix(arg,"west") || !str_prefix(arg,"north") || !str_prefix(arg,"south") || !str_prefix(arg,"up") || !str_prefix(arg,"down"))
	{
		if (!str_prefix(arg,"east"))
			direction = DIR_EAST;
		else if (!str_prefix(arg,"west"))
			direction = DIR_WEST;
		else if (!str_prefix(arg,"north"))
			direction = DIR_NORTH;
		else if (!str_prefix(arg,"south"))
			direction = DIR_SOUTH;
		else if (!str_prefix(arg,"up"))
			direction = DIR_UP;
		else if (!str_prefix(arg,"down"))
			direction = DIR_DOWN;

		if (number_percent() < get_skill(ch, skill_lookup("drive")) * .75)
		{
			sprintf(buf,"Help! %s is driving me away!",ch->name);
			do_myell(victim,buf);
			if (direction != DIR_UP && direction != DIR_DOWN)
				sprintf(buf,"You charge at %s and drive %s to the %s!",IS_NPC(victim) ? victim->short_descr : victim->name, him_her[URANGE(0, victim->sex, 2)], dir_name[direction]);
			else
				sprintf(buf,"You charge at %s and drive %s %swards!",IS_NPC(victim) ? victim->short_descr : victim->name, him_her[URANGE(0, victim->sex, 2)], dir_name[direction]);
			act(buf,ch,0,victim,TO_CHAR);
			if (direction != DIR_UP && direction != DIR_DOWN)
				sprintf(buf,"%s charges at %s, driving %s to the %s!",IS_NPC(ch) ? ch->short_descr : ch->name,IS_NPC(victim) ? victim->short_descr : victim->name, him_her[URANGE(0, victim->sex, 2)], dir_name[direction]);
			else
				sprintf(buf,"%s charges at %s, driving %s %swards!",IS_NPC(ch) ? ch->short_descr : ch->name,IS_NPC(victim) ? victim->short_descr : victim->name, him_her[URANGE(0, victim->sex, 2)], dir_name[direction]);
			act(buf,ch,0,victim,TO_NOTVICT);
			if (direction != DIR_UP && direction != DIR_DOWN)
				sprintf(buf,"%s charges at you, driving you to the %s!",IS_NPC(ch) ? ch->short_descr : ch->name, dir_name[direction]);
			else
				sprintf(buf,"%s charges at you, driving you %swards!",IS_NPC(ch) ? ch->short_descr : ch->name,dir_name[direction]);
			act(buf,ch,0,victim,TO_VICT);	
			WAIT_STATE(ch,PULSE_VIOLENCE*2);
		}
		else
		{
			sprintf(buf,"Help! %s is trying to drive me away!",ch->name);
			do_myell(victim,buf);
			if (direction != DIR_UP && direction != DIR_DOWN)
				sprintf(buf,"You charge at %s, but fail to drive %s to the %s!",IS_NPC(victim) ? victim->short_descr : victim->name, him_her[URANGE(0, victim->sex, 2)], dir_name[direction]);
			else
				sprintf(buf,"You charge at %s, but fail to drive %s %swards!",IS_NPC(victim) ? victim->short_descr : victim->name, him_her[URANGE(0, victim->sex, 2)], dir_name[direction]);
			act(buf,ch,0,victim,TO_CHAR);
			if (direction != DIR_UP && direction != DIR_DOWN)
				sprintf(buf,"%s charges at %s, but fails to drive %s to the %s!",IS_NPC(ch) ? ch->short_descr : ch->name,IS_NPC(victim) ? victim->short_descr : victim->name, him_her[URANGE(0, victim->sex, 2)], dir_name[direction]);
			else
				sprintf(buf,"%s charges at %s, but fails to drive %s %swards!",IS_NPC(ch) ? ch->short_descr : ch->name,IS_NPC(victim) ? victim->short_descr : victim->name, him_her[URANGE(0, victim->sex, 2)], dir_name[direction]);
			act(buf,ch,0,victim,TO_NOTVICT);
			if (direction != DIR_UP && direction != DIR_DOWN)
				sprintf(buf,"%s charges at you, but fails to drive you to the %s!",IS_NPC(ch) ? ch->short_descr : ch->name, dir_name[direction]);
			else
				sprintf(buf,"%s charges at you, but fails to drive you %swards!",IS_NPC(ch) ? ch->short_descr : ch->name,dir_name[direction]);
			act(buf,ch,0,victim,TO_VICT);
			WAIT_STATE(ch,PULSE_VIOLENCE*2);
			return;
		}

	if (direction != -1)
		stop_fighting(ch,TRUE);
		init_affect(&af);
		af.aftype = 	AFT_INVIS;
		af.type =	skill_lookup("drive");
		affect_to_char(victim,&af);
		move_char(ch,direction,FALSE);
		move_char(victim,direction,FALSE);
		affect_remove(victim,&af);
		set_fighting(ch,victim);
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
		return;
	}

	else
		return send_to_char("That is not a valid direction.",ch);
}

	
void do_bind(CHAR_DATA *ch,char *argument)
{
	CHAR_DATA *victim;
        int chance;
	AFFECT_DATA af;
        
	char arg[MAX_INPUT_LENGTH];
        one_argument(argument,arg);
        
	if (IS_NPC(ch))
                return;
	
	if (get_skill(ch,gsn_bind) == 0)
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

        if (arg[0] == '\0')
        {
        	send_to_char("Attempt to bind who?\n\r",ch);
        	return;
        }

        if ( (victim = get_char_room(ch,arg)) == NULL)
        {
        	send_to_char("They aren't here.\n\r",ch);
        	return;
        }

        if (victim == ch)
        {
        	send_to_char("You can't do that.\n\r",ch);
        	return;
        }

        if (victim->position != POS_SLEEPING)
        {
        	send_to_char("They must be sleeping for you to bind them.\n\r",ch);
        	return;
        }


	if (is_affected(victim,gsn_bind))
	{
	  	send_to_char("They are already bound.\n\r",ch);
	  	return;
	}

	if (is_safe(victim, ch))
		return;

	chance = get_skill(ch,gsn_bind)-20;

	if(is_affected(victim,AFF_HASTE))
		chance-=30;

	if(is_affected(ch,AFF_HASTE))
		chance+=15;

	if(get_curr_stat(victim,STAT_DEX)>get_curr_stat(ch,STAT_DEX))
		chance-=20;

	if(number_percent() < chance)
	{
		init_affect(&af);
		af.aftype	= AFT_SKILL;
		af.where 	= TO_AFFECTS;
		af.type 	= gsn_bind;
		af.level 	= ch->level;
		af.duration 	= -1;
		af.bitvector 	= AFF_BLIND;
		af.modifier 	= -5;
		af.location 	= APPLY_DEX;
		affect_to_char(victim,&af);
		act("$n puts a sack over $N's head and ties a rope around his legs tightly.",ch,0,victim,TO_NOTVICT);
		send_to_char("You feel someone putting something over your head and legs.", victim);
		act("You put a sack over $N's head and tie a rope around his legs tightly.",ch,0,victim,TO_CHAR);
		WAIT_STATE(ch,PULSE_VIOLENCE);
		check_improve(ch,gsn_bind,TRUE,1);
	}
	else
	{
		act("$n tries to put a sack over $N's head but it rips.",ch,0,victim,TO_NOTVICT);
		send_to_char("You feel someone trying to put something over your head and legs.",victim);
		act("You try to put a sack over $N's head but it rips.",ch,0,victim,TO_CHAR);
		WAIT_STATE(ch,PULSE_VIOLENCE*3);
		check_improve(ch,gsn_bind,FALSE,3);
	}
	return;
}
void check_cheap_shot(CHAR_DATA *ch, CHAR_DATA *victim)
{
	int chance;
	int dam;
	chance=get_skill(ch,gsn_cheap_shot);
	if(chance<2)
		return;
	if(is_affected(victim,AFF_HASTE))
		chance-=30;
	if(is_affected(ch,AFF_HASTE))
		chance+=15;
	if(is_affected(victim,AFF_FLYING))
		chance-=20;
	chance*=.65;
	if(number_percent() < chance)
	{
		act("Seizing upon $N's moment of weakness, you brutally kick him while he's down!",ch,0,victim,TO_CHAR);
		act("Seizing upon your moment of weakness, $n brutally kicks you while you're down!",ch,0,victim,TO_VICT);
		act("Seizing upon $N's moment of weakness, $n brutally kicks him while he's down!",ch,0,victim,TO_NOTVICT);
		dam=40;
		if(number_percent() < 10)
		{
			act("$N grunts in pain as you land a particularly vicious kick!",ch,0,victim,TO_CHAR);
			act("You grunt in pain as $n lands a particularly vicious kick!",ch,0,victim,TO_VICT);
			act("$N grunts in pain as $n lands a particularly vicious kick!",ch,0,victim,TO_NOTVICT);
			dam=125;
			ADD_WAIT_STATE(victim,PULSE_VIOLENCE);
		}
		if (spike_toed_boots(ch))
			dam *= 1.5;
		damage_old(ch,victim,dam,gsn_cheap_shot,DAM_PIERCE,TRUE);
		check_improve(ch,gsn_cheap_shot,TRUE,1);
		ADD_WAIT_STATE(victim,PULSE_VIOLENCE);
		return;
	}
	else
	{
		send_to_char("You were unable to get a cheap shot in.\n\r",ch);
		check_improve(ch,gsn_cheap_shot,FALSE,1);
		return;
	}
}
void do_unbind(CHAR_DATA *ch,char *argument)
{
	if(!is_affected(ch,gsn_bind))
	{
		send_to_char("You are not bound.\n\r",ch);
		return;
	}
	if(number_percent()>30)
	{
		act("You break free of the bindings on your head and legs.",ch,0,0,TO_CHAR);
		act("$n breaks free of the bindings on $s head and legs.",ch,0,0,TO_ROOM);
		affect_strip( ch, gsn_bind );
		WAIT_STATE(ch,PULSE_VIOLENCE);
	}
	else
	{
		act("You fail to break free of the bindings on your head and legs.",ch,0,0,TO_CHAR);
		act("$n fails to break free of the bindings on $s head and legs.",ch,0,0,TO_ROOM);
		WAIT_STATE(ch,PULSE_VIOLENCE);
	}
}

void do_blitz( CHAR_DATA *ch, char *argument ) // Do not disable this! -Det.
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	int chance, imod;
	char buf[MAX_STRING_LENGTH];
	bool isDS;

	if (cabal_down(ch,CABAL_RAGER))
		return;

	one_argument(argument,arg);

	if ((chance = get_skill(ch,gsn_blitz)) == 0)
	{
		send_to_char("Blitzing? What's that?\n\r",ch);
		return;
	}

	if (IS_NPC(ch) && IS_AFFECTED(ch,AFF_CHARM))
	return;

	if (arg[0] == '\0')
	{
		victim = ch->fighting;
		if (victim == NULL)
		{
			send_to_char("But you aren't fighting anyone!\n\r",ch);
			return;
		}
	}
	else if ((victim = get_char_room(ch,arg)) == NULL)
	{
		send_to_char("They aren't here.\n\r",ch);
		return;
	}

	if (victim->position < POS_FIGHTING)
	{
		act("You'll have to let $M get back up first.",ch,NULL,victim,TO_CHAR);
		return;
	}

	if (victim == ch)
	{
		send_to_char("You try to blitz your brains out, but fail.\n\r",ch);
		return;
	}
	if ((victim->fighting!=ch) && (ch->fighting != NULL))
	{
		send_to_char("You must be directly fighting that person to blitz them.\n\r",ch);
		return;
	}
	if (is_safe(ch,victim))
	return;

	if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
	{
		act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
		return;
	}

	/* modify chance */
	chance += ch->carry_weight / 200;
	chance -= victim->carry_weight / 200;
	if (ch->size < victim->size)
	{
		chance += (ch->size - victim->size) * 5;
	} else {
		chance += (ch->size - victim->size) * 5;
	}
	if (is_affected(victim,gsn_protective_shield))
	{
		chance -= 5;
	}
	 if (is_affected(victim, skill_lookup("wraithform")))
	{
		chance -= 5;
	}
	if (is_affected(victim,gsn_fireshield))
	{
		chance -= 5;
	}
	chance += get_curr_stat(ch,STAT_STR) + 5;
	chance -= (get_curr_stat(victim,STAT_DEX));
	chance -= GET_AC(victim,AC_BASH) /20;
    	if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
	{
		chance += 10;
	}
    	if (is_affected(ch,skill_lookup("bloodthirst")))
	{
		chance += 10;
	}
    	if (is_affected(ch,skill_lookup("berserk")))
	{
		chance += 10;
	}
	if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
	{
		chance -= 5;
	}
	chance += (ch->level - victim->level);
	if (!IS_NPC(ch) && !IS_NPC(victim) && (victim->fighting == NULL || ch->fighting == NULL))
	{
		sprintf(buf,"Help! %s is blitzing me!",PERS(ch,victim));
		do_myell(victim,buf);
	}
	if (!IS_NPC(victim) && chance < get_skill(victim,gsn_dodge) )
	{
		chance -= 3 * (get_skill(victim,gsn_dodge) - chance);
	}

	imod = 25;

	isDS = FALSE;

        if (!IS_NPC(ch) )
                chance += 15;

	/* now the attack */
	if (number_percent() < chance - 20)
	{
		act("$n pummels you to the ground with excruciating force!", ch,NULL,victim,TO_VICT);
		act("You slam into $N, pummeling $M into the ground!",ch,NULL,victim,TO_CHAR);
		act("$n pummels $N into the ground with a powerful blitz.",ch,NULL,victim,TO_NOTVICT);
		damage_old(ch,victim,(3 * ch->size + chance),gsn_blitz,DAM_BASH,TRUE);
		check_improve(ch,gsn_blitz,TRUE,3);
		WAIT_STATE(victim,PULSE_VIOLENCE*1.5);
		WAIT_STATE(ch,2*PULSE_VIOLENCE);
		if (number_percent() > 90)
		{
			if (is_affected(victim,gsn_sanctuary) && (number_percent() < chance-imod))
			{
				if (check_dispel(ch->level-3,victim,skill_lookup("sanctuary")))
				{
					act("$n looks stunned as $s mind wanders from $s spells.",victim,0,0,TO_ROOM);
					act("Your head throbs with pain as you lose control over your spells.",victim,0,0,TO_CHAR);
					act("The white aura about $n vanishes!",victim,NULL,NULL,TO_ROOM);
					isDS = TRUE;
				}
			}
			if ((isDS == FALSE) && is_affected(victim,gsn_fireshield) && (number_percent() < chance-imod))
			{
				if (check_dispel(ch->level,victim,skill_lookup("fireshield")))
				{
					act("$n looks stunned as $s mind wanders from $s spells.",victim,0,0,TO_ROOM);
					act("Your head throbs with pain as you lose control over your spells.",victim,0,0,TO_CHAR);
					act("The fire around $n vanishes!",victim,NULL,NULL,TO_ROOM);
					isDS = TRUE;

				}
			}

			if ((isDS == FALSE) && is_affected(victim,gsn_stoneskin) && (number_percent() < chance-imod))
			{
				if (check_dispel(ch->level-2,victim,skill_lookup("stone skin")))
				{
					act("$n looks stunned as $s mind wanders from $s spells.",victim,0,0,TO_ROOM);
					act("Your head throbs with pain as you lose control over your spells.",victim,0,0,TO_CHAR);
					act("The skin of $n softens into flesh!",victim,NULL,NULL,TO_ROOM);
					isDS = TRUE;

				}
			}
			if ((isDS == FALSE) && is_affected(victim,gsn_wraithform) && (number_percent() < chance-imod))
			{
				if (check_dispel(ch->level-2,victim,skill_lookup("wraithform")))
				{
					act("$n looks stunned as $s mind wanders from $s spells.",victim,0,0,TO_ROOM);
					act("Your head throbs with pain as you lose control over your spells.",victim,0,0,TO_CHAR);
					act("The body of $n fades back into the Prime Material Plane!",victim,NULL,NULL,TO_ROOM);
					isDS = TRUE;

				}
			}
			if ((isDS == FALSE) && is_affected(victim,gsn_shroud) && (number_percent() < chance-imod))
			{
				if (check_dispel(ch->level,victim,skill_lookup("shroud")))
				{
					act("$n looks stunned as $s mind wanders from $s spells.",victim,0,0,TO_ROOM);
					act("Your head throbs with pain as you lose control over your spells.",victim,0,0,TO_CHAR);
					act("The black aura about $n vanishes!",victim,NULL,NULL,TO_ROOM);
					isDS = TRUE;

				}
			}
		}
	} else {
		damage_old(ch,victim,0,gsn_blitz,DAM_BASH,FALSE);
		act("You fall over and bite the dust!",ch,NULL,victim,TO_CHAR);
		act("$n falls flat on $s face, eating a mouthful of dirt.",ch,NULL,victim,TO_NOTVICT);
		act("You evade $n's blitz, and $e falls flat on $s face.",ch,NULL,victim,TO_VICT);
		check_cheap_shot(victim,ch);
		check_improve(ch,gsn_blitz,FALSE,3);
		ch->position = POS_RESTING;
		WAIT_STATE(ch,2*PULSE_VIOLENCE);
	}
	return;
}

void check_parting_blow( CHAR_DATA *ch, CHAR_DATA *victim )
{
	if (get_skill(victim,gsn_parting_blow) > 1)
     	{
		if (number_percent() < get_skill(victim,gsn_parting_blow))
         	{
			act("You get in one more shot as $N flees.",victim,0,ch,TO_CHAR);
			act("$n gets in one more shot as $N flees.",victim,0,ch,TO_NOTVICT);
			act("$n gets in one more shot as you flee.",victim,0,ch,TO_VICT);
			one_hit(victim,ch,gsn_parting_blow);
			check_improve(victim,gsn_parting_blow,TRUE,1);
		} else {
			check_improve(victim,gsn_parting_blow,TRUE,1);
		}
	}
	return;
}

bool check_ironhands( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    float chance, hit_ratio;
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char *attack;
	int ch_hitroll, victim_hitroll, str = get_curr_stat(victim,STAT_STR);

    if ( !IS_AWAKE(victim) )
        return FALSE;

    if (IS_NPC(victim))
        return FALSE;

    if ((chance = get_skill(victim,gsn_ironhands) / 3) == 0)
        return FALSE;
		
		

    if (get_eq_char(victim,WEAR_WIELD) != NULL)
        return FALSE;

    if (get_eq_char(victim,WEAR_SHIELD) != NULL)
	return FALSE;

    if (!can_see(ch,victim))
        chance *= number_range(6/10,3/4);
		
	ch_hitroll = GET_HITROLL(ch);
	victim_hitroll = GET_HITROLL(victim);
	
	//For simplicity...
	if (victim_hitroll <= 0) victim_hitroll = 1;
	if (ch_hitroll <= 0) ch_hitroll = 1;

	if (victim_hitroll < ch_hitroll) {
		hit_ratio = (float)ch_hitroll / (float)victim_hitroll;
		
		if (hit_ratio > 1.1)
			chance *= 1.1;
		else 
			chance *= hit_ratio;
	}
	
	if (str <= 5)
	chance += 0;
    else if (str <= 10)
	chance += str/2;
    else if (str <=15)
	chance += (str);
    else if (str <= 20)
	chance += (6*str/5);
    else
	chance += 3*str/2;

    if ( number_percent( ) >= ((chance + (victim->level +ch->level) - 40)/2))
    {
        check_improve(victim,gsn_ironhands,FALSE,5);
        return FALSE;
    }

    attack=get_dam_message(ch,dt);

    sprintf(buf1,"You parry $n's %s.",attack);
    sprintf(buf2,"$N parries your %s.",attack);
    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);
    check_improve(victim,gsn_ironhands,TRUE,6);
    return TRUE;
}

bool check_unarmed_defense( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    int ch_hitroll, victim_hitroll, dex = get_curr_stat(victim,STAT_DEX);
	float chance, hit_ratio;
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char *attack;

    if ( !IS_AWAKE(victim) )
        return FALSE;

    if (IS_NPC(victim))
        return FALSE;

    if ((chance = get_skill(victim,gsn_unarmed_defense) /2) == 0)
        return FALSE;
		
	ch_hitroll = GET_HITROLL(ch);
	victim_hitroll = GET_HITROLL(victim);
	
	//For simplicity...
	if (victim_hitroll <= 0) victim_hitroll = 1;
	if (ch_hitroll <= 0) ch_hitroll = 1;

	if (victim_hitroll < ch_hitroll) {
		hit_ratio = (float)ch_hitroll / (float)victim_hitroll;
		
		if (hit_ratio > 1.25)
			chance *= 1.25;
		else 
			chance *= hit_ratio;
	}

    if (get_eq_char(victim,WEAR_WIELD) != NULL)
        return FALSE;

	if (dex <= 5)
	chance += 0;
    else if (dex <= 10)
	chance += dex/2;
    else if (dex <=15)
	chance += (2*dex/3);
    else if (dex <= 20)
	chance += dex;
    else
	chance += 3*dex/2;
	
    if (!can_see(ch,victim))
        chance /= 2;

    chance += get_curr_stat(victim,STAT_DEX) - get_curr_stat(ch,STAT_DEX);

    if (IS_AFFECTED(victim, AFF_HASTE))
	chance += 10;

    if (IS_AFFECTED(ch, AFF_HASTE))
	chance -= 10;

    if (number_percent() >= chance)
    {
        check_improve(victim,gsn_unarmed_defense,FALSE,5);
        return FALSE;
    }

    if (chance > 90)
        chance = 90;


    attack=get_dam_message(ch,dt);

    sprintf(buf1,"You deftly avoid $n's %s.",attack);
    sprintf(buf2,"$N deftly avoids your %s.",attack);
    act(buf1,ch,0,victim,TO_VICT);
    act(buf2,ch,0,victim,TO_CHAR);
    check_improve(victim,gsn_unarmed_defense,TRUE,6);
    return TRUE;
}

char *get_dam_message(CHAR_DATA *ch, int dt)
{
        return get_attack_noun(ch,dt);
}

char * get_attack_noun(CHAR_DATA *ch, int dt)
{
    OBJ_DATA *wield;
    int tmp_dt;
    char *attack;
	
	if (is_affected(ch,skill_lookup("valiant wrath")))
    {
		attack = str_dup("valiant wrath");
		return attack;
	}

    tmp_dt = TYPE_HIT;
    if (dt == gsn_dual_wield)
        wield = get_eq_char(ch,WEAR_DUAL_WIELD);
    else
		wield = get_eq_char(ch,WEAR_WIELD);
        if (wield != NULL)
			tmp_dt += wield->value[3];
    else
        tmp_dt += ch->dam_type;
    if (tmp_dt > TYPE_HIT && tmp_dt < TYPE_HIT + MAX_DAMAGE_MESSAGE)
        attack = attack_table[tmp_dt - TYPE_HIT].noun;
    else
    {
        attack = attack_table[17].noun;
    }
    return attack;
}

int get_attack_number(CHAR_DATA *ch, int dt)
{
      OBJ_DATA *wield;
      int tmp_dt;

      tmp_dt = TYPE_HIT;
      if (dt  == gsn_dual_wield)
          wield = get_eq_char(ch,WEAR_DUAL_WIELD);
      else
          wield = get_eq_char(ch,WEAR_WIELD);
        if (wield != NULL)
          tmp_dt += wield->value[3];
      else
          tmp_dt += ch->dam_type;
      if (tmp_dt > TYPE_HIT && tmp_dt < TYPE_HIT + MAX_DAMAGE_MESSAGE)
        return (tmp_dt - TYPE_HIT);
      return 1;
}

void do_gslay (CHAR_DATA * ch, char *argument)
  {
  CHAR_DATA *victim;
  char arg[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];

  one_argument (argument, arg);
  if (arg[0] == '\0')
  {
  send_to_char ("Slay whom?\n\r", ch);
  return;
  }

  if ((victim = get_char_world (ch, arg)) == NULL)
  {
  send_to_char ("They aren't here.\n\r", ch);
  return;
  }

  if (ch == victim)
  {
  send_to_char ("Suicide is a mortal sin.\n\r", ch);
  return;
  }

  if (!IS_NPC (victim) && victim->level >= get_trust (ch))
  {
  send_to_char ("You failed.\n\r", ch);
  return;
  }

  act ("You slay $M in accordance with your will!", ch,
  NULL,
  victim, TO_CHAR);
  act ("$n slays you according to his will!", ch, NULL, victim,
TO_VICT);
  act ("$n slays $N in accordance with his will!", ch,
  NULL,
  victim, TO_NOTVICT);

  sprintf(buf, "{WAn Immortal {Gglares at {Y%s {Gand he explodes into a million pieces!{x", victim->name);
  do_echo(ch, buf);

  raw_kill (ch, victim );

  do_help(ch, "rules" );

  return;
  }

void do_ambush( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];   
    int chance;

    
    one_argument( argument, arg);


    if (arg[0] == '\0')
    {
	send_to_char("Ambush whom?\n\r",ch);
	return;
    }

    if (ch->fighting != NULL)
    {
	send_to_char("But you're still fighting!\n\r",ch);
	return;
    }
    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char("They aren't here.\n\r",ch);
	return;
    }
    else if (!IS_AFFECTED(ch, AFF_CAMOUFLAGE) || can_see(victim, ch) )
    {
	send_to_char("But they can still see you.\n\r",ch);
	return;
    }
 
    if ( victim == ch )
    {
	send_to_char("That's just not possible.\n\r",ch);
	return;
    }
    if ( is_safe( ch, victim ) )
      return;

	if (check_chargeset(ch,victim)) {return;}

    chance = get_skill(ch,gsn_moving_ambush);
    if (victim->fighting != NULL 
	&& ( chance < 3
	|| ch->level < skill_table[gsn_moving_ambush].skill_level[ch->class]))
    {
	send_to_char("They are moving around too much to ambush.\n\r",ch);
	return;
    }

    if (victim->fighting != NULL
	&& number_percent() >= chance)
	{
	send_to_char("You can't quite pin them down for your ambush.\n\r",ch);
	return; 
	}

	if (!IS_NPC(ch) && !IS_NPC(victim) && victim->fighting == NULL)
	{
	sprintf(buf,"Help! I've been ambushed by %s!",PERS(ch,victim));
	do_myell(victim,buf);
	}

   WAIT_STATE( ch, skill_table[gsn_ambush].beats );
   if ( (number_percent( ) < get_skill(ch, gsn_ambush) )
   || ( get_skill(ch,gsn_ambush) >= 2 && !IS_AWAKE(victim) ) )
    {
	check_improve(ch,gsn_ambush, TRUE,1);
	multi_hit( ch, victim, gsn_ambush );
    }
    else
    {
	check_improve(ch,gsn_ambush, FALSE,1);
	damage( ch, victim, 0, gsn_ambush, DAM_NONE, TRUE);
    }

    return;
}

bool spike_toed_boots(CHAR_DATA *ch)
{
    OBJ_DATA *boots;

    boots = get_eq_char(ch, WEAR_FEET);

    if (boots != NULL && boots->pIndexData->vnum == 29801)
	return TRUE;

    return FALSE;
}

bool check_ward_diminution(CHAR_DATA *mage, CHAR_DATA *attacker, int dam, int type, int dt)
{
	AFFECT_DATA *ward;
	AFFECT_DATA new_ward;
	int mod=0, duration=0;

	if (!is_affected(mage, gsn_ward_diminution))
		return FALSE;

	if (number_percent() > 35)
		return FALSE;

	if (mage == attacker)
		return FALSE;

	if (type > 3)
		return FALSE;

	ward = affect_find(mage->affected, gsn_ward_diminution);

	if (!ward)
		return FALSE;

	mod = ward->modifier;
	duration = ward->duration;

	if (mod >= mage->max_hit / 2)
	{
		act("Your ward of diminution crumbles under the force of $N's attack.", mage, 0, attacker, TO_CHAR);
		act("$n's ward of diminution crumbles under the force of your attack.", mage, 0, attacker, TO_VICT);
		act("$n's ward of diminution crumbles under the force of $N's attack.", mage, 0, attacker, TO_NOTVICT);
		affect_strip(mage, gsn_ward_diminution);
		return FALSE;
	}

	mod += dam;

	affect_strip(mage, gsn_ward_diminution);

	init_affect(&new_ward);
    	new_ward.where     	= TO_AFFECTS;
        new_ward.aftype		= AFT_POWER;
    	new_ward.type      	= gsn_ward_diminution;
    	new_ward.level     	= 51;
    	new_ward.duration  	= duration;
    	new_ward.location  	= 0;
    	new_ward.modifier  	= mod;
    	new_ward.bitvector 	= 0;
    	affect_to_char( mage, &new_ward );

	act("Your ward absorbs the pain of $N's attack!", mage, 0, attacker, TO_CHAR);
	act("$n's ward absorbs the pain of your attack!", mage, 0, attacker, TO_VICT);
	act("$n's ward absorbs the pain of $N's attack!", mage, 0, attacker, TO_NOTVICT);

	return TRUE;
}

void check_paladin_combo(CHAR_DATA *ch, CHAR_DATA *victim)
{
	int level = 0, sn = gsn_paladin_tech_counter_pc;
	AFFECT_DATA *af, *af_next, affect;
	OBJ_DATA *wield = get_eq_char(ch,WEAR_WIELD);
		
	for (af = ch->affected; af != NULL; af = af_next)
	{
		af_next = af->next;
		if (af->type == sn)
			level = af->level;
	}
	
	if (!is_affected(ch,sn))
		return;
	if (level == 15 || level == 25 || level == 35 || level == 45 || level == 55 || level == 65)
		do_say(ch,"Raaaagh!!");
	if (level == 15)
	{
		act("$n's $p glows softly.",ch,wield,victim,TO_VICT);
		act("$n's $p glows softly.",ch,wield,victim,TO_NOTVICT);
		act("$p glows softly.",ch,wield,victim,TO_CHAR);
		act("$n delivers a powerful slice!",ch,wield,victim,TO_VICT);
		act("$n delivers a powerful slice!",ch,wield,victim,TO_NOTVICT);
		act("You slash $N with great force!",ch,wield,victim,TO_CHAR);
		one_hit_new(ch,victim,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,300,"slice");
		affect_strip(ch,sn);
		return;
	}

	if (level == 25)
	{
		act("$n's $p glows faintly.",ch,wield,victim,TO_VICT);
		act("$n's $p glows faintly.",ch,wield,victim,TO_NOTVICT);
		act("$p glows faintly.",ch,wield,victim,TO_CHAR);
		act("$n delivers a powerful slice!",ch,wield,victim,TO_VICT);
		act("$n delivers a powerful slice!",ch,wield,victim,TO_NOTVICT);
		act("You slash $N with great force!",ch,wield,victim,TO_CHAR);
		one_hit_new(ch,victim,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,500,"slice");
		obj_cast_spell(skill_lookup("cure light"),51,ch,ch,wield);
		affect_strip(ch,sn);
		return;
	}

	if (level == 35)
	{
		act("$n's $p hums loudly.",ch,wield,victim,TO_VICT);
		act("$n's $p hums loudly.",ch,wield,victim,TO_NOTVICT);
		act("$p hums loudly.",ch,wield,victim,TO_CHAR);
		act("$n delivers a powerful strike of faith!",ch,wield,victim,TO_VICT);
		act("$n delivers a powerful strike of faith!",ch,wield,victim,TO_NOTVICT);
		act("You slash $N with immense force!",ch,wield,victim,TO_CHAR);
		one_hit_new(ch,victim,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,700,"strike of faith");
		obj_cast_spell(skill_lookup("cure serious"),51,ch,ch,wield);
		if(number_percent()<50)
		{
			act("You feel your bones shatter under the force of $n's attack!",ch,wield,victim,TO_VICT);
			act("$N's bones shatter under the force of $n's attack!",ch,wield,victim,TO_NOTVICT);
			act("$N's bones shatter under the force of your attack!",ch,wield,victim,TO_CHAR);
			init_affect(&affect);
			affect.aftype = AFT_SKILL;
			affect.level = 51;
			affect.type = gsn_boneshatter;
			affect.duration = 7;
			affect.location = APPLY_DEX;
			affect.modifier = -5;
			affect_to_char(victim,&affect);
			affect.location = APPLY_STR;
			affect.modifier = -5;
			affect_to_char(victim,&affect);
		}
		affect_strip(ch,sn);
		return;
	}

	if (level == 45)
	{
		act("$n's $p glows with radiant light.",ch,wield,victim,TO_VICT);
		act("$n's $p glows with radiant light.",ch,wield,victim,TO_NOTVICT);
		act("$p glows with radiant light.",ch,wield,victim,TO_CHAR);
		act("$n delivers a powerful strike of fervor!",ch,wield,victim,TO_VICT);
		act("$n delivers a powerful strike of fervor!",ch,wield,victim,TO_NOTVICT);
		act("You slash $N with immense force!",ch,wield,victim,TO_CHAR);
		one_hit_new(ch,victim,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,850,"strike of fervor");
		obj_cast_spell(skill_lookup("cure critical"),51,ch,ch,wield);

		if(number_percent()<50)
		{
			act("You feel frail.",ch,wield,victim,TO_VICT);
			act("$N seems more frail.",ch,wield,victim,TO_NOTVICT);
			act("$N seems more frail.",ch,wield,victim,TO_CHAR);
			init_affect(&affect);
			affect.aftype = AFT_SKILL;
			affect.type = gsn_soften;
			affect.level = 51;
			affect.duration = 3;
			affect.location = APPLY_DAM_MOD;
			affect.modifier = 10;
			affect_to_char(victim,&affect);
		}
		affect_strip(ch,sn);
		return;
	}
	
	if (level == 55)
	{
		act("$n's $p shines brightly.",ch,wield,victim,TO_VICT);
		act("$n's $p shines brightly.",ch,wield,victim,TO_NOTVICT);
		act("$p shines brightly.",ch,wield,victim,TO_CHAR);
		act("$n delivers a powerful strike of conviction!",ch,wield,victim,TO_VICT);
		act("$n delivers a powerful strike of conviction!",ch,wield,victim,TO_NOTVICT);
		act("You slash $N with immense force!",ch,wield,victim,TO_CHAR);
		one_hit_new(ch,victim,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,1000,"strike of conviction");
		obj_cast_spell(skill_lookup("heal"),51,ch,ch,wield);

		if(number_percent()<50 && !is_affected(ch,gsn_boneshatter))
		{
			act("You feel your bones shatter under the force of $n's attack!",ch,wield,victim,TO_VICT);
			act("$N's bones shatter under the force of $n's attack!",ch,wield,victim,TO_NOTVICT);
			act("$N's bones shatter under the force of your attack!",ch,wield,victim,TO_CHAR);
			init_affect(&affect);
			affect.aftype = AFT_SKILL;
			affect.level = 51;
			affect.type = gsn_boneshatter;
			affect.duration = 3;
			affect.location = APPLY_DEX;
			affect.modifier = -8;
			affect_to_char(victim,&affect);
			affect.location = APPLY_STR;
			affect.modifier = -8;
			affect_to_char(victim,&affect);
		}
		if(number_percent()<50 && !is_affected(ch,gsn_soften))
		{
			act("You feel frail.",ch,wield,victim,TO_VICT);
			act("$N seems more frail.",ch,wield,victim,TO_NOTVICT);
			act("$N seems more frail.",ch,wield,victim,TO_CHAR);
			init_affect(&affect);
			affect.aftype = AFT_SKILL;
			affect.level = 51;
			affect.type = gsn_soften;
			affect.duration = 3;
			affect.location = APPLY_DAM_MOD;
			affect.modifier = 15;
			affect_to_char(victim,&affect);
		}
		affect_strip(ch,sn);
		return;
	}

	if (level == 65)
	{
		act("$n's $p pulses with power!",ch,wield,victim,TO_VICT);
		act("$n's $p pulses with power!",ch,wield,victim,TO_NOTVICT);
		act("$p pulses with power!",ch,wield,victim,TO_CHAR);
		act("$n delivers a powerful strike of virtue!",ch,wield,victim,TO_VICT);
		act("$n delivers a powerful strike of virtue!",ch,wield,victim,TO_NOTVICT);
		act("You slash $N with immense force!",ch,wield,victim,TO_CHAR);
		one_hit_new(ch,victim,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,1200,"strike of divine virtue");
		obj_cast_spell(skill_lookup("rejuvenate"),51,ch,ch,wield);
		if (!is_affected(victim,gsn_boneshatter))
		{
			init_affect(&affect);
			affect.aftype = AFT_INVIS;
			affect.level = 51;
			affect.type = gsn_boneshatter;
			affect.duration = 3;
			affect.aftype = AFT_SKILL;
			affect.location = APPLY_DEX;
			affect.modifier = -10;
			affect_to_char(victim,&affect);
			affect.aftype = AFT_SKILL;
			affect.location = APPLY_STR;
			affect.modifier = -10;
			affect_to_char(victim,&affect);
		}
		if (!is_affected(victim,gsn_soften))
		{
			act("You feel frail.",ch,wield,victim,TO_VICT);
			act("$N seems more frail.",ch,wield,victim,TO_NOTVICT);
			act("$N seems more frail.",ch,wield,victim,TO_CHAR);
			init_affect(&affect);
			affect.aftype = AFT_SKILL;
			affect.type = gsn_soften;
			affect.level = 51;
			affect.duration = 3;
			affect.location = APPLY_DAM_MOD;
			affect.modifier = 20;
			affect_to_char(victim,&affect);
		}
		affect_strip(ch,sn);
		return;
	}
	if (level > 65)
	{
		act("The power running through your veins becomes too much to bare and you lose your focus!",ch,0,0,TO_CHAR);
		affect_strip(ch,sn);
	}
	return;
}	

void do_swordplay(CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA *af, *af_next, affectCounter, affectLanded, initialAffect;
	OBJ_DATA *wield = get_eq_char(ch,WEAR_WIELD), *victimWield, *secondary;
	CHAR_DATA *victim = ch->fighting;
	int level = 0, sn = gsn_paladin_tech_counter_pc;

	if (get_skill(ch,gsn_swordplay) < 5)
		return send_to_char("You are not learned in two-handed swordplay.\n\r",ch);

	if ( wield == NULL || 
	     wield->value[0] != WEAPON_SWORD || 
  	     ( wield->value[0] == WEAPON_SWORD &&              !IS_WEAPON_STAT(wield,WEAPON_TWO_HANDS) ) )
		return send_to_char("You must be using a two-handed sword to use the paladin techniques.\n\r",ch);

	if (victim == NULL)
		return send_to_char("You aren't fighting anyone.\n\r",ch);

	if (argument[0] == '\0')
		return send_to_char("Which technique (Jab, Slash, Charge, Thrust, Downslash, Doubleslash, Whirl)?\n\r",ch);

	if (number_percent() > get_skill(ch,skill_lookup("swordplay")))
	{
		act("$n attempts to execute a complex maneuver with $s sword, but botches it and fails.",ch,0,0,TO_ROOM);
		return send_to_char("You attempt to execute a swordplay maneuver, but botch it.\n\r",ch);
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
	}
	
	check_improve(ch,gsn_swordplay,TRUE,4);
	victimWield = get_eq_char(victim,WEAR_WIELD);
	secondary = get_eq_char(victim,WEAR_DUAL_WIELD);
        if (victimWield == NULL)
            victimWield = get_eq_char(victim,WEAR_DUAL_WIELD);

	for (af = ch->affected; af != NULL; af = af_next)
	{
		af_next = af->next;
		if (af->type == sn)
			level = af->level;
	}

	if (!is_affected(ch,sn))
	{
		init_affect(&initialAffect);
		initialAffect.aftype = AFT_INVIS;
		initialAffect.level = 0;
		initialAffect.type = sn;
		initialAffect.duration = 1;
		affect_to_char(ch,&initialAffect);
	}

	if (!str_prefix(argument,"slash"))
	{
		act("Bringing $p around, $n slashes you across the stomach!",ch,wield,victim,TO_VICT);
		act("Bringing $p sword around, $n slashes $N across the stomach!",ch,wield,victim,TO_NOTVICT);
		act("Bringing $p around, you slash $N across the stomach!",ch,wield,victim,TO_CHAR);
		if(number_percent() < 20 && !is_affected(victim,gsn_cross_slice))
		{
			act("$n's slash leaves a deep wound within you!",ch,0,victim,TO_VICT);
			act("$n's slash leaves a deep wound within $N!",ch,0,victim,TO_NOTVICT);
			act("Your slash leaves a deep wound within $N!",ch,0,victim,TO_CHAR);
			init_affect(&affectLanded);
			affectLanded.where  = TO_AFFECTS;
			affectLanded.aftype = AFT_SKILL;
			affectLanded.type = gsn_cross_slice;
			affectLanded.name = "bleeding";
			affectLanded.level = 25;
			affectLanded.duration = 10;
			affect_to_char(victim,&affectLanded);
		}
		one_hit_new(ch,victim,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,150,"slash");
		affect_strip(ch,sn);
		init_affect(&affectCounter);
		affectCounter.aftype = AFT_INVIS;
		affectCounter.level = level + 3;
		affectCounter.type = sn;
		affectCounter.duration = 1;
		affect_to_char(ch,&affectCounter);
		send_to_char("You gain 3 combo points!\n",ch);
		check_paladin_combo(ch, victim);
		WAIT_STATE(ch,PULSE_VIOLENCE*1.5);
	}	
	else if (!str_prefix(argument,"charge"))
	{
		act("Yelling a cry, $n charges towards you and uses $s momentum to deliver a powerful slice!",ch,0,victim,TO_VICT);
		act("Yelling a cry, $n charges towards $N and uses $s momentum to deliver a powerful slice!",ch,0,victim,TO_NOTVICT);
		act("Yelling a cry, you charge towards $N and use your momentum to deliver a powerful slice!",ch,0,victim,TO_CHAR);

		if(number_percent() < 30)
		{
			act("$n sees an opening in your defenses and charges into you, sending you flying!",ch,0,victim,TO_VICT);
			act("$n charges into $N, sending $S flying!",ch,0,victim,TO_NOTVICT);
			act("Seizing a moment of weakness, you charge into $N, sending $M flying!",ch,0,victim,TO_CHAR);
			WAIT_STATE(victim,PULSE_VIOLENCE*2);
		}
		one_hit_new(ch,victim,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,170,"charge");
		affect_strip(ch,sn);
		init_affect(&affectCounter);
		affectCounter.aftype = AFT_INVIS;
		affectCounter.level = level + 6;
		affectCounter.type = sn;
		affectCounter.duration = 1;
		affect_to_char(ch,&affectCounter);
		send_to_char("You gain 6 combo points!\n",ch);
		check_paladin_combo(ch, victim);
		WAIT_STATE(ch,PULSE_VIOLENCE*2.5);
	}

	else if (!str_prefix(argument,"thrust"))
	{
		act("$n thrusts $p past your defenses and into your flesh!",ch,wield,victim,TO_VICT);
		act("$n thrusts $p past $N's defenses and into $S flesh!",ch,wield,victim,TO_NOTVICT);
		act("You thrust $p past $N's defenses and into $S flesh!",ch,wield,victim,TO_CHAR);

		if(number_percent() < 20 && victimWield != NULL && can_see_obj(ch,victimWield))
		{
			if (IS_OBJ_STAT(victimWield,ITEM_NODISARM) || IS_OBJ_STAT(victimWield,ITEM_NOREMOVE) || is_affected(victim,gsn_spiderhands) || (is_affected(victim,gsn_unholy_affinity) && is_affected_obj(victimWield,gsn_unholy_bless)))
			{
				act("$n twists $p gracefully, but cannot disarm you! ",ch,wield,victim,TO_VICT);
				act("$n twists $p gracefully, but cannot disarm $N!",ch,wield,victim,TO_NOTVICT);
				act("Finding an opening, you twist $p, but cannot disarm $N!",ch,wield,victim,TO_CHAR);
			}
			else
			{
				act("$n twists $p gracefully and disarms you! ",ch,wield,victim,TO_VICT);
				act("$n twists $p gracefully and disarms $N!",ch,wield,victim,TO_NOTVICT);
				act("Finding an opening, you twist $p and disarm $N!",ch,wield,victim,TO_CHAR);
				obj_from_char( victimWield );
    				if ( IS_OBJ_STAT(victimWield,ITEM_NODROP) || IS_OBJ_STAT(victimWield,ITEM_INVENTORY) )
        				obj_to_char( victimWield, victim );
    				else
    				{
        				obj_to_room( victimWield, victim->in_room );
        				if (IS_NPC(victim) && victim->wait == 0 && can_see_obj(victim,victimWield))
        				    get_obj(victim,victimWield,NULL);
    				}


			}
		}
		one_hit_new(ch,victim,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,160,"thrust");
		affect_strip(ch,sn);
		init_affect(&affectCounter);
		affectCounter.aftype = AFT_INVIS;
		affectCounter.level = level + 5;
		affectCounter.type = sn;
		affectCounter.duration = 1;
		affect_to_char(ch,&affectCounter);
		send_to_char("You gain 5 combo points!\n",ch);
		check_paladin_combo(ch, victim);
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
	}

	else if (!str_prefix(argument,"downslash"))
	{
		act("$n swings $p diagonally across your legs!",ch,wield,victim,TO_VICT);
		act("$n swings $p diagonally across $N's legs!",ch,wield,victim,TO_NOTVICT);
		act("You swing $p diagonally across $N's legs!",ch,wield,victim,TO_CHAR);

		if(number_percent() < 20 && !is_affected(victim,gsn_hobble))
		{
			act("A shooting pain runs through your thigh and you find yourself limping!",ch,0,victim,TO_VICT);
			act("$N's eyes widen as $S thigh begins pouring blood!",ch,0,victim,TO_NOTVICT);
			act("$N's eyes widen as $S thigh begins pouring blood!",ch,0,victim,TO_CHAR);
			init_affect(&affectLanded);
			affectLanded.where  = TO_AFFECTS;
			affectLanded.aftype = AFT_SKILL;
			affectLanded.type = gsn_hobble;
			affectLanded.level = 51;
			affectLanded.duration = number_range(1,3);
			affect_to_char(victim,&affectLanded);
			affectLanded.location = APPLY_DEX;
			affectLanded.modifier = number_range(-6,-4);
			affect_to_char(victim,&affectLanded);
		}

		one_hit_new(ch,victim,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,130,"downslash");
		affect_strip(ch,sn);
		init_affect(&affectCounter);
		affectCounter.aftype = AFT_INVIS;
		affectCounter.level = level + 4;
		affectCounter.type = sn;
		affectCounter.duration = 1;
		affect_to_char(ch,&affectCounter);
		send_to_char("You gain 4 combo points!\n",ch);
		check_paladin_combo(ch, victim);
		WAIT_STATE(ch,PULSE_VIOLENCE*1.75);
	}


	else if (!str_prefix(argument,"jab"))
	{
		act("$n quickly jabs you with $p!",ch,wield,victim,TO_VICT);
		act("$n quickly jabs $N with $p!",ch,wield,victim,TO_NOTVICT);
		act("You quickly jab $N with $p!",ch,wield,victim,TO_CHAR);

		one_hit_new(ch,victim,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,110,"jab");
		affect_strip(ch,sn);
		init_affect(&affectCounter);
		affectCounter.aftype = AFT_INVIS;
		affectCounter.level = level + 2;
		affectCounter.type = sn;
		affectCounter.duration = 1;
		affect_to_char(ch,&affectCounter);
		send_to_char("You gain 2 combo points!\n",ch);
		check_paladin_combo(ch, victim);
		WAIT_STATE(ch,PULSE_VIOLENCE);
	}

	else if (!str_prefix(argument,"doubleslash"))
	{
		act("$n brings $p around and slashes you across the chest!",ch,wield,victim,TO_VICT);
		act("$n brings $p around and slashes $N across the chest!",ch,wield,victim,TO_NOTVICT);
		act("You bring $p around and slash $N across the chest!",ch,wield,victim,TO_CHAR);
		one_hit_new(ch,victim,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,150,"slash");
		if (victim != NULL) {
			act("$n quickly spins and brings $p around a second time, slamming its flat into $N's head!",ch,wield,victim,TO_VICT);
			act("$n quickly spins and brings $p around a second time, slamming its flat into $N's head!",ch,wield,victim,TO_NOTVICT);
			act("You quickly spin and bring $p around a second time, slamming its flat into $N's head!",ch,wield,victim,TO_CHAR);
			one_hit_new(ch,victim,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,150,"smash");
		}
		affect_strip(ch,sn);
		init_affect(&affectCounter);
		affectCounter.aftype = AFT_INVIS;
		affectCounter.level = level + 7;
		affectCounter.type = sn;
		affectCounter.duration = 1;
		affect_to_char(ch,&affectCounter);
		send_to_char("You gain 7 combo points!\n",ch);
		check_paladin_combo(ch, victim);
		WAIT_STATE(ch,PULSE_VIOLENCE*2.5);
	}

	else if (!str_prefix(argument,"whirl"))
	{
		CHAR_DATA *target, *targetLooper;
		int whirl;
		char message[MSL];

		for (target = ch->in_room->people; target != NULL; target = targetLooper)
		{
			targetLooper = target->next;
			if (target->fighting == ch)
			{
				act("$n whirls and brings $p around, dicing $s enemies!",ch,wield,0,TO_ROOM);
				whirl = number_range(1,4);
				if (whirl == 1) sprintf(message,"$n slashes you across the chest!");
				else if (whirl == 2) sprintf(message,"$n's slash catches you in the arm!");
				else if (whirl == 3) sprintf(message,"$n's slash connects with your stomach!");
				else if (whirl == 4) sprintf(message,"$n's slash cleaves into your leg!");
				act(message,ch,wield,target,TO_VICT);
				act("You whirl and bring $p around, dicing your enemies!",ch,wield,target,TO_CHAR);
				one_hit_new(ch,target,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,100,"whirl");
				affect_strip(ch,sn);
				
			}
		}
		init_affect(&affectCounter);
		affectCounter.aftype = AFT_INVIS;
		affectCounter.level = level + 8;
		affectCounter.type = sn;
		affectCounter.duration = 1;
		affect_to_char(ch,&affectCounter);
		send_to_char("You gain 8 combo points!\n",ch);
		check_paladin_combo(ch, victim);
		WAIT_STATE(ch,PULSE_VIOLENCE*3);
	}

	else
		return send_to_char("That is not a known swordplay maneuver.\n\r",ch);
	return;
}

void do_lash(CHAR_DATA *ch, char *argument)
{
	char arg[MIL];
	CHAR_DATA *victim;
	int chance;
	char buf[MSL];
	OBJ_DATA *weapon;

	one_argument(argument,arg);
 
	if ((chance = get_skill(ch, gsn_lash)) == 0)
	{   
	  	send_to_char("You don't have the skill to lash people's legs.\n\r",ch);
	  	return;
	}

	if (IS_NPC(ch) && IS_AFFECTED(ch,AFF_CHARM))
	  	return;

	weapon = get_eq_char(ch,WEAR_WIELD);
	if (weapon == NULL || (weapon->value[0] != WEAPON_WHIP && weapon->value[0] != WEAPON_FLAIL) )
	{
	  	chance -= 15;
	  	weapon = get_eq_char(ch,WEAR_DUAL_WIELD);
	}
	if (weapon == NULL)
	{
	  	send_to_char("You aren't wielding any weapon to lash with.\n\r",ch);
	  	return;
	}
	if (weapon->value[0] != WEAPON_WHIP && weapon->value[0] != WEAPON_FLAIL)
	{
	  	send_to_char("You need to be wielding a whip or flail to lash.\n\r",ch);
	  	return;
	}

	if (arg[0] == '\0')
	{
		victim = ch->fighting;
		if (victim == NULL)
		{
		  send_to_char("But you aren't fighting anyone!\n\r",ch);
		  return;
		}
	}

	else if ((victim = get_char_room(ch, arg)) == NULL)
	{
	  	send_to_char("They aren't here.\n\r",ch);
	  	return;
	}

	if (victim->position == POS_SLEEPING || victim->position == POS_RESTING)
	{
	  	act("$N isn't on $S feet.",ch,NULL,victim,TO_CHAR);
	  	return;
	} 

	if (victim == ch)
	{
	  	send_to_char("You try to lash your feet and look clumsy doing it.\n\r",ch);
	  	return;
	}

	if (is_safe(ch, victim))
	  	return;

	if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
	{
	  	act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
	  	return;
	}

	if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
	  	chance += 5;
	if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
	  	chance -= 15;
	chance += get_curr_stat(ch,STAT_DEX)/2;
	chance -= get_curr_stat(victim,STAT_DEX)/2;
	if (IS_AFFECTED(victim,AFF_FLYING))
	  	chance -= dice(2,5);
	if (!IS_NPC(ch) && (ch->class == CLASS_CHANNELER
	|| ch->class == CLASS_NECROMANCER
	|| ch->class == CLASS_ELEMENTALIST) )
	  	chance -= 10;

	chance += (ch->level - victim->level)*3;

	if (!IS_NPC(ch) && !IS_NPC(victim) && (victim->fighting == NULL || ch->fighting == NULL))
	{
	 	sprintf(buf,"Help! %s is lashing me!",PERS(ch,victim));
	  	do_myell(victim,buf);
	}
	if (number_percent() > chance)
	{
	  	act("$n lashes at $N's legs but misses.",ch,0,victim,TO_NOTVICT);
	  	act("$n lashes at your legs but misses.",ch,0,victim,TO_VICT);
	  	act("You lash at $N's legs but miss.",ch,0,victim,TO_CHAR);
	  	check_improve(ch,gsn_lash,FALSE,1);
	  	WAIT_STATE(ch,skill_table[gsn_lash].beats);
	  	return;
	}
	act("$n lashes $N's legs, sending $M crashing to the ground.",ch,0,victim,TO_NOTVICT);
	act("$n lashes your legs, sending you crashing to the ground.",ch,0,victim,TO_VICT);
	act("You lash $N's legs, sending $M crashing to the ground.",ch,0,victim,TO_CHAR);
	check_improve(ch,gsn_lash,TRUE,1);
	WAIT_STATE(victim,2 * PULSE_VIOLENCE);
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	damage(ch,victim,dice(2,7),gsn_lash,DAM_BASH,TRUE);
	check_cheap_shot(ch,victim);
	victim->position = POS_RESTING;
	return;
}

bool check_shield_magnetism( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
	OBJ_DATA *weapon;
	float chance = 0;
	char *attack;
	char buf1[MSL], buf2[MSL];

	if ( !IS_AWAKE(victim) )
		return FALSE;
	
	if (!is_affected(victim, skill_lookup("ward of magnetism")))
		return FALSE;
	
	weapon = (dt == gsn_dual_wield ? get_eq_char(ch, WEAR_DUAL_WIELD) : get_eq_char(ch, WEAR_WIELD));
	
	if (weapon == NULL) return FALSE;
	
	if (!IS_METAL(weapon)) return FALSE;
	
	chance = 80;
	
	//Mobs are dumb and can't swap out weapons. They were getting totally crushed by this skill, so I'm going to reduce its effectiveness against them. Zornath
	if (!IS_NPC(ch))
		chance -= weapon->weight * 3;
	else {
		chance -= weapon->weight * 6;
		if (chance < 15) chance = 15;
	}
		
	chance += get_curr_stat(ch, STAT_STR) * 1.5;
	
	chance = UMIN(chance, 95);
	
	if (number_percent() > chance) return FALSE;
	
	attack = get_dam_message(ch,dt);
	
	sprintf(buf1,"$n's %s is repelled by your ward of magnetism.", attack);
	sprintf(buf2,"Your %s is repelled by $N's ward of magnetism.", attack);
	
	act(buf1,ch,0,victim,TO_VICT);
	act(buf2,ch,0,victim,TO_CHAR);
	
	return TRUE;
}

void check_critical_strike ( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
//////////////////////////// Begin Critical Strike Check /////////////////////////////////
        if (number_percent() < ch->pcdata->learned[skill_lookup("critical strike")] / 6)
        {
                if (is_wielded(ch,WEAPON_SWORD,WIELD_PRIMARY) 
                 || is_wielded(ch,WEAPON_AXE,WIELD_PRIMARY) 
                 || is_wielded(ch,WEAPON_DAGGER,WIELD_PRIMARY) 
                 || is_wielded(ch,WEAPON_SPEAR,WIELD_PRIMARY) 
                 || is_wielded(ch,WEAPON_POLEARM,WIELD_PRIMARY) 
                 || is_wielded(ch,WEAPON_EXOTIC,WIELD_PRIMARY))
                {
                        act("You deliver a critical strike!",ch,0,0,TO_CHAR);
                        act("$n delivers a critical strike!",ch,0,0,TO_ROOM);
                        one_hit_new(ch,victim,dt,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,115,"critical strike");
                        if (!is_affected(victim,skill_lookup("critical strike")))
                        {
                                act("$n's critical strike tears open an immense gash in your body!",ch,0,victim,TO_VICT);
                                act("$n's critical strike tears open an immense gash in $N's body!",ch,0,victim,TO_NOTVICT);
                                act("Your critical strike tears open an immense gash in $N's body!",ch,0,victim,TO_CHAR);
                                AFFECT_DATA bleeding;
                                init_affect(&bleeding);
                                bleeding.aftype = AFT_SKILL;
                                bleeding.type = skill_lookup("critical strike");
                                bleeding.affect_list_msg = str_dup("induces major bleeding");
                                bleeding.owner_name = str_dup(ch->original_name);
                                bleeding.duration = 3;
                                if (is_wielded(ch,WEAPON_SWORD,WIELD_PRIMARY))
                                        bleeding.level = 3;
                                else if (is_wielded(ch,WEAPON_AXE,WIELD_PRIMARY))
                                        bleeding.level = 4;
                                else if (is_wielded(ch,WEAPON_DAGGER,WIELD_PRIMARY))
                                        bleeding.level = 2;
                                else if (is_wielded(ch,WEAPON_POLEARM,WIELD_PRIMARY))
                                        bleeding.level = 5;
                                else if (is_wielded(ch,WEAPON_SPEAR,WIELD_PRIMARY))
                                        bleeding.level = 6;
                                else
                                        bleeding.level = 10;
                                affect_to_char(victim,&bleeding);
                        }
                }
               else if (is_wielded(ch,WEAPON_FLAIL,WIELD_PRIMARY) 
                     || is_wielded(ch,WEAPON_MACE,WIELD_PRIMARY) 
                     || is_wielded(ch,WEAPON_STAFF,WIELD_PRIMARY) 
                     || is_wielded(ch,WEAPON_WHIP,WIELD_PRIMARY))
                {
                        act("You deliver a critical strike!",ch,0,0,TO_CHAR);
                        act("$n delivers a critical strike!",ch,0,0,TO_ROOM);
                        one_hit_new(ch,victim,dt,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,115,"critical strike");
                        if (!is_affected(victim,skill_lookup("critical strike")))
                        {
                                act("You feel your bones shatter under the force of $n's critical strike!",ch,0,victim,TO_VICT);
                                act("$n's critical strike shatters $N's bones!",ch,0,victim,TO_NOTVICT);
                                act("Your critical strike shatters $N's bones!",ch,0,victim,TO_CHAR);
                                AFFECT_DATA boneshatter;
                                init_affect(&boneshatter);
                                boneshatter.aftype = AFT_SKILL;
                                boneshatter.type = skill_lookup("critical strike");
                                boneshatter.duration = 3;
                                boneshatter.level = 1;
                                boneshatter.owner_name = str_dup(ch->original_name);
                                boneshatter.location = APPLY_STR;
                                if (is_wielded(ch,WEAPON_FLAIL,WIELD_PRIMARY))
                                        boneshatter.modifier = -3;
                                else if (is_wielded(ch,WEAPON_MACE,WIELD_PRIMARY))
                                        boneshatter.modifier = -4;
                                else if (is_wielded(ch,WEAPON_STAFF,WIELD_PRIMARY))
                                        boneshatter.modifier = -3;
                                else if (is_wielded(ch,WEAPON_WHIP,WIELD_PRIMARY))
                                        boneshatter.modifier = -2;
                                else
                                        boneshatter.modifier = -1;
                                affect_to_char(victim,&boneshatter);

                                boneshatter.location = APPLY_DEX;
                                if (is_wielded(ch,WEAPON_FLAIL,WIELD_PRIMARY))
                                        boneshatter.modifier = -3;
                                else if (is_wielded(ch,WEAPON_MACE,WIELD_PRIMARY))
                                        boneshatter.modifier = -4;
                                else if (is_wielded(ch,WEAPON_STAFF,WIELD_PRIMARY))
                                        boneshatter.modifier = -3;
                                else if (is_wielded(ch,WEAPON_WHIP,WIELD_PRIMARY))
                                        boneshatter.modifier = -2;
                                else
                                        boneshatter.modifier = -1;
                                affect_to_char(victim,&boneshatter);
                        }
                }
        }
//////////////////////////// End Critical Strike Check /////////////////////////////////                        

}
