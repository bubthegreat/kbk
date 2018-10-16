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

char *  flag_name_lookup        args((long bitv, const struct flag_type *flag_table));

bool is_wielded(CHAR_DATA *ch,int weapon,int type) {

	OBJ_DATA *wield;
	bool primary = FALSE;

        if (IS_NPC(ch))
		return TRUE;

	wield = get_eq_char(ch,WEAR_WIELD);

	//This block of code commented out because there may be certain instances where
	//the MUD will crash if the weapon is not physically on the character. It is not
	//such a heartache for an Imp to meet the same skill requirements as everyone else.
	//It would have been completely removed, but it remains here in case the situation
	//arises where it would need to be removed (unlikely, however).
	/*if(IS_IMP(ch) && wield)
	{
                send_to_char("You're an Implementor, you can do anything!\n\r",ch);
                return TRUE;
        }
	*/

	if (wield == NULL)
	{
		if (weapon == WEAPON_HAND && (get_eq_char(ch,WEAR_DUAL_WIELD) == NULL))
			return TRUE;
		else
			return FALSE;
	}
	if (wield->value[0] == weapon)
	{
		if (type == WIELD_ONE || type == WIELD_PRIMARY) {
			return TRUE;
		}
		primary = TRUE;
	} else {
		if (type == WIELD_PRIMARY) {
			return FALSE;
		}
	}
	wield = get_eq_char(ch,WEAR_DUAL_WIELD);
	if (wield == NULL)
	{
		return FALSE;
	}
	if (wield->value[0] == weapon)
	{
		if (type == WIELD_TWO && primary) {
			return TRUE;
		}
	} else {
		return FALSE;
	}
	return TRUE;
}

void do_pincer(CHAR_DATA *ch,char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	char buf[MAX_STRING_LENGTH];
	int strdam, chance;

	one_argument(argument,arg);

	if (get_skill(ch,gsn_pincer) == 0)
	{
		send_to_char("Huh?\n\r",ch);
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

	if (victim == ch)
	{
		send_to_char("Wouldn't that hurt?\n\r",ch);
		return;
	}

	obj = get_eq_char(ch,WEAR_WIELD);

        if (obj == NULL || obj->value[0] != WEAPON_AXE)
        {
        	send_to_char("You must be dual wielding axes to do that.\n\r",ch);
        	return;
        }

	obj = get_eq_char(ch,WEAR_DUAL_WIELD);

        if (obj == NULL || obj->value[0] != WEAPON_AXE)
        {
        	send_to_char("You must be dual wielding axes to do that.\n\r",ch);
        	return;
        }


	if ( is_safe( ch, victim ) )
		return;

	if (check_chargeset(ch,victim)) {return;}

	if (!IS_NPC(ch) && !IS_NPC(victim) && (victim->fighting == NULL || ch->fighting == NULL))
	{
		sprintf(buf,"Help! %s is hacking me to pieces!",PERS(ch,victim) );
		do_myell(victim,buf);
	}
      
	chance = get_skill(ch, gsn_pincer);
	chance -= 20;
	chance += get_curr_stat(ch, STAT_DEX) - get_curr_stat(victim, STAT_DEX);
	if (IS_AFFECTED(ch, AFF_HASTE))
		chance += 10;
	if (IS_AFFECTED(victim, AFF_HASTE))
		chance -= 10;

	strdam = get_curr_stat(ch, STAT_STR);

	if(number_percent() > chance)
	{
		act("You try to pincer $N, but blunder it.",ch,0,victim,TO_CHAR);
		act("$n tries some fancy axe-work, but blunders it.",ch,0,0,TO_ROOM);
		damage( ch, victim, 0, gsn_pincer,DAM_NONE,TRUE);
		check_improve(ch,gsn_pincer,FALSE,2);
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
		return;
	}
	act("You brutally hack your axes together into $N, momentarily stunning $M!",ch,0,victim,TO_CHAR);
	act("$n brutally hacks $s axes together into you, momentarily stunning you!",ch,0,victim,TO_VICT);
	act("$n brutally hacks $s axes together into $N, momentarily stunning $M!",ch,0,victim,TO_NOTVICT);
	one_hit_new(ch,victim,gsn_pincer,HIT_NOSPECIALS,HIT_UNBLOCKABLE,strdam,HIT_NOMULT,NULL);
	one_hit_new(ch,victim,gsn_pincer,HIT_NOSPECIALS,HIT_UNBLOCKABLE,strdam,150,NULL);
	check_improve(ch,gsn_pincer,TRUE,2);
	WAIT_STATE(ch,3*PULSE_VIOLENCE);
	WAIT_STATE(victim,2*PULSE_VIOLENCE);

	return;
}


/* STRIP -- Graelik */

void strip( CHAR_DATA *ch, CHAR_DATA *victim )
{
    OBJ_DATA *obj;
    OBJ_DATA *secondary;

    obj = get_eq_char( victim, WEAR_WIELD );

    if ( obj == NULL )
    {
        obj = get_eq_char( victim, WEAR_DUAL_WIELD );
	if ( obj == NULL )
            return;
    }

    if (is_affected(victim,gsn_spiderhands) )
    {
        act("Your try to strip $N of his weapon, but $s grip is too tight.", ch,NULL,victim,TO_CHAR);
        act("$n tries to strip you of your weapon, but your grip is too tight.", ch,NULL,victim,TO_VICT);
        act("$n tries to strip $N of his weapon, but $s grip is too tight.", ch,NULL,victim,TO_NOTVICT);
        return;
    }

    if ( IS_OBJ_STAT(obj,ITEM_NOREMOVE) || IS_OBJ_STAT(obj,ITEM_NODISARM))
    {
        act("You try to strip $N of $s weapon but it won't budge!", ch,NULL,victim,TO_CHAR);
        act("$n tries to strip you of your weapon, but your weapon won't budge!", ch,NULL,victim,TO_VICT);
        act("$n tries to strip $N of $s weapon, but $s weapon won't budge.", ch,NULL,victim,TO_NOTVICT);
        return;
    }

    act( "$n strips your weapon from your hands and sends it flying!", ch, NULL, victim, TO_VICT    );
    act( "You strip $N's weapon from $s hands and send it flying!", ch, NULL, victim, TO_CHAR    );
    act( "$n strips $N's weapon from $s hands and sends it flying!", ch, NULL, victim, TO_NOTVICT );

    int chanceToCatch = number_percent();

        if (get_skill(ch,gsn_strip) >= 98 && chanceToCatch >= 66)
        {
    		act( "$n snatches your weapon in midflight!", ch, NULL, victim, TO_VICT    );
    		act( "You snatch $N's weapon in midflight!", ch, NULL, victim, TO_CHAR    );
    		act( "$n snatches $N's weapon in midflight!", ch, NULL, victim, TO_NOTVICT );
         	obj_from_char( obj );
          	obj_to_char( obj, ch );
        }

  	if (get_skill(ch,gsn_strip) < 98 || chanceToCatch < 66)
	{
    	obj_from_char( obj );
        obj_to_room( obj, victim->in_room );
        if (IS_NPC(victim) && victim->wait == 0 && can_see_obj(victim,obj) && IS_AWAKE(victim))
            get_obj(victim,obj,NULL);
	}

    if ((secondary = get_eq_char(victim,WEAR_DUAL_WIELD)) != NULL)
    {
        unequip_char(victim,secondary);
        equip_char(victim,secondary,WEAR_WIELD);
    }
    return;
}

void do_strip( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *wield;
    OBJ_DATA *dwield;
    OBJ_DATA *obj;
    int chance,ch_weapon,vict_weapon,ch_vict_weapon;

    one_argument (argument, arg );


    if ((chance = get_skill(ch,gsn_strip)) == 0)
    {
        send_to_char( "Huh?\n\r", ch );
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

    if (is_safe(ch,victim))	
    {
	send_to_char( "The gods wouldn't like that.\n\r", ch );
	return;
    }

    wield = get_eq_char( ch, WEAR_WIELD );

    if(!wield)
        return send_to_char("You must be wielding a whip or flail to do that.\n\r",ch);

    if((wield->value[0] != WEAPON_WHIP) && (wield->value[0] != WEAPON_FLAIL))
    {
        send_to_char("You must be wielding a whip or flail to do that.\n\r",ch);
        return;
    }


    if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL && ( dwield = get_eq_char( victim, WEAR_DUAL_WIELD ) ) == NULL )
    {
        send_to_char( "Your opponent is not wielding a weapon.\n\r", ch );
        return;
    }

    ch_weapon = get_weapon_skill(ch,get_weapon_sn(ch));
    vict_weapon = get_weapon_skill(victim,get_weapon_sn(victim));
    ch_vict_weapon = get_weapon_skill(ch,get_weapon_sn(victim));

    chance = chance * ch_weapon/100;

    chance += (ch_vict_weapon/2 - vict_weapon) / 2;

    chance += get_curr_stat(ch,STAT_DEX);
    chance -= 1.6 * get_curr_stat(victim,STAT_STR);

    chance += (ch->drain_level + ch->level - victim->drain_level - victim->level) * 2;

    if (victim->position==POS_SLEEPING)
	chance -= 5;

    if (number_percent() < chance)
    {
        WAIT_STATE( ch,2*PULSE_VIOLENCE);
        strip( ch, victim );
        check_improve(ch,gsn_strip,TRUE,1);
    }
    else
    {
        WAIT_STATE(ch,2*PULSE_VIOLENCE);
        act("You fail to strip $N of $s weapon.",
        ch,NULL,victim,TO_CHAR);
        act("$n tries to strip you of your weapon, but fails.",
        ch,NULL,victim,TO_VICT);
        act("$n tries to strip $N of $s weapon, but fails.",
        ch,NULL,victim,TO_NOTVICT);
        check_improve(ch,gsn_strip,FALSE,1);
    }
    if (IS_AWAKE(victim) && !IS_NPC(victim) && (victim->fighting != ch))
    {
        sprintf(buf,"%s is trying to strip me of my weapon!",PERS(ch,victim) );
        do_myell( victim, buf );
        multi_hit( victim, ch, TYPE_UNDEFINED );
    }
    else if (!IS_AWAKE(victim) && !IS_NPC(victim))
    {
        sprintf(buf,"%s is trying to strip me of my weapon!",PERS(ch,victim) );
	victim->position=POS_STANDING;
        do_myell( victim, buf );
	one_hit( victim, ch, TYPE_UNDEFINED );
    }
    else if ( !IS_NPC(ch))
    {
        if ( IS_NPC(victim) && IS_AWAKE(victim))
        {
            multi_hit( victim, ch, TYPE_UNDEFINED );
        }
    }
    check_killer(ch,victim);
    return;
}

void do_choke( CHAR_DATA *ch,char *argument)
{
	AFFECT_DATA af;
	CHAR_DATA *victim;
	int chance;
	char arg[MAX_STRING_LENGTH];
	OBJ_DATA *wield;

	if ((chance = get_skill(ch,gsn_choke)) == 0 )
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}
	one_argument(argument,arg);

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
		if (victim->position == POS_SLEEPING)
		{
			send_to_char("But that person is already asleep!\n\r",ch);
			return;
		}
	}

	if (victim == ch)
	{
		send_to_char("You can't choke yourself.\n\r",ch);
		return;
	}

	wield = get_eq_char(ch,WEAR_WIELD);

	if(!wield)
		return send_to_char("You must be wielding a whip or flail to do that.\n\r",ch);	

	if((wield->value[0] != WEAPON_WHIP) && (wield->value[0] != WEAPON_FLAIL))
	{
		send_to_char("You must be wielding a whip or flail to do that.\n\r",ch);
		return;
	}

	if (is_safe(ch,victim))
	return;

	if (is_affected(victim,gsn_choke))
	{
		send_to_char("They are already choked.\n\r",ch);
		return;
	}

	chance += (ch->drain_level - victim->level);
	if (is_affected(victim,skill_lookup("haste")))
		chance -= 10;
	if (is_affected(ch,skill_lookup("haste")))
		chance += 10;

	chance -= 20;

	if (!(str_cmp(race_table[victim->race].name,"lich")))
	{
		chance = 0;
	}

	if (ch->level == MAX_LEVEL) {
		chance = 100;
	}

	if (IS_CABAL_OUTER_GUARD(victim))
		chance = 0;

	act("You lash out $p and wrap it around $N's neck!",ch,wield,victim,TO_CHAR);
	act("$n lashes out $p and wraps it around $N's neck!",ch,wield,victim,TO_NOTVICT);
	act("$n lashes out $p and wraps it around your neck!",ch,wield,victim,TO_VICT);

	if (number_percent() < chance)
	{
		act("You pass out from the choking hold of $p.",victim,wield,0,TO_CHAR);
		act("$n passes out from the choking hold of $p.",victim,wield,0,TO_ROOM);
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_SKILL;
		af.level = ch->level;
		af.location = 0;
		af.type = gsn_choke;
		af.duration = 2;
		af.bitvector = AFF_SLEEP;
		af.modifier = 0;
		af.affect_list_msg = str_dup("knocked unconscious");
		affect_to_char(victim,&af);
		stop_fighting(victim, TRUE);
		victim->position = POS_SLEEPING;
		check_improve(ch,gsn_choke,TRUE,2);
		WAIT_STATE(ch,2*PULSE_VIOLENCE);
		return;
	} else {
		damage(ch, victim, 0, gsn_choke, DAM_NONE,TRUE);
		check_improve(ch,gsn_choke,FALSE,3);
		WAIT_STATE(ch, 3*PULSE_VIOLENCE);
		WAIT_STATE(victim, PULSE_VIOLENCE);
	}
	return;
}

void do_overhead(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *victim;

        if (get_skill(ch,gsn_overhead) == 0)
        {
                send_to_char("Huh?\n\r",ch);
                return;
        }
        if ((victim = ch->fighting) == NULL) {
                send_to_char("You need to be fighting someone to overhead them.\n\r",ch);
                return;
        }
        if (victim == ch)
        {
                send_to_char("Wouldn't that hurt?\n\r",ch);
                return;
        }
        if (!is_wielded(ch,WEAPON_AXE,WIELD_ONE)) {
        send_to_char("You must be wielding an axe to do that.\n\r",ch);
        return;
        }
        if ( is_safe( ch, victim ) )
        return;
        act("You attempt to bring your axe down in a wild overhead swing!",ch,0,victim,TO_CHAR);
        act("$n attempts to bring $s axe down in a wild overhead swing!",ch,0,0,TO_ROOM);
        if(number_percent()>(get_skill(ch,gsn_overhead) - 10))
        {
                damage( ch, victim, 0, gsn_overhead,DAM_NONE,TRUE);
                check_improve(ch,gsn_overhead,FALSE,2);
                WAIT_STATE(ch,PULSE_VIOLENCE*2);
                return;
        }
        check_improve(ch,gsn_overhead,TRUE,2);
        one_hit_new(ch,victim,gsn_overhead,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,275,NULL);
        WAIT_STATE(ch,4*PULSE_VIOLENCE);
        return;
}

void do_bearcharge( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;
    char buf[MAX_STRING_LENGTH];

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_bearcharge)) == 0
    ||   (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_BASH) && ch->pIndexData->vnum!=MOB_VNUM_ENFORCER)
    ||   (!IS_NPC(ch)
    &&    ch->level < skill_table[gsn_bearcharge].skill_level[ch->class]))
    {
        send_to_char("You dont know how to Bear Charge.\n\r",ch);
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
        send_to_char("You cannot charge yourself.\n\r",ch);
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
	act("$N's charge seems to slide around you.",victim,0,ch,TO_CHAR);
	act("$N's charge seems to slide around $n.",victim,0,ch,TO_NOTVICT);
	act("Your charge seems to slide around $n.",victim,0,ch,TO_VICT);
	WAIT_STATE(ch,PULSE_VIOLENCE*.5);
	return;
    }

    if(is_affected(ch,gsn_shadowstrike)) {
	send_to_char("The shadowy arms restrict you.\n\r",ch);
	act("$N's tries to bash, but is restrained by shadowy arms!",ch,NULL,NULL,TO_ROOM);
	WAIT_STATE(ch,20);
	return;
    }

    /* modifiers */

    /* size  and weight */
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
    if (IS_SET(ch->off_flags,OFF_FAST) || is_affected(ch,skill_lookup("haste")))
        chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST) || is_affected(victim,skill_lookup("haste")))
        chance -= 40;

    /* level */
    chance += (ch->drain_level + ch->level - victim->level - victim->drain_level);
	if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (victim->fighting == NULL || ch->fighting == NULL))
	{
	sprintf(buf,"Help! %s is charging me like a bear!",PERS(ch,victim));
	do_myell(victim,buf);
	}


    if (!IS_NPC(victim)
        && chance < get_skill(victim,gsn_dodge) )
    {   /*
        act("$n tries to charge you, but you dodge it.",ch,NULL,victim,TO_VICT);
        act("$N dodges your charge, you fall flat on your face.",ch,NULL,victim,TO_CHAR);
        WAIT_STATE(ch,PULSE_VIOLENCE*1.5);
        return;*/
        chance -= 3 * (get_skill(victim,gsn_dodge) - chance);
    }

    /* now the attack */
    if (number_percent() < chance )
    {

        act("$n sends you to the ground with a powerful bear charge!",
                ch,NULL,victim,TO_VICT);
        act("You slam into $N, and send $M flying!",ch,NULL,victim,TO_CHAR);
        act("$n sends $N sprawling with a powerful bear charge.",
                ch,NULL,victim,TO_NOTVICT);
        check_improve(ch,gsn_bearcharge,TRUE,1);
        WAIT_STATE(victim, 2 * PULSE_VIOLENCE);
        damage_old(ch,victim,number_range(2,2 + 2 * ch->size + chance/20),gsn_bearcharge,
       DAM_BASH,TRUE);

        WAIT_STATE(ch,skill_table[gsn_bearcharge].beats);
        victim->position = POS_RESTING;
	if (!IS_NPC(ch) && (ch->class == CLASS_WARRIOR)
	&& (number_percent() < ch->level))
		ch->pcdata->sp++;
    }
    else
    {
        damage_old(ch,victim,0,gsn_bearcharge,DAM_BASH,FALSE);
        act("You tumble to the ground!",
            ch,NULL,victim,TO_CHAR);
        act("$n tumbles to the ground.",
            ch,NULL,victim,TO_NOTVICT);
        act("You evade $n's charge, causing $m to tumble to the ground.",
            ch,NULL,victim,TO_VICT);
        check_improve(ch,gsn_bearcharge,FALSE,1);
	check_cheap_shot(victim, ch);
        ch->position = POS_RESTING;
        WAIT_STATE(ch,skill_table[gsn_bearcharge].beats);
    }
        check_killer(ch,victim);
}

void do_whirl( CHAR_DATA *ch, char *argument)
{
        AFFECT_DATA af;
        CHAR_DATA *victim;
        char arg[MAX_INPUT_LENGTH];
        int chance;
	bool exeresult;
	int exedam;
        one_argument(argument,arg);
        if ((chance = get_skill(ch,gsn_whirl) ) == 0
        || (ch->level < skill_table[gsn_whirl].skill_level[ch->class]
        && !IS_NPC(ch)))
        {
        send_to_char("Huh??\n\r",ch);
        return;
        }
        if ( ( victim = ch->fighting ) == NULL )
        {
        send_to_char( "You aren't fighting anyone.\n\r", ch );
        return;
        }
	if (!is_wielded(ch,WEAPON_AXE,WIELD_ONE))
        {
        send_to_char("You must be wielding an axe to whirl.\n\r",ch);
        return;
        }
        if (is_safe(ch,victim) )
        return;
        chance += ch->carry_weight/25;
        chance -= victim->carry_weight/20;
        chance += (ch->size - victim->size)*20;
        chance -= get_curr_stat(victim,STAT_DEX);
        chance += get_curr_stat(ch,STAT_STR)/3;
        chance += get_curr_stat(ch,STAT_DEX)/2;
        if (IS_AFFECTED(ch,AFF_HASTE) )
                chance += 10;
        if (IS_AFFECTED(victim,AFF_HASTE) )
                chance -= 15;
        act("$n whirls $s axe in an attempt to strike $N with its points!",ch,0,victim,TO_NOTVICT);
        act("You whirl your axe in an attempt to strike $N with its points!",ch,0,victim,TO_CHAR);
        act("$n whirls $s axe in an attempt to strike you with its points!",ch,0,victim,TO_VICT);
        if (number_percent() < chance && !is_affected(victim,gsn_whirl))
        {
	exedam=GET_DAMROLL(ch)+200;
	exeresult = damage(ch, victim, exedam, gsn_whirl, DAM_SLASH, TRUE );
	init_affect(&af);
	af.aftype = AFT_SKILL;
	af.level = ch->level;
        af.type = gsn_whirl;
        af.modifier = -(ch->level/10);
        af.duration = 5;
        af.location = APPLY_DEX;
        af.bitvector = 0;
        affect_to_char(victim,&af);
        af.location = APPLY_STR;
        af.bitvector = 0;
        affect_to_char(victim,&af);
        check_improve(ch,gsn_whirl,TRUE,1);
        }
        else if (number_percent() <chance && is_affected(victim,gsn_whirl))
        {
	exedam=GET_DAMROLL(ch)+200;
	exeresult = damage(ch, victim, exedam, gsn_pincer, DAM_SLASH, TRUE );
        check_improve(ch,gsn_whirl,TRUE,1);
        }
        else
        {
	exedam=GET_DAMROLL(ch)+200;
	exeresult = damage(ch, victim, exedam, gsn_pincer, DAM_SLASH, TRUE );
        check_improve(ch,gsn_whirl,FALSE,1);
        }
        WAIT_STATE(ch,2*PULSE_VIOLENCE);
        return;
}

void do_crushing_blow(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim = ch->fighting;
	int chance;

	if (get_skill(ch,gsn_crushing_blow) == 0) {
		send_to_char("You don't know how to land crushing blows!\n\r",ch);
		return;
	}
	if (victim == NULL) {
		send_to_char("You must be in combat to land a crushing blow!\n\r",ch);
		return;
	}
        if (get_eq_char(ch,WEAR_SHIELD) != NULL || get_eq_char(ch,WEAR_HOLD) != NULL
        || get_eq_char(ch,WEAR_LIGHT) != NULL || get_eq_char(ch,WEAR_DUAL_WIELD) != NULL
        || get_eq_char(ch,WEAR_WIELD) != NULL)
	{
		send_to_char("You need your hands free to be able to land a crushing blow!\n\r",ch);
		return;
	}
	chance = get_skill(ch,gsn_crushing_blow);
	chance -= (get_curr_stat(victim,STAT_DEX) + get_curr_stat(victim,STAT_STR))*2 - (get_curr_stat(ch,STAT_STR) + get_curr_stat(ch,STAT_DEX))*2;

	if (number_percent() > chance) {
		act("$n laces $s fingers together to jab at $N but misses.",ch,0,victim,TO_NOTVICT);
		act("$n laces $s fingers together to jab at you but misses.",ch,0,victim,TO_VICT);
		act("You lace your fingers together to jab at $N but miss.",ch,0,victim,TO_CHAR);
		damage_old(ch,victim,0,gsn_crushing_blow,DAM_BASH,TRUE);
		check_improve(ch,gsn_crushing_blow,FALSE,2);
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
	} else {
		act("$n laces $s fingers together and hammers $s fists at $N!",ch,0,victim,TO_NOTVICT);
		act("$n laces $s fingers together and hammers $s fists at you!",ch,0,victim,TO_VICT);
		act("You lace your fingers together and hammer your fists at $N!",ch,0,victim,TO_CHAR);
		WAIT_STATE(ch,PULSE_VIOLENCE*2);

		if (number_percent() > 60 )
		{
			act("$n crushing blow knocks $N down!",ch,0,victim,TO_NOTVICT);
			act("Your crushing blow knocks $N down!",ch,0,victim,TO_CHAR);
			act("$n crushing blow knocks you down!",ch,0,victim,TO_VICT);
			WAIT_STATE(victim,PULSE_VIOLENCE*2);
			WAIT_STATE(ch,PULSE_VIOLENCE*2);
		}
		one_hit_new(ch,victim,gsn_crushing_blow,HIT_SPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,150,NULL);
		check_improve(ch,gsn_crushing_blow,TRUE,1);
	}
	return;
}

void do_findwood(CHAR_DATA *ch,char *argument)
{
        OBJ_DATA *maple;
        OBJ_DATA *magic;
        OBJ_DATA *oak;
        char arg[MAX_STRING_LENGTH];
        int chance;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_fwood)) == 0
    || (ch->level < skill_table[gsn_fwood].skill_level[ch->class]) )
    {
        send_to_char("Huh?\n\r",ch);
		return;
    }

    if (ch->in_room->sector_type != SECT_FOREST)
    {
    send_to_char("Does it look like theres wood here?\n\r",ch);
    return;
    }

    if (arg[0] == '\0')
    {
		send_to_char("You can find: Maple, Magic or Oak.\n\r",ch);
		return;
	}

    if (chance > 90)
        chance = 90;

    if (!str_cmp(arg,"maple") && (number_percent() < chance))
    {
    	act("$n makes some noise and comes up with a large log.",ch,0,0,TO_ROOM);
    	send_to_char("You search about and find yourself a large log.\n\r",ch);
    	check_improve(ch,gsn_fwood,TRUE,2);
		maple = create_object(get_obj_index(OBJ_VNUM_WOOD_MAPLE),1);
		obj_to_char(maple,ch);
		return;
	}
    else if (!str_cmp(arg,"magic") && (number_percent() < chance))
	{
	    act("$n makes some noise and comes up with a large log.",ch,0,0,TO_ROOM);
	    send_to_char("You search about and find yourself a large log.\n\r",ch);
	    check_improve(ch,gsn_fwood,TRUE,2);
		magic = create_object(get_obj_index(OBJ_VNUM_WOOD_MAGIC),1);
		obj_to_char(magic,ch);
		return;
	}
	else if (!str_cmp(arg,"oak") && (number_percent() < chance))
	{
	    act("$n makes some noise and comes up with a large log.",ch,0,0,TO_ROOM);
	    send_to_char("You search about and find yourself a large log.\n\r",ch);
	    check_improve(ch,gsn_fwood,TRUE,2);
		oak = create_object(get_obj_index(OBJ_VNUM_WOOD_OAK),1);
		obj_to_char(oak,ch);
		return;
	}
	else
	{
		send_to_char("You search about but fail to find that.\n\r",ch);
		return;
	}
}


void do_carve(CHAR_DATA *ch,char *argument)
{
        OBJ_DATA *maple_staff;
        OBJ_DATA *oak_staff;
        OBJ_DATA *magic_staff;
        OBJ_DATA *wood;
        char arg[MAX_STRING_LENGTH];
		int chance, number, dice;

	  one_argument(argument,arg);

  	if ( (chance = get_skill(ch,gsn_carve)) == 0
  	|| (ch->level < skill_table[gsn_carve].skill_level[ch->class]) )
  	{
			send_to_char("Huh?\n\r",ch);
			return;
	}

	if (arg[0] == '\0')
	{
			send_to_char("You must carve either a chunk of maple, oak or magic wood.\n\r",ch);
			return;
	}

	if (chance > 90)
	chance = 90;

	wood =  get_obj_list(ch,argument,ch->carrying);

	if (wood == NULL)
	{
		send_to_char("You aren't carrying that.\n\r",ch);
		return;
	}

	if (number_percent() > chance)
	{
		send_to_char("You fail and destroy the wood!\n\r",ch);
		obj_from_char(wood);
		return;
	}

	if (!str_cmp(arg,"maple"))
	{
	   	if (wood->pIndexData->vnum != OBJ_VNUM_WOOD_MAPLE)
	   	{
			send_to_char("You can't carve that!\n\r",ch);
			return;
		}
	   	act("$n carves a beautiful staff out of wood.",ch,0,0,TO_ROOM);
	   	send_to_char("You make yourself a beautiful staff out of wood.\n\r",ch);
	   	check_improve(ch,gsn_fwood,TRUE,2);
		maple_staff = create_object(get_obj_index(OBJ_VNUM_RANGER_MAPLE),1);
		number = 6;     dice = 9;
		maple_staff->level = ch->level;
		maple_staff->value[1] = number;
        maple_staff->value[2] = dice;
		obj_to_char(maple_staff,ch);
		obj_from_char(wood);
		return;
	}
	else if (!str_cmp(arg,"magic"))
	{
	   	if (wood->pIndexData->vnum != OBJ_VNUM_WOOD_MAGIC)
	   	{
			send_to_char("You can't carve that!\n\r",ch);
			return;
		}
	   	act("$n carves a beautiful staff out of wood.",ch,0,0,TO_ROOM);
	   	send_to_char("You make yourself a beautiful staff out of wood.\n\r",ch);
	   	check_improve(ch,gsn_fwood,TRUE,2);
		magic_staff = create_object(get_obj_index(OBJ_VNUM_RANGER_MAGIC),1);
		number = 8;     dice = 7;
		magic_staff->level = ch->level;
		magic_staff->value[1] = number;
	    magic_staff->value[2] = dice;
		obj_to_char(magic_staff,ch);
		obj_from_char(wood);
		return;
	}
	else if (!str_cmp(arg,"oak"))
	{
	   	if (wood->pIndexData->vnum != OBJ_VNUM_WOOD_OAK)
	   	{
			send_to_char("You can't carve that!\n\r",ch);
			return;
		}
	   	act("$n carves a beautiful staff out of wood.",ch,0,0,TO_ROOM);
	   	send_to_char("You make yourself a beautiful staff out of wood.\n\r",ch);
	   	check_improve(ch,gsn_fwood,TRUE,2);
		oak_staff = create_object(get_obj_index(OBJ_VNUM_RANGER_OAK),1);
		number = 8;     dice = 7;
		oak_staff->level = ch->level;
		oak_staff->value[1] = number;
	    oak_staff->value[2] = dice;
		obj_to_char(oak_staff,ch);
		obj_from_char(wood);
		return;
	}
	else
	{
		send_to_char("You must either choose: oak, maple or magic.\n\r",ch);
		return;
	}
}

void do_dash(CHAR_DATA *ch, char *argument)
{
	int where,skill;
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	char command[MSL],dir[MSL], word[MSL];

 	/* Must have the skill to use it */
 	if ((skill = get_skill(ch,gsn_dash)) == 0)
		return send_to_char("Huh?\n\r",ch);

 	/* break up the argument into two variables */
	if(!*argument)
 		return send_to_char("Dash where and do what?\n\r", ch);

 	/* Check to see if they are fighting */
	if(ch->fighting)
  		return send_to_char("You can't dash while you are fighting!\n\r",ch);

 	half_chop(argument, dir, command);

 	if(!*command)
  		return send_to_char("You must specify an action when you dash.\n\r", ch);

 	if(!*dir)
  		return send_to_char("Dash which direction?\n\r", ch);
	one_argument(command, word);

	if(!str_prefix(word,"dash") || !str_prefix(word,"east") || !str_prefix(word,"north")
	   || !str_prefix(word,"west") || !str_prefix(word,"south") || !str_prefix(word,"up")
	   || !str_prefix(word,"down"))
		return send_to_char("You can't do that.\n\r",ch);

 	/* Check to see if the direction verb is valid */
 	if((where = direction_lookup(dir)) == -1)
		return send_to_char("That is not a direction!\n\r",ch);

 	/* Get the default word for the direction */
        sprintf(dir,"%s",(char*)flag_name_lookup(reverse_d(where),direction_table));

 	/* Make sure the exit is valid */
	if ((pexit = ch->in_room->exit[where]) == NULL)
  		return send_to_char("You cannot dash in that direction.\n\r",ch);

 	/* Set the direction room */
	to_room = pexit->u1.to_room;

	if (number_percent() < skill)
	{
		/* stop and move driver */
		move_char(ch,where,TRUE);
		if(ch->in_room != to_room)
			return;
		act("$n dashes in from the $t!",ch,dir,0,TO_ROOM);
		interpret( ch, command );
		check_improve(ch,gsn_dash,TRUE,2);
 	}
 	else
 	{
		send_to_char("You fail to prepare for your dash properly, and manage only to stumble in!\n\r",ch);
		/* stop and move driver */
		move_char(ch,where,TRUE);
		act("$n stumbles in from the $t!",ch,dir,0,TO_ROOM);
		check_improve(ch,gsn_dash,FALSE,2);
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
 	}
}

void check_restrike(CHAR_DATA *ch,CHAR_DATA *victim) 
{
        int chance;
        OBJ_DATA *wield;
        char buf1[MAX_STRING_LENGTH];
        char buf2[MAX_STRING_LENGTH];
                
	if ((chance = get_skill(ch,gsn_restrike)) == 0)
                return;

        wield = get_eq_char(ch,WEAR_WIELD);
	if (wield == NULL)
		return;
	if (wield->value[0] != WEAPON_DAGGER) {
		chance -= 30;
	        wield = get_eq_char(ch,WEAR_DUAL_WIELD);
	}
	if (wield == NULL)
		return;
	if (wield->value[0] != WEAPON_DAGGER)
		return;

	int dex = get_curr_stat(victim,STAT_DEX);
	
	if (dex <= 5)
	chance += 0;
    else if (dex <= 10)
	chance += dex/5;
    else if (dex <=15)
	chance += (dex/4);
    else if (dex <= 20)
	chance += (dex/3);
    else
	chance += dex/2;

 	chance /= 3;
	if (IS_AFFECTED(ch,AFF_HASTE))
		chance += 10;
	if (IS_AFFECTED(ch,AFF_SLOW))
		chance -= 20;

        chance = URANGE(1,chance,75);

        if (number_percent() < chance) 
        {
	sprintf(buf1,"You rapidly redirect your dagger midway through $N's parry.");
	sprintf(buf2,"$n rapidly redirects $s dagger midway through your parry.");
	act(buf1,ch,0,victim,TO_CHAR);
	act(buf2,ch,0,victim,TO_VICT);
	one_hit_new(ch,victim,gsn_restrike,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
        check_improve(ch,gsn_restrike,TRUE,1);
        return;
        }
        check_improve(ch,gsn_restrike,FALSE,3);
        return;
}

void do_crossslice(CHAR_DATA *ch,char *argument) 
{
    CHAR_DATA *victim;
    int chance;
    OBJ_DATA *wield;
    OBJ_DATA *dwield;
    AFFECT_DATA af;

    if ((chance = get_skill(ch,gsn_cross_slice) ) == 0) 
	return send_to_char("Huh?\n\r",ch);
 
    if ((victim = ch->fighting) == NULL)
	return send_to_char("But you aren't fighting anyone!\n\r",ch);

    wield = get_eq_char(ch,WEAR_WIELD);
    dwield = get_eq_char(ch,WEAR_DUAL_WIELD);

    if (wield == NULL || dwield == NULL) 
  	return send_to_char("You must be dual wielding swords to cross-slice.\n\r",ch);

    if (wield->value[0] != WEAPON_SWORD || dwield->value[0] != WEAPON_SWORD) 
  	return send_to_char("You must be dual wielding swords to cross-slice.\n\r",ch);

    chance /= 2;
    chance += get_curr_stat(ch,STAT_STR)/2;
    chance += get_curr_stat(ch,STAT_DEX)/3;

    if (IS_AFFECTED(ch,AFF_HASTE))
	chance += 5;
  
    if (wield->pIndexData->vnum == 5704) 
	chance += 15;
  
    if (dwield->pIndexData->vnum == 5704)
	chance += 5;

    chance += ch->level;
    chance = URANGE(5,chance,95);

    if (number_percent() >= chance) 
    {
  	act("$n tries to do something fancy with $s swords but fails.",ch,0,0,TO_ROOM);
  	send_to_char("You mix up your swords and fail to cross them properly.\n\r",ch);

  	WAIT_STATE(ch,12);
  	check_improve(ch,gsn_cross_slice,FALSE,2);
  	return;
    }

    act("$n slices $s swords across $N in a smooth scissor motion.",ch,0,victim,TO_NOTVICT);
    act("You slice your swords across $N in a smooth scissor motion.",ch,0,victim,TO_CHAR);
    act("$n slices $s swords across you in a smooth scissor motion.",ch,0,victim,TO_VICT);
  
    one_hit_new(ch,victim,gsn_cross_slice,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
    one_hit_new(ch,victim,gsn_cross_slice,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);

    if (!is_affected(victim,gsn_cross_slice)) 
    {
	if (number_percent() < chance/2)
	{
	    act("A gaping wound is torn across $n's body!",victim,0,0,TO_ROOM);
	    act("A gaping wound is torn across your body!",victim,0,0,TO_CHAR);
	    init_affect(&af);
	    af.where 		= TO_AFFECTS;
	    af.aftype 		= AFT_SKILL;
	    af.type 		= gsn_cross_slice;
	    af.duration 	= 2;
	    af.level 		= ch->level;
	    af.modifier 	= -3;
	    af.bitvector 	= 0;
	    af.location 	= APPLY_STR;
		af.owner_name		= str_dup(ch->original_name);
	    affect_to_char(victim,&af);
	}
    }
	
    check_improve(ch,gsn_cross_slice,TRUE,2);
    WAIT_STATE(ch,24);

    return;
}
