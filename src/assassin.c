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
*       In using this code you agree to comply with the Tartarus license   *
*       found in the file /Tartarus/doc/tartarus.doc                       *
***************************************************************************/
#include "include.h"

DECLARE_DO_FUN(do_myell);
DECLARE_DO_FUN(do_yell);

void 	check_ground_control 	args( ( CHAR_DATA *ch,CHAR_DATA *victim,int chance,int dam) );
void	check_owaza		args( (CHAR_DATA *ch,CHAR_DATA *victim) );

void prelag_skills( void )
{
    	CHAR_DATA *ch;
    	CHAR_DATA *ch_next;
		CHAR_DATA *owner = NULL;
	AFFECT_DATA *paf;
	AFFECT_DATA *paf_next;
	

	for ( ch = char_list; ch != NULL; ch = ch_next )
	{
		ch_next = ch->next;

		for ( paf = ch->affected; paf != NULL; paf = paf_next )
		{
			paf_next = paf->next;
			if (paf->owner_name)
				owner = find_char_by_name(paf->owner_name);
			
			if (paf->wait > 0)
			{
				if (ch->fighting != owner)
					paf->wait = 0;
				else
					paf->wait -= 1;
			}

			if (paf->wait == 0)
			{
				// Owaza
				if ( paf->type == gsn_owaza && paf->wait == 0)
					check_owaza(ch,ch->fighting);
			}

		}
	}
}

void do_crescent_kick(CHAR_DATA *ch,char *argument)
{
	char arg[MIL];
	CHAR_DATA *victim;
    OBJ_DATA *obj;
	int dam, chance;

	one_argument(argument,arg);

	if ((chance = get_skill(ch,gsn_crescent_kick)) == 0)
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

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

    if (is_affected(ch,AFF_BLIND))
    {
        chance /= 2;
    }

	if (chance > number_percent())
	{
		act("You begin the fluid motions of the crescent kick.",ch,NULL,victim,TO_CHAR);
		act("$n begins the fluid motions of the crescent kick.",ch,NULL,victim,TO_VICT);
		act("$n begins the fluid motions of the crescent kick.",ch,NULL,victim,TO_NOTVICT);

		dam = (ch->drain_level + ch->level)/2;
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(ch->level/5,ch->level/4);

		WAIT_STATE(ch,PULSE_VIOLENCE*2);
		damage(ch,victim,dam,gsn_crescent_kick,DAM_BASH,TRUE);

		chance /= 1.4;
		
		obj = get_eq_char(victim,WEAR_WIELD);
                if (obj == NULL)
                     obj = get_eq_char(victim,WEAR_DUAL_WIELD);
		
		if ( (chance - 30> number_percent()) && (obj != NULL) )
		{

		    act("As you complete the motions of your crescent kick you aim for $N's weapon.",ch,NULL,victim,TO_CHAR);
		    act("As $n completes the motions of $s crescent kick $m aims for your weapon.",ch,NULL,victim,TO_VICT);
		    act("As $n completes the motions of $s crescent kick $m aims for $n's weapon.",ch,NULL,victim,TO_NOTVICT);

			if (IS_OBJ_STAT(obj,ITEM_NOREMOVE) || IS_OBJ_STAT(obj,ITEM_NODISARM) || is_affected(victim,gsn_spiderhands))
			{
			    act("Your heel makes solid contact with $N's weapon, but it doesn't budge!",ch,NULL,victim,TO_CHAR);
			    act("$n's heel makes solid contact with your weapon, but it doesn't budge!",ch,NULL,victim,TO_VICT);
			    act("$n's heel makes solid contact with $N's weapon, but it doesn't budge!",ch,NULL,victim,TO_NOTVICT);
			    return;
			}
			else
			{
				act("You strike $N's weapon with your heel and send it flying!",ch,NULL,victim,TO_CHAR);
				act("$N strikes your weapon with $s heel and sends it flying!",ch,NULL,victim,TO_VICT);
				act("$n strikes $N's weapon with $s heel and sends it flying!",ch,NULL,victim,TO_NOTVICT);
				
				obj_from_char( obj );
				if (IS_OBJ_STAT(obj,ITEM_NODROP))
				{
					obj_to_char(obj,victim);
				}
				else
				{
					obj_to_room( obj, victim->in_room );
					if (IS_NPC(victim) && victim->wait == 0 && can_see_obj(victim,obj))
					{
						WAIT_STATE(victim,PULSE_VIOLENCE*3);
						get_obj(victim,obj,NULL);
						do_wear(victim,obj->name);
					}
				}
			}
		}
		check_improve(ch,gsn_crescent_kick,TRUE,1);
		check_follow_through(ch,victim,dam);
	}
	else
	{
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
		damage(ch,victim,0,gsn_crescent_kick,DAM_BASH,TRUE);
		check_improve(ch,gsn_crescent_kick,FALSE,1);
	}
}

void do_axe_kick(CHAR_DATA *ch,char *argument)
{
	char arg[MIL];
	AFFECT_DATA af;
	CHAR_DATA *victim;
	int dam, chance;

	one_argument(argument,arg);

	if ((chance = get_skill(ch,gsn_axe_kick)) == 0)
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

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

    if (is_affected(ch,AFF_BLIND))
    {
        chance /= 2;
    }

	if (chance > number_percent() && IS_SET(victim->parts,PART_ARMS))
	{
		act("You raise your leg high above $N's shoulder and lower it in a swift strike.",ch,NULL,victim,TO_CHAR);
		act("$n raises $s leg high above your shoulder and lowers it in a swift strike.",ch,NULL,victim,TO_VICT);
		act("$n raises $s leg high above $S's shoulder and lowers it in a swift strike.",ch,NULL,victim,TO_NOTVICT);

		dam = (ch->drain_level + ch->level)/1.5;
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(0,(ch->drain_level + ch->level)/6);
		dam += number_range(ch->level/5,ch->level/4);

		WAIT_STATE(ch,PULSE_VIOLENCE*2);
		damage_new(ch,victim,dam,gsn_axe_kick,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"axe kick");

		chance -= (victim->size - ch->size) * 4;
		
		if (chance > number_percent())
		{
			if (!is_affected(victim,gsn_axe_kick))
			{
				act("The sound of bones cracking echos from $N's shoulder as you shatter $S shoulder blade.",ch,NULL,victim,TO_CHAR);
				act("The sound of bones cracking echos from your shoulder as $n shatters your shoulder blade.",ch,NULL,victim,TO_VICT);
				act("The sound of bones cracking echos from $N's shoulder as $n shatters $S shoulder blade.",ch,NULL,victim,TO_NOTVICT);

				init_affect(&af);
				af.where        = TO_AFFECTS;
				af.type         = gsn_axe_kick;
				af.aftype		= AFT_MALADY;
				af.level        = ch->level;
				af.duration     = ch->level/8;
				af.modifier     = -(ch->level/6);
				af.bitvector    = 0;
				af.location     = APPLY_STR;
				affect_to_char(victim,&af);
				af.location     = APPLY_DEX;
				affect_to_char(victim,&af);
			}
			else
			{
				act("You hear the shattered bone in $N's shoulder shifting as you strike $S shoulder blade squarely again.",ch,NULL,victim,TO_CHAR);
				act("You hear the shattered bone in your shoulder shifting as $n strikes your shoulder blade squarely again.",ch,NULL,victim,TO_VICT);
				act("You hear the shattered bone in $N's shoulder shifting as $n strikes $S shoulder blade squarely again.",ch,NULL,victim,TO_NOTVICT);
			}

		check_follow_through(ch,victim,dam);
		}
		check_improve(ch,gsn_axe_kick,TRUE,1);
	}
	else
	{
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
		damage(ch,victim,0,gsn_axe_kick,DAM_BASH,TRUE);
		check_improve(ch,gsn_axe_kick,FALSE,1);
	}
}

void do_scissor_kick(CHAR_DATA *ch,char *argument)
{
	char arg[MIL];
	CHAR_DATA *victim;
	int dam, chance;

	one_argument(argument,arg);

	if ((chance = get_skill(ch,gsn_scissor_kick)) == 0)
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

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

    	if (is_affected(ch,AFF_BLIND))
    	{
        	chance /= 2;
    	}

	if (chance > number_percent())
	{
		act("You leap towards $N in an attempt to wrap your legs around $M.",ch,NULL,victim,TO_CHAR);
		act("$n leaps towards you in an attempt to wrap $s legs around you.",ch,NULL,victim,TO_VICT);
		act("$n leaps towards $N in an attempt to wrap $s legs around $M.",ch,NULL,victim,TO_NOTVICT);

		dam = 25+(chance + GET_DAMROLL(ch))/1.5;
		damage(ch,victim,dam,gsn_scissor_kick,DAM_BASH,TRUE);

		chance -= (victim->size - ch->size) * 10;
		
		if (chance > number_percent())
		{
			act("Upon successfully enclosing $N's upper body in your legs, you bring $M to the ground!",ch,NULL,victim,TO_CHAR);
			act("Upon successfully enclosing you in $s legs, $n brings you to the ground!",ch,NULL,victim,TO_VICT);
			act("Upon successfully enclosing $N in $s legs, $n brings $N to the ground!.",ch,NULL,victim,TO_NOTVICT);
			ADD_WAIT_STATE(victim,PULSE_VIOLENCE*2);
		}
		check_improve(ch,gsn_scissor_kick,TRUE,1);
		check_follow_through(ch,victim,dam);
	}
	else
	{
		damage(ch,victim,0,gsn_scissor_kick,DAM_BASH,TRUE);
		check_improve(ch,gsn_scissor_kick,FALSE,1);
	}
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
}

void do_tigerclaw (CHAR_DATA *ch,char *argument)
{
	char arg[MIL];
	char buf[MSL];
	CHAR_DATA *victim;
	int dam, chance;
	AFFECT_DATA af;

	one_argument(argument,arg);

	if ((chance = get_skill(ch,gsn_tigerclaw)) == 0)
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

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
        	send_to_char("You jab at your own throat like an idiot.\n\r",ch);
        	return;
    	}

    	if (hands_full(ch))
    	{
        	send_to_char("You cannot tigerclaw without an open hand.\n\r",ch);
        	return;
    	}

    	if (is_safe(ch,victim))
        	return;

        if (is_affected(victim,gsn_tigerclaw))
        {
                send_to_char("Their throat is already constricted.\n\r",ch);
                return;
        }


        if (check_chargeset(ch,victim)) {return;}

        update_pc_last_fight(ch,victim);

    	if (is_affected(ch,AFF_BLIND))
    	{
        	chance /= 5;
    	}

     	chance -= ( (get_curr_stat(victim,STAT_DEX))-(get_curr_stat(ch,STAT_DEX)) );

        if (!(str_cmp(race_table[victim->race].name,"lich")))
                chance = 0;

	if (chance -20> number_percent())
	{
		act("You tighten your fist with a knuckle extruding and strike at $N's throat.",ch,NULL,victim,TO_CHAR);
		act("$n tightens $s fist with a knuckle extruding and strikes at your throat.",ch,NULL,victim,TO_VICT);
		act("$n tightens $s fist with a knuckle extruding and strikes at $N's throat.",ch,NULL,victim,TO_NOTVICT);

		dam = number_range(0,(victim->level + ch->level));

		damage(ch,victim,dam,gsn_tigerclaw,DAM_BASH,TRUE);

		
		act("You strike $N's throat, temporarily closing $S vocal passage!",ch,NULL,victim,TO_CHAR);
		act("$n strikes your throat, temporarily closing your vocal passage!",ch,NULL,victim,TO_VICT);
		act("$n strikes $N's throat, temporarily closing $S vocal passage!",ch,NULL,victim,TO_NOTVICT);

		init_affect(&af);
		af.where        = TO_AFFECTS;
		af.type         = gsn_tigerclaw;
		af.aftype	= AFT_MALADY;
		af.level        = ch->level;
		af.duration     = number_range(1,3);
		af.modifier     = 0;
		af.bitvector    = 0;
		affect_to_char(victim,&af);
		check_improve(ch,gsn_tigerclaw,TRUE,1);
	}
	else
	{
        	sprintf(buf,"Help! %s is jabbing at my throat!",PERS(ch,victim));
        	do_myell(victim,buf);
		damage(ch,victim,0,gsn_tigerclaw,DAM_BASH,TRUE);
		check_improve(ch,gsn_tigerclaw,FALSE,1);
	}

	WAIT_STATE(ch,PULSE_VIOLENCE*2);
}

void do_bindwounds(CHAR_DATA *ch,char *argument)
{
	AFFECT_DATA af;

	if ( (get_skill(ch,gsn_bindwounds) == 0) || (ch->level < skill_table[gsn_bindwounds].skill_level[ch->class]) )
	{
        	send_to_char("Huh?\n\r",ch);
        	return;
	}

	if (is_affected(ch,gsn_bindwounds))
	{
        	send_to_char("You can't apply more aid yet.\n\r",ch);
		return;
	}
	
	if (ch->mana < 40)
	{
        	send_to_char("You don't have the mana.\n\r",ch);
        	return;
	}

	if (number_percent() > ch->pcdata->learned[gsn_bindwounds])
	{
        	send_to_char("You fail to focus on your injuries.\n\r",ch);
        	act("$n fails to focus on $s injuries and bind $s wounds.",ch,0,0,TO_ROOM);
        	ch->mana -= 12;
        	check_improve(ch,gsn_bindwounds,FALSE,3);
        	return;
	}

	ch->mana -= 40;

	act("$n focuses on $s injuries and binds $s wounds.",ch,0,0,TO_ROOM);
	send_to_char("You focus on your injuries and bind your wounds.\n\r",ch);
	send_to_char("You feel better.\n\r",ch);

	ch->hit += ch->max_hit/10;
	ch->hit  = UMIN(ch->hit,ch->max_hit);

	if ( number_percent() < UMAX(1,(ch->drain_level + ch->level)/4) && is_affected(ch,AFF_PLAGUE) )
	{
		affect_strip(ch,gsn_plague);
		act("The sores on $n's body vanish.\n\r",ch,0,0,TO_ROOM);
		send_to_char("The sores on your body vanish.\n\r",ch);
	}

	if ( number_percent() < UMAX(1,(ch->drain_level + ch->level)) && is_affected(ch, gsn_blindness) )
	{
		affect_strip(ch, gsn_blindness);
		send_to_char("Your vision returns!\n\r",ch);
	}

	if ( number_percent() < UMAX(1,(ch->drain_level + ch->level)) && is_affected(ch, gsn_blindness_dust) )
	{
		affect_strip(ch, gsn_blindness_dust);
		send_to_char("You wipe the dust from your eyes.\n\r",ch);
	}

	if ( number_percent() < UMAX(1,(ch->drain_level + ch->level)/2) && is_affected(ch,gsn_poison))
	{
		affect_strip(ch,gsn_poison);
		send_to_char("A warm feeling goes through your body.\n\r",ch);
		act("$n looks better.",ch,0,0,TO_ROOM);
	}

	if ( number_percent() < UMAX(1,(ch->drain_level + ch->level)/2) && is_affected(ch,gsn_poison_dust))
	{
		affect_strip(ch,gsn_poison_dust);
		send_to_char("The dust in your eyes is cleaned out.\n\r",ch);
		act("The dust in $n's eyes looks cleaned out.",ch,0,0,TO_ROOM);
	}

	check_improve(ch,gsn_bindwounds,TRUE,3);
	init_affect(&af);
	af.where		= TO_AFFECTS;
	af.type			= gsn_bindwounds;
	af.location		= 0;
	af.duration		= number_range(4,8);
	af.modifier		= 0;
	af.bitvector		= 0;
	af.level		= ch->level;
	af.aftype		= AFT_SKILL;
	affect_to_char(ch,&af);
	return;
}

void do_vanish(CHAR_DATA *ch,char *argument)
{
    ROOM_INDEX_DATA *pRoomIndex;
    int chance;
    CHAR_DATA *victim;

    if ( (chance = get_skill(ch,gsn_vanish)) == 0
	|| ch->level < skill_table[gsn_vanish].skill_level[ch->class])
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if(is_affected(ch,gsn_vanish)) 
    {
	send_to_char("You cannot vanish again so soon.\n\r",ch);
	return;
    }
/*
    if(ch->position==POS_FIGHTING)
    {
	send_to_char("You cannot vanish while fighting.\n\r",ch);
	return;
    }
*/
    if (ch->mana < 20)
    {
    	send_to_char("You don't have the mana.",ch);
	return;
    }

    chance -= 5;

    if (ch->position==POS_FIGHTING) 
    {
	chance -= 50;
	victim = ch->fighting;
    }

    if (number_percent() > chance)
    {
	send_to_char("You attempt to vanish without trace but fail.\n\r",ch);
	act("$n attempts to slide into the shadows but fails.",ch,0,0,TO_ROOM);
	check_improve(ch,gsn_vanish,FALSE,2);
    	ch->mana -= 10;
        WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
    }

    if (no_vanish_area(ch))
    {
	send_to_char("You attempt to vanish without trace but fail.\n\r",ch);
	act("$n attempts to slide into the shadows but fails.",ch,0,0,TO_ROOM);
    	ch->mana -= 10;
        WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
    }

    ch->mana -= 20;

    for (; ;)
    {
	pRoomIndex = get_room_index(number_range(0,30000));
 	if (pRoomIndex != NULL)
	if (pRoomIndex->area == ch->in_room->area
	&& !IS_SET(pRoomIndex->room_flags,ROOM_PRIVATE)
	&& (pRoomIndex->guild == 0)
	&& can_see_room(ch,pRoomIndex)
	&& !IS_SET(pRoomIndex->room_flags,ROOM_SOLITARY)
	&& ch->in_room->vnum != 1212
	&& pRoomIndex->vnum != 1212)
		break;
    }

    act("$n suddenly vanishes into the shadows!",ch,0,0,TO_ROOM);
    send_to_char("You slip into the shadows and vanish!\n\r",ch);
    char_from_room(ch);
    char_to_room(ch,pRoomIndex);
    act("$n appears from the shadows.",ch,0,0,TO_ROOM);
    do_look(ch,"auto");
    if (ch->position==POS_FIGHTING)
    {
        stop_fighting( ch, FALSE );
        stop_fighting( victim, FALSE );
    }

    AFFECT_DATA af;
    init_affect(&af);
    af.where                = TO_AFFECTS;
    af.aftype               = AFT_SKILL;
    af.type                 = gsn_vanish;
    af.duration             = 0;
    af.level                = ch->level;
    affect_to_char(ch,&af);


    WAIT_STATE(ch,PULSE_VIOLENCE*2);

    return;
}


void do_endure(CHAR_DATA *ch,char *argument)
{
	AFFECT_DATA af;
	int chance;
	
	if ( (chance = get_skill(ch,gsn_endure)) == 0 || ch->level < skill_table[gsn_endure].skill_level[ch->class])
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

	if (ch->fighting != NULL)
	{
		send_to_char("You can't concentrate enough.\n\r",ch);
		return;
	}

	if (ch->mana < 30)
	{
		send_to_char("You don't have enough energy.\n\r",ch);
		return;
	}

	if (is_affected(ch,gsn_endure))
	{
		send_to_char("You are already prepared for magical encounters.\n\r",ch);
		return;
	}

	WAIT_STATE(ch,PULSE_VIOLENCE*2);

	if (number_percent() < chance)
	{
		act("$n concentrates and resumes $s position.",ch,0,0,TO_ROOM);
		send_to_char("You prepare yourself for magical encounters.\n\r",ch);

		ch->mana -= 30;

		init_affect(&af);
		af.where		= TO_AFFECTS;
		af.aftype		= AFT_SKILL;
		af.type			= gsn_endure;
		af.location		= APPLY_SAVING_SPELL;
		af.duration		= ch->level/2;
		af.modifier		= -(ch->level/4);
		af.bitvector		= 0;
		af.level		= ch->level;
		affect_to_char(ch,&af);
		check_improve(ch,gsn_endure,TRUE,1);
	}
	else
	{
		send_to_char("You fail to prepare yourself for magical encounters.\n\r",ch);

		ch->mana -= 15;

		check_improve(ch,gsn_endure,FALSE,1);
	}
	return;
}

void do_heightened_awareness(CHAR_DATA *ch,char *argument)
{
	AFFECT_DATA af;
	int chance;

	if ( (chance = get_skill(ch,gsn_heightened_awareness)) == 0 || ch->level < skill_table[gsn_heightened_awareness].skill_level[ch->class])
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}

	if (ch->fighting != NULL)
	{
		send_to_char("You can't concentrate enough.\n\r",ch);
		return;
	}

	if (ch->mana < 20)
	{
		send_to_char("You don't have enough energy.\n\r",ch);
		return;
	}

	if (is_affected(ch,gsn_heightened_awareness))
	{
		send_to_char("You cannot heighten your awareness any further.\n\r",ch);
		return;
	}

	WAIT_STATE(ch,PULSE_VIOLENCE*1);

	if (number_percent() < chance)
	{
		send_to_char("You feel more aware of your surroundings.\n\r",ch);

		ch->mana -= 20;

		init_affect(&af);
		af.where		= TO_AFFECTS;
		af.aftype		= AFT_SKILL;
		af.type			= gsn_heightened_awareness;
		af.location		= 0;
		af.duration		= ch->level/2;
		af.modifier		= 0;
		af.bitvector		= 0;
		af.level		= ch->level;
        	af.bitvector 		= AFF_DETECT_INVIS;
		af.affect_list_msg = str_dup("grants the ability to detect the invisible");
        	affect_to_char(ch, &af);
        	af.bitvector 		= AFF_INFRARED;;
		af.affect_list_msg = str_dup("grants dark vision");
		affect_to_char(ch, &af);
		check_improve(ch,gsn_heightened_awareness,TRUE,1);
	}
	else
	{
		send_to_char("You fail to heighten your awareness.\n\r",ch);

		ch->mana -= 10;

		check_improve(ch,gsn_heightened_awareness,FALSE,1);
	}
	return;
}

void do_owaza(CHAR_DATA *ch,char *argument)
{
    AFFECT_DATA af;
    CHAR_DATA *victim;
    int chance;

    if ((chance = get_skill(ch,gsn_owaza)) == 0)
    {
 	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
    }

    if (victim == NULL)
    {
        send_to_char("You aren't fighting anyone.\n\r",ch);
        return;
    }
    if (victim == ch)
    {
        send_to_char("Now that would be a bit stupid.\n\r",ch);
        return;
    }

    if (is_affected(ch,AFF_BLIND))
    {
        chance /= 2;
    }

    if (chance > number_percent())
    {
		act("You prepare yourself for a complex manuever.",ch,NULL,victim,TO_CHAR);
		act("$n prepares $mself for a complex manuever.",ch,NULL,victim,TO_VICT);
		act("$n prepares $mself for a complex manuever.",ch,NULL,victim,TO_NOTVICT);

		affect_strip(ch,gsn_owaza);
		init_affect(&af);
		af.where		= TO_AFFECTS;
		af.aftype		= AFT_INVIS;
		af.type			= gsn_owaza;
		af.wait			= 3;
		af.location		= 0;
		af.duration		= 2;
		af.modifier		= 0;
		af.owner_name		= str_dup(ch->original_name);
		af.bitvector		= 0;
		af.level		= ch->level;
		affect_to_char(ch,&af);
    }
    else
    {
    	send_to_char("You attempt to prepare yourself for a complex maneuver, but lose your rhythm.\n\r", ch);
    }
    WAIT_STATE(ch,PULSE_VIOLENCE*3);
    return;
}

void check_owaza(CHAR_DATA *ch,CHAR_DATA *victim)
{
	char owazabuf[MSL];
	int base_owaza_chance, owaza_chance=0, dam;

	base_owaza_chance = get_skill(ch,gsn_owaza);

	dam = (ch->level*3 + number_percent()*3) / 5 * 2;

	if ((!victim) || (!ch))
		return;

	if (victim->fighting != NULL && victim->fighting != ch)
	{
		sprintf(owazabuf,"%s is not fighting you anymore, and you cannot complete your manuever.\n\r",victim->name);
		send_to_char(owazabuf,ch);
		affect_strip(ch,gsn_owaza);
		return;
	}
	
	affect_strip(ch,gsn_owaza);

	act("With $N's defenses open, you deliver a powerful shuto strike to $M!",ch,NULL,victim,TO_CHAR);
	act("With your defenses open, $n delivers a powerful shuto strike to you!",ch,NULL,victim,TO_VICT);
	act("With $N's defenses open, $n delivers a powerful shuto strike to $S!",ch,NULL,victim,TO_NOTVICT);
	damage(ch,victim,dam,gsn_shuto_strike,DAM_BASH,TRUE);
	owaza_chance += base_owaza_chance + (get_curr_stat(ch,STAT_DEX) - get_curr_stat(victim,STAT_DEX))*2;
	owaza_chance /= 3;
	owaza_chance *= 2;
	if (number_percent() < owaza_chance && dam != 0 && victim->fighting == ch)
	{
		dam = (ch->level*3 + number_percent()*3) / 5 * 2;
		act("Securing your grasp, you perform a sukuinage on $N.",ch,NULL,victim,TO_CHAR);
		act("Securing $s grasp, $n performs a sukuinage on you.",ch,NULL,victim,TO_VICT);
		act("Securing $s grasp, $n prepares a sukuinage on $N.",ch,NULL,victim,TO_NOTVICT);
		damage(ch,victim,dam,gsn_sukuinage,DAM_BASH,TRUE);
		owaza_chance = 0;
		owaza_chance += base_owaza_chance + (get_curr_stat(ch,STAT_DEX) - get_curr_stat(victim,STAT_DEX));
		owaza_chance /= 3;
		owaza_chance *= 2;
		if (number_percent() < owaza_chance && dam != 0 && victim->fighting == ch)
		{
			dam = (ch->level*3 + number_percent()*3) / 5 * 2;
			act("With $N momentarily stunned, you execute a seiken strike!",ch,NULL,victim,TO_CHAR);
			act("With you momentarily stunned, $n executes a seiken strike!",ch,NULL,victim,TO_VICT);
			act("With $N momentarily stunned, $n executes a seiken strike!",ch,NULL,victim,TO_NOTVICT);
			damage(ch,victim,dam,gsn_seiken_strike,DAM_BASH,TRUE);
			owaza_chance = 0;
			owaza_chance += base_owaza_chance + (get_curr_stat(ch,STAT_DEX) - get_curr_stat(victim,STAT_DEX));
			owaza_chance += (get_curr_stat(ch,STAT_STR) - get_curr_stat(victim,STAT_STR))*3;
			owaza_chance /= 3;
			owaza_chance *= 2;
			if (number_percent() < owaza_chance && dam != 0 && victim->fighting == ch)
			{
				dam = (ch->level*3 + number_percent()*3) / 5 * 2;
				act("In a swift motion, you throw $N to the ground.",ch,NULL,victim,TO_CHAR);
				act("In a swift motion, $n throws you to the ground.",ch,NULL,victim,TO_VICT);
				act("Int a swift motion, $n throws $N to the ground.",ch,NULL,victim,TO_NOTVICT);
				damage(ch,victim,dam,gsn_throw,DAM_BASH,TRUE);
				if (dam != 0)
				{
					WAIT_STATE(victim,PULSE_VIOLENCE*2);
					dam = (ch->level*3 + number_percent()*3) / 5 * 2;
					check_ground_control(ch,victim,owaza_chance,dam);
				}
		        check_improve(ch,gsn_owaza,TRUE,3);
			}
			else
			{
				if (victim->fighting == ch)
				{
					check_improve(ch,gsn_owaza,TRUE,3);
				}
				else
				{
					check_improve(ch,gsn_owaza,FALSE,3);
				}
				return;
			}
		}
		else
		{
			if (victim->fighting == ch)
			{
				check_improve(ch,gsn_owaza,TRUE,3);
			}
			else
			{
				check_improve(ch,gsn_owaza,FALSE,3);
			}
			return;
		}
	}
	else
	{
		if (victim->fighting == ch)
		{
			check_improve(ch,gsn_owaza,TRUE,3);
		}
		else
		{
			check_improve(ch,gsn_owaza,FALSE,3);
		}
		return;
	}

	return;
}

void do_assassinate(CHAR_DATA *ch,char *argument)
{
	CHAR_DATA *victim;
	int chance, dam;
	char arg[MIL], buf[MIL];
	OBJ_DATA *obj;

	argument = one_argument(argument,arg);
	
	if (IS_NPC(ch))
	  	return;

	if ((get_skill(ch,gsn_assassinate) == 0))
	{
	  	send_to_char("Huh?\n\r",ch);
	  	return;
	}

	if (arg[0] == '\0')
	{
	  	send_to_char("Attempt to assassinate who?\n\r",ch);
	  	return;
	}

	if ( (victim = get_char_room(ch, arg)) == NULL)
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

	if (victim->hit < victim->max_hit - 200)
	{
	  	send_to_char("They have too much blood on them right now to locate their strike point.\n\r",ch);
	  	return;
	}

	chance = get_skill(ch,gsn_assassinate);

        if (IS_CABAL_OUTER_GUARD(victim) || IS_CABAL_GUARD(victim))
                chance = 0;

	if (number_percent() > chance)
	{
	  	sprintf(buf,"Help! %s just tried to assassinate me!",PERS(ch,victim));
	  	do_myell(victim,buf);
	  	damage(ch,victim,0,gsn_assassinate,DAM_OTHER,TRUE);
	  	check_improve(ch,gsn_assassinate,1,TRUE);
	  	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	  	return;
	}

	chance *=2;
	chance /=3;
	chance += (ch->level * 3);
	chance -= (victim->level * 4);

	if (!can_see(victim,ch))
		chance += 10;

	if (victim->position == POS_FIGHTING)
	  	chance -= 10;
	else if (victim->position == POS_SLEEPING)
	  	chance += 10;

	if (ch->pause == 0)
		chance += 15;
	else
		chance -= 15;

	if (victim->pause == 0)
		chance += 15;
	else
		chance -= 15;

	chance /= 2;
	chance = URANGE(2,chance,25);

	if (victim->level > (ch->level + 8))
	  	chance = 0;

        if (!(str_cmp(race_table[victim->race].name,"lich")))
                chance = 0;

	if (!str_cmp(argument, "ohjustdie"))
		chance = 99;

	obj = get_eq_char(ch, WEAR_WIELD);

	if (number_percent() < chance)
	{
		if (!obj) {
			// hand echos
		} else if (obj->value[0] == WEAPON_SWORD) {
			act("The bitter taste of blood fills your mouth as you feel the sharp pain of a blade plunge into your vital organs.", ch, 0, victim, TO_VICT);
			act("You deftly slide from the shadows and pierce the underbelly of $N slicing your blade upwards through his innards, spilling them.", ch, 0, victim, TO_CHAR);
			act("$N clutches at his abdomen in horror as $S innards spill to the ground in a bloody cascade.", ch, 0, victim, TO_NOTVICT);
		} else if (obj->value[0] == WEAPON_DAGGER) {
			act("Your neck jerks involuntarily as you feel a biting pain in the small of your neck. ", ch, 0, victim, TO_VICT);
			act("As the unseen blade retracts from your vertabrae your whole body goes numb as you slump to the ground.", ch, 0, victim, TO_VICT);
			act("You feel the satisfying grinding of bone on your blade as it slices through $N's spine, rendering $M a writhing mass of spasming muscles upon the ground.", ch, 0, victim, TO_CHAR);
			act("$N's mouth opens in a silent scream as $S neck jerks to the side and crumples to the ground as the life drains out of $M.", ch, 0, victim, TO_NOTVICT);
		} else if (obj->value[0] == WEAPON_SPEAR) {
			act("You find yourself looking at the sky as the sudden and quick blow to your knees takes you onto your back.", ch, 0, victim, TO_VICT);
			act("A breeze of pain whips past you as you feel the searing pain erupt from your chest as your heart is pierced.", ch, 0, victim, TO_VICT);
			act("Taking the shaft of your spear, you legsweep your unsuspecting victim bringing him to his back upon the ground.", ch, 0, victim, TO_CHAR);
			act("Quickly switching your grip you thrust with every inch of strength in your body in a brutal downward plunge directly at the heart ending in a bloody geyser of triumph.", ch, 0, victim, TO_CHAR);
			act("A quick breeze is felt, as a sudden eruption of blood spews from the fatal wound within $N.", ch, 0, victim, TO_NOTVICT);
		} else {
        		act("$n {r+++ ASSASSINATES +++{x $N!",ch,0,victim,TO_NOTVICT);
        		act("You {r+++ ASSASSINATE +++{x $N!",ch,0,victim,TO_CHAR);
        		act("$n {r+++ ASSASSINATES +++{x you!",ch,0,victim,TO_VICT);
		}
	  	raw_kill(ch,victim);
	  	check_improve(ch,gsn_assassinate,TRUE,3);
	  	return;
	}
	else
	{
		AFFECT_DATA af;
		dam = ch->damroll*2;
		if (number_percent() < get_skill(ch,gsn_enhanced_damage))
		{
		  	dam += (number_percent() *dam/50);
		  	dam += (number_percent() * dam/100);
		}

		if (!is_affected(victim,skill_lookup("assassinate"))) {
			init_affect(&af);
			af.aftype = AFT_SKILL;
			af.duration = number_range(7,10);
			af.location = APPLY_CON;
			af.modifier = -1 * number_range(4,6);
			af.type = skill_lookup("assassinate");
			af.owner_name = str_dup(ch->original_name);
			affect_to_char(victim, &af);
			af.affect_list_msg = str_dup("bleeding profusely");
			af.location = 0;
			af.modifier = 0;
			affect_to_char(victim, &af);
		}
		
		damage(ch,victim,dam,gsn_assassinate,DAM_ENERGY,TRUE);
		check_improve(ch,gsn_assassinate,FALSE,1);
		if (!IS_NPC(victim) && !IS_NPC(ch) && (victim->ghost == 0))
		{
		  	sprintf(buf,"Help! %s just tried to assassinate me!",PERS(ch,victim));
		  	do_myell(victim,buf);
		}
		multi_hit(victim,ch,TYPE_UNDEFINED);
	}
	return;
}
