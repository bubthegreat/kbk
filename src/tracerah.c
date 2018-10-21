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

void printCabalInfo();

CHAR_DATA *get_cabal_guardian(int cabal)
{
	CHAR_DATA *cabalguardian;
	for(cabalguardian=char_list;cabalguardian!=NULL;cabalguardian=cabalguardian->next)
		if(IS_NPC(cabalguardian) && cabalguardian->cabal==cabal && IS_CABAL_GUARD(cabalguardian))
			return cabalguardian;
	return NULL;
}

void do_cranial( CHAR_DATA *ch,char *argument)
{
        char arg[MAX_INPUT_LENGTH];
        AFFECT_DATA af;
        CHAR_DATA *victim;
        int chance,size;
        char buf[MAX_STRING_LENGTH];
	int wtime;
	OBJ_DATA *mace;

        one_argument(argument,arg);
        if ((chance = get_skill(ch,gsn_cranial)) == 0)
        {
        send_to_char("You don't know how to do that.\n\r",ch);
        return;
        }
        else if ((victim = get_char_room(ch,arg)) == NULL)
        {
        send_to_char("They aren't here.\n\r",ch);
        return;
        }
	if (!is_wielded(ch,WEAPON_MACE,WIELD_ONE)) {
        send_to_char("You must be wielding an mace to do that.\n\r",ch);
        return;
        }
	mace = get_eq_char(ch,WEAR_WIELD);
        if ((victim = get_char_room(ch,arg)) == NULL)
        {
        send_to_char("They aren't here.\n\r",ch);
        return;
        }
        if (arg[0] == '\0')
        {
                victim = ch->fighting;
                if (victim == NULL)
                {
                send_to_char("Who are you trying to do that to?\n\r",ch);
                return;
                }
        }
        if (victim == ch)
        {
        send_to_char("You cannot cranial yourself.\n\r",ch);
        return;
        }
	if (is_affected(victim,gsn_cranial)) {
        send_to_char("They are already dazed with a strike to the head.\n\r",ch);
        return;
	}
        if (is_safe(ch,victim))
        return;
        size=victim->size-ch->size;
        if(size>2)
        {
            send_to_char("That person is too large for you to reach their head with your mace.\n\r",ch);
            return;
        }
        if (victim->position == POS_SLEEPING)
        {
        send_to_char("But that person is already asleep!\n\r",ch);
        return;
        }
        chance *= 40;
        chance /= 100;
        chance += (ch->level - victim->level);
        if(size>0)
                chance -= 20*size;
        if (IS_AFFECTED(victim,AFF_HASTE))
                chance -= 20;
        if (IS_AFFECTED(ch,AFF_HASTE))
                chance += 10;
        if (get_eq_char(victim,WEAR_HEAD) != NULL)
                chance -= 20;
	if (IS_CABAL_OUTER_GUARD(victim) || IS_CABAL_GUARD(victim))
		chance = 0;
	chance -= 15;
        init_affect(&af);
        af.where = TO_AFFECTS;
        af.level = ch->level + victim->level;
        af.location = 0;
        af.aftype = AFT_SKILL;
        af.type = gsn_cranial;
        af.modifier = 0;
		char msg_buf[MSL];
		sprintf(msg_buf,"knocked unconscious");
		af.affect_list_msg = str_dup(msg_buf);
        af.bitvector = AFF_SLEEP;
	if(!IS_NPC(ch) && get_trust(ch) ==MAX_LEVEL)
		chance=101;
        if (number_percent() < chance)
        {
	update_pc_last_fight(ch,victim);
        act("You strike $N over the head with $p.",ch,mace,victim,TO_CHAR);
        act("$n strikes $N over the head with $p.",ch,mace,victim,TO_NOTVICT);
        send_to_char("You feel a crack in the back of your head, followed by a thud as you hit the ground.\n\r",victim);
        af.duration=2;
        affect_to_char(victim,&af);
        stop_fighting( victim, TRUE);
        victim->position = POS_SLEEPING;
        check_improve(ch,gsn_cranial,TRUE,1);
        WAIT_STATE(ch,2*PULSE_VIOLENCE);
        return;
        }
        else
        {
        act("You strike $N over the head with $p.",ch,mace,victim,TO_CHAR);
        act("$n strikes $N over the head with $p.",ch,mace,victim,TO_NOTVICT);
        act("$n strikes you over the head with $p.",ch,mace,victim,TO_VICT);
        check_improve(ch,gsn_cranial,FALSE,1);
        damage_old(ch,victim,2 + 8*number_range(2,5),gsn_cranial,DAM_BASH,TRUE);
        WAIT_STATE(ch, 2*PULSE_VIOLENCE);
	if((wtime=victim->size-ch->size)>0)
		WAIT_STATE(victim,1*PULSE_VIOLENCE);
	if(wtime==0)
		WAIT_STATE(victim,1.7*PULSE_VIOLENCE);
	if(wtime<0)
		WAIT_STATE(victim,2*PULSE_VIOLENCE);
          if (!IS_NPC(victim) && (victim->fighting != ch))
           {
                sprintf(buf, "Help! %s hit me over the head!",ch->name);
                do_myell(victim,buf);
                multi_hit(ch,victim,TYPE_UNDEFINED);
           }
        }
        return;
}

void do_entwine(CHAR_DATA *ch,char *argument)
{
    	char arg[MAX_INPUT_LENGTH];
    	AFFECT_DATA af;
    	CHAR_DATA *victim=NULL;
    	OBJ_DATA *wield;
    	int chance;

    	one_argument(argument,arg);
 
    	if ( (chance = get_skill(ch,gsn_entwine)) == 0)
    	{   
        	send_to_char("Huh?\n\r",ch);
        	return;
    	}

	wield = get_eq_char(ch, WEAR_WIELD);
	if (!wield)
		return send_to_char("You need a whip or flail to entwine with.\n\r", ch);

	if (wield->value[0] != WEAPON_WHIP && wield->value[0] != WEAPON_FLAIL)
		return send_to_char("You need a whip or flail to entwine with.\n\r", ch);

    	if (arg[0] == '\0')
    	{
        	victim = ch->fighting;
        	if (victim == NULL)
    		{
        		send_to_char("But you aren't fighting anyone!\n\r",ch);
            		return;
    		}
    	}
	else
		victim = ch->fighting;

	if (!victim)
		return send_to_char("But you aren't fighting anyone!\n\r",ch);

    	if (ch->fighting != victim)
        {
        	send_to_char("You aren't fighting them.\n\r",ch);
          	return;
        }

        if (is_affected(victim,gsn_entwine))
        {
          	send_to_char("Their legs are already entwined.\n\r",ch);
          	return;
        }

    	if (is_safe(ch,victim))
        	return;

    	if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
        	chance += 5;
    	
	if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
        	chance -= 15;
    
	chance += get_curr_stat(ch,STAT_DEX)/2;
    	
	chance -= get_curr_stat(victim,STAT_DEX)/2;
    
	if (IS_AFFECTED(victim,AFF_FLYING))
        	chance -= dice(3,6);

    	/* level */
    	wield=get_eq_char(ch,WEAR_WIELD); 
    	chance += (ch->level - victim->level)*3;

	init_affect(&af);
        af.where 	= TO_AFFECTS;
        af.level 	= ch->level + victim->drain_level;
        af.location 	= 0;
        af.type 	= gsn_entwine;
        af.aftype 	= AFT_SKILL;
        af.modifier 	= 0;
        af.bitvector 	= 0;
		char msg_buf[MSL];
		sprintf(msg_buf,"unable to flee");
		af.affect_list_msg = str_dup(msg_buf);

        if (number_percent() > chance)
        {
        	act("$n lashes at $N's legs in an attempt to entwine $M, but fails.",ch,0,victim,TO_NOTVICT);
        	act("$n lashes at your legs in an attempt to entwine you, but fails.",ch,0,victim,TO_VICT);
        	act("You lash at $N's legs in an attempt to entwine $M, but fails.",ch,0,victim,TO_CHAR);
        	check_improve(ch,gsn_entwine,FALSE,1);
        	WAIT_STATE(ch,2 * PULSE_VIOLENCE);
          	return;
        }

        act("$n wraps $p around $N's legs, entwining $M tightly!",ch,wield,victim,TO_NOTVICT);
        act("$n wraps $p around your legs, entwining you tightly!",ch,wield,victim,TO_VICT);
        act("You wrap $p around $N's legs, entwining $M tightly!",ch,wield,victim,TO_CHAR);
        af.duration=1;
        affect_to_char(victim,&af);
        check_improve(ch,gsn_entwine,TRUE,1);
        WAIT_STATE(ch,2 * PULSE_VIOLENCE);
        return;
}

void do_legsweep( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;
    char buf[MAX_STRING_LENGTH];

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_legsweep)) == 0
    ||   (!IS_NPC(ch)
          && ch->level < skill_table[gsn_legsweep].skill_level[ch->class]))
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }


    if (arg[0] == '\0')
    {
        if ((victim = ch->fighting) == NULL)
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
	if (!is_wielded(ch,WEAPON_STAFF,WIELD_PRIMARY) &&
		!is_wielded(ch,WEAPON_SPEAR,WIELD_PRIMARY) &&
		!is_wielded(ch,WEAPON_POLEARM,WIELD_PRIMARY)) {
        send_to_char("You can't do that with your weapon.\n\r",ch);
        return;
        }
    if (is_safe(ch,victim))
        return;
    if (victim->position < POS_FIGHTING)
    {
        act("$N is already down.",ch,NULL,victim,TO_CHAR);
        return;
    }
    if (victim == ch)
    {
        send_to_char("You can't do that!\n\r",ch);
        return;
    }
    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
        act("$N is your beloved master.",ch,NULL,victim,TO_CHAR);
        return;
    }
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
        chance -= 20;
    /* level */
    chance += (ch->drain_level + ch->level - victim->level - victim->drain_level) * 2;
    /* now the attack */
    if (number_percent() < chance)
    {
        act("$n legsweeps you and you hit the ground hard!",ch,NULL,victim,TO_VICT);
        act("You legsweep $N and $E hits the ground hard!",ch,NULL,victim,TO_CHAR);
        act("$n legsweeps $N and $E hits the ground hard!",ch,NULL,victim,TO_NOTVICT);
        check_improve(ch,gsn_legsweep,TRUE,1);

        if (!IS_NPC(ch) && !IS_NPC(victim)
        && (ch->fighting == NULL || victim->fighting == NULL))
        {
        sprintf(buf,"Help! %s is legsweeping me!",PERS(ch,victim));
        do_myell(victim,buf);
        }
    if (IS_AFFECTED(victim,AFF_FLYING)
        && !is_affected(victim,gsn_earthbind)) {
        WAIT_STATE(victim,PULSE_VIOLENCE);
	} else {
        WAIT_STATE(victim,1.75 * PULSE_VIOLENCE);
	}
        WAIT_STATE(ch,2 * PULSE_VIOLENCE);
        victim->position = POS_RESTING;
        damage(ch,victim,number_range(2, 2 +  10 * victim->size),gsn_legsweep, DAM_BASH,TRUE);
        check_downstrike(ch,victim);
      check_cheap_shot(ch,victim);
    }
    else
    {
        if (!IS_NPC(ch) && !IS_NPC(victim)
        && (ch->fighting == NULL || victim->fighting == NULL))
        {
        sprintf(buf,"Help! %s is trying to legsweep me!",PERS(ch,victim));
        do_myell(victim,buf);
        }
        damage(ch,victim,0,gsn_legsweep,DAM_BASH,TRUE);
        WAIT_STATE(ch,1 * PULSE_VIOLENCE);
        check_improve(ch,gsn_legsweep,FALSE,1);
    }
        check_killer(ch,victim);
}

void do_rising_phoenix_kick( CHAR_DATA *ch, char *argument )
{
    	CHAR_DATA *victim;
	int dam, chance;

    	if (IS_NPC(ch))
			return;

        if ((chance = get_skill(ch,gsn_rising_pheonix_kick)) == 0)
        {
                send_to_char("Huh?\n\r",ch);
                return;
        }

    	if ( ( victim = ch->fighting ) == NULL )
    	{
		send_to_char( "You aren't fighting anyone.\n\r", ch );
		return;
    	}

    	if (is_affected(victim,AFF_HASTE))
		chance /= 2;

    	if (is_affected(ch,AFF_BLIND))
		chance /= 2;

	// Copied Graelik's damage setup to keep the Assassin kicks sort of unified
	// and based on the same math. Hope ya don't mind. 
	dam = (ch->drain_level + ch->level)/2;
	dam += number_range(0,(ch->drain_level + ch->level)/6);
	dam += number_range(0,(ch->drain_level + ch->level)/6);
	dam += number_range(0,(ch->drain_level + ch->level)/6);
	dam += number_range(0,(ch->drain_level + ch->level)/6);
	dam += number_range(0,(ch->drain_level + ch->level)/6);
	dam += number_range(0,(ch->drain_level + ch->level)/6);
	dam += number_range(ch->level/5,ch->level/4);
	dam *= 2;

    	for ( victim = ch->in_room->people; victim != NULL; victim = victim->next_in_room )
    	{
    		if (victim->fighting != ch && ch->fighting != victim)
			continue;

    		if (chance > number_percent())
    		{
                act("With a powerful leap, you execute the rising pheonix kick on $M!",ch,NULL,victim,TO_CHAR);
                act("With a powerful leap, $n executes the rising pheonix kick on you!",ch,NULL,victim,TO_VICT);
                act("With a powerful leap, $n executes the rising pheonix kick on $M!",ch,NULL,victim,TO_NOTVICT);
				damage(ch,victim,dam, gsn_rising_pheonix_kick,DAM_BASH,TRUE);
				WAIT_STATE(victim, 2 * PULSE_VIOLENCE);
				check_improve(ch,gsn_rising_pheonix_kick,TRUE,1);
				check_follow_through(ch,victim,dam);
				continue;
    		}
		else
		{
                	act("$M narrowly avoids your rising phoenix kick!",ch,NULL,victim,TO_CHAR);
                	act("You narrowly avoid $n's rising phoenix kick!",ch,NULL,victim,TO_VICT);
                	act("$N narrowly avoids $n's rising phoenix kick!",ch,NULL,victim,TO_NOTVICT);
			damage(ch,victim,0, gsn_rising_pheonix_kick,DAM_BASH,TRUE);
			check_improve(ch,gsn_rising_pheonix_kick,TRUE,1);
			continue;
		}
		

	}
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
    	return;
}

void do_double_spin_kick(CHAR_DATA *ch,char *argument)
{
        CHAR_DATA *victim;
        int dam, chance;
        bool again = FALSE;

        if ((chance = get_skill(ch,gsn_double_spin_kick)) == 0)
        {
                send_to_char("Huh?\n\r",ch);
                return;
        }

        if ((victim = ch->fighting) == NULL)
                return send_to_char("You aren't fighting anyone.\n\r",ch);

    	chance += (ch->drain_level + ch->level - victim->level - victim->drain_level);
    	chance = URANGE(5, chance, 95);

	dam = 25+(chance + GET_DAMROLL(ch))/1.5;

	if(IS_AFFECTED(ch,AFF_HASTE))
		chance+=20;

	if(IS_AFFECTED(victim,AFF_HASTE))
		chance-=20;

    	if (is_affected(ch,AFF_BLIND))
        	chance /= 2;

	if (number_percent() < chance - 10)
	{
		act("With a quick hop, you spin your leg around and smash $N square in the face!",ch,0,victim,TO_CHAR);
		act("With a quick hop, $n spins $s leg around and smashes $N square in the face!",ch,0,victim,TO_NOTVICT);
		act("With a quick hop, $n spins $s leg around and smashes you square in the face!",ch,0,victim,TO_VICT);
        	damage(ch, victim, dam, gsn_double_spin_kick, DAM_BASH,TRUE);
        	check_improve(ch,gsn_double_spin_kick,TRUE,2);
			check_follow_through(ch,victim,dam);
		again = TRUE;
	}
	else
	{
        	damage(ch, victim, 0, gsn_double_spin_kick, DAM_SLASH,TRUE);
        	check_improve(ch,gsn_double_spin_kick,TRUE,2);
		WAIT_STATE(ch,PULSE_VIOLENCE*1);
		return;
	}	

	if (is_affected(ch,AFF_BLIND))	
		again = FALSE;
	
	if (number_percent() < (chance - 10) && again)
    	{
		act("Seizing upon natural rhythm, you spin around again and kick $N across the jaw!",ch,0,victim,TO_CHAR);
		act("Seizing upon $s natural rhythm, $n spins around again and kicks $N across the jaw!",ch,0,victim,TO_NOTVICT);
		act("Seizing upon $s natural rhythm, $n spins around again and kicks you across the jaw!",ch,0,victim,TO_VICT);
        	damage(ch, victim, dam, gsn_double_spin_kick, DAM_BASH,TRUE);
			check_follow_through(ch,victim,dam);
        	WAIT_STATE(victim, 1 * PULSE_VIOLENCE);
		check_improve(ch,gsn_double_spin_kick,TRUE,2);
	}

        WAIT_STATE(ch,PULSE_VIOLENCE*2);
        return;
}

void do_charge(CHAR_DATA *ch,char *argument)
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	char buf[MAX_STRING_LENGTH];
	one_argument(argument,arg);
	if (get_skill(ch,gsn_charge) == 0)
	{
		send_to_char("Huh?\n\r",ch);
		return;
	}
	if (ch->fighting) {
		send_to_char("You can't charge while fighting.n\r",ch);
		return;
	}

	if (arg[0] == '\0')
	{
		send_to_char("Charge into who?\n\r",ch);
		return;
	}
	if ((victim = get_char_room(ch,arg)) == NULL)
	{
		send_to_char("They aren't here.\n\r",ch);
		return;
	}
	if (victim == ch)
	{
		send_to_char("Wouldn't that hurt?\n\r",ch);
		return;
	}
	if (!is_wielded(ch,WEAPON_POLEARM,WIELD_PRIMARY)) {
        send_to_char("You must be wielding a polearm to do that.\n\r",ch);
        return;
        }     
	if ( is_safe( ch, victim ) )
	return;

        if (victim->hit < victim->max_hit*9/10)
        {
        	send_to_char("They are too hurt to attempt charge.\n\r",ch);
        	return;
        }

	if (check_chargeset(ch,victim)) {return;}

	act("You lower your weapon and charge $N!",ch,0,victim,TO_CHAR);
	act("$n lowers $s weapon and charges $N!",ch,0,victim,TO_NOTVICT);
	act("$n lowers $s weapon and charges you!",ch,0,victim,TO_VICT);

	if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (victim->fighting == NULL || ch->fighting == NULL))
	{
		sprintf(buf,"Help! %s is charging into me!",PERS(ch,victim));
		do_myell(victim,buf);
	}

	if(number_percent()>(get_skill(ch,gsn_charge)))
	{

		damage( ch, victim, 0, gsn_charge,DAM_NONE,TRUE);
		check_improve(ch,gsn_charge,FALSE,2);
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
		return;
	}
	one_hit_new(ch,victim,gsn_charge,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,250,NULL);
	check_improve(ch,gsn_charge,TRUE,2);
	WAIT_STATE(ch,2*PULSE_VIOLENCE);

	return;
}

bool can_live_in(CHAR_DATA *ch,int hometown)
{
	if (hometown > MAX_HOMETOWN || hometown <= 0)
		return FALSE;

	if ((hometown_table[hometown].align == ALIGN_NONE)
	|| (hometown_table[hometown].align == ALIGN_GN && IS_EVIL(ch))
	|| (hometown_table[hometown].align == ALIGN_NE && IS_GOOD(ch))
	|| (hometown_table[hometown].align == ALIGN_G && !IS_GOOD(ch))
	|| (hometown_table[hometown].align == ALIGN_N && !IS_NEUTRAL(ch))
	|| (hometown_table[hometown].align == ALIGN_E && !IS_EVIL(ch))
	|| (hometown_table[hometown].align == ALIGN_GE && IS_NEUTRAL(ch)))
		return FALSE;

	if ((hometown_table[hometown].ethos == ETHOS_NONE)
	|| (hometown_table[hometown].ethos == ETHOS_LN && IS_CHAOTIC(ch))
	|| (hometown_table[hometown].ethos == ETHOS_NC && IS_LAWFUL(ch))
	|| (hometown_table[hometown].ethos == ETHOS_L && !IS_LAWFUL(ch))
	|| (hometown_table[hometown].ethos == ETHOS_N && !IS_ENEUTRAL(ch))
	|| (hometown_table[hometown].ethos == ETHOS_C && !IS_CHAOTIC(ch))
	|| (hometown_table[hometown].ethos == ETHOS_LC && IS_ENEUTRAL(ch)))
		return FALSE;

	return TRUE;
}

void ADD_WAIT_STATE(CHAR_DATA *ch, int npulse)
{
	if (!ch)
		return;

	WAIT_STATE(ch,ch->wait + npulse);
	return;
}

void WAIT_STATE(CHAR_DATA *ch, int npulse)
{
	int wait = npulse;

	if (!ch)
		return;

	if(get_trust(ch)==MAX_LEVEL)
		wait = 0;
	ch->wait = UMAX(ch->wait,wait);
}

bool check_counterspell(CHAR_DATA *ch, CHAR_DATA *victim)
{
        int skill, chance;

        if(!ch || !victim)
                return FALSE;
        if(!(skill=get_skill(victim,gsn_counterspell)) || victim->fighting || !IS_AWAKE(victim) || victim==ch)
                return FALSE;
        if(get_trust(victim) == MAX_LEVEL)
                return TRUE;
        chance = skill * .6;
        chance -= get_curr_stat(ch,STAT_INT);
        chance += get_curr_stat(victim,STAT_INT);
        if(number_percent()<chance)
                return TRUE;
        return FALSE;
}

void do_kotegaeshi(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	AFFECT_DATA af;
	int chance, dam;

	if ((chance = get_skill(ch,gsn_kotegaeshi))  < 1)
	{
		send_to_char( "Huh?\n\r", ch );
		return;
	}

        if ((victim = ch->fighting) == NULL)
                return send_to_char("You aren't fighting anyone.\n\r",ch);


	if (is_safe(ch,victim))
		return;

	act( "You grab $N's wrist and twist it violently.",  ch, NULL, victim, TO_CHAR    );
	act( "$n grabs your wrist and twists it violently.", ch, NULL, victim, TO_VICT    );
	act( "$n grabs $N's wrist and twists it violently.",  ch, NULL, victim, TO_NOTVICT );

	dam = GET_DAMROLL(ch)*number_range(1.5,2);

	if (chance < number_percent())
	{
		damage_old( ch, victim, 0, gsn_kotegaeshi, DAM_BASH, TRUE);
		check_improve(ch,gsn_kotegaeshi,FALSE,1);
		WAIT_STATE( ch, PULSE_VIOLENCE * 1.5 );		
		return;
	}

 	damage_old( ch, victim, dam, gsn_kotegaeshi, DAM_BASH, TRUE);

	if(!is_affected(victim, gsn_kotegaeshi))
	{
		init_affect(&af);
		af.where     = TO_AFFECTS;
		af.aftype    = AFT_MALADY;
		af.type      = gsn_kotegaeshi;
		af.level     = ch->level;
		af.duration  = 15;
		af.location  = APPLY_DEX;
		af.modifier  = -(6 + number_range(3,5));
		af.bitvector = 0;
		affect_to_char( victim, &af );
		af.location  = APPLY_STR;
		af.modifier  = -(6 + number_range(3,5));
		affect_to_char( victim, &af );
	}

	WAIT_STATE( ch, PULSE_VIOLENCE * 2 );
	check_improve(ch,gsn_kotegaeshi,TRUE,1);

}

bool no_vanish_area(CHAR_DATA *ch)
{
  bool found = FALSE;
  int i;

  const char *aname[50] =
  {
	"shadcave.are",
	"knightcastle.are",
	"masters.are",
	"ragervillage.are",
	"entropy.are",
	"empire.are",
	"sylvangrove.are",
	"bounty.are",
	
	"END"
  };

  if (IS_NPC(ch))
	return TRUE;

    if (IS_EXPLORE(ch->in_room))
	return TRUE;

    for (i = 0; i < 100; i++)
	{
	if (!str_cmp(aname[i],"END"))
		break;
	if (found)
		break;
    	if (!str_cmp(ch->in_room->area->file_name,aname[i]))
		found = TRUE;
	}

  return found;
}

void do_impale(CHAR_DATA *ch,char *argument)
{
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
	int chance, skill; 
	char arg[MAX_INPUT_LENGTH];
	AFFECT_DATA af;
	OBJ_DATA *spear;
	bool combat=FALSE;

	one_argument( argument, arg );	

	if ( (chance = get_skill(ch,gsn_impale)) == 0 )
	{
	      	send_to_char("Huh?\n\r",ch); 
      		return;
	}

	spear=get_eq_char(ch,WEAR_WIELD);

	if(!spear)
		return;

        if (spear->value[0] != WEAPON_SPEAR )
        {
                send_to_char("You must be wielding a spear to impale people.\n\r",ch);
                return;
        }

	if ( arg[0] == '\0')
	{
		if ((victim = ch->fighting) == NULL)
			return send_to_char("Impale who?\n\r", ch );
		victim = ch->fighting;
		combat = TRUE;
	}
	else if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}

	if (is_safe( ch, victim ))
		return;

	if (is_affected(victim,gsn_impale)) 
	{
                send_to_char( "They already have a chest wound.\n\r", ch );
                return;
	}

	if (!combat)
		if (check_chargeset(ch,victim)) {return;}

	chance = number_percent();
	skill = get_skill(ch,gsn_impale);

	if ((skill > 90) && (chance > 97) && (number_percent() < 40)) 
	{
		act("$n charges towards $N and drives $p straight through $S heart!",ch,spear,victim,TO_NOTVICT);
		act("$n charges towards you and drives $p straight through your heart!",ch,spear,victim,TO_VICT);
		act("You charge towards $N and drive $p straight through $s heart!",ch,spear,victim,TO_CHAR);
		act("{RYou gasp for breath as you feel your heart explode.{x",victim,0,0,TO_CHAR);
		raw_kill(ch,victim);
		check_improve(ch,gsn_impale,TRUE,3);
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
		return;
	}

	if (!IS_NPC(victim) && (!combat)) {
		switch(number_range(0,2)) {
		case (0):
			sprintf(buf,"Help! %s is trying to impale me!",PERS(ch,victim));
			break;
		case (1):
			sprintf(buf,"Help! %s is spiking my heart!",PERS(ch,victim));
			break;
		case (2):
			sprintf(buf,"Help! %s is impaling me!",PERS(ch,victim));
			break;
		}
		do_myell(victim,buf);
	}

	skill*=.75;

	if(IS_AFFECTED(ch,AFF_HASTE))
		skill+=10;

	if(IS_AFFECTED(victim,AFF_HASTE))
		skill-=15;

	if(victim->position==POS_SLEEPING)
		skill*=1.7;

	if(victim->position==POS_RESTING)
		skill*=1.3;

	if (skill < chance) {
		act("$n tries to drive $p through $N, but misses and steps too close!",ch,spear,victim,TO_NOTVICT);
		act("$n tries to drive $p through you, but misses and steps too close!",ch,spear,victim,TO_VICT);
		act("You try to drive $p through $N, but miss and step too close!",ch,spear,victim,TO_CHAR);
		check_improve(ch,gsn_impale,FALSE,2);
		one_hit_new(victim,ch,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,120,NULL);
		WAIT_STATE(ch,PULSE_VIOLENCE);
		return;
	} else {
		act("$n drives $p into $N, impaling $M!",ch,spear,victim,TO_NOTVICT);
		act("$n drives $p into you, impaling you!",ch,spear,victim,TO_VICT);
		act("You drive $p into $N, impaling $M!",ch,spear,victim,TO_CHAR);
		one_hit_new(ch,victim,TYPE_UNDEFINED,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,180,NULL);
		check_improve(ch,gsn_impale,TRUE,1);
		WAIT_STATE(ch,PULSE_VIOLENCE*2);

		init_affect(&af);
		af.where   	= TO_AFFECTS;
		af.aftype     	= AFT_MALADY;
		af.type      	= gsn_impale;
		af.level     	= ch->level;
		af.duration  	= 5;
		af.bitvector 	= 0;
		af.aftype    	= AFT_SKILL;
		af.location  	= APPLY_DEX;
		af.modifier  	= -(ch->level/5);
		af.owner_name  	= str_dup(ch->original_name);
		affect_to_char(victim,&af);
		af.location  	= APPLY_STR;
		af.modifier  	= -(ch->level/5);
		af.bitvector 	= 0;
		af.owner_name	= str_dup(ch->original_name);
		affect_to_char(victim,&af);
		af.location		= 0;
		af.owner_name	= str_dup(ch->original_name);
		char msg_buf[MSL];
		sprintf(msg_buf,"induces major bleeding");
		af.affect_list_msg = str_dup(msg_buf);
		affect_to_char(victim,&af);
		return;
	}
}

void do_chop(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	OBJ_DATA *wield;
	int chance;
	
	chance = get_skill(ch, gsn_chop);

	if (chance == 0)
	{
		send_to_char("You don't know how to chop.\n\r", ch);
		return;
	}
	
	if ((victim = ch->fighting) == NULL)
	{
		send_to_char("But you aren't fighting anyone.\n\r", ch);
		return;
	}
	
	wield = get_eq_char(ch, WEAR_WIELD);
	
	if (wield == NULL)
	{
		send_to_char("You need a weapon to chop with.\n\r", ch);
		return;
	}
	
	if (wield->value[0] != WEAPON_POLEARM)
	{
		send_to_char("You can only chop with a polearm.\n\r", ch);
		return;
	}
	
	chance -= 5;

	if (number_percent() > chance)
	{
		act("Your chop is a little too slow and $N evades it.",ch,0,victim,TO_CHAR);
		act("$n's chop is a little too slow and you evade it.",ch,0,victim,TO_VICT);
		damage( ch, victim, 0, gsn_chop, DAM_NONE, TRUE);
		check_improve(ch, gsn_chop, FALSE, 2);
		WAIT_STATE(ch, 2*PULSE_VIOLENCE);
		return;
	}
	
	act("You execute a controlled overhead attack with your polearm.",ch,0,0,TO_CHAR);
	act("$n executes a controlled overhead attack with $s polearm.",ch,0,0,TO_ROOM);
	
	one_hit_new(ch, victim, gsn_chop, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 130, NULL);
	
	check_improve(ch, gsn_chop, TRUE, 2);
	WAIT_STATE(ch, PULSE_VIOLENCE*2);
	return;
}	

void do_trip_wire(CHAR_DATA *ch, char *argument)
{
	ROOM_AFFECT_DATA raf;
	AFFECT_DATA af;
	int skill;
	/* Added variable to create two kinds of tripwires. A tripwire room affect with a level
	   value of 75 would be low in the room to trip non-flyers. If it's 100, it will be high
	   in the room, to catch flyers. These two values are extremely sensitive and should not
	   be changed. char_to_room() in handler.c holds the code that utilizes the values. - Zornath */

	/* Added - a level value of 125 indicates an advanced tripwire. - Zornath */
	int highOrLow = 0;
	char arg[MIL];
	
	skill = get_skill(ch, gsn_trip_wire);

	one_argument(argument, arg);
	
	if (is_affected_room(ch->in_room, gsn_trip_wire))
	{
		send_to_char("There is already a tripwire here.\n\r", ch);
		return;
	}
	
	if (is_affected(ch, gsn_trip_wire))
	{
		send_to_char("You already have a tripwire setup elsewhere.\n\r", ch);
		return;
	}
	
	if (is_affected(ch, gsn_wire_delay))
		return send_to_char("You cannot set up another tripwire yet.\n\r",ch);

	skill -= 10;
	if (arg[0] == '\0' && get_skill(ch,skill_lookup("advanced tripwire")) <= 1)
		return send_to_char("Set a high tripwire or a low tripwire?\n\r",ch);

	if (!str_prefix(arg,"high"))
		highOrLow = 100;

	if (!str_prefix(arg,"low"))
		highOrLow = 75;

	if (get_skill(ch,skill_lookup("advanced tripwire")) > 70)
		highOrLow = 125;

	if (number_percent() > skill)
	{
		send_to_char("You attempt to set up a tripwire, but botch the effort.\n\r", ch);
		check_improve(ch, gsn_trip_wire, FALSE, 1);
		return;
	}
	
	init_affect(&af);
	af.where		= TO_AFFECTS;
	af.aftype		= AFT_INVIS;
	af.type			= gsn_trip_wire;
	af.level		= ch->level;
	af.duration		= 3;
	af.modifier		= 0;
	af.bitvector		= 0;
	affect_to_char(ch, &af);

	init_affect(&af);
	af.where		= TO_AFFECTS;
	af.aftype		= AFT_SKILL;
	af.type			= gsn_wire_delay;
	af.name			= str_dup("tripwire");
	af.level		= ch->level;
	af.duration		= 3;
	af.modifier		= 0;
	af.bitvector		= 0;
	char msg_buf[MSL];
	sprintf(msg_buf,"prevents usage of tripwire");
	af.affect_list_msg = str_dup(msg_buf);
	affect_to_char(ch, &af);
	
	init_affect_room(&raf);
	raf.where		= TO_ROOM_AFFECTS;
	raf.aftype		= AFT_SKILL;
	raf.type		= gsn_trip_wire;
	raf.level		= highOrLow;
	raf.duration		= 3;
	raf.modifier		= 0;
	raf.bitvector		= AFF_ROOM_TRIPWIRE;
	raf.owner_name		= str_dup(ch->original_name);
        raf.end_fun             = NULL;
        raf.tick_fun            = NULL;
	raf.name                = NULL;
	new_affect_to_room(ch->in_room, &raf);
	
	if (highOrLow == 100)
	{
		send_to_char("You carefully conceal a tripwire high in the room.\n\r", ch);
		act("$n carefully conceals a tripwire high in the room.", ch, NULL, NULL, TO_ROOM);
	}
	else if (highOrLow == 75)
 	{
		send_to_char("You carefully conceal a tripwire low in the room.\n\r", ch);
		act("$n carefully conceals a tripwire low in the room.", ch, NULL, NULL, TO_ROOM);
	}
	else if (highOrLow == 125)
	{
		send_to_char("You masterfully conceal a tripwire in the room.\n\r", ch);
		act("$n masterfully conceals a tripwire in the room.",ch,NULL,NULL,TO_ROOM);
	}
	check_improve(ch, gsn_trip_wire, TRUE, 1);
	return;
}


void do_weapontrip(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    OBJ_DATA *weapon;
    char arg[MIL];
    char buf[MSL];
    int chance, dam;
    AFFECT_DATA af;
	
    one_argument(argument, arg);
	
    chance = get_skill(ch, gsn_weapontrip);
	
    if (chance == 0)
    {
	send_to_char("You don't know how to weapon trip.\n\r", ch);
	return;
    }
	
    weapon = get_eq_char(ch, WEAR_WIELD);
	
    if (weapon == NULL)
    {
	send_to_char("You need a weapon to trip with.\n\r", ch);
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

    if (victim->position < POS_FIGHTING)
    {
        act("$N is already down.",ch,NULL,victim,TO_CHAR);
        return;
    }
    
    if (victim == ch)
    {
	send_to_char("That's impossible.\n\r", ch);
	return;
    }
    
    if (ch->size < victim->size)
        chance += (ch->size - victim->size) * 10; 
        
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= get_curr_stat(victim,STAT_DEX) * 3 / 2;

    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
        chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
        chance -= 40;
        
    dam = number_range(40, 90);
    
    if (is_affected(victim,gsn_asscounter))
    {
	act("You dart to the side kicking the inside of $N's knee!",victim,0,ch,TO_CHAR);
        act("$n darts to the side kicking the inside of $N's knee!",victim,0,ch,TO_NOTVICT);
        act("$n dodges your weapon trip, then kicks the inside of your knee!",victim,0,ch,TO_VICT);
        WAIT_STATE(victim,1 * PULSE_VIOLENCE);
        dam = victim->level*2 + number_percent()*2 - 30;
        damage_old(victim,ch,dam,gsn_legbreak,DAM_BASH,TRUE);

        if (!is_affected(ch,gsn_legbreak))
    	{
                init_affect(&af);
                af.where     	= TO_AFFECTS;
                af.aftype 	= AFT_MALADY;
                af.type      	= gsn_legbreak;
                af.level     	= victim->level;
                af.duration  	= victim->level/5;
                af.bitvector 	= 0;
                af.location  	= APPLY_DEX;
                af.modifier  	= 1-(victim->level/5);
                affect_to_char(ch,&af);
        }
        affect_strip( victim, gsn_asscounter );
        return;
    }
    
    if (number_percent() < chance)
    {
        act("$n swings $p low and delivers a punishing blow to your legs!",ch,weapon,victim,TO_VICT);
        act("You swing $p low and deliver a punishing blow to $N's legs!",ch,weapon,victim,TO_CHAR);
        act("$n swings $p low and delivers a punishing blow to $N's legs!",ch,weapon,victim,TO_NOTVICT);
        check_improve(ch, gsn_weapontrip, TRUE, 1);

        if (!IS_NPC(ch) && !IS_NPC(victim) && (ch->fighting == NULL || victim->fighting == NULL))
        {
	    sprintf(buf,"Help! %s just tripped me with %s weapon!",
		PERS(ch,victim), ch->sex == 0 ? "its" : ch->sex == 1 ? "his" : "her");
            do_myell(victim,buf);
        }
        
        WAIT_STATE(victim, 1 * PULSE_VIOLENCE);
        WAIT_STATE(ch,skill_table[gsn_weapontrip].beats);
        victim->position = POS_RESTING;
        damage_new(ch, victim, dam, TYPE_UNDEFINED, DAM_TRUESTRIKE, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "weapon trip");
        check_downstrike(ch,victim);
        check_cheap_shot(ch,victim);
    }
    else
    {
        if (!IS_NPC(ch) && !IS_NPC(victim) && (ch->fighting == NULL || victim->fighting == NULL))
        {
	    sprintf(buf,"Help! %s just tried to trip me with %s weapon!",
		PERS(ch,victim), ch->sex == 0 ? "its" : ch->sex == 1 ? "his" : "her");
            do_myell(victim,buf);
        }
        
        damage(ch, victim, 0, gsn_weapontrip, DAM_BASH, TRUE);
        WAIT_STATE(ch,skill_table[gsn_weapontrip].beats*2/3);
        check_improve(ch, gsn_weapontrip, FALSE, 1);
    }
        check_killer(ch,victim);
}

void spell_true_sight( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    AFFECT_DATA af;

    if (is_affected(ch, gsn_true_sight))
	return send_to_char("You already see truly.\n\r", ch);

    if (is_affected(ch, gsn_blindness))
    {
        send_to_char("You can see again!\n\r", ch);
        affect_strip(ch, gsn_blindness);
        WAIT_STATE(ch, PULSE_VIOLENCE * 2);
    }

    init_affect(&af);	
    af.where		= TO_AFFECTS;
    af.type		= gsn_true_sight;
    af.location		= 0;
    af.level 		= level;
    af.modifier		= 0;
    af.duration		= ch->level/2;
    af.aftype		= AFT_POWER;
    af.bitvector	= AFF_DETECT_INVIS | AFF_DETECT_MAGIC;
	af.affect_list_msg = str_dup("grants the ability to detect the magical and the invisible");
    affect_to_char(ch, &af);
	

    send_to_char("Your vision sharpens!\n\r",ch);
    return;
}

void spell_resistance( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    	AFFECT_DATA af;

    	if (is_affected(ch, gsn_resistance))
		return send_to_char("You already resist blows.\n\r", ch);

    	if (cabal_down_new(ch, CABAL_RAGER, TRUE))
		return;

    	init_affect(&af);
    	af.where            = TO_AFFECTS;
    	af.type             = gsn_resistance;
    	af.location         = APPLY_AC;
    	af.level            = level;
    	af.modifier         = 20 * AC_PER_ONE_PERCENT_DECREASE_DAMAGE;
    	af.duration         = ch->level/3;
    	af.aftype           = AFT_POWER;
    	af.bitvector        = 0;
    	affect_to_char(ch, &af);
	
    	send_to_char("You build up an intense mental resolve and prepare for coming battles.\n\r",ch);
    	return;
}

void do_barkskin( CHAR_DATA *ch,char *argument)
{
    	AFFECT_DATA af;

    	if ( (get_skill(ch,gsn_barkskin) == 0)
        || (ch->level < skill_table[gsn_barkskin].skill_level[ch->class] ) )
    	{
        	send_to_char("You do not know how to turn your skin to bark.\n\r",ch);
        	return;
    	}

    	if (is_affected(ch,gsn_barkskin) )
    	{
        	send_to_char("Your skin is already covered in bark.\n\r",ch);
        	return;
    	}

    	if (ch->mana < 40)
    	{
        	send_to_char("You do not have the mana.\n\r",ch);
        	return;
    	}

        init_affect(&af);
        af.aftype 	= AFT_SKILL;
    	af.where 	= TO_AFFECTS;
    	af.type 	= gsn_barkskin;
    	af.level 	= ch->level;
    	af.location 	= APPLY_AC;
    	af.modifier 	= 10 * AC_PER_ONE_PERCENT_DECREASE_DAMAGE;
    	af.duration 	= ch->level/4;
    	af.bitvector 	= 0;
    	affect_to_char(ch,&af);

    	ch->mana -= 40;
    	act("$n's skin slowly becomes covered in bark.",ch,NULL,NULL,TO_ROOM);
    	send_to_char("Your skin slowly becomes covered in hardened bark.\n\r",ch);
    	return;
}

void do_jinx(CHAR_DATA *ch, char *argument)
{
	int sn=0, gn=0;
	for (sn=0; sn<MAX_SKILL; sn++) {
		if (skill_table[sn].name == NULL)
			break;
	}
        printf_to_char(ch, "Entries found in skill table: %d\n\r", sn);
	for (gn=0;gn<MAX_GROUP;gn++) {
		if (group_table[gn].name == NULL)
			break;
		printf_to_char(ch, "%d) %s\n\r", gn, group_table[gn].name);
	}
	printCabalInfo();
	return;
}

void do_pummel(CHAR_DATA *ch,char *argument)
{
	CHAR_DATA *victim;
	int chance, numhits, i, skill;

	if ((skill = get_skill(ch, gsn_pummel) ) == 0)
	{
		send_to_char("You don't know how to pummel.\n\r",ch); 
		return; 
      	}

	if ((victim = ch->fighting) == NULL)
	{
		send_to_char("You aren't fighting anyone.\n\r",ch);
		return;
	}

	if (get_eq_char(ch,WEAR_SHIELD) != NULL || get_eq_char(ch,WEAR_HOLD) != NULL
	|| get_eq_char(ch,WEAR_LIGHT) != NULL || get_eq_char(ch,WEAR_DUAL_WIELD) != NULL
	|| get_eq_char(ch,WEAR_WIELD) != NULL)
	{
            send_to_char("You need both hands free to pummel with.\n\r",ch);
            return;
	}

	chance = number_percent();

	if (chance > skill) {
		act("You attempt to pummel $N with your fists, but fail.",ch,0,victim,TO_CHAR);
		act("$n attempts to pummel $N with $s fists, but fails.",ch,0,victim,TO_ROOM); 
		check_improve(ch,gsn_pummel,FALSE,2); 
		WAIT_STATE(ch, PULSE_VIOLENCE);
		return;
	}

	chance+=get_curr_stat(ch,STAT_STR)*2;
	chance-=30;

	if(IS_AFFECTED(ch,AFF_HASTE))
		chance+=20;
	if(IS_AFFECTED(victim,AFF_HASTE))
		chance-=20;

	if ((chance + skill) > 175) {
		numhits = 6;
	} else if ((chance + skill) > 160) {
		numhits = 5;
	} else if ((chance + skill) > 145) {
		numhits = 4;
	} else if ((chance + skill) > 130) {
		numhits = 3;
	} else {
		numhits = 2;
	}

	if (IS_SET(ch->act,PLR_MORON)) {
		numhits = 2;
	}

      	act("You pummel $N with your fists.",ch,0,victim,TO_CHAR); 
      	act("$n pummels $N with $s fists.",ch,0,victim,TO_ROOM); 
	
	for ( i=0; i < numhits; i++ ) {
		if (ch->in_room != victim->in_room) break;
		if (number_percent() > 95) {
			damage( ch, victim, 0, gsn_pummel,DAM_NONE,TRUE);
			continue;
		}
		one_hit_new(ch, victim, gsn_pummel, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 135, NULL);
	}

	check_improve(ch,gsn_pummel,TRUE,1);
	WAIT_STATE(ch,2*PULSE_VIOLENCE);
	return;
}

void do_throwdown(CHAR_DATA *ch, char *argument)
{
	OBJ_DATA *obj;
	int count=0;

	if (ch->in_room == NULL) {
		n_logf("Do_throwdown: %s is trying to throw weapons in null room.", ch->original_name);
		return;
	}

	obj = get_eq_char(ch, WEAR_LIGHT);
	if (obj != NULL) {
		if (!IS_OBJ_STAT(obj,ITEM_NOREMOVE) && !IS_OBJ_STAT(obj,ITEM_NODROP)) {
			act("You throw $p to the ground.", ch, obj, 0, TO_CHAR);
			act("$n throws $p to the ground.", ch, obj, 0, TO_ROOM);
			obj_from_char(obj);
			obj_to_room(obj, ch->in_room);
			count++;
		}
	}

        obj = get_eq_char(ch, WEAR_SHIELD);
        if (obj != NULL) {
                if (!IS_OBJ_STAT(obj,ITEM_NOREMOVE) && !IS_OBJ_STAT(obj,ITEM_NODROP)) {
                        act("You throw $p to the ground.", ch, obj, 0, TO_CHAR);
                        act("$n throws $p to the ground.", ch, obj, 0, TO_ROOM);
                        obj_from_char(obj);
                        obj_to_room(obj, ch->in_room);
                        count++;
                }
        }

        obj = get_eq_char(ch, WEAR_DUAL_WIELD);
        if (obj != NULL) {
                if (!IS_OBJ_STAT(obj,ITEM_NOREMOVE) && !IS_OBJ_STAT(obj,ITEM_NODROP)) {
                        act("You throw $p to the ground.", ch, obj, 0, TO_CHAR);
                        act("$n throws $p to the ground.", ch, obj, 0, TO_ROOM);
                        obj_from_char(obj);
                        obj_to_room(obj, ch->in_room);
                        count++;
                }
        }

        obj = get_eq_char(ch, WEAR_WIELD);
        if (obj != NULL) {
                if (!IS_OBJ_STAT(obj,ITEM_NOREMOVE) && !IS_OBJ_STAT(obj,ITEM_NODROP)) {
                        act("You throw $p to the ground.", ch, obj, 0, TO_CHAR);
                        act("$n throws $p to the ground.", ch, obj, 0, TO_ROOM);
                        obj_from_char(obj);
                        obj_to_room(obj, ch->in_room);
                        count++;
                }
        }


        obj = get_eq_char(ch, WEAR_HOLD);
        if (obj != NULL) {
                if (!IS_OBJ_STAT(obj,ITEM_NOREMOVE) && !IS_OBJ_STAT(obj,ITEM_NODROP)) {
                        act("You throw $p to the ground.", ch, obj, 0, TO_CHAR);
                        act("$n throws $p to the ground.", ch, obj, 0, TO_ROOM);
                        obj_from_char(obj);
                        obj_to_room(obj, ch->in_room);
                        count++;
                }
        }

	if (count > 2) {
		n_logf("Do_throwdown: %s threw down more than 2 items.", ch->original_name);
	}
	else if (count == 0) {
		send_to_char("You have nothing to throw down.\n\r", ch);
	}

	return;
}

void do_darkshout(CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	int devil, i;
	CHAR_DATA *nightwalker, *gch;

	if (get_skill(ch, gsn_darkshout) == 0)
	{
		send_to_char("You haven't the slightest idea how to invoke an unholy presence.\n\r", ch);
		return;
	}

	if (is_affected(ch, gsn_darkshout))
		return send_to_char("You already embrace the will of an unholy shade.\n\r", ch);

	if (weather_info.sunlight != SUN_DARK && !IS_IMMORTAL(ch))
	{
		send_to_char("You can only darkshout during the night.\n\r", ch);
		return;
	}

	if (number_percent() > get_skill(ch,gsn_darkshout))
	{
		send_to_char("You shout into the night, but no unholy presence responds.\n\r", ch);
		act("$n shouts into the night, but no unholy presence responds.", ch, NULL, NULL, TO_ROOM);
		check_improve(ch, gsn_darkshout, FALSE, 1);
		WAIT_STATE(ch, 24);
		return;
	}

	devil = number_range(1, 8);

  	for (gch = char_list; gch != NULL; gch = gch->next)
    	{
      		if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch && gch->pIndexData->vnum == 1200)
        	{
			while (devil == DARKSHOUT_BAALZEBUB) 
				devil = number_range(1, 8);
        	}
    	}

	switch(devil)
	{
		case DARKSHOUT_BELAL:
			printf_to_char(ch, "The will of The Devil Be'lal overcomes you.\n\r", ch);
			act("The will of The Devil Be'lal overcomes $n.", ch, NULL, NULL, TO_ROOM);
        		init_affect(&af);
        		af.aftype 	= AFT_SKILL;
        		af.where     	= TO_AFFECTS;
        		af.type      	= gsn_darkshout;
        		af.level     	= ch->level;
        		af.duration  	= number_fuzzy(ch->level);
        		af.modifier  	= 20;
        		af.bitvector 	= 0;
        		af.location  	= APPLY_DAMROLL;
        		affect_to_char(ch, &af);
        		af.location  	= APPLY_HITROLL;
        		affect_to_char(ch, &af);
			break;
		case DARKSHOUT_ISHMAEL:
			printf_to_char(ch, "The will of The Devil Ishmael overcomes you.\n\r", ch);
			act("The will of The Devil Ishmael overcomes $n.", ch, NULL, NULL, TO_ROOM);
        		init_affect(&af);
        		af.aftype 	= AFT_SKILL;
        		af.where     	= TO_AFFECTS;
        		af.type      	= gsn_darkshout;
        		af.level     	= ch->level;
        		af.duration  	= number_fuzzy(ch->level/4);
        		af.modifier  	= -50;
        		af.location  	= APPLY_DAM_MOD;
        		af.bitvector 	= 0;
        		affect_to_char(ch, &af);
			break;
		case DARKSHOUT_AGAMEMNON:
			printf_to_char(ch, "The fiery hand of Agamemnon cauterizes your wounds.\n\r", ch);
			act("The fiery hand of Agamemnon cauterizes $n's wounds.", ch, NULL, NULL, TO_ROOM);
			ch->hit = UMIN( ch->hit + 1000, ch->max_hit );
			update_pos( ch );
			if (is_affected(ch,skill_lookup("impale"))) {          
				affect_strip(ch,skill_lookup("impale")); 
			}
			if (is_affected(ch,skill_lookup("boneshatter"))) {     
				affect_strip(ch,skill_lookup("boneshatter")); 
			}
			if (is_affected(ch,skill_lookup("gouge"))) {          
				affect_strip(ch,skill_lookup("gouge")); 
			}
			if (is_affected(ch,skill_lookup("crossslice"))) {       
				affect_strip(ch,skill_lookup("crossslice")); 
			}
			break;
		case DARKSHOUT_MEPHISTOPHELES:
			printf_to_char(ch, "The will of The Archdevil Mephistopheles overcomes you.\n\r", ch);
			act("The will of The Archdevil Mephistopheles overcomes $n.", ch, NULL, NULL, TO_ROOM);
			break;
		case DARKSHOUT_BAALZEBUB: 
			printf_to_char(ch, "The Archdevil Baalzebub sends forth a nightwalker to aid you.\n\r");
			act("A shadowy nightwalker appears.", ch, NULL, NULL, TO_ROOM);
			nightwalker = create_mobile( get_mob_index(1200) );
			for (i=0;i < MAX_STATS; i++)
				nightwalker->perm_stat[i] = ch->perm_stat[i];
			nightwalker->max_hit 	= URANGE(ch->pcdata->perm_hit,1*ch->pcdata->perm_hit,30000);
			nightwalker->hit 	= nightwalker->max_hit;
			nightwalker->max_mana 	= ch->max_mana;
			nightwalker->mana 	= nightwalker->max_mana;
			nightwalker->level 	= ch->level;
			for (i=0; i < 3; i++)
				nightwalker->armor[i] = interpolate(nightwalker->level,100,-100);
			nightwalker->armor[3] 	= interpolate(nightwalker->level,100,0);
			nightwalker->gold 	= 0;
			nightwalker->damage[DICE_NUMBER] = number_range(ch->level/15, ch->level/10);
			nightwalker->damage[DICE_TYPE]   = number_range(ch->level/3, ch->level/2);
			nightwalker->damage[DICE_BONUS]  = 0;
			char_to_room(nightwalker,ch->in_room);
			SET_BIT(nightwalker->affected_by, AFF_CHARM);
			nightwalker->master = nightwalker->leader = ch;
			send_to_char("A nightwalker appears before you, bound to your servitude.\n\r", ch);
			break;
		case DARKSHOUT_NECRUVIAN:
			printf_to_char(ch, "The will of The Lich Necruvian overcomes you.\n\r", ch);
			act("The will of The Lich Necruvian overcomes $n.", ch, NULL, NULL, TO_ROOM);
			break;
		case DARKSHOUT_THANATOS:
			printf_to_char(ch, "The will of Thanatos overcomes you.\n\r", ch);
			act("The will of Thanatos overcomes $n.", ch, NULL, NULL, TO_ROOM);
        		init_affect(&af);
        		af.aftype 	= AFT_SKILL;
        		af.where     	= TO_AFFECTS;
        		af.type      	= gsn_darkshout;
        		af.level     	= ch->level;
        		af.duration  	= number_fuzzy(ch->level/3);
        		af.modifier  	= 35;
        		af.bitvector 	= 0;
        		af.location  	= APPLY_DAMROLL;
        		affect_to_char(ch, &af);
        		af.location  	= APPLY_HITROLL;
        		affect_to_char(ch, &af);
			break;
		case DARKSHOUT_BAAL:
			printf_to_char(ch, "The will of Baal overcomes you.\n\r", ch);
			act("The will of Baal overcomes $n.", ch, NULL, NULL, TO_ROOM);
			break;
		default: send_to_char("Error!", ch); return;
	}

	if (devil == DARKSHOUT_MEPHISTOPHELES || devil == DARKSHOUT_BAALZEBUB || devil == DARKSHOUT_NECRUVIAN
	|| devil == DARKSHOUT_NECRUVIAN || devil == DARKSHOUT_BAAL)
	{
        	init_affect(&af);
			
			switch (devil) {
				case DARKSHOUT_MEPHISTOPHELES: af.affect_list_msg = str_dup("grants a chance for an additional attack"); break;
				case DARKSHOUT_BAAL: af.affect_list_msg = str_dup("relentlessly assaulting those with good hearts"); break;
				case DARKSHOUT_NECRUVIAN: af.affect_list_msg = str_dup("siphoning the souls of good-aligned beings"); break;
				case DARKSHOUT_BAALZEBUB: af.affect_list_msg = str_dup("prevents usage of darkshout"); break;
			}
        	af.aftype 	= AFT_SKILL;
        	af.where     	= TO_AFFECTS;
        	af.type      	= gsn_darkshout;
        	af.level     	= ch->level;
        	af.duration  	= number_fuzzy(ch->level/2);
        	af.modifier  	= devil;
        	af.bitvector 	= 0;
        	af.location  	= APPLY_NONE;
        	affect_to_char(ch, &af);
	}
	check_improve(ch, gsn_darkshout, TRUE, 1);
	return;
}

int check_darkshout(CHAR_DATA *ch)
{
        AFFECT_DATA *paf;
        int modifier = -1;

        for (paf = ch->affected; paf != NULL; paf = paf->next)
        {
                if (paf->type == skill_lookup("darkshout")) {
                        modifier = paf->modifier;
                        break;
                }
        }

        return modifier;
}

void spell_charge_weapon(int sn,int level,CHAR_DATA *ch, void *vo,int target)
{
	OBJ_DATA *obj = (OBJ_DATA *) vo;
	AFFECT_DATA af;

    	if (obj->item_type != ITEM_WEAPON)
        	return send_to_char("That is not a weapon.\n\r",ch);

	if (obj->wear_loc != -1)
		return send_to_char("You aren't carrying that.\n\r",ch);

	if (is_affected_obj(obj, sn))
		return send_to_char("That weapon is already charged.\n\r", ch);

	if (attack_table[obj->value[3]].damage != DAM_SLASH
	&& attack_table[obj->value[3]].damage != DAM_PIERCE
	&& attack_table[obj->value[3]].damage != DAM_BASH)
		return send_to_char("You can't charge that weapon.\n\r", ch);
	
	init_affect(&af);	
	af.where	= TO_WPN_V3;
	af.owner_name= str_dup(ch->original_name);
	af.aftype	= AFT_SPELL;
	af.level	= ch->level;
	af.duration	= number_range(8,16);	
	af.type		= sn;
	af.modifier	= attack_lookup("shockingbite");
	char msg_buf[MSL];
	sprintf(msg_buf,"electrically charges your weapon");
	af.affect_list_msg = str_dup(msg_buf);
	af.location	= APPLY_NONE;
	af.bitvector	= WEAPON_SHOCKING;
	affect_to_obj(obj, &af);

	act("$p becomes electrified.", ch, obj, NULL, TO_CHAR);
	return;
}

void do_dent(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	OBJ_DATA *mace, *item;
	int chance=0;

	if ((chance = get_skill(ch,gsn_dent)) == 0)
		return send_to_char("You don't know how to properly dent an opponents armor.\n\r", ch);

	if ((victim = ch->fighting) == NULL)
		return send_to_char("But you aren't fighting anyone!\n\r",ch);

	mace = get_eq_char(ch, WEAR_WIELD);
	if (!mace || mace->value[0] != WEAPON_MACE)
		return send_to_char("You need a mace in your primary hand to dent with.\n\r", ch);

	WAIT_STATE(ch, PULSE_VIOLENCE);

	if (number_percent() < chance - 70) {
		item = get_obj_wear(victim, argument);
		if (!item || item == NULL) {
			act("You swing wildly at $N's armor, but fail to make contact with anything.", ch, 0, victim, TO_CHAR);
			act("$n swings wildly at your armor, but fails to make contact with anything.", ch, 0, victim, TO_VICT);
			act("$n swings wildly at $N's armor, but fails to make contact with anything.", ch, 0, victim, TO_NOTVICT);
			return;
		}
		if (!str_cmp(item->material->name, "energy") || item->wear_loc == WEAR_BRAND) {
			act("Your mace makes contact with $p, but it seems unaffected.", ch, item, victim, TO_CHAR);
			act("$n's mace makes contact with $p, but it seems unaffected.", ch, item, 0, TO_ROOM);
			return;
		}
		// Success
		if (item->item_type == ITEM_WEAPON) {
			act("You smash your mace into $p, shattering it into pieces.", ch, item, victim, TO_CHAR);
			act("$n smashes $s mace into $p, shattering it into pieces.", ch, item, 0, TO_ROOM);
		} else {
			act("You dent $p with your mace, destroying it.", ch, item, victim, TO_CHAR);
			act("$n dents $p with $s mace, destroying it.", ch, item, 0, TO_ROOM);
		}
		ADD_WAIT_STATE(ch, PULSE_VIOLENCE);
		unequip_char(victim, item);
		extract_obj(item);
		check_improve(ch, gsn_dent, TRUE, 4);
	} else {
		act("You swing wildly at $N's armor, but fail to make contact with anything.", ch, 0, victim, TO_CHAR);
		act("$n swings wildly at your armor, but fails to make contact with anything.", ch, 0, victim, TO_VICT);
		act("$n swings wildly at $N's armor, but fails to make contact with anything.", ch, 0, victim, TO_NOTVICT);	
		check_improve(ch, gsn_dent, FALSE, 3);
	}
	return;
}

// New Chromatic Fire
void spell_chromatic_fire_new( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
        CHAR_DATA *victim = (CHAR_DATA *) vo;
        char *spell=NULL;
        AFFECT_DATA af;
        int newsn, chance, slev=ch->level;

	char *offensive[15] =
	{
		"blind", "curse", "faerie fire", "fire and ice", "icelance", 
		"forget", "power word kill", "energy drain", "chill", "hydrolic ram",
		"mental knife", "wither", "rot", "deafen", "spire of flames"
	};

	char *defensive[20] = 
	{
		"rejuvenate", "resist weapon", "haste", "enlarge", "reduce",
		"shield", "armor", "cure critical", "stone skin", "detect invis",
		"resist acid", "resist cold", "resist cold", "resist mental", "resist negative",
		"fly", "aura", "shield", "static charge", "summon earth elemental"
	};

	if (is_affected(ch, sn))
		return send_to_char("Nothing happens.\n\r", ch);

        act("A fiery mass appears at your bidding and flies a zig-zag course to $N.",ch,NULL,victim,TO_CHAR);
        act("A large ball of strange flame appears in front of $n and flies around before striking you!",ch,NULL,victim,TO_VICT);
        act("A fiery mass appears at $n's bidding and flies around the room, nearly hitting you, before striking $N.",ch,NULL,victim,TO_NOTVICT);

	if (victim == ch)
	{
		chance = number_range(0,19);
		spell = defensive[chance];
	} else {
		chance = number_range(0,14);
		spell = offensive[chance];
	}

        if ((newsn = skill_lookup(spell)) < 0)
	{
		n_logf("Chromatic_fire: bad spell sn for %s", spell);
		bug("Chromatic_fire: bad spell sn.", 0);
                return;
	}

	if (newsn == skill_lookup("summon earth elemental"))
		slev = 70;

        (*skill_table[newsn].spell_fun) (newsn,slev,ch,victim,TARGET_CHAR);

        if (number_range(1,4) == 1 && !is_affected(ch,skill_lookup("chromatic fire")))
        {
                init_affect(&af);
                af.where     = TO_AFFECTS;
                af.aftype    = AFT_POWER;
                af.type      = sn;
                af.level     = ch->level;
                af.duration  = ch->level / 15;
                af.modifier  = 0;
                af.location  = APPLY_NONE;
                af.bitvector = 0;
				af.affect_list_msg = str_dup("prevents further usage of chromatic fire");
                affect_to_char(ch, &af);
				send_to_char("You are engulfed in the chromatic fire.\n\r",ch);
        }
        return;
}

void do_blackjack( CHAR_DATA *ch,char *argument)
{
	CHAR_DATA *victim;
	AFFECT_DATA af, timer;
	char arg[MSL], buf[MSL];
	int chance, size=0;

	one_argument(argument, arg);

	if ((chance = get_skill(ch, gsn_blackjack)) == 0)
		return send_to_char("You don't know how to blackjack people.\n\r", ch);

	if (arg[0] == '\0')
		return send_to_char("Blackjack who?\n\r", ch);

	if ((victim = get_char_room(ch,arg)) == NULL)
		return send_to_char("They aren't here.\n\r", ch);

	if (victim == ch)
		return send_to_char("Amusing, but not possible.\n\r",ch);

	if (is_safe(ch,victim))
		return;

	if (check_chargeset(ch,victim))
		return;

	// At this point, the attempt is considered a valid attack.
	if (!IS_NPC(ch))
		update_pc_last_fight(ch,victim);

	size = victim->size - ch->size;

	if (size > 2)
		return printf_to_char(ch, "%s is too large for you to blackjack.\n\r", victim->name);

	if (victim->position == POS_SLEEPING)
		return send_to_char("They are already sleeping.\n\r", ch);

	if (is_affected(victim, gsn_blackjack))
		return send_to_char("They are already blackjacked.\n\r", ch);

	if (is_affected(victim, gsn_blackjack_timer))
		return send_to_char("They are guarding their head too well right now.\n\r", ch);

	if(size>0)
		chance -= 5*size;
        if (is_affected(victim,skill_lookup("haste")))
                chance -= 10;
        if (is_affected(ch,skill_lookup("haste")))
                chance += 10;
        if (get_eq_char(victim,WEAR_HEAD) != NULL)
                chance -= 10;
	if(is_affected(victim,gsn_cloak_form))
		chance+=10;

	if (victim->position == POS_FIGHTING)
		chance -= 45;

	if (get_skill(ch,skill_lookup("improved blackjack")) == 100) {
		if (chance < 40) chance = 70;
		else if (chance < 50) chance = 75;
		else if (chance < 60) chance = 80;
		else if (chance < 70) chance = 85;
		else if (chance < 80) chance = 90;
		else if (chance < 90) chance = 95;
		else chance = 100;
	}
	
	chance = (chance * 8) / 10;
	
	if (ch->level == MAX_LEVEL)
		chance = 100; // Teehee

	// Every blackjack attempt (failure or not) gives the victim the protection timer.
	init_affect(&timer);
	timer.where		= TO_AFFECTS;
	timer.type		= gsn_blackjack_timer;
	timer.level		= 51;
	timer.location		= 0;
	timer.modifier		= 0;
	timer.aftype		= AFT_INVIS;
	timer.bitvector		= 0;
	timer.owner_name	= str_dup(ch->original_name);
	timer.duration		= 6;
	affect_to_char(victim, &timer);

	if (number_percent() < chance)
	{
		act("You whack $N over the head with a heavy looking sack. Ouch.",ch,0,victim,TO_CHAR);
        	act("$n whacks $N over the head with a heavy looking sack. Ouch.",ch,0,victim,TO_NOTVICT);
        	send_to_char("You feel a sudden pain erupt through the back of your skull.\n\r",victim);
		if (get_skill(ch,skill_lookup("improved blackjack")) == 100)
			send_to_char("You feel the pain push agonizingly forward through your head, numbing your brain.\n\r",victim);
		stop_fighting(victim, TRUE);
		victim->position = POS_SLEEPING;
		
		init_affect(&af);
		af.where	= TO_AFFECTS;
		af.type		= gsn_blackjack;
		af.level	= 51;
		af.location	= 0;
		af.modifier	= 0;
		af.bitvector	= AFF_SLEEP;
		af.aftype	= AFT_MALADY;
		af.affect_list_msg = str_dup("inflicts sleep");
		af.owner_name= str_dup(ch->original_name);
		af.duration	= 2;
		if (get_skill(ch,skill_lookup("improved blackjack")) == 100)
			af.duration = 4;
		affect_to_char(victim, &af);
		if (get_skill(ch,skill_lookup("improved blackjack")) == 100)
		{
			init_affect(&af);
			af.where	= TO_AFFECTS;
			af.type	= gsn_confuse;
			af.aftype = AFT_MALADY;
			af.level	= 53;
			af.duration = 2;
			af.affect_list_msg = str_dup("inflicts a confused state");
			affect_to_char(victim,&af);
			send_to_char("The impact against your skull leaves you dazed and confused.\n\r",victim);
			act("Your brutal head impact leaves $N dazed and confused.",ch,0,victim,TO_CHAR);
			act("The brutal impact leaves $N appearing dazed and confused.",ch,0,victim,TO_NOTVICT);
		}
			

        	check_improve(ch, gsn_blackjack, TRUE, 3);
        	WAIT_STATE(ch, 2*PULSE_VIOLENCE);		
		return;
	} else {
        	act("You attempt to blackjack $N but fail.",ch,0,victim,TO_CHAR);
        	act("$n attempts to blackjack $N but misses.",ch,0,victim,TO_NOTVICT);
        	act("$n hits you over the head with a heavy sack.",ch,0,victim,TO_VICT);
        	check_improve(ch, gsn_blackjack, FALSE, 2);
		damage_new(ch, victim, 85, gsn_blackjack, DAM_BASH, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "blow to the head");
        	WAIT_STATE(ch, 2*PULSE_VIOLENCE);
        	if (!IS_NPC(victim))
           	{
                	sprintf(buf, "Help! %s tried to blackjack me!",PERS(ch,victim));
                	do_myell(victim,buf);
                	multi_hit(ch,victim,TYPE_UNDEFINED);
           	}
        }
	return;
}

void blizzard_update(ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *raf)
{
	CHAR_DATA *owner, *victim, *victim_next;
	int dam=0;

	for (owner = char_list; owner != NULL; owner=owner->next) 
		if (!IS_NPC(owner) && !str_cmp(owner->original_name, raf->owner_name))
			break;

	if (!owner)
		return affect_remove_room(room, raf);
	
	dam = 2*raf->level;
	
	for (victim = room->people; victim != NULL; victim = victim_next)
	{
		victim_next = victim->next_in_room;

		if (IS_NPC(victim))
			continue;

		if (is_safe(victim, owner))
			continue;

		if (check_immune(victim, DAM_COLD) == IS_IMMUNE)
			continue;

		damage_newer(owner, victim, dam, TYPE_UNDEFINED, DAM_COLD, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "The extreme cold",
			PLURAL, DAM_NOT_POSSESSIVE,NULL, FALSE);		
	}
	return;
}

void blizzard_end(ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *raf)
{
	room_echo(room, "The blizzard disappates.");
}

void spell_blizzard(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	ROOM_AFFECT_DATA raf;
	AFFECT_DATA af;

	if (!IS_OUTSIDE(ch))
		return send_to_char("You can't cast this spell outside.\n\r", ch);

	if (is_affected(ch, sn))
		return send_to_char("You can't cast this spell again so soon.\n\r", ch);

	if (is_affected_room(ch->in_room, sn))
		return send_to_char("This room is already affected by a blizzard.\n\r", ch);

	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.type		= sn;
	af.level	= level;
	af.owner_name= str_dup(ch->original_name);
	af.duration	= level/5;
	af.modifier	= 0;
	af.aftype	= AFT_INVIS;
	affect_to_char(ch, &af);

	init_affect_room(&raf);
	raf.where	= TO_ROOM_AFFECTS;
	raf.aftype	= AFT_SPELL;
	raf.type	= sn;
	raf.level	= level;
	raf.duration	= level/5;
	raf.modifier	= 0;
	raf.bitvector	= 0;
	raf.owner_name	= str_dup(ch->original_name);
	raf.tick_fun	= blizzard_update;
	raf.end_fun	= blizzard_end;
	affect_to_room(ch->in_room, &raf);

	send_to_char("You intone a word of magic, and with that create a fierce blizzard!\n\r", ch);
	act("$n intones a word of magic, and with that creates a fierce blizzard!", ch, 0, 0, TO_ROOM);
	return;
}	

void spell_sequester_demon(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = get_char_room(ch, target_name);
	CHAR_DATA *mob, *check;
	int m_num=0;

//	if (weather_info.sunlight != SUN_DARK)
//		return send_to_char("Nothing happens...\n\r", ch);

	for (check = char_list; check != NULL; check = check->next)
	{
		if (IS_NPC(check) && check->owner != NULL && check->owner == ch)
			return send_to_char("You have already sequestered a being of the night.\n\r", ch);
	}

	if(!victim)
		return send_to_char("They aren't here.\n\r", ch);

	if (IS_NPC(victim))
		return send_to_char("Nothing happens...\n\r", ch);

	if (is_safe(ch, victim))
		return;
	
	// Mob vnum range for spell is 200 through 202.
	m_num = number_range(200,202);

	mob = create_mobile(get_mob_index(m_num));
	if (!mob)
		return;

	char_to_room(mob, victim->in_room);
	act("You sequester a demonic creature to assault $N!", ch, 0, victim, TO_CHAR);
	mob->hunting = victim;
	mob->last_fought = victim;
	mob->owner = ch;
	multi_hit(mob, victim, TYPE_UNDEFINED);
	return;
}

void spell_sigil_pain(int sn,int level, CHAR_DATA *ch,void *vo,int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
        int dam=0, nsn=sn;
	bool anathema=FALSE;
	
	if (cabal_down(ch,CABAL_EMPIRE))
		return;

	dam = dice(ch->level, 10);
	
	if (!IS_NPC(victim) && victim->pcdata->empire == EMPIRE_ANATH)
		anathema = TRUE;

	if (anathema)
		nsn = TYPE_TRUESTRIKE;

	if (anathema)
		dam *= 1.5;

	damage_new(ch, victim, dam, nsn, DAM_NEGATIVE, TRUE, HIT_UNBLOCKABLE, HIT_NOADD, HIT_NOMULT, "sigil of pain");

	if (!is_affected(victim, gsn_sigil_pain) && number_range(1,4) == 1)
	{
		init_affect(&af);
		af.where	= TO_AFFECTS;
		af.type		= sn;
		af.aftype	= AFT_POWER;
		af.level	= anathema ? level*2 : level;
		af.duration	= anathema ? level/2 : level/6;
		af.location	= APPLY_DAM_MOD;
		af.modifier	= anathema ? 60 : 40;
		af.bitvector	= 0;
		affect_to_char(victim, &af);
		af.location	= APPLY_DAMROLL;
		af.modifier	= anathema ? -25 : -10;
		affect_to_char(victim, &af);
		act("You begin to feel weak and frail as $n's sigil claims your flesh!", ch, 0, victim, TO_VICT);
		act("$N looks weak and frail.", ch, 0, victim, TO_NOTVICT);
	}
	return;
}

bool is_protected_city(AREA_DATA *area)
{
	if (!str_cmp(area->file_name, "midgaard.are"))
		return TRUE;

	if (!str_cmp(area->file_name, "newthalos.are"))
		return TRUE;

        if (!str_cmp(area->file_name, "tarvalon.are"))
                return TRUE;

        if (!str_cmp(area->file_name, "arkham.are"))
                return TRUE;

	return FALSE;
}

void do_crescent(CHAR_DATA *ch, char *argument)
{
        CHAR_DATA *victim;
        OBJ_DATA *wield;
        int chance;

        chance = get_skill(ch, gsn_crescent);

        if (chance == 0)
        {
                send_to_char("You don't know how to do that.\n\r", ch);
                return;
        }

        if ((victim = ch->fighting) == NULL)
        {
                send_to_char("But you aren't fighting anyone.\n\r", ch);
                return;
        }

        wield = get_eq_char(ch, WEAR_WIELD);

        if (wield == NULL)
        {
                send_to_char("You aren't wearing a weapon.\n\r", ch);
                return;
        }

        if (wield->value[0] != WEAPON_SPEAR)
        {
                send_to_char("You need a spear to perform this maneuver.\n\r", ch);
                return;
        }

        chance -= 15;

        if (number_percent() > chance)
        {
		act("You swing $p in a wide arc, but miss!", ch, wield, victim, TO_CHAR);
		act("$n swings $p in a wide arc, but fails to hit you.", ch, wield, victim, TO_VICT);
		act("$n swings $p in a wide arc, but fails to hit $N.", ch, wield, victim, TO_NOTVICT);
                damage( ch, victim, 0, gsn_crescent, DAM_NONE, TRUE);
                check_improve(ch, gsn_crescent, FALSE, 2);
                WAIT_STATE(ch, PULSE_VIOLENCE);
                return;
        }

	act("You swing $p in a wide arc, slashing $N across the chest.", ch, wield, victim, TO_CHAR);
	act("$n swings $p in a wide arc, slashing you across the chest.", ch, wield, victim, TO_VICT);

        one_hit_new(ch, victim, gsn_crescent, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, 135, "crescent slash");
        check_improve(ch, gsn_crescent, TRUE, 2);
        WAIT_STATE(ch, PULSE_VIOLENCE*2);
        return;
}

void do_garrotte(CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	int chance=0, dam=0;
	AFFECT_DATA af;
	char arg[MIL], buf[MSL];

	one_argument(argument,arg);

        if ( (chance = get_skill(ch,gsn_garrotte)) == 0)
        {
                send_to_char("You have no clue how to use a garrotte.\n\r",ch);
                return;
        }
	
        if ((victim = get_char_room(ch,arg)) == NULL)
        {
                send_to_char("They aren't here.\n\r",ch);
                return;
        }

        if (victim == ch)
        {
                send_to_char("Just go to sleep.\n\r",ch);
                return;
        }

        if (is_affected(victim,gsn_garrotte) && !IS_AWAKE(victim))
        {
                send_to_char("They are already lying on the ground unconcious.\n\r",ch);
                return;
        }

        if (is_safe(ch,victim))
                return;

        if (check_chargeset(ch,victim)) 
		return;

	update_pc_last_fight(ch,victim);

        if (is_affected(victim,gsn_asscounter)) // Preparation skill.
        {
		act("Seeing $N's attack coming, you grab $S arm and throw $M to the ground.", victim, 0, ch, TO_CHAR);
		act("$n senses your attack and grabs your arm, throwing you to the ground.", victim, 0, ch, TO_VICT);	
                WAIT_STATE(ch,2 * PULSE_VIOLENCE);
                dam = GET_DAMROLL(victim)*2;
                damage_old(victim,ch,dam,gsn_throw,DAM_BASH,TRUE);
                affect_strip( victim, gsn_asscounter );
                return;
        }


        if (is_affected(victim,gsn_garrotte) || is_affected(victim,gsn_garrotte_timer) )
        {
                act("$N is alert and too prepared to get up close.", ch, 0, victim, TO_CHAR);
                return;
        }

	// Begin success calc.
	chance /= 2;

	if (!can_see(victim, ch))
		chance += 25;
	
	if (victim->position == POS_FIGHTING)
		chance /= 2;

	chance += get_curr_stat(ch,STAT_DEX) - get_curr_stat(victim,STAT_DEX);
	chance += get_curr_stat(ch, STAT_STR)/5;
	
	if (ch->pause == 0)
		chance += 10;

	if (victim->pause == 0)
		chance += 10;
	else
		chance -= victim->pause;

	if (IS_AFFECTED(ch, AFF_HASTE))
		chance += 5;
	if (IS_AFFECTED(victim, AFF_HASTE))
		chance -= 5;

	if (IS_CABAL_OUTER_GUARD(victim) || IS_CABAL_GUARD(victim) || IS_IMMORTAL(victim))
		chance = 0;

	chance = UMIN(chance, 85);

        if (number_percent() > chance)
        {
				init_affect(&af);
				af.where	= TO_AFFECTS;
				af.level	= ch->level;
                af.type         = gsn_garrotte_timer;
                af.duration     = number_range(1,5);
				af.location 	= APPLY_CON;
				af.modifier 	= -1 * number_range(4,6);
				af.aftype		= AFT_SKILL;
				af.name			= str_dup("garrotte");
				affect_to_char(victim,&af);
				damage_old(ch, victim, -1 * af.modifier * number_range(30,50), gsn_garrotte, DAM_BASH, TRUE);
				af.location		= APPLY_HITROLL;
				af.modifier		= -1 * GET_HITROLL(victim) / 3;
				affect_to_char(victim,&af);
               check_improve(ch,gsn_garrotte,FALSE,2);
                WAIT_STATE(ch, PULSE_VIOLENCE*2);

                if (!IS_NPC(ch) && !IS_NPC(victim))
		{
			sprintf(buf, "Help! %s is trying to choke me with a cord!", PERS(ch, victim));
                        do_myell(victim, buf);
		}

                multi_hit(victim, ch, TYPE_UNDEFINED);
                return;
        }

	// Success
	act("Catching $N off guard, you manage to slip a length of cord around $S neck and pull it tight.", ch, 0, victim, TO_CHAR);
	act("Catching you off guard, $n slips a length of cord around your neck and pulls it tight.", ch, 0, victim , TO_VICT);
	act("Catching $N off guard, $n slips a length of cord around $N's neck and pulls it tight.", ch, 0, victim, TO_NOTVICT);
	if (victim->position == POS_FIGHTING)
		stop_fighting(victim, TRUE);
        init_affect(&af);
        af.where        = TO_AFFECTS;
        af.aftype       = AFT_MALADY;
        af.type         = gsn_garrotte;
        af.level        = ch->level;
        af.duration     = number_range(2,3);
        af.modifier     = 0;
        af.location     = 0;
        af.bitvector    = AFF_SLEEP;
		char msg_buf[MSL];
		sprintf(msg_buf,"knocked unconscious");
		af.affect_list_msg = str_dup(msg_buf);
        affect_to_char(victim, &af);
        victim->position = POS_SLEEPING;
        check_improve(ch, gsn_garrotte, TRUE, 2);
        WAIT_STATE(ch, PULSE_VIOLENCE*3);
        return;
}

int count_hands(CHAR_DATA *ch)
{
    int count=0;
    OBJ_DATA *weapon;

    if (get_eq_char(ch,WEAR_LIGHT) != NULL)
        count++;
    if (get_eq_char(ch,WEAR_WIELD) != NULL)
    {
        count++;
        weapon = get_eq_char(ch,WEAR_WIELD);

        if ((weapon != NULL && ch->size < SIZE_LARGE
            && IS_WEAPON_STAT(weapon,WEAPON_TWO_HANDS))
            || (weapon != NULL && weapon->value[0]==WEAPON_STAFF)
            || (weapon != NULL && weapon->value[0]==WEAPON_POLEARM)
            || (weapon != NULL && weapon->value[0]==WEAPON_SPEAR))
        {
            // We're wearing a two-handed weapon, that counts for both hands.
            count++;
        }
    }
    if (get_eq_char(ch,WEAR_HOLD) != NULL)
        count++;
    if (get_eq_char(ch,WEAR_SHIELD) != NULL)
        count++;
    if (get_eq_char(ch,WEAR_DUAL_WIELD) != NULL)
        count++;
    if (count > 2)
        bug("Count_hands: Character holding %d items.",count);
    return count;

}

void do_intercept( CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	int skill = get_skill(ch, skill_lookup("intercept"));

	if (IS_NPC(ch) || skill == 0) {
		return send_to_char("You don't know how to do that.\n\r", ch);;
	}

	if (argument[0] == '\0') {
		return send_to_char("Syntax: intercept <on/off>\n\r", ch);
	}

	if (!str_cmp(argument,"on")) {
		if (is_affected(ch, skill_lookup("intercept"))) {
			return;
		}
		init_affect(&af);
		af.aftype = AFT_INVIS;
		af.type = skill_lookup("intercept");
		af.where = TO_AFFECTS;
		af.duration = -1;
		af.level = ch->level;
		af.strippable = FALSE;
		affect_to_char(ch, &af);
		return send_to_char("You will now attempt to intercept items from other thieves.\n\r", ch);
	} else if (!str_cmp(argument,"off"))  {
		if (is_affected(ch, skill_lookup("intercept"))) {
			affect_strip(ch, skill_lookup("intercept"));
		}
		return send_to_char("You will no longer attempt to intercept items from other thieves.\n\r", ch);
	} else {
		return send_to_char("Syntax: intercept <on/off>\n\r", ch);
	}
}
