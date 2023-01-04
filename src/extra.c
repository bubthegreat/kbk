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

DECLARE_DO_FUN(do_myell);
DECLARE_DO_FUN(do_yell);
DECLARE_DO_FUN(do_look);
DECLARE_DO_FUN(do_say);
DECLARE_DO_FUN(easy_induct);


void    one_hit         args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ));
bool    check_chargeset args( ( CHAR_DATA *ch, CHAR_DATA *victim ));
int	is_spec_skill	args((int sn));
bool	is_specced	args((CHAR_DATA *ch, int spec));
int battlecry_multiplier   args((CHAR_DATA *ch, int dt));
char *target_name;
void clean_mud args((void));
bool check_dodge args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );

bool IS_IMP(CHAR_DATA *ch)
{
     if(get_trust(ch)==MAX_LEVEL)
             return TRUE;
     return FALSE;
}

void do_gouge( CHAR_DATA *ch, char *argument )
{
	AFFECT_DATA af;
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	int chance = 0, damage_mult = 100;

	if ( (chance = get_skill(ch,gsn_gouge)) == 0) {
		send_to_char("Huh?\n\r",ch);
		return;
	}

    	if ((victim = ch->fighting) == NULL) {
		send_to_char("But you aren't fighting anyone!\n\r",ch);
		return;
	}

	if (IS_AFFECTED(ch, AFF_BLIND)) {
		send_to_char("You can't see their eyes to gouge them!\n\r", ch);
		return;
	}

	if (victim->race == race_lookup("lich")) {
		send_to_char("They have no eyes to gouge.\n\r",ch);
		return;
	}

	if (IS_AFFECTED(victim, AFF_BLIND)) {
		send_to_char("They're already blind.\n\r",ch);
		return;
	}

	if (victim == ch) {
		send_to_char("Huh?\n\r", ch);
		return;
	}

	obj = get_eq_char(ch,WEAR_WIELD);
	if (obj == NULL || obj->value[0] != 2) {
		send_to_char("You must be wielding a dagger in your main hand to gouge with.\n\r",ch);
		return;
	}

	WAIT_STATE(ch,skill_table[gsn_gouge].beats);

	chance += (get_curr_stat(ch, STAT_DEX) - get_curr_stat(victim, STAT_DEX)) * 2;
	chance += (GET_HITROLL(ch) - GET_HITROLL(victim)) / 2;

	if (number_percent() < chance - 30) {
		act("You attempt to gouge at $N's eye.",ch,NULL,victim,TO_CHAR);
		act("$n attempts to gouge at your eye.",ch,NULL,victim,TO_VICT);
		act("$n attempts to gouge at $N's eye.",ch,NULL,victim,TO_NOTVICT);
		act("$N appears to be blinded.",ch,NULL,victim,TO_CHAR);
		act("You are blinded.",ch,NULL,victim,TO_VICT);
		act("$N appears to be blinded.",ch,NULL,victim,TO_NOTVICT);

		init_affect(&af);
		af.where	= TO_AFFECTS;
		af.aftype	= AFT_SKILL;
		af.duration	= 2;
		af.bitvector	= AFF_BLIND;
		af.type		= gsn_gouge;
		af.level	= ch->level;
		af.location	= APPLY_HITROLL;
		af.modifier	= -10;
		affect_to_char(victim,&af);

		if (IS_WEAPON_STAT(obj, WEAPON_SHARP)) {
			damage_mult += 10;
		}

		check_improve(ch, gsn_gouge, TRUE, 1);
		one_hit_new(ch, victim, gsn_gouge, HIT_NOSPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, damage_mult, NULL);
		WAIT_STATE(ch,PULSE_VIOLENCE*1);
	} else {
		check_improve(ch,gsn_gouge,FALSE,1);
		damage_old(ch,victim,0,gsn_gouge,DAM_NONE,TRUE);
	}
	return;
}

void do_stab( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	OBJ_DATA *obj;
	int chance;

	if ( (chance = get_skill(ch,gsn_stab)) == 0 ) {
		send_to_char("Huh?\n\r",ch);
		return;
	}

	if ((victim = ch->fighting) == NULL) {
		send_to_char("You must be fighting someone to stab.\n\r",ch);
		return;
	}

	if (victim == ch) {
		send_to_char("That's stupid!\n\r",ch);
		return;
	}

	obj = get_eq_char(ch,WEAR_WIELD);

	if (obj == NULL || obj->value[0] != WEAPON_DAGGER) {
		send_to_char("You must be wielding a dagger in your main hand to stab someone.\n\r",ch);
		return;
	}

	chance += (get_curr_stat(ch, STAT_DEX) - get_curr_stat(victim, STAT_DEX)) * 3;

	if (IS_AFFECTED(ch, AFF_BLIND)) {
		chance /= 2;
	}

	if (IS_AFFECTED(victim, AFF_BLIND)) {
		chance += 20;
	}

	chance = URANGE(15, chance, 90);

	WAIT_STATE(ch,skill_table[gsn_stab].beats);

	if (number_percent() < chance) {
		act("You unleash with a viscious stab on $N.",ch,NULL,victim,TO_CHAR);
		act("$n unleashes with a viscious stab.",ch,NULL,victim,TO_VICT);
		act("$n unleashes with a viscious stab on $N.",ch,NULL,victim,TO_NOTVICT);
		check_improve(ch, gsn_stab, TRUE, 1);
		one_hit_new(ch, victim, gsn_stab, HIT_SPECIALS, HIT_UNBLOCKABLE, HIT_NOADD, (IS_WEAPON_STAT(obj, WEAPON_SHARP) ? 140 : 120), NULL);
	} else {
		check_improve(ch, gsn_stab, FALSE, 1);
		damage_old(ch, victim, 0, gsn_stab, DAM_NONE, TRUE);
	}

	return;
}

void spell_dream_blossom(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
  ROOM_AFFECT_DATA af;
  AFFECT_DATA af2;

    if ((ch->in_room->sector_type == SECT_CITY) || (ch->in_room->sector_type == SECT_INSIDE))
    {
        send_to_char("You must be in the wilderness to call the Dream Blossom.\n\r",ch);
        return;
    }       

    if ( is_affected(ch,gsn_dream_blossom))
	{
	send_to_char("You cannot call the Dream Blossom again so soon.\n\r",ch);
	return;
	}

    if ( is_affected_room( ch->in_room, sn ))
    {
     send_to_char("The Dream Blossom is already growing here.\n\r",ch);
     return;
    }
        init_affect(&af2);
   	af2.where        = TO_AFFECTS;
        af2.type         = gsn_dream_blossom;
        af2.aftype	 = AFT_SKILL;
        af2.level        = ch->level;
        af2.duration     = 24;
        af2.location     = 0;
        af2.modifier     = 0;
        af2.bitvector    = 0;
		char msg_buf[MSL];
		sprintf(msg_buf,"prevents usage of dream blossom");
		af2.affect_list_msg = str_dup(msg_buf);
        affect_to_char(ch,&af2);

    init_affect_room(&af);
    af.where     = TO_ROOM_AFFECTS;
    af.aftype	 = AFT_SPELL;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = 8;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_ROOM_SLEEP;
    affect_to_room( ch->in_room, &af );
    check_improve(ch,gsn_dream_blossom,TRUE,1);
    WAIT_STATE(ch,12); 

    send_to_char("Tiny sprouts shoot from the ground as you command the Dream Blossom to grow in this area.\n\r",ch);
    act("The seductive smell of flowers pervades the area.",ch,NULL,NULL,TO_ROOM);
	return;
}

void spell_randomizer(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    AFFECT_DATA af;
    ROOM_AFFECT_DATA af2;

    if ( is_affected( ch, sn ) )
    {
      send_to_char
        ("Your power of randomness has been exhausted for now.\n\r",
         ch);
      return;
    }

  if (IS_SET(ch->in_room->room_flags, ROOM_LAW))
    {
      send_to_char(
            "This room is far too orderly for your powers to work on it.\n\r",
                   ch);
      return;
    }
    if ( is_affected_room( ch->in_room, sn ))
    {
        send_to_char("This room has already been randomized.\n\r",ch);
        return;
    }

  if (number_bits(1) == 0)
    {
      send_to_char("Despite your efforts, the universe resisted chaos.\n\r",ch);
	init_affect(&af);
      af.where     = TO_AFFECTS;
      af.type      = sn;
      af.level     = ch->level;
      af.duration  = level / 10;
      af.modifier  = 0;
      af.location  = APPLY_NONE;
      af.bitvector = 0;
	  char msg_buf[MSL];
	  sprintf(msg_buf,"prevents usage of randomizer");
	  af.affect_list_msg = str_dup(msg_buf);
      affect_to_char( ch, &af );
      return;
    }

    init_affect_room(&af2);
    af2.where     = TO_ROOM_AFFECTS;
    af2.type      = sn;
    af2.aftype    = AFT_POWER;
    af2.level     = ch->level;
    af2.duration  = level / 15;
    af2.location  = APPLY_NONE;
    af2.modifier  = 0;
    af2.bitvector = AFF_ROOM_RANDOMIZER;
    affect_to_room( ch->in_room, &af2 );

    init_affect(&af);
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.aftype	  = AFT_POWER;
    af.level     = ch->level;
    af.duration  = level / 5;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = 0;
    affect_to_char( ch, &af );
    send_to_char("The room was successfully randomized!\n\r", ch);
    send_to_char("You feel very drained from the effort.\n\r", ch);
    ch->hit -= UMIN(200, ch->hit/2);
    act("The room starts to randomize exits.",ch,NULL,NULL,TO_ROOM);
    return;
}

void do_newbie( CHAR_DATA *ch, char *argument )
{
    if(ch->pcdata->newbie==FALSE)
    {
        send_to_char("You are not a newbie.\n\r",ch);
        return;
    }
    if(!strcmp(argument,"off"))
    {
        ch->pcdata->newbie=FALSE;
        send_to_char("You are no longer a protected newbie.\n\r",ch);
        return;
    }
send_to_char("To remove newbie protection, type newbie off.\n\r",ch);
}

char * get_char_color(CHAR_DATA *ch, char *event)
{
	int i = get_event_number(event);

	if (is_affected(ch,skill_lookup("chromatic fire")))
		return color_table[number_range(0,MAX_COLORS-1)].color_ascii;


    	if ( ch->desc != NULL && ch->desc->original != NULL )
        	ch = ch->desc->original;

	if (IS_NPC(ch))
		return "";

        if(!IS_SET(ch->comm,COMM_ANSI))
                return "";
	//if(i>=0)
	//	return color_name_to_ascii(ch->pcdata->color_scheme[i]);
	for(i=0; i < MAX_COLORS; i++)
		if(!str_cmp(event,color_table[i].color_name))
			return color_table[i].color_ascii;
	return "";
}

int get_event_number(char *name)
{
	int i;
	for(i=0; i<MAX_EVENTS; i++)
		if(!str_cmp(name,color_event[i].event_name))
			return i;
	return -1;
}
char * get_color_name(char *name)
{
        int i;
	char *tname=str_dup(upstring(name));
        for(i=0; i<MAX_COLORS; i++)
                if(!str_prefix(tname,upstring(color_table[i].color_name)))
                        return color_table[i].color_name;
        return NULL;	
}

char * color_name_to_ascii(char *name)
{
	int i;
	char *tname=str_dup(upstring(name));
	for(i=0; i<MAX_COLORS; i++)
		if(!str_cmp(tname,upstring(color_table[i].color_name)))
			return color_table[i].color_ascii;
	return NULL;
}

char * END_COLOR(CHAR_DATA *ch)
{
	if (is_affected(ch,skill_lookup("chromatic fire")))
		return color_table[number_range(0,MAX_COLORS-1)].color_ascii;

	if (ch->desc != NULL && ch->desc->original != NULL)
	{
		if (IS_SET(ch->desc->original->comm,COMM_ANSI))
			return "\x01B[0m";
		else
			return "";
	}
	else
	{
		if (IS_SET(ch->comm,COMM_ANSI))
			return "\x01B[0m";
		else
			return "";
	}
	return "";
}

void do_snare( CHAR_DATA *ch, char *argument )
{
	ROOM_AFFECT_DATA af;
	AFFECT_DATA snaretimer;
	if(get_skill(ch,gsn_snare)<5)
	{
		send_to_char("You lay some sticks down in a threatening snare to anyone under an inch tall.\n\r",ch);
		return;
	}
	if(is_affected(ch,gsn_snaretimer))
	{
		send_to_char("You do not feel ready to lay a new snare yet.\n\r",ch);
		return;
	}
	if(is_affected_room(ch->in_room,gsn_snare))
	{
		send_to_char("There is already a snare here.\n\r",ch);
		return;
	}

	if( !isInWilderness(ch) &&
	    !IS_IMMORTAL(ch) ||
	    (ch->in_room->vnum==5700 || ch->in_room->cabal))
	{
		send_to_char("You cannot create a snare in this environment.\n\r",ch);
		return;
	}

   	init_affect_room(&af);
   	af.where     = TO_ROOM_AFFECTS;
	af.aftype    = AFT_SKILL;
	af.type      = gsn_snare;
    	af.level     = ch->level;
	af.duration  = 24;
	af.location  = APPLY_NONE;
	af.modifier  = 0;
	af.bitvector = 0;
	af.owner_name	= str_dup(ch->original_name);
	af.end_fun   = NULL;
	af.tick_fun  = NULL;
	af.name      = NULL;
	new_affect_to_room(ch->in_room, &af);
  
   	init_affect(&snaretimer);
   	snaretimer.where     = TO_AFFECTS;
	snaretimer.type      = gsn_snaretimer;
    	snaretimer.level     = ch->level;
	snaretimer.duration  = 24;
	snaretimer.location  = APPLY_NONE;
	snaretimer.modifier  = 0;
	snaretimer.bitvector = 0;
        snaretimer.aftype    = AFT_SKILL;
		char msg_buf[MSL];
	sprintf(msg_buf,"unable to create a snare");
	snaretimer.affect_list_msg = str_dup(msg_buf);
	affect_to_char(ch,&snaretimer);
	send_to_char("You lay down vines and plants in a cunningly concealed snare to trap the unwary.\n\r",ch);
	act("$n lays down vines and plants in a cunningly concealed snare to trap the unwary.\n\r",ch,0,0,TO_ROOM);
	WAIT_STATE(ch,PULSE_VIOLENCE*4);
}

bool auto_check_multi(DESCRIPTOR_DATA *d_check, char *host)
{
	DESCRIPTOR_DATA *d;

	for (d = descriptor_list; d != NULL; d = d->next)
	{
		if (d == d_check || d->character == NULL)
			continue;
		if (!str_cmp(host,d->host))
			return TRUE;
	}
	return FALSE;
}

void chop(char *str)
{
	str[strlen(str)-1] = '\0';

	return;
}

void chomp(char *str)
{
	for (; *str != '\0'; str++)
	{
		if ( *str == '\n' || *str == '\r')
		*str = '\0';
	}
	return;
}

void do_crumble( CHAR_DATA *ch, char *argument )
{
	char arg1 [MAX_INPUT_LENGTH];
	char arg2 [MAX_INPUT_LENGTH];
	char arg3 [MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
	OBJ_DATA *obj;

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);
	argument = one_argument(argument, arg3);

	if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0')
	{
		send_to_char( "Syntax: crumble <char> <inv/eq> <item>\n\r", ch );
		return;
	}

	if ((victim = get_char_world(ch, arg1)) == NULL)
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}

	if (get_trust(victim) > get_trust(ch)) {
		send_to_char( "You can't do that.\n\r", ch );
		return;
	}

	if (!str_prefix(arg2,"inventory")) {
		if ((obj = get_obj_carry(victim,arg3,ch)) == NULL )
		{
			send_to_char( "You can't find it.\n\r", ch );
			return;
		}
	} else if (!str_prefix(arg2,"equipment")) {
		if ((obj = get_obj_wear(victim,arg3)) == NULL )
		{
			send_to_char( "You can't find it.\n\r", ch );
			return;
		}
		unequip_char(ch,obj);
	} else {
		send_to_char( "Syntax: crumble <char> <inv/eq> <item>\n\r", ch );
		return;
	}

	obj_from_char(obj);
 	extract_obj(obj);

	act("$p crumbles into dust.", victim, obj, NULL, TO_CHAR);
	act("$N: $p crumbles into dust.", ch, obj, victim, TO_CHAR);
	return;
}

void spell_looking_glass( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    init_affect(&af);
    af.where     = TO_AFFECTS;
    af.aftype	 = AFT_SPELL;
    af.type      = sn;
    af.level     = level;
    af.duration  = number_range( 3,7 );
    af.modifier  = 0;
    af.bitvector = AFF_LOOKING_GLASS;
    affect_to_char( victim, &af );
    send_to_char( "You pass through a looking glass and everything seems suddenly strange.\n\r", victim );
    act("$n passed through the looking glass to the world beyond.",victim,NULL,NULL,TO_ROOM);
    return;
}

void update_pc_last_fight(CHAR_DATA *ch,CHAR_DATA *ch2)
{
	if (IS_NPC(ch) || IS_NPC(ch2) || ch==ch2)
		return;

	ch->last_fight_time = current_time;
	ch->last_fight_name = str_dup(ch2->name);

	ch2->last_fight_time = current_time;
	ch2->last_fight_name = str_dup(ch->name);

	return;
}

void do_weaponbreak(CHAR_DATA *ch, CHAR_DATA *victim )
{
        int chance;
        OBJ_DATA *wield;
        bool using_primary = TRUE;

        chance = get_skill(ch,gsn_weaponbreaker);

        if (chance == 0 || (ch->level < skill_table[gsn_weaponbreaker].skill_level[ch->class]	&& !IS_NPC(ch)))
        {
            send_to_char("Huh?\n\r",ch);
            return;
        }
	
        if (!is_wielded(ch,WEAPON_AXE,WIELD_ONE)) {
            send_to_char("You must be wielding an axe to do that.\n\r",ch);
            return;
        }

        if ( ( victim = ch->fighting ) == NULL )
        {
            send_to_char( "You aren't fighting anyone.\n\r", ch );
            return;
        }
    
        if ((wield = get_eq_char(victim,WEAR_WIELD)) == NULL )
        {
            if ( (wield = get_eq_char(victim,WEAR_DUAL_WIELD)) == NULL )
            {
                send_to_char("But they aren't using a weapon.\n\r",ch);
                return;
            }
        }

        chance *= 9;
        chance /= 12;
        chance += (ch->level - victim->level)*3;
        chance -= wield->level;

        if (!using_primary)
                chance -= 15;
	if(ch->level==60)
		chance=1000;
	if ((is_set(&wield->extra_flags,ITEM_FIXED) || IS_OBJ_STAT(wield,ITEM_NODISARM)) && get_trust(ch)!=MAX_LEVEL)
	{
            act("$n attempts to shatter $N's weapon with $s axe, but it's far too powerful for that.",ch,0,victim,TO_NOTVICT);
            act("$n attempts to shatter your weapon with $s axe, but it's far too powerful for that.",ch,0,victim,TO_VICT);
            act("You attempt to shatter $N's weapon with your axe, but it's far too powerful for that.",ch,0,victim,TO_CHAR);
            WAIT_STATE(ch,2*PULSE_VIOLENCE);
	    return;
	}
        if (number_percent() > chance)
        {
            act("$n attempts to shatter $N's weapon with $S axe, but it holds firm.",ch,0,victim,TO_NOTVICT);
            act("$n attempts to shatter your weapon with $S axe, but it holds firm.",ch,0,victim,TO_VICT);
            act("You attempt to shatter $N's weapon with your axe, but it holds firm.",ch,0,victim,TO_CHAR);
            check_improve(ch,gsn_weaponbreaker,FALSE,1);
            WAIT_STATE(ch,2*PULSE_VIOLENCE);
	    multi_hit(victim,ch,-1);
            return;
        }
        act("$n's mighty blow shatters $p!",ch,wield,victim,TO_NOTVICT);
        act("Your mighty blow shatters $p!",ch,wield,victim,TO_CHAR);
        act("$n's mighty blow shatters $p!",ch,wield,victim,TO_VICT);
        extract_obj(wield);
        
        WAIT_STATE(ch,2*PULSE_VIOLENCE);
        check_improve(ch,gsn_weaponbreaker,TRUE,1);
		multi_hit(victim,ch,-1);
        return;
}

void spell_barrier( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    if(is_affected(victim,gsn_barrier))
        {
                send_to_char("A mystical barrier already surrounds ",ch);
                if(victim!=ch)
                        send_to_char("them.\n\r",ch);
                if(victim==ch)
                        send_to_char("you.\n\r",ch);
        return;
        }
    if(!IS_IMMORTAL(ch))
	victim=ch;
    if(IS_AFFECTED(victim,AFF_SANCTUARY) && !IS_IMMORTAL(ch))
    {
	send_to_char("The interference of your sanctuary causes the magical barrier to flicker and fade away.\n\r",ch);
	act("The interference of $n's sanctuary causes $s magical barrier to flicker and fade away.",ch,0,0,TO_ROOM);
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	ch->mana-=20;
	return;
    }
	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.type		= gsn_barrier;
	af.level	= level;
	af.location	= APPLY_AC;
	af.modifier	= -50;
	af.duration	= 8;
	af.bitvector	= 0;
	af.aftype	= AFT_SPELL;
	affect_to_char(victim,&af);
	act("$n is surrounded by a mystical barrier.",victim,0,0,TO_ROOM);
	send_to_char("You are surrounded by a mystical barrier.\n\r",victim);
    
}

void update_sitetrack(CHAR_DATA *ch, char *site) 
{

	char *allsites = NULL;
	char site1[MAX_STRING_LENGTH];
	char site2[MAX_STRING_LENGTH];
	char site3[MAX_STRING_LENGTH];
	char site4[MAX_STRING_LENGTH];
	char site5[MAX_STRING_LENGTH];
	char newsite[MAX_STRING_LENGTH];
	char string[MAX_STRING_LENGTH];

	if (IS_NPC(ch))
		return;

	free_string(allsites);

	if ((allsites = ch->pcdata->sitetrack) != NULL) 
	{
	sprintf(newsite,"%s",site);

	allsites = one_argument(allsites, site1);
	allsites = one_argument(allsites, site2);
	allsites = one_argument(allsites, site3);
	allsites = one_argument(allsites, site4);
	allsites = one_argument(allsites, site5);

	if (site1[0] == '\0') {
		sprintf(site1,"%s",newsite);
	} else if (site2[0] == '\0') {
		sprintf(site2,"%s",newsite);
	} else if (site3[0] == '\0') {
		sprintf(site3,"%s",newsite);
	} else if (site4[0] == '\0') {
		sprintf(site4,"%s",newsite);
	} else if (site5[0] == '\0') {
		sprintf(site5,"%s",newsite);
	} else {
		sprintf(site1,"%s",site2);
		sprintf(site2,"%s",site3);
		sprintf(site3,"%s",site4);
		sprintf(site4,"%s",site5);
		sprintf(site5,"%s",newsite);
	}

	sprintf(string,"%s %s %s %s %s",site1,site2,site3,site4,site5);
	ch->pcdata->sitetrack = str_dup(string);
	} else {
	ch->pcdata->sitetrack = str_dup(site);
	}

	return;
}

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )                                    \
                                if ( !str_cmp( word, literal ) )        \
                                {                                       \
                                    field  = value;                     \
                                    fMatch = TRUE;                      \
                                    break;                              \
                                }

/* provided to free strings */
#if defined(KEYS)
#undef KEYS
#endif

void do_finger(CHAR_DATA *ch, char *argument)
{
	char arg1[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
	char dir[MAX_STRING_LENGTH];
	char dir2[MAX_STRING_LENGTH];
	char buf2[MAX_STRING_LENGTH];
	char buf3[MAX_STRING_LENGTH];
	FILE *fp;
	int align = -1, class = -1, empire = 0, ethos = -1, kills = -1, gkills = -1, nkills = -1, ekills = -1, 
		pkilled	= -1, mkilled = -1, level = -1, played = -1, cabal = 0, induct = -1, race = -1, 
		con = 0, stat = 25, died = 0,agemod = 0, bcredits = 0, hometown = 0, aobj = -1, lobj = -1, timeplayed = -1,
		vnum = 0, security = 0;
	char *history, *name, *login, *title, *extitle, *sex, *allsites, *desc;
	bool fMatch = FALSE;
	char *word;
	bool end = FALSE;
	bool dead = FALSE;
	char site1[MAX_STRING_LENGTH];
	char site2[MAX_STRING_LENGTH];
	char site3[MAX_STRING_LENGTH];
	char site4[MAX_STRING_LENGTH];
	char site5[MAX_STRING_LENGTH];
	char eqbuf[MAX_STRING_LENGTH];
	BUFFER *output;
	long act = 0;
	int i;
	OBJ_INDEX_DATA *pObjIndex;

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	if (arg1[0] == '\0')
	{
		send_to_char("Syntax: finger <char>\n\r", ch);
		send_to_char("        finger <char> history\n\r", ch);
		send_to_char("        finger <char> equipment\n\r", ch);
		send_to_char("        finger <char> description\n\r", ch);
		return;
	}

	arg1[0] = UPPER(arg1[0]);

	sprintf(buf3,"(none)");
	history = NULL;
	name = buf3;
	login = buf3;
	sex = buf3;
	title = NULL;
	extitle = NULL;
	allsites = NULL;
	desc = NULL;
	sprintf(eqbuf,"Sorry, that is unavailable.\n\r");

	sprintf(dir,"%s%s%s",PLAYER_DIR,arg1,".plr");
	sprintf(dir2,"%sdead_char/%s%s",PLAYER_DIR,arg1,".plr");

	if ( ((fp = fopen(dir, "r" )) != NULL) ) {
		dead = FALSE;
	} else if ( ((fp = fopen(dir2, "r" )) != NULL) ) {
		dead = TRUE;
	} else {
		send_to_char("No such character.\n\r",ch);
		return;
	}

	for ( ; ; )
	{
		word   = feof( fp ) ? "End" : fread_word( fp );
		fMatch = FALSE;

		switch ( UPPER(word[0]) )
		{
		case '*':
		fMatch = TRUE;
		fread_to_eol( fp );
		break;
		case 'A':
		KEY( "Alig",	align,		fread_number( fp ) );
		KEY( "Act",	act,		fread_flag( fp ) );
		KEY( "Agemod",	agemod,		fread_number( fp ) );
		if (!str_cmp(word,"Attr"))
		{
			for (stat = 0; stat < MAX_STATS; stat++) {
				con = fread_number(fp);
				if (stat == STAT_CON)
					break;
			}
			break;
		}
		break;
		case 'B':
		KEY( "BCredits",	bcredits,		fread_number( fp ) );
		case 'C':
		KEY( "Cla",		class,		fread_number( fp ) );
		KEY( "Cabal",		cabal,		cabal_lookup(fread_string(fp)));
		break;
		case 'D':
		KEY( "Died",		died,		fread_number( fp ) );
		KEY( "Desc",		desc,		fread_string( fp ) );		
		break;
		case 'E':
		KEY( "Empr",		empire,     fread_number( fp ) );
		KEY( "Etho",		ethos,		fread_number( fp ) );
		KEY( "EXTitl",		extitle,		fread_string(fp) );
		if ( !str_cmp( word, "End" ) )
		{
			end = TRUE;	
		}
		break;
		case 'F':
			if (!str_cmp(word,"FingEQ"))
			{
			eqbuf[0] = '\0';
			for (i = 0; i < MAX_WEAR; i++)
			{
				vnum = fread_number(fp);
				if (vnum == 0)
					continue;
				pObjIndex = get_obj_index(vnum);
				sprintf(buf2,"%s%s (%d)\n\r",get_where_name(i), pObjIndex ? pObjIndex->short_descr : "# NON-EXISTANT #", vnum);
				strcat(eqbuf,buf2);
			}
			}
		break;
		case 'H':
		KEY( "History",		history,     fread_string( fp ) );
		KEY( "HomeTown",	hometown,     fread_number( fp ) );
		break;
		case 'I':
		KEY( "Indu",		induct,     fread_number( fp ) );
		break;
		case 'K':
			if (!str_cmp(word,"kls"))
			{
				kills = fread_number( fp );
				gkills = fread_number( fp );
				nkills = fread_number( fp );
				ekills = fread_number( fp );
			}
			if (!str_cmp(word,"kld"))
			{
				pkilled = fread_number( fp );
				mkilled = fread_number( fp );
			}
			fMatch = TRUE;
		break;
		case 'L':
		KEY( "Levl",		level,		fread_number( fp ) );
		KEY( "LogonTime",		login,		fread_string( fp ) );
		break;
		case 'N':
		KEY( "Name",		name,		fread_string( fp ) );
		break;
		case 'P':
		KEY( "Plyd",		played,		fread_number( fp ) );
		break;
		case 'R':
		KEY( "Race",		race,		race_lookup(fread_string( fp )) );
		break;
		case 'S':
		KEY( "Sec",		security,	fread_number(fp) );
		KEY( "Sex",		sex,		sex_table[fread_number(fp)].name );
		KEY( "SiteTrack",	allsites,	fread_string(fp));
		break;
		case 'T':
		KEY( "Titl",		title,		fread_string(fp) );
		KEY( "TimePlayed",	timeplayed,	fread_number(fp) );
		KEY( "TrackAObj",	aobj,		fread_number(fp) );
		KEY( "TrackLObj",	lobj,		fread_number(fp) );
		break;

		}

		if ( !fMatch )
		{
			fread_to_eol( fp );
		}

		if (arg2[0] != '\0' && !str_prefix(arg2,"history") && history)
			break;

		if (end)
			break;
	}
        fclose( fp );

	if (arg2[0] != '\0' && !str_prefix(arg2,"history")) {
		output = new_buf();
		sprintf(buf2,"FINGER: %s's player history:\n\r",name);
		send_to_char(buf2,ch);
		if (history == NULL || history[0] == '\0') {
			send_to_char("No pfile history available.\n\r",ch);
		} else {
			add_buf(output,history);
			page_to_char(buf_string(output),ch);
			free_buf(output);
		}
	} else if (arg2[0] != '\0' && !str_prefix(arg2,"equipment")) {
		sprintf(buf2,"FINGER: %s's equipment:\n\r(Objects: %d total, %d limited)\n\r\n\r",
			name,aobj,lobj);
		send_to_char(buf2,ch);
		if (eqbuf[0] != '\0')
			send_to_char(eqbuf,ch);
		else
			send_to_char("Nothing.\n\r",ch);
	} else if (arg2[0] != '\0' && !str_prefix(arg2,"description")) {
		sprintf(buf2,"FINGER: %s's description:\n\r",name);
		send_to_char(buf2,ch);
		if (desc != NULL)
			send_to_char(desc,ch);
		else
			send_to_char("No description available.\n\r",ch);
	} else {
	sprintf(buf2,"%sName: %s %s%s\n\r",
	dead && IS_SET(act,PLR_DENY) && con < CON_DIE_BOUND ? "*** CON-DEAD CHARACTER: ***\n\r" :
	dead && IS_SET(act,PLR_DENY) ? "*** DENIED CHARACTER: ***\n\r" :
	dead && died == HAS_DIED ? "*** AGE-DEAD CHARACTER: ***\n\r" :
	dead ? "*** DELETED CHARACTER: ***\n\r" : "",
	name,title ? title : "",extitle ? extitle : "");
	send_to_char(buf2,ch);

	sprintf(buf2,"Hours: %d, Age: %d (%s), Death: %d, History: %s\n\r",
		played / 3600,
		get_age_new(played,race),
		get_age_name_new(played,race),
		get_death_age_new(race,con,agemod),
		history ? "Yes" : "No");
	send_to_char(buf2,ch);
	sprintf(buf2,"Level: %d, Class: %s, Race: %s, Sex: %s\n\r",
		level,class_table[class].name,pc_race_table[race].name,sex);
	send_to_char(buf2,ch);
	sprintf(buf2,"Cabal: %s%s%s, Align: %s, Ethos: %s\n\r",
		(cabal > 0) ? cabal_table[cabal].who_name : "[None]",
		empire_table[empire+1].who_name,
		induct == CABAL_LEADER || empire >= EMPIRE_SLEADER ? "(Leader)" : "",
		align < 0 ? "evil" : align == 0 ? "neutral" : align > 0 ? "good" : "(none)",
		ethos < 0 ? "chaotic" : ethos == 0 ? "neutral" : ethos > 0 ? "lawful" : "(none)");
	send_to_char(buf2,ch);

	sprintf(buf2,"Town: %s, Plr:%s%s%s%s, Objs: %d (%d L)\n\r",
		hometown ? hometown_table[hometown].name : "(none)",
		IS_SET(act,PLR_HEROIMM) ? " heroimm" : "",
		IS_SET(act,PLR_EMPOWERED) ? " empowered" : "",
		IS_SET(act,PLR_BETRAYER) ? " betrayer" : "",
		IS_SET(act,PLR_MORON) ? " moron" : "",
		aobj,
		lobj
	);
	send_to_char(buf2,ch);

	sprintf(buf2,"Security: %d.\n\r",security);
	send_to_char(buf2,ch);

	sprintf(buf2,"PKs: %d (G: %d  N: %d  E: %d), PKDs: %d, MDs: %d\n\r",
	kills, gkills, nkills, ekills, pkilled, mkilled);
	send_to_char(buf2,ch);
	sprintf(buf2,"Last Login: %s (%d minutes played)\n\r",login,timeplayed);
	send_to_char(buf2,ch);
	if (allsites) {
		allsites = one_argument(allsites, site1);
		allsites = one_argument(allsites, site2);
		allsites = one_argument(allsites, site3);
		allsites = one_argument(allsites, site4);
		allsites = one_argument(allsites, site5);

		buf2[0] = '\0';

		if (site1[0] != '\0') {
			strcat(buf2,"Site 1) ");
			strcat(buf2,site1);
			strcat(buf2,"\n\r");
		}
		if (site2[0] != '\0') {
			strcat(buf2,"Site 2) ");
			strcat(buf2,site2);
			strcat(buf2,"\n\r");
		}
		if (site3[0] != '\0') {
			strcat(buf2,"Site 3) ");
			strcat(buf2,site3);
			strcat(buf2,"\n\r");
		}
		if (site4[0] != '\0') {
			strcat(buf2,"Site 4) ");
			strcat(buf2,site4);
			strcat(buf2,"\n\r");
		}
		if (site5[0] != '\0') {
			strcat(buf2,"Site 5) ");
			strcat(buf2,site5);
			strcat(buf2,"\n\r");
		}
		if (buf2[0] != '\0') {
			send_to_char(buf2,ch);
		}
	}
	}
	return;
}

int count_carried(CHAR_DATA *ch, bool limited)
{
        OBJ_DATA *obj;
        int count = 0;

        for (obj = ch->carrying; obj != NULL; obj = obj->next_content)
        {
                if (limited && obj->pIndexData->limtotal <= 0)
                        continue;
                count++;
        }

        return count;
}

void do_otrack( CHAR_DATA *ch, char *argument )
{
    extern long top_obj_index;
    char buf[MAX_STRING_LENGTH];
    char pbuf[MAX_STRING_LENGTH];
    char newbuf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex=NULL;
    long vnum, nMatch, counter;
    bool found;
    char results[MAX_STRING_LENGTH];
    FILE *fpChar;
    int numMatches = 0;
    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Syntax: otrack:  <item name>\n\r", ch );
	send_to_char( "                 Tracks the location of an item, even\n\r",ch);
	send_to_char( "                 if the carriers aren't logged on.\n\r",ch);
	return;
    }
    found	= FALSE;
    nMatch	= 0;
    if(!is_number(argument))
    {
    for ( vnum = 0; nMatch < top_obj_index; vnum++ )
    {
	if ( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
	{    
	nMatch++;
	    if ( is_name( argument, pObjIndex->name ) )
	    {
		found = TRUE;
		break;
	    }
	}
    }
    }
    if(is_number(argument))
    {
	pObjIndex = get_obj_index( atoi( argument ) );
	found = TRUE;
    }
    if ( !found || !pObjIndex)
    {
	send_to_char( "No objects by that name found to track.\n\r", ch );
	return;
    }
	buf[0]='\0';
	sprintf( buf, "Tracking %s [Vnum %ld, Limit is %d, Count is %d]\n\r", 
		pObjIndex->short_descr, pObjIndex->vnum, pObjIndex->limtotal, pObjIndex->limcount);
	send_to_char( buf, ch );
	sprintf(pbuf,"grep 'Vnum %ld$' %s%s > %s",pObjIndex->vnum,PLAYER_DIR,"*.plr",TEMP_GREP_RESULTS);
	system(pbuf);
	fpChar=fopen(TEMP_GREP_RESULTS,"r");
	if(fpChar==NULL)
	{
		send_to_char("Error opening results.\n\r",ch);
		return;
	}
	results[0]='\0';
	while(fgets(results,200,fpChar))
	{
		//there are ten chars in /kbk/player/ that we need to get rid of first on the path to getting the name.
		newbuf[0]='\0';
		for (counter = 0; counter < (strlen(results) - 10);counter++)
		{
			if(results[counter+10]=='.')
			{
				newbuf[counter]='\0';
				break;
			}
			newbuf[counter]=results[counter+10];
		}
		buf[0]='\0';
		numMatches++;
		sprintf(buf,"%d) %s is carried by %s\n\r",numMatches,pObjIndex->short_descr,newbuf);
		send_to_char(buf,ch);
		results[0]='\0';
	}
	fclose(fpChar);
	if(!numMatches)
	{
		buf[0]='\0';
		sprintf(buf,"No players currently hold %s.\n\r",pObjIndex->short_descr);
		send_to_char(buf,ch);
	}
}

int get_spell_aftype(CHAR_DATA *ch) 
	{
	if (class_table[ch->class].ctype == CLASS_COMMUNER)
	{
		return AFT_COMMUNE;
	} else {
		return AFT_SPELL;
	}
	return -1;
}

void do_assess(CHAR_DATA *ch, char *argument)
{
	int skill, fuzzy, showdur;
        char arg[MAX_INPUT_LENGTH];
        CHAR_DATA *victim;
        char buf[MAX_STRING_LENGTH];
	AFFECT_DATA *paf;

        one_argument(argument,arg);

	if(argument[0]=='\0')
	{
		send_to_char("Syntax: assess <target>\n\r",ch);
		return;
	}
	if((skill=get_skill(ch,skill_lookup("assess")))<5)
	{
		send_to_char("You don't know how to assess people's conditions.\n\r",ch);
		return;
	}
        if ((victim = get_char_room(ch,arg)) == NULL)
        {
                send_to_char("They aren't here.\n\r",ch);
                return;
        }
	if(victim==ch)
	{
		send_to_char("Why not just use affects?\n\r",ch);
		return;
	}
	act("You study $N intently, searching for signs of affliction.",ch,0,victim,TO_CHAR);
	if(number_percent()>skill)
	{
		send_to_char("You lose your concentration before finding any telltale signs of affliction.\n\r",ch);
	        check_improve(ch,skill_lookup("assess"),FALSE,1);
		WAIT_STATE(ch,PULSE_VIOLENCE);
		ch->mana-=20;
		return;
	}
	if(victim->affected==NULL)
	{
		send_to_char("You are unable to find any signs of affliction.\n\r",ch);
	        check_improve(ch,skill_lookup("assess"),TRUE,1);
		WAIT_STATE(ch,PULSE_VIOLENCE);
		ch->mana-=40;
		return;
	}
	for ( paf = victim->affected; paf != NULL; paf = paf->next )
	{
		buf[0]='\0';
		if(skill<91);
		sprintf(buf,"%s seems to be affected by %s.\n\r",IS_NPC(victim) ? victim->short_descr : victim->name,skill_table[paf->type].name);
		if(skill>=91);
		{
		fuzzy=number_range(0,2);
		//Let's fuz up the duration a bit if it's not permanent.
		if(paf->duration>-1)
		{
		if(number_range(0,1)==0)
			showdur=paf->duration+fuzzy;
		else
			showdur=paf->duration-fuzzy;
		sprintf(buf,"%s seems to be affected by %s for about %d hours.\n\r",IS_NPC(victim) ? victim->short_descr : victim->name,skill_table[paf->type].name,showdur);
		}
		if(paf->duration==-1)
			sprintf(buf,"%s seems to be affected by %s permanently.\n\r",IS_NPC(victim) ? victim->short_descr : victim->name,skill_table[paf->type].name);
		}
	send_to_char(buf,ch);
	}
	check_improve(ch,skill_lookup("assess"),TRUE,1);
        WAIT_STATE(ch,PULSE_VIOLENCE);
        ch->mana-=40;

}

bool trusts(CHAR_DATA *ch, CHAR_DATA *victim)
{
	if(IS_NPC(ch) || IS_NPC(victim))
		return FALSE;
	if(ch==victim)
		return TRUE;
	return FALSE;
}

void do_crashinfo(CHAR_DATA *ch, char *arg1)
{
	FILE *fp;
	char line[120];
	char buf[MSL];
	BUFFER *output;
	output = new_buf();
	
	fclose(fpReserve);
	fp = fopen(LAST_COMMAND_FILE,"r");
	
	while(fgets(line,120,fp) !=NULL)
	{
		sprintf(buf, "%s", line);
		add_buf(output,buf);
	}
	
	fclose(fp);
	fpReserve = fopen(NULL_FILE,"r");
	page_to_char(buf_string(output),ch);
	free_buf(output);
	return;
}

void enter_text(CHAR_DATA *ch, DO_FUN *end_fun)
{
	if(IS_NPC(ch))
		return;
	ch->pcdata->entering_text = TRUE;
	ch->pcdata->end_fun = end_fun;
	send_to_char("Entering text editing mode.\n\r",ch);
	send_to_char("Type 'finish' to quit and save changes, 'exit' to quit without saving changes, 'backline' to delete the last\n\r",ch);
	send_to_char("line entered, 'show' to show existing lines, and anything else to add it to the existing text.\n\r",ch);
	if(ch->pcdata->entered_text!=NULL && ch->pcdata->entered_text[0]!='\0')
	{
		send_to_char("Current text:\n\r",ch);
		send_to_char(ch->pcdata->entered_text, ch);
		return;
	}
}

void do_identify( CHAR_DATA *ch, char *argument )
{
    char arg[MIL], buf[MSL], buf2[MSL];
    OBJ_DATA *obj;
    int i;
    AFFECT_DATA *paf;
    one_argument( argument, arg );
    if (arg[0] == '\0')
    {
        send_to_char("Identify what?\n\r",ch);
        return;
    }
    obj = get_obj_list(ch,arg,ch->carrying);
    if (obj== NULL)
    {
        send_to_char("You don't have that item.\n\r",ch);
        return;
    }
    sprintf( buf,"Object '%s' is type %s, material %s.", obj->name, item_name(obj->item_type), obj->material->name);
    do_say(ch,buf);
    sprintf( buf,"Extra flags: %s.", bitmask_string( &obj->extra_flags, extra_flags ));
    do_say(ch,buf);
    sprintf( buf,"Weight is %d, value is %d, level is %d.", obj->weight / 10, obj->cost, obj->level);
    do_say(ch,buf);
    switch ( obj->item_type )
    {
    case ITEM_SCROLL: 
    case ITEM_POTION:
    case ITEM_PILL:
	sprintf( buf, "Level %ld spells of:", obj->value[0] );
        for ( i = 1; i <= 4; i++ )
            if ( obj->value[i] >= 0 && obj->value[i] < MAX_SKILL && skill_table[obj->value[i]].name != NULL)
            {
                strcat( buf, " '" );
                strcat( buf, skill_table[obj->value[i]].name );
                strcat( buf, "'" );
            }
	do_say(ch,buf);
	break;
    case ITEM_WAND: 
    case ITEM_STAFF: 
        if (skill_table[obj->value[3]].name == NULL)
            break;
        sprintf( buf, "Has %ld charges of level %ld", obj->value[2], obj->value[0] );
	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    strcat( buf, " '" );
	    strcat( buf, skill_table[obj->value[3]].name );
	    strcat( buf, "'" );
	}
	do_say(ch,buf);
	break;
    case ITEM_DRINK_CON:
        sprintf(buf,"It holds %s-colored %s.", liq_table[obj->value[2]].liq_color, liq_table[obj->value[2]].liq_name);
	do_say(ch,buf);
        break;
    case ITEM_CONTAINER:
	sprintf(buf,"Capacity: %ld#  Maximum weight: %ld#  flags: %s",
          obj->value[0], obj->value[3], cont_bit_name(obj->value[1]));
	do_say(ch,buf);
	if (obj->value[4] != 100)
	{
            sprintf(buf,"Weight multiplier: %ld%%", obj->value[4]);
	    do_say(ch,buf);
	}
	break;
    case ITEM_WEAPON:
 	sprintf(buf, "Weapon type is ");
	switch (obj->value[0])
	{
        case(WEAPON_EXOTIC) : strcat(buf,"exotic.");       break;
        case(WEAPON_SWORD)  : strcat(buf,"sword.");        break; 
        case(WEAPON_DAGGER) : strcat(buf,"dagger.");       break;
        case(WEAPON_SPEAR)  : strcat(buf,"spear.");        break;
        case(WEAPON_MACE)   : strcat(buf,"mace/club.");    break;
        case(WEAPON_AXE)    : strcat(buf,"axe.");          break;
        case(WEAPON_FLAIL)  : strcat(buf,"flail.");        break;
        case(WEAPON_WHIP)   : strcat(buf,"whip.");         break;
        case(WEAPON_POLEARM): strcat(buf,"polearm.");      break;
        case(WEAPON_STAFF)  : strcat(buf,"staff.");        break;
        default             : strcat(buf,"unknown.");      break;
 	}
	do_say(ch,buf);
	if (obj->pIndexData->new_format)
	    sprintf(buf,"Damage is %ldd%ld (average %ld).", obj->value[1],obj->value[2], (1 + obj->value[2]) * obj->value[1] / 2);
	else
	    sprintf( buf, "Damage is %ld to %ld (average %ld).", obj->value[1], obj->value[2], ( obj->value[1] + obj->value[2] ) / 2 );
	do_say( ch,buf );
	break;
    case ITEM_ARMOR:
        sprintf( buf, "Armor class is %ld pierce, %ld bash, %ld slash, and %ld vs. magic.", 
          obj->value[0], obj->value[1], obj->value[2], obj->value[3] );
	do_say( ch,buf );
	break;
    }
    if (!obj->enchanted)
        for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
        {
	    if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	    {
                sprintf( buf, "Affects %s by %d.", affect_loc_name( paf->location ), paf->modifier );
	        do_say( ch,buf );
            }
            if (paf->bitvector)
            {
                switch(paf->where)
                {
                case TO_AFFECTS: sprintf(buf,"Adds %s affect.", affect_bit_name(paf->bitvector)); break;
                case TO_OBJECT:  sprintf(buf,"Adds %s object flag.", extra_bit_name(paf->bitvector)); break;
                case TO_IMMUNE:  sprintf(buf,"Adds immunity to %s.", imm_bit_name(paf->bitvector)); break;
                case TO_RESIST:  sprintf(buf,"Adds resistance to %s.", imm_bit_name(paf->bitvector)); break;
                case TO_VULN:    sprintf(buf,"Adds vulnerability to %s.", imm_bit_name(paf->bitvector)); break;
                default:         sprintf(buf,"Unknown bit %d: %d", paf->where,paf->bitvector); break;
                }
	        do_say( ch,buf );
            }
        }
    for ( paf = obj->affected; paf != NULL; paf = paf->next )
	if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
            sprintf( buf, "Affects %s by %d", affect_loc_name( paf->location ), paf->modifier );
            if ( paf->duration > -1)
                sprintf(buf2,", %d hours.",paf->duration);
            else
                sprintf(buf2,".");
	    strcat (buf, buf2);
	    do_say( ch,buf );
            if (paf->bitvector)
            {
                switch(paf->where)
                {
                case TO_AFFECTS: sprintf(buf,"Adds %s affect.", affect_bit_name(paf->bitvector)); break;
                case TO_OBJECT:  sprintf(buf,"Adds %s object flag.", extra_bit_name(paf->bitvector)); break;
                case TO_WEAPON:  sprintf(buf,"Adds %s weapon flags.", weapon_bit_name(paf->bitvector)); break;
                case TO_IMMUNE:  sprintf(buf,"Adds immunity to %s.", imm_bit_name(paf->bitvector)); break;
                case TO_RESIST:  sprintf(buf,"Adds resistance to %s.", imm_bit_name(paf->bitvector)); break;
                case TO_VULN:    sprintf(buf,"Adds vulnerability to %s.", imm_bit_name(paf->bitvector)); break;
                default:         sprintf(buf,"Unknown bit %d: %d", paf->where,paf->bitvector); break;
                }
	        do_say( ch,buf );
            }
	}
    
}

void do_pload( CHAR_DATA *ch, char *argument )
{
  DESCRIPTOR_DATA d;
  CHAR_DATA *charmie_count;
  bool isChar = FALSE;
  char name[MAX_INPUT_LENGTH];

  if (argument[0] == '\0')
  {
    send_to_char("Load who?\n\r", ch);
    return;
  }

  argument[0] = UPPER(argument[0]);
  argument = one_argument(argument, name);

  if ( get_char_world( ch, name ) != NULL )
  {
    send_to_char( "That person is already connected!\n\r", ch );
    return;
  }

  isChar = load_char_obj(&d, name);

  if (!isChar)
  {
    send_to_char("Load Who? Are you sure? I can't seem to find them.\n\r", ch);
    return;
  }

  d.character->desc     = NULL;
  d.character->next     = char_list;
  char_list             = d.character;
  d.connected           = CON_PLAYING;
  reset_char(d.character);

  if ( d.character->in_room != NULL )
  {
    char_to_room( d.character, ch->in_room);
  }

  act( "$n has pulled $N from the pattern!",
        ch, NULL, d.character, TO_ROOM );
  act( "You pull $N from the pattern!",
	ch, NULL, d.character, TO_CHAR );

  for (charmie_count = char_list; charmie_count != NULL; charmie_count=charmie_count->next)
  {
	if(charmie_count->leader == d.character)
	{
	    	char_to_room(charmie_count,d.character->in_room);
		act("$n has entered the game.",charmie_count,NULL,NULL,TO_ROOM);
	}
  }
}

void do_punload( CHAR_DATA *ch, char *argument )
{
  CHAR_DATA *victim;
  char who[MAX_INPUT_LENGTH];

  argument = one_argument(argument, who);

  if ( ( victim = get_char_world( ch, who ) ) == NULL )
  {
    send_to_char( "They aren't here.\n\r", ch );
    return;
  }

  if (victim->desc != NULL)
  {
    send_to_char("I dont think that would be a good idea...\n\r", ch);
    return;
  }

  if (victim->was_in_room != NULL)
  {
    char_to_room(victim, victim->was_in_room);
  }

  save_char_obj(victim);
  do_quit_new(victim,"",TRUE);

  act("$n has released $N back to the pattern.",
       ch, NULL, victim, TO_ROOM);
  act("You release $N back to the pattern.",
       ch, NULL, victim, TO_CHAR);
}

char *log_time(void)
{
        char result[200];
        strftime(result, 200, "%a %b %d, %l:%M%P", localtime(&current_time));
        return str_dup(result);
}

void do_prep( CHAR_DATA *ch )
{
    CHAR_DATA *victim=0;
    int chance;
    AFFECT_DATA af;

    if ( (chance = get_skill(ch,gsn_asscounter)) == 0
    ||   (!IS_NPC(ch)
    &&    ch->level < skill_table[gsn_asscounter].skill_level[ch->class]))
    {
        send_to_char("You don't know how to do that.\n\r",ch);
        return;
    }

    if (is_affected(ch,gsn_asscounter))
    {
        send_to_char("You cannot counter again so soon.\n\r",ch);
        return;
    }

    if (ch->mana < 50)
    {
        send_to_char("You can't find the energy to concentrate on countering.\n\r",ch);
        return;
    }

    if ((victim = ch->fighting) == NULL)
        {
            send_to_char("But you aren't fighting anyone!\n\r",ch);
            return;
        }

    /* modifiers */

    /* size */
    if (ch->size < victim->size)
        chance -= 10;

    /* dex */
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= get_curr_stat(victim,STAT_DEX) * 3 / 2;

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
        chance += 15;
    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
        chance -= 15;

    /* level */
    chance += (ch->drain_level + ch->level - victim->level - victim->drain_level) * 2;

    if (number_percent() < chance)
    {

        WAIT_STATE(ch,PULSE_VIOLENCE);
        ch->mana -= 50;

        send_to_char("Heightening your senses you concentrate on countering your foe!\n\r",ch);
        act("$n face grows grim as he focuses his concentration.",ch,NULL,NULL,TO_ROOM);
        check_improve(ch,gsn_asscounter,TRUE,1);

	init_affect(&af);
        af.where        = TO_AFFECTS;
	af.aftype	= AFT_INVIS;
        af.type         = gsn_asscounter;
        af.level        = ch->level;
        af.duration     = 1;
	af.location     = 0;
        af.modifier     = 0;
        af.bitvector    = 0;
  	af.name		= str_dup("Prep Skill Counter");
        affect_to_char(ch,&af);

        }
        else
    {
        WAIT_STATE(ch,PULSE_VIOLENCE);
        ch->mana -= 25;

        send_to_char("You attempt to focus your concentration, but fail.\n\r",ch);
        check_improve(ch,gsn_asscounter,FALSE,1);
    }
}

void do_flurry(CHAR_DATA *ch,char *argument)
{
      	CHAR_DATA *victim;
      	int chance, numhits, i, skill, lag;
	OBJ_DATA *weapon;
	OBJ_DATA *weapon2;

	skill = get_skill(ch,gsn_flurry);

      	if ( skill == 0 )
	{
		send_to_char("Huh?\n\r",ch);
		return;
      	}

	if ((victim = ch->fighting) == NULL)
	{
		send_to_char("You aren't fighting anyone.\n\r",ch);
		return;
	}

	/* CLS - Need to check to see if they are even wielding a primary or secondary weapon. */
	weapon = get_eq_char(ch,WEAR_WIELD);
        if(weapon == NULL)
	{
	 	send_to_char("You first need to find a weapon to flurry with.\n\r",ch);
	 	return;
 	}

	weapon2 = get_eq_char(ch,WEAR_DUAL_WIELD);
        if(weapon2 == NULL)
        {
         	send_to_char("You need to find another weapon to flurry with.\n\r",ch);
         	return;
        }

	if ((weapon->value[0] != WEAPON_SWORD) || (weapon2->value[0] != WEAPON_SWORD) )
	{
		send_to_char("You must be wielding two swords to flurry.\n\r",ch);
		return;
	}

	if (ch->move < 0) {
		send_to_char("You cannot gather the energy to flurry.\n\r",ch);
		return;
	}

	chance = number_percent();

	chance -= 20;

	if (chance > skill) {
		act("You attempt to start a flurry, but fail.",ch,0,victim,TO_CHAR);
		act("$n flails out wildly with $s swords but blunders.",ch,0,0,TO_ROOM);
		check_improve(ch,gsn_flurry,FALSE,2);
		WAIT_STATE(ch,2*PULSE_VIOLENCE);
		return;
	}

	if ((chance + skill) > 175) {
		numhits = 7;
	} else if ((chance + skill) > 160) {
		numhits = 6;
	} else if ((chance + skill) > 145) {
		numhits = 5;
	} else if ((chance + skill) > 130) {
		numhits = 4;
	} else if ((chance + skill) > 115) {
		numhits = 3;
	} else if ((chance + skill) > 100) {
		numhits = 2;
	} else if ((chance + skill) > 85) {
		numhits = 2;
	} else {
		numhits = 2;
	}

      act("You begin a wild flurry of attacks!",ch,0,victim,TO_CHAR);
      act("$n begins a wild flurry of attacks!",ch,0,0,TO_ROOM);

	for ( i=0; i< numhits; i++ ) {
		if (ch->in_room != victim->in_room) break;
		if (number_percent() > 95) {
			damage( ch, victim, 0, gsn_flurry,DAM_NONE,TRUE);
			continue;
		}
		if (number_percent() > (skill - 30)) { 
			act("$N partially blocks $n's attack, but $e still gets through.",ch,0,victim,TO_NOTVICT);
			act("You partially block $n's attack, but $e still gets through.",ch,0,victim,TO_VICT);
			act("$N partially blocks your attack, but you still get through.",ch,0,victim,TO_CHAR);
			one_hit_new(ch,victim,gsn_flurry,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);
			continue;
		}
		one_hit_new(ch,victim,gsn_flurry,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,NULL);

	}
	check_improve(ch,gsn_flurry,TRUE,1);
	ch->move -= 25 * numhits;
		lag = 5;
        if(numhits<6)
                lag = 4;
	if(numhits<3)
		lag = 3;
	WAIT_STATE(ch,PULSE_VIOLENCE*lag);
	return;
}

void do_drum(CHAR_DATA *ch,char *argument)
{
      	CHAR_DATA *victim;
      	int chance, numhits, i, skill;
	OBJ_DATA *weapon;
	OBJ_DATA *weapon2;

	skill = get_skill(ch,gsn_drum);

      	if ( skill == 0 )
	{
		send_to_char("Huh?\n\r",ch);
		return;
      	}

	if ((victim = ch->fighting) == NULL)
	{
		send_to_char("You aren't fighting anyone.\n\r",ch);
		return;
	}

        weapon = get_eq_char(ch,WEAR_WIELD);
        if(weapon == NULL)
        {
         	send_to_char("You first need to find a weapon to drum with.\n\r",ch);
         	return;
        }

        weapon2 = get_eq_char(ch,WEAR_DUAL_WIELD);
        if(weapon2 == NULL)
        {
         	send_to_char("You need to find another weapon to drum with.\n\r",ch);
         	return;
        }

	if ((weapon->value[0] != WEAPON_MACE) || (weapon2->value[0] != WEAPON_MACE) )
	{
		send_to_char("You must be wielding two maces to drum.\n\r",ch);
		return;
	}

	chance = number_percent();

	if (chance > skill) {
		act("You attempt to start drumming with your maces, but fail.",ch,0,victim,TO_CHAR);
		act("$n attempts to start drumming with two maces, but fails.",ch,0,0,TO_ROOM);
		check_improve(ch,gsn_drum,FALSE,1);
		WAIT_STATE(ch,2*PULSE_VIOLENCE);
		return;
	}

	if ((chance + skill) > 175) {
		numhits = 4;
	} else if ((chance + skill) > 160) {
		numhits = 3;
	} else if ((chance + skill) > 145) {
		numhits = 2;
	} else {
		numhits = 2;
	}

	act("$n drums at $N with $s maces.",ch,0,victim,TO_NOTVICT);
	act("$n drums at you with $s maces.",ch,0,victim,TO_VICT);
	act("You drum at $N with your maces.",ch,0,victim,TO_CHAR);

	for ( i=0; i < numhits; i++ ) {
		if (ch->in_room != victim->in_room) break;
		if (number_percent() > 85) {
			damage( ch, victim, 0, gsn_drum,DAM_NONE,TRUE);
			continue;
		}
		one_hit_new(ch,victim,gsn_drum,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,110,NULL);
	}
	one_hit_new(ch,victim,gsn_drum,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,150,NULL);

	check_improve(ch,gsn_drum,TRUE,1);
	
      	WAIT_STATE(ch,3*PULSE_VIOLENCE);
	return;
}


void do_siton(CHAR_DATA *ch,char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance=0, dam, sn_fart;
    char buf[MAX_STRING_LENGTH];

    one_argument(argument,arg);

     if ( (get_skill(ch,gsn_siton) == 0 )
        || ch->level < skill_table[gsn_siton].skill_level[ch->class])
    {
        if (!IS_NPC(ch))
        {
        send_to_char("You don't have the skill to sit on people.\n\r",ch);
        return;
        }
    }
	if(ch->position==POS_FIGHTING) {
		act("You can't sit on people while you're fighting!",ch,0,0,TO_CHAR);
		return;
	}

    if (arg[0] == '\0')
    {
		send_to_char("Sit on who?\n\r",ch);
		return;
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }

    if (victim == ch)
    {
        send_to_char("You can't sit on yourself.\n\r",ch);
        return;
    }

    if (is_safe(ch,victim))
        return;

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
        act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
        return;
    }

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE))
        chance += 5;
    if (IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE))
        chance -= 15;
    chance += get_curr_stat(ch,STAT_DEX)/2;
    chance -= get_curr_stat(victim,STAT_DEX)/2;
    if (IS_AFFECTED(victim,AFF_FLYING))
        chance -= dice(2,5);

    /* level */
    chance += (ch->level - victim->level)*3;
    chance -= 55;

        act("$n shrieks, waddles furiously, and leaps forward!",ch,0,victim,TO_NOTVICT);
        act("$n shrieks, waddles furiously, and leaps forward!",ch,0,victim,TO_VICT);
        act("You shriek, waddle furiously, and leap foward!",ch,0,victim,TO_CHAR);

        act("$N's eyes widen in pure terror as $n's shadow descends apon $M.",ch,0,victim,TO_NOTVICT);
        act("Your eyes widen in pure terror as $n's shadow descends apon you!",ch,0,victim,TO_VICT);
        act("$N's eyes widen in pure terror as your shadow descends apon $M.",ch,0,victim,TO_CHAR);

        if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (victim->fighting == NULL || ch->fighting == NULL))
	{
        sprintf(buf,"Help! %s is sitting on me!",PERS(ch,victim));
	do_myell(victim,buf);
	}

        dam = ch->level*2 + number_percent()*2 - 50;
        damage_old(ch,victim,dam,gsn_siton,DAM_BASH,TRUE);
        check_improve(ch,gsn_siton,TRUE,1);

        if (number_percent() > chance)
        {
	dam = dam+100;
        act("Seizing upon $N's weakness, $n sits on the head of $N and releases a spurt of methane!",ch,0,victim,TO_NOTVICT);
        act("Seizing upon your weakness, $n sits on your head and releases a spurt of methane!",ch,0,victim,TO_VICT);
        act("Seizing upon $N's weakness, you sit on $S head and release a spurt of methane!",ch,0,victim,TO_CHAR);
       if (!IS_NPC(ch) && !IS_NPC(victim))
	{
        sprintf(buf,"Help! %s is farting on me!",PERS(ch,victim));
	do_myell(victim,buf);
	}
	sn_fart = skill_lookup("gas breath");
        damage_old(ch,victim,dam,sn_fart,DAM_BASH,TRUE);
        }
        WAIT_STATE(ch,PULSE_VIOLENCE*3);
        WAIT_STATE(victim,PULSE_VIOLENCE*2);
        return;
}

void do_knife( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );


    if ( (get_skill(ch,gsn_knife) == 0 )
	|| ch->level < skill_table[gsn_knife].skill_level[ch->class])
    {
	if (!IS_NPC(ch))
	{
	send_to_char("You don't know how to knife.\n\r",ch);
	return;
	}
    }

    if (arg[0] == '\0')
    {
        send_to_char("Knife whom?\n\r",ch);
        return;
    }

    if (ch->fighting != NULL)
    {
        send_to_char("No way! You're still fighting!\n\r",ch);
        return;
    }
 
    if ((victim = get_char_room(ch,arg)) == NULL)
    {
        send_to_char("They aren't here.\n\r",ch);
        return;
    }

    if ( victim == ch )
    {
        send_to_char( "Bah, you can't knife yourself.\n\r", ch );
        return;
    }

    if (!is_wielded(ch,WEAPON_DAGGER,WIELD_ONE)) 
    {
	send_to_char("You need to be wielding a dagger to knife.\n\r",ch);
	return;
    }

    if ( is_safe( ch, victim ) )
	return;

    if (check_chargeset(ch,victim)) {return;}


    WAIT_STATE( ch, PULSE_VIOLENCE);

    if (!IS_NPC(ch) && !IS_NPC(victim))
    {
	switch(number_range(0,1))
	{
		case (0):
			sprintf(buf,"Die, %s you knifing fool!",PERS(ch,victim));
		break;
		case (1):
			sprintf(buf,"Help! %s knifed me!",PERS(ch,victim));
		break;
	}
	do_myell(victim,buf);
    }


    if ( number_percent( ) < get_skill(ch,gsn_knife))
    {
	one_hit_new(ch,victim,gsn_knife,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,125,NULL);
      	check_improve(ch,gsn_knife,TRUE,1);
    }
    else
    {
        damage( ch, victim, 0, gsn_knife,DAM_NONE,TRUE);
        check_improve(ch,gsn_knife,FALSE,2);
    }
    return;
}

void do_praclist( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int sn;
    CHAR_DATA *victim;
    int col;

    col = 0;
    argument = one_argument( argument, arg1 );

    if ( arg1[0] == '\0')
    {
	send_to_char( "See whose praclist?\n\r", ch );
	return;
    }
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
	return;
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name == NULL )
		break;

	    if (skill_table[sn].skill_level[victim->class] > 52)
		continue;
	    if ( victim->level < skill_table[sn].skill_level[victim->class]
	      || victim->pcdata->learned[sn] < 1 /* skill is not known */)
		continue;

	if (is_spec_skill(sn) != -1 && !is_specced(victim,is_spec_skill(sn))) {
	    	sprintf( buf, "%-24s {R%3d%%{x  ",
		skill_table[sn].name, victim->pcdata->learned[sn] );
	    	send_to_char( buf, ch );
	    	if ( ++col % 3 == 0 )
		send_to_char( "\n\r", ch );

			continue;
        }
	    sprintf( buf, "%-24s %3d%%  ",
		skill_table[sn].name, victim->pcdata->learned[sn] );
	    send_to_char( buf, ch );
	    if ( ++col % 3 == 0 )
		send_to_char( "\n\r", ch );
	}

	if ( col % 3 != 0 )
	    send_to_char( "\n\r", ch );

	sprintf( buf, "%d practice sessions left.\n\r",
	    victim->practice );
	send_to_char( buf, ch );
  return;
}

void do_decapitate(CHAR_DATA *ch,char *argument)
{
	char buf[MAX_STRING_LENGTH];
        CHAR_DATA *victim;
        int chance, skill;
	OBJ_DATA *disarmed;
	char arg[MAX_INPUT_LENGTH];
	AFFECT_DATA af;

	one_argument( argument, arg );
	
        if ( (chance = get_skill(ch,gsn_decapitate)) == 0 )
	{
      		send_to_char("Huh?\n\r",ch);
      		return;
	}

	if (arg[0] == '\0') 
	{
		victim = ch->fighting;
		if (victim == NULL) 
		{
			send_to_char("You aren't fighting anyone.\n\r",ch);
			return;
		}
	}
	else if ((victim = get_char_room(ch,arg)) == NULL)
	{
		send_to_char("They aren't here.\n\r",ch);
		return;
	}

	if (!is_wielded(ch,WEAPON_AXE,WIELD_TWO)) {
		send_to_char("You must be wielding two axes to attempt a decapitation.\n\r",ch);
		return;
	}

	if (victim == ch)
	{
		send_to_char("You cannot decapitate yourself.\n\r",ch);
		return;
	}
	if (victim->hit < victim->max_hit - 350)
	{
		send_to_char("They are too bloody, obscuring your judgment of a suitable decapitation spot.\n\r",ch);
		return;
	}
	disarmed=get_eq_char(ch,WEAR_WIELD);
	act("$n whirls $s axes impressively with $s hands, and brings their blades together at $N's neck!",ch,0,victim,TO_NOTVICT);
	act("$n whirls $s axes impressively with $s hands, and brings their blades together at your neck!",ch,0,victim,TO_VICT);
	act("You whirl your axes impressively with your hands, and bring their blades together at $N's neck!",ch,0,victim,TO_CHAR);

	chance = number_percent();
	skill = get_skill(ch,gsn_decapitate);

	if(IS_CABAL_OUTER_GUARD(victim) || IS_CABAL_GUARD(victim))
		return send_to_char("You can't decapitate cabal guardians.\n\r", ch);

	if ((chance > (skill/10)) && (chance - (skill/10)) < 50) 
	{
		sprintf(buf,"Help! %s is trying to slice my head off!",PERS(ch,victim));
		do_myell(victim,buf);
		one_hit(ch, victim, gsn_decapitate);
		check_improve(ch,gsn_decapitate,FALSE,1);
		WAIT_STATE(ch,3*PULSE_VIOLENCE);
		return;
	}

	if (chance > (skill/10)) {
		sprintf(buf,"Die %s, you decapitating fool!",PERS(ch,victim));
		do_myell(victim,buf);
		damage( ch, victim, 0, gsn_decapitate,DAM_NONE,TRUE);

		switch(number_range(0,1)) {
		case (0):
			disarmed = get_eq_char(ch,WEAR_WIELD);
		case (1):
			disarmed = get_eq_char(ch,WEAR_DUAL_WIELD);
		}
		if (IS_OBJ_STAT(disarmed,ITEM_NOREMOVE)) {
			act("Your weapon won't budge!",ch,disarmed,0,TO_CHAR);
			act("$n's weapon won't budge!",ch,disarmed,0,TO_ROOM);
		} else {
			act("Your grip on $p loosens and you send it flying!",ch,disarmed,0,TO_CHAR);
			act("$n's grip on $p loosens and $m sends it flying!",ch,disarmed,0,TO_ROOM);
			obj_from_char(disarmed);
			if (IS_OBJ_STAT(disarmed,ITEM_NODROP)) {
				obj_to_char(disarmed, ch);
			} else {
				obj_to_room(disarmed, ch->in_room);
			}
		}
		check_improve(ch,gsn_decapitate,FALSE,1);
		WAIT_STATE(ch,3*PULSE_VIOLENCE);
		return;
	}

	act("$n {R+++ DECAPITATES +++{x $N!",ch,0,victim,TO_NOTVICT);
	act("$n {R+++ DECAPITATES +++{x you!",ch,0,victim,TO_VICT);
	act("You {R+++ DECAPITATE +++{x $N!",ch,0,victim,TO_CHAR);
	init_affect(&af);
        af.where     = TO_AFFECTS;
        af.type      = gsn_decapitate;
        af.aftype    = AFT_SKILL;
        af.bitvector = 0;
	af.modifier  = 0;
	af.duration  = 1;
	af.location  = 0;
        af.level     = ch->level;
        affect_to_char(victim, &af);
	act("You see the world from a new perspective as your head hits the ground.",victim,0,0,TO_CHAR);
	raw_kill(ch,victim);
	check_improve(ch,gsn_decapitate,TRUE,3);
	WAIT_STATE(ch,PULSE_VIOLENCE);
	return;
}

void do_lightwalk(CHAR_DATA *ch,char *argument)
{
	CHAR_DATA *victim;
	char arg1[MAX_INPUT_LENGTH];
	int level;

	one_argument(argument,arg1);

      	if (get_skill(ch,gsn_lightwalk) == 0)
      	{
      		send_to_char("Huh?\n\r",ch);
	      	return;
      	}

	if (cabal_down(ch,CABAL_KNIGHT))
                return;

    if (affect_find(ch->affected,gsn_insect_swarm) != NULL)
    {
        send_to_char( "You attempt to transport yourself to safety but loose your concentration as you are bitten by yet another insect.\n\r", ch);
        return;
    }



	if (ch->mana < 70)
	{
		send_to_char("You do not have the mana.\n\r",ch);
		return;
	}

	if (number_percent() > ch->pcdata->learned[gsn_lightwalk])
	{
		send_to_char("You fail to be transported.\n\r",ch);
		check_improve(ch,gsn_lightwalk,FALSE,1);
		ch->mana -= 35;
		return;
	}

	level=ch->level;
	victim = get_char_world(ch,arg1);

    	if ( victim == NULL
    	||   victim == ch
    	||   victim->in_room == NULL
    	||   !can_see_room(ch,victim->in_room)
    	||   IS_SET(victim->in_room->room_flags, ROOM_NO_GATE)
    	||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    	||   (victim->in_room->cabal != 0 && victim->in_room->cabal != ch->cabal)
    	||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    	||   IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    	||   victim->level >= level + 4
    	||   (!IS_NPC(victim) && !can_pk(ch,victim) && IS_SET(victim->act,PLR_NOSUMMON))
    	||   (!IS_NPC(victim) && saves_spell(level,victim,DAM_OTHER))
    	||   (!IS_NPC(victim) && victim->level > LEVEL_HERO)
    	||   (IS_NPC(victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
    	||   (IS_NPC(victim) && saves_spell( level, victim,DAM_OTHER) ) )
    	{
		ch->mana -= 35;
		send_to_char("You failed.\n\r",ch);
		WAIT_STATE(ch,12);
		return;
	}

    	un_camouflage( ch, NULL);
    	un_earthfade(ch, NULL);
    	un_hide(ch, NULL);
    	un_sneak(ch, NULL);

	ch->mana -= 70;
	act("$n becomes enshrouded in a bright white light and vanishes!",ch,0,0,TO_ROOM);
	char_from_room(ch);
	char_to_room(ch,victim->in_room);
	send_to_char("You become enshrouded in a bright white light and appear at your target.\n\r",ch);
	act("$n appears in a bright flash of light.",ch,0,0,TO_ROOM);
	do_look(ch,"auto");
	WAIT_STATE(ch,12);
	return;
}

void do_bobbit(CHAR_DATA *ch,char *argument)
{
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
	OBJ_DATA *weapon;
	OBJ_DATA *weapon2;
	char arg1[MAX_INPUT_LENGTH];
	AFFECT_DATA af;

	argument = one_argument( argument, arg1 );

      if (get_skill(ch,gsn_bobbit) == 0)
      {
      	send_to_char("Huh?\n\r",ch);
	      return;
      }

	if ( arg1[0] == '\0')
	{
		send_to_char("Perform the Bobbit on who?\n\r", ch );
		return;
	}

	if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}

	if ( IS_NPC(victim) )
	{
		send_to_char( "They aren't here.\n\r", ch );
		return;
	}
	if ( victim == ch)
	{
		send_to_char( "You can't cut off your own wanker.\n\r", ch);
		return;
	}

	/* CHECK IF WIELDING TWO DAGGERS */
	weapon = get_eq_char(ch,WEAR_WIELD);
	weapon2 = get_eq_char(ch,WEAR_DUAL_WIELD);

        if(weapon == NULL)
        {
         send_to_char("You first need to find a weapon to Bobbit with.\n\r",ch);
         return;
        }

        if(weapon2 == NULL)
        {
         send_to_char("You need to find another weapon to Bobbit with.\n\r",ch);
         return;
        }

	if (weapon == NULL || weapon2 == NULL || (weapon->value[0] != WEAPON_DAGGER && weapon2->value[0] != WEAPON_DAGGER) )
	{
		send_to_char("You must be wielding two daggers to attempt to Bobbit.\n\r",ch);
		return;
	}

	if (victim->position != POS_SLEEPING && !(ch->level > LEVEL_HERO))
	{
		send_to_char("You cannot do this when that person is awake!\n\r",ch);
		return;
	}

	if (victim->sex != SEX_MALE) {
		send_to_char("But that isn't a guy!\n\r",ch);
		return;
	}

	act("$n slowly lowers $N's pants, whips out two glinting daggers, and slashes madly!",ch,0,victim,TO_NOTVICT);
	act("You feel your pants being lowered. Curious.",ch,0,victim,TO_VICT);
	act("You slower lower $N's pants, brandish two glinting daggers, and slash madly!",ch,0,victim,TO_CHAR);

	switch(number_range(0,3)) {
	case (0):
		sprintf(buf,"Help! %s is severing my private parts!",PERS(ch,victim));
		break;
	case (1):
		sprintf(buf,"Help! %s is slicing off my genitalia!",PERS(ch,victim));
		break;
	case (2):
		sprintf(buf,"Help! %s is chopping off my organs of generation!",PERS(ch,victim));
		break;
	case (3):
		sprintf(buf,"Help! %s is chopping off my reproductive organs!",PERS(ch,victim));
		break;
	}
	act("You shriek in pure horror!",victim,0,0,TO_CHAR);
	act("$n shrieks in pure horror!",victim,0,0,TO_ROOM);


	do_myell(victim,buf);

	one_hit(ch, victim, gsn_bobbit);
	check_improve(ch,gsn_bobbit,TRUE,1);
	WAIT_STATE(ch,2*PULSE_VIOLENCE);

	init_affect(&af);
	af.where     = TO_AFFECTS;
	af.type      = gsn_bobbit;
	af.aftype    = AFT_SKILL;
	af.level     = ch->level;
	af.duration  = ch->level/5;
	af.bitvector = 0;
	af.location  = APPLY_DEX;
	af.modifier  = -15;
	af.owner_name	 = str_dup(ch->original_name);
	affect_to_char(victim,&af);
	af.location  = APPLY_SEX;
	af.modifier  = 2;
	af.bitvector = 0;
	affect_to_char(victim,&af);
	return;
}

void do_affrem(CHAR_DATA *ch, char *argument) {
	char arg1[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
	int sn;
	AFFECT_DATA *af;

	argument = one_argument(argument,arg1);
	argument = one_argument(argument,arg2);

	if (arg1[0] == '\0')
	{
		send_to_char("Syntax: affrem <char> <affect>\n\r",ch);
		return;
	}

	if ((victim = get_char_world(ch,arg1)) == NULL)
	{
		send_to_char( "They aren't playing.\n\r",ch);
		return;
	}

	if (arg2[0] == '\0')
	{
		send_to_char("Syntax: affrem <char> <affect>\n\r",ch);
		return;
	}

	sn = 0;
	if ((sn = skill_lookup(arg2)) < 0)
	{
		send_to_char("No such skill or spell.\n\r", ch);
		return;
	}

	if (!(is_affected(victim,sn)))
	{
		send_to_char("Target is not affected by that.\n\r", ch);
		return;
	}
	
	af = affect_find(victim->affected,sn);
	affect_strip(victim, sn);
	if (skill_table[sn].msg_off )
	{
		send_to_char(skill_table[sn].msg_off, victim );
		send_to_char("\n\r", victim);
	}

	if (af->strippable)
		sprintf(buf,"The %s affect is removed from %s.\n\r",skill_table[sn].name,victim->name);
	else
		sprintf(buf,"The %s affect on %s is not removable.\n\r",skill_table[sn].name,victim->name);
	send_to_char(buf, ch);
	return;
}

void do_divine_intervention(CHAR_DATA *ch, char *argument) {
	char arg1[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
	char arg3[MAX_STRING_LENGTH];
	char arg4[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *victim;
	int enchant_type, value, duration;
	bool isgoodaffect;
	AFFECT_DATA af;

	argument = one_argument(argument,arg1);
	argument = one_argument(argument,arg2);
	argument = one_argument(argument,arg3);
	argument = one_argument(argument,arg4);

	if ((arg1[0] == '\0') || (arg2[0] == '\0') || (arg3[0] == '\0') || arg4[0] == '\0')
	{
		send_to_char("Syntax:  intervention <char> <affect> <value> <duration>\n\r",ch);
		send_to_char("Affects: hp mana move ac hitroll damroll saves\n\r", ch);
		return;
	}

	if ((victim = get_char_world(ch,arg1)) == NULL)
	{
		send_to_char("They aren't playing.\n\r",ch);
		return;
	}

	if (!(is_number(arg3))) {
		send_to_char("Value must be numeric.\n\r",ch);
		return;
	}

	if (!(is_number(arg4))) {
		send_to_char("Duration must be numeric.\n\r",ch);
		return;
	}

	isgoodaffect = FALSE;

	value = atoi(arg3);
	duration = atoi(arg4);

	if (!str_prefix(arg2,"hp")) {
		if (value > 0) {isgoodaffect = TRUE;}
		enchant_type=APPLY_HIT;
	} else if (!str_prefix(arg2,"mana")) {
		if (value > 0) {isgoodaffect = TRUE;}
		enchant_type=APPLY_MANA;
	} else if (!str_prefix(arg2,"move")) {
		if (value > 0) {isgoodaffect = TRUE;}
		enchant_type=APPLY_MOVE;
	} else if (!str_prefix(arg2,"ac")) {
		if (value < 0) {isgoodaffect = TRUE;}
		enchant_type=APPLY_AC;
	} else if (!str_prefix(arg2,"hitroll")) {
		if (value > 0) {isgoodaffect = TRUE;}
		enchant_type=APPLY_HITROLL;
	} else if (!str_prefix(arg2,"damroll")) {
		if (value > 0) {isgoodaffect = TRUE;}
		enchant_type=APPLY_DAMROLL;
	} else if (!str_prefix(arg2,"saves")) {
		if (value < 0) {isgoodaffect = TRUE;}
		enchant_type=APPLY_SAVING_SPELL;
	} else {
		send_to_char("No affect of that name.\n\r",ch);
		return;
	}

        init_affect(&af);
	af.where        = TO_AFFECTS;
	af.type         = gsn_divine_intervention;
	af.aftype	= AFT_SKILL;
	af.level        = ch->level;
	af.duration     = duration;
	af.modifier     = value;
	af.bitvector    = 0;
	af.location     = enchant_type;
	affect_to_char(victim,&af);

	if (isgoodaffect == TRUE)
	{
		sprintf(buf,"%s is blessed with %s.\n\r",victim->name,arg2);
		send_to_char(buf,ch);
		send_to_char("You have been blessed by the Immortals.\n\r", victim);
	} else {
		sprintf(buf,"%s is cursed with %s.\n\r",victim->name,arg2);
		send_to_char(buf,ch);
		send_to_char("You have been cursed by the Immortals.\n\r", victim);
	}

	return;
}


void do_ccb( CHAR_DATA *ch, char *argument )
{
	char arg1[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];
	int cabal;
	DESCRIPTOR_DATA *d;
	argument = one_argument(argument,arg1);
	strcpy(arg2, argument);

	if (arg1[0] == '\0' )
	{
		send_to_char("Syntax: ccb <cabal> <message>\n\r",ch);
		return;
	}

	if (arg2[0] == '\0' )
	{
		send_to_char("Syntax: ccb <cabal> <message>\n\r",ch);
		return;
	}

	cabal = cabal_lookup(arg1);
	if (!(cabal)) {
		send_to_char("That's not a valid cabal.\n\r",ch);
		return;
	}

	if (!IS_IMMORTAL(ch))
      		sprintf(buf, "%s%s: {c%s{x\n\r",
			cabal_table[cabal].who_name,
			IS_NPC(ch) ? ch->short_descr : ch->name,
			arg2);
	else
		sprintf(buf, "%s%s (%s): {c%s{x\n\r",
			cabal_table[cabal].who_name,
			IS_NPC(ch) ? ch->short_descr : ch->name, timestamp(),
			arg2);

	send_to_char(buf, ch);

	for (d = descriptor_list; d != NULL; d = d->next)
	{
		if (d->connected == CON_PLAYING)
		{
			if (((d->character != ch) && (!IS_SET(d->character->comm,COMM_NOCABAL)) && (d->character->cabal) && (d->character->cabal == cabal))
			|| ((IS_SET(d->character->comm,COMM_ALL_CABALS)) && (d->character != ch)))
			{
				if(!IS_IMMORTAL(d->character))
					sprintf(buf, "%s%s: {c%s{x\n\r",
					cabal_table[cabal].who_name,
					PERS(ch,d->character),
					arg2);
				else
					sprintf(buf, "%s%s (%s): {c%s{x\n\r",
					cabal_table[cabal].who_name,
					PERS(ch,d->character), timestamp(),
					arg2);
				send_to_char(buf, d->character);
			}
		}
	}
	return;
}

void do_stun( CHAR_DATA *ch,char *argument)
{
	AFFECT_DATA af;
	CHAR_DATA *victim;
	int chance;
	OBJ_DATA *weapon;

	if ((chance = get_skill(ch,gsn_stun)) == 0)
	{
		send_to_char("You don't know how to stun people.\n\r",ch);
		return;
	}

	if ((victim = ch->fighting))
	{
		if( IS_CABAL_OUTER_GUARD(victim) || IS_CABAL_GUARD(victim))
		{
			act("Try as you might, you can't seem to stun $N.", ch, 0, victim, TO_CHAR);
			WAIT_STATE(ch, PULSE_VIOLENCE);
			return;
		}
	}

	if ((victim = ch->fighting) == NULL)
	{
		send_to_char("You can't stun anyone if you're not fighting.\n\r",ch);
		return;
	}

	if (victim == ch)
	{
		send_to_char("You stun yourself repeatedly to no avail.\n\r",ch);
		return;
	}

	weapon = get_eq_char(ch,WEAR_WIELD);

	if (weapon != NULL) {
		send_to_char("You cannot stun while wielding a weapon.\n\r",ch);
		return;
	}

	if (get_eq_char(ch, WEAR_SHIELD) != NULL)
	{
		send_to_char("You cannot stun while wielding a shield.\n\r",ch);
		return;
	}

	if (get_eq_char(ch, WEAR_DUAL_WIELD) != NULL)
	{
		send_to_char("You cannot stun while dual-wielding a weapon.\n\r",ch);
		return;
	}

	if (is_safe(ch,victim))
		return;

	if (victim->position == POS_SLEEPING)
	{
		send_to_char("But that person is already asleep!\n\r",ch);
		return;
	}

	if (is_affected(victim,gsn_stun))
	{
		send_to_char("They are already quite stunned.\n\r",ch);
		return;
	}

	if (victim->level > ch->level)
		chance -= (victim->level-ch->level)*10;
	if (IS_AFFECTED(victim,AFF_HASTE))
		chance -= 20;
	if (IS_AFFECTED(ch,AFF_HASTE))
		chance += 10;

	chance -= 25;
	chance += (ch->size-victim->size)*5;
	chance += (get_curr_stat(ch,STAT_STR) - get_curr_stat(victim,STAT_STR));
	chance += (get_curr_stat(ch,STAT_DEX) - get_curr_stat(victim,STAT_DEX));
	
	act("You pummel $N with your fists, stunning $m.",ch,0,victim,TO_CHAR);
	act("$n pummels $N with $s fists, stunning $M.",ch,0,victim,TO_NOTVICT);
	act("$n pummels at you with $s fists, stunning you.",ch,0,victim,TO_VICT);

	if (number_percent() < chance)
	{
		init_affect(&af);
		af.where	= TO_AFFECTS;
		af.level 	= ch->level;
		af.location 	= 0;
		af.type 	= gsn_stun;
		af.aftype 	= AFT_SKILL;
		af.duration 	= 1;
		af.bitvector 	= AFF_SLEEP;
		af.modifier 	= 0;
		char msg_buf[MSL];
		sprintf(msg_buf,"knocked unconscious");
		af.affect_list_msg = str_dup(msg_buf);
		affect_to_char(victim,&af);
		stop_fighting(victim, TRUE);
		victim->position = POS_SLEEPING;
		check_improve(ch,gsn_stun,TRUE,2);
		WAIT_STATE(ch,2*PULSE_VIOLENCE);
		return;
	} else {
		one_hit(ch, victim, gsn_stun);
		check_improve(ch,gsn_stun,FALSE,3);
		WAIT_STATE(ch, 3*PULSE_VIOLENCE);
		WAIT_STATE(victim, PULSE_VIOLENCE);
	}
	return;
}

void spell_divine_aid(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	int dam, eng=0, gsn, add;

	dam = level * 3;

	if (IS_GOOD(ch)) {
		eng = 1;
	} else if (IS_EVIL(ch)) {
		eng = -1;
	} else {
		switch(number_range(0,1)) {
		case (0):
			eng = 1;
			break;
		case (1):
			eng = -1;
			break;
		}
		dam -= level + 25;
	}

	if (eng == 1) {
		act("$n summons forth an angel to aid $m!",ch,NULL,victim,TO_NOTVICT);
		act("$n summons forth an angel to aid $m!",ch,NULL,victim,TO_VICT);
		act("You summon forth an angel to aid you!",ch,NULL,victim,TO_CHAR);
		gsn = skill_lookup("wrath");
		dam += add = number_percent();
		damage_old(ch,victim,dam,gsn,DAM_HOLY,TRUE);
		dam -= add;
		dam += add = number_percent();
		damage_old(ch,victim,dam,gsn,DAM_HOLY,TRUE);
		dam -= add;
		dam += add = number_percent();
		damage_old(ch,victim,dam,gsn,DAM_HOLY,TRUE);
	} else {
		act("$n summons forth a demon to aid $m!",ch,NULL,victim,TO_NOTVICT);
		act("$n summons forth a demon to aid $m!",ch,NULL,victim,TO_VICT);
		act("You summon forth a demon to aid you!",ch,NULL,victim,TO_CHAR);
		gsn = skill_lookup("dark wrath");
		dam += add = number_percent();
		damage_old(ch,victim,dam,gsn,DAM_NEGATIVE,TRUE);
		dam -= add;
		dam += add = number_percent();
		damage_old(ch,victim,dam,gsn,DAM_NEGATIVE,TRUE);
		dam -= add;
		dam += add = number_percent();
		damage_old(ch,victim,dam,gsn,DAM_NEGATIVE,TRUE);
	}
	return;

}

void do_shieldbash_templar( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance, wait;
    char buf[MAX_STRING_LENGTH];
    bool FightingCheck;
    OBJ_DATA *obj;
    OBJ_DATA *obj_templar;
    int check;

    if (ch->fighting != NULL)
	FightingCheck = TRUE;
    else
	FightingCheck = FALSE;

    one_argument(argument,arg);
    obj_templar = get_eq_char(ch, WEAR_SHIELD);
    check = skill_lookup("shieldbash");

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("But you aren't fighting anyone!\n\r",ch);
	    return;
	}
    }

    if ((victim = get_char_room(ch,arg)) == NULL)
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
	send_to_char("You try to bash your head with your shield, but fail.\n\r",ch);
	return;
    }

    if (is_safe(ch,victim))
	return;

    if ( ( obj = get_eq_char( ch, WEAR_SHIELD ) ) == NULL )
	{
	send_to_char("You need a shield to bash with it!\n\r",ch);
	return;
	}

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
	act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

    if (is_affected(victim, gsn_protective_shield))
     {
	act("Your shield bash seems to slide around $N.", ch, NULL, victim, TO_CHAR);
	act("$n's bash slides off your protective shield.", ch, NULL, victim,
	    TO_VICT);
	act("$n's bash seems to slide around $N.",ch,NULL,victim,TO_NOTVICT);
	return;
     }

    /* modifiers */

    chance = skill_lookup("shieldbash");

    if (ch->size < victim->size)
	chance += (ch->size - victim->size) * 25;
    else
	chance += (ch->size - victim->size) * 10;


    /* stats */
    chance += get_curr_stat(ch,STAT_STR);
    chance -= get_curr_stat(victim,STAT_DEX) * 4/3;

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST))
	chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST))
	chance -= 20;

    /* level */
    chance += (ch->level - victim->level) * 2;

    /* now the attack */
    if (number_percent() < chance)
    {

	act("$n sends you sprawling with a powerful shield bash!",
		ch,NULL,victim,TO_VICT);
	act("You slam into $N with your shield, and send $M flying!",ch,NULL,victim,TO_CHAR);
	act("$n sends $N sprawling with a powerful bash with his shield.",
		ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_shieldbash,TRUE,1);

	wait = 3;

	switch(number_bits(2)) {
	case 0: wait = 1; break;
	case 1: wait = 2; break;
	case 2: wait = 4; break;
	case 3: wait = 3; break;
	}

	WAIT_STATE(victim, wait * PULSE_VIOLENCE);
	WAIT_STATE(ch,skill_table[gsn_shieldbash].beats+PULSE_VIOLENCE);
	victim->position = POS_RESTING;
	damage(ch,victim,number_range(4,4 + 4 * ch->size + chance/3),gsn_shieldbash,
	    DAM_BASH, TRUE);

    }
    else
    {
	damage(ch,victim,0,gsn_shieldbash,DAM_BASH, TRUE);
	act("You fall flat on your face!",
	    ch,NULL,victim,TO_CHAR);
	act("$n falls flat on $s face.",
	    ch,NULL,victim,TO_NOTVICT);
	act("You evade $n's shield bash, causing $m to fall flat on $s face.",
	    ch,NULL,victim,TO_VICT);
	check_improve(ch,gsn_shieldbash,FALSE,1);
	ch->position = POS_RESTING;
	WAIT_STATE(ch,skill_table[gsn_shieldbash].beats * 3/2);
    }
    if (!(IS_NPC(victim)) && !(IS_NPC(ch)) && victim->position > POS_STUNNED
		&& !FightingCheck)
      {
	if (!can_see(victim, ch))
	  do_myell(victim, "Help! Someone is bashing me with a shield!");
	else
	  {
	    sprintf(buf, "Help! %s is bashing me with a shield!",
		PERS(ch,victim));
	    do_myell(victim, buf);
	  }
      }
}

void do_shieldbash( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance, wait;
    char buf[MAX_STRING_LENGTH];
    bool FightingCheck;
    OBJ_DATA *obj;
    OBJ_DATA *obj_templar;

    if (ch->fighting != NULL)
	FightingCheck = TRUE;
    else
	FightingCheck = FALSE;

    one_argument(argument,arg);
    obj_templar = get_eq_char(ch, WEAR_SHIELD);

    if ((chance = get_skill(ch,gsn_shieldbash)) == 0) 
    {
	send_to_char("Shield bashing? What's that?\n\r",ch);
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

    if (victim->position < POS_FIGHTING)
    {
	act("You'll have to let $M get back up first.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (victim == ch)
    {
	send_to_char("You try to bash your head with your shield, but fail.\n\r",ch);
	return;
    }

    if (is_safe(ch,victim))
	return;

    if ( ( obj = get_eq_char( ch, WEAR_SHIELD ) ) == NULL )
	{
	send_to_char("You need a shield to bash it with!\n\r",ch);
	return;
	}

    if (IS_AFFECTED(ch,AFF_CHARM) && ch->master == victim)
    {
	act("But $N is your friend!",ch,NULL,victim,TO_CHAR);
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

    if (is_affected(victim, gsn_armor_of_thorns))
    {
        damage(victim,ch,number_range(victim->level,(victim->level *2)), gsn_armor_of_thorns,DAM_PIERCE, TRUE);
        WAIT_STATE(ch,PULSE_VIOLENCE*1);
        return;
    }

    if (is_affected(victim, gsn_protective_shield))
     {
	act("Your shield bash seems to slide around $N.", ch, NULL, victim, TO_CHAR);
	act("$n's bash slides off your protective shield.", ch, NULL, victim,
	    TO_VICT);
	act("$n's bash seems to slide around $N.",ch,NULL,victim,TO_NOTVICT);
	return;
     }

    /* modifiers */

    /* size  and weight */
    chance += ch->carry_weight / 25;
    chance -= victim->carry_weight / 20;

    if (ch->size < victim->size)
	chance += (ch->size - victim->size) * 25;
    else
	chance += (ch->size - victim->size) * 10;


    /* stats */
    chance += get_curr_stat(ch,STAT_STR);
    chance -= get_curr_stat(victim,STAT_DEX) * 4/3;

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST))
	chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST))
	chance -= 20;

    /* level */
    chance += (ch->level - victim->level) * 2;

    /* now the attack */
    if (number_percent() < chance)
    {

	act("$n sends you sprawling with a powerful shield bash!",
		ch,NULL,victim,TO_VICT);
	act("You slam into $N with your shield, and send $M flying!",ch,NULL,victim,TO_CHAR);
	act("$n sends $N sprawling with a powerful bash with his shield.",
		ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_shieldbash,TRUE,1);

	wait = 2;

	switch(number_bits(2)) {
	case 0: wait = 0; break;
	case 1: wait = 1; break;
	case 2: wait = 2; break;
	case 3: wait = 3; break;
	}

	WAIT_STATE(victim, wait * PULSE_VIOLENCE);
	WAIT_STATE(ch,skill_table[gsn_shieldbash].beats+PULSE_VIOLENCE);
	victim->position = POS_RESTING;

	if (is_affected(ch,gsn_shield_of_purity))
	{
		int purityMultiplier = number_percent();
		act("Your divine shield strikes $N with unnatural force!",ch,0,victim,TO_CHAR);
		act("$n's divine shield strikes you with unnatural force!",ch,0,victim,TO_VICT);
		act("$n's divine shield strikes $N with unnatural force!",ch,0,victim,TO_NOTVICT);
		
		if (purityMultiplier < 40)
			damage(ch,victim,number_range(4,4 + 4 * ch->size + chance/3)*3,gsn_shieldbash,
	    DAM_BASH, TRUE);
		else if (purityMultiplier >= 40 && purityMultiplier < 60)
			damage(ch,victim,number_range(4,4 + 4 * ch->size + chance/3)*4,gsn_shieldbash,
	    DAM_BASH, TRUE);
		else if (purityMultiplier >= 60 && purityMultiplier < 75)
			damage(ch,victim,number_range(4,4 + 4 * ch->size + chance/3)*6,gsn_shieldbash,
	    DAM_BASH, TRUE);
		else if (purityMultiplier >= 75 && purityMultiplier < 85)
			damage(ch,victim,number_range(4,4 + 4 * ch->size + chance/3)*8,gsn_shieldbash,
	    DAM_BASH, TRUE);
		else if (purityMultiplier >= 85 && purityMultiplier < 94)
			damage(ch,victim,number_range(4,4 + 4 * ch->size + chance/3)*10,gsn_shieldbash,
	    DAM_BASH, TRUE);
		else if (purityMultiplier >= 94)
			damage(ch,victim,number_range(4,4 + 4 * ch->size + chance/3)*12,gsn_shieldbash,
	    DAM_BASH, TRUE);
	}

	else if (is_affected(ch,gsn_shield_of_redemption))
	{
		act("The surface of $n's divine shield jolts you as he slams into your body!",ch,0,victim,TO_VICT);
		act("The surface of your divine shield jolts $N as you slam into him!",ch,0,victim,TO_CHAR);
		damage_new(ch,victim,GET_DAMROLL(ch),TYPE_UNDEFINED,DAM_HOLY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"strike of redemption"); 
	}

	else if (is_affected(ch,gsn_shield_of_justice))
	{
		OBJ_DATA *objWieldedJustice, *secondaryWieldedJustice;
		damage(ch,victim,number_range(4,4 + 4 * ch->size + chance/3)*4,gsn_shieldbash,
	    DAM_BASH, TRUE);
		if (number_percent() < 20 && get_eq_char(victim,WEAR_WIELD) != NULL)
		{
			objWieldedJustice = get_eq_char(victim,WEAR_WIELD);
			if ( IS_OBJ_STAT(objWieldedJustice,ITEM_NOREMOVE) || IS_OBJ_STAT(objWieldedJustice,ITEM_NODISARM) || is_affected(victim,gsn_spiderhands))
			{
				act("The surface of your shield catches $N's weapon, but you can't pry it away!",ch,0,victim,TO_CHAR);
				act("The surface of $n's shield catches your weapon, but he can't pry it away!",ch,0,victim,TO_VICT);
				act("The surface of $n's shield catches $N's weapon, but he can't pry it away!",ch,0,victim,TO_NOTVICT);
			}

			else
			{
				act("The surface of your shield catches $N's weapon and you pry it away from him, hurling it to the ground!",ch,0,victim,TO_CHAR);
				act("The surface of $n's shield catches your weapon and he rips it from your grip, throwing it to the ground!",ch,0,victim,TO_VICT);
				act("The surface of $n's shield catches $N's weapon and he rips it from $N's grip, throwing it to the ground!",ch,0,victim,TO_NOTVICT);
				obj_from_char( objWieldedJustice );
        			obj_to_room( objWieldedJustice, victim->in_room );
				if ((secondaryWieldedJustice = get_eq_char(victim,WEAR_DUAL_WIELD)) != NULL)
    				{
       					unequip_char(victim,secondaryWieldedJustice);
        				equip_char(victim,secondaryWieldedJustice,WEAR_WIELD);
    				}
			}
		}
	}
	
	else if (is_affected(ch,gsn_shield_of_truth))
	{
		int sapHealth = GET_DAMROLL(ch) * .75;
		act("Your god senses the proximity of $N through your shield!",ch,0,victim,TO_CHAR);
		act("The shield's energy reaches out and saps $N's health, using it to fuel your own!",ch,0,victim,TO_CHAR);
		act("The energy surrounding $n's divine shield reaches out and saps your health!",ch,0,victim,TO_VICT);
		act("The energy surrounding $n's divine shield reaches out and saps $N's health!",ch,0,victim,TO_NOTVICT);
damage_new(ch,victim,sapHealth,TYPE_UNDEFINED,DAM_HOLY,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"strike of truth"); 
		act("$n absorbs your lost health!",ch,0,victim,TO_VICT);
		ch->hit += sapHealth;
		if (ch->hit > ch->max_hit)
			ch->hit = ch->max_hit;
	}

	else
		damage(ch,victim,number_range(4,4 + 4 * ch->size + chance/3),gsn_shieldbash,
	    DAM_BASH, TRUE);
    }
    else
    {
	damage(ch,victim,0,gsn_shieldbash,DAM_BASH, TRUE);
	act("You fall flat on your face!",
	    ch,NULL,victim,TO_CHAR);
	act("$n falls flat on $s face.",
	    ch,NULL,victim,TO_NOTVICT);
	act("You evade $n's shield bash, causing $m to fall flat on $s face.",
	    ch,NULL,victim,TO_VICT);
	check_improve(ch,gsn_shieldbash,FALSE,1);
	ch->position = POS_RESTING;
	WAIT_STATE(ch,skill_table[gsn_shieldbash].beats * 3/2);
    }
    if (!(IS_NPC(victim)) && !(IS_NPC(ch)) && victim->position > POS_STUNNED
		&& !FightingCheck)
      {
	if (!can_see(victim, ch))
	  do_myell(victim, "Help! Someone is bashing me with a shield!");
	else
	  {
	    sprintf(buf, "Help! %s is bashing me with a shield!",
		PERS(ch,victim));
	    do_myell(victim, buf);
	  }
      }
}

void do_boneshatter( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	OBJ_DATA *weapon;
	int skill;
	AFFECT_DATA af;

	if ((skill = get_skill(ch,gsn_boneshatter)) == 0) {
		send_to_char("You don't know how to boneshatter properly.\n\r",ch);
		return;
	}

	if ((victim = ch->fighting) == NULL)
	{
		send_to_char("You can't shatter bones when you're not fighting.\n\r",ch);
		return;
	}

	weapon = get_eq_char(ch,WEAR_WIELD);

	if (weapon == NULL)
	{
		send_to_char("You must be wielding a weapon for that.\n\r",ch);
		return;
	}

	if (weapon->value[0] != WEAPON_MACE)
	{
		weapon = get_eq_char(ch,WEAR_DUAL_WIELD);
		if (weapon == NULL)
		{
			send_to_char("You must be wielding a weapon to for that.\n\r",ch);
			return;
		}
	}

	if (weapon->value[0] != WEAPON_MACE)
	{
		send_to_char("You need to be wielding a mace to shatter bones.\n\r",ch);
		return;
	}

	if (is_affected(victim,gsn_boneshatter)) {
		send_to_char("Those bones are already shattered.\n\r",ch);
		return;
	}

	act("You bring $p around with bone-shattering force!",ch,weapon,victim,TO_CHAR);
	act("$n brings $p around with bone-shattering force!",ch,weapon,0,TO_ROOM);

	if (number_percent( ) < (get_skill(ch,gsn_boneshatter) - 15))
	{
		init_affect(&af);
		af.where     = TO_AFFECTS;
		af.type      = gsn_boneshatter;
		af.aftype    = AFT_SKILL;
		af.level     = ch->level;
		af.duration  = ch->level/5;
		af.bitvector = 0;
		af.location  = APPLY_DEX;
		af.modifier  = 1-(ch->level/5);
		affect_to_char(victim,&af);
		af.location  = APPLY_STR;
		af.modifier  = 1-(ch->level/5);
		af.bitvector = 0;
		affect_to_char(victim,&af);
		one_hit_new(ch,victim,gsn_boneshatter,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,175,NULL);
		check_improve(ch,gsn_boneshatter,TRUE,1);
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
	} else {
		damage(ch, victim, 0, gsn_boneshatter,DAM_NONE,TRUE);
		check_improve(ch,gsn_boneshatter,FALSE,2);
		WAIT_STATE(ch,PULSE_VIOLENCE*2);
	}

	return;
}


void spell_rejuvenate( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	victim->hit = UMIN( victim->hit + 300, victim->max_hit );
	update_pos( victim );
	send_to_char( "A very warm feeling fills your body.\n\r", victim );
	if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
	return;
}

void do_powers(CHAR_DATA *ch, char *argument)
{
	BUFFER *buffer;
	char arg[MAX_INPUT_LENGTH];
	char skill_list[LEVEL_HERO + 1][MAX_STRING_LENGTH];
	char skill_columns[LEVEL_HERO + 1];
	int gn, gns, sn, level, min_lev = 1, max_lev = LEVEL_HERO;
	bool fAll = FALSE, found = FALSE;
	char buf[MAX_STRING_LENGTH];


	if (IS_NPC(ch))
	return;

	if (ch->cabal == 0) {
		send_to_char("You are homeless; you have no powers.\n\r",ch);
		return;
	}

	if (argument[0] != '\0')
	{
		fAll = TRUE;

		if (str_prefix(argument,"all"))
		{
			argument = one_argument(argument,arg);
			if (!is_number(arg))
			{
				send_to_char("Arguments must be numerical or all.\n\r",ch);
				return;
			}

			max_lev = atoi(arg);

			if (max_lev < 1 || max_lev > LEVEL_HERO)
			{
				sprintf(buf,"Levels must be between 1 and %d.\n\r",LEVEL_HERO);
				send_to_char(buf,ch);
				return;
			}

			if (argument[0] != '\0')
			{
				argument = one_argument(argument,arg);
				if (!is_number(arg))
				{
					send_to_char("Arguments must be numerical or all.\n\r",ch);
					return;
				}
				min_lev = max_lev;
				max_lev = atoi(arg);

				if (max_lev < 1 || max_lev > LEVEL_HERO)
				{
					sprintf(buf,"Levels must be between 1 and %d.\n\r",LEVEL_HERO);
					send_to_char(buf,ch);
					return;
				}

				if (min_lev > max_lev)
				{
					send_to_char("That would be silly.\n\r",ch);
					return;
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

	gn = group_lookup(cabal_table[ch->cabal].name);

	for (gns = 0; gns < MAX_SKILL; gns++)
	{
		if (group_table[gn].spells[gns] == NULL )
		break;

		sn = skill_lookup(group_table[gn].spells[gns]);

		if ((level = skill_table[sn].skill_level[ch->class]) < LEVEL_HERO + 1
		&&  level >= min_lev && level <= max_lev
		&&  ch->pcdata->learned[sn] > 0)
		{
			found = TRUE;
			level = skill_table[sn].skill_level[ch->class];
			if (ch->level < level) {
				sprintf(buf,"%-18s n/a      ", skill_table[sn].name);
			} else {
				sprintf(buf,"%-18s %3d%%      ",skill_table[sn].name,	ch->pcdata->learned[sn]);
 			}

			if (skill_list[level][0] == '\0')
			{
				sprintf(skill_list[level],"\n\rLevel %2d: %s",level,buf);
			} else /* append */ {
				if ( ++skill_columns[level] % 2 == 0)
				{
				strcat(skill_list[level],"\n\r          ");
				}
				strcat(skill_list[level],buf);
			}
		}
	}

	/* return results */

	if (!found)
	{
		send_to_char("No powers found.\n\r",ch);
      	return;
	}

	buffer = new_buf();
	for (level = 0; level < LEVEL_HERO + 1; level++)
	{
		if (skill_list[level][0] != '\0')
		{
			add_buf(buffer,skill_list[level]);
		}
	}
	add_buf(buffer,"\n\r");
	page_to_char(buf_string(buffer),ch);
	free_buf(buffer);
	return;
}

void spell_spiderhands( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if (victim != ch)
	{
		send_to_char("You cannot cast this power on another.\n\r",ch);
		return;
	}

	if (is_affected(victim, sn))
	{
		if (victim == ch)
			send_to_char("Your hands are already covered in a sticky coating.\n\r",ch);
		return;
	}
	init_affect(&af);
	af.where     = TO_AFFECTS;
	af.type      = sn;
	af.aftype    = AFT_POWER;
	af.level     = level;
	af.duration  = 10;
	af.modifier  = 0;
	af.bitvector = 0;
	affect_to_char( victim, &af );
	send_to_char( "Your hands are covered in a sticky coating.\n\r", victim );
	return;
}

bool check_chargeset(CHAR_DATA *ch, CHAR_DATA *victim)
{
	int chance;
	OBJ_DATA *weapon;

	if ((chance = get_skill(victim,gsn_chargeset)) == 0
		|| victim->level < skill_table[gsn_chargeset].skill_level[ch->class])
	return FALSE;

	/* check if primary wielding a polearm */
	weapon = get_eq_char(victim,WEAR_WIELD);

	if (weapon == NULL)
		return FALSE;

	if (weapon->value[0] != WEAPON_POLEARM)
		return FALSE;

	/* determine ups/downs in chance */

	if (!can_see(victim,ch))
		chance -= 30;

	if (victim->position != POS_FIGHTING && IS_AWAKE(victim))
	{
		if (number_percent() < chance)
		{
			act("$n's attack is suddenly halted by the tip of $p!",ch,weapon,victim,TO_VICT);
			act("Your attack is suddenly halted by the tip of $p!",ch,weapon,victim,TO_CHAR);
			act("$n's attack is suddenly halted by the tip of $p!",ch,weapon,victim,TO_NOTVICT);
			check_improve(victim,gsn_chargeset,TRUE,1);
			one_hit_new(victim, ch,gsn_chargeset,HIT_NOSPECIALS,HIT_UNBLOCKABLE,HIT_NOADD,350,NULL);

			return TRUE;
		}
		act("Your $p fails to stop $n.",ch,weapon,victim,TO_VICT);
		check_improve(victim,gsn_chargeset,FALSE,2);
	}

	return FALSE;
}

void do_headclap( CHAR_DATA *ch,char *argument)
{
	AFFECT_DATA af;
	CHAR_DATA *victim;
	int chance;
	char buf[MAX_STRING_LENGTH];
	char arg[MAX_STRING_LENGTH];
	OBJ_DATA *weapon;

	one_argument (argument, arg);

	if ((chance = get_skill(ch,gsn_headclap)) == 0)
	{
		send_to_char("You don't know how to headclap people.\n\r",ch);
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

	if (is_affected(victim,gsn_noheadclap))
	{
		send_to_char("They are guarding their ears to well.\n\r",ch);
		return;
	}

	if (victim == ch)
	{
		send_to_char("You headclap yourself repeatedly to no avail.\n\r",ch);
		return;
	}

	weapon = get_eq_char(ch,WEAR_WIELD);

	if (weapon != NULL) {
		send_to_char("You cannot headclap while wielding a weapon.\n\r",ch);
		return;
	}

	if (get_eq_char(ch, WEAR_SHIELD) != NULL)
	{
		send_to_char("You cannot headclap while wielding a shield.\n\r",ch);
		return;
	}

	if (is_safe(ch,victim))
	return;

	if (is_affected(victim,gsn_deafen))
	{
		send_to_char("Their ears are already deafened.\n\r",ch);
		return;
	}

	chance += ch->level - victim->level;
	chance += get_curr_stat(ch,STAT_STR);
	chance -= get_curr_stat(victim,STAT_DEX);
	if (IS_AFFECTED(ch,AFF_HASTE))
		chance += 10;
	if (is_affected(victim,skill_lookup("haste")))
		chance -= 10;

	chance -= 45;

	if (chance > 60) {
		chance = 60;
	}

	if (ch->level == MAX_LEVEL) {
		chance = 100;
	}

	if (!IS_NPC(victim) && (victim->fighting == NULL)) {
		switch(number_range(0,1)) {
		case (0):
			sprintf(buf,"Help! %s is slapping my head!",PERS(ch,victim));
			break;
		case (1):
			sprintf(buf,"Help! %s is boxing my ears!",PERS(ch,victim));
			break;
		}
		do_myell(victim,buf);
	}

	act("You whirl your arms at $N, slamming your palms against $S head!",ch,0,victim,TO_CHAR);
	act("$n whirls $s arms at $N, slamming $s palms against $S head!",ch,0,victim,TO_NOTVICT);
	act("$n whirls $s arms at you, slamming $s palms against your head!",ch,0,victim,TO_VICT);

	if (number_percent() < chance)
	{
		one_hit(ch, victim, gsn_headclap);
		act("$n appears deafened.",victim,0,0,TO_ROOM);
		act("A loud ringing fills your ears... you can't hear anything!",victim,0,0,TO_CHAR);
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.level = ch->level;
		af.location = 0;
		af.type = gsn_deafen;
		af.duration = 4;
		af.bitvector = 0;
		af.modifier = 0;
		af.duration = 2;
		char msg_buf[MSL];
		sprintf(msg_buf,"prevents casting and induces a deafened state");
		af.affect_list_msg = str_dup(msg_buf);
		affect_to_char(victim,&af);
		af.where = TO_AFFECTS;
		af.location = 0;
		af.bitvector = 0;
		af.type=gsn_noheadclap;
		af.aftype = AFT_SKILL;
		af.duration=10;
		affect_to_char(victim,&af);
		check_improve(ch,gsn_headclap,TRUE,2);
		WAIT_STATE(ch,2*PULSE_VIOLENCE);
		return;
	} else {
		one_hit(ch, victim, gsn_headclap);
		check_improve(ch,gsn_headclap,FALSE,3);
		WAIT_STATE(ch,2*PULSE_VIOLENCE);
	}
	return;
}

void do_heroimm( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *victim;
	char arg[MAX_STRING_LENGTH];

	if (IS_NPC(ch))
	return;

	one_argument(argument,arg);

	if (ch->level < MAX_LEVEL) {
		return;
	}

	if (arg[0] == '\0')
	{
		send_to_char("Heroimm who?\n\r",ch);
		return;
	}

	if ((victim = get_char_world(ch,arg)) == NULL) {
		send_to_char("They aren't here.\n\r", ch);
		return;
	}

	if (victim == ch) {
		send_to_char("Uh, no?\n\r",ch);
		return;
	}

	if (IS_HEROIMM(victim)) {
		send_to_char("They are already a Hero Imm.\n\r", ch);
		return;
	}

	SET_BIT(victim->act,PLR_HEROIMM);
	SET_BIT(victim->act,PLR_HOLYLIGHT);

	victim->max_hit = 10000;
	victim->hit = victim->max_hit;
	victim->max_mana = 10000;
	victim->mana = victim->max_mana;
	victim->max_move = 10000;
	victim->move = victim->max_move;

	send_to_char("They are now a Hero Immortal.\n\r", ch);
	send_to_char("You are now a Hero Immortal.\n\r", victim);

	return;
}

void check_eagle_eyes(CHAR_DATA *ch, CHAR_DATA *victim)
{
	char buf[MAX_STRING_LENGTH];
	CHAR_DATA *gch;
	int chance;

	for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
	{
		if (gch == ch || gch == victim) {
			continue;
		}
		if (!can_see(gch,ch)) {
			continue;
		}
		chance = get_skill(gch,gsn_eagle_eyes);
		if (chance == 0) {
			continue;
		}
		if (number_percent() < chance)
		{
			sprintf(buf,"You notice %s slipping into %s's inventory.\n\r",PERS(ch,gch),PERS(victim,gch));
			send_to_char(buf,gch);
			check_improve(gch,gsn_eagle_eyes,TRUE,1);
		}
	}

	return;
}

bool check_cutoff(CHAR_DATA *ch, CHAR_DATA *victim)
{
	int chance;
	OBJ_DATA *weapon;

	if ((chance = get_skill(victim,gsn_cutoff)) == 0)
	return FALSE;

	/* check if primary wielding a polearm */
	weapon = get_eq_char(victim,WEAR_WIELD);

	if (weapon == NULL)
	{
		return FALSE;
	}

	if ((weapon->value[0] != WEAPON_POLEARM && weapon->value[0] != WEAPON_STAFF && weapon->value[0] != WEAPON_SPEAR))
	{
		return FALSE;
	}

	/* determine ups/downs in chance */

	if (!can_see(victim,ch))
		chance -= 30;

	chance += (victim->level - ch->level);

	if (chance > 65)
		chance = 65;

	if (number_percent() < chance)
	{
		act("$n's route of escape is cut off by $p.",ch,weapon,victim,TO_VICT);
		act("Your route of escape is cut off by $p.",ch,weapon,victim,TO_CHAR);
		act("$n's route of escape is cut off by $p.",ch,weapon,victim,TO_NOTVICT);
		check_improve(victim,gsn_cutoff,TRUE,1);
		return TRUE;
	  	WAIT_STATE(victim, PULSE_VIOLENCE);
	} else {
		act("You fail to cut off $n's route of escape.",ch,weapon,victim,TO_VICT);
		check_improve(victim,gsn_cutoff,FALSE,2);
		return FALSE;
	}
}

void report_cabal_items(CHAR_DATA *ch, char *argument)
{
	char buf[MAX_STRING_LENGTH];
	char pbuf[MAX_STRING_LENGTH];
	CHAR_DATA *gch;
	CHAR_DATA *guardian;
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;
	bool found;
	bool afound;
	int total=0;
	if (strstr(argument,"which items") || strstr(argument,"Which items"))
	{

        if (ch->cabal == 0 && !IS_IMMORTAL(ch))
                return;

	found = FALSE;
	afound = FALSE;
	guardian = NULL;
	for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
	{
		if (IS_CABAL_GUARD(gch)) {
			if (ch->cabal != gch->cabal && !IS_IMMORTAL(ch)) {
				continue;
			}
			sprintf(buf,"I hold ");
			for (obj = gch->carrying; obj != NULL; obj = obj_next )
			{
				obj_next = obj->next_content;
				if ((obj->pIndexData->vnum) && isCabalItem(obj))
				{
				total++;
				found = TRUE;
				if (obj_next != NULL) {
					sprintf(pbuf,"%s, ",obj->short_descr);
					strcat(buf,pbuf);
				} else {
					if (total == 1) {
						sprintf(pbuf,"%s.",obj->short_descr);
						strcat(buf,pbuf);
					} else {
						sprintf(pbuf,"and %s.",obj->short_descr);
						strcat(buf,pbuf);
					}
				}
				}
			}
			guardian=gch;
			if (!found) {
				strcat(buf,"nothing.");
			}
			afound = TRUE;
		}
	}
	if(afound && guardian!=NULL)
		do_say(guardian,buf);
	return;
	}

	guardian=NULL;
	if (strstr(argument,"where is") || strstr(argument,"Where is"))
	{
	if (ch->cabal == 0 && !IS_IMMORTAL(ch))
		return;
	for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
	{
		if (IS_CABAL_GUARD(gch)) {
			if (ch->cabal == gch->cabal || IS_IMMORTAL(ch)) {
				guardian=gch;
			}
		}
	}
	if(guardian==NULL)
		return;
	for (obj = object_list; obj != NULL; obj = obj_next)
	{
	obj_next = obj->next;
		if ((obj->pIndexData->vnum) && obj->pIndexData->cabal>0 && obj->pIndexData->cabal==guardian->cabal && obj->carried_by!=NULL)
		{
			sprintf(pbuf,"%s is carried by %s.",obj->short_descr,IS_NPC(obj->carried_by) ? obj->carried_by->short_descr : obj->carried_by->name);
			pbuf[0] = UPPER(pbuf[0]);
			break;
		}
	}
	if(guardian!=NULL && pbuf!=NULL && obj!=NULL)
		do_say(guardian,pbuf);
	}
}

void spell_aura(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if(is_affected(victim,skill_lookup("aura"))) {
		if(victim == ch) {
			return send_to_char("You are already protected by a magical aura.\n\r",ch);
	      } else {
			send_to_char("You are already protected by a magical aura.\n\r",victim);
			send_to_char("They are already protected by a magical aura\n\r",ch);
			return;
		}
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
	af.type = skill_lookup("aura");
	af.affect_list_msg = str_dup("reduces damage taken by 30%");
	af.duration = level/6;
	af.owner_name = str_dup(ch->original_name);
	affect_to_char(victim,&af);
	if(victim == ch) {
	act("$n is surrounded by a magical aura.",ch,0,0,TO_ROOM);
	send_to_char("You are surrounded by a magical aura.\n\r",ch);
	return;
	} else {
	act("$N is surrounded by a magical aura.",ch,0,victim,TO_NOTVICT);
	act("$N is surrounded by a magical aura.",ch,0,victim,TO_CHAR);
	send_to_char("You are surrounded by a magical aura.\n\r",victim);
	return;
	}
}

void spell_soften(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	int chance;
	bool found = FALSE;

	if(is_affected(victim,skill_lookup("soften")))
		return send_to_char("Their skin is already softened.\n\r",ch);
	
	chance = get_skill(ch,skill_lookup("soften"));
	if(is_affected(victim,skill_lookup("stone skin"))) {
		found = TRUE;
		chance -= 75;
	}

	if(number_percent() < chance) {
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_SPELL;
		af.duration = ch->level/7;
		af.modifier = -40 * AC_PER_ONE_PERCENT_DECREASE_DAMAGE;
		af.location = APPLY_AC;
		af.owner_name = str_dup(ch->original_name);
		af.type = skill_lookup("soften");
		affect_to_char(victim,&af);
		act("$N's skin becomes soft and supple.",ch,0,victim,TO_CHAR);
		act("$N's skin becomes soft and supple.",ch,0,victim,TO_NOTVICT);
		act("Your skin softens around you.",ch,0,victim,TO_VICT);
		if(found) {
			affect_strip(victim,skill_lookup("stone skin"));
			send_to_char("Your skin loses its stone protection.\n\r",victim);
		}
		return;
	} else {
		act("$N's skin resists your softening.",ch,0,victim,TO_CHAR);
		act("Your skin feels momentarily softer, but quickly firmens.",ch,0,victim,TO_VICT);
		return;
	}
} 

void do_dedicate(CHAR_DATA *ch, char *argument)
{ 
	int gns, gn = 0, sn;

	if (ch->class!=class_lookup("paladin"))
		return send_to_char("You cannot dedicate unless you are a Paladin.\n\r",ch);

	if (ch->pcdata->dedication != NO_DEDICATION)
		return send_to_char("You've already dedicated.\n\r", ch);
  
	if (!str_cmp(argument, "shield"))
	{
  		ch->pcdata->dedication = DED_SHIELD;
		gn = group_lookup("ded shield");
		if (gn == -1) return;
		send_to_char("You have chosen to dedicate your life to the Shield.\n\r",ch); 
		send_to_char("{yYou feel graced with the knowledge of Shield of Purity!{x\n\r",ch);
		send_to_char("{yYou feel graced with the knowledge of Shield of Redemption!{x\n\r",ch);
		send_to_char("{yYou feel graced with the knowledge of Shield of Truth!{x\n\r",ch);
		send_to_char("{yYou feel graced with the knowledge of Shield of Justice!{x\n\r",ch);
	} 

	else if (!str_cmp(argument, "two-hand")) 
	{
  		ch->pcdata->dedication = DED_TWOHAND;
		gn = group_lookup("ded sword");
		if (gn == -1) return;
		send_to_char("You have chosen to dedicate your life to the Two-Handed Sword.\n\r",ch);
		send_to_char("{yYou feel graced with the knowledge of Swordplay!{x\n\r",ch);
		send_to_char("{yYou feel graced with the knowledge of Drive!{x\n\r",ch);
	} 

	else 
	{
  		send_to_char("That is not a valid dedication.\nValid dedications: two-hand, shield.\n\r",ch);
		return; 
	}
  
	for (gns = 0; gns < MAX_SKILL; gns++)
        {
		if (gn < 1 || group_table[gn].spells[gns] == NULL )
	   		break;
     
		sn = skill_lookup(group_table[gn].spells[gns]);
        
		if (sn > 0 && skill_table[sn].skill_level[ch->class] < LEVEL_HERO + 1)
	   		ch->pcdata->learned[sn] = 1;
        }
	return;
}


void do_stance(CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA *check, *checkLooper, af;

	if (argument[0] == '\0')
		return send_to_char("Which combat stance do you wish to take?\n\r",ch);

	if (!str_prefix(argument,"none"))
	{
		if (is_affected(ch,gsn_stance))
		{
			affect_strip(ch,gsn_stance);
			for (check = ch->affected; check != NULL; check = checkLooper)
			{
				checkLooper = check->next;
				if (check->type == skill_lookup("body of diamond") && !str_cmp(check->name,"diamondback stance"))
					affect_remove(ch,check);
			}
			return send_to_char("You come up from your fighting stance.\n\r",ch);
			WAIT_STATE(ch,PULSE_VIOLENCE);
		}
		else
			return send_to_char("You aren't in a combat stance!\n\r",ch);
	}

	
	if (get_skill(ch,gsn_stance) < 5)
		return send_to_char("You do not know how to take an effective combat stance.\n\r",ch);

	if (is_affected(ch,gsn_stance))
		return send_to_char("You are already in a combat stance!\n\r",ch);

	if (!str_prefix(argument,"black bear"))
	{
		if (ch->race != race_lookup("cloud") && ch->race != race_lookup("troll"))
			return send_to_char("Only cloud giants and trolls are capable of adopting the Black Bear stance.\n\r",ch);
		else
		{
			init_affect(&af);
			af.where 	= TO_AFFECTS;
			af.aftype 	= AFT_SKILL;
			af.type 	= gsn_stance;
			af.level 	= 1;
			af.duration	= -1;
			af.name		= str_dup("black bear stance");
			af.location	= APPLY_ENHANCED_MOD;
			af.modifier 	= 20;
			affect_to_char(ch,&af);
			act("$n drops into a combat stance.",ch,0,0,TO_ROOM);
			act("You drop into the Black Bear stance.",ch,0,0,TO_CHAR);
		}
	}
	else if (!str_prefix(argument,"rabbit"))
	{
		if (ch->race != race_lookup("elf") && ch->race != race_lookup("wood-elf"))
			return send_to_char("Only elves and wood-elves are capable of adopting the Rabbit stance.\n\r",ch);
		else
		{
			init_affect(&af);
			af.where	= TO_AFFECTS;
			af.aftype	= AFT_SKILL;
			af.type		= gsn_stance;
			af.level	= 2;
			af.duration	= -1;
			af.name		= str_dup("rabbit stance");
			af.location	= APPLY_AC;
			af.modifier	= 20 * AC_PER_ONE_PERCENT_DECREASE_DAMAGE;
			affect_to_char(ch,&af);
			act("$n drops into a combat stance.",ch,0,0,TO_ROOM);
			act("You drop into the Rabbit stance.",ch,0,0,TO_CHAR);
		}
	}
	else if (!str_prefix(argument,"deer"))
	{
		if (ch->race != race_lookup("centaur"))
			return send_to_char("Only centaurs are capable of adopting the Deer stance.\n\r",ch);
		else
		{
			init_affect(&af);
			af.where	= TO_AFFECTS;
			af.aftype	= AFT_SKILL;
			af.type		= gsn_stance;
			af.level	= 3;
			af.duration	= -1;
			af.name		= str_dup("deer stance");
			af.location 	= APPLY_ATTACK_MOD;
			af.modifier	= 2;
			affect_to_char(ch,&af);
			act("$n drops into a combat stance.",ch,0,0,TO_ROOM);
			act("You drop into the Deer stance.",ch,0,0,TO_CHAR);
		}
	}
	else if (!str_prefix(argument,"wolf"))
	{
		init_affect(&af);
		af.where	= TO_AFFECTS;
		af.aftype	= AFT_SKILL;
		af.type		= gsn_stance;
		af.level	= 4;
		af.duration	= -1;
		af.name		= str_dup("wolf stance");
		af.location 	= APPLY_ATTACK_MOD;
		af.modifier	= 1;
		affect_to_char(ch,&af);
		af.location	= APPLY_ENHANCED_MOD;
		af.modifier	= 5;
		affect_to_char(ch,&af);
		act("$n drops into a combat stance.",ch,0,0,TO_ROOM);
		act("You drop into the Wolf stance.",ch,0,0,TO_CHAR);
	}
	else if (!str_prefix(argument,"mongoose"))
	{
		init_affect(&af);
		af.where	= TO_AFFECTS;
		af.aftype	= AFT_SKILL;
		af.type		= gsn_stance;
		af.level	= 6;
		af.duration	= -1;
		af.name		= str_dup("mongoose stance");
		af.location	= APPLY_HITROLL;
		af.modifier	= 45;
		affect_to_char(ch,&af);
		af.location	= APPLY_DAMROLL;
		af.modifier	= 5;
		affect_to_char(ch,&af);
		act("$n drops into a combat stance.",ch,0,0,TO_ROOM);
		act("You drop into the Mongoose stance.",ch,0,0,TO_CHAR);
	}
	else if (!str_prefix(argument,"ent"))
	{
		init_affect(&af);
		af.where	= TO_AFFECTS;
		af.aftype	= AFT_SKILL;
		af.type		= gsn_stance;
		af.level	= 7;
		af.duration	= -1;
		af.name		= str_dup("ent stance");
		af.location	= APPLY_AC;
		af.modifier	= 60 * AC_PER_ONE_PERCENT_DECREASE_DAMAGE;
		affect_to_char(ch,&af);
		af.location	= APPLY_ENHANCED_MOD;
		af.modifier	= -60;
		affect_to_char(ch,&af);	
		act("$n drops into a combat stance.",ch,0,0,TO_ROOM);
		act("You drop into the Ent stance.",ch,0,0,TO_CHAR);
	}
	else if (!str_prefix(argument,"hawk"))
	{
		init_affect(&af);
		af.where	= TO_AFFECTS;
		af.aftype	= AFT_SKILL;
		af.type		= gsn_stance;
		af.level	= 8;
		af.duration	= -1;
		af.name		= str_dup("hawk stance");
		af.location	= APPLY_ENHANCED_MOD;
		af.modifier	= 6;
		affect_to_char(ch,&af);
		af.location	= APPLY_HITROLL;
		af.modifier	= 20;
		affect_to_char(ch,&af);
		af.location = APPLY_NONE;
		af.modifier = 0;
		af.bitvector	= AFF_FLYING;
		af.affect_list_msg = str_dup("grants flight");
		affect_to_char(ch,&af);
		act("$n drops into a combat stance.",ch,0,0,TO_ROOM);
		act("You drop into the Hawk stance.",ch,0,0,TO_CHAR);
	}
	else if (!str_prefix(argument,"diamondback"))
	{
		init_affect(&af);
		af.where	= TO_AFFECTS;
		af.aftype	= AFT_INVIS;
		af.type		= gsn_stance;
		af.level	= 8;
		af.duration	= -1;
		af.name		= str_dup("diamondback stance");
		affect_to_char(ch,&af);
		af.aftype 	= AFT_SKILL;
		af.type		= skill_lookup("body of diamond");
		af.duration	= -1;
		af.affect_list_msg = str_dup("grants a chance to reflect spellcasting");
		af.name		= str_dup("diamondback stance");
		affect_to_char(ch,&af);
		act("$n drops into a combat stance.",ch,0,0,TO_ROOM);
		act("You drop into the Diamondback stance.",ch,0,0,TO_CHAR);
	}
	else
		return send_to_char("That is not a valid stance.\n\r",ch);
	return;
}

void do_march(CHAR_DATA *ch, char *argument)
{
	if (get_skill(ch,skill_lookup("march")) < 75)
		return send_to_char("You do not know how to march effectively.\n\r",ch);
	if (is_affected(ch,skill_lookup("march")))	
		return send_to_char("You are already marching effectively.\n\r",ch);
	int move = ch->move;
	int hp = ch->hit;
	AFFECT_DATA af;

	ch->move += move * number_range(1.2, 1.3);
	if (ch->move > ch->max_move)
		ch->move = ch->max_move;
	ch->hit -= hp * .2;
	init_affect(&af);
	af.aftype = AFT_SKILL;
	af.type   = skill_lookup("march");
	af.name   = str_dup("march of the zephyr");
	af.affect_list_msg = str_dup("grants flight and raises dexterity by 2");
	af.duration = 10;
	af.bitvector = AFF_FLYING;
	af.location = APPLY_DEX;
	af.modifier = 2;
	affect_to_char(ch,&af);
	act("Sacrificing some of your life energy, you summon divine winds to aid you!",ch,0,0,TO_CHAR);
	act("The winds sweep under your feet and you begin marching upon them.",ch,0,0,TO_CHAR);
	act("The wind sweeps under $n's feet and lifts him slightly into the air.",ch,0,0,TO_ROOM);
	WAIT_STATE(ch,PULSE_VIOLENCE);
	return;
}

