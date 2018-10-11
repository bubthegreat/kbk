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

/* command procedures needed */
DECLARE_DO_FUN(do_gtell		);
DECLARE_DO_FUN(do_follow	);
DECLARE_DO_FUN(do_look		);
DECLARE_DO_FUN(do_laying_hands	);
DECLARE_DO_FUN(do_barkskin	);
DECLARE_DO_FUN(do_yell		);
DECLARE_DO_FUN(do_myell		);
DECLARE_DO_FUN(do_say		);
DECLARE_DO_FUN(do_flee		);
DECLARE_DO_FUN(do_murder        );

/*
 * Local functions.
 */
void	say_spell	args( ( CHAR_DATA *ch, int sn ) );
int	spellcraft_dam	args( ( int num, int dice) );
bool	check_spellcraft args( (CHAR_DATA *ch, int sn) );
bool 	eye_of_beholder_nullify args((CHAR_DATA *ch));
bool 	cleansed args ((CHAR_DATA *ch, CHAR_DATA *victim,int diffmodifier, int sn));
bool 	check_counterspell args((CHAR_DATA *ch, CHAR_DATA *victim));
bool    remove_obj      args( ( CHAR_DATA *ch, int iWear, bool fReplace ) );
bool	is_affected_obj args( ( OBJ_DATA *obj, int sn )	);
void 	wear_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace ) );



/*
 * Lookup a skill by name.
 */
int skill_lookup( const char *name )
{
    int sn;

    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
	if ( skill_table[sn].name == NULL )
	    break;
	if ( LOWER(name[0]) == LOWER(skill_table[sn].name[0])
	&&   !str_prefix( name, skill_table[sn].name ) )
	    return sn;
    }

    return -1;
}

int find_spell( CHAR_DATA *ch, const char *name )
{
    /* finds a spell the character can cast if possible */
    int sn, found = -1;

    if (IS_NPC(ch))
	return skill_lookup(name);

    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
	if (skill_table[sn].name == NULL)
	    break;
	if (LOWER(name[0]) == LOWER(skill_table[sn].name[0])
	&&  !str_prefix(name,skill_table[sn].name))
	{
	    if ( found == -1)
		found = sn;
	    if (ch->level >= skill_table[sn].skill_level[ch->class]
	    &&  ch->pcdata->learned[sn] > 0)
		    return sn;
	}
    }
    return found;
}



/*
 * Lookup a skill by slot number.
 * Used for object loading.
 */
int slot_lookup( int slot )
{
    extern bool fBootDb;
    int sn;

    if ( slot <= 0 )
	return -1;

    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
	if ( slot == skill_table[sn].slot )
	    return sn;
    }

    if ( fBootDb )
    {
	bug( "Slot_lookup: bad slot %d.", slot );
	abort( );
    }

    return -1;
}

/* Eye of beholder dispel code */
bool eye_of_beholder_nullify(CHAR_DATA *ch)
{
    OBJ_DATA *eye;
    eye = get_eq_char(ch,WEAR_LIGHT);
    if (eye == NULL
    || eye->pIndexData->vnum != 14003)
        return FALSE;
    if (saves_spell(eye->level, ch,DAM_OTHER))
        return FALSE;
    send_to_char("The beholder eye in your hands nullifies your magic!\n\r",ch);
    act("$n's spell fizzles.",ch,0,0,TO_ROOM);
    return TRUE;
}



/*
 * Utter mystical words for an sn.
 */
void say_spell( CHAR_DATA *ch, int sn )
{
    char buf  [MAX_STRING_LENGTH];
    char buf2 [MAX_STRING_LENGTH];
    CHAR_DATA *rch;
    char *pName;
    int iSyl;
    int length;

    struct syl_type
    {
	char *	old;
	char *	new;
    };

    static const struct syl_type syl_table[] =
    {
	{ " ",          " "             },
	{ "ar",         "abra"          },
	{ "au",         "kada"          },
	{ "bless",      "fido"          },
	{ "blind",      "nose"          },
	{ "bur",        "mosa"          },
	{ "cu",         "judi"          },
	{ "de",         "oculo"         },
	{ "en",         "unso"          },
	{ "light",      "dies"          },
	{ "lo",         "hi"            },
	{ "mor",        "zak"           },
	{ "move",       "sido"          },
	{ "ness",       "lacri"         },
	{ "ning",       "illa"          },
	{ "per",        "duda"          },
	{ "ra",         "gru"           },
	{ "fresh",      "ima"           },
	{ "re",         "candus"        },
	{ "son",        "sabru"         },
	{ "tect",       "infra"         },
	{ "tri",        "cula"          },
	{ "ven",        "nofo"          },
	{ "a", "a" }, { "b", "b" }, { "c", "q" }, { "d", "e" },
	{ "e", "z" }, { "f", "y" }, { "g", "o" }, { "h", "p" },
	{ "i", "u" }, { "j", "y" }, { "k", "t" }, { "l", "r" },
	{ "m", "w" }, { "n", "i" }, { "o", "a" }, { "p", "s" },
	{ "q", "d" }, { "r", "f" }, { "s", "g" }, { "t", "h" },
	{ "u", "j" }, { "v", "z" }, { "w", "x" }, { "x", "n" },
	{ "y", "l" }, { "z", "k" },
	{ "", "" }
    };

    if ( (sn == skill_lookup("power word kill"))
	|| (sn == skill_lookup("power word stun")) )
	return;

    buf[0]	= '\0';
    for ( pName = skill_table[sn].name; *pName != '\0'; pName += length )
    {
	for ( iSyl = 0; (length = strlen(syl_table[iSyl].old)) != 0; iSyl++ )
	{
	    if ( !str_prefix( syl_table[iSyl].old, pName ) )
	    {
		strcat( buf, syl_table[iSyl].new );
		break;
	    }
	}

	if ( length == 0 )
	    length = 1;
    }

    sprintf( buf2, "$n utters the words, '%s'.", buf );
    sprintf( buf,  "$n utters the words, '%s'.", skill_table[sn].name );

    for ( rch = ch->in_room->people; rch; rch = rch->next_in_room )
    {
	if ( rch != ch )
            act((IS_IMMORTAL(rch) && IS_SET(rch->act,PLR_HOLYLIGHT)) || (!IS_NPC(rch) && ch->class==rch->class) ? buf : buf2,
            ch, NULL, rch, TO_VICT );
	   /* act( ch->class==rch->class ? buf : buf2, ch, NULL, rch, TO_VICT );*/
    }

    return;
}



/*
 * Compute a saving throw.
 * Negative apply's make saving throw better.
 */
bool saves_spell( int level, CHAR_DATA *victim, int dam_type )
{
    int save;

    if(victim->fighting && get_trust(victim->fighting)==MAX_LEVEL)
	return FALSE;  

	//Base 35% chance to save
	//Subtract 5% for the difference between the spell's level and the victim's level
	//Add 2/3 of the victim's saving throw in
	//So for a 51 spell with -100 svs, 35 + 66 = 101 chance to save
    save = 35 + ( victim->level - level) * 5 - (victim->saving_throw*.66);
    
    if (IS_AFFECTED(victim,AFF_BERSERK))
	save += (victim->drain_level + victim->level)/5;

    if (IS_NPC(victim))
	save += victim->level/3;

    switch(check_immune(victim,dam_type))
    {
	case IS_IMMUNE:		return TRUE;
	case IS_RESISTANT:	save *= 1.3;	break;
	case IS_VULNERABLE:	save *= .7;	break;
    }

    if(is_affected(victim,skill_lookup("strength of faith")) && (number_percent()<50 || dam_type==DAM_HOLY))
	return TRUE;
	
	if(get_skill(victim,skill_lookup("elemental affinity")) > 75 && dam_type > 3) {
		int chance = number_percent();
		save = (10 + (chance / 10)) * save / 10;
	}

    save = 9 * save / 10;

    if (!IS_NPC(victim))
    n_logf("Saves_spell: VICTIM=%s LEVEL=%d DAM=%d SAVE=%d", victim->name, level, dam_type, save);

    save = URANGE( 5, save, 85 );
    return number_percent( ) < save;
}

/* RT save for dispels */

bool saves_dispel( int dis_level, int spell_level, int duration)
{
    int save;

    save = 68 + (spell_level - dis_level) * 5;
    if(duration==-1)
		return TRUE;
		
    save = URANGE( 20, save, 80 );
    return number_percent( ) < save;
}

/* for finding mana costs -- temporary version */
int mana_cost (CHAR_DATA *ch, int min_mana, int level)
{
    if (ch->level + 2 == level)
	return 1000;
    return UMAX(min_mana,(100/(2 + ch->level - level)));
}

void do_laying_hands( CHAR_DATA *ch, char *argument )
{	
	char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;   
    int heal, sn;
    AFFECT_DATA af;

    if ( (get_skill(ch,gsn_laying_hands) == 0)
	|| (ch->level < skill_table[gsn_laying_hands].skill_level[ch->class] ) )
    {
	send_to_char("You do not have the ability to divinely heal.\n\r",ch);
	return;
    }

    one_argument(argument, arg);

    if (arg[0] == '\0')
    {
	send_to_char("Give your divine touch to whom?\n\r", ch);
	return;
    }

    if ( (victim = get_char_room(ch, arg)) == NULL )
    {
	send_to_char("They aren't here.\n\r",ch);
	return;
    }

    if (victim == ch)
	return send_to_char("That wouldn't be very selfless.\n\r", ch);

    if (is_affected(ch, gsn_laying_hands) )
	return send_to_char("The gods have not granted you divine touch yet.\n\r", ch);

    if (IS_EVIL(victim))
	return send_to_char("Your deity would not approve of that.\n\r",ch);

    if (ch->mana < 80)
	return send_to_char("You don't have the mana.\n\r",ch);
 
    ch->mana -= skill_table[gsn_laying_hands].min_mana;
    
    WAIT_STATE( ch, skill_table[gsn_laying_hands].beats );

    init_affect(&af);
    af.where     = TO_AFFECTS;
    af.aftype    = AFT_SKILL;
    af.type      = gsn_laying_hands;
    af.level     = ch->level;
    af.duration  = 24;
    af.modifier  = 0;
    af.location  = 0;
    af.bitvector = 0;
	af.affect_list_msg = str_dup("prevents usage of laying hands");
    affect_to_char( ch, &af);

    sn=skill_lookup("poison");
    if(is_affected(victim,sn) && cleansed(ch,victim,2,sn))
    {
	act("$n looks less ill.",victim,0,ch,TO_ROOM);
	send_to_char(skill_table[sn].msg_off, victim );
	send_to_char("\n\r", victim);
	affect_strip(victim,sn);
    }

    sn=skill_lookup("blindness");
    if(is_affected(victim,sn) && cleansed(ch,victim,2,sn))
    {
	act("$n looks less ill.",victim,0,ch,TO_ROOM);
	send_to_char(skill_table[sn].msg_off, victim );
	send_to_char("\n\r", victim);
	affect_strip(victim,sn);
    }

    sn=skill_lookup("weaken");
    if(is_affected(victim,sn) && cleansed(ch,victim,1,sn))
    {
	act("$n looks stronger.",victim,0,ch,TO_ROOM);
	send_to_char(skill_table[sn].msg_off, victim );
	send_to_char("\n\r", victim);
	affect_strip(victim,sn);
    }

    sn=skill_lookup("plague");
    if(is_affected(victim,sn) && cleansed(ch,victim,2,sn))
    {
	act("$n looks less diseased.",victim,0,ch,TO_ROOM);
	send_to_char(skill_table[sn].msg_off, victim );
	send_to_char("\n\r", victim);
	affect_strip(victim,sn);
    }

    sn=skill_lookup("curse");
    if(is_affected(victim,sn) && cleansed(ch,victim,2,sn))
    {
 	act("$n's curse wears off.",victim,0,ch,TO_ROOM);
	send_to_char(skill_table[sn].msg_off, victim );
	send_to_char("\n\r", victim);
	affect_strip(victim,sn);
    }

    heal = ch->level * 6;
    victim->hit = UMIN(victim->hit + heal, victim->max_hit);
    act("$n lays $s hands upon $N and $N's wounds seem to heal.",ch,NULL,victim, TO_NOTVICT);
    act("You lay hands upon $N and $S wounds seem to heal.", ch, NULL, victim, TO_CHAR);
    act("$n lays hands upon you in blessed communication and your wounds heal.",ch,NULL,victim,TO_VICT);

   return;
}

/*
 * The kludgy global is for spells who want more stuff from command line.
 */
char *target_name;

void do_cast( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    void *vo;
    int mana;
    int sn;
    bool runeDagger=FALSE;

    int target;
    bool absorbed = FALSE;

    bool reflected = FALSE;
    /*
     * Switched NPC's can cast spells, but others can't.
     */
    if ( IS_NPC(ch) && ch->desc == NULL)
	return;

	if(class_table[ch->class].ctype!=CLASS_CASTER && !IS_IMMORTAL(ch))
	{
	send_to_char("You must ask your deity for prayers instead of pulling it from the world.\n\r",ch);
	return;
        }

    if (ch->cabal == CABAL_RAGER && !IS_IMMORTAL(ch))
    {
	send_to_char("You cannot cast spells as a Rager.\n\r",ch);
	return;
    }

    if (is_affected(ch, gsn_deafen) && number_percent()<40)
    {
       send_to_char("You can't get the right intonations.\n\r", ch);
	 WAIT_STATE(ch,PULSE_VIOLENCE);
	return;
    }
    if (is_affected(ch, gsn_garble) && number_percent()<40)
    {
       send_to_char("You can't get the right intonations.\n\r", ch);
	 WAIT_STATE(ch,PULSE_VIOLENCE);
	return;
    }

    if(is_affected(ch,gsn_tigerclaw))
    {
	if (number_percent() > 65) 	// Fail, the command does NOT go through
	{
		send_to_char("Blood bubbles from your throat as you fail to make sound.\n\r",ch);
		damage_old(ch,ch,number_range(50,200),gsn_tigerclaw,DAM_NONE,TRUE);
		return;		
		WAIT_STATE(ch,PULSE_VIOLENCE);
	} else {			// It goes through, and less damage   	
        	send_to_char("Your throat throbs with pain but you overcome it and continue.\n\r",ch);
		damage_old(ch,ch,number_range(20,80),gsn_tigerclaw,DAM_NONE,TRUE);
	}
    }

    if (is_affected(ch, gsn_gag) && number_percent()<90)
    {
    	send_to_char("You nearly choke as your try to speak with a gag in your mouth.\n\r",ch);
    	act("$n makes a muffled noise from behind a gag.",ch,NULL,NULL,TO_ROOM);
    	return;
    }

    target_name = one_argument( argument, arg1 );
    one_argument( target_name, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Cast which what where?\n\r", ch );
	return;
    }

    if ((sn = find_spell(ch,arg1)) < 1
    ||  skill_table[sn].spell_fun == spell_null
	|| (ch->level < skill_table[sn].skill_level[ch->class] && !IS_IMMORTAL(ch))
    || (IS_NPC(ch))
    || ch->pcdata->learned[sn] == 0)
    {
	send_to_char( "You don't know any spells of that name.\n\r", ch );
	return;
    }

    if ( ch->position < skill_table[sn].minimum_position )
    {
	send_to_char( "You can't concentrate enough.\n\r", ch );
	return;
    }

    if(is_affected(ch,gsn_shadowstrike) && number_percent()<35) {
	send_to_char("The shadowy arms restrict your movements.\n\r",ch);
	act("$n tries to cast a spell but is held back by shadowy arms.",ch,0,0,TO_ROOM);
	WAIT_STATE(ch,12);
	return;
    }

    if (ch->level + 2 == skill_table[sn].skill_level[ch->class])
	mana = 50;
    else
    	mana = UMAX(
	    skill_table[sn].min_mana,
	    100 / ( 2 + ch->level - skill_table[sn].skill_level[ch->class] ) );

		switch (get_curr_stat(ch,STAT_INT)) {
			case 25: mana = 43 * mana / 50; break;
			case 24: mana = 22 * mana / 25; break;
			case 23: mana = 45 * mana / 50; break;
			case 22: mana = 23 * mana / 25; break;
			case 21: mana = 47 * mana / 50; break;
			case 20: mana = 24 * mana / 25; break;
			case 19: mana = 49 * mana / 50; break;
		}
    {
	OBJ_DATA *ring;
	ring = get_eq_char(ch,WEAR_FINGER_R);
	if (ring != NULL && ring->pIndexData->vnum == OBJ_VNUM_WIZARDRY_1)
		mana -= (7*mana/100);
	else if (ring != NULL && ring->pIndexData->vnum == OBJ_VNUM_WIZARDRY_2)
		mana -= (15*mana/100);
	ring = get_eq_char(ch,WEAR_FINGER_L);
	if (ring != NULL && ring->pIndexData->vnum == OBJ_VNUM_WIZARDRY_1)
		mana -= (7*mana/100);
	else if (ring != NULL && ring->pIndexData->vnum == OBJ_VNUM_WIZARDRY_2)
		mana -= (15*mana/100);
    }
    /*
     * Locate targets.
     */
    victim	= NULL;
    obj		= NULL;
    vo		= NULL;
    target	= TARGET_NONE;

    switch ( skill_table[sn].target )
    {
    default:
	bug( "Do_cast: bad target for sn %d.", sn );
	return;

    case TAR_IGNORE:
	break;

    case TAR_CHAR_OFFENSIVE:
	if ( arg2[0] == '\0' )
	{
	    if ( ( victim = ch->fighting ) == NULL )
	    {
		send_to_char( "Cast the spell on whom?\n\r", ch );
		return;
	    }
	}
	else
	{
	    if ( ( victim = get_char_room( ch, target_name ) ) == NULL )
	    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
	    }
	}

	if ( !IS_NPC(ch) )
	{
	check_killer(ch,victim);
	}

        if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
	{
	    send_to_char( "You can't do that on your own master.\n\r",
		ch );
	    return;
	}

	vo = (void *) victim;
	target = TARGET_CHAR;
	break;

    case TAR_CHAR_DEFENSIVE:
	if ( arg2[0] == '\0' )
	{
	    victim = ch;
	}
	else
	{
	    if ( ( victim = get_char_room( ch, target_name ) ) == NULL )
	    {
		send_to_char( "They aren't here.\n\r", ch );
		return;
	    }
	}

	vo = (void *) victim;
	target = TARGET_CHAR;
	break;

    case TAR_CHAR_SELF:
	if ( arg2[0] != '\0' && !is_name( target_name, ch->name ) )
	{
	    send_to_char( "You cannot cast this spell on another.\n\r", ch );
	    return;
	}

	vo = (void *) ch;
	target = TARGET_CHAR;
	break;

    case TAR_OBJ_INV:
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "What should the spell be cast upon?\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_carry( ch, target_name, ch ) ) == NULL )
	{
	    send_to_char( "You are not carrying that.\n\r", ch );
	    return;
	}

	vo = (void *) obj;
	target = TARGET_OBJ;
	break;

    case TAR_OBJ_CHAR_OFF:
	if (arg2[0] == '\0')
	{
	    if ((victim = ch->fighting) == NULL)
	    {
		send_to_char("Cast the spell on whom or what?\n\r",ch);
		return;
	    }

	    target = TARGET_CHAR;
	}
	else if ((victim = get_char_room(ch,target_name)) != NULL)
	{
	    target = TARGET_CHAR;
	}

	if (target == TARGET_CHAR) /* check the sanity of the attack */
	{
            if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
            {
                send_to_char( "You can't do that on your own follower.\n\r",
                    ch );
                return;
            }

	    if (!IS_NPC(ch))
		check_killer(ch,victim);

	    vo = (void *) victim;
 	}
	else if ((obj = get_obj_here(ch,target_name)) != NULL)
	{
	    vo = (void *) obj;
	    target = TARGET_OBJ;
	}
	else
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
	break;

    case TAR_OBJ_CHAR_DEF:
        if (arg2[0] == '\0')
        {
            vo = (void *) ch;
            target = TARGET_CHAR;
        }
        else if ((victim = get_char_room(ch,target_name)) != NULL)
        {
            vo = (void *) victim;
            target = TARGET_CHAR;
	}
	else if ((obj = get_obj_carry(ch,target_name,ch)) != NULL)
	{
	    vo = (void *) obj;
	    target = TARGET_OBJ;
	}
	else
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
	break;
    }

    if ( !IS_NPC(ch) && ch->mana < mana )
    {
	send_to_char( "You don't have enough mana.\n\r", ch );
	return;
    }


    if ( str_cmp( skill_table[sn].name, "ventriloquate" ) )
	say_spell( ch, sn );


    WAIT_STATE( ch, skill_table[sn].beats );

    if ( number_percent( ) > get_skill(ch,sn) )
    {
		send_to_char( "You lost your concentration.\n\r", ch );
		check_improve(ch,sn,FALSE,1);
		if (get_skill(ch,skill_lookup("mental clarity")) > 75)
			ch->mana -= mana / 4;
		else
			ch->mana -= mana / 2;
    }
    else
    {
		if (get_skill(ch,skill_lookup("mental clarity")) > 75)
	        	ch->mana -= mana / 2;
		else
			ch->mana -= mana;
	if (IS_SET(ch->in_room->room_flags,ROOM_NO_MAGIC) && !(ch->level > LEVEL_HERO))
	{
	act("$n's spell fizzles.",ch,0,0,TO_ROOM);
	send_to_char("Your spell fizzles and dies.\n\r",ch);
	return;
	}
	if (eye_of_beholder_nullify(ch))
		return;
	if (skill_table[sn].target == TAR_CHAR_OFFENSIVE
	&& is_safe(ch,victim))
		return;
	if (skill_table[sn].target == TAR_CHAR_DEFENSIVE && is_affected(victim,gsn_timestop))
	{
	act("$N is stuck in time and is unaffected by your spell.",ch,0,victim,TO_CHAR);
	act("$N is untouched by $n's spell.",ch,0,victim,TO_NOTVICT);
	act("$n tries to cast something on you but you are stuck in time and unaffected.",ch,0,victim,TO_VICT);
	return;
	}

	if (skill_table[sn].target == TAR_CHAR_DEFENSIVE
	|| skill_table[sn].target == TAR_CHAR_OFFENSIVE)
		reflected = spellbaned(ch,victim,sn);

	if (skill_table[sn].target == TAR_CHAR_OFFENSIVE)
	{
	if (!IS_NPC(ch) && !IS_NPC(victim)
	&& (ch->fighting == NULL || victim->fighting == NULL) && str_cmp(skill_table[sn].name, "incinerate")!=1)
	{
		switch(number_range(0,2))
		{
			case (0):
			case (1):
			sprintf(buf,"Die, %s you sorcerous dog!",PERS(ch,victim));
			break;
			case (2):
			sprintf(buf,"Help! %s is casting a spell on me!",PERS(ch,victim));
		}

	if (victim != ch && !IS_NPC(ch))
		do_myell(victim,buf);

	}
	
        	if(check_counterspell(ch,victim))
        	{
                        act("$N reflects your spell right back at you!",ch,0,victim,TO_CHAR);
                        act("You reflect $n's spell right back at $m!",ch,0,victim,TO_VICT);
                        act("$N reflects $n's spell right back at $m!",ch,0,victim,TO_NOTVICT);
                        (*skill_table[sn].spell_fun) ( sn, ch->level*2, victim, ch, target);
                        return;
                }

			if (is_affected(victim,skill_lookup("body of diamond"))) {	
				if (number_percent() < 25) {
					act("Your spell is reflected off $N's diamond skin!",ch,0,victim,TO_CHAR);
	                act("Your diamond skin reflects $n's spell!",ch,0,victim,TO_VICT);
	                act("$N's diamond skin reflects $n's spell!",ch,0,victim,TO_NOTVICT);
					(*skill_table[sn].spell_fun) ( sn, ch->level/4, victim, ch, target);
				}
			}
				
	}

	absorbed = check_absorb(ch,victim,sn);

	if (reflected && skill_table[sn].target == TAR_CHAR_OFFENSIVE)
		vo = (void *) ch;
	
	OBJ_DATA *dagger;
	dagger = get_eq_char(ch, WEAR_WIELD);
	if (dagger != NULL && dagger->pIndexData->vnum == 27500)
		runeDagger = TRUE;

	if ((!absorbed) && (!reflected))
            (*skill_table[sn].spell_fun) ( sn, runeDagger ? ch->level * 1.4 + GET_HITROLL(ch)/20 : ch->level + GET_HITROLL(ch)/20, ch, vo,target);

        check_improve(ch,sn,TRUE,1);

    }

    if ((skill_table[sn].target == TAR_CHAR_OFFENSIVE
    ||   (skill_table[sn].target == TAR_OBJ_CHAR_OFF && target == TARGET_CHAR))
    &&   victim != ch
    &&   victim->master != ch)
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	for ( vch = ch->in_room->people; vch; vch = vch_next )
	{
	    vch_next = vch->next_in_room;
	    if ( victim == vch && victim->fighting == NULL )
	    {	check_killer(victim,ch);
		multi_hit( victim, ch, TYPE_UNDEFINED );
		break;
	    }
	}
    }

    return;
}

bool check_absorb(CHAR_DATA *ch,CHAR_DATA *victim,int dt)
{
    int mana;

    if (dt >= 0 && dt < MAX_SKILL)
    {
	if (skill_table[dt].target != TAR_CHAR_OFFENSIVE || !is_affected(victim,gsn_absorb))
	    return FALSE;

	if (skill_table[dt].ctype == CMD_COMMUNE 
	|| (skill_table[dt].ctype == CMD_BOTH && class_table[ch->class].ctype == CLASS_COMMUNER))
	    return FALSE;

	if (ch == victim) 
	    return FALSE;

    	if (!IS_NPC(ch))
    	{
    	    if (ch->level + 2 == skill_table[dt].skill_level[ch->class])
		mana = 50;
    	    else
    		mana = UMAX( skill_table[dt].min_mana, 100 / ( 2 + ch->level - skill_table[dt].skill_level[ch->class] ) );
    	}
    	else
	    mana = skill_table[dt].min_mana;

	if (number_percent() < ((victim->level*2) + (victim->pcdata->learned[gsn_absorb]) / 2) && (number_percent() > 40))
	{
		act("The air about $N shimmers as it absorbs $n's spell.",ch,NULL,victim,TO_NOTVICT);
		act("The air about you shimmers as it absorbs $n's spell.",ch,NULL,victim,TO_VICT);
		act("The air about $N shimmers as it absorbs your spell.",ch,NULL,victim,TO_CHAR);
		victim->mana += mana;
		send_to_char("You feel mana surge through you!\n\r",victim);
		return TRUE;
	}
    }
    return FALSE;
}

/*
 * Cast spells at targets using a magical object.
 */
void obj_cast_spell( int sn, int level, CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj )
{
    void *vo;
    int target = TARGET_NONE;

    if ( sn <= 0 )
	return;

    if ( sn >= MAX_SKILL || skill_table[sn].spell_fun == 0 )
    {
	bug( "Obj_cast_spell: bad sn %d.", sn );
	return;
    }

	if (IS_SET(ch->in_room->room_flags,ROOM_NO_MAGIC))
	{
	act("$n's spell fizzles.",ch,0,0,TO_ROOM);
	send_to_char("Your spell fizzles and dies.\n\r",ch);
	return;
	}


    switch ( skill_table[sn].target )
    {
    default:
	bug( "Obj_cast_spell: bad target for sn %d.", sn );
	return;

    case TAR_IGNORE:
	vo = NULL;
	break;

    case TAR_CHAR_OFFENSIVE:
	if ( victim == NULL )
	    victim = ch->fighting;
	if ( victim == NULL )
	{
	    send_to_char( "You can't do that.\n\r", ch );
	    return;
	}
	if (is_safe(ch,victim) && ch != victim)
	{
	    send_to_char("Something isn't right...\n\r",ch);
	    return;
	}
	vo = (void *) victim;
	target = TARGET_CHAR;
	break;

    case TAR_CHAR_DEFENSIVE:
    case TAR_CHAR_SELF:
	if ( victim == NULL )
	    victim = ch;
	vo = (void *) victim;
	target = TARGET_CHAR;
	break;

    case TAR_OBJ_INV:
	if ( obj == NULL )
	{
	    send_to_char( "You can't do that.\n\r", ch );
	    return;
	}
	vo = (void *) obj;
	target = TARGET_OBJ;
	break;

    case TAR_OBJ_CHAR_OFF:
        if ( victim == NULL && obj == NULL)
	{
	    if (ch->fighting != NULL)
		victim = ch->fighting;
	    else
	    {
		send_to_char("You can't do that.\n\r",ch);
		return;
	    }
	}

	    if (victim != NULL)
	    {
		if (is_safe_spell(ch,victim,FALSE) && ch != victim)
		{
		    send_to_char("Something isn't right...\n\r",ch);
		    return;
		}

		vo = (void *) victim;
		target = TARGET_CHAR;
	    }
	    else
	    {
	    	vo = (void *) obj;
	    	target = TARGET_OBJ;
	    }
        break;


    case TAR_OBJ_CHAR_DEF:
	if (victim == NULL && obj == NULL)
	{
	    vo = (void *) ch;
	    target = TARGET_CHAR;
	}
	else if (victim != NULL)
	{
	    vo = (void *) victim;
	    target = TARGET_CHAR;
	}
	else
	{
	    vo = (void *) obj;
	    target = TARGET_OBJ;
	}

	break;
    }

    target_name = "";
    (*skill_table[sn].spell_fun) ( sn, level, ch, vo,target);



    if ( (skill_table[sn].target == TAR_CHAR_OFFENSIVE
    ||   (skill_table[sn].target == TAR_OBJ_CHAR_OFF && target == TARGET_CHAR))
    &&   victim != ch
    &&   victim->master != ch )
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	for ( vch = ch->in_room->people; vch; vch = vch_next )
	{
	    vch_next = vch->next_in_room;
	    if ( victim == vch && victim->fighting == NULL )
	    {
		check_killer(victim,ch);
		multi_hit( victim, ch, TYPE_UNDEFINED );
		break;
	    }
	}
    }

    return;
}



/*
 * Spell functions.
 */
void spell_absorb( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (is_affected(victim,gsn_absorb) )
	{
	send_to_char("You are already absorbing spells.\n\r",victim);
	return;
	}

        init_affect(&af);
        af.aftype = AFT_SPELL;
    af.where 	= TO_AFFECTS;
    af.type 	= gsn_absorb;
    af.level 	= level;
    af.duration = 3 + level/5;
    af.modifier = 0;
    af.location = 0;
    af.bitvector = 0;
    af.affect_list_msg = str_dup("grants a chance to absorb spells");
    affect_to_char(victim, &af);
    send_to_char("A shimmering force field crackles around you.\n\r",victim);
    act("A sizzling force field shimmers into existence around $n.",victim,NULL,NULL,TO_ROOM);
    return;
}

void spell_acidneedles( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
    int num;

    if (check_spellcraft(ch,sn))
	dam = spellcraft_dam(level/3,5);
    else
    dam = dice( level/3, 5 );

	if (is_affected(victim,skill_lookup("plague"))) {
		AFFECT_DATA *plague = affect_find(victim->affected,skill_lookup("plague"));
		if (!plague) goto skip_acid_multiplier;
		
		if (plague->modifier <= -4) {
			act("You wince in agony as the acid needles penetrate your plague sores!",ch,0,victim,TO_VICT);
			act("$N winces in agony as the acid needles penetrate $S plague sores!",ch,0,victim,TO_NOTVICT);
			act("$N winces in agony as the acid needles penetrate $S plague sores!",ch,0,victim,TO_CHAR);
			dam += 5 + dice(-1 * plague->modifier,1);
			
			if (number_percent() < 30) {
				act("The plague worsens!",0,0,victim,TO_VICT);
				plague->modifier -= 1;
				
			}
		}
	}
	else if (is_affected(victim,skill_lookup("poison"))) {
		AFFECT_DATA *poison = affect_find(victim->affected,skill_lookup("poison"));
		if (!poison) goto skip_acid_multiplier;
		
		if (poison->modifier <= -4) {
			act("The acid needles exacerbate your poison as they sink into your flesh!",ch,0,victim,TO_VICT);
			act("$N goes slightly pale as the acid needles sink into $S flesh!",ch,0,victim,TO_NOTVICT);
			act("$N goes slightly pale as the acid needles sink into $S flesh!",ch,0,victim,TO_CHAR);
			dam += 8 + dice(-1 * poison->modifier,3);
			level += 2;
			
			if (number_percent() < 50) {
				act("The poison becomes more potent!",0,0,victim,TO_VICT);
				poison->modifier -= 1;
			}
		}
	}
	else if (is_affected(victim,skill_lookup("weaken"))) {
		AFFECT_DATA *weaken = affect_find(victim->affected,skill_lookup("weaken"));
		if (!weaken) goto skip_acid_multiplier;

		act("The acid needles sizzle and burn your weakened muscles as they pierce your flesh!",ch,0,victim,TO_VICT);
		act("The acid needles sizzle and burn $N's weakened muscles as they pierce $S flesh!",ch,0,victim,TO_NOTVICT);
		act("The acid needles sizzle and burn $N's weakened muscles as they pierce $S flesh!",ch,0,victim,TO_CHAR);
		dam += 12 + dice(-1 * weaken->modifier,5);
		level += 4;
		
		if (number_percent() < 75) {
			act("You feel weaker as the acid melts away your muscle tissue.",0,0,victim,TO_VICT);
			weaken->modifier -= 1;
		}
	}
	
	skip_acid_multiplier:

	if ( saves_spell( level, victim, DAM_ACID ) )
		dam = 2 * dam / 3;
		
    for (num = 0; num < 4; num++)
	{
	     if (number_percent() < 100 - num*5)
	     damage_old(ch,victim,dam,sn,DAM_ACID,TRUE);
	     dam = dam - dam/5;
	}
	WAIT_STATE(ch,PULSE_VIOLENCE*1);
	return;

}

void spell_acid_blast( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if (check_spellcraft(ch,sn))
	dam = spellcraft_dam(level,9);
    else
    dam = dice( level, 8.5 );

    if ( saves_spell( level, victim, DAM_ACID ) )
	dam /= 2;
    damage_old( ch, victim, dam, sn,DAM_ACID,TRUE);
    return;
}

void spell_armor( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You are already armored.\n\r",ch);
	else
	  act("$N is already armored.",ch,NULL,victim,TO_CHAR);
	return;
    }
	init_affect(&af);
    af.where	 = TO_AFFECTS;
	af.aftype	 = get_spell_aftype(ch);
    af.type      = sn;
    af.level	 = level;
    af.duration  = 24;
    af.modifier  = AC_PER_ONE_PERCENT_DECREASE_DAMAGE;
    af.location  = APPLY_AC;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( "You feel someone protecting you.\n\r", victim );
    if ( ch != victim )
	act("$N is protected by your magic.",ch,NULL,victim,TO_CHAR);
    return;
}



void spell_bless( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim;
    AFFECT_DATA af;

    if (target == TARGET_OBJ)
    {
        send_to_char("Uh, what are you thinking?\n\r", ch);
        return;
    }
    /* character target */
    victim = (CHAR_DATA *) vo;


    if ( victim->position == POS_FIGHTING || is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You are already blessed.\n\r",ch);
	else
	  act("$N already has divine favor.",ch,NULL,victim,TO_CHAR);
	return;
    }

        init_affect(&af);
        af.aftype = AFT_COMMUNE;
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = 6+level;
    af.location  = APPLY_HITROLL;
    if ( level < 8 )
       af.modifier = 1;
    else
       af.modifier  = level / 8;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_SAVING_SPELL;
    if ( level < 8 )
       af.modifier = -1;
    else
       af.modifier  = 0 - level / 8;
    affect_to_char( victim, &af );
    send_to_char( "You feel righteous.\n\r", victim );
    if ( ch != victim )
	act("You grant $N the favor of your god.",ch,NULL,victim,TO_CHAR);
    return;
}

void spell_blindness( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    int dur;

    if ( IS_AFFECTED(victim, AFF_BLIND)) {
		act("But $E is already blind!",ch,0,victim,TO_CHAR);
		return;
    }
	
	if (saves_spell(level,victim,DAM_OTHER) || (!(str_cmp(race_table[victim->race].name,"lich")))) {
		act("You failed to blind $N.",ch,0,victim,TO_CHAR);
		return;
	}
		

    init_affect(&af);
    af.where     	= TO_AFFECTS;
    af.aftype     	= get_spell_aftype(ch);
    af.type      	= sn;
    af.level     	= level;
    af.location  	= APPLY_HITROLL;
    af.modifier  	= -4;
    dur = level/3;
    if (dur > 9)
	dur = 9;
    af.duration  	= dur;
    af.bitvector 	= AFF_BLIND;

    affect_to_char( victim, &af );
    send_to_char( "You are blinded!\n\r", victim );
    act("$n appears to be blinded.",victim,NULL,NULL,TO_ROOM);
    return;
}

void spell_burning_hands(int sn,int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] =
    {
	 0,
	 0,  0,  0,  0,	14,	17, 20, 23, 26, 29,
	29, 29, 30, 30,	31,	31, 32, 32, 33, 33,
	34, 34, 35, 35,	36,	36, 37, 37, 38, 38,
	39, 39, 40, 40,	41,	41, 42, 42, 43, 43,
	44, 44, 45, 45,	46,	46, 47, 47, 48, 48
    };
    int dam;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( level, victim,DAM_FIRE) )
	dam /= 2;
    damage_old( ch, victim, dam, sn, DAM_FIRE,TRUE);
    return;
}



void spell_call_lightning( int sn, int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam;

    if ( !IS_OUTSIDE(ch) )
    {
	send_to_char( "You must be out of doors.\n\r", ch );
	return;
    }

    if ( weather_info.sky < SKY_RAINING )
    {
	send_to_char( "You need bad weather.\n\r", ch );
	return;
    }

    dam = dice(level/2, 8);

    send_to_char( "Lightning from above strikes your foes!\n\r", ch );
    act( "$n calls lightning from above to strike $s foes!", ch, NULL, NULL, TO_ROOM );

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch->in_room == NULL )
	    continue;

	if ( vch->in_room == ch->in_room )
	{
	    if ( vch != ch && ( IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch) ) )
		damage_old( ch, vch, saves_spell( level,vch,DAM_LIGHTNING) ? dam / 2 : dam, sn,DAM_LIGHTNING,TRUE);
	    continue;
	}

	if ( vch->in_room->area == ch->in_room->area
	&&   IS_OUTSIDE(vch)
	&&   IS_AWAKE(vch) )
	    send_to_char( "Lightning flashes in the sky.\n\r", vch );
    }

    return;
}

/* RT calm spell stops all fighting in the room */

void spell_calm( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *vch;
    int mlevel = 0;
    int count = 0;
    int high_level = 0;
    int chance;
    long vnum;
    AFFECT_DATA af;

    /* get sum of all mobile levels in the room */
    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
	if (vch->position == POS_FIGHTING)
	{
	    count++;
	/*    if (IS_NPC(vch))*/
	      mlevel += vch->level;
	/*    else
	      mlevel += vch->level/2;*/
	    high_level = UMAX(high_level,vch->level);
	}
    }

    /* compute chance of stopping combat */
    chance = 4 * level - high_level + 2 * count;

    if (IS_IMMORTAL(ch)) /* always works */
      mlevel = 0;

    if (number_range(0, chance) >= mlevel)  /* hard to stop large fights */
    {
	for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
   	{
	    if (IS_NPC(vch) && (IS_SET(vch->imm_flags,IMM_MAGIC) ||
				IS_SET(vch->act,ACT_UNDEAD)))
	     continue;

	    if (IS_AFFECTED(vch,AFF_CALM) || IS_AFFECTED(vch,AFF_BERSERK)
	    ||  is_affected(vch,skill_lookup("frenzy")))
	      continue;

	    if(IS_NPC(vch)) {
		vnum=vch->pIndexData->vnum;
		if(vnum==MOB_VNUM_RAGER||vnum==MOB_VNUM_ARCANA||
			vnum==MOB_VNUM_KNIGHT||vnum==MOB_VNUM_ANCIENT||
			vnum==MOB_VNUM_ENFORCER||vnum==MOB_VNUM_BOUNTY||
                  vnum==MOB_VNUM_OUTLAW)
		    continue;
	    }

            send_to_char("A wave of calmness passes over you.\n\r",vch);

	    if (vch->fighting || vch->position == POS_FIGHTING)
		vch->fighting=NULL;
	     stop_fighting(vch,FALSE);

		init_affect(&af);
	    af.where = TO_AFFECTS;
		af.aftype	 = get_spell_aftype(ch);
	    af.type = sn;
  	    af.level = level;
	    af.duration = level/4;
	    af.location = APPLY_HITROLL;

	    if(IS_NPC(vch)) {
	    af.modifier=-5;
	    } else {
     	    af.modifier=-1 * level/10;
	    af.duration=2;
	    }
	    af.bitvector = AFF_CALM;
	    affect_to_char(vch,&af);

	    af.location = APPLY_DAMROLL;
	    affect_to_char(vch,&af);

	}
    }
}

void spell_cancellation( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    bool found = FALSE;
    bool commune = FALSE;

    if (class_table[ch->class].ctype==CLASS_COMMUNER) commune = TRUE;

    if (victim != ch && victim->master != ch)
    {
	send_to_char("You can only cast cancel on followers.\n\r",ch);
	return;
    }

    level += 2;

    if ((!IS_NPC(ch) && IS_NPC(victim) &&
	 !(IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim) ) ||
        (IS_NPC(ch) && !IS_NPC(victim)) )
    {
	send_to_char("You failed, try dispel magic.\n\r",ch);
	return;
    }

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("armor"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("bless"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("blindness"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("calm"),commune))
	found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("change sex"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("charm person"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("chill"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("curse"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("detect evil"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("detect good"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("detect invis"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("detect magic"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("faerie fire"),commune))
	found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("fly"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("frenzy"),commune))
	found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("giant strength"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("haste"),commune))
	found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("infravision"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("invis"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("mass invis"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("pass door"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("protection"),commune))
        found = TRUE; 
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("sanctuary"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("shield"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("sleep"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("slow"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("stone skin"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("weaken"),commune))
        found = TRUE;

    if (found)
        send_to_char("Ok.\n\r",ch);
    else
        send_to_char("Spell failed.\n\r",ch);
}

void spell_cause_light( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    damage_old( ch, (CHAR_DATA *) vo, dice(1, 8) + level / 3, sn,DAM_HARM,TRUE);
    return;
}



void spell_cause_critical(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    damage_old( ch, (CHAR_DATA *) vo, dice(3, 8) + level - 6, sn,DAM_HARM,TRUE);
    return;
}



void spell_cause_serious(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    damage_old( ch, (CHAR_DATA *) vo, dice(2, 8) + level / 2, sn,DAM_HARM,TRUE);
    return;
}

void spell_chain_lightning(int sn,int level,CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    CHAR_DATA *tmp_vict,*last_vict,*next_vict;
    bool found;
    int dam;

    /* first strike */
    if (is_affected(ch,gsn_grounding))
	{
	send_to_char("The electricity fizzles at your feet.\n\r",ch);
	act("A lightning bolt leaps from $n's hand but fizzles into the ground.",ch,0,0,TO_ROOM);
	return;
	}

    act("A lightning bolt leaps from $n's hand and arcs to $N.",
        ch,NULL,victim,TO_ROOM);
    act("A lightning bolt leaps from your hand and arcs to $N.",
	ch,NULL,victim,TO_CHAR);
    act("A lightning bolt leaps from $n's hand and hits you!",
	ch,NULL,victim,TO_VICT);
	 WAIT_STATE(ch, PULSE_VIOLENCE * 2);

    dam = dice(level,6);
    if (saves_spell(level,victim,DAM_LIGHTNING))
 	dam /= 3;
    damage_old(ch,victim,dam,sn,DAM_LIGHTNING,TRUE);
    last_vict = victim;
    level -= 4;   /* decrement damage */

    /* new targets */
    while (level > 0)
    {
	found = FALSE;
	for (tmp_vict = ch->in_room->people;
	     tmp_vict != NULL;
	     tmp_vict = next_vict)
	{
	  next_vict = tmp_vict->next_in_room;
	  if (!is_safe_spell(ch,tmp_vict,TRUE) && tmp_vict != last_vict)
	  {
	    found = TRUE;
	    last_vict = tmp_vict;
	    if (last_vict == ch)
	    {
	     if (number_percent()>=ch->level)
	      {
	       act("The bolt arcs back to $n, and it stabilizes!",tmp_vict,NULL,NULL,TO_ROOM);
	       act("The bolt arcs back to you and you stabilize its energy!",tmp_vict,NULL,NULL,TO_ROOM);
	       level+=7;
	      }
	     else
	      {
	       act("The bolt arcs back to $n!",tmp_vict,NULL,NULL,TO_ROOM);
	       act("The bolt arcs back to you!",tmp_vict,NULL,NULL,TO_ROOM);
	      }
	    }
	    else
	    {
	    act("The bolt arcs to $n!",tmp_vict,NULL,NULL,TO_ROOM);
	    act("The bolt hits you!",tmp_vict,NULL,NULL,TO_CHAR);
	    dam = dice(level,6);
	    if (saves_spell(level,tmp_vict,DAM_LIGHTNING))
		dam /= 3;
	    damage_old(ch,tmp_vict,dam,sn,DAM_LIGHTNING,TRUE);
	    level -= 4;  /* decrement damage */
	    }
	  }
	}   /* end target searching loop */

	if (!found) /* no target found, hit the caster */
	{
	  if (ch == NULL)
     	    return;

	  if (last_vict == ch) /* no double hits */
	  {
	    act("The bolt seems to have fizzled out.",ch,NULL,NULL,TO_ROOM);
	    act("The bolt grounds out harmlessly through your body.",
		ch,NULL,NULL,TO_CHAR);
	    return;
	  }

	  last_vict = ch;
	  act("The bolt arcs back to $n!",ch,NULL,NULL,TO_ROOM);
	  send_to_char("The bolt arcs harmlessly back to you.\n\r",ch);
	  level -= 4;  /* decrement damage */
	  if (ch == NULL)
	    return;
	}
    /* now go back and find more targets */
    }
}


void spell_change_sex( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ))
    {
	if (victim == ch)
	  send_to_char("You've already been changed.\n\r",ch);
	else
	  act("$N has already had $s(?) sex changed.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (saves_spell(level , victim,DAM_OTHER))
	return;
	init_affect(&af);
    af.where     = TO_AFFECTS;
	af.aftype = AFT_SPELL;
    af.type      = sn;
    af.level     = level;
    af.duration  = 2 * level;
    af.location  = APPLY_SEX;
    do
    {
	af.modifier  = number_range( 0, 2 ) - victim->sex;
    }
    while ( af.modifier == 0 );
    af.bitvector = 0;
	af.affect_list_msg = str_dup("modifies anatomy");
    affect_to_char( victim, &af );
    send_to_char( "You feel different.\n\r", victim );
    act("$n doesn't look like $mself anymore...",victim,NULL,NULL,TO_ROOM);
    return;
}



void spell_charm_person( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    CHAR_DATA *check;
    int count;

     count = 0;

    if ((!IS_NPC(victim)) && (!IS_IMMORTAL(ch)))
    {
        send_to_char( "You can only charm mobiles.\n\r", ch );
        return;
    }

    if (is_safe(ch,victim)) 
	return;

    if ( victim == ch )
    {
	send_to_char( "You like yourself even better!\n\r", ch );
	return;
    }

    if (IS_NPC(victim) && (victim->pIndexData->pShop != NULL)) 
    {
	act("$N suddenly becomes very angry and destroys $n!",ch,0,victim,TO_ROOM);
	act("$N suddenly becomes very angry and destroys you!",ch,0,victim,TO_CHAR);
	raw_kill(victim,ch);
	return;
    }

    if ( IS_AFFECTED(victim, AFF_CHARM)
    ||   IS_AFFECTED(ch, AFF_CHARM)
    ||   level < victim->level
    ||   IS_CABAL_GUARD(victim)
    ||   IS_SET(victim->imm_flags,IMM_CHARM)
    ||   (IS_NPC(victim) && victim->spec_fun != NULL)
    ||   (IS_NPC(victim) && IS_SET(victim->act,ACT_AGGRESSIVE))) {
		act("$E completely ignores your charming spell.",ch,0,victim,TO_CHAR);
		return;
	}
	
	if ( saves_spell( level-3, victim,DAM_CHARM) )
	{
		act("$E resists your charming magic.",ch,0,victim,TO_CHAR);
		act("You resist $n's charming magic.",ch,0,victim,TO_VICT);
		act("$N resists $n's charming magic.",ch,0,victim,TO_NOTVICT);
		return;
	}
		

    for (check = char_list; check != NULL; check = check->next)
    {
	if (check->leader == ch && IS_AFFECTED(check,AFF_CHARM))
		count++;
    }

    if (count > 0)
    {
	send_to_char("You already control as many charmies as you can.\n\r",ch);
	return;
    }

    if (victim->level > ch->level)
    {
	send_to_char("They are too powerful for you to charm.\n\r", ch);
	return;
    }

    if ( victim->master )
	stop_follower( victim );

    add_follower( victim, ch );
    victim->leader = ch;

    init_affect(&af);
    af.where     = TO_AFFECTS;
    af.aftype 	 = AFT_SPELL;
    af.type      = sn;
    af.level	 = level;
    af.duration  = number_fuzzy( level / 4 );
    af.location  = 0;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
	char msg_buf[MSL];
	sprintf(msg_buf,"under %s's command", IS_NPC(ch) ? ch->short_descr : ch->name);
	af.affect_list_msg = str_dup(msg_buf);
    affect_to_char( victim, &af );
    act( "Isn't $n just so nice?", ch, NULL, victim, TO_VICT );
    if ( ch != victim )
	act("$N looks at you with adoring eyes.",ch,NULL,victim,TO_CHAR);
    return;
}

void spell_chill_touch( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] =
    {
	 0,
	 0,  0,  6,  7,  8,	 9, 12, 13, 13, 13,
	14, 14, 14, 15, 15,	15, 16, 16, 16, 17,
	17, 17, 18, 18, 18,	19, 19, 19, 20, 20,
	20, 21, 21, 21, 22,	22, 22, 23, 23, 23,
	24, 24, 24, 25, 25,	25, 26, 26, 26, 27
    };
    AFFECT_DATA af;
    int dam;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );
    dam *= 2;
    if (check_spellcraft(ch,sn))
	dam *= 2;

    if ( !saves_spell( level, victim,DAM_COLD ) )
    {
	act("$n turns blue and shivers.",victim,NULL,NULL,TO_ROOM);
	init_affect(&af);
	af.where     = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
	af.type      = sn;
        af.level     = level;
	af.duration  = 6;
	af.location  = APPLY_STR;
	af.modifier  = -1;
	af.bitvector = 0;
	affect_join( victim, &af );
    }
    else
    {
	dam /= 2;
    }

    damage_old( ch, victim, dam, sn, DAM_COLD,TRUE );
    return;
}


void spell_colour_spray( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] =
    {
	 0,
	 0,  0,  0,  0,  0,	 0,  0,  0,  0,  0,
	30, 35, 40, 45, 50,	55, 55, 55, 56, 57,
	58, 58, 59, 60, 61,	61, 62, 63, 64, 64,
	65, 66, 67, 67, 68,	69, 70, 70, 71, 72,
	73, 73, 74, 75, 76,	76, 77, 78, 79, 79
    };
    int dam;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2,  dam_each[level] * 2 );
    if ( saves_spell( level, victim,DAM_LIGHT) )
	dam /= 2;
    else
	spell_blindness(skill_lookup("blindness"),
	    level/2,ch,(void *) victim,TARGET_CHAR);

    damage_old( ch, victim, dam, sn, DAM_LIGHT,TRUE );
    return;
}


void spell_concatenate(int sn,int level,CHAR_DATA *ch,void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    act("$n points an arm at $N and releases a blast of pure energy.",ch,NULL,victim,TO_NOTVICT);
    act("$n gestures at you and a blast of white light engulfs you!",ch,NULL,victim,TO_VICT);
    act("You concatenate the magical energies around you and channel them at $N.",ch,NULL,victim,TO_CHAR);


    if (level <= 35)
    {
    if (check_spellcraft(ch,sn))
	dam = spellcraft_dam(level,11);
    else
	    dam = dice(level,11);
    }
    else if (level <= 40)
    {
    if (check_spellcraft(ch,sn))
	dam = spellcraft_dam(level,12);
    else
	dam = dice(level,12);
    }
    else if (level <= 48)
    {
    if (check_spellcraft(ch,sn))
	dam = spellcraft_dam(level,13);
    else
	dam = dice(level,13);
    }
    else
    {
    if (check_spellcraft(ch,sn))
	dam = spellcraft_dam(level, 13);
    else
	dam = dice(level,13);

	dam += number_range(0,level);
    }


    if (IS_NPC(victim))
    {
	if (level <= 35)
		dam += level;
	else if (level <=40)
		dam += (3*level);
	else
		dam += 4*level;
    }

    if (saves_spell(level,victim, DAM_ENERGY) );
	dam /= 2;

    damage_old( ch, victim, dam, sn, DAM_ENERGY, TRUE);
    if (number_bits(2) == 0)
    	spell_blindness(skill_lookup("blindness"), level*3/4, ch, (void *) victim, TARGET_CHAR);

    send_to_char("You stop channeling the energy and the beam of light dissipates.\n\r", ch);
    act("$n's beam of light dissipates.",ch,NULL,NULL,TO_ROOM);

    return;

}

/* Very messy group gate spell. I wanted players to 'feel' that their
entire group had transfered with them rather than just dumping them all in
the room and doing a look (then they'd just see whoever was transfered
before them but not following members).
-Ceran
*/
void spell_venueport(int sn, int level,CHAR_DATA *ch,void *vo, int target)
{
    CHAR_DATA *victim;
    CHAR_DATA *group;
    CHAR_DATA *arrival;
    CHAR_DATA *g_next;
    ROOM_INDEX_DATA *VENUEPORT;
    CHAR_DATA *last_to_venue;
    int numb;

    if (affect_find(ch->affected,gsn_insect_swarm) != NULL)
    {
        send_to_char( "You attempt to transport yourself to safety but loose your concentration as you are bitten by yet another insect.\n\r", ch);
        return;
    }

    if (cabal_down(ch,CABAL_ARCANA))
		return;
    last_to_venue = ch;
    victim = get_char_world( ch, target_name );
/*    send_to_char("You utter the words, 'tesseract'.\n\r",ch);
    act("$n utters the words, 'tesseract'.",ch,NULL,NULL,TO_ROOM); */
    if ( victim == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   !can_see_room(ch,victim->in_room)
    ||   (victim->in_room->cabal != 0 && victim->in_room->cabal != CABAL_ARCANA)
    ||   ch->in_room->guild != 0
    ||   victim->in_room->guild != 0
    ||   (IS_NPC(victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
    ||   victim->level >= level + 9
    ||   (saves_spell( level, victim,DAM_OTHER) )
    ||   (!IS_NPC(victim) && !can_pk(ch,victim) && IS_SET(victim->act,PLR_NOSUMMON)))
    {
        send_to_char( "You failed.\n\r", ch );
        return;
    }

    if (victim->in_room == ch->in_room)
    {
	send_to_char("You are already at that venue.\n\r",ch);
	return;
    }

    VENUEPORT = get_room_index(ROOM_VNUM_VENUEPORT);
    numb = 1;
    for (group = ch->in_room->people; group != NULL; group = g_next)
    {
	g_next = group->next_in_room;
	if (!is_same_group(group,ch) || (group->fighting != NULL) )
		continue;
	if (group == ch )
		continue;
	numb++;
	affect_strip(group,gsn_ambush);
	send_to_char("You feel space and time rupture around you as everything shifts.\n\r",group);
	send_to_char("With a blinding flash of light you find yourself somewhere else!\n\r",group);
   send_to_char("\n\r",group);
	char_from_room(group);
	char_to_room(group,victim->in_room);
        act("$n arrives suddenly!",group,NULL,NULL,TO_ROOM);
	char_from_room(group);
	char_to_room(group, VENUEPORT);
	last_to_venue = group;
	if (is_affected(group,gsn_earthfade))
	{
		affect_strip(group, gsn_earthfade);
	}
   }

   act("With a brilliant flash of light $n and $s friends disappear!",ch,NULL,NULL,TO_ROOM);
   send_to_char("You feel space and time rupture around you as everything shifts.\n\r",ch);
   send_to_char("With a blinding flash of light you find yourself somewhere else!\n\r",ch);
   send_to_char("\n\r",ch);
   char_from_room(ch);
   char_to_room(ch,victim->in_room);
   act("$n arrives suddenly!",ch,NULL,NULL,TO_ROOM);
   un_earthfade(ch,NULL);

   if (last_to_venue == ch)
    {
	do_look(ch,"auto");
	return;
    }

   for (arrival = last_to_venue->in_room->people; arrival != NULL; arrival = g_next)
   {
	g_next = arrival->next_in_room;
	if (arrival == ch)
		continue;
	if ((is_same_group(arrival,ch)) && (arrival != ch) )
	{
	char_from_room(arrival);
	char_to_room(arrival,ch->in_room);
	}
   }

   for (arrival = ch->in_room->people; arrival != NULL; arrival = arrival->next_in_room)
    {
	do_look(arrival,"auto");
	if (--numb == 0)
	break;
    }
return;
}

void spell_electric_storm(int sn, int level, CHAR_DATA *ch, void *vo, int
target)
{
    AFFECT_DATA af;
    ROOM_AFFECT_DATA raf;

    if ( is_affected( ch, sn ) )
    {
      send_to_char
        ("Your unable to call upon the storm again so soon.\n\r", ch);
      return;
    }

    if ( is_affected_room( ch->in_room, sn ))
    {
        send_to_char("The storm already obeys your will here.\n\r",ch);
        return;
    }

	if (number_range(1,100) > ch->pcdata->learned[sn])
	{
		send_to_char("You call to the storm but it does not heed your summons.\n\r",ch);
		WAIT_STATE(ch, PULSE_TICK / 2);
	}

	init_affect(&af);
    af.where     = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
    af.type      = sn;
    af.level     = ch->level;
    af.duration  = 48;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
	af.affect_list_msg = str_dup("unable to summon an electrical storm");
    af.bitvector = 0;
    affect_to_char( ch, &af );

	init_affect_room(&raf);
    raf.where     = TO_ROOM_AFFECTS;
	raf.aftype    = AFT_SPELL;
    raf.type      = sn;
    raf.level     = ch->level;
    raf.duration  = level / 10;
    raf.location  = APPLY_NONE;
    raf.modifier  = 0;
    raf.bitvector = AFF_ROOM_ELECTRIC_STORM;
    affect_to_room( ch->in_room, &raf );

    send_to_char("The storm heads your summons as a massive electric storm begins to brew overhead!\n\r", ch);
    act("$n raises their arms to the sky and a massive storm cloud begins to form overhead.",ch,NULL,NULL,TO_ROOM);

	WAIT_STATE(ch, PULSE_TICK);
}

void spell_wrath(int sn,int level,CHAR_DATA *ch,void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if (!IS_EVIL(victim))
    {
    	act("$N is unaffected by $n's heavenly wrath.",ch,NULL,victim,TO_NOTVICT);
	act("You are unaffected by $n's heavenly wrath.\n\r",ch,NULL,victim,TO_VICT);
	send_to_char("The Gods do not enhance your wrath and frown on your actions\n\r",ch);
	return;
    }

    dam = dice(level, 8);

    dam += dice(level,5);

    if (saves_spell(level,victim,DAM_HOLY) || saves_spell(level + 5, victim,DAM_HOLY))
	dam /= 2;

    act("You call down the wrath of god upon $N.",ch,0,victim,TO_CHAR);
    act("$n calls down the wrath of god upon $N.",ch,0,victim,TO_NOTVICT);
    act("$n calls down the wrath of god upon you.",ch,0,victim,TO_VICT);
    damage_old(ch,victim,dam,sn,DAM_HOLY,TRUE);

    if (number_range(0,3) != 0)
	return;
    spell_curse(gsn_curse,level,ch,(void *) victim, TARGET_CHAR);
    return;
}


void spell_continual_light(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    OBJ_DATA *light;

    if (target_name[0] != '\0')  /* do a glow on some object */
    {
	light = get_obj_carry(ch,target_name,ch);

	if (light == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}

	if (IS_OBJ_STAT(light,ITEM_GLOW))
	{
	    act("$p is already glowing.",ch,light,NULL,TO_CHAR);
	    return;
	}

	set_bit(&light->extra_flags,ITEM_GLOW);
	act("$p glows with a white light.",ch,light,NULL,TO_ALL);
	return;
    }

    light = create_object( get_obj_index( OBJ_VNUM_LIGHT_BALL ), 0 );
    obj_to_room( light, ch->in_room );
    act( "$n twiddles $s thumbs and $p appears.",   ch, light, NULL, TO_ROOM );
    act( "You twiddle your thumbs and $p appears.", ch, light, NULL, TO_CHAR );
    return;
}



void spell_control_weather(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    if ( !str_cmp( target_name, "better" ) )
	weather_info.change += dice( level / 3, 4 );
    else if ( !str_cmp( target_name, "worse" ) )
	weather_info.change -= dice( level / 3, 4 );
    else
	send_to_char ("Do you want it to get better or worse?\n\r", ch );

    send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_create_food( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *mushroom;

    mushroom = create_object( get_obj_index( OBJ_VNUM_MUSHROOM ), 0 );
    mushroom->value[0] = level / 2;
    mushroom->value[1] = level;
    obj_to_room( mushroom, ch->in_room );
    act( "$p suddenly appears.", ch, mushroom, NULL, TO_ROOM );
    act( "$p suddenly appears.", ch, mushroom, NULL, TO_CHAR );
    return;
}

void spell_create_rose( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    OBJ_DATA *rose;
    rose = create_object(get_obj_index(OBJ_VNUM_ROSE), 0);
    act("$n has created a beautiful red rose.",ch,rose,NULL,TO_ROOM);
    send_to_char("You create a beautiful red rose.\n\r",ch);
    obj_to_char(rose,ch);
    return;
}

void spell_create_spring(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    OBJ_DATA *spring;

    spring = create_object( get_obj_index( OBJ_VNUM_SPRING ), 0 );
    spring->timer = level;
    obj_to_room( spring, ch->in_room );
    act( "$p flows from the ground.", ch, spring, NULL, TO_ROOM );
    act( "$p flows from the ground.", ch, spring, NULL, TO_CHAR );
    return;
}



void spell_create_water( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    int water;

    if ( obj->item_type != ITEM_DRINK_CON )
    {
	send_to_char( "It is unable to hold water.\n\r", ch );
	return;
    }

    if ( obj->value[2] != LIQ_WATER && obj->value[1] != 0 )
    {
	send_to_char( "It contains some other liquid.\n\r", ch );
	return;
    }

    water = UMIN(
		level * (weather_info.sky >= SKY_RAINING ? 4 : 2),
		obj->value[0] - obj->value[1]
		);

    if ( water > 0 )
    {
	obj->value[2] = LIQ_WATER;
	obj->value[1] += water;
	if ( !is_name( "water", obj->name ) )
	{
	    char buf[MAX_STRING_LENGTH];

	    sprintf( buf, "%s water", obj->name );
	    free_string( obj->name );
	    obj->name = str_dup( buf );
	}
	act( "$p is filled.", ch, obj, NULL, TO_CHAR );
    }

    return;
}



void spell_cure_blindness(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( !is_affected( victim, gsn_blindness ))
    {
        if (victim == ch)
          send_to_char("You aren't blind.\n\r",ch);
        else
          act("$N doesn't appear to be blinded.",ch,NULL,victim,TO_CHAR);
        return;
    }

    if (check_dispel(level,victim,gsn_blindness))
    {
        send_to_char( "Your vision returns!\n\r", victim );
        act("$n is no longer blinded.",victim,NULL,NULL,TO_ROOM);
    }
    else
        send_to_char("Spell failed.\n\r",ch);
}



void spell_cure_critical( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int heal;

    heal = dice(3, 8) + level - 6;
    victim->hit = UMIN( victim->hit + heal, victim->max_hit );
    update_pos( victim );
    send_to_char( "You feel better!\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}

/* RT added to cure plague */
void spell_cure_disease( int sn, int level, CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if (!str_cmp(class_table[ch->class].name, "monk")
	&& victim != ch)
    {
send_to_char("You are only able to purge your own diseases.\n\r",ch);
	return;
    }

    if ( !is_affected( victim, gsn_plague ) )
    {
        if (victim == ch)
          send_to_char("You aren't ill.\n\r",ch);
        else
          act("$N doesn't appear to be diseased.",ch,NULL,victim,TO_CHAR);
        return;
    }

    if (check_dispel(level,victim,gsn_plague))
    {
	send_to_char("Your sores vanish.\n\r",victim);
	act("$n looks relieved as $s sores vanish.",victim,NULL,NULL,TO_ROOM);
    }
    else
	send_to_char("Spell failed.\n\r",ch);
}



void spell_cure_light( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int heal;

    heal = dice(1, 8) + level / 3;
    victim->hit = UMIN( victim->hit + heal, victim->max_hit );
    update_pos( victim );
    send_to_char( "You feel better!\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_cure_poison( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;


    if (!str_cmp(class_table[ch->class].name, "monk")
	&& victim != ch)
    {
send_to_char("You are only able to purge toxins from your own body.\n\r",ch);
	return;
    }

    if ( !is_affected( victim, gsn_poison )
	&& !is_affected(victim, gsn_poison_dust))
    {
        if (victim == ch)
          send_to_char("You aren't poisoned.\n\r",ch);
        else
          act("$N doesn't appear to be poisoned.",ch,NULL,victim,TO_CHAR);
        return;
    }

    if (is_affected(victim,gsn_poison_dust))
	{
	if (check_dispel(level,victim,gsn_poison_dust))
	send_to_char("A warm feelings runs through you as the poison dust vanishes.\n\r",victim);
	act("$n looks much better.",victim,0,0,TO_ROOM);
	if (!is_affected(victim,gsn_poison))
		return;
	}

    if (check_dispel(level,victim,gsn_poison))
    {
        send_to_char("A warm feeling runs through your body.\n\r",victim);
        act("$n looks much better.",victim,NULL,NULL,TO_ROOM);
    }
    else
        send_to_char("Spell failed.\n\r",ch);
}

void spell_cure_serious( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int heal;

    heal = dice(2, 8) + level /2 ;
    victim->hit = UMIN( victim->hit + heal, victim->max_hit );
    update_pos( victim );
    send_to_char( "You feel better!\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_curse( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    AFFECT_DATA af;


    /* deal with the object case first */
    if (target == TARGET_OBJ)
    {
        obj = (OBJ_DATA *) vo;

	if (obj==NULL) return;

        if (IS_OBJ_STAT(obj,ITEM_EVIL))
        {
            act("$p is already filled with evil.",ch,obj,NULL,TO_CHAR);
            return;
        }

        if (IS_OBJ_STAT(obj,ITEM_BLESS))
        {
            AFFECT_DATA *paf;

            paf = affect_find(obj->affected,skill_lookup("bless"));
            if (!saves_dispel(level,paf != NULL ? paf->level : obj->level,0))
            {
                if (paf != NULL)
                    affect_remove_obj(obj,paf);
                act("$p glows with a red aura.",ch,obj,NULL,TO_ALL);
                remove_bit(&obj->extra_flags,ITEM_BLESS);
                return;
            }
            else
            {
                act("The holy aura of $p is too powerful for you to overcome.",
                    ch,obj,NULL,TO_CHAR);
                return;
            }
        }

	init_affect(&af);
        af.where        = TO_OBJECT;
	af.aftype	= get_spell_aftype(ch);
        af.type         = sn;
        af.level        = level;
        af.duration     = 2 * level;
        af.location     = APPLY_SAVES;
        af.modifier     = +5;
        af.bitvector    = ITEM_EVIL;
        affect_to_obj(obj,&af);

        act("$p glows with a malevolent aura.",ch,obj,NULL,TO_ALL);

	if (obj->wear_loc != WEAR_NONE)
	    ch->saving_throw += 1;
        return;
    }

    /* character curses */
    victim = (CHAR_DATA *) vo;
    if ((obj = get_eq_char(victim,WEAR_BODY)) != NULL
    && obj->pIndexData->vnum == 14005)
            level -= 20;

    if (IS_AFFECTED(victim,AFF_CURSE))
	{
		send_to_char("You are already cursed.\n\r", victim );
		if( ch != victim )
			act("$N is already cursed.",ch,NULL,victim,TO_CHAR );
		return;
	}

    if (saves_spell(level,victim,DAM_NEGATIVE))
    	{
		if( ch == victim )
			act("You resist your curse.",ch,NULL,0,TO_CHAR );

		if( ch != victim )
		{
			act("$N resists your curse.",ch,NULL,victim,TO_CHAR );
			act("You resist $n's curse.",ch,NULL,victim,TO_VICT );
			act("$N resists $n's curse.",ch,NULL,victim,TO_ROOM );
		}
		return;
	}
		
	init_affect(&af);
    af.where     = TO_AFFECTS;
	af.aftype	 = get_spell_aftype(ch);
    af.type      = sn;
    af.level     = level;
    af.duration  = ( 2 + level/5);
    af.location  = APPLY_HITROLL;
    af.modifier  = -1 * (level / 8);
    af.bitvector = AFF_CURSE;
    affect_to_char( victim, &af );

    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = level / 8;
    affect_to_char( victim, &af );
	af.affect_list_msg = str_dup("unable to recall");
	af.location = APPLY_NONE;
	af.modifier	= 0;
	affect_to_char( victim, &af );

    send_to_char( "You feel unclean.\n\r", victim );
    if ( ch != victim )
	act("$N looks very uncomfortable.",ch,NULL,victim,TO_CHAR);
    return;
}


void spell_dark_wrath(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if (!IS_GOOD(victim) )
    {
	act("$N is unaffected by $n's dark wrath.", ch, NULL,victim,TO_NOTVICT);
	act("$N is unaffected by your dark wrath.",ch,NULL,victim,TO_CHAR);
	act("You are unaffected by $n's dark wrath.",ch,NULL,victim,TO_VICT);
	return;
    }

    dam = dice(level,9);
    if (saves_spell(level,victim,DAM_NEGATIVE) )
	dam /= 2;
    damage_old(ch,victim,dam,sn, DAM_NEGATIVE, TRUE);

    if (number_range(0,3) != 0)
	return;
    spell_curse(gsn_curse, level, ch, (void *) victim, TARGET_CHAR);
     return;
}

/* RT replacement demonfire spell */

void spell_demonfire(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
/* Retarded alignment based stuff removed in favor of plain damage spell */ 

    dam = dice(level, 7);
    if (victim != ch)
    {
	act("$n calls forth the demons of Hell upon $N!",
	    ch,NULL,victim,TO_ROOM);
        act("$n has assailed you with the demons of Hell!",
	    ch,NULL,victim,TO_VICT);
	send_to_char("You conjure forth the demons of hell!\n\r",ch);
    }

    if ( saves_spell( level, victim,DAM_NEGATIVE) )
        dam /= 2;
    damage_old( ch, victim, dam, sn, DAM_NEGATIVE ,TRUE);
    spell_curse(gsn_curse, 3 * level / 4, ch, (void *) victim,TARGET_CHAR);
}

void spell_detect_evil( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_DETECT_EVIL) )
    {
	if (victim == ch)
	  send_to_char("You can already sense evil.\n\r",ch);
	else
	  act("$N can already detect evil.",ch,NULL,victim,TO_CHAR);
	return;
    }
        init_affect(&af);
        af.aftype = AFT_COMMUNE;
    af.where     = TO_AFFECTS;
    af.type      = sn;
    af.level	 = level;
    af.duration  = level;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = AFF_DETECT_EVIL;
    af.affect_list_msg = str_dup("allows you to distinguish evil hearts");
    affect_to_char( victim, &af );
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}


void spell_detect_good( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_DETECT_GOOD) )
    {
        if (victim == ch)
          send_to_char("You can already sense good.\n\r",ch);
        else
          act("$N can already detect good.",ch,NULL,victim,TO_CHAR);
        return;
    }
	init_affect(&af);
    af.where     = TO_AFFECTS;
	af.aftype	 = get_spell_aftype(ch);
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = AFF_DETECT_GOOD;
    af.affect_list_msg = str_dup("allows you to distinguish light-hearts");
    affect_to_char( victim, &af );
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
        send_to_char( "Ok.\n\r", ch );
    return;
}



void do_detect_hidden(CHAR_DATA *ch, char *argument)
{
    AFFECT_DATA af;
    int number;

    if ( IS_AFFECTED(ch, AFF_DETECT_HIDDEN) )
    {
        send_to_char("You are already as alert as you can be. \n\r",ch);
        return;
    }

    if ( (number = get_skill(ch,gsn_detect_hidden)) == 0 ||
     ch->level < skill_table[gsn_detect_hidden].skill_level[ch->class] )
    {
	send_to_char("Huh?\n\r", ch);
	return;
    }
    if ( (number = number_percent()) > get_skill(ch, gsn_detect_hidden) )
    {
	send_to_char("You peer into the shadows but your vision stays the same.\n\r", ch);
	check_improve(ch,gsn_detect_hidden,FALSE, 2);
	return;
    }

	init_affect(&af);
    af.where     = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
    af.type      = gsn_detect_hidden;
    af.level     = ch->level;
    af.duration  = ch->level;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_DETECT_HIDDEN;
    af.affect_list_msg = str_dup("grants the ability to detect the hidden");
    affect_to_char( ch, &af );
    send_to_char( "Your awareness improves.\n\r", ch );
    check_improve( ch, gsn_detect_hidden, TRUE, 2 );
    return;
}

void spell_detect_invis( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_DETECT_INVIS) )
    {
        if (victim == ch)
          send_to_char("You can already see invisible.\n\r",ch);
        else
          act("$N can already see invisible things.",ch,NULL,victim,TO_CHAR);
        return;
    }

	init_affect(&af);
    af.where     = TO_AFFECTS;
	af.aftype	 = get_spell_aftype(ch);
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = AFF_DETECT_INVIS;
    af.affect_list_msg = str_dup("grants the ability to detect the invisible");
    affect_to_char( victim, &af );
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_detect_magic( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_DETECT_MAGIC) )
    {
        if (victim == ch)
          send_to_char("You can already sense magical auras.\n\r",ch);
        else
          act("$N can already detect magic.",ch,NULL,victim,TO_CHAR);
        return;
    }

	init_affect(&af);
    af.where     = TO_AFFECTS;
	af.aftype	 = get_spell_aftype(ch);
    af.type      = sn;
    af.level	 = level;
    af.duration  = level;
    af.modifier  = 0;
    af.location  = APPLY_NONE;
    af.bitvector = AFF_DETECT_MAGIC;
    af.affect_list_msg = str_dup("grants the ability to detect magical auras");
    affect_to_char( victim, &af );
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_detect_poison( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;

    if ( obj->item_type == ITEM_DRINK_CON || obj->item_type == ITEM_FOOD )
    {
	if ( obj->value[3] != 0 )
	    send_to_char( "You smell poisonous fumes.\n\r", ch );
	else
	    send_to_char( "It looks delicious.\n\r", ch );
    }
    else
    {
	send_to_char( "It doesn't look poisoned.\n\r", ch );
    }

    return;
}


void spell_dispel_evil( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if ( !IS_NPC(ch) && IS_EVIL(ch) )
	victim = ch;

    if ( IS_GOOD(victim) )
    {
	act( "$N is protected.", ch, NULL, victim, TO_ROOM );
	return;
    }

    if ( IS_NEUTRAL(victim) )
    {
	act( "$N does not seem to be affected.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if (victim->hit > (ch->level * 4))
      dam = dice( level, 4 );
    else
      dam = UMAX(victim->hit, dice(level,4));
    if ( saves_spell( level, victim,DAM_HOLY) )
	dam /= 2;
    damage_old( ch, victim, dam, sn, DAM_HOLY ,TRUE);
    return;
}


void spell_dispel_good( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if ( !IS_NPC(ch) && IS_GOOD(ch) )
        victim = ch;

    if ( IS_EVIL(victim) )
    {
        act( "$N is protected by $S evil.", ch, NULL, victim, TO_ROOM );
        return;
    }

    if ( IS_NEUTRAL(victim) )
    {
        act( "$N does not seem to be affected.", ch, NULL, victim, TO_CHAR );
        return;
    }

    if (victim->hit > (ch->level * 4))
      dam = dice( level, 4 );
    else
      dam = UMAX(victim->hit, dice(level,4));
    if ( saves_spell( level, victim,DAM_NEGATIVE) )
        dam /= 2;
    damage_old( ch, victim, dam, sn, DAM_NEGATIVE ,TRUE);
    return;
}

void spell_earthquake( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
	char buf[MAX_STRING_LENGTH];

    send_to_char( "The earth trembles beneath your feet!\n\r", ch );
    act( "$n makes the earth tremble and shiver.", ch, NULL, NULL, TO_ROOM );

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch->in_room == NULL )
	    continue;
	if ( vch->in_room == ch->in_room )
	{
	    if ( vch != ch && !is_safe_spell(ch,vch,TRUE))
		{
	if (!IS_NPC(ch) && !IS_NPC(vch)
	&& (ch->fighting == NULL || vch->fighting == NULL))
	{
		switch(number_range(0,2))
		{
		case (0):
		case (1):
		sprintf(buf,"Die, %s you sorcerous dog!",PERS(ch,vch));
		break;
		case (2):
		sprintf(buf,"Help! %s is casting a spell on me!",PERS(ch,vch));
		}
	if (vch != ch)
		do_myell(vch,buf);
	}

		if (IS_AFFECTED(vch,AFF_FLYING))
		    damage_old(ch,vch,0,sn,DAM_EARTH,TRUE);
		else
		    damage_old( ch,vch,level + dice(2, 8), sn, DAM_EARTH,TRUE);
	}
	    continue;
	}

	if ( vch->in_room->area == ch->in_room->area )
	    send_to_char( "The earth trembles and shivers.\n\r", vch );
    }

    return;
}

void spell_enchant_armor( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf;
    int result, fail;
    int ac_bonus, added;
    bool ac_found = FALSE;

    if (obj->item_type != ITEM_ARMOR)
    {
	send_to_char("That isn't an armor.\n\r",ch);
	return;
    }

    if (obj->wear_loc != -1)
    {
	send_to_char("The item must be carried to be enchanted.\n\r",ch);
	return;
    }

    /* this means they have no bonus */
    ac_bonus = 0;
    fail = 25;	/* base 25% chance of failure */

    /* find the bonuses */

    if (!obj->enchanted)
    	for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
    	{
            if ( paf->location == APPLY_AC )
            {
	    	ac_bonus = paf->modifier;
		ac_found = TRUE;
	    	fail += 5 * (ac_bonus * ac_bonus);
 	    }

	    else  /* things get a little harder */
	    	fail += 20;
    	}

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location == APPLY_AC )
  	{
	    ac_bonus = paf->modifier;
	    ac_found = TRUE;
	    fail += 5 * (ac_bonus * ac_bonus);
	}

	else /* things get a little harder */
	    fail += 20;
    }

    /* apply other modifiers */
    fail -= level;

    if (IS_OBJ_STAT(obj,ITEM_BLESS))
	fail -= 15;
    if (IS_OBJ_STAT(obj,ITEM_GLOW))
	fail -= 5;

    fail = URANGE(5,fail,85);

    result = number_percent();

    /* the moment of truth */
    if (result < (fail / 5))  /* item destroyed */
    {
	act("$p flares blindingly... and evaporates!",ch,obj,NULL,TO_CHAR);
	act("$p flares blindingly... and evaporates!",ch,obj,NULL,TO_ROOM);
	extract_obj(obj);
	return;
    }

    if (result < (fail / 3)) /* item disenchanted */
    {
	AFFECT_DATA *paf_next;

	act("$p glows brightly, then fades...oops.",ch,obj,NULL,TO_CHAR);
	act("$p glows brightly, then fades.",ch,obj,NULL,TO_ROOM);
	obj->enchanted = TRUE;

	/* remove all affects */
	for (paf = obj->affected; paf != NULL; paf = paf_next)
	{
	    paf_next = paf->next;
	    free_affect(paf);
	}
	obj->affected = NULL;

	remove_bit(&obj->extra_flags, ITEM_GLOW);
	remove_bit(&obj->extra_flags, ITEM_HUM);
	remove_bit(&obj->extra_flags, ITEM_MAGIC);
	remove_bit(&obj->extra_flags, ITEM_INVIS);
	remove_bit(&obj->extra_flags, ITEM_NODROP);
	remove_bit(&obj->extra_flags, ITEM_NOREMOVE);
	return;
    }

    if ( result <= fail )  /* failed, no bad result */
    {
	send_to_char("Nothing seemed to happen.\n\r",ch);
	return;
    }

    /* okay, move all the old flags into new vectors if we have to */
    if (!obj->enchanted)
    {
	AFFECT_DATA *af_new;
	obj->enchanted = TRUE;

	for (paf = obj->pIndexData->affected; paf != NULL; paf = paf->next)
	{
	    af_new = new_affect();

	    af_new->next = obj->affected;
	    obj->affected = af_new;

	    af_new->where	= paf->where;
	    af_new->type 	= UMAX(0,paf->type);
	    af_new->level	= paf->level;
	    af_new->duration	= paf->duration;
	    af_new->location	= paf->location;
	    af_new->modifier	= paf->modifier;
	    af_new->bitvector	= paf->bitvector;
	}
    }

    if (result <= (90 - level/5))  /* success! */
    {
	act("$p shimmers with a gold aura.",ch,obj,NULL,TO_CHAR);
	act("$p shimmers with a gold aura.",ch,obj,NULL,TO_ROOM);
	set_bit(&obj->extra_flags, ITEM_MAGIC);
	added = -1;
    }

    else  /* exceptional enchant */
    {
	act("$p glows a brillant gold!",ch,obj,NULL,TO_CHAR);
	act("$p glows a brillant gold!",ch,obj,NULL,TO_ROOM);
	set_bit(&obj->extra_flags,ITEM_MAGIC);
	added = AC_PER_ONE_PERCENT_DECREASE_DAMAGE/3;
    }

    if ( (result = number_percent()) <= 25 && !is_set(&obj->extra_flags,ITEM_HUM) )
    {
	set_bit(&obj->extra_flags,ITEM_HUM);
	remove_bit(&obj->extra_flags,ITEM_GLOW);
    }
    if ( (result = number_percent()) <= 25 && !is_set(&obj->extra_flags,ITEM_GLOW) )
	set_bit(&obj->extra_flags,ITEM_GLOW);

    /* now add the enchantments */

    if (obj->level < LEVEL_HERO)
	obj->level = UMIN(LEVEL_HERO - 1,obj->level + 1);

    if (ac_found)
    {
	for ( paf = obj->affected; paf != NULL; paf = paf->next)
	{
	    if ( paf->location == APPLY_AC)
	    {
		paf->type = sn;
		paf->modifier += added;
		paf->level = UMAX(paf->level,level);
	    }
	}
    }
    else /* add a new affect */
    {
 	paf = new_affect();

	paf->where	= TO_OBJECT;
	paf->type	= sn;
	paf->level	= level;
	paf->duration	= -1;
	paf->location	= APPLY_AC;
	paf->modifier	=  added;
	paf->bitvector  = 0;
    	paf->next	= obj->affected;
    	obj->affected	= paf;
    }

}




void spell_enchant_weapon(int sn,int level,CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf;
    int result, fail;
    int hit_bonus, dam_bonus, added;
    bool hit_found = FALSE, dam_found = FALSE;

    if (obj->item_type != ITEM_WEAPON)
    {
	send_to_char("That isn't a weapon.\n\r",ch);
	return;
    }

    if (obj->wear_loc != -1)
    {
	send_to_char("The item must be carried to be enchanted.\n\r",ch);
	return;
    }

	if (is_affected_obj(obj,skill_lookup("unholy bless")))
		return send_to_char("The unholy aura surrounding that prevents you.\n\r",ch);
    /* this means they have no bonus */
    hit_bonus = 0;
    dam_bonus = 0;
    fail = 25;	/* base 25% chance of failure */

    /* find the bonuses */

    if (!obj->enchanted)
    	for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
    	{
            if ( paf->location == APPLY_HITROLL )
            {
	    	hit_bonus = paf->modifier;
		hit_found = TRUE;
	    	fail += 2 * (hit_bonus * hit_bonus);
 	    }

	    else if (paf->location == APPLY_DAMROLL )
	    {
	    	dam_bonus = paf->modifier;
		dam_found = TRUE;
	    	fail += 2 * (dam_bonus * dam_bonus);
	    }

	    else  /* things get a little harder */
	    	fail += 25;
    	}

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location == APPLY_HITROLL )
  	{
	    hit_bonus = paf->modifier;
	    hit_found = TRUE;
	    fail += 2 * (hit_bonus * hit_bonus);
	}

	else if (paf->location == APPLY_DAMROLL )
  	{
	    dam_bonus = paf->modifier;
	    dam_found = TRUE;
	    fail += 2 * (dam_bonus * dam_bonus);
	}

	else /* things get a little harder */
	    fail += 25;
    }

    /* apply other modifiers */
    fail -= 3 * level/2;

    if (IS_OBJ_STAT(obj,ITEM_BLESS))
	fail -= 15;
    if (IS_OBJ_STAT(obj,ITEM_GLOW))
	fail -= 5;

    fail = URANGE(5,fail,95);

    result = number_percent();

    /* the moment of truth */
    if (result < (fail / 5))  /* item destroyed */
    {
	act("$p shivers violently and explodes!",ch,obj,NULL,TO_CHAR);
	act("$p shivers violently and explodeds!",ch,obj,NULL,TO_ROOM);
	extract_obj(obj);
	return;
    }

    if (result < (fail / 2)) /* item disenchanted */
    {
	AFFECT_DATA *paf_next;

	act("$p glows brightly, then fades...oops.",ch,obj,NULL,TO_CHAR);
	act("$p glows brightly, then fades.",ch,obj,NULL,TO_ROOM);
	obj->enchanted = TRUE;

	/* remove all affects */
	for (paf = obj->affected; paf != NULL; paf = paf_next)
	{
	    paf_next = paf->next;
	    free_affect(paf);
	}
	obj->affected = NULL;

        remove_bit(&obj->extra_flags, ITEM_GLOW);
        remove_bit(&obj->extra_flags, ITEM_HUM);
        remove_bit(&obj->extra_flags, ITEM_MAGIC);
        remove_bit(&obj->extra_flags, ITEM_INVIS);
        remove_bit(&obj->extra_flags, ITEM_NODROP);
        remove_bit(&obj->extra_flags, ITEM_NOREMOVE);
	return;
    }

    if ( result <= fail )  /* failed, no bad result */
    {
	send_to_char("Nothing seemed to happen.\n\r",ch);
	return;
    }

    /* okay, move all the old flags into new vectors if we have to */
    if (!obj->enchanted)
    {
	AFFECT_DATA *af_new;
	obj->enchanted = TRUE;

	for (paf = obj->pIndexData->affected; paf != NULL; paf = paf->next)
	{
	    af_new = new_affect();

	    af_new->next = obj->affected;
	    obj->affected = af_new;

	    af_new->where	= paf->where;
	    af_new->type 	= UMAX(0,paf->type);
	    af_new->level	= paf->level;
	    af_new->duration	= paf->duration;
	    af_new->location	= paf->location;
	    af_new->modifier	= paf->modifier;
	    af_new->bitvector	= paf->bitvector;
	}
    }

    if (result <= (100 - level/5))  /* success! */
    {
	act("$p glows blue.",ch,obj,NULL,TO_CHAR);
	act("$p glows blue.",ch,obj,NULL,TO_ROOM);
	set_bit(&obj->extra_flags, ITEM_MAGIC);
	added = 1;
    }

    else  /* exceptional enchant */
    {
	act("$p glows a brillant blue!",ch,obj,NULL,TO_CHAR);
	act("$p glows a brillant blue!",ch,obj,NULL,TO_ROOM);
	set_bit(&obj->extra_flags,ITEM_MAGIC);
	added = 2;
    }

    if ( (result = number_percent()) <= 25 && !is_set(&obj->extra_flags,ITEM_HUM) )
    {
	set_bit(&obj->extra_flags,ITEM_HUM);
	remove_bit(&obj->extra_flags,ITEM_GLOW);
    }
    if ( (result = number_percent()) <= 25 && !is_set(&obj->extra_flags,ITEM_GLOW) )
	set_bit(&obj->extra_flags,ITEM_GLOW);

    if (level >= 100)
	set_bit(&obj->extra_flags,ITEM_ROT_DEATH);

    /* now add the enchantments */

    if (obj->level < LEVEL_HERO - 1)
	obj->level = UMIN(LEVEL_HERO - 1,obj->level + 1);

    if (dam_found)
    {
	for ( paf = obj->affected; paf != NULL; paf = paf->next)
	{
	    if ( paf->location == APPLY_DAMROLL)
	    {
		paf->type = sn;
		paf->modifier += added;
		paf->level = UMAX(paf->level,level);
	    }
	}
    }
    else /* add a new affect */
    {
	paf = new_affect();

	paf->where	= TO_OBJECT;
	paf->type	= sn;
	paf->level	= level;
	paf->duration	= -1;
	paf->location	= APPLY_DAMROLL;
	paf->modifier	=  added;
	paf->bitvector  = 0;
    	paf->next	= obj->affected;
    	obj->affected	= paf;
    }

    if (hit_found)
    {
        for ( paf = obj->affected; paf != NULL; paf = paf->next)
	{
            if ( paf->location == APPLY_HITROLL)
            {
		paf->type = sn;
                paf->modifier += added;
                paf->level = UMAX(paf->level,level);
            }
	}
    }
    else /* add a new affect */
    {
        paf = new_affect();

        paf->type       = sn;
        paf->level      = level;
        paf->duration   = -1;
        paf->location   = APPLY_HITROLL;
        paf->modifier   =  added;
        paf->bitvector  = 0;
        paf->next       = obj->affected;
        obj->affected   = paf;
    }

	return;

}

void spell_fireball( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *vch;
	char buf[MSL];
    int dam = number_range(125,135);

    if (check_spellcraft(ch,sn))
		dam += spellcraft_dam(level,3);
    else
		dam += dice(level,3);

	act("$n conjures a fireball into the room!",ch,0,0,TO_ROOM);
	act("You conjure a fireball into the room!",ch,0,0,TO_CHAR);
	
    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
		if ((is_same_group(vch,ch) || IS_IMMORTAL(vch)) && ch->fighting != vch) {
		    continue;
		}
		if (ch->cabal != 0 && is_same_cabal(vch,ch )) {
		    continue;
		}

		act("You are engulfed by $n's fireball!",ch,0,vch,TO_VICT);
		switch(number_range(0,2))
		{
			case (0):
			case (1):
			sprintf(buf,"Die, %s you sorcerous dog!",PERS(ch,vch));
			break;
			case (2):
			sprintf(buf,"Help! %s is casting a spell on me!",PERS(ch,vch));
		}
		
	    if ( saves_spell( level, vch, DAM_FIRE) )
			dam /= 2;
	    damage_old( ch, vch, dam, sn, DAM_FIRE ,TRUE);
    }

    return;
}


void spell_fireproof(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA af;

    if (IS_OBJ_STAT(obj,ITEM_BURN_PROOF))
    {
        act("$p is already protected from burning.",ch,obj,NULL,TO_CHAR);
        return;
    }

    init_affect(&af);
    af.where     = TO_OBJECT;
    af.type      = sn;
    af.aftype	 = AFT_SPELL;
    af.level     = level;
    af.duration  = number_fuzzy(level / 4);
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = ITEM_BURN_PROOF;

    affect_to_obj(obj,&af);

    act("You protect $p from fire.",ch,obj,NULL,TO_CHAR);
    act("$p is surrounded by a protective aura.",ch,obj,NULL,TO_ROOM);
}



void spell_flamestrike( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if (check_spellcraft(ch,sn))
	dam = spellcraft_dam(6 + level / 2,8);
    else
    dam = dice(6 + level / 2, 8);
    if ( saves_spell( level, victim,DAM_FIRE) )
	dam /= 2;
    damage_old( ch, victim, dam, sn, DAM_FIRE ,TRUE);
    return;
}



void spell_faerie_fire( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_FAERIE_FIRE) )
	{		
		send_to_char("You are already glowing.\n\r", victim );

		if( ch != victim )
		{
			act("$N is already glowing.",ch,NULL,victim,TO_CHAR );
			act("$N is already glowing.",ch,NULL,victim,TO_ROOM );
		}
		return;
	}
		
    if (saves_spell(level+15, victim,DAM_OTHER))
	{
		if( ch == victim )
			send_to_char("You resist your faerie fire.\n\r",victim);

		if( ch != victim )
		{
			act("$N resists your faerie fire.",ch,NULL,victim,TO_CHAR );
			act("$N resists $n's faerie fire.",ch,NULL,victim,TO_ROOM );
			act("You resist $n's faerie fire.",ch,NULL,victim,TO_VICT );
		}
		return;
	}
	
	affect_strip ( victim,gsn_camouflage		);
	affect_strip ( victim, gsn_invis			);
	affect_strip ( victim, gsn_mass_invis		);
	affect_strip ( victim, gsn_sneak			);
	affect_strip ( victim, gsn_earthfade		);
	affect_strip ( victim, gsn_duo 			);
	REMOVE_BIT   ( victim->affected_by, AFF_HIDE	);
	REMOVE_BIT   ( victim->affected_by, AFF_INVISIBLE	);
	REMOVE_BIT   ( victim->affected_by, AFF_SNEAK	);
	REMOVE_BIT	( victim->affected_by, AFF_CAMOUFLAGE );

	init_affect(&af);
    af.where     = TO_AFFECTS;
	af.aftype	 = get_spell_aftype(ch);
    af.type      = sn;
    af.level	 = level;
    af.duration  = 5;
    af.location  = APPLY_AC;
    af.modifier  = -5 * AC_PER_ONE_PERCENT_DECREASE_DAMAGE;
    af.bitvector = AFF_FAERIE_FIRE;
    affect_to_char( victim, &af );
    send_to_char( "You are surrounded by a pink outline.\n\r", victim );
    act( "$n is surrounded by a pink outline.", victim, NULL, NULL, TO_ROOM );
    return;
}



void spell_faerie_fog( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *ich;
    AFFECT_DATA af;

    act( "$n conjures a cloud of purple smoke.", ch, NULL, NULL, TO_ROOM );
    send_to_char( "You conjure a cloud of purple smoke.\n\r", ch );

	init_affect(&af);
    af.where = TO_AFFECTS;
af.aftype	 = get_spell_aftype(ch);
    af.type = sn;
    af.level = level;
    af.duration = 4 + (level/8);
    af.bitvector = 0;
    af.modifier = 0;

    for ( ich = ch->in_room->people; ich != NULL; ich = ich->next_in_room )
    {
	if (ich->invis_level > 0)
	    continue;

	if (ich->trust > 51 )
	    continue;

	if ( ich == ch || saves_spell( level, ich,DAM_OTHER) )
	    continue;

	af.affect_list_msg = str_dup("unable to conceal yourself");
	affect_to_char( ich,&af);

	affect_strip ( ich,gsn_camouflage		);
	affect_strip ( ich, gsn_invis			);
	affect_strip ( ich, gsn_mass_invis		);
	affect_strip ( ich, gsn_sneak			);
	affect_strip ( ich, gsn_earthfade		);
	affect_strip ( ich, gsn_duo 			);
	REMOVE_BIT   ( ich->affected_by, AFF_HIDE	);
	REMOVE_BIT   ( ich->affected_by, AFF_INVISIBLE	);
	REMOVE_BIT   ( ich->affected_by, AFF_SNEAK	);
	REMOVE_BIT	( ich->affected_by, AFF_CAMOUFLAGE );
	act( "$n is revealed!", ich, NULL, NULL, TO_ROOM );
	send_to_char( "You are revealed!\n\r", ich );
    }

    return;
}


void spell_fly( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_FLYING) )
    {
	if (victim == ch)
	  send_to_char("You are already airborne.\n\r",ch);
	else
	  act("$N doesn't need your help to fly.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (is_affected(ch,skill_lookup("bolo toss"))) {
	return send_to_char("Your magic cannot overcome the weight of the bolo twisted around your torso.\n\r",ch);
    }

    init_affect(&af);
    af.where     = TO_AFFECTS;
    af.aftype	 = get_spell_aftype(ch);
    af.type      = sn;
    af.level	 = level;
    af.duration  = level + 3;
    af.location  = 0;
    af.modifier  = 0;
    af.affect_list_msg = str_dup("grants flight");
    af.bitvector = AFF_FLYING;
    affect_to_char( victim, &af );
    send_to_char( "Your feet rise off the ground.\n\r", victim );
    act( "$n's feet rise off the ground.", victim, NULL, NULL, TO_ROOM );
    return;
}

/* RT clerical berserking spell */

void spell_frenzy(int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (is_affected(victim,sn) || IS_AFFECTED(victim,AFF_BERSERK))
    {
	if (victim == ch)
	  send_to_char("You are already in a frenzy.\n\r",ch);
	else
	  act("$N is already in a frenzy.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (is_affected(victim,skill_lookup("calm")))
    {
	if (victim == ch)
	  send_to_char("Why don't you just relax for a while?\n\r",ch);
	else
	  act("$N doesn't look like $e wants to fight anymore.", ch,NULL,victim,TO_CHAR);
	return;
    }

    if ((IS_GOOD(ch) && !IS_GOOD(victim)) ||
	(IS_NEUTRAL(ch) && !IS_NEUTRAL(victim)) ||
	(IS_EVIL(ch) && !IS_EVIL(victim)) )
    {
	act("Your god doesn't seem to like $N",ch,NULL,victim,TO_CHAR);
	return;
    }

    init_affect(&af);
    af.aftype 		= AFT_COMMUNE;
    af.where     	= TO_AFFECTS;
    af.type 	 	= sn;
    af.level	 	= level;
    af.duration	 	= level / 3;
    af.modifier  	= level / 6;
    af.bitvector 	= 0;

    af.location  	= APPLY_HITROLL;
    affect_to_char(victim,&af);

    af.location  	= APPLY_DAMROLL;
    affect_to_char(victim,&af);

    af.modifier  	= -2 * AC_PER_ONE_PERCENT_DECREASE_DAMAGE;
    af.location  	= APPLY_AC;
    affect_to_char(victim,&af);

    send_to_char("You are filled with holy wrath!\n\r",victim);
    act("$n gets a wild look in $s eyes!",victim,NULL,NULL,TO_ROOM);
}

/* RT ROM-style gate */

void spell_gate( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim;

    if (ch->fighting != NULL)
    {
	 send_to_char("You can't concentrate enough.\n\r",ch);
	 return;
    }

    victim = get_char_world( ch, target_name );

    if (!(victim==NULL))
    {
    	if ((IS_NPC(victim)) && ( IS_SET(victim->res_flags,RES_SUMMON) ))
	    level -= 5;
    	if ((IS_NPC(victim)) && ( IS_SET(victim->vuln_flags,VULN_SUMMON) ))
	    level += 5;
    }

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
    || 	 (!IS_NPC(victim) && saves_spell(level,victim,DAM_OTHER))
    ||   (!IS_NPC(victim) && victim->level > LEVEL_HERO)
    ||   (IS_NPC(victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
    ||   (IS_NPC(victim) && saves_spell( level, victim,DAM_OTHER) ) )
    {
        send_to_char( "You failed.\n\r", ch );
        return;
    }

    if (victim->in_room->cabal != 0 && victim->in_room->cabal != victim->cabal)
    {
	send_to_char("You failed.\n\r",victim);
	return;
    }

    act("$n steps through a gate and vanishes.",ch,NULL,NULL,TO_ROOM);
    send_to_char("You step through a gate and vanish.\n\r",ch);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);

    act("$n has arrived through a gate.",ch,NULL,NULL,TO_ROOM);
    do_look(ch,"auto");

    un_camouflage( ch, NULL);
    un_earthfade(ch, NULL);
    un_hide(ch, NULL);
    un_sneak(ch, NULL);

}



void spell_giant_strength(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You are already as strong as you can get!\n\r",ch);
	else
	  act("$N can't get any stronger.",ch,NULL,victim,TO_CHAR);
	return;
    }

	init_affect(&af);
    af.where     = TO_AFFECTS;
	af.aftype	 = get_spell_aftype(ch);
    af.type      = sn;
    af.level	 = level;
    af.duration  = level;
    af.location  = APPLY_STR;
    af.modifier  = 1 + (level >= 18) + (level >= 25) + (level >= 32);
    af.bitvector = 0;
    affect_to_char( victim, &af );
    af.location  = APPLY_ENHANCED_MOD;
    af.modifier  = 2;
    affect_to_char( victim, &af );
    send_to_char( "Your muscles surge with heightened power!\n\r", victim );
    act("$n's muscles surge with heightened power.",victim,NULL,NULL,TO_ROOM);
    return;
}



void spell_harm( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = UMAX(  20, victim->hit - dice(1,4) );
    if ( saves_spell( level, victim,DAM_HARM) )
	dam = UMIN( 50, dam / 2 );
    dam = UMIN( 100, dam );
    damage_old( ch, victim, dam, sn, DAM_HARM ,TRUE);
    return;
}

/* RT haste spell */

void spell_haste( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (!str_cmp(class_table[ch->class].name, "monk")
	&& victim != ch)
    {
	send_to_char("You are only able to enhance your own physical abilities.\n\r",ch);
	return;
    }

    if ( is_affected( victim, sn ) || IS_AFFECTED(victim,AFF_HASTE)
    ||   IS_SET(victim->off_flags,OFF_FAST))
    {
	if (victim == ch)
	  send_to_char("You can't move any faster!\n\r",ch);
 	else
	  act("$N is already moving as fast as $E can.",
	      ch,NULL,victim,TO_CHAR);
        return;
    }

	if (IS_AFFECTED(victim,AFF_SLOW))
    	{
	  	send_to_char("You start moving at normal speed again.\n\r",victim);
        act("$n starts moving at normal speed again.",victim,NULL,NULL,TO_ROOM);
	affect_strip(victim,skill_lookup("slow"));
        return;
    	}

	init_affect(&af);
    af.where     = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
    af.type      = sn;
    af.level     = level;
    if (victim == ch)
      af.duration  = level/2;
    else
      af.duration  = level/2;
    af.location  = APPLY_DEX;
    af.modifier  = 1 + (level >= 18) + (level >= 25) + (level >= 32);
    af.bitvector = AFF_HASTE;
	char msg_buf[MSL];
	sprintf(msg_buf,"increases likelihood of attacks and dexterity by %d",af.modifier);
	af.affect_list_msg = str_dup(msg_buf);
    affect_to_char( victim, &af );
    send_to_char( "You feel yourself moving more quickly.\n\r", victim );
    act("$n is moving more quickly.",victim,NULL,NULL,TO_ROOM);
    if ( ch != victim )
        send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_heal( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    victim->hit = UMIN( victim->hit + 100, victim->max_hit );
    update_pos( victim );
    send_to_char( "A warm feeling fills your body.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}

void spell_heat_metal( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    OBJ_DATA *obj_lose, *obj_next;
    int dam = 0;
    bool fail = TRUE;
	return;

   if (!saves_spell(level + 2,victim,DAM_FIRE)
   &&  !IS_SET(victim->imm_flags,IMM_FIRE))
   {
        for ( obj_lose = victim->carrying;
	      obj_lose != NULL;
	      obj_lose = obj_next)
        {
	    obj_next = obj_lose->next_content;
            if ( number_range(1,2 * level) > obj_lose->level
	    &&   !saves_spell(level,victim,DAM_FIRE)
	    &&   IS_OBJ_STAT(obj_lose,ITEM_METAL))
            {
                switch ( obj_lose->item_type )
                {
               	case ITEM_ARMOR:
		if (obj_lose->wear_loc != -1) /* remove the item */
		{
		    if (can_drop_obj(victim,obj_lose)
		    &&  (obj_lose->weight / 10) <
			number_range(1,2 * get_curr_stat(victim,STAT_DEX))
		    &&  remove_obj( victim, obj_lose->wear_loc, TRUE ))
		    {
		        act("$n yelps and throws $p to the ground!",
			    victim,obj_lose,NULL,TO_ROOM);
		        act("You remove and drop $p before it burns you.",
			    victim,obj_lose,NULL,TO_CHAR);
			dam += (number_range(1,obj_lose->level) / 3);
                        obj_from_char(obj_lose);
                        obj_to_room(obj_lose, victim->in_room);
                        fail = FALSE;
                    }
		    else /* stuck on the body! ouch! */
		    {
			act("Your skin is seared by $p!",
			    victim,obj_lose,NULL,TO_CHAR);
			dam += (number_range(1,obj_lose->level));
			fail = FALSE;
		    }

		}
		else /* drop it if we can */
		{
		    if (can_drop_obj(victim,obj_lose))
		    {
                        act("$n yelps and throws $p to the ground!",
                            victim,obj_lose,NULL,TO_ROOM);
                        act("You and drop $p before it burns you.",
                            victim,obj_lose,NULL,TO_CHAR);
                        dam += (number_range(1,obj_lose->level) / 6);
                        obj_from_char(obj_lose);
                        obj_to_room(obj_lose, victim->in_room);
			fail = FALSE;
                    }
		    else /* cannot drop */
		    {
                        act("Your skin is seared by $p!",
                            victim,obj_lose,NULL,TO_CHAR);
                        dam += (number_range(1,obj_lose->level) / 2);
			fail = FALSE;
                    }
		}
                break;
                case ITEM_WEAPON:
		if (obj_lose->wear_loc != -1) /* try to drop it */
		{
		    if (IS_WEAPON_STAT(obj_lose,WEAPON_FLAMING))
			continue;

		    if (can_drop_obj(victim,obj_lose)
		    &&  remove_obj(victim,obj_lose->wear_loc,TRUE))
		    {
			act("$n is burned by $p, and throws it to the ground.",
			    victim,obj_lose,NULL,TO_ROOM);
			send_to_char(
			    "You throw your red-hot weapon to the ground!\n\r",
			    victim);
			dam += 1;
			obj_from_char(obj_lose);
			obj_to_room(obj_lose,victim->in_room);
			fail = FALSE;
		    }
		    else /* YOWCH! */
		    {
			send_to_char("Your weapon sears your flesh!\n\r",
			    victim);
			dam += number_range(1,obj_lose->level);
			fail = FALSE;
		    }
		}
                else /* drop it if we can */
                {
                    if (can_drop_obj(victim,obj_lose))
                    {
                        act("$n throws a burning hot $p to the ground!",
                            victim,obj_lose,NULL,TO_ROOM);
                        act("You and drop $p before it burns you.",
                            victim,obj_lose,NULL,TO_CHAR);
                        dam += (number_range(1,obj_lose->level) / 6);
                        obj_from_char(obj_lose);
                        obj_to_room(obj_lose, victim->in_room);
                        fail = FALSE;
                    }
                    else /* cannot drop */
                    {
                        act("Your skin is seared by $p!",
                            victim,obj_lose,NULL,TO_CHAR);
                        dam += (number_range(1,obj_lose->level) / 2);
                        fail = FALSE;
                    }
                }
                break;
		}
	    }
	}
    }
    if (fail)
    {
        send_to_char("Your spell had no effect.\n\r", ch);
	send_to_char("You feel momentarily warmer.\n\r",victim);
    }
    else /* damage! */
    {
	if (saves_spell(level,victim,DAM_FIRE))
	    dam = 2 * dam / 3;
	damage_old(ch,victim,dam,sn,DAM_FIRE,TRUE);
    }
}

void spell_identify( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;

    sprintf( buf,
	"Object '%s' is type %s, extra flags %s.\n\rWeight is %d, value is %d, level is %d.\n\rMaterial is %s.\n\r",

	obj->name,
        item_name(obj->item_type),
	bitmask_string(&obj->extra_flags, extra_flags),
	obj->weight,
	obj->cost,
	obj->level,
	obj->material->name
	);
    send_to_char( buf, ch );

    sprintf( buf, "restrict flags %s.\n\r",
restrict_bit_name(obj->pIndexData->restrict_flags));
    send_to_char(buf,ch);

    switch ( obj->item_type )
    {
    case ITEM_SCROLL:
    case ITEM_POTION:
    case ITEM_PILL:
	sprintf( buf, "Level %ld spells of:", obj->value[0] );
	send_to_char( buf, ch );

	if ( obj->value[1] >= 0 && obj->value[1] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[1]].name, ch );
	    send_to_char( "'", ch );
	}

	if ( obj->value[2] >= 0 && obj->value[2] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[2]].name, ch );
	    send_to_char( "'", ch );
	}

	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[3]].name, ch );
	    send_to_char( "'", ch );
	}

	if (obj->value[4] >= 0 && obj->value[4] < MAX_SKILL)
	{
	    send_to_char(" '",ch);
	    send_to_char(skill_table[obj->value[4]].name,ch);
	    send_to_char("'",ch);
	}

	send_to_char( ".\n\r", ch );
	break;

    case ITEM_WAND:
    case ITEM_STAFF:
	sprintf( buf, "Has %ld charges of level %ld",
	    obj->value[2], obj->value[0] );
	send_to_char( buf, ch );

	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[3]].name, ch );
	    send_to_char( "'", ch );
	}

	send_to_char( ".\n\r", ch );
	break;

    case ITEM_DRINK_CON:
        sprintf(buf,"It holds %s-colored %s.\n\r",
            liq_table[obj->value[2]].liq_color,
            liq_table[obj->value[2]].liq_name);
        send_to_char(buf,ch);
        break;

    case ITEM_CONTAINER:
	sprintf(buf,"Capacity: %ld#  Maximum weight: %ld#  flags: %s\n\r",
	    obj->value[0], obj->value[3], cont_bit_name(obj->value[1]));
	send_to_char(buf,ch);
	break;

    case ITEM_WEAPON:
 	send_to_char("Weapon type is ",ch);
	switch (obj->value[0])
	{
	    case(WEAPON_EXOTIC) : send_to_char("exotic.\n\r",ch);	break;
	    case(WEAPON_SWORD)  : send_to_char("sword.\n\r",ch);	break;
	    case(WEAPON_DAGGER) : send_to_char("dagger.\n\r",ch);	break;
	    case(WEAPON_SPEAR)	: send_to_char("spear/staff.\n\r",ch);	break;
	    case(WEAPON_MACE) 	: send_to_char("mace/club.\n\r",ch);	break;
	    case(WEAPON_AXE)	: send_to_char("axe.\n\r",ch);		break;
	    case(WEAPON_FLAIL)	: send_to_char("flail.\n\r",ch);	break;
	    case(WEAPON_WHIP)	: send_to_char("whip.\n\r",ch);		break;
	    case(WEAPON_POLEARM): send_to_char("polearm.\n\r",ch);	break;
	case (WEAPON_STAFF) : send_to_char("staff.\n\r",ch);	break;
	    default		: send_to_char("unknown.\n\r",ch);	break;
 	}
	if (obj->pIndexData->new_format)
	    sprintf(buf,"Damage is %ldd%ld (average %ld).\n\r",
		obj->value[1],obj->value[2],
		(1 + obj->value[2]) * obj->value[1] / 2);
	else
	    sprintf( buf, "Damage is %ld to %ld (average %ld).\n\r",
	    	obj->value[1], obj->value[2],
	    	( obj->value[1] + obj->value[2] ) / 2 );
	send_to_char( buf, ch );
        if (obj->value[4])  /* weapon flags */
        {
            sprintf(buf,"Weapons flags: %s\n\r",weapon_bit_name(obj->value[4]));
            send_to_char(buf,ch);
        }
	break;

    case ITEM_ARMOR:
	sprintf( buf,
	"Armor class is %ld pierce, %ld bash, %ld slash, and %ld vs. magic.\n\r",
	    obj->value[0], obj->value[1], obj->value[2], obj->value[3] );
	send_to_char( buf, ch );
	break;
    }

    if (!obj->enchanted)
    for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
	    sprintf( buf, "Affects %s by %d.\n\r",
		affect_loc_name( paf->location ), paf->modifier );
	    send_to_char(buf,ch);
            if (paf->bitvector)
            {
                switch(paf->where)
                {
                    case TO_AFFECTS:
                        sprintf(buf,"Adds %s affect.\n",
                            affect_bit_name(paf->bitvector));
                        break;
                    case TO_OBJECT:
                        sprintf(buf,"Adds %s object flag.\n",
                            extra_bit_name(paf->bitvector));
                        break;
                    case TO_IMMUNE:
                        sprintf(buf,"Adds immunity to %s.\n",
                            imm_bit_name(paf->bitvector));
                        break;
                    case TO_RESIST:
                        sprintf(buf,"Adds resistance to %s.\n\r",
                            imm_bit_name(paf->bitvector));
                        break;
                    case TO_VULN:
                        sprintf(buf,"Adds vulnerability to %s.\n\r",
                            imm_bit_name(paf->bitvector));
                        break;
                    default:
                        sprintf(buf,"Unknown bit %d: %d\n\r",
                            paf->where,paf->bitvector);
                        break;
                }
	        send_to_char( buf, ch );
	    }
	}
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
	    if (strcmp(affect_loc_name( paf->location ),"none") != 0)
			sprintf( buf, "Affects %s by %d",
				affect_loc_name( paf->location ), paf->modifier );
				
	    send_to_char( buf, ch );
            if ( paf->duration > -1)
                sprintf(buf,", %d hours.\n\r",paf->duration);
            else
                sprintf(buf,".\n\r");
	    send_to_char(buf,ch);
            if (paf->bitvector)
            {
                switch(paf->where)
                {
                    case TO_AFFECTS:
                        sprintf(buf,"Adds %s affect.\n",
                            affect_bit_name(paf->bitvector));
                        break;
                    case TO_OBJECT:
                        sprintf(buf,"Adds %s object flag.\n",
                            extra_bit_name(paf->bitvector));
                        break;
		    case TO_WEAPON:
			sprintf(buf,"Adds %s weapon flags.\n",
			    weapon_bit_name(paf->bitvector));
			break;
                    case TO_IMMUNE:
                        sprintf(buf,"Adds immunity to %s.\n",
                            imm_bit_name(paf->bitvector));
                        break;
                    case TO_RESIST:
                        sprintf(buf,"Adds resistance to %s.\n\r",
                            imm_bit_name(paf->bitvector));
                        break;
                    case TO_VULN:
                        sprintf(buf,"Adds vulnerability to %s.\n\r",
                            imm_bit_name(paf->bitvector));
                        break;
                    default:
                        sprintf(buf,"Unknown bit %d: %d\n\r",
                            paf->where,paf->bitvector);
                        break;
                }
                send_to_char(buf,ch);
            }
	}
    }

    return;
}



void spell_infravision( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_INFRARED) )
    {
	if (victim == ch)
	  send_to_char("You can already see in the dark.\n\r",ch);
	else
	  act("$N already has infravision.\n\r",ch,NULL,victim,TO_CHAR);
	return;
    }

    act( "$n's eyes glow red.\n\r", victim, NULL, NULL, TO_ROOM );

	init_affect(&af);
    af.where	 = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
    af.type      = sn;
    af.level	 = level;
    af.duration  = 2 * level;
    af.location  = APPLY_NONE;
    af.affect_list_msg = str_dup("grants dark vision");
    af.modifier  = 0;
    af.bitvector = AFF_INFRARED;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes glow red.\n\r", victim );
    return;
}



void spell_invis( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim;
    AFFECT_DATA af;
    int sn_fog, sn_fire;

    sn_fog = skill_lookup("faerie fog");
    sn_fire = skill_lookup("faerie fire");
    if (is_affected(ch,sn_fog) || is_affected(ch,sn_fire))
    {
	send_to_char("You can't turn invisible while revealed.\n\r",ch);
	return;
    }
    if (target == TARGET_OBJ)
    {
        send_to_char("You must be crazy.\n\r", ch);
        return;
    }
    /* character invisibility */
    victim = (CHAR_DATA *) vo;

    if ( IS_AFFECTED(victim, AFF_INVISIBLE) )
	return;

    act( "$n fades out of existence.", victim, NULL, NULL, TO_ROOM );

	init_affect(&af);
    af.where     = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
    af.type      = sn;
    af.level     = level;
    af.duration  = level + 12;
    af.location  = APPLY_NONE;
    af.affect_list_msg = str_dup("grants invisibility");
    af.modifier  = 0;
    af.bitvector = AFF_INVISIBLE;
    affect_to_char( victim, &af );
    send_to_char( "You fade out of existence.\n\r", victim );
    return;
}



void spell_know_alignment(int sn,int level,CHAR_DATA *ch,void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    char *msg;
    int ap;

    ap = victim->alignment;

         if ( ap >  0 ) msg = "$N has a pure and good aura.";
    else if ( ap == 0 ) msg = "$N doesn't have a firm moral commitment.";
    else msg = "$N is the embodiment of pure evil!.";

    act( msg, ch, NULL, victim, TO_CHAR );

    if (!IS_NPC(victim))
	ap = victim->pcdata->ethos;

    if ( ap > 0 ) msg = "$N abides by the law of the land.";
    else if ( ap == 0 ) msg = "$N doesn't know what to think about the law";
    else msg = "$N has absolutely no cares for the law.";

    act( msg, ch, NULL, victim, TO_CHAR );
    return;
}


void spell_channel(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
        AFFECT_DATA af;

     	if (cabal_down(ch,CABAL_ARCANA))
		return;

   	if (is_affected(ch,sn) || ch->hit > ch->max_hit)
	{
		send_to_char("You are already as healthy as you can get.\n\r",ch);
		return;
	}

        if (IS_NPC(ch))
        	return;

        ch->hit += ch->pcdata->perm_mana*.4;

	init_affect(&af);
        af.where 	= TO_AFFECTS;
	af.aftype    	= AFT_POWER;
        af.type 	= sn;
        af.level 	= level;
        af.duration 	= 24;
        af.location	= APPLY_HIT;
        af.modifier 	= ch->pcdata->perm_mana*.4;
        af.bitvector 	= 0;
        affect_to_char(ch,&af);

        send_to_char("You feel your health improve as you control your body with your mind.\n\r",ch);
}

void spell_lightning_bolt(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] =
    {
	 0,
	 0,  0,  0,  0,  0,	 0,  0,  0, 25, 28,
	31, 34, 37, 40, 40,	41, 42, 42, 43, 44,
	44, 45, 46, 46, 47,	48, 48, 49, 50, 50,
	51, 52, 52, 53, 54,	54, 55, 56, 56, 57,
	58, 58, 59, 60, 60,	61, 62, 62, 63, 64
    };
    int dam;

    if (is_affected(ch,gsn_grounding))
	{
	send_to_char("The electricity fizzles at your feet.\n\r",ch);
	return;
	}

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if (check_spellcraft(ch,sn))
	dam += level;

    if ( saves_spell( level, victim,DAM_LIGHTNING) )
	dam /= 2;
    damage_old( ch, victim, dam, sn, DAM_LIGHTNING ,TRUE);
    return;
}

void spell_locate_object( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    char buf[MAX_INPUT_LENGTH];
    BUFFER *buffer;
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    bool found;
    int number = 0, max_found;

    found = FALSE;
    number = 0;
    max_found = IS_IMMORTAL(ch) ? 200 : 2 * level;

    buffer = new_buf();
 
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( !can_see_obj( ch, obj ) || !is_name( target_name, obj->name ) 
    	||   IS_OBJ_STAT(obj,ITEM_NOLOCATE) || number_percent() > 2 * level
	||   ch->level < obj->level)
	    continue;

	found = TRUE;
        number++;

	for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
	    ;
	if(in_obj->carried_by==NULL||!is_affected(in_obj->carried_by,gsn_cloak_form)) {
	if ( in_obj->carried_by != NULL && can_see(ch,in_obj->carried_by))
	{
	    sprintf( buf, "%s is carried by %s\n\r",
		obj->short_descr, PERS(in_obj->carried_by, ch) );
	}
	else
	{
	    if (IS_IMMORTAL(ch) && in_obj->in_room != NULL) {
		sprintf( buf, "%s is in %s [Room %ld]\n\r",
		    obj->short_descr, in_obj->in_room->name, in_obj->in_room->vnum);
	    } else {
		number--;
		if (number ==0)
			found = FALSE;
		continue;
	    }
	}
	buf[0] = UPPER(buf[0]);
	add_buf(buffer,buf);

    	if (number >= max_found)
	    break;
	} else {
	    found=FALSE;
	    number--;
	}
    }

    if ( !found )
	send_to_char( "Nothing like that in heaven or earth.\n\r", ch );
    else
	page_to_char(buf_string(buffer),ch);

    free_buf(buffer);

    return;
}

void spell_magic_missile( int sn, int level, CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] =
    {
	 0,
	 3,  3,  4,  4,  5,	 6,  6,  6,  6,  6,
	 7,  7,  7,  7,  7,	 8,  8,  8,  8,  8,
	 9,  9,  9,  9,  9,	10, 10, 10, 10, 10,
	11, 11, 11, 11, 11,	12, 12, 12, 12, 12,
	13, 13, 13, 13, 13,	14, 14, 14, 14, 14
    };
    int dam;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );

    if (check_spellcraft(ch,sn))
	dam += level;
    if ( saves_spell( level, victim,DAM_ENERGY) )
	dam /= 2;
    damage_old( ch, victim, dam, sn, DAM_ENERGY ,TRUE);
    return;
}

void spell_mass_healing(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *gch;
    int heal_num, refresh_num;

    heal_num = skill_lookup("heal");
    refresh_num = skill_lookup("refresh");

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if (((IS_NPC(ch) && IS_NPC(gch)) ||
	    (!IS_NPC(ch) && !IS_NPC(gch))) && is_same_group(ch,gch))
	{
	    spell_heal(heal_num,level,ch,(void *) gch,TARGET_CHAR);
	    spell_refresh(refresh_num,level,ch,(void *) gch,TARGET_CHAR);
	}
    }
}


void spell_mass_invis( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    AFFECT_DATA af;
    CHAR_DATA *gch;
    int sn_fog, sn_fire;

    sn_fog = skill_lookup("faerie fog");
    sn_fire = skill_lookup("faerie fire");

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( !is_same_group( gch, ch ) || IS_AFFECTED(gch, AFF_INVISIBLE) )
	    continue;
    if (is_affected(ch,sn_fog) || is_affected(ch,sn_fire))
    {
	send_to_char("You can't turn invisible while revealed.\n\r",gch);
	continue;
    }
	act( "$n slowly fades out of existence.", gch, NULL, NULL, TO_ROOM );
	send_to_char( "You slowly fade out of existence.\n\r", gch );

	init_affect(&af);
	af.where     = TO_AFFECTS;
	af.aftype	 = get_spell_aftype(ch);
	af.type      = sn;
    	af.level     = level/2;
	af.duration  = 24;
	af.location  = APPLY_NONE;
	af.affect_list_msg = str_dup("grants invisibility");
	af.modifier  = 0;
	af.bitvector = AFF_INVISIBLE;
	affect_to_char( gch, &af );
    }
    send_to_char( "Ok.\n\r", ch );

    return;
}



void spell_null( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    send_to_char( "That's not a spell!\n\r", ch );
    return;
}



void spell_pass_door( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_PASS_DOOR) )
    {
	if (victim == ch)
	  send_to_char("You are already out of phase.\n\r",ch);
	else
	  act("$N is already shifted out of phase.",ch,NULL,victim,TO_CHAR);
	return;
    }

	init_affect(&af);
    af.where     = TO_AFFECTS;
	af.aftype	 = get_spell_aftype(ch);
    af.type      = sn;
    af.level     = level;
    af.duration  = number_fuzzy( level / 4 );
    af.location  = APPLY_NONE;
    af.affect_list_msg = str_dup("grants the ability to pass through doors");
    af.modifier  = 0;
    af.bitvector = AFF_PASS_DOOR;
    affect_to_char( victim, &af );
    act( "$n turns translucent.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You turn translucent.\n\r", victim );
    return;
}

/* RT plague spell, very nasty */

void spell_plague( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (victim->class==class_lookup("lich"))
    {
	act("You ignore the plague completely.",ch,0,victim,TO_VICT);
	act("$n ignores the plague completely.",ch,0,victim,TO_CHAR);
	act("$n ignores the plague completely.",ch,0,victim,TO_NOTVICT);
	return;
    }

    if (saves_spell(level,victim,DAM_DISEASE) ||
        (IS_NPC(victim) && IS_SET(victim->act,ACT_UNDEAD))
	|| (IS_NPC(victim) && victim->pIndexData->vnum == MOB_VNUM_RAGER))
    {
	if (ch == victim)
	  send_to_char("You feel momentarily ill, but it passes.\n\r",ch);
	else
	  act("$N seems to be unaffected.",ch,NULL,victim,TO_CHAR);
	return;
    }

        init_affect(&af);
    af.where     = TO_AFFECTS;
	af.aftype	 = get_spell_aftype(ch);
    af.type 	  = sn;
    af.level	  = level * 3/4;
    af.duration  = 12;
    af.location  = APPLY_STR;
    af.modifier  = -2;
    af.bitvector = AFF_PLAGUE;
    af.aftype    = AFT_MALADY;
    af.owner_name = str_dup(ch->original_name);
    af.end_fun	 = NULL;
    affect_join(victim,&af);

    send_to_char
      ("You scream in agony as plague sores erupt from your skin.\n\r",victim);
    act("$n screams in agony as plague sores erupt from $s skin.",
	victim,NULL,NULL,TO_ROOM);
}

void spell_poison( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    AFFECT_DATA af;


    if (target == TAR_OBJ_INV)
    {
	obj = (OBJ_DATA *) vo;

	if (obj->item_type == ITEM_FOOD || obj->item_type == ITEM_DRINK_CON)
	{
	    if (IS_OBJ_STAT(obj,ITEM_BLESS) || IS_OBJ_STAT(obj,ITEM_BURN_PROOF))
	    {
		act("Your spell fails to corrupt $p.",ch,obj,NULL,TO_CHAR);
		return;
	    }
	    obj->value[3] = 1;
	    act("$p is infused with poisonous vapors.",ch,obj,NULL,TO_ALL);
	    return;
	}

	if (obj->item_type == ITEM_WEAPON)
	{
	    if (IS_WEAPON_STAT(obj,WEAPON_FLAMING)
	    ||  IS_WEAPON_STAT(obj,WEAPON_FROST)
	    ||  IS_WEAPON_STAT(obj,WEAPON_VAMPIRIC)
	    ||  IS_WEAPON_STAT(obj,WEAPON_SHARP)
	    ||  IS_WEAPON_STAT(obj,WEAPON_VORPAL)
	    ||  IS_WEAPON_STAT(obj,WEAPON_SHOCKING)
	    ||  IS_OBJ_STAT(obj,ITEM_BLESS) || IS_OBJ_STAT(obj,ITEM_BURN_PROOF))
	    {
		act("You can't seem to envenom $p.",ch,obj,NULL,TO_CHAR);
		return;
	    }

	    if (IS_WEAPON_STAT(obj,WEAPON_POISON))
	    {
		act("$p is already envenomed.",ch,obj,NULL,TO_CHAR);
		return;
	    }

	    init_affect(&af);
	    af.where	 = TO_WEAPON;
	    af.aftype	 = get_spell_aftype(ch);
	    af.type	 = sn;
	    af.level	 = level / 2;
	    af.duration	 = level/8;
 	    af.location	 = 0;
	    af.modifier	 = 0;
	    af.bitvector = WEAPON_POISON;
	    affect_to_obj(obj,&af);

	    act("$p is coated with deadly venom.",ch,obj,NULL,TO_ALL);
	    return;
	}

	act("You can't poison $p.",ch,obj,NULL,TO_CHAR);
	return;
    }

    victim = (CHAR_DATA *) vo;

    if ( victim->race==race_lookup("lich"))
    {
	act("$n ignores the poison completely.",victim,NULL,NULL,TO_ROOM);
	send_to_char("You ignore the poison completely.",victim);
	return;
    }

    if ( saves_spell( level, victim,DAM_POISON) )
    {
	act("$n turns slightly green, but it passes.",victim,NULL,NULL,TO_ROOM);
	send_to_char("You feel momentarily ill, but it passes.\n\r",victim);
	return;
    }

    if (IS_NPC(victim) && victim->pIndexData->vnum == MOB_VNUM_RAGER)
    {
	act("$n turns slightly green, but it passes.",victim,0,0,TO_ROOM);
	return;
    }

	init_affect(&af);
    af.where     = TO_AFFECTS;
	af.type  = get_spell_aftype(ch);
    af.type      = sn;
    af.level     = level;
    af.duration  = level/8;
    af.location  = APPLY_STR;
    af.modifier  = -2;
    af.bitvector = AFF_POISON;
	af.owner_name	= str_dup(ch->original_name);
    affect_join( victim, &af );
    send_to_char( "You feel very sick.\n\r", victim );
    act("$n looks very ill.",victim,NULL,NULL,TO_ROOM);
    return;
}

void spell_power_word_stun(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (is_affected(victim,gsn_power_word_stun) )
    {
	send_to_char("They are already stunned from another word of power.\n\r",ch);
	return;
    }
/*
    if (victim == ch)
    {
    send_to_char("You can not direct a word of power at yourself.\n\r",ch);
    return;
    }
*/
    act("$n gestures at $N and utters the word, 'Stun'.",ch,NULL,victim,TO_NOTVICT);
    act("$n gestures at you and utters the word, 'Stun'.",ch,NULL,victim,TO_VICT);
    act("You gesture at $N and invoke a word of dark power.",ch,NULL,victim,TO_CHAR);

    if (saves_spell(level-4,victim,DAM_NEGATIVE) )
    {
	act("$n reels from the dark energy but resists the power.",victim,NULL,NULL,TO_ROOM);
 	act("You feel the power blast at you, but you resist the stun.",victim,NULL,NULL,TO_CHAR);
      WAIT_STATE(ch,PULSE_VIOLENCE*3);
	return;
    }

	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
    af.type = gsn_power_word_stun;
    af.location = APPLY_HITROLL;
    af.modifier = -4;
    af.duration = 0;
    af.bitvector = 0;
    af.level = level;
    affect_to_char(victim,&af);
    af.location = APPLY_DEX;
    af.modifier = -3;
    affect_to_char(victim,&af);
    WAIT_STATE(ch,PULSE_VIOLENCE*3);
    act("The word of power sends $N to the ground with stunning force!",ch,NULL,victim,TO_NOTVICT);
    act("$N is crushed to the floor by your word of power!",ch,NULL,victim,TO_CHAR);
    send_to_char("You are sent crashing to the ground as the word hits you!",victim);
    victim->position = POS_RESTING;
    return;
}



void spell_protection_evil(int sn,int level,CHAR_DATA *ch,void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if(IS_EVIL(victim)) {
	send_to_char("Your soul rejects the purity.\n\r",victim);
	return;
    }

    if ( IS_AFFECTED(victim, AFF_PROTECT_EVIL)
    ||   IS_AFFECTED(victim, AFF_PROTECT_GOOD))
    {
        if (victim == ch)
          send_to_char("You are already protected.\n\r",ch);
        else
          act("$N is already protected.",ch,NULL,victim,TO_CHAR);
        return;
    }

	init_affect(&af);
    af.where     = TO_AFFECTS;
	af.aftype	 = get_spell_aftype(ch);
    af.type      = sn;
    af.level     = level;
    af.duration  = 24;
    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = -1;
    af.bitvector = AFF_PROTECT_EVIL;
    affect_to_char( victim, &af );
    af.location  = 0;
    af.modifier  = 0;
    af.affect_list_msg = str_dup("reduces damage from evil opponents by 25%");
    send_to_char( "You feel holy and pure.\n\r", victim );
    affect_to_char( victim, &af );
    if ( ch != victim )
        act("$N is protected from evil.",ch,NULL,victim,TO_CHAR);
    return;
}

void spell_protection_good(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if(IS_GOOD(victim)) {
	send_to_char("Your soul rejects the corruption.\n\r",victim);
	return;
    }

    if ( IS_AFFECTED(victim, AFF_PROTECT_GOOD)
    ||   IS_AFFECTED(victim, AFF_PROTECT_EVIL))
    {
        if (victim == ch)
          send_to_char("You are already protected.\n\r",ch);
        else
          act("$N is already protected.",ch,NULL,victim,TO_CHAR);
        return;
    }

	init_affect(&af);
    af.where     = TO_AFFECTS;
	af.aftype	 = get_spell_aftype(ch);
    af.type      = sn;
    af.level     = level;
    af.duration  = 24;
    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = -1;
    af.bitvector = AFF_PROTECT_GOOD;
    affect_to_char( victim, &af );
    af.location  = 0;
    af.modifier  = 0;
    af.affect_list_msg = str_dup("reduces damage from good opponents by 25%");
    affect_to_char( victim, &af );
    send_to_char( "You feel aligned with darkness.\n\r", victim );
    if ( ch != victim )
        act("$N is protected from good.",ch,NULL,victim,TO_CHAR);
    return;
}

void spell_ray_of_truth (int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
    if (IS_EVIL(ch))
    {
        victim = ch;
        send_to_char("The energy explodes inside you!\n\r",ch);
    }
 
    if (victim != ch)
    {
        act("$n raises $s hand, and a blinding ray of light shoots forth!", ch,NULL,NULL,TO_ROOM);
        send_to_char( "You raise your hand and a blinding ray of light shoots forth!\n\r", ch);
    }

    if (IS_GOOD(victim))
    {
	act("$n seems unharmed by the light.",victim,NULL,victim,TO_ROOM);
	send_to_char("The light seems powerless to affect you.\n\r",victim);
	return;
    }

    dam  = dice( level, 5 );
    dam += 20;
    if ( saves_spell( level, victim,DAM_HOLY) )
        dam /= 2;
    if(!IS_EVIL(victim))
	dam /= 2;
    damage_old( ch, victim, dam, sn, DAM_HOLY ,TRUE);
    spell_blindness(gsn_blindness, 3 * level / 4, ch, (void *) victim,TARGET_CHAR);
}

void spell_recharge( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    int chance, percent;

    if (obj->item_type != ITEM_WAND && obj->item_type != ITEM_STAFF)
    {
	send_to_char("That item does not carry charges.\n\r",ch);
	return;
    }

    if (obj->value[3] >= 3 * level / 2)
    {
	send_to_char("Your skills are not great enough for that.\n\r",ch);
	return;
    }

    if (obj->value[1] == 0)
    {
	send_to_char("That item has already been recharged once.\n\r",ch);
	return;
    }

    chance = 40 + 2 * level;

    chance -= obj->value[3]; /* harder to do high-level spells */
    chance -= (obj->value[1] - obj->value[2]) *
	      (obj->value[1] - obj->value[2]);

    chance = UMAX(level/2,chance);

    percent = number_percent();

    if (percent < chance / 2)
    {
	act("$p glows softly.",ch,obj,NULL,TO_CHAR);
	act("$p glows softly.",ch,obj,NULL,TO_ROOM);
	obj->value[2] = UMAX(obj->value[1],obj->value[2]);
	obj->value[1] = 0;
	return;
    }

    else if (percent <= chance)
    {
	int chargeback,chargemax;

	act("$p glows softly.",ch,obj,NULL,TO_CHAR);
	act("$p glows softly.",ch,obj,NULL,TO_CHAR);

	chargemax = obj->value[1] - obj->value[2];

	if (chargemax > 0)
	    chargeback = UMAX(1,chargemax * percent / 100);
	else
	    chargeback = 0;

	obj->value[2] += chargeback;
	obj->value[1] = 0;
	return;
    }

    else if (percent <= UMIN(95, 3 * chance / 2))
    {
	send_to_char("Nothing seems to happen.\n\r",ch);
	if (obj->value[1] > 1)
	    obj->value[1]--;
	return;
    }

    else /* whoops! */
    {
	act("$p glows brightly and explodes!",ch,obj,NULL,TO_CHAR);
	act("$p glows brightly and explodes!",ch,obj,NULL,TO_ROOM);
	extract_obj(obj);
    }
}

void spell_refresh( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    victim->move = UMIN( victim->move + level, victim->max_move );
    if (victim->max_move == victim->move)
        send_to_char("You feel fully refreshed!\n\r",victim);
    else
        send_to_char( "You feel less tired.\n\r", victim );
    if ( ch != victim )
        send_to_char( "Ok.\n\r", ch );
    return;
}

void spell_remove_curse( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    bool found = FALSE;

    /* do object cases first */
    if (target == TARGET_OBJ)
    {
	obj = (OBJ_DATA *) vo;

	if (IS_OBJ_STAT(obj,ITEM_NODROP) || IS_OBJ_STAT(obj,ITEM_NOREMOVE))
	{
	    if (!saves_dispel(level + 2,obj->level,0))
	    {
		remove_bit(&obj->extra_flags,ITEM_NODROP);
		remove_bit(&obj->extra_flags,ITEM_NOREMOVE);
		act("$p glows blue.",ch,obj,NULL,TO_ALL);
		return;
	    }

	    act("The curse on $p is beyond your power.",ch,obj,NULL,TO_CHAR);
	    return;
	}
	act("There doesn't seem to be a curse on $p.",ch,obj,NULL,TO_CHAR);
	return;
    }

    /* characters */
    victim = (CHAR_DATA *) vo;

    if (check_dispel(level,victim,gsn_curse))
    {
	send_to_char("You feel better.\n\r",victim);
	act("$n looks more relaxed.",victim,NULL,NULL,TO_ROOM);
    }

   for (obj = victim->carrying; (obj != NULL && !found); obj = obj->next_content)
   {
        if ((IS_OBJ_STAT(obj,ITEM_NODROP) || IS_OBJ_STAT(obj,ITEM_NOREMOVE)))
        {   /* attempt to remove curse */
            if (!saves_dispel(level,obj->level,0))
            {
                found = TRUE;
                remove_bit(&obj->extra_flags,ITEM_NODROP);
                remove_bit(&obj->extra_flags,ITEM_NOREMOVE);
                act("Your $p glows blue.",victim,obj,NULL,TO_CHAR);
                act("$n's $p glows blue.",victim,obj,NULL,TO_ROOM);
            }
         }
    }
}

void spell_sanctuary( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_SANCTUARY) )
    {
	if (victim == ch)
	    send_to_char("You are already in sanctuary.\n\r",ch);
	else
	    act("$N is already in sanctuary.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if ((ch != victim) && (ch->class == CLASS_ZEALOT))
    {
	send_to_char("You can only grant this supplications on yourself.\n\r",ch);
        return;
    }

    if ((ch != victim) && (ch->class == CLASS_PALADIN))
    {
        send_to_char("You can only grant this supplications on yourself.\n\r",ch);
        return;
    }

    if ((50 < get_skill(ch,gsn_shroud)) && !cabal_down(ch,CABAL_EMPIRE) && IS_EVIL(ch)) 
    {
	init_affect(&af);
    	af.where     	= TO_AFFECTS;
		af.aftype 	= AFT_POWER;
    	af.type      	= gsn_shroud;
    	af.level     	= level;
    	af.duration  	= level/6;
    	af.location  	= APPLY_NONE;
    	af.modifier  	= 0;
		char msg_buf[MSL];
		sprintf(msg_buf,"reduces damage taken by %d%%",DAM_REDUX_BLACK_SANC);
		af.affect_list_msg = str_dup(msg_buf);
    	af.bitvector 	= AFF_SANCTUARY;
    	affect_to_char( victim, &af );
    	send_to_char( "You reverse the polarity of your sanctuary to negative energy.\n\r",ch);
    	act( "$n is surrounded by a black aura.", victim, NULL, NULL, TO_ROOM );
    	send_to_char( "You are surrounded by a black aura.\n\r", victim );
    } else {
	init_affect(&af);
    	af.where     	= TO_AFFECTS;
		af.aftype	= get_spell_aftype(ch);
    	af.type      	= sn;
    	af.level     	= level;
    	af.duration  	= level/6;
    	af.location  	= APPLY_NONE;
		char msg_buf[MSL];
		sprintf(msg_buf,"reduces damage taken by %d%%",DAM_REDUX_SANC);
		af.affect_list_msg = str_dup(msg_buf);
    	af.modifier  	= 0;
    	af.bitvector 	= AFF_SANCTUARY;
    	affect_to_char( victim, &af );
    	act( "$n is surrounded by a white aura.", victim, NULL, NULL, TO_ROOM );
    	send_to_char( "You are surrounded by a white aura.\n\r", victim );
    }
    return;
}

void spell_shield( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You are already shielded from harm.\n\r",ch);
	else
	  act("$N is already protected by a shield.",ch,NULL,victim,TO_CHAR);
	return;
    }
	init_affect(&af);
    af.where     = TO_AFFECTS;
	af.aftype	 = get_spell_aftype(ch);
    af.type      = sn;
    af.level     = level;
    af.duration  = 14 + level;
    af.affect_list_msg = str_dup("reduces damage taken by 5%");
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n is surrounded by a force shield.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are surrounded by a force shield.\n\r", victim );
    return;
}



void spell_shocking_grasp(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const int dam_each[] =
    {
	 0,
	 0,  0,  0,  16, 18,	 20, 20, 25, 29, 33,
	36, 39, 39, 39, 40,	40, 41, 41, 42, 42,
	43, 43, 44, 44, 45,	45, 46, 46, 47, 47,
	48, 48, 49, 49, 50,	50, 51, 51, 52, 52,
	53, 53, 54, 54, 55,	55, 56, 56, 57, 57
    };
    int dam;

    level	= UMIN(level, sizeof(dam_each)/sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( level, victim,DAM_LIGHTNING) )
	dam /= 2;
    damage_old( ch, victim, dam, sn, DAM_LIGHTNING ,TRUE);
    return;
}



void spell_sleep( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_SLEEP)
    ||   (IS_NPC(victim) && IS_SET(victim->act,ACT_UNDEAD))
    ||   (level + 2) < victim->level
    ||   saves_spell( level-1, victim,DAM_CHARM)
    ||   (!(str_cmp(race_table[victim->race].name,"lich"))))
	{
		act("$N resisted your sleep spell.",ch,0,victim,TO_CHAR);
		return;
	}

    if (IS_CABAL_OUTER_GUARD(victim))
    {
	act("$N resisted your sleep spell.",ch,0,victim,TO_CHAR);
	return;
    }

    init_affect(&af);
    af.where     = TO_AFFECTS;
    af.aftype    = AFT_SPELL;
    af.type      = sn;
    af.level     = level;
    af.duration  = 5;
    af.location  = APPLY_NONE;
    af.affect_list_msg = str_dup("afflicts sleep");
    af.modifier  = 0;
    af.bitvector = AFF_SLEEP;
    affect_join( victim, &af );

    if ( IS_AWAKE(victim) )
    {
	send_to_char( "You feel very sleepy ..... zzzzzz.\n\r", victim );
	act( "$n goes to sleep.", victim, NULL, NULL, TO_ROOM );
	victim->position = POS_SLEEPING;
    }
    return;
}

void spell_slow( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    char buf[MAX_STRING_LENGTH];

	if(!trusts(ch,victim) && ch->ghost > 0)
    	return send_to_char("You cannot cast that aggressively as a ghost!",ch);
		
	if(!trusts(ch,victim) && victim->ghost > 0)
    	return send_to_char("You cannot cast that aggressively at a ghost!",ch);
		
    if(victim == NULL && ch->fighting == NULL)
  	victim = ch;

    if(ch->fighting != NULL)
	victim = ch->fighting;

    if(victim!=ch && (victim!=ch->fighting || !trusts(ch,victim))) {
		sprintf(buf,"Die, %s, you sorcerous dog!",ch->name);
		do_myell(victim,buf);
		multi_hit(victim,ch,TYPE_UNDEFINED);
     }

    if ( is_affected( victim, sn ) || IS_AFFECTED(victim,AFF_SLOW))
    {
	if (victim == ch)
            send_to_char("You can't move any slower!\n\r",ch);
        else
            act("$N can't get any slower than that.",ch,NULL,victim,TO_CHAR);
        return;
    }

    if ((victim != ch) &&
    (saves_spell(level,victim,DAM_OTHER)
    ||  IS_SET(victim->imm_flags,IMM_MAGIC)))
    {
	if (victim != ch)
            send_to_char("Nothing seemed to happen.\n\r",ch);
        send_to_char("You feel momentarily lethargic.\n\r",victim);
        return;
    }

    if (IS_AFFECTED(victim,AFF_HASTE))
    {
    	affect_strip(victim,skill_lookup("haste"));
        if(IS_AFFECTED(victim,AFF_HASTE))
            REMOVE_BIT(victim->affected_by,AFF_HASTE);
        send_to_char("You start moving at normal speed again.\n\r",victim);
        act("$n starts moving at normal speed again.",victim,NULL,NULL,TO_ROOM);
        return;
    }

    init_affect(&af);
    af.where     	= TO_AFFECTS;
    af.aftype    	= AFT_SPELL;
    af.type      	= sn;
    af.level     	= level;
    af.duration  	= level/4;
    af.location  	= APPLY_DEX;
    af.modifier  	= -1 - (level >= 18) - (level >= 25) - (level >= 32);
    af.bitvector	= AFF_SLOW;
	char msg_buf[MSL];
	sprintf(msg_buf,"reduces likelihood of attacks and decreases dexterity by %d",af.modifier);
	af.affect_list_msg = str_dup(msg_buf);
    affect_to_char( victim, &af );

    send_to_char( "You feel yourself moving more slowly.\n\r", victim );
    act("$n is moving more slowly.",victim,NULL,NULL,TO_ROOM);

    if(!trusts(ch,victim))
    	multi_hit(victim,ch,TYPE_UNDEFINED);

    return;
}




void spell_stone_skin( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( ch, sn ) )
    {
	if (victim == ch)
	  send_to_char("Your skin is already as hard as a rock.\n\r",ch);
	else
	  act("$N is already as hard as can be.",ch,NULL,victim,TO_CHAR);
	return;
    }

	init_affect(&af);
    af.where     = TO_AFFECTS;
	af.aftype	 = get_spell_aftype(ch);
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.affect_list_msg = str_dup("reduces damage taken by 20%");
    af.bitvector = 0;
    affect_to_char( victim, &af );

    act( "$n's skin turns to stone.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "Your skin turns to stone.\n\r", victim );
    return;
}



void spell_summon( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim;

    if ( ( victim = get_char_world( ch, target_name ) ) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   (!IS_NPC(victim) && victim->in_room->area != ch->in_room->area)
    ||   IS_SET(ch->in_room->room_flags, ROOM_SAFE)
    ||   (IS_NPC(victim) && IS_AFFECTED(victim,AFF_CHARM) && victim->in_room->area != ch->in_room->area)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(ch->in_room->room_flags, ROOM_NO_SUMMON)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_SUMMON)
    ||   (IS_NPC(victim) && IS_SET(victim->act,ACT_AGGRESSIVE))
    ||   victim->level >= (level + 5)
    ||   (!IS_NPC(victim) && victim->level >= LEVEL_IMMORTAL)
    ||   victim->fighting != NULL
    ||   (IS_EXPLORE(ch->in_room))
    ||   (IS_EXPLORE(victim->in_room))
    ||   (IS_NPC(victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
    ||	 (IS_NPC(victim) && victim->pIndexData->pShop != NULL)
    ||   (IS_NPC(victim) && victim->spec_fun != NULL)
    ||   (IS_NPC(victim) && IS_SET(victim->act,ACT_AGGRESSIVE))
    ||   (!IS_NPC(victim) && !can_pk(ch, victim) && IS_SET(victim->act,PLR_NOSUMMON))
    ||   (saves_spell( level, victim,DAM_OTHER)) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    act( "$n disappears suddenly.", victim, NULL, NULL, TO_ROOM );
    char_from_room( victim );
    char_to_room( victim, ch->in_room );
    act( "$n arrives suddenly.", victim, NULL, NULL, TO_ROOM );
    act( "$n has summoned you!", ch, NULL, victim,   TO_VICT );
    do_look( victim, "auto" );
    return;
}

void spell_teleport( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo; 
    ROOM_INDEX_DATA *pRoomIndex;


    if ( victim->in_room == NULL
    || (!IS_NPC(ch) && IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL))
    || ( victim != ch && IS_SET(victim->imm_flags,IMM_SUMMON))
    || ( !IS_NPC(ch) && victim->fighting != NULL )
    || ( victim != ch))
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    for ( ; ; )
    {
    pRoomIndex = get_random_room(victim);
    if(!IS_SET(pRoomIndex->room_flags, ROOM_NO_RECALL) && !IS_EXPLORE(pRoomIndex) && !pRoomIndex->cabal)
	break;
    }
    if (victim != ch)
	send_to_char("You have been teleported!\n\r",victim);

    act( "$n vanishes!", victim, NULL, NULL, TO_ROOM );
    char_from_room( victim );
    char_to_room( victim, pRoomIndex );
    act( "$n slowly fades into existence.", victim, NULL, NULL, TO_ROOM );
    do_look( victim, "auto" );
    return;
}

void spell_turn_undead( int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
//    OBJ_DATA *obj;
    int dam = 0;
    int value, count, num;
    CHAR_DATA *follower;

    follower = NULL; /* follower, count and num used for evil clerics */
    count = 0;
    num = 0;

    if (!IS_EVIL(ch))
    {
    act("$n raises $s hands and calls upon the gods to destroy the unholy.",ch,NULL,NULL,TO_ROOM);
    act("You raise your hands and call upon the gods to destroy the unholy.", ch,NULL,NULL,TO_CHAR);
    if (!IS_GOOD(ch))
	level -= 3;
    }
    else
    {
	act("$n turns $s unholy influence upon the room.",ch,0,0,TO_ROOM);
	send_to_char("You turn your unholy influence upon those in the room.\n\r",ch);
    }


/*
    if (!IS_NPC(ch) && ch->pcdata->special == SPEC_PALADIN_HUNTER)
		level += number_range(2,level/7);
    */

    if (!IS_EVIL(ch))
    {
	    for (victim = ch->in_room->people; victim != NULL; victim = v_next)
	    {
			v_next = victim->next_in_room;
			if (is_same_group(victim,ch) || !IS_SET(victim->act,ACT_UNDEAD) )
				continue;
			value = UMAX(1, level - victim->level + 10);
			value = UMAX(13, value - 4);
			if (IS_GOOD(ch))
				dam = dice(level, value)/2;
			else dam = 3 * dam / 4;

			damage_old(ch,victim,dam,skill_lookup("turn undead"),DAM_HOLY,TRUE);
			if (!IS_CABAL_GUARD(victim)) {
				if (victim->master) {
					//hack for fleeing, remove the charm effect
					CHAR_DATA *master = victim->master;
					victim->master = NULL;
					REMOVE_BIT(victim->affected_by,AFF_CHARM);
					do_flee(victim,"");
					victim->master = master;
					SET_BIT(victim->affected_by,AFF_CHARM);
				}
				else do_flee(victim,"");
			}
		}
		return;
    }
    else
    {
    for (follower = char_list; follower != NULL; follower = follower->next)
	if ( (follower->master == ch) && IS_SET(follower->act,ACT_UNDEAD)
	&& follower != ch)
		{
		num++;
		count += follower->level;
		}

    for (victim = ch->in_room->people; victim != NULL; victim = v_next)
    {
	v_next = victim->next_in_room;
	if (is_same_group(victim,ch) || !IS_SET(victim->act,ACT_UNDEAD) )
		continue;
	if (IS_AFFECTED(victim,AFF_CHARM))
		continue;

	if ( (ch->level*3 < count*2 )
	|| (ch->level < 20 && num > 0)
	|| (ch->level < 40 && num > 1)
	|| (ch->level < 51 && num > 1)
	|| (ch->level < (victim->level + 3))
	|| saves_spell(ch->level,victim,DAM_OTHER)
	|| !IS_NPC(victim))
	{
	act("You attempt to control $N but do not have the influence.",ch,0,victim,TO_CHAR);
	continue;
	}
	else
	{
	act("$n stares in hatred for a moment then suddenly becomes very subdued.",victim,0,0,TO_NOTVICT);
	stop_fighting(victim,TRUE);
	victim->master = ch;
	victim->leader = ch;
	SET_BIT(victim->affected_by,AFF_CHARM);
	REMOVE_BIT(victim->act,ACT_AGGRESSIVE);
	act("$N now follows you.",ch,0,victim,TO_CHAR);
	num++;
	count += victim->level;
	}
    }
    }
    return;
}


void spell_ventriloquate( int sn, int level, CHAR_DATA *ch,void *vo,int target)
{
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char speaker[MAX_INPUT_LENGTH];
    CHAR_DATA *vch;

    target_name = one_argument( target_name, speaker );

    sprintf( buf1, "%s says '%s'.\n\r",              speaker, target_name );
    sprintf( buf2, "Someone makes %s say '%s'.\n\r", speaker, target_name );
    buf1[0] = UPPER(buf1[0]);

    for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room )
    {
	if (!is_exact_name( speaker, vch->name) && IS_AWAKE(vch))
	    send_to_char( saves_spell(level,vch,DAM_OTHER) ? buf2 : buf1, vch );
    }

    return;
}



void spell_weaken( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) || saves_spell( level, victim,DAM_OTHER) )
	{
		send_to_char("You failed to weaken them.\n\r",ch);
		return;
	}

	init_affect(&af);
    af.where     = TO_AFFECTS;
	af.aftype	 = get_spell_aftype(ch);
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 2;
    af.location  = APPLY_STR;
    af.modifier  = -1 * (level / 8);
    af.bitvector = AFF_WEAKEN;
    affect_to_char( victim, &af );
    send_to_char( "You feel your strength slip away.\n\r", victim );
    act("$n looks tired and weak.",victim,NULL,NULL,TO_ROOM);
    return;
}

/* RT recall spell is back */

void spell_word_of_recall( int sn, int level, CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    ROOM_INDEX_DATA *location;

    if (IS_NPC(victim))
      return;

    if (!is_same_group(ch,victim))
	{
	send_to_char("You can only recall those in your own group.\n\r",ch);
	return;
	}

        if (affect_find(ch->affected,gsn_insect_swarm) != NULL)
        {
	send_to_char( "You attempt to transport yourself to safety but loose your concentration as you are bitten by yet another insect.\n\r", ch );
	return;
        }

    location = get_room_index(hometown_table[victim->hometown].recall);

    if (location == NULL)
    {
    	if ((location = get_room_index( ROOM_VNUM_TEMPLE)) == NULL)
    	{
	send_to_char("You are completely lost.\n\r",victim);
	return;
    	} 
    } 


/*    send_to_char("You utter the words, 'xafe ay candusarr'.\n\r",ch);
    act("$n utters the words, 'xafe ay candusarr'.",ch,NULL,NULL,TO_ROOM); */
    if ((IS_SET(victim->in_room->room_flags,ROOM_NO_RECALL)
	&& !IS_SET(victim->in_room->room_flags,ROOM_CONSECRATED))
	|| IS_AFFECTED(victim,AFF_CURSE) || is_affected(victim,gsn_aristaeia))
    {
	send_to_char("Spell failed.\n\r",victim);
	return;
    }

    if (victim->in_room->cabal != 0
        && victim->in_room->cabal != victim->cabal)
    {
	send_to_char("Spell failed.\n\r",victim);
	return;
    }


    if (victim->fighting != NULL)
	stop_fighting(victim,TRUE);

    ch->move /= 2;


    if (is_affected(ch,gsn_camouflage))
    {
affect_strip(ch,gsn_camouflage);
act("$n steps out from $s cover.",ch,0,0,TO_ROOM);
send_to_char("You step out from your cover.\n\r",ch);
    }

    act("$n disappears.",victim,NULL,NULL,TO_ROOM);
    char_from_room(victim);
    char_to_room(victim,location);
    act("$n appears in the room.",victim,NULL,NULL,TO_ROOM);
    do_look(victim,"auto");
}

/*
 * NPC spells.
 */
void spell_acid_breath( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam,hp_dam,dice_dam,hpch;

    act("$n spits acid at $N.",ch,NULL,victim,TO_NOTVICT);
    act("$n spits a stream of corrosive acid at you.",ch,NULL,victim,TO_VICT);
    act("You spit acid at $N.",ch,NULL,victim,TO_CHAR);

    hpch = ch->hit/9;
    dice_dam = dice(level,4);
    dam = hpch + dice_dam;
    if (dam > ch->hit)
	dam = ch->hit;
     hp_dam = 0;
/*
    hpch = UMAX(12,ch->hit);
    hp_dam = number_range(hpch/11 + 1, hpch/6);
    dice_dam = dice(level,16);

    dam = UMAX(hp_dam + dice_dam/10,dice_dam + hp_dam/10);
  */

    if (saves_spell(level,victim,DAM_ACID))
    {
	acid_effect(victim,level/2,dam/4,TARGET_CHAR);
	damage_old(ch,victim,dam/2,sn,DAM_ACID,TRUE);
    }
    else
    {
	acid_effect(victim,level,dam,TARGET_CHAR);
	damage_old(ch,victim,dam,sn,DAM_ACID,TRUE);
    }
}



void spell_fire_breath( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    CHAR_DATA *vch, *vch_next;
    int dam,hp_dam,dice_dam;
    int hpch;

    act("$n breathes forth a cone of fire.",ch,NULL,victim,TO_NOTVICT);
    act("$n breathes a cone of hot fire over you!",ch,NULL,victim,TO_VICT);
    act("You breath forth a cone of fire.",ch,NULL,NULL,TO_CHAR);

    hpch = ch->hit/9;
    dice_dam = dice(level,5);
    dam = hpch + dice_dam;
    if (dam > ch->hit)
	dam = ch->hit;

    hp_dam  = number_range( hpch/9+1, hpch/5 );
/*
    dice_dam = dice(level,20);

    dam = UMAX(hp_dam + dice_dam /10, dice_dam + hp_dam / 10);
*/

    fire_effect(victim->in_room,level,dam/2,TARGET_ROOM);

    for (vch = victim->in_room->people; vch != NULL; vch = vch_next)
    {
	vch_next = vch->next_in_room;
/*
	if (is_safe_spell(ch,vch,TRUE)
	||  (IS_NPC(vch) && IS_NPC(ch)
	&&   (ch->fighting != vch || vch->fighting != ch)))
	    continue;
*/
	if (is_same_group(vch,ch))
	    continue;
	if (IS_IMMORTAL(vch)) {
		continue;
	}
	if (vch == victim) /* full damage */
	{
	    if (saves_spell(level,vch,DAM_FIRE))
	    {
		fire_effect(vch,level/2,dam/4,TARGET_CHAR);
		damage_old(ch,vch,dam/2,sn,DAM_FIRE,TRUE);
	    }
	    else
	    {
		fire_effect(vch,level,dam,TARGET_CHAR);
		damage_old(ch,vch,dam,sn,DAM_FIRE,TRUE);
	    }
	}
	else /* partial damage */
	{
	    if (saves_spell(level - 2,vch,DAM_FIRE))
	    {
		fire_effect(vch,level/4,dam/8,TARGET_CHAR);
		damage_old(ch,vch,dam/4,sn,DAM_FIRE,TRUE);
	    }
	    else
	    {
		fire_effect(vch,level/2,dam/4,TARGET_CHAR);
		damage_old(ch,vch,dam/2,sn,DAM_FIRE,TRUE);
	    }
	}
    }
}

void spell_frost_breath( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    CHAR_DATA *vch, *vch_next;
    int dam,hp_dam,dice_dam, hpch;

    act("$n breathes out a freezing cone of frost!",ch,NULL,victim,TO_NOTVICT);
    act("$n breathes a freezing cone of frost over you!",
	ch,NULL,victim,TO_VICT);
    act("You breath out a cone of frost.",ch,NULL,NULL,TO_CHAR);

    hpch = ch->hit/9;
    dice_dam = dice(level,5);
    dam = hpch + dice_dam;
    if (dam > ch->hit)
	dam = ch->hit;
    hp_dam = 0;
/*
    hpch = UMAX(12,ch->hit);
    hp_dam = number_range(hpch/11 + 1, hpch/6);
    dice_dam = dice(level,16);

    dam = UMAX(hp_dam + dice_dam/10,dice_dam + hp_dam/10);
*/
    cold_effect(victim->in_room,level,dam/2,TARGET_ROOM);

    for (vch = victim->in_room->people; vch != NULL; vch = vch_next)
    {
	vch_next = vch->next_in_room;
/*
	if (is_safe_spell(ch,vch,TRUE)
	||  (IS_NPC(vch) && IS_NPC(ch)
	&&   (ch->fighting != vch || vch->fighting != ch)))
	    continue;
*/
	if (is_safe(ch,vch) || vch == ch)
		continue;

	if (vch == victim) /* full damage */
	{
	    if (saves_spell(level,vch,DAM_COLD))
	    {
		cold_effect(vch,level/2,dam/4,TARGET_CHAR);
		damage_old(ch,vch,dam/2,sn,DAM_COLD,TRUE);
	    }
	    else
	    {
		cold_effect(vch,level,dam,TARGET_CHAR);
		damage_old(ch,vch,dam,sn,DAM_COLD,TRUE);
	    }
	}
	else
	{
	    if (saves_spell(level - 2,vch,DAM_COLD))
	    {
		cold_effect(vch,level/4,dam/8,TARGET_CHAR);
		damage_old(ch,vch,dam/4,sn,DAM_COLD,TRUE);
	    }
	    else
	    {
		cold_effect(vch,level/2,dam/4,TARGET_CHAR);
		damage_old(ch,vch,dam/2,sn,DAM_COLD,TRUE);
	    }
	}
    }
}


void spell_gas_breath( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam,hp_dam,dice_dam,hpch;

    act("$n breathes out a cloud of poisonous gas!",ch,NULL,NULL,TO_ROOM);
    act("You breath out a cloud of poisonous gas.",ch,NULL,NULL,TO_CHAR);

    hpch = ch->hit/9;
    dice_dam = dice(level,5);
    dam = hpch + dice_dam;
    if (dam > ch->hit)
	dam = ch->hit;


    hp_dam = 0;
/*
    hpch = UMAX(16,ch->hit);
    hp_dam = number_range(hpch/15+1,8);
    dice_dam = dice(level,12);

    dam = UMAX(hp_dam + dice_dam/10,dice_dam + hp_dam/10);
*/
    poison_effect(ch->in_room,level,dam,TARGET_ROOM);

    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
	vch_next = vch->next_in_room;
	if (is_same_group(ch,vch))
	    continue;
	if (is_safe(vch, ch))
	    continue;

	if (saves_spell(level,vch,DAM_POISON))
	{
	    poison_effect(vch,level/2,dam/4,TARGET_CHAR);
	    damage_old(ch,vch,dam/2,sn,DAM_POISON,TRUE);
	}
	else
	{
	    poison_effect(vch,level,dam,TARGET_CHAR);
	    damage_old(ch,vch,dam,sn,DAM_POISON,TRUE);
	}
    }
}

void spell_lightning_breath(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam,hp_dam,dice_dam,hpch;

    act("$n breathes a bolt of lightning at $N.",ch,NULL,victim,TO_NOTVICT);
    act("$n breathes a bolt of lightning at you!",ch,NULL,victim,TO_VICT);
    act("You breathe a bolt of lightning at $N.",ch,NULL,victim,TO_CHAR);

    hpch = ch->hit/9;
    dice_dam = dice(level,5);
    dam = hpch + dice_dam;
    if (dam > ch->hit)
	dam = ch->hit;
     hp_dam = 0;
/*
    hpch = UMAX(10,ch->hit);
    hp_dam = number_range(hpch/9+1,hpch/5);
    dice_dam = dice(level,20);

    dam = UMAX(hp_dam + dice_dam/10,dice_dam + hp_dam/10);
*/
    if (saves_spell(level,victim,DAM_LIGHTNING))
    {
	shock_effect(victim,level/2,dam/4,TARGET_CHAR);
	damage_old(ch,victim,dam/2,sn,DAM_LIGHTNING,TRUE);
    }
    else
    {
	shock_effect(victim,level,dam,TARGET_CHAR);
	damage_old(ch,victim,dam,sn,DAM_LIGHTNING,TRUE);
    }
}

/*
 * Spells for mega1.are from Glop/Erkenbrand.
 */
void spell_general_purpose(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = number_range( 25, 100 );
    if ( saves_spell( level, victim, DAM_PIERCE) )
        dam /= 2;
    damage_old( ch, victim, dam, sn, DAM_PIERCE ,TRUE);
    return;
}

void spell_high_explosive(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = number_range( 30, 120 );
    if ( saves_spell( level, victim, DAM_PIERCE) )
        dam /= 2;
    damage_old( ch, victim, dam, sn, DAM_PIERCE ,TRUE);
    return;
}



/* This is a big block of elementalist spells all at once */

void spell_windwall(int sn, int level, CHAR_DATA *ch,void *vo, int target)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
	char buf[MAX_STRING_LENGTH];
    int dam, check;
    AFFECT_DATA af;

	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
    af.location = APPLY_HITROLL;
    af.modifier = -3;
    af.duration = number_range(0,1);
    af.level = level;
    af.type = sn;
    af.bitvector = AFF_BLIND;

    send_to_char("You raise a violent wall of wind to strike your foes.\n\r",ch);
    act("$n raises a violent wall of wind, sending debri flying!",ch,NULL,NULL,TO_ROOM);
    dam = dice(level, 6);
    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
	vch_next = vch->next_in_room;
        if (is_same_group(vch,ch))
            continue;
	if (is_safe(ch,vch))
 	    continue;
	if (is_same_cabal(ch,vch))
	    continue;
	if (check_immune(vch, DAM_WIND) == IS_IMMUNE)
	    continue;
	if (!IS_NPC(ch) && !IS_NPC(vch)
	&& (ch->fighting == NULL || vch->fighting == NULL))
	{
		switch(number_range(0,2))
		{
		case (0):
		case (1):
		sprintf(buf,"Die, %s you sorcerous dog!",PERS(ch,vch));
		break;
		case (2):
		sprintf(buf,"Help! %s is casting a spell on me!",PERS(ch,vch));
		}
	if (vch != ch)
		do_myell(vch,buf);
	}

        check = skill_lookup ("windwall");
        if ((number_range(0,1) == 0)
        && !saves_spell(level,vch,DAM_BASH)
        && !is_affected(vch, check))
        {
            act("$n appears blinded by the debris.",vch,NULL,NULL,TO_ROOM);
            affect_to_char(vch,&af);
        }
        damage_old(ch,vch,dam,sn,DAM_WIND,TRUE);
	  if (number_percent()>50)
		{
            act("$n is thrown wildly to the ground by the air blast!",vch,NULL,NULL,TO_ROOM);
            send_to_char("You are thrown down by the air blast!\n\r",vch);
            affect_strip(vch,skill_lookup("fly"));
		WAIT_STATE(vch, PULSE_VIOLENCE);
		}
    }
    return;
}


/* The summon elemental spells for elementalists. Fun. */
void spell_summon_fire_elemental(int sn, int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *elemental;
    AFFECT_DATA af;
    int count;
    CHAR_DATA *check;

    if (is_affected(ch,sn))
    {
        send_to_char("You can't call upon a fire elemental yet.\n\r",ch);
        return;
    }
    count = 0;

    for (check = char_list; check != NULL; check = check->next)
    {
    if (IS_NPC(check))
        if ( (check->master == ch)
        && ( (check->pIndexData->vnum == MOB_VNUM_FIRE_ELEMENTAL)
          || (check->pIndexData->vnum  == MOB_VNUM_AIR_ELEMENTAL)
          || (check->pIndexData->vnum  == MOB_VNUM_EARTH_ELEMENTAL)
          || (check->pIndexData->vnum  == MOB_VNUM_WATER_ELEMENTAL) ) )
            count++;
    }
    if ( (count == 1 && ch->level < 30) || (count == 2 && ch->level < 37)
      || (count == 4) )
    {
        send_to_char("You already control as many elementals as you can.\n\r",ch);
        return;
    }

    if ( (ch->in_room->sector_type == SECT_WATER_SWIM)
      || (ch->in_room->sector_type == SECT_WATER_NOSWIM) )
    {
        send_to_char("There is too much water here to summon a fire elemental.\n\r",ch);
        return;
    }
    if ((weather_info.sky > SKY_CLOUDY) && (!IS_SET(ch->in_room->room_flags,ROOM_INDOORS)))
    {

        send_to_char("The weather is too wet to summon a fire elemental.\n\r",ch);
        return;
    }

    elemental = create_mobile(get_mob_index(MOB_VNUM_FIRE_ELEMENTAL) );
    elemental->level = 51;
    elemental->max_hit = ch->max_hit-1000;
    elemental->hit = elemental->max_hit;
    elemental->damroll += 70;
    elemental->alignment = ch->alignment;
	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
    af.type = sn;
    af.location = 0;
    af.level = level;
    af.modifier = 0;
    af.affect_list_msg = str_dup("restricts the summoning of a fire elemental");
    af.duration = 15;
    af.bitvector = 0;
    affect_to_char(ch,&af);
  char_to_room(elemental,ch->in_room);

    act("$n arrives in a column of searing flames.",elemental,NULL,NULL,TO_ROOM);

    if (number_percent() > ch->pcdata->learned[sn])
    {
    act("$n says, 'How dare you call me!?!",elemental,NULL,NULL,TO_ROOM);
    multi_hit(elemental,ch,TYPE_UNDEFINED);
    return;
    }
    else
    {
        add_follower(elemental,ch);
        elemental->leader = ch;
        SET_BIT(elemental->affected_by, AFF_CHARM);
    }
    return;
}

void spell_summon_water_elemental(int sn, int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *elemental;
    AFFECT_DATA af;
    int count;
    CHAR_DATA *check;

    if (is_affected(ch,sn))
    {
        send_to_char("You can't call upon a water elemental yet.\n\r",ch);
        return;
    }
    count = 0;
    for (check = char_list; check != NULL; check = check->next)
    {
    if (IS_NPC(check))
        if ( (check->master == ch)
        && ( (check->pIndexData->vnum == MOB_VNUM_FIRE_ELEMENTAL)
          || (check->pIndexData->vnum  == MOB_VNUM_AIR_ELEMENTAL)
          || (check->pIndexData->vnum  == MOB_VNUM_EARTH_ELEMENTAL)
          || (check->pIndexData->vnum  == MOB_VNUM_WATER_ELEMENTAL) ) )
            count++;
    }

    if ( (count == 1 && ch->level < 30) || (count == 2 && ch->level < 37)
      || (count == 4) )
    {
        send_to_char("You already control as many elementals as you can.\n\r",ch);
        return;
    }

    if ( (ch->in_room->sector_type != SECT_WATER_SWIM)
      && (ch->in_room->sector_type != SECT_WATER_NOSWIM) )
    {
        send_to_char("There is not enough water here to summon a water elemental.\n\r",ch);
        return;
    }


    elemental = create_mobile(get_mob_index(MOB_VNUM_WATER_ELEMENTAL) );

    elemental->level = 51;
    elemental->max_hit = ch->max_hit + dice(level,10);
    elemental->hit = elemental->max_hit;
    elemental->damroll += 45;
    elemental->alignment = ch->alignment;
	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
    af.type = sn;
    af.location = 0;
    af.level = level;
    af.modifier = 0;
    af.duration = 15;
    af.affect_list_msg = str_dup("restricts the summoning of a water elemental");
    af.bitvector = 0;
    affect_to_char(ch,&af);

    char_to_room(elemental,ch->in_room);

    act("Water surges around you as $n arrives.",elemental,NULL,NULL,TO_ROOM);

    if (number_percent() > ch->pcdata->learned[sn])
    {
    act("You dare to call upon me!?!",elemental,NULL,NULL,TO_ROOM);
    multi_hit(elemental,ch,TYPE_UNDEFINED);
    return;
    }
    else
    {
        add_follower(elemental,ch);
        elemental->leader = ch;
        SET_BIT(elemental->affected_by, AFF_CHARM);
    }
    return;
}


void spell_summon_earth_elemental(int sn, int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *elemental;
    AFFECT_DATA af;
    int count, slevel;
    CHAR_DATA *check;

    slevel = get_skill(ch, sn);
    if (level == 70)
	slevel = 95;

    if (is_affected(ch,sn))
    {
        send_to_char("You can't call upon an earth elemental yet.\n\r",ch);
        return;
    }
    count = 0;
    for (check = char_list; check != NULL; check = check->next)
    {
    if (IS_NPC(check))
        if ( (check->master == ch)
        && ( (check->pIndexData->vnum == MOB_VNUM_FIRE_ELEMENTAL)
          || (check->pIndexData->vnum  == MOB_VNUM_AIR_ELEMENTAL)
          || (check->pIndexData->vnum  == MOB_VNUM_EARTH_ELEMENTAL)
          || (check->pIndexData->vnum  == MOB_VNUM_WATER_ELEMENTAL) ) )
            count++;
    }
    if ( (count == 1 && ch->level < 30) || (count == 2 && ch->level < 37)
      || (count == 4) || (count == 1 && level==70))
    {
        send_to_char("You already control as many elementals as you can.\n\r",ch);
        return;
    }

    if ( (ch->in_room->sector_type == SECT_WATER_SWIM)
      || (ch->in_room->sector_type == SECT_WATER_NOSWIM)
      || (ch->in_room->sector_type == SECT_AIR) )
    {
        send_to_char("You are not in an area with enough raw earth.\n\r",ch);
        return;
    }

    elemental = create_mobile(get_mob_index(MOB_VNUM_EARTH_ELEMENTAL) );
    elemental->level = 51;
    elemental->max_hit = ch->max_hit + 2000;
    elemental->hit = elemental->max_hit;
    elemental->damroll += 15;
    elemental->alignment = ch->alignment;
	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
    af.type = sn;
    af.location = 0;
    af.level = level;
    af.affect_list_msg = str_dup("restricts the summoning of an earth elemental");
    af.modifier = 0;
    af.duration = 15;
    af.bitvector = 0;
    affect_to_char(ch,&af);
  char_to_room(elemental,ch->in_room);

    act("The earth rumbles and $n bursts out from the ground.",elemental,NULL,NULL,TO_ROOM);

    if (number_percent() > slevel)
    {
    act("You dare to call upon me!?!",elemental,NULL,NULL,TO_ROOM);
    multi_hit(elemental,ch,TYPE_UNDEFINED);
    return;
    }
    else
    {
        add_follower(elemental,ch);
        elemental->leader = ch;
        SET_BIT(elemental->affected_by, AFF_CHARM);
    }
    return;
}

void spell_summon_air_elemental(int sn, int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *elemental;
    AFFECT_DATA af;
    int count;
    CHAR_DATA *check;

    if (is_affected(ch,sn))
    {
        send_to_char("You can't call upon an air elemental yet.\n\r",ch);
        return;
    }
    count = 0;
    for (check = char_list; check != NULL; check = check->next)
    {
    if (IS_NPC(check))
        if ( (check->leader == ch)
        && ( (check->pIndexData->vnum == MOB_VNUM_FIRE_ELEMENTAL)
          || (check->pIndexData->vnum  == MOB_VNUM_AIR_ELEMENTAL)
          || (check->pIndexData->vnum  == MOB_VNUM_EARTH_ELEMENTAL)
          || (check->pIndexData->vnum  == MOB_VNUM_WATER_ELEMENTAL) ) )
            count++;
    }
    if ( (count == 1 && ch->level < 30) || (count == 2 && ch->level < 37)
      || (count == 4) )
    {
        send_to_char("You already control as many elementals as you can.\n\r",ch);
        return;
    }

/* Put this back in if you want it...I found there weren't enough SECT_AIR
areas to really make this spell much use. (Ceran)
    if  (ch->in_room->sector_type != SECT_AIR)
    {
        send_to_char("You need to be in the air to find an air elemental.\n\r",ch);
        return;
    }
*/
    elemental = create_mobile(get_mob_index(MOB_VNUM_AIR_ELEMENTAL) );
    elemental->level = 51;
    elemental->max_hit = ch->max_hit + dice(level,10);
    elemental->hit = elemental->max_hit;
    elemental->damroll += 45;
    elemental->alignment = ch->alignment;

	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
    af.type = sn;
    af.location = 0;
    af.level = level;
    af.modifier = 0;
    af.affect_list_msg = str_dup("restricts the summoning of an air elemental");
    af.duration = 15;
    af.bitvector = 0;
    affect_to_char(ch,&af);
    char_to_room(elemental,ch->in_room);

    act("$n forms before you in a violent vortex of wind.",elemental,NULL,NULL,TO_ROOM);

    if (number_percent() > ch->pcdata->learned[sn])
    {
    act("$n says, 'You dare to call upon me!?!",elemental,NULL,NULL,TO_ROOM);
    multi_hit(elemental,ch,TYPE_UNDEFINED);
    return;
    }
    else
    {
        add_follower(elemental,ch);
        elemental->leader = ch;
        SET_BIT(elemental->affected_by, AFF_CHARM);
    }
    return;
}

void spell_earthmaw(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam, save_num;
    int count;
    if ( (ch->in_room->sector_type == SECT_AIR)
    || ( ch->in_room->sector_type == SECT_WATER_SWIM)
    || ( ch->in_room->sector_type == SECT_WATER_NOSWIM) )
    {
        send_to_char("You can't do that in this environment.\n\r",ch);
        return;
    }

    act("$n sunders the ground beneath $N.",ch,NULL,victim,TO_NOTVICT);
    act("$n sunders the ground beneath you.",ch,NULL,victim,TO_VICT);
    act("You sunder the ground beneath $N.",ch,NULL,victim,TO_CHAR);
    save_num = 0;
    if (check_spellcraft(ch,sn))
	dam = spellcraft_dam(level,10);
    else
    	dam = dice(level, 10);

    dam += dice(level,3);
    for (count = 0; count < 1; count++)
    {
        if (saves_spell(level+5,victim,DAM_BASH) )
            save_num++;
    }
    if (save_num == 0)
    {
        act("$n cries out as $s is crushed savagely within the rift!",victim,NULL,NULL,TO_ROOM);
        send_to_char("You fall into the rift and scream in agony as it crushes you!\n\r",victim);
        damage_old(ch,victim,dam,sn,DAM_EARTH,TRUE);
        return;
    }
    dam /= save_num * 2;
    damage_old(ch,victim,dam,sn,DAM_EARTH,TRUE);
    WAIT_STATE(ch,PULSE_VIOLENCE*2);
    return;
}

void spell_tsunami(int sn,int level,CHAR_DATA *ch,void *vo, int target)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam;

    if (ch->in_room->sector_type != SECT_WATER_SWIM
	&& ch->in_room->sector_type != SECT_WATER_NOSWIM)
    {
        send_to_char("You need to be on water to do that.\n\r",ch);
        return;
    }

    act("$n raises $s arms and causes the waters to rise up in violence.",ch,NULL,NULL,TO_ROOM);
    send_to_char("You call upon the water around you to surge at your foes.\n\r",ch);
    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
        vch_next = vch->next_in_room;
        if (is_same_group(vch,ch) )
            continue;
        dam = dice(level, 11);
        if(saves_spell(level,vch,DAM_DROWNING) )
            dam /= 2;
        if (saves_spell(level,vch,DAM_DROWNING) )
            dam /= 2;
        damage_old(ch,vch,dam,sn,DAM_DROWNING,TRUE);
    }
    act("$n's tsunami washes past and subsides.",ch,NULL,NULL,TO_ROOM);
    send_to_char("Your tsunami washes past and subsides.\n\r",ch);

    return;
}

void spell_drain(int sn, int level, CHAR_DATA *ch,void *vo, int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf;
    int drain,fail;

    if (obj->wear_loc != WEAR_NONE)
    {
        send_to_char("You can't do that on a worn object.\n\r",ch);
        return;
    }
    if (!is_set(&obj->extra_flags,ITEM_MAGIC))
    {
        send_to_char("That item is not magical.\n\r",ch);
        return;
    }
    switch (obj->item_type)
    {
        default: bug("Invalid item drain type: %d.",obj->item_type);
        drain = 1;
        break;

    case ITEM_LIGHT:        if (obj->value[2] == -1)     drain = 9;
    else drain = 4;
    break;
    case ITEM_SCROLL:
    case ITEM_WAND:
    case ITEM_STAFF:    drain = obj->value[0]/10;       break;
    case ITEM_WEAPON:
    drain = (obj->value[1] + obj->value[2])/3;
    break;
    case ITEM_TREASURE:     drain = 4;      break;
    case ITEM_ARMOR:        drain = 12;     break;
    case ITEM_POTION:       drain = 5;      break;
    }

    for (paf = obj->affected; paf != NULL; paf = paf->next)
    {
        if (paf->location > APPLY_NONE && paf->location < APPLY_SEX)
            drain += paf->modifier;
   else if ( (paf->location == APPLY_HITROLL) || (paf->location == APPLY_DAMROLL) )
            drain += paf->modifier;
        else continue;
    }

    drain *= dice(3,3);
    drain *= obj->level/2;

    drain = UMIN(drain,200);

    fail = 95 * ch->pcdata->learned[sn];
    act("$p vaporises in a flash of light!",ch,obj,NULL,TO_ROOM);
    if (number_percent() > fail)
    {
        act("$p vaporises in a flash of light but you fail to channel the energy.",ch,obj,NULL,TO_CHAR);
        extract_obj(obj);
    }
    act("$p vaporises in a flash of light and you feel the energy surge through you.",ch,obj,NULL,TO_CHAR);
    extract_obj(obj);
    ch->mana = UMIN(ch->mana + drain, ch->max_mana);
    return;
}

void spell_disenchant_weapon(int sn,int level, CHAR_DATA *ch,void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;
    int fail;
    int check;

    if (obj->item_type != ITEM_WEAPON)
    {
        send_to_char("But that item is not a weapon.\n\r",ch);
        return;
    }
    if (obj->wear_loc != -1)
    {
        send_to_char("The item must be in your inventory.\n\r",ch);
        return;
    }
    check = number_percent();

    fail = URANGE(5, 25 + level * ch->pcdata->learned[sn], 95);
    if ((check > 3/2*fail ) || (check > 94) )
    {
        act("$p shudders and explodes!",ch,obj,NULL,TO_ROOM);
        act("$p shudders and explodes!",ch,obj,NULL,TO_CHAR);
        extract_obj(obj);
        return;
    }
    if (check > fail)
    {
        act("$p glows faintly but then fades.",ch,obj,NULL,TO_ROOM);
        act("$p glows faintly but nothing happens.",ch,obj,NULL,TO_CHAR);
        return;
    }
    act("$p glows brightly and then fades to a dull lustre.",ch,obj,NULL,TO_ROOM);
    act("$p glows brightly and then fades to a dull lustre.",ch,obj,NULL,TO_CHAR);

	for (paf = obj->affected; paf != NULL; paf = paf_next)
	{
	    paf_next = paf->next;
	    free_affect(paf);
	}
	obj->affected = NULL;
        remove_bit(&obj->extra_flags, ITEM_GLOW);
        remove_bit(&obj->extra_flags, ITEM_HUM);
        remove_bit(&obj->extra_flags, ITEM_MAGIC);
        remove_bit(&obj->extra_flags, ITEM_INVIS);
        remove_bit(&obj->extra_flags, ITEM_NODROP);
        remove_bit(&obj->extra_flags, ITEM_NOREMOVE);
	return;
}

void spell_disenchant_armor(int sn,int level, CHAR_DATA *ch,void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;
    int fail;
    int check;

    if (obj->item_type != ITEM_ARMOR)
    {
        send_to_char("But that item is not a piece of armor.\n\r",ch);
        return;
    }
    if (obj->wear_loc != -1)
    {
        send_to_char("The item must be in your inventory.\n\r",ch);
        return;
    }
    check = number_percent();

    fail = URANGE(5, 25 + level * ch->pcdata->learned[sn], 95);
    if ((check > 3/2*fail ) || (check > 94) )
    {
        act("$p shudders and explodes!",ch,obj,NULL,TO_ROOM);
        act("$p shudders and explodes!",ch,obj,NULL,TO_CHAR);
        extract_obj(obj);
        return;
    }
    if (check > fail)
    {
        act("$p glows faintly but then fades.",ch,obj,NULL,TO_ROOM);
        act("$p glows faintly but nothing happens.",ch,obj,NULL,TO_CHAR);
        return;
    }
    act("$p glows brightly and then fades to a dull lustre.",ch,obj,NULL,TO_ROOM);
    act("$p glows brightly and then fades to a dull lustre.",ch,obj,NULL,TO_CHAR);

	for (paf = obj->affected; paf != NULL; paf = paf_next)
	{
	    paf_next = paf->next;
	    free_affect(paf);
	}
	obj->affected = NULL;

        remove_bit(&obj->extra_flags, ITEM_GLOW);
        remove_bit(&obj->extra_flags, ITEM_HUM);
        remove_bit(&obj->extra_flags, ITEM_MAGIC);
        remove_bit(&obj->extra_flags, ITEM_INVIS);
        remove_bit(&obj->extra_flags, ITEM_NODROP);
        remove_bit(&obj->extra_flags, ITEM_NOREMOVE);

    obj->enchanted = FALSE;
    return;
}

void spell_life_transfer(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
        CHAR_DATA *victim = (CHAR_DATA *) vo;
        int hp;

        if (victim == ch)
        {
        send_to_char("You can't grant life to yourself.\n\r",ch);
        return;
        }

        if (ch->hit < 10)
                hp = ch->hit;
        else hp = 10;
        act("$n transfers part of $s life to $N.",ch,0,victim,TO_NOTVICT);
        act("You transfer part of your life to $N.",ch,0,victim,TO_CHAR);
        act("$n transfers part of $s life to you.",ch,0,victim,TO_VICT);

        ch->hit -= hp;
	hp += ch->level/2;
	hp += number_range(0,ch->level);
        victim->hit = UMIN(victim->hit + hp, victim->max_hit);
        return;
}

void spell_spiritblade(int sn,int level, CHAR_DATA *ch,void *vo,int target)
{
        CHAR_DATA *victim = (CHAR_DATA *) vo;
        int dam;

        if (cabal_down(ch,CABAL_ARCANA))
		return;

        act("A shadowy blade appears above $n and strikes down!",victim,0,0,TO_ROOM);
        send_to_char("A shadowy blade manifests above you and suddenly descends!\n\r",victim);

        dam = dice(level, 5);
        if (saves_spell(level,victim,DAM_ENERGY) )
                dam /= 2;

        if (number_percent() < (level + ch->pcdata->learned[sn])/10 )
        {
        act("$n's spiritblade brutally cleaves $N!",ch,0,victim,TO_NOTVICT);
        act("Your spiritblade brutally cleaves $N!",ch,0,victim,TO_CHAR);
        act("$n's spiritblade brutally cleaves you!",ch,0,victim,TO_VICT);
        dam += dice(level,2);
        dam += dice(level/2, 2);
        }

        damage_old(ch,victim,dam,sn,DAM_ENERGY,TRUE);
        return;
}

void spell_iceball( int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
        CHAR_DATA *vch;
		char buf[MSL];

        int dam = number_range(125,135);

	    if (check_spellcraft(ch,sn))
			dam += spellcraft_dam(level,3);
	    else
			dam += dice(level,3);

		act("$n conjures an shattering iceball into the room!",ch,0,0,TO_ROOM);
		act("You conjure a shattering iceball into the room!",ch,0,0,TO_CHAR);
		
        for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
        {
        	if (is_same_group(vch,ch))
                continue;
		
			if (ch->cabal != 0 && is_same_cabal(vch, ch))
				continue;

			act("You are engulfed by shards from $n's iceball!",ch,0,vch,TO_VICT);
			switch(number_range(0,2))
			{
				case (0):
				case (1):
				sprintf(buf,"Die, %s you sorcerous dog!",PERS(ch,vch));
				break;
				case (2):
				sprintf(buf,"Help! %s is casting a spell on me!",PERS(ch,vch));
			}
			damage_old(ch,vch, (saves_spell(level,vch,DAM_COLD) ? dam/2 : dam), sn,DAM_COLD,TRUE);
        }
        return;
}

void spell_cone_of_cold(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;
	char buf[MAX_STRING_LENGTH];

        static const sh_int dam_each[] =
        {
        0,
        0, 0, 0, 0, 0,  0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,  0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,  0, 0, 0, 0, 0,
        0, 90, 92, 94, 96,      98, 100, 102, 105, 130,
        132, 134, 136, 138, 150,        152, 154, 156, 158, 170, 172
        };
        int dam, tmp_dam;

        act("$n creates a freezing blast of air!",ch,0,0,TO_ROOM);
        send_to_char("You draw heat from the room to create a blast of freezing air!\n\r",ch);

        level = UMIN(level,sizeof(dam_each)/sizeof(dam_each[0]) - 1);
        level = UMAX(0, level);
        dam = number_range(dam_each[level]/2, dam_each[level]*2);
        for (vch = ch->in_room->people;vch != NULL; vch = vch_next)
        {
        vch_next = vch->next_in_room;
        if (is_same_group(vch,ch))
                continue;
	if (!IS_NPC(ch) && !IS_NPC(vch)
	&& (ch->fighting == NULL || vch->fighting == NULL))
	{
		switch(number_range(0,2))
		{
		case (0):
		case (1):
		sprintf(buf,"Die, %s you sorcerous dog!",PERS(ch,vch));
		break;
		case (2):
		sprintf(buf,"Help! %s is casting a spell on me!",PERS(ch,vch));
		}
	if (vch != ch)
		do_myell(vch,buf);
	}

        if (saves_spell(level,vch,DAM_COLD) )
                tmp_dam = dam/2;
        else tmp_dam = dam;
        damage_old(ch,vch,tmp_dam,sn,DAM_COLD,TRUE);
        }
        return;
}

void spell_protective_shield(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
        AFFECT_DATA af;

        if (is_affected(ch,sn))
        {
        send_to_char("You are already protected by a protective shield.\n\r",ch);
        return;
        }
	init_affect(&af);
        af.where = TO_AFFECTS;
	af.aftype	 = get_spell_aftype(ch);
        af.location = 0;
        af.modifier = 0;
        af.duration = 3 + level/9;
        af.bitvector = 0;
        af.affect_list_msg = str_dup("grants immunity to most bashing attempts");
	af.type = sn;
        af.level = level;
        affect_to_char(ch,&af);
        act("$n is surrounded by a protective shield.",ch,0,0,TO_ROOM);
        send_to_char("You are surrounded by a protective shield.\n\r",ch);
        return;
}
void spell_wraithform(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
        AFFECT_DATA af;

        if (is_affected(ch,sn))
        {
        send_to_char("You are already partially in the Prime Negative Plane.\n\r",ch);
        return;
        }
	init_affect(&af);
        af.where = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
        af.location = 0;
        af.affect_list_msg = str_dup("reduces damage taken by 35%");
        af.modifier = 0;
        af.duration = 8;
	if (ch->race == race_lookup("lich"))
        	af.bitvector = AFF_FLYING;
	else
		af.bitvector = 0;
	  af.type = sn;
        af.level = level;
        affect_to_char(ch,&af);
	af.affect_list_msg = str_dup("grants immunity to most bashing attempts");
	affect_to_char(ch,&af);
	if (ch->race == race_lookup("lich"))
	{
		af.affect_list_msg = str_dup("grants flight");
		affect_to_char(ch,&af);
	}
        act("$n is briefly surrounded in darkness as he pulls himself into the Prime Negative Plane.",ch,0,0,TO_ROOM);
        send_to_char("You become briefly insubstantial as you pull yourself into the Prime Negative Plane.\n\r",ch);
        return;
}

void spell_timestop(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;
        AFFECT_DATA af;

        if (is_affected(ch,gsn_timestop_done))
        {
        send_to_char("The Gods prevent you from rupturing the fabric of time in such periodic bursts.\n\r",ch);
        return;
        }
	init_affect(&af);
        af.where = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
        af.location = 0;
        af.modifier = 0;
        af.duration = 24;
        af.bitvector = 0;
        af.level = level;
	af.affect_list_msg = str_dup("restricts casting of Timestop");
        af.type = gsn_timestop_done;
        affect_to_char(ch,&af);
	af.affect_list_msg = str_dup("restricts all movement");
        af.duration = 1;
        af.type = gsn_timestop;
        send_to_char("You draw upon the essence of time, causing a temporal break!\n\r",ch);
        act("The room shifts and warps as $n ruptures the flow of time within it!\n\r",ch,0,0,TO_ROOM);
          send_to_char("You feel a strange sensation as time flows in all directions around you.\n\r",ch);

        for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
        {

	vch_next = vch->next_in_room;
	if (vch == ch)
	 continue;
	if (is_safe(ch,vch))
         continue;

         if (is_affected(vch,gsn_timestop) )
                continue;
         if ( saves_spell(level,vch,DAM_OTHER)  )
          {
          send_to_char("You feel a strange sensation as time flows in all directions around you.\n\r",vch);
          }
         else
          {
          send_to_char("You feel a strange sensation as everything around you freezes in time.\n\r",vch);
	  act("$n appears to suddenly stop and freeze in time!",vch,0,0,TO_ROOM);
	  send_to_char("You feel yourself suddenly slow down then everything stops.\n\r",vch);
          affect_to_char(vch,&af);
          }
        }
        return;
}


void spell_consecrate(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
        ROOM_INDEX_DATA *in_room;
        ROOM_INDEX_DATA *room_check;
        int count, door;
        EXIT_DATA *pexit;
        AFFECT_DATA af;
	int chance;

        if (is_affected(ch,gsn_consecrate) )
        {
        	send_to_char("You do not feel up to purging a room yet.\n\r",ch);
        	return;
        }

        if (IS_SET(ch->in_room->room_flags, ROOM_NO_CONSECRATE ))
        {
        	send_to_char("This room cannot be consecrated.\n\r",ch);
        	return;
        }

        if (IS_SET(ch->in_room->room_flags, ROOM_CONSECRATED ))
        {
        	send_to_char("This room is already consecrated.\n\r",ch);
        	return;
        }

        if (!IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL) )
        {
        	send_to_char("This room is not in need of holy blessing.\n\r",ch);
        	return;
        }

        chance = ch->level + (ch->pcdata->learned[sn]/2);

        in_room = ch->in_room;
	count = 0;
        for ( door =0; door < 6; door ++)
        {
        	if ( (pexit = in_room->exit[door]) == NULL
        	|| (room_check = pexit->u1.to_room ) == NULL)
        		continue;
        	
		if (room_check == in_room)
                	continue;
        	
		if (IS_SET(room_check->room_flags, ROOM_NO_RECALL) )
                	count++;
        }

        chance -= count*10;
        act("$n gestures around the room and utters a blessing of holy rights.",ch,0,0,TO_ROOM);
        send_to_char("You chant a blessing and give the divine somatic motions of consecration.\n\r",ch);

        if (number_percent() > chance)
        {
        	act("You feel the atmosphere lighten for a moment but it passes.",ch,0,0,TO_ROOM);
        	send_to_char("Your holy rights are invoked but fail to affect the room's evil.\n\r",ch);
        	return;
        }

        act("The atmosphere in the room lightens.",ch,0,0,TO_ROOM);
        send_to_char("You succeed in consecrating the ground for a brief period!\n\r",ch);
        SET_BIT(in_room->room_flags, ROOM_CONSECRATED);

        init_affect(&af);
        af.aftype 	= AFT_COMMUNE;
        af.where 	= TO_AFFECTS;
        af.type 	= gsn_consecrate;
        af.location 	= 0;
        af.modifier 	= in_room->vnum;
	af.affect_list_msg = str_dup("restricts consecration");
        af.bitvector 	= 0;
        af.level 	= level;
        af.duration 	= 12;
        affect_to_char(ch,&af);
        return;
}

void spell_wither(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
        CHAR_DATA *victim = (CHAR_DATA *) vo;
        int dam, saves = 0, i;
        AFFECT_DATA af;

        if (is_affected(victim,sn))
	{
		send_to_char("Their flesh is already withered.\n\r",ch);
		return;
	}
	dam = dice(level,9);
	for(i=0;i<5;i++)
		if(saves_spell(level+1,victim,DAM_NEGATIVE))
			saves++;
	dam-=saves * 30;
	act("You wither the flesh of $N!",ch,0,victim,TO_CHAR);
	act("$n withers the flesh of $N!",ch,0,victim,TO_NOTVICT);
	act("$n withers your flesh!",ch,0,victim,TO_VICT);

        init_affect(&af);
        af.where = TO_AFFECTS;
        af.level = level;
        af.duration = level/4;
        af.type = sn;
        af.bitvector = 0;
	af.aftype = AFT_MALADY;
	af.location = APPLY_STR;
	af.modifier = -5;
	af.owner_name = str_dup(ch->original_name);
	affect_to_char(victim,&af);
        damage_old(ch,victim,dam,sn,DAM_NEGATIVE,TRUE);
        return;
}

void spell_shadowstrike(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

    if(is_affected(victim,gsn_shadowstrike)) {
	send_to_char("They are already held by shadowy arms.\n\r",ch);
	return;
    }

    if(saves_spell(level+7,victim,DAM_OTHER)) {
	act("A pair of misty arms fade into existence around $n, but fail to constrict him.",victim,0,0,TO_ROOM);
	act("A pair of misty arms fade into existence around you, but you deftly avoid them.",victim,0,0,TO_CHAR);
	return;
    }
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
	af.modifier = -2;
	af.level = level;
	af.type = gsn_shadowstrike;
	af.duration = level/10;
	af.owner_name = ch->original_name;
	af.bitvector = 0;
	af.location = APPLY_DEX;
	affect_to_char(victim,&af);
	af.modifier = 0;
	af.location = 0;
	af.affect_list_msg = str_dup("restricts motion");
	affect_to_char(victim,&af);
	act("A pair of intangible arms form beside $n and embrace him.",victim,0,0,TO_ROOM);
	act("A pair of intangible arms form beside you and embrace you!",victim,0,0,TO_CHAR);
	send_to_char("They have been trapped by shadowy arms.\n",ch);
	return;
}

void spell_brew(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    OBJ_DATA *created = NULL, *obj = (OBJ_DATA *) vo;
    int result, value[5], i;
    CHAR_DATA *mob;
    AFFECT_DATA af;

    int success=0; /* {FAIL,POTION,SCROLL,WAND,STAFF} */

    if ( is_affected( ch, sn ) )
    {
	send_to_char("You cannot brew another item so soon.\n\r",ch);
        return;
    }

    if (obj->wear_loc != -1) {
      send_to_char("You must carry your components.\n\r",ch);
      return;
    }

    if ( (obj->pIndexData->vnum == 4406)
    || (obj->pIndexData->vnum == 4406))
    {
	act("With a slight growl $p says, 'Hey, don't brew me!",ch,obj,0,TO_ROOM);
	act("With a slight growl $p says, 'Hey, don't brew me!",ch,obj,0,TO_CHAR);
	mob = create_mobile(get_mob_index(MOB_VNUM_DRAGON));
	mob->max_hit = 400;
	mob->hit = 400;
	mob->damroll = 14;
	mob->hitroll = 10;
	mob->level = 20;
	char_to_room(mob,ch->in_room);
	do_myell(ch,"Help! I'm being attacked by the library guardian!");
	multi_hit(mob,ch,TYPE_UNDEFINED);
	return;
    }

    result = number_percent();
    if (result < 50-level/2)  /* item destroyed */ {
      send_to_char("Your alchemy process causes a small explosion!\n\r",ch);
      act("BANG!  A small explosion errupts in $N's face.",ch,NULL,ch,TO_ROOM);
      extract_obj(obj);
      return;
    }
    result-=obj->level/2+(ch->perm_stat[1]-20)*3;

    value[0]=level;
    for(i=1;i<5;i++) {
      value[i]=-1;
    }

    if(obj->item_type == ITEM_LIGHT) {
      success=1;
      if(result>60 || obj->level<10) {
	value[1]=skill_lookup("detect evil");
	value[2]=skill_lookup("detect good");
	value[3]=skill_lookup("detect magic");
      } else if(result>40 || obj->level<30) {
	success=3;
	value[3]=skill_lookup("know alignment");
      } else {
	value[1]=skill_lookup("detect invis");
      }
    }
    if(obj->item_type == ITEM_MAP) {
      success=2;
      if(result>80 || obj->level<5) {
	value[1]=skill_lookup("infravision");
      } else if(result>60 || obj->level<15) {
	value[1]=skill_lookup("teleport");
      } else if(result>35) {
	value[1]=skill_lookup("bless");
      } else {
	value[1]=skill_lookup("word of recall");
      }
    }
    if(obj->item_type == ITEM_WEAPON) {
      result+=obj->level/2;
      if(obj->level<40) {
	value[0]=level/2+obj->level/2;
      }
     if(result>60 || obj->level<25) {
	success=3;
	value[3]=skill_lookup("lightning bolt");
      } else if(result>40 || obj->level<10) {
	success=3;
	value[3]=skill_lookup("colour spray");
      } else if(result>30) {
	success=2;
	value[1]=skill_lookup("fireball");
      } else {
	success=1;
	value[1]=skill_lookup("fireball");
      }
    }
    if(obj->item_type == ITEM_ARMOR) {
      success=1;
      value[0]=level*2/3;
      if(result>80) {
	value[1]=skill_lookup("armor");
      } else if(result>60) {
	value[1]=skill_lookup("shield");
      } else if(result>40 || obj->level < 20) {
	value[1]=skill_lookup("armor");
	value[2]=skill_lookup("shield");
      } else if(result>20 ) {
	if(ch->alignment>333) {
	  value[1]=skill_lookup("protection evil");
	} else if(ch->alignment<-333) {
	 value[1]=skill_lookup("protection good");
	} else if (result > 15)
	{
	  value[0]=level;
	  value[1]=skill_lookup("armor");
	  value[2]=skill_lookup("shield");
	  value[3]=skill_lookup("stone skin");
	}
      } else {
	value[1]=skill_lookup("sanctuary");
      }
    }
    if(obj->item_type == ITEM_KEY) {
      if(result>30 || obj->level<5) {
	success=2;
	value[1]=skill_lookup("pass door");
      } else {
	success=1;
	value[1]=skill_lookup("pass door");
      }
    }
    if(obj->item_type == ITEM_CONTAINER) {
      if(result>70 || obj->level<30) {
	success=2;
	value[1]=skill_lookup("create food");
	value[2]=skill_lookup("create food");
	value[3]=skill_lookup("create food");
	value[4]=skill_lookup("create food");
      } else if(result>40) {
	success=1;
	value[1]=skill_lookup("create food");
	value[2]=skill_lookup("create food");
	value[3]=skill_lookup("create food");
	value[4]=skill_lookup("create food");
      } else {
	success=4;
	value[3]=skill_lookup("create food");
      }
    }
    if(obj->item_type == ITEM_DRINK_CON) {
      success=1;
      value[1]=skill_lookup("create spring");
    }
    if(obj->item_type == ITEM_TRASH) {
      success=3;
      if(result>80 || obj->level<15) {
	value[3]=skill_lookup("faerie fire");
      } else if(result>60 || obj->level<35) {
	success=4;
	value[3]=skill_lookup("faerie fog");
      } else if(result>30){
	value[3]=skill_lookup("dispel magic");
      } else {
	success=1;
	value[1]=skill_lookup("cancellation");
      }
    }
    if(obj->item_type == ITEM_BOAT) {
      success=1;
      if(result>50 || obj->level<10) {
	value[1]=skill_lookup("refresh");
      } else {
	value[1]=skill_lookup("fly");
      }
    }
    if(obj->item_type == ITEM_TREASURE) {
      success=4;
      if(result>60 ) {
	value[3]=skill_lookup("giant strength");
      } else if(result>45 || obj->level<10) {
	value[3]=skill_lookup("stone skin");
     } else if(result>10 || obj->level<15) {
	value[3]=skill_lookup("frenzy");
      } else {
	value[3]=skill_lookup("haste");
      }
    }
    if(obj->item_type == ITEM_CORPSE_NPC) {
      result+=obj->level/2;
      success=3;
      value[0]=level/2+obj->level/3;
      if(result>70 || obj->level<20) {
	value[3]=skill_lookup("chill touch");
      } else if(result>55 || obj->level<30) {
	value[3]=skill_lookup("weaken");
      } else if(result>40 || obj->level<40) {
	value[3]=skill_lookup("plague");
      } else if(result>25 || obj->level<55) {
	value[3]=skill_lookup("energy drain");
      } else {
	value[3]=skill_lookup("acid blast");
      }
    }
    if(obj->item_type == ITEM_GEM || obj->item_type == ITEM_JEWELRY) {
      success=1;
      result+=obj->level/2;
      if(result>70) {
	success=0;
      } else if(result>40) {
	value[1]=skill_lookup("cure disease");
      } else if(result>25) {
	value[1]=skill_lookup("cure poison");
      } else {
	value[1]=skill_lookup("cure serious");
      }
    }

    switch(success) {
    case 1:
      created = create_object(get_obj_index(OBJ_VNUM_ARCPOTION), 0);
      send_to_char("You have created a potion.\n\r",ch);
      act("$N has created a potion.",ch,NULL,ch,TO_ROOM);
      break;
    case 2:
      created = create_object(get_obj_index(OBJ_VNUM_ARCSCROLL), 0);
      send_to_char("You have created a scroll.\n\r",ch);
      act("$N has created a scroll.",ch,NULL,ch,TO_ROOM);
      break;
    case 3:
      created = create_object(get_obj_index(OBJ_VNUM_ARCWAND), 0);
      send_to_char("You have created a wand.\n\r",ch);
      act("$N has created a wand.",ch,NULL,ch,TO_ROOM);
      value[1]=level/5;
      value[2]=level/5;
      break;
    case 4:
      created = create_object(get_obj_index(OBJ_VNUM_ARCSTAFF), 0);
      send_to_char("You have created a staff.\n\r",ch);
      act("$N has created a staff.",ch,NULL,ch,TO_ROOM);
      value[1]=level/5;
      value[2]=level/5;
      break;
    default:
      send_to_char("Your alchemy process causes a small explosion!\n\r",ch);
      act("BANG!  A small explosion errupts in $N's face.",ch,NULL,ch,TO_ROOM);
      break;
    }

    if(success>0) {
      for(i=0;i<5;i++) {
	created->value[i]=value[i];
      }
      created->level=level;
       created->value[0] = level;
      obj_to_char(created,ch);
    }

    init_affect(&af);
    af.where     = TO_AFFECTS;
    af.aftype 	 = AFT_POWER;
    af.type      = sn;
    af.level     = level;
    af.duration  = 8;
    af.modifier  = 0;
    af.location  = 0;
    af.bitvector = 0;
    affect_to_char( ch, &af );

    extract_obj(obj);
    return;
}

void spell_familiar(int sn,int level, CHAR_DATA *ch,void *vo,int target)
{
        CHAR_DATA *familiar;
        AFFECT_DATA af;
        CHAR_DATA *check;
	int chance;
	char buf[MAX_STRING_LENGTH];

        if (is_affected(ch,sn))
        {
	send_to_char("You aren't up to calling another familiar yet.\n\r",ch);
        return;
        }

        for (check = char_list; check != NULL; check = check->next)
        {
	if (IS_NPC(check))
   		if ( (check->master == ch) && (check->pIndexData->vnum == MOB_VNUM_UNICORN
		|| check->pIndexData->vnum == MOB_VNUM_DRAGON) )
        	{
		send_to_char("You already have a familiar under your command.\n\r",ch);
        	return;
        	}
	}
	init_affect(&af);
        af.where = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
        af.level = level;
        af.location = 0;
        af.modifier = 0;
        af.duration = 24;
        af.bitvector = 0;
		af.affect_list_msg = str_dup("prevents summoning of another familiar");
        af.type = sn;
        affect_to_char(ch,&af);

	if (IS_GOOD(ch))
		familiar = create_mobile(get_mob_index(MOB_VNUM_UNICORN));
	else
		familiar = create_mobile(get_mob_index(MOB_VNUM_DRAGON));

	familiar->alignment = ch->alignment;
	act("$n calls forth a familiar to aid $s!",ch,0,0,TO_ROOM);
	send_to_char("You call forth for a familiar from the astral planes!\n\r",ch);

        familiar->level = ch->level;
        familiar->damroll += level/2;
        familiar->max_hit = ch->max_hit;
        familiar->hit = familiar->max_hit;
        familiar->max_move = ch->max_move;
        familiar->move = familiar->max_move;
        char_to_room(familiar,ch->in_room);

	if (!IS_GOOD(ch))
	{
	chance = ch->pcdata->learned[sn] * 9/10;
	if (number_percent() > chance)
		{
	act("$n says, 'I do not follow such weak willed mortals!'",familiar,0,0,TO_ROOM);
		sprintf(buf,"Help! I'm being attacked by %s!",familiar->short_descr);
		do_myell(ch,buf);
		multi_hit(familiar,ch,TYPE_UNDEFINED);
		return;
		}
	}


        add_follower(familiar, ch);
        familiar->leader = ch;
        SET_BIT(familiar->affected_by, AFF_CHARM);
        return;
}


void spell_firestream(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
        CHAR_DATA *victim = (CHAR_DATA *) vo;
        OBJ_DATA *obj_lose;
        OBJ_DATA *obj_next;
        int dam;
	int dice_sz;

	dice_sz = 4;
        if (ch->level < 20)
		dice_sz = 4;
        else if (ch->level < 25)
		dice_sz = 5;
        else if (ch->level < 30)
		dice_sz = 5;
        else if (ch->level < 40)
		dice_sz = 6;
        else dice_sz = 6;

        if (number_percent() < level)
	{
        for (obj_lose = victim->carrying; obj_lose != NULL; obj_lose = obj_next)
		{
                char *msg;
                obj_next = obj_lose->next_content;
                if (number_bits(2) != 0)
                        continue;
                switch(obj_lose->item_type)
		{
                default: continue;
                case ITEM_SCROLL: msg = "$p bursts into flames and turns to ash."; break;
                case ITEM_POTION: msg = "$p bubbles and vaporises."; break;
		}
                act(msg,victim,obj_lose,NULL,TO_CHAR);
                extract_obj(obj_lose);
		}
	}
        dam = dice(level/2, dice_sz - 1) + dice(level/2, dice_sz);
    if (check_spellcraft(ch,sn))
    {
		dam *= 3;
		dam /= 2;
    }

        if (saves_spell(level,victim,DAM_FIRE))
                dam /= 2;
        act("$n clenches a fist and releases a stream of searing flames!",ch,0,0,TO_ROOM);
        act("You gesture at $N and release a stream of searing fire!",ch,0,victim,TO_CHAR);
        damage_old(ch,victim,dam,sn,DAM_FIRE,TRUE);
        return;
}

void spell_fireshield(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
	AFFECT_DATA af;

	if (is_affected(ch,gsn_fireshield))
	{
	send_to_char("You are already wreathed in a flaming halo.\n\r",ch);
	return;
	}
	if (is_affected(ch,gsn_iceshield))
	{
	send_to_char("Your iceshield vaporises in a steaming explosion!\n\r",ch);
	act("$n's iceshield vaporises in a blast of steam!",ch,0,0,TO_ROOM);
	affect_strip(ch,gsn_iceshield);
	return;
	}

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
	af.type = gsn_fireshield;
	af.modifier = 0;
	af.level = level;
	af.duration = 3 + ch->level/10;
	af.location = 0;
	af.bitvector = 0;
	affect_to_char(ch,&af);
	send_to_char("You are surrounded in a flaming halo of heat.\n\r",ch);
	act("$n is surrounded by a flaming halo of heat.",ch,0,0,TO_ROOM);
return;
}

void spell_iceshield(int sn,int level,CHAR_DATA *ch,void *vo, int target)
{
	AFFECT_DATA af;

	if (is_affected(ch,gsn_iceshield))
	{
	send_to_char("You are already surrounded by a frozen aura.\n\r",ch);
	return;
	}
	if (is_affected(ch,gsn_fireshield))
	{
	send_to_char("Your fireshield vaporises in a steaming explosion!\n\r",ch);
	act("$n's fireshield vaporises in a blast of steam!",ch,0,0,TO_ROOM);
	affect_strip(ch,gsn_fireshield);
	return;
	}
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
	af.type = gsn_iceshield;
	af.modifier = 0;
	af.level = level;
	af.duration = 3 + ch->level/10;
	af.location = 0;
	af.bitvector = 0;
	affect_to_char(ch,&af);
	send_to_char("You are surrounded by an aura of freezing air.\n\r",ch);
	act("$n is surrounded by a freezing aura of air.",ch,0,0,TO_ROOM);
return;
}

void spell_shadowself(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *shadow;
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *check;

    for (check = char_list; check != NULL; check = check->next)
    {
	if (IS_NPC(check) && check->pIndexData->vnum == MOB_VNUM_SHADOW
	&& (!str_cmp(check->name,ch->name)) )
	{
	send_to_char("Your shadow is already seperated from you.\n\r",ch);
	return;
	}
    }

    send_to_char("You sever your shadow and give it substance.\n\r",ch);
    act("The shadows around $n seem to shift and take form....",ch,0,0,TO_ROOM);
    shadow = create_mobile(get_mob_index(MOB_VNUM_SHADOW));
    shadow->alignment = -1000;
    shadow->max_hit = 1;
    shadow->hit = 1;
    sprintf(buf,"%s",ch->name);
    free_string(shadow->name);
    free_string(shadow->short_descr);
    free_string(shadow->long_descr);
    shadow->name = str_dup(buf);
    shadow->short_descr = str_dup(buf);
    sprintf(buf,"%s is here.\n\r",ch->name);
    shadow->long_descr = str_dup(buf);
    sprintf(buf,shadow->description,ch->name);
    free_string(shadow->description);
    shadow->description = str_dup(buf);
    char_to_room(shadow,ch->in_room);
    shadow->master = ch;
return;
}

void spell_ranger_staff(int sn,int level,CHAR_DATA *ch,void *vo, int target)
{
        OBJ_DATA *staff;
        int bonus,dice,number;
        int ch_level;
        AFFECT_DATA af;

        staff = create_object(get_obj_index(OBJ_VNUM_RANGER_STAFF),0);
        ch_level = level;
        if (number_percent() < 50)
                ch_level -= (number_range(0,3));
        else
                ch_level += (number_range(0,2));

        if (ch_level < 32)
        {
        dice = 5; number = 6;
        }
        else if (ch_level < 34)
        {
        dice = 3; number = 11;
        }
        else if (ch_level < 36)
        {
        number = 5; dice = 8;
        }
        else if (ch_level < 38)
        {
        number = 6; dice = 7;
        }
        else if (ch_level < 40)
        {
        number = 6; dice = 7;
        }
        else if (ch_level < 42)
        {
        number = 5; dice = 9;
        }
        else if (ch_level < 44)
        {
        number = 5;     dice = 9;
        }
        else if (ch_level < 46)
        {
        number = 5;     dice = 9;
        }
        else if (ch_level < 47)
        {
        number = 13;    dice = 3;
        }
        else if (ch_level < 48)
        {
        number = 6;     dice = 8;
        }
        else if (ch_level < 49)
        {
        number = 8;     dice = 6;
        }
        else if (ch_level < 50)
        {
        number = 8;     dice = 6;
        }
        else
        {
        number = 6;     dice = 9;
        }

        bonus = ch->level/4;
	init_affect(&af);
        af.where = TO_OBJECT;
	af.aftype    = AFT_SPELL;
        af.type = sn;
        af.level = level;
        af.duration = -1;
        af.location = APPLY_HITROLL;
        af.modifier = bonus;
        af.bitvector = 0;
        affect_to_obj(staff,&af);
        af.location = APPLY_DAMROLL;
        affect_to_obj(staff,&af);
        staff->level = level;
	staff->timer = (20 + number_range(0,10));
        staff->value[1] = number;
        staff->value[2] = dice;
        obj_to_char(staff,ch);
        act("$n creates $p!",ch,staff,0,TO_ROOM);
        act("You create $p!",ch,staff,0,TO_CHAR);
        return;
}


void spell_bless_weapon(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    OBJ_DATA *avenger;
/*    OBJ_DATA *obj_check; */
    AFFECT_DATA af;
    bool two_hands = FALSE;
    int dice,number,dam_bonus,hit_bonus;
    int fail, result;
    char buf[MAX_STRING_LENGTH];
    char *type_name;

    if (obj->item_type != ITEM_WEAPON || obj->value[0] != WEAPON_SWORD)
    {
        send_to_char("You can only bless a sword for divine empowerment.\n\r",ch);
        return;
    }
    if (IS_WEAPON_STAT(obj,WEAPON_AVENGER))
    {
        send_to_char("It is already empowered by holy blessing.\n\r",ch);
        return;
    }
/*
    for (obj_check = object_list; obj_check != NULL; obj_check = obj_check->next)
    {
        if (obj_check->pIndexData->vnum == OBJ_VNUM_AVENGER
        && !str_cmp(obj_check->owner,ch->name) )
            found = TRUE;
    }
    if (found)
    {
        send_to_char("You already have a blessed weapon in existence.\n\r",ch);
        return;
    }
*/
    if (IS_WEAPON_STAT(obj,WEAPON_TWO_HANDS))
        two_hands = TRUE;
    fail = 275;
    fail -= ch->pcdata->learned[sn];
    fail -= 3 * ch->level;
    if (is_set(&obj->extra_flags,ITEM_EVIL))
        fail += 200;
    if (is_set(&obj->extra_flags,ITEM_BLESS))
        fail -= 20;
    if (is_set(&obj->extra_flags,ITEM_GLOW))
        fail -= 10;
    fail -= ( (obj->value[2] + 1) * obj->value[1]/2);
    result = number_percent();

    if (result < (fail/3))
    {
        act("$p flares blindingly... and vaporises!",ch,obj,0,TO_ROOM);
        act("$p flares blindingly... and vaporises!",ch,obj,0,TO_CHAR);
        extract_obj(obj);
        return;
    }
    if (result < fail)
    {
        send_to_char("Nothing seemed to happen.\n\r",ch);
        return;
    }
    act("$p flares blindingly and a divine power seems to flow into it!",ch,obj,0,TO_CHAR);
    act("$p flares a brilliant white and appears more perfect!",ch,obj,0,TO_ROOM);

         if (level < 42)
        {
        number = 5; dice = 9;
        }
        else if (level < 44)
        {
        number = 5;     dice = 9;
        }
        else if (level < 46)
        {
        number = 5;     dice = 9;
        }
        else if (level < 47)
        {
        number = 13;    dice = 3;
        }
        else if (level < 48)
        {
        number = 6;     dice = 8;
        }
        else if (level < 49)
        {
        number = 8;     dice = 6;
        }
        else if (level < 50)
        {
        number = 8;     dice = 6;
        }
        else
        {
        number = 6;     dice = 9;
        }

	dam_bonus = ch->level/8;
	hit_bonus = ch->level/8;

	avenger = create_object(get_obj_index(OBJ_VNUM_AVENGER),0);
	avenger->value[1] = number;
	avenger->value[2] = dice;
	avenger->weight = obj->weight;
	init_affect(&af);
	af.where = TO_OBJECT;
	af.aftype    = AFT_SPELL;
	af.type = sn;
	af.level = level;
	af.duration = -1;
	af.bitvector = 0;
	af.location = APPLY_HITROLL;
	af.modifier = dam_bonus;
	affect_to_obj(avenger,&af);
	af.location = APPLY_DAMROLL;
	af.modifier = hit_bonus;
	affect_to_obj(avenger,&af);

	type_name = "Peacekeeper";
	switch(number_range(0,2))
    {
	case (0):	type_name = "Peacekeeper";	break;
	case (1):	type_name = "Lightbringer";	break;
	case (2):	type_name = "Shadowbane";	break;
    }

	if (two_hands)
	sprintf(buf,"a claymore named '%s'",type_name);
	else
	sprintf(buf,"a longsword named '%s'",type_name);

	free_string(avenger->short_descr);
	avenger->short_descr = str_dup(buf);

	if (!str_cmp(type_name,"Peacekeeper"))
	{
	af.location = APPLY_AC;
	af.modifier = -20;
	affect_to_obj(avenger,&af);
	}
	if (!str_cmp(type_name,"Lightbringer"))
	{
	af.location = APPLY_HIT;
	af.modifier = 25;
	affect_to_obj(avenger,&af);
	SET_BIT(avenger->value[4], WEAPON_LIGHTBRINGER);
	}

	if (!str_cmp(type_name,"Shadowbane"))
	SET_BIT(avenger->value[4], WEAPON_SHADOWBANE);

        if (two_hands)
        SET_BIT(avenger->value[4], WEAPON_TWO_HANDS);

	sprintf(buf,"%s",ch->name);
	avenger->owner = str_dup(buf);

	obj_to_char(avenger,ch);
	extract_obj(obj);

return;
}


void spell_revolt(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    CHAR_DATA *charmie;
    CHAR_DATA *charmie_next;
    char buf[MAX_STRING_LENGTH];
    return;
    for (charmie = ch->in_room->people; charmie != NULL; charmie = charmie_next)
    {
        charmie_next = charmie->next_in_room;
        if (!IS_AFFECTED(charmie,AFF_CHARM) || charmie->leader != victim
        || saves_spell(level,victim,DAM_OTHER) )
            continue;
        act("$n suddenly looks very angry.",charmie,0,0,TO_ROOM);
        act("You suddenly feel incited by $n's words and turn on your master!",charmie,0,ch,TO_CHAR);
        sprintf(buf,"I refuse to follow a tyrannt like you, %s!",victim->name);
        do_gtell(charmie,buf);
        REMOVE_BIT(charmie->affected_by,AFF_CHARM);
        do_follow(charmie,"self");
        sprintf(buf,"Help! %s is revolting!", IS_NPC(charmie) ? charmie->short_descr : charmie->name);
        do_myell(victim, buf);
	charmie->fighting = victim;
        multi_hit(charmie, victim,-1);
    }

    return;
}


void spell_team_spirit(int sn,int level, CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int amount;

    if (victim == ch)
    {
	send_to_char("That isn't possible with yourself.\n\r",ch);
	return;
    }
    if (victim->cabal != CABAL_ARCANA)
   {
        send_to_char("You can only team up with Cabal Arcana members.\n\r",ch);
	return;
    }
    amount = dice(4,5) + level/2;

    if (ch->level > (victim->level + 30))
	amount *= 3;
    else if (ch->level > (victim->level + 20))
	amount *= 8/3;
    else if (ch->level > (victim->level + 10))
	amount *= 2;
    else
	amount *= 4/3;

    amount = dice(ch->level,2)*(3/2);
    damage_old(ch,ch,60,sn,DAM_ENERGY,TRUE);

    victim->mana = UMIN(victim->mana + amount, victim->max_mana);
    send_to_char("You feel energy surge through you!\n\r",victim);
    return;
}

void spell_safety(int sn,int level, CHAR_DATA *ch,void *vo, int target)
{
    CHAR_DATA *gch;
    CHAR_DATA *gch_next;
    ROOM_INDEX_DATA *pRoomIndex;

    pRoomIndex = get_room_index(hometown_table[ch->hometown].recall);
    if (pRoomIndex == NULL)
	pRoomIndex = get_room_index(ROOM_VNUM_TEMPLE);

    for (gch = ch->in_room->people; gch != NULL; gch = gch_next)
    {
	gch_next = gch->next_in_room;
	if (IS_NPC(gch))
		continue;
	if (!is_same_group(ch,gch))
		continue;
    if (IS_SET(gch->in_room->room_flags,ROOM_NO_RECALL) ||
	IS_AFFECTED(gch,AFF_CURSE)
        || (ch->in_room->cabal != 0
        && ch->cabal != ch->in_room->cabal))
    {
	send_to_char("Spell failed.\n\r",gch);
    }
    else
	act("$n suddenly vanishes!",gch,0,0,TO_ROOM);
    }

    for (gch = ch->in_room->people; gch != NULL; gch = gch_next)
    {
	gch_next = gch->next_in_room;
	if (!is_same_group(ch,gch))
		continue;
    if (IS_SET(gch->in_room->room_flags,ROOM_NO_RECALL) ||
	IS_AFFECTED(gch,AFF_CURSE))
		continue;
	if (gch->fighting != NULL)
		stop_fighting(gch,TRUE);
	char_from_room(gch);
	char_to_room(gch,pRoomIndex);
	act("$n suddenly appears in the room.",gch,0,0,TO_ROOM);
	do_look(gch,"auto");
    }
    ch->move /=2;
    return;
}

void spell_decoy(int sn,int level, CHAR_DATA *ch,void *vo, int target)
{
    CHAR_DATA *decoy;
    int num,count;
    AFFECT_DATA af;
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    char buf[MAX_INPUT_LENGTH];

    if (cabal_down(ch,CABAL_OUTLAW))
	return;

    if (is_affected(ch,sn))
    {
	send_to_char("The Godfather has not yet prepared more decoys for you.\n\r",ch);
        return;
    }

    if (IS_NPC(victim))
    {
        send_to_char("They don't need decoys to elude the law.\n\r",ch);
        return;
    }
     
    num = number_range(1,2);
    for (count = 0; count < num; count++)
    {
        decoy = create_mobile(get_mob_index(MOB_VNUM_DECOY));
        sprintf(buf,"%s is here.\n\r",victim->name);
        free_string(decoy->long_descr);
        decoy->long_descr = str_dup(buf);
        sprintf(buf,"%s", victim->name);
        free_string(decoy->name);
        decoy->name = str_dup(buf);
        free_string(decoy->short_descr);
        decoy->short_descr = str_dup(buf);
        decoy->max_hit = 1;
        decoy->hit = 1;
        decoy->master = victim;
        char_to_room(decoy,victim->in_room);
    }

    send_to_char("You feel a little disoriented as images of you appear all around!\n\r",victim);
    act("Suddenly there are more of $n all around!",victim,0,0,TO_ROOM);

    init_affect(&af);
    af.where		= TO_AFFECTS;
    af.aftype   	= AFT_SPELL;
    af.type 		= sn;
    af.level 		= level;
    af.modifier 	= 0;
    af.duration 	= 12;
    af.bitvector	= 0;
    af.location 	= 0;
    affect_to_char(ch,&af);
    return;
}

void spell_protectorate_plate(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    OBJ_DATA *plate;
    AFFECT_DATA af;

    plate = create_object(get_obj_index(OBJ_VNUM_PROTECTORATE),0);
    plate->level = ch->level;
    plate->timer = 48;


//    if (mod > 6)    mod = 6;

	init_affect(&af);
    af.where = TO_OBJECT;
	af.aftype    = AFT_SPELL;
    af.type = sn;
    af.duration = -1;
    af.bitvector = 0;
    af.level = level;
    af.location = APPLY_HITROLL;
    af.modifier = 10;
    affect_to_obj(plate,&af);
    af.location = APPLY_DAMROLL;
    affect_to_obj(plate,&af);
    af.location = APPLY_HIT;
    af.modifier = URANGE(20,2*ch->level,100);
    affect_to_obj(plate,&af);
    af.location = APPLY_STR;
    af.modifier = 5;
    affect_to_obj(plate,&af);
    af.location = APPLY_WIS;
    affect_to_obj(plate,&af);
    if (ch->level > 35)
        set_bit(&plate->extra_flags, ITEM_HUM);
    if (ch->level > 40)
        set_bit(&plate->extra_flags, ITEM_GLOW);
    obj_to_char(plate,ch);
    act("$n creates $p!",ch,plate,0,TO_ROOM);
    act("You create $p!",ch,plate,0,TO_CHAR);
    return;
}



void spell_nightwalk(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    ROOM_INDEX_DATA *pRoomIndex;
    int chance;

    pRoomIndex = get_room_index(ROOM_VNUM_NIGHTWALK);
    chance = (level + 30);
    if (number_percent() > chance
    || pRoomIndex == NULL)
    {
        send_to_char("You fail to enter the planes of night.\n\r",ch);
        return;
    }
    act("$n is surrounded in shadows and disappears!.",ch,0,0,TO_ROOM);
    send_to_char("You overcome your ties to the material plane and step into the shadow world.\n\r",ch);
    char_from_room(ch);
    char_to_room(ch,pRoomIndex);
    act("$n fades into existence.",ch,0,0,TO_ROOM);
    do_look(ch,"auto");
    return;
}

void spell_alarm(int sn,int level,CHAR_DATA *ch,void *vo, int target)
{
    ROOM_INDEX_DATA *pRoomIndex;
    char buf[MAX_STRING_LENGTH];

    pRoomIndex = ch->in_room;
    if (pRoomIndex->cabal != 0
    && pRoomIndex->cabal != ch->cabal)
    {
        send_to_char("You can't place alarms in an enemy Cabal.\n\r",ch);
        return;
    }
    if (IS_SET(pRoomIndex->room_flags,ROOM_NO_ALARM))
    {
        send_to_char("Your spell fizzles and dies.\n\r",ch);
        act("$n's spell fizzles and dies.",ch,0,0,TO_ROOM);
        return;
    }

    if (ch->alarm != NULL && ch->alarm == ch->in_room)
    {
        send_to_char("You already have an alarm placed here.\n\r",ch);
        return;
    }
    else if (ch->alarm != NULL)
    {
        sprintf(buf,"You relocate your alarm from %s and place it here.\n\r",ch->alarm->name);
        send_to_char(buf,ch);
    }
    else
    {
        send_to_char("You create a new alarm and place it within the room.\n\r",ch);
    }

    ch->alarm = ch->in_room;
    return;
}

void spell_holy_fire(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
    int chance, dam_mod;

    chance = 100;
    dam_mod = 10;
    if (IS_GOOD(ch) && IS_GOOD(victim))
    {
        dam_mod = 5;
        chance = 0;
    }
    else if (IS_EVIL(ch))
    {
        dam_mod = 7;
        chance = 0;
    }
    else if (IS_GOOD(ch) && IS_NEUTRAL(victim))
         dam_mod = 8;
    else if (IS_NEUTRAL(ch))
    {
        dam_mod = 6;
        chance = 70;
    }

    dam = dice(level, 7);
    if (number_percent() > chance)
    {
        act("$n's heavenly fire turns on $m!",ch,0,0,TO_ROOM);
        act("Your heavenly fire turns on you for your sins!",ch,0,0,TO_CHAR);
        dam *= dam_mod;
        dam /= 10;
	if (saves_spell(level,ch,DAM_HOLY))
		dam /= 2;
        damage_old(ch,ch,dam,sn,DAM_HOLY,TRUE);
        return;
    }
    act("$n calls down fire from the heavens!",ch,0,0,TO_ROOM);
    act("You call down fire from the heavens!",ch,0,0,TO_CHAR);
    dam *= dam_mod;
    dam /= 10;
    damage_old(ch,victim,dam,sn,DAM_HOLY,TRUE);
    return;
}

void spell_badge(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    OBJ_DATA *plate;
    AFFECT_DATA af;
    int mod;

    if (cabal_down(ch,CABAL_OUTLAW))
	return;

    plate = create_object(get_obj_index(OBJ_VNUM_BADGE),0);
    plate->level = ch->level;
    plate->timer = 45;

    mod = (2 + ch->level/20);
    if (mod > 4)    mod = 4;

	init_affect(&af);
    af.where = TO_OBJECT;
	af.aftype    = AFT_POWER;
    af.type = sn;
    af.duration = -1;
    af.bitvector = 0;
    af.level = level;
    af.location = APPLY_HITROLL;
    af.modifier = mod;
    affect_to_obj(plate,&af);
    af.location = APPLY_DAMROLL;
    affect_to_obj(plate,&af);
    af.location = APPLY_HIT;
    af.modifier = URANGE(20,2*ch->level,75);
    affect_to_obj(plate,&af);
    af.location = APPLY_STR;
    af.modifier = 1;
    affect_to_obj(plate,&af);
    af.location = APPLY_WIS;
    affect_to_obj(plate,&af);
    obj_to_char(plate,ch);
    act("$n creates $p!",ch,plate,0,TO_ROOM);
    act("You create $p!",ch,plate,0,TO_CHAR);
    return;
}

void spell_trace(int sn,int level,CHAR_DATA *ch,void *vo, int target)
{
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *pRoomIndex;

    if (target_name[0] == '\0')
    {
send_to_char("Attempt to trace which criminal?\n\r",ch);
    return;
    }
    victim = get_char_world(ch,target_name);
    if (victim == NULL)
    {
send_to_char("The enforcer patrols can't trace that person anywhere.\n\r",ch);
    return;
    }
/*
if (saves_spell(level + 5,victim,DAM_OTHER))
    {
send_to_char("The enforcer patrols can't trace that person anywhere.\n\r",ch);
    return;
   }
*/

if (IS_NPC(victim) && victim->pIndexData->vnum != MOB_VNUM_DECOY)
    {
send_to_char("That person is not wanted and has not been reported by enforcer patrols.\n\r",ch);
return;
    }

if (!IS_NPC(victim) && (!IS_SET(victim->act, PLR_CRIMINAL)))
    {
send_to_char("That person is not wanted and has not been reported by enforcer patrols.\n\r",ch);
return;
    }


    pRoomIndex = ch->in_room;
    ch->in_room=victim->in_room;
    do_look(ch,"auto");
    ch->in_room=pRoomIndex;
    return;
}

/* New energy drain spell due to anti-paladin's always complaining how
useless the old one was.
-Ceran
*/

void spell_energy_drain(int sn, int level, CHAR_DATA *ch,void *vo,int target)
{
        CHAR_DATA *victim = (CHAR_DATA *) vo;
        int type;
        int amount, amounthp;
        AFFECT_DATA af;

        if (victim == ch)
        {
        send_to_char("You can't drain your own life force.\n\r",ch);
        return;
        }
        switch(number_range(0,3))
        {
	default: type = 1; amount = dice(level,3); break;
        case (0): case (1):          /* HP */
        type = 1;       amount = dice(level,4); break;
        case (2):       /* move */
        type = 2;       amount = dice(level,3); break;
        case (3):       /* mana */
        type = 3;       amount = dice(level,3); break;
        }
        amounthp = dice(level,2);
        send_to_char("You feel an icy hand brush against your soul.\n\r",victim);

        if (saves_spell(level,victim,DAM_NEGATIVE))
        {
        act("$n turns pale and shivers briefly.",victim,0,0,TO_ROOM);
        damage_old(ch,victim,amounthp,sn,DAM_NEGATIVE,TRUE);
        return;
        }
        act("$n gets a horrible look of pain in $s face and shudders in shock.",victim,0,0,TO_ROOM);

	init_affect(&af);
        af.where = TO_AFFECTS;
        af.type = sn;
	af.aftype = AFT_SPELL;
        af.level = level;
        af.duration = level/2;
        af.bitvector = 0;

        switch(type)
        {
        default:
        case (1):
        act("You drain $N's vitality with vampiric magic.",ch,0,victim,TO_CHAR);
        send_to_char("You feel your body being mercilessly drained.\n\r",victim);
	ch->hit = URANGE(0, ch->hit + amount/3, ch->max_hit);
        af.location = APPLY_STR;
        af.modifier = -2;
        if (!is_affected(victim,sn))
        {
                affect_to_char(victim,&af);
                af.location = APPLY_CON;
                af.modifier = -3;
                affect_to_char(victim,&af);
        }
        break;
        case (2):
        send_to_char("Your energy draining invigorates you!\n\r",ch);
        victim->move = URANGE(0,victim->move - amount, victim->max_move);
	ch->move = URANGE(0,ch->move + amount/2, ch->max_move);
        send_to_char("You feel tired and weakened.\n\r",victim);
        af.location = APPLY_DEX;
        af.modifier = -2;
        if (!is_affected(victim,sn))
        {
                affect_to_char(victim,&af);
                af.location = APPLY_STR;
                af.modifier = -1;
                af.location = APPLY_MOVE;
                af.modifier = -amount/2;
                affect_to_char(victim,&af);
        }
        break;
        case (3):
        victim->mana = URANGE(0,victim->mana - amount,victim->max_mana);
        send_to_char("Your draining sends warm energy through you!\n\r",ch);

	ch->mana = URANGE(0,ch->mana + amount/3, ch->max_mana);
        send_to_char("You feel part of your mind being savagely drained.\n\r",victim);
        if (!is_affected(victim,sn))
                {
                af.location = APPLY_INT;
                af.modifier = -3;
                affect_to_char(victim,&af);
                af.location = APPLY_WIS;
                af.modifier = -2;
                affect_to_char(victim,&af);
                }
        break;
        }

        damage_old(ch,victim,dice(level,3) + amounthp,sn,DAM_NEGATIVE,TRUE);
        return;
}

bool check_spellcraft(CHAR_DATA *ch,int sn)
{
        int chance;

	if(IS_NPC(ch))
		return FALSE;

        chance = get_skill(ch,gsn_spellcraft);

        if (chance == 0
        || ch->level < skill_table[gsn_spellcraft].skill_level[ch->class])
                return FALSE;

        chance /= 5;

        if (sn == skill_lookup("windwall")
        || sn == skill_lookup("tsunami"))
                chance = 0;

        if (sn == skill_lookup("fireball")
        || sn == skill_lookup("iceball")
        || sn == skill_lookup("cone of cold"))
                chance -= 12;

        if (sn == skill_lookup("acid blast")
        || sn == skill_lookup("concatenate"))
                chance -= 10;

        if (sn == skill_lookup("magic missle")
        || sn == skill_lookup("chill touch")
        || sn == skill_lookup("firestream")
        || sn == skill_lookup("color spray"))
                chance += 10;

	if (sn == skill_lookup("fire and ice"))
		chance -= 6;

	if (sn == skill_lookup("earthmaw"))
		chance -= 11;

	chance += GET_HITROLL(ch) / 5;
	
        if (number_percent() > chance)
        {
        	check_improve(ch,gsn_spellcraft,FALSE,6);
        	return FALSE;
        }
        check_improve(ch,gsn_spellcraft,TRUE,6);

	if (IS_IMMORTAL(ch))
		send_to_char("Spellcraft boost now!\n\r",ch);

        return TRUE;
}

int spellcraft_dam(int num, int die)
{
        int dam;
        if (num == 0 || die == 0)
                return 0;

        if (die == 1)
                return num;
        else if (die == 2)
                return (num*2);
        else if (die == 3)
                return (num * number_range(2,3));

        dam = (num*die)/2;
        dam += dice(num/2,die);
        return dam;
}

/* This is one weird spell... (Ceran)*/
void spell_animate_object(int sn,int level, CHAR_DATA *ch,void *vo, int target)
{
        OBJ_DATA *obj = (OBJ_DATA *) vo;
        CHAR_DATA *mob;
        CHAR_DATA *check;
        int hp = 1, damroll = 1, move = 1;
        int ilevel, type, chance, ac, acm;
        char buf[MAX_STRING_LENGTH];
        bool found = FALSE;

		for (check = char_list; check != NULL; check = check->next)
		{
		if (!IS_NPC(check) || check->master != ch)
		        continue;
		if (check->pIndexData->vnum == MOB_VNUM_ARMOR
		|| check->pIndexData->vnum == MOB_VNUM_WEAPON
		|| check->pIndexData->vnum == MOB_VNUM_KEY)
		        found = TRUE;

		if (found)
		 break;
		}

		if (found)
		{
		send_to_char("You already have an animated object following you.\n\r",ch);
		return;
		}
		
        if (obj->wear_loc != WEAR_NONE)
        {
        send_to_char("You can only animate objects you are carrying.\n\r",ch);
        return;
        }
        ilevel = obj->level;

        if (obj->pIndexData->limtotal > 0)
                level += (25/obj->pIndexData->limtotal);

        type = obj->item_type;
        if (type != ITEM_WEAPON
        && type != ITEM_ARMOR
        && type != ITEM_KEY
        && type != ITEM_FURNITURE)
        {
        send_to_char("You can't animate that kind of object.\n\r",ch);
	return;
        }
        if (type == ITEM_ARMOR
        && (!IS_SET(obj->wear_flags,ITEM_WEAR_BODY)
        && !IS_SET(obj->wear_flags,ITEM_WEAR_HANDS)
        && !IS_SET(obj->wear_flags,ITEM_WEAR_SHIELD) ) )
        {
        send_to_char("You can only animate armor that is body, hands, or shields.\n\r",ch);
        return;
        }

        chance = ch->pcdata->learned[sn];
        chance = URANGE(2, chance + (level - ilevel)*3, 95);

        if (number_percent() > chance)
        {
        act("$p shudders for a moment then flares up and vaporises!",ch,obj,0,TO_ROOM);
        act("$p shudders for a moment then flares up and vaporises!",ch,obj,0,TO_CHAR);
        extract_obj(obj);
        return;
        }

        if (type == ITEM_ARMOR || type == ITEM_FURNITURE)
                mob = create_mobile(get_mob_index(MOB_VNUM_ARMOR));
        else if (type == ITEM_WEAPON)
                mob = create_mobile(get_mob_index(MOB_VNUM_WEAPON));
        else
                mob = create_mobile(get_mob_index(MOB_VNUM_KEY));

        free_string(mob->short_descr);
        mob->short_descr = str_dup(obj->short_descr);
        free_string(mob->name);
        mob->name = str_dup(obj->name);
        sprintf(buf,"%s is here awaiting it's orders.\n\r",capitalize(mob->short_descr));
        free_string(mob->long_descr);
        mob->long_descr = str_dup(buf);
        mob->level = ilevel;

        if (type == ITEM_ARMOR)
        {
        hp = (50*ilevel);
        ac = (50 - 5*ilevel);
        move = ilevel*10;
        acm = (-ilevel*2);
        damroll = ilevel/3;
        if (IS_SET(obj->wear_flags,ITEM_WEAR_BODY))
                SET_BIT(mob->off_flags,OFF_BASH);
        if (IS_SET(obj->wear_flags,ITEM_WEAR_HANDS)
        || IS_SET(obj->wear_flags,ITEM_WEAR_SHIELD))
                SET_BIT(mob->off_flags,OFF_PARRY);
        }
        else if (type == ITEM_FURNITURE)
        {
        hp = (40*ilevel);
        ac = (100 - 4*ilevel);
        move = ilevel;
        acm = -(ilevel);
        damroll = ilevel/4;
        SET_BIT(mob->off_flags,OFF_BASH);
        }
        else if (type == ITEM_KEY)
        {
        hp = ilevel;
        ac = 100;
        move = 400;
        acm = 100;
        damroll = 0;
        }
        else
        {
	        hp = ilevel*25;
	        ac = 100 - (4*ilevel);
	        acm = 100 - (4*ilevel);
	        move = ilevel*3;
	        damroll = (ilevel);
			if (type == ITEM_WEAPON) {
				mob->dam_type = obj->value[3];
				switch (obj->value[0]) {
						case WEAPON_EXOTIC: mob->material = str_dup("exotic"); break;
						case WEAPON_SWORD: mob->material = str_dup("sword"); break;
						case WEAPON_DAGGER: mob->material = str_dup("dagger"); break;
						case WEAPON_SPEAR: mob->material = str_dup("spear"); break;
						case WEAPON_MACE: mob->material = str_dup("mace"); break;
						case WEAPON_AXE: mob->material = str_dup("axe"); break;
						case WEAPON_FLAIL: mob->material = str_dup("flail"); break;
						case WEAPON_WHIP: mob->material = str_dup("whip"); break;
						case WEAPON_POLEARM: mob->material = str_dup("polearm"); break;
						case WEAPON_STAFF: mob->material = str_dup("staff"); break;
						case WEAPON_HAND: mob->material = str_dup("hands");break;
				}
				if (IS_SET(obj->value[4],WEAPON_TWO_HANDS)) damroll *= 1.5;
			}
        }
        mob->max_move = move;
        mob->move = move;
        mob->damroll = damroll;
		mob->hitroll = damroll;
        mob->max_hit = hp;
        mob->hit = hp;

        SET_BIT(mob->affected_by,AFF_CHARM);
		char_to_room(mob,ch->in_room);
		act("$p shudders and slowly rises into the air to follow $n!",ch,obj,0,TO_ROOM);
		act("$p shudders for a moment then slowly rises up beside you!",ch,obj,0,TO_CHAR);
		act("$N now follows you.",ch,0,mob,TO_CHAR);
        extract_obj(obj);

        mob->leader = ch;
        mob->master = ch;
        return;
}

void spell_deathspell(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;
        int klevel, dam, bonus;
    char buf[MAX_STRING_LENGTH];
        klevel = level - 7;

        act("$n utters a word of power and the negative energy explodes in the room!",ch,0,0,TO_ROOM);
send_to_char("You utter a word of power and negative energy explodes into the room!\n\r",ch);
bonus = 0;
dam = dice(klevel,4);
        for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
        {
        vch_next=  vch->next_in_room;
        if (is_safe(ch,vch))
                continue;
	if (!IS_NPC(ch) && !IS_NPC(vch)
	&& (ch->fighting == NULL || vch->fighting == NULL))
	{
		switch(number_range(0,2))
		{
		case (0):
		case (1):
		sprintf(buf,"Die, %s you sorcerous dog!",PERS(ch,vch));
		break;
		case (2):
		sprintf(buf,"Help! %s is casting a spell on me!",PERS(ch,vch));
		}
	if (vch != ch)
		do_myell(vch,buf);
	}

        if (vch->level > klevel
        || IS_SET(vch->act,ACT_UNDEAD)
        || IS_SET(vch->imm_flags,IMM_NEGATIVE))
        {
        act("$n is unaffected by the negative energy field.",vch,0,0,TO_ROOM);
        act("You are unaffected by the negative energy field.",vch,0,0,TO_CHAR);
        }
        else if (vch == ch)
                bonus = 6;
        else
                bonus = 0;

        if (saves_spell(klevel - bonus, vch, DAM_NEGATIVE)
 	|| saves_spell(klevel,vch,DAM_NEGATIVE))
        {
                if (saves_spell(level - bonus, vch, DAM_NEGATIVE))
                        damage_old(ch,vch,dam/2,sn,DAM_NEGATIVE,TRUE);
                else
                        damage_old(ch,vch,dam,sn,DAM_NEGATIVE,TRUE);
        }
        else
        {
        act("$n gets a horrible look in $s eye's then keels over dead!",vch,0,0,TO_ROOM);
send_to_char("You feel your an intense pain in your head as the energy ruptures your skull!\n\r",vch);
raw_kill(ch,vch);
        }
        }
        return;
}

void spell_lifebane(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;
	char buf[MAX_STRING_LENGTH];

        int sn_poison, sn_curse, sn_weaken;
        int dam, tmp_dam;
        bool inside = FALSE;

        sn_poison = skill_lookup("poison");
        sn_curse = skill_lookup("curse");
        sn_weaken = skill_lookup("weaken");

        if (IS_SET(ch->in_room->room_flags,ROOM_INDOORS)
        || ch->in_room->sector_type == SECT_INSIDE)
                inside = TRUE;

        dam = dice(level,4);
        for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
        {
        vch_next = vch->next_in_room;
        if (is_safe(ch,vch))    continue;
        if (!(inside) && is_same_group(ch,vch)) continue;
        tmp_dam = dam;
        if (vch == ch)
                tmp_dam /= 2;
	if (!IS_NPC(ch) && !IS_NPC(vch)
	&& (ch->fighting == NULL || vch->fighting == NULL))
	{
		switch(number_range(0,2))
		{
		case (0):
		case (1):
		sprintf(buf,"Die, %s you sorcerous dog!",PERS(ch,vch));
		break;
		case (2):
		sprintf(buf,"Help! %s is casting a spell on me!",PERS(ch,vch));
		}
	if (vch!= ch)
		do_myell(vch,buf);
	}

if (saves_spell(level,vch,DAM_NEGATIVE))
        damage_old(ch,vch,dam/2,sn,DAM_NEGATIVE,TRUE);
else
        damage_old(ch,vch,dam,sn,DAM_NEGATIVE,TRUE);
spell_poison(sn_poison,level - 5,ch,vch,target);
spell_weaken(sn_weaken,level - 5,ch,vch,target);
spell_curse(sn_curse,level - 8, ch,vch,target);
        }
        return;
}

void spell_curse_weapon(int sn,int level,CHAR_DATA *ch, void *vo,int target)
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf;
    AFFECT_DATA af;
    int result, fail, mod2;
    int hit_bonus, dam_bonus, added;
    bool hit_found = FALSE, dam_found = FALSE;

    if (obj->item_type != ITEM_WEAPON)
    {
	send_to_char("That isn't a weapon.\n\r",ch);
	return;
    }

    if (obj->wear_loc != -1)
    {
	send_to_char("You must be carrying the weapon to curse it.\n\r",ch);
	return;
    }

	if (is_affected_obj(obj,skill_lookup("unholy bless")))
		return send_to_char("The unholy aura surrounding that prevents you.\n\r",ch);
    /* this means they have no bonus */
    hit_bonus = 0;
    dam_bonus = 0;
    fail = 25;	/* base 25% chance of failure */

    /* find the bonuses */

    if (!obj->enchanted)
    	for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
    	{
            if ( paf->location == APPLY_HITROLL && paf->type == sn )
            {
	    	hit_bonus = paf->modifier;
		hit_found = TRUE;
	    	fail += 2 * (hit_bonus * hit_bonus);
 	    }

	    else if (paf->location == APPLY_DAMROLL && paf->type == sn )
	    {
	    	dam_bonus = paf->modifier;
		dam_found = TRUE;
	    	fail += 2 * (dam_bonus * dam_bonus);
	    }

	    else  /* things get a little harder */
	    	fail += 25;
    	}

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location == APPLY_HITROLL && paf->type == sn )
  	{
	    hit_bonus = paf->modifier;
	    hit_found = TRUE;
	    fail += 2 * (hit_bonus * hit_bonus);
	}

	else if (paf->location == APPLY_DAMROLL && paf->type == sn )
  	{
	    dam_bonus = paf->modifier;
	    dam_found = TRUE;
	    fail += 2 * (dam_bonus * dam_bonus);
	}

	else /* things get a little harder */
	    fail += 25;
    }

    /* apply other modifiers */
    fail -= 3 * level/2;

    if (IS_OBJ_STAT(obj,ITEM_BLESS))
	fail += 15;
    if (IS_OBJ_STAT(obj,ITEM_GLOW))
	fail += 10;

    fail = number_range(5,95);

    result = number_percent();

    /* the moment of truth */
    if (result < (fail / 4))  /* item destroyed */
    {
	act("$p shivers violently and explodes!",ch,obj,NULL,TO_CHAR);
	act("$p shivers violently and explodes!",ch,obj,NULL,TO_ROOM);
	extract_obj(obj);
	return;
    }

    if (result < (fail / 2)) /* item disenchanted */
    {
	AFFECT_DATA *paf_next;

	act("$p is surrounded in a sickly red aura, then fades.",ch,obj,NULL,TO_CHAR);
	act("$p is surroudned in a sickly red aura, then fades.",ch,obj,NULL,TO_ROOM);
	obj->enchanted = TRUE;

	/* remove all affects */
	for (paf = obj->affected; paf != NULL; paf = paf_next)
	{
	    paf_next = paf->next;
	    free_affect(paf);
	}
	obj->affected = NULL;

	remove_bit(&obj->extra_flags, ITEM_GLOW);
	remove_bit(&obj->extra_flags, ITEM_HUM);
	remove_bit(&obj->extra_flags, ITEM_MAGIC);
	remove_bit(&obj->extra_flags, ITEM_INVIS);
	remove_bit(&obj->extra_flags, ITEM_NODROP);
	remove_bit(&obj->extra_flags, ITEM_NOREMOVE);
	return;
    }

    if ( result <= fail )  /* failed, no bad result */
    {
	send_to_char("Nothing seemed to happen.\n\r",ch);
	return;
    }

    /* okay, move all the old flags into new vectors if we have to */
    if (!obj->enchanted)
    {
	AFFECT_DATA *af_new;
	obj->enchanted = TRUE;

	for (paf = obj->pIndexData->affected; paf != NULL; paf = paf->next)
	{
	    af_new = new_affect();

	    af_new->next = obj->affected;
	    obj->affected = af_new;

	    af_new->where	= paf->where;
	    af_new->type 	= UMAX(0,paf->type);
	    af_new->level	= paf->level;
	    af_new->duration	= paf->duration;
	    af_new->location	= paf->location;
	    af_new->modifier	= paf->modifier;
	    af_new->bitvector	= paf->bitvector;
	}
    }

init_affect(&af);
af.type = sn;
af.where = TO_OBJECT;
af.modifier = -1;
af.duration = -1;
af.bitvector = 0;
af.level = level;
	mod2 = number_range(0,6);
	if (mod2 == 0)
		af.location = APPLY_STR;
	else if (mod2 == 1)
		af.location = APPLY_DEX;
	else if (mod2 == 2)
		af.location = APPLY_CON;
	else if (mod2 == 3)
		af.location = APPLY_INT;
	else if (mod2 == 4)
		af.location = APPLY_WIS;
	else if (mod2 == 5)
	{
		af.modifier = -(number_range(5,20));
		af.location = APPLY_HIT;
	}
	else
	{
		af.location = APPLY_MANA;
		af.modifier = -(number_range(5,20));
	}



    if (result <= (100 - level/5))  /* success! */
    {
	act("$p glows with a sickly green aura.",ch,obj,NULL,TO_CHAR);
	act("$p glows with a sickly green aura.",ch,obj,NULL,TO_ROOM);
if (number_percent() < 20)
affect_to_obj(obj,&af);
	added = 2;
    }

    else  /* exceptional enchant */
    {
	act("$p burns a fierce red!",ch,obj,NULL,TO_CHAR);
	act("$p glows a brillant blue!",ch,obj,NULL,TO_ROOM);
affect_to_obj(obj,&af);
	set_bit(&obj->extra_flags,ITEM_NOREMOVE);
	added = 4;
    }

    if ( (result = number_percent()) <= 25 && is_set(&obj->extra_flags,ITEM_HUM) )
    {
	remove_bit(&obj->extra_flags,ITEM_HUM);
	remove_bit(&obj->extra_flags,ITEM_GLOW);
    }
    if ( (result = number_percent()) <= 25 && is_set(&obj->extra_flags,ITEM_GLOW) )
	remove_bit(&obj->extra_flags,ITEM_GLOW);

    /* now add the enchantments */

    if (obj->level < LEVEL_HERO - 1)
	obj->level = UMIN(LEVEL_HERO - 1,obj->level);

    if (dam_found)
    {
	for ( paf = obj->affected; paf != NULL; paf = paf->next)
	{
	    if ( paf->location == APPLY_DAMROLL && paf->type == sn )
	    {
		paf->type = sn;
		paf->modifier += added;
		paf->level = UMAX(paf->level,level);
	    }
	}
    }
    else /* add a new affect */
    {
	paf = new_affect();

	paf->where	= TO_OBJECT;
	paf->type	= sn;
	paf->level	= level;
	paf->duration	= -1;
	paf->location	= APPLY_DAMROLL;
	paf->modifier	=  added;
	paf->bitvector  = 0;
    	paf->next	= obj->affected;
    	obj->affected	= paf;
    }

    if (hit_found)
    {
        for ( paf = obj->affected; paf != NULL; paf = paf->next)
	{
            if ( paf->location == APPLY_HITROLL && paf->type == sn )
            {
		paf->type = sn;
                paf->modifier += added;
                paf->level = UMAX(paf->level,level);
            }
	}
    }
    else /* add a new affect */
    {
        paf = new_affect();

        paf->type       = sn;
        paf->level      = level;
        paf->duration   = -1;
        paf->location   = APPLY_HITROLL;
        paf->modifier   =  added;
        paf->bitvector  = 0;
        paf->next       = obj->affected;
        obj->affected   = paf;
    }
}


void spell_globe_of_darkness(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    int chance;
    AFFECT_DATA af;
	ROOM_AFFECT_DATA raf;

    if (is_affected(ch,sn))
    {
send_to_char("You are not yet able to call upon your power of the darkness.\n\r",ch);
return;
}

	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
    af.type = sn;
    af.level = level;
    af.duration = 8;
    af.location = 0;
    af.modifier = 0;
    af.bitvector = 0;
     chance = ch->level;
     chance *= get_skill(ch,sn);
     chance /= 100;

    if (IS_SET(ch->in_room->room_flags,ROOM_DARK))
		chance += 15;
    if (IS_SET(ch->in_room->room_flags,ROOM_NO_CONSECRATE))
		chance += 5;
    if (IS_SET(ch->in_room->room_flags,ROOM_CONSECRATED))
		chance -= 15;
    if (IS_SET(ch->in_room->room_flags,ROOM_LAW))
		chance = 0;
    if (IS_SET(ch->in_room->room_flags,ROOM_LOW_ONLY))
		chance = 0;


	act("$n utters a word of drow power and the room becomes as black as darkest night.",ch,0,0,TO_ROOM);
	send_to_char("You utter a word of drow power and darkness enshrouds the room!\n\r",ch);

	init_affect_room(&raf);
	raf.where = TO_ROOM_AFFECTS;
	raf.aftype = AFT_SPELL;
	raf.type = sn;
	raf.level = level;
	raf.duration = 6;
	affect_to_room(ch->in_room, &raf);
	
	af.duration = 24;
	af.affect_list_msg = str_dup("unable to call another globe of darkness");
	affect_to_char(ch,&af);

return;
}


/* Green tower obj progs */


/* sceptre of heavens...say 'i am the wrath of god' */
void spell_heavenly_sceptre_frenzy(int sn,int level, CHAR_DATA *ch,void *vo,int target)
{
        AFFECT_DATA af;
        if (is_affected(ch,sn) || (!IS_GOOD(ch) && !IS_EVIL(ch)))
        {
        act("$n's sceptre hums softly but nothing seems to happen.",ch,0,0,TO_ROOM);
        act("Your sceptre hums softly but nothing seems to happen.",ch,0,0,TO_CHAR);
        return;
        }
	init_affect(&af);
        af.type = sn;
        af.where = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
        af.duration = 10;
        af.modifier = ch->level/7;
        af.location = APPLY_DAMROLL;
        af.bitvector = 0;
        af.level = level;
        affect_to_char(ch,&af);
        af.location = APPLY_HITROLL;
        affect_to_char(ch,&af);
	if (ch->level > 40)
	af.location = APPLY_SAVING_SPELL;
	af.modifier = -ch->level/10;

        if (IS_GOOD(ch))
        {
        act("$n's sceptre glows white and a look of righteous anger in $s eyes.",ch,0,0,TO_ROOM);
send_to_char("Your sceptre glows white and you feel a surge of righteous anger!\n\r",ch);
        }
        else if (IS_EVIL(ch))
        {
        act("$n's sceptre darkens and a look of righteous anger enters $s eyes.",ch,0,0,TO_ROOM);
send_to_char("Your sceptre darkens and you feel a surge of righteous anger!\n\r",ch);
        }

        return;
}


/* sceptre of heavens, say 'Feel the force of god' */
void spell_heavenly_sceptre_fire(int sn, int level, CHAR_DATA *ch,void *vo,int target)
{
        AFFECT_DATA af;
        OBJ_DATA *sceptre = NULL;
        int dam;
        CHAR_DATA *victim;

        victim = ch->fighting;
        if (is_affected(ch,sn) || victim == NULL || ch->level < 30)
        {
        act("$n's sceptre hums softly but nothing seems to happen.",ch,0,0,TO_ROOM);
        send_to_char("Your sceptre hums softly but nothing seems to happen.\n\r",ch);
        WAIT_STATE(ch,12);
        return;
        }

        dam = dice(15,20);

        if (saves_spell(ch->level,victim,DAM_HOLY))
                dam /= 2;
        if (saves_spell(65,victim,DAM_OTHER))
                dam /= 2;

        damage_old(ch,victim, dam,sn,DAM_HOLY,TRUE);

        if (IS_NPC(ch))
                return;
        if (number_percent() > ch->level * 2)
        {
        act("Your sceptre of heavenly orders crumbles to dust.",ch,0,0,TO_ROOM);
        send_to_char("Your sceptre of heavenly orders crumbles to dust.\n\r",ch);
        extract_obj(sceptre);
        return;
        }
	init_affect(&af);
        af.where = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
        af.type = sn;
        af.modifier = 0;
        af.duration = 1;
        af.location = 0;
        af.bitvector = 0;
        af.level = ch->level;
        affect_to_char(ch,&af);
        return;
}

void spell_soulbind(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    return;
}

void spell_lightshield( int sn, int level, CHAR_DATA *ch, void *vo, int
target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (!IS_GOOD(victim))
    {
	if (victim == ch)
	  send_to_char("You are not pure enough to receive the gift of light.\n\r",ch);
	else
    	  act("$N is not pure enough to receive the gift of light.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You are already protected.\n\r",ch);
	else
	  act("$N is already protected.",ch,NULL,victim,TO_CHAR);
	return;
    }
	init_affect(&af);
    af.where	 = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
    af.type      = sn;
    af.level	 = level;
    af.duration  = level;
    af.modifier  = -level;
    af.location  = APPLY_AC;
	af.modifier	 = 2 * AC_PER_ONE_PERCENT_DECREASE_DAMAGE;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    af.location = APPLY_SAVING_SPELL;
    af.modifier = -level/10;
    af.bitvector = AFF_INFRARED;
    affect_to_char( victim, &af );
    send_to_char( "You are surrounded by a glowing afflatus of purity.\n\r", victim );
    if ( ch != victim )
	act("$N is surrounded by a glowing afflatus of purity.",ch,NULL,victim,TO_CHAR);
    return;
}


void spell_wrath_of_purity( int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;
	char buf[MAX_STRING_LENGTH];

        int dam;

        dam = dice(level,18);

        for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
        {
        vch_next = vch->next_in_room;
        if (is_same_group(vch,ch))
                continue;
	if (!IS_EVIL(vch))
		continue;
	if (!IS_NPC(ch) && !IS_NPC(vch)
	&& (ch->fighting == NULL || vch->fighting == NULL))
	{
		switch(number_range(0,2))
		{
		case (0):
		case (1):
		sprintf(buf,"Help! %s has called the wrath of purity upon me!",PERS(ch,vch));
		break;
		case (2):
		sprintf(buf,"Help! %s has called the wrath of purity upon me!",PERS(ch,vch));
		}
	if (vch!= ch)
		do_myell(vch,buf);
	}

 damage_old(ch,vch, (saves_spell(level,vch,DAM_HOLY) ? dam/2 : dam), sn,DAM_HOLY,TRUE);
 	WAIT_STATE(ch,PULSE_VIOLENCE*3);

        }
        return;
}

void spell_frostbolt(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if (victim == ch)
    {
        act("$n is blasted with a bolt of ice!",ch,0,0,TO_ROOM);
        send_to_char("You are blasted by a bolt of ice!\n\r",ch);
    }
    else
    {
    act("$n points at $N and a bolt of ice flies forth!",ch,0,victim,TO_NOTVICT);
    act("$n points at you and a bolt of ice flies forth!",ch,0,victim,TO_VICT);
    act("You point at $N and a bolt of ice flies forth!",ch,0,victim,TO_CHAR);
    }
    dam = dice(level,7);
    if (saves_spell(level,victim,DAM_COLD))
        dam /= 2;
    damage_old(ch,victim,dam,sn,DAM_COLD,TRUE);
    return;
}

void spell_icelance(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    if (victim == ch)
    {
        act("$n throws out a shard of sharp ice!",ch,0,0,TO_ROOM);
        send_to_char("You are struck by your shard of ice!\n\r",ch);
    }
    else
    {
    	act("$n throws forth a shard of sharp ice at $N!",ch,0,victim,TO_NOTVICT);
    	act("$n throws forth a shard of sharp ice at you!",ch,0,victim,TO_VICT);
    	act("You point at $N and throw forth a shard of ice!",ch,0,victim,TO_CHAR);
    }
    dam = dice(level,6);
    if (saves_spell(level,victim,DAM_COLD))
        dam /= 2;
    damage_old(ch,victim,dam,sn,DAM_COLD,TRUE);
    if (number_percent() < 30 && !is_affected(victim,gsn_icelance))
    {
		AFFECT_DATA af;
		act("A fine mist of blood sprays forth from $N as $n's icelance pierces deep into $S chest!",ch,0,victim,TO_NOTVICT);
		act("A mist of blood sprays forth as $n's icelance pierces deep into your chest!",ch,0,victim,TO_VICT);
		act("A fine mist of blood sprays forth from $N as your icelance pierces deep into $S chest!",ch,0,victim,TO_CHAR);
		init_affect(&af);
		af.aftype 	= AFT_SKILL;
   		af.type   	= gsn_icelance;
		af.duration 	= 5;
		af.affect_list_msg = str_dup("induces major bleeding");
		af.name		= str_dup("icelance wound");
		af.level	= ch->level;
		af.owner_name	= str_dup(ch->original_name);
		affect_to_char(victim,&af);
   	}
    return;
}

void spell_earthfade(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    AFFECT_DATA af;
    int sn_fog, sn_fire;
    sn_fog = skill_lookup("faerie fog");
    sn_fire = skill_lookup("faerie fire");
    if (is_affected(ch,sn))
    {
    send_to_char("You are already melded with the ground.\n\r",ch);
    return;
    }
    if (ch->in_room->sector_type == SECT_WATER_SWIM
    || ch->in_room->sector_type == SECT_WATER_NOSWIM
    || ch->in_room->sector_type == SECT_AIR)
    {
    send_to_char("There isn't enough natural earth to meld with here.\n\r",ch);
    return;
    }
    if (is_affected(ch,sn_fog) || is_affected(ch,sn_fire))
    {
    send_to_char("You are glowing too much to hide in the ground.\n\r",ch);
    return;
    }
    if ( ch->pause > 3 )
    {
    send_to_char("You are breathing too heavily to hide in the ground.\n\r",ch);
    return;
    }
    act("$n vanishes into the ground!",ch,0,0,TO_ROOM);
    send_to_char("You vanish into the ground!\n\r",ch);

	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
    af.type = gsn_earthfade;
    af.level = level;
    af.modifier = 0;
    af.bitvector = 0;
    af.affect_list_msg = str_dup("grants improved invisibility");
    af.duration = (level/5);
    af.location = 0;
    affect_to_char(ch,&af);

    return;
}

void spell_forget(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    AFFECT_DATA af;
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if (is_affected(victim,sn))
    {
    if (victim == ch)
        send_to_char("You are already as senile as you can get.\n\r",ch);
    else
        send_to_char("They are already affected by a forget spell.\n\r",ch);
    return;
    }

    if (saves_spell(level - 3,victim,DAM_OTHER))
    {
    send_to_char("Spell failed.\n\r",ch);
    return;
    }
    init_affect(&af);
    af.type = sn;
    af.modifier = 0;
    af.duration = level/10;
    af.location = 0;
    af.bitvector = 0;
    af.level = level;
    af.affect_list_msg = str_dup("induces memory loss of skills and spells");
    af.where = TO_AFFECTS;
    af.aftype    = AFT_SPELL;
    affect_to_char(victim,&af);
    send_to_char("You feel your memories slip away.\n\r",victim);
    act("$n suddenly looks disoriented.",victim,0,0,TO_ROOM);
    return;
}

void spell_earthbind(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    AFFECT_DATA af;
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if (is_affected(victim,sn) || !IS_AFFECTED(victim,AFF_FLYING))
    {
    send_to_char("Their feet are already on the ground.\n\r",ch);
    return;
    }
    if (saves_spell(level,victim,DAM_OTHER))
    {
    send_to_char("Spell failed.\n\r",ch);
    return;
    }
	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
	af.type = gsn_earthbind;
    af.modifier = 0;
    af.level =  level;
    af.duration = level/5;
    af.location = 0;
    af.bitvector = 0;
    affect_to_char(victim,&af);
    send_to_char("Your feet suddenly become heavy and earthbound.\n\r",victim);
    act("$n suddenly drops to the ground.",victim,0,0,TO_ROOM);
    return;
}

void spell_divine_touch(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    AFFECT_DATA af;
    if (is_affected(ch,sn))
    {
    send_to_char("You are already blessed with a divine touch.\n\r",ch);
    return;
    }
	init_affect(&af);
    af.type = sn;
	af.aftype    = AFT_SPELL;
    af.modifier = 0;
    af.location = 0;
    af.bitvector = 0;
    af.duration = level/5;
    af.level = level;
    affect_to_char(ch,&af);
    send_to_char("Your hands are surrounded in holy energy.\n\r",ch);
    act("$n's hands seem to glow with an inner energy.",ch,0,0,TO_ROOM);
    return;
}

void spell_transfer_object(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
   char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];

    target_name = one_argument(target_name,arg1);
    one_argument(target_name,arg2);

	if (arg1[0] == '\0'
	|| arg2[0] == '\0')
	{
	send_to_char("Send what to whom?\n\r",ch);
	return;
 }

	if ( ( obj = get_obj_carry( ch, arg1, ch ) ) == NULL )
	{
	    send_to_char( "You are not carrying that.\n\r", ch );
	    return;
	}


    victim = get_char_world(ch,arg2);
	if (victim == NULL)
	{
send_to_char("They aren't here.\n\r",ch);
	return;
	}

    if (IS_NPC(victim))
    {
    send_to_char("They wouldn't want that sent to them.\n\r",ch);
    return;
    }
    if (victim == ch)
	{
send_to_char("Why not just hold it in your other hand?\n\r",ch);
	return;
	}
    if (IS_IMMORTAL(victim) && !IS_IMMORTAL(ch))
    {
    send_to_char("You can't send to an immortal.\n\r",ch);
    return;
    }
    if (is_set(&obj->extra_flags,ITEM_NODROP))
    {
    send_to_char("You can't seem to remove it from yourself to prepare for transfer.\n\r",ch);
    return;
    }
    if (victim->carry_number >= can_carry_n(victim)
    || victim->carry_weight >= can_carry_w(victim)
    || obj->pIndexData->vnum == OBJ_VNUM_ARCANA
    || obj->pIndexData->vnum == OBJ_VNUM_RAGER
    || obj->pIndexData->vnum == OBJ_VNUM_ANCIENT
    || obj->pIndexData->vnum == OBJ_VNUM_KNIGHT
    || obj->pIndexData->vnum == OBJ_VNUM_OUTLAW
    || obj->pIndexData->vnum == OBJ_VNUM_BOUNTY
    || obj->pIndexData->vnum == OBJ_VNUM_EMPIRE
    || obj->pIndexData->vnum == OBJ_VNUM_ENFORCER)
    {
    act("$p glows softly but nothing happens.",ch,obj,0,TO_CHAR);
    act("$p glows softly but nothing happens.",ch,obj,0,TO_ROOM);
    return;
    }

    if (IS_SET(victim->act,PLR_NO_TRANSFER))
    {
	send_to_char("They wouldn't want them sent to them.\n\r",ch);
    return;
    }

    act("$p glows softly and vanishes!",ch,obj,0,TO_CHAR);
    act("$p glows softly and vanishes!",ch,obj,0,TO_ROOM);
    obj_from_char(obj);
    obj_to_char(obj,victim);
act("You suddenly feel heavier as $p pops into your inventory!",victim,obj,0,TO_CHAR);
act("$p suddenly appears from nowhere and pops into $n's possession!",victim,obj,0,TO_ROOM);
    return;
}

void spell_disintegrate(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    int dam, obj_save, count=0;

    if (saves_spell(level - 1,victim,DAM_ENERGY)
    	|| saves_spell(level - 4,victim,DAM_ENERGY)
	|| saves_spell(level + 12, victim,DAM_ENERGY))
    {
    	act("$n shudders and spasms momentarily.",victim,0,0,TO_ROOM);
    	send_to_char("You feel a sudden intense pain throughout your body!\n\r",victim);
    	dam = dice(level,10);
	damage_old(ch,victim,saves_spell(level,victim,DAM_ENERGY) ? dam/2 : dam,sn,DAM_ENERGY,TRUE);
    	WAIT_STATE(ch,PULSE_VIOLENCE*2);
    	return;
    }

    act("$n vaporizes in a cloud of bloody red mist!",victim,0,0,TO_ROOM);
    send_to_char("You feel a sudden intense pain as your body vaporizes into a red mist!\n\r",victim);

    for (obj = victim->carrying; obj != NULL; obj = obj_next)
    {
	obj_next = obj->next_content;
	obj_save = obj->level;

	if (obj_save == 0)
		obj_save = 51;


	if (obj->pIndexData->limtotal == 1)
		obj_save += 15;
	else if (obj->pIndexData->limtotal <= 3
	&& obj->pIndexData->limtotal > 0)
		obj_save += 10;
	else if (obj->pIndexData->limtotal <= 10
	&& obj->pIndexData->limtotal > 0)
		obj_save += 5;
	else if (obj->pIndexData->limtotal <= 15
	&& obj->pIndexData->limtotal > 0)
		obj_save += 3;
	else if (obj->pIndexData->limtotal > 0)
		obj_save += 1;

   	obj_save *= 2;
	obj_save -= (2 * level);
	obj_save += 55;

	if (number_percent() > obj_save && count < 9)
	{
		act("$p disintergrates in a flash of light!",ch,obj,NULL,TO_ROOM);
		act("$p disintergrates in a flash of light!",ch,obj,NULL,TO_CHAR);
		extract_obj(obj);
		count++;
	}
    }

    raw_kill(ch,victim);
    WAIT_STATE(ch,PULSE_VIOLENCE*3);
    return;

}

void spell_grounding(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    AFFECT_DATA af;

    if (is_affected(ch,sn))
    {
    send_to_char("Your body is already at a ground potential.\n\r",ch);
    return;
    }

    send_to_char("Your body loses all affinity to electricity.\n\r",ch);
	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
    af.type = sn;
    af.modifier = 0;
    af.level = level;
    af.duration = ch->level/8;
    af.bitvector = 0;
	af.affect_list_msg = str_dup("provides immunity to lightning-based spells");
    affect_to_char(ch,&af);
    return;
}

void spell_shock_sphere(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    int dam;

    act("A sphere of crackling energy detonates around $n with explosive sound!", victim,0,0,TO_ROOM);
    send_to_char("A sphere of fierce energy detonates around you with deafening sound!\n\r",victim);

    if (is_affected(victim,gsn_shock_sphere || is_affected(victim,gsn_deafen)))
	{
	dam = dice(level, 3);
	damage_old(ch,victim,saves_spell(level,victim,DAM_SOUND) ? dam/2 : dam,sn,DAM_SOUND,TRUE);
	return;
	}
    if (saves_spell(level,victim,DAM_SOUND))
    {
	dam = dice(level,5);
	damage_old(ch,victim,saves_spell(level,victim,DAM_SOUND) ? dam/2 : dam,sn,DAM_SOUND,TRUE);
    return;
    }

	dam = dice(level,7);
	damage_old(ch,victim,saves_spell(level,victim,DAM_SOUND) ? dam/2 : dam,sn,DAM_SOUND,TRUE);

	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
    af.modifier = -2;
    af.location = APPLY_HITROLL;
    af.bitvector = 0;
    af.type = gsn_shock_sphere;
    af.level = level;
    af.duration = 2;

    if (is_affected(victim,gsn_deafen))
	{
		return;
	}

    affect_to_char(victim,&af);
    af.location = 0;
    af.modifier = 0;
    af.affect_list_msg = str_dup("prevents casting and inflicts a deafened state");
    act("$n appears deafened.",victim,0,0,TO_ROOM);
    send_to_char("You can't hear a thing!\n\r",victim);
    return;
}

/* Make a victim flee...if they really fail their saves big time it can
kill them outright..(Ceran)
*/
void spell_power_word_fear(int sn, int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    bool bad_fail, utter_fail;
    int range;
    /*
    if (get_eq_char(victim, WEAR_BRAND)->pIndexData->vnum == 67)
    {
	send_to_char("Your spell has no affect on the fearless.\n\r",ch);
	return;
    }*/
    bad_fail = FALSE;
    utter_fail = FALSE;

    if (victim == ch)
    {
        send_to_char("That wouldn't work.\n\r",ch);
        return;
    }

    act("$n points at $N and utters the word 'Fear!'",ch,0,victim,TO_NOTVICT);
    act("$n points at you and utters the word 'Fear!'",ch,0,victim,TO_VICT);
    act("You point at $N and utter the word 'Fear!'",ch,0,victim,TO_CHAR);

    if (!IS_AWAKE(victim))
    {
        act("$n shivers momentarily but it passes.",victim,0,0,TO_ROOM);
        send_to_char("You feel a brief terror, but it passes away in your dreams.\n\r",victim);
        return;
    }
    if (is_affected(victim,sn))
    {
	send_to_char("They are already affected by a word of power.\n\r",ch);
	send_to_char("You feel a shiver pass through you but it has no further affect.\n\r",victim);
	return;
    }

    if (IS_NPC(victim))
    {
	if (victim->pIndexData->vnum == MOB_VNUM_ANCIENT
	|| victim->pIndexData->vnum == MOB_VNUM_BOUNTY
	|| victim->pIndexData->vnum == MOB_VNUM_RAGER
      || victim->pIndexData->vnum == MOB_VNUM_OUTLAW
	|| victim->pIndexData->vnum == MOB_VNUM_KNIGHT
	|| victim->pIndexData->vnum == MOB_VNUM_ARCANA
 	|| victim->pIndexData->vnum == MOB_VNUM_ENFORCER)
	{
        act("$n shivers momentarily but it passes.",victim,0,0,TO_ROOM);
        send_to_char("You feel a brief terror, but it passes.\n\r",victim);
        return;
        }
    }


    if (saves_spell(level,victim,DAM_OTHER))
    {
    act("$n shivers momentarily but it passes.",victim,0,0,TO_ROOM);
    send_to_char("You feel a brief terror, but it passes.\n\r",victim);
    return;
    }

    if (!saves_spell(level - 2,victim,DAM_OTHER))
    {
        bad_fail = TRUE;
        if (!saves_spell(level - 5,victim,DAM_OTHER))
            utter_fail = TRUE;
    }


    if (utter_fail)
    {
    act("$n's eyes widen and $s heart ruptures from shock!",victim,0,0,TO_ROOM);
    send_to_char("You feel a terror so intense your heart stops dead!\n\r",victim);
        raw_kill(ch,victim);
        return;
    }
    act("$n's eyes widen in shock and $s entire body freezes in momentary terror.",victim,NULL,NULL,TO_ROOM);
    send_to_char("You feel an overwhelming terror and you shudder in momentary shock.\n\r",victim);

    range = level/10;

	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
    af.type = sn;
    af.level = level;
    af.bitvector = 0;
    af.duration = (number_range(1,5) + range);

    af.location = APPLY_CON;
    af.modifier = -number_range(2,range + 1);
    affect_to_char(victim,&af);
    af.location = APPLY_STR;
    af.modifier = -number_range(2,range + 1);
    affect_to_char(victim,&af);
    af.location = APPLY_DEX;
    af.modifier = -number_range(1,range);
    affect_to_char(victim,&af);
    af.location = APPLY_HIT;
    af.modifier = -number_range(2,range);
    affect_to_char(victim,&af);
    af.location = APPLY_DAMROLL;
    af.modifier = -number_range(2,range);
    affect_to_char(victim,&af);

    if (victim->position == POS_FIGHTING)
        do_flee(victim,"");

    if (victim->position == POS_FIGHTING)
    {
        do_flee(victim,"");
    }
    if (victim->position == POS_FIGHTING)
    {
        do_flee(victim,"");
    }

    if (bad_fail)
        WAIT_STATE(victim,12);
    return;
}


/* Causes a target's hp to regenerate at a constant rate, plus removes the
   effects of a wither prevent healing spell */
void spell_regeneration(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (is_affected(victim,gsn_atrophy))
	{
	act("$n's body stops wasting away.",victim,0,0,TO_ROOM);
send_to_char("Your body stops wasting away.\n\r",victim);
	affect_strip(victim,gsn_atrophy);
	return;
	}

    if (is_affected(victim,skill_lookup("prevent healing")))
    {
        act("$n's sickly looking complexion clears up.",victim,0,0,TO_ROOM);
        send_to_char("You no longer feel so sick and weary.\n\r",victim);
        affect_strip(victim,skill_lookup("prevent healing"));
        return;
    }

    if (is_affected(victim,skill_lookup("wither")))
    {
        if (victim != ch)
        {
act("$N's emanciated body heals up with your touch.",ch,0,victim,TO_CHAR);
act("Your emanciated body heals up with $n's touch.",ch,0,victim,TO_VICT);
act("$N's emanciated body heals up with $n's touch.",ch,0,victim,TO_NOTVICT);
        }
        else
        {
send_to_char("Your emaciated body parts heal up.\n\r",ch);
act("$n's emaciated body heals up.",ch,0,0,TO_ROOM);
        }
        affect_strip(victim,skill_lookup("wither"));
    return;
    }

    if (is_affected(victim,sn))
    {
        if (victim == ch)
        send_to_char("You are already healing at an improved rate.\n\r",ch);
        else
        send_to_char("They are already healing at an improved rate.\n\r",ch);
        return;
    }
        init_affect(&af);
        af.aftype = AFT_COMMUNE;
    af.where = TO_AFFECTS;
    af.type = sn;
    af.location = APPLY_REGENERATION;
	
    if (get_skill(ch,skill_lookup("greater regeneration")) < 5)
		af.modifier = 10;
	else {
		send_to_char("Your deity enhances the potency of the regeneration!\n\r",ch);
		af.modifier = 35;
	}
		
    af.level = level;
    af.bitvector = 0;
    af.duration = level/5;
    affect_to_char(victim,&af);

	send_to_char("You feel your body warm with an inner health.\n\r",victim);
	if (victim != ch)
		send_to_char("You boost their recuperation ability.\n\r",ch);

    return;
}

/*
    Restores forget, wither, prevent healing, atrophy.
    Also restores lost levels due to energy drain by powerful undead
*/
void spell_restoration(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int sn_forget, sn_wither, sn_drain, sn_prevent_healing;
    bool success = FALSE;

    sn_prevent_healing = skill_lookup("prevent healing");
    sn_forget = skill_lookup("forget");
    sn_wither = skill_lookup("wither");
    sn_drain = skill_lookup("undead drain");

    if (is_affected(victim,sn_forget))
    {
send_to_char("You feel your mind becoming clearer.\n\r",victim);
act("$n looks less confused.",victim,0,0,TO_ROOM);
affect_strip(victim,sn_forget);
success = TRUE;
    }
    if (is_affected(victim,sn_wither))
    {
send_to_char("Your emaciated body is restored.\n\r",victim);
act("$n's emaciated body looks healthier.",victim,0,0,TO_ROOM);
    affect_strip(victim,sn_wither);
    success = TRUE;
    }
    if (is_affected(victim,sn_prevent_healing))
    {
send_to_char("You no longer feel so sickly and weary.\n\r",victim);
act("$n's sickly complexion clears up.",victim,NULL,NULL,TO_ROOM);
affect_strip(victim,sn_prevent_healing);
success = TRUE;
    }

    if (is_affected(victim,gsn_atrophy))
	{
	act("$n's body stops wasting away.",victim,0,0,TO_ROOM);
send_to_char("Your body stops wasting away.\n\r",victim);
	affect_strip(victim,gsn_atrophy);
	}

    if (is_affected(victim,sn_drain)
	&& check_dispel(level + 15,victim,sn_drain))
    {
act("$n looks much better.",victim,NULL,NULL,TO_ROOM);
	affect_strip(victim,sn_drain);
    success = TRUE;
    }
	
	if ((is_affected(victim,skill_lookup("rot")))
	&& check_dispel(level, victim,skill_lookup("rot")))
    {
		act("$n stops decomposing.",victim,NULL,NULL,TO_ROOM);
		affect_strip(victim,skill_lookup("rot"));
		success = TRUE;
    }
	
    if (!success)
    send_to_char("Spell had no effect.\n\r",ch);

    return;
}

void spell_undead_drain(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    return;
}



void spell_prevent_healing(int sn,int level, CHAR_DATA *ch,void *vo,int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;

	if (is_affected(victim,sn))
	{
send_to_char("Their body is already deficient in healing ability.\n\r",ch);
	return;
	}

	if (saves_spell(level,victim,DAM_OTHER)
	|| !str_cmp(race_table[victim->race].name,"troll"))
	{
	send_to_char("Spell failed.\n\r",ch);
	return;
	}
	send_to_char("You feel your body losing it's ability to heal.\n\r",victim);
	act("$n looks very sick.",victim,0,0,TO_ROOM);
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype    = AFT_SPELL;
	af.type = gsn_prevent_healing;
	af.level = level;
	af.location = 0;
	af.modifier = 0;
	af.duration = level/6;
	af.affect_list_msg = str_dup("prevents healing");
	af.bitvector = 0;
	affect_to_char(victim,&af);

	return;
}

/* Fire and Ice spell for elementalists */
void spell_fire_and_ice(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam, sn_frost, sn_fire, d_type, sn_type;

    sn_frost = skill_lookup("frost breath");
    sn_fire = skill_lookup("fire breath");
    if (sn_fire == -1 || sn_frost == -1)
    {
        send_to_char("The elements fail to combine.\n\r",ch);
        return;
    }

    if (number_percent() > 50)
    {
        sn_type = sn_frost;
        d_type = DAM_COLD;
    }
    else
    {
        sn_type = sn_fire;
        d_type = DAM_FIRE;
    }
    if (check_spellcraft(ch,sn))
        dam = spellcraft_dam(level*3,4);
    else
        dam = dice(level*1.75,4);
    act("$n unleashes a blast of fire and ice!",ch,0,0,TO_ROOM);
    send_to_char("You unleash a blast of fire and ice!\n\r",ch);

    if(!IS_NPC(ch))
	dam/=2;

    damage_old(ch,victim,dam,sn_type,d_type,TRUE);

    if (check_spellcraft(ch,sn))
        dam = spellcraft_dam(level*3,4);
    else
        dam = dice(level*1.75, 4);

    if (d_type == DAM_COLD)
    {
        d_type = DAM_FIRE;
        sn_type = sn_fire;
    }
    else
    {
        d_type = DAM_COLD;
        sn_type = sn_frost;
    }

    if(!IS_NPC(ch))
	dam/=3;

    damage_old(ch,victim,dam,sn_type,d_type,TRUE);
    return;

}

void spell_atrophy(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;


    if (is_affected(victim,sn))
    {
        if (victim == ch)
            send_to_char("You are already wasting away.\n\r",ch);
        else
            send_to_char("They are already wasting away.\n\r",ch);
        return;
    }

    if (saves_spell(level,victim,DAM_OTHER)
	|| !str_cmp(race_table[victim->race].name,"troll"))
    {
        send_to_char("Spell failed.\n\r",ch);
        return;
    }
	init_affect(&af);
    af.where = TO_AFFECTS;
	af.aftype    = get_spell_aftype(ch);
    af.type = sn;
    af.duration = 2;
    af.location = APPLY_REGENERATION;
    af.modifier = -10;
    af.bitvector = 0;
    af.level = level;
    affect_to_char(victim,&af);

    send_to_char("You feel an intense pain as your body begins to waste away.\n\r",victim);
    act("$n looks very sick as $s body starts wasting away before your eyes!",victim,NULL,NULL,TO_ROOM);
    return;
}

/*
    Heal spell, but also for only slightly more cost can cure poison and disease.
    Can stop wasting, but no heal benefit is gained if used this way. Won't
    restore undead_drains or wither etc.
*/
void spell_utter_heal(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int sn_atrophy, sn_poison;

    if (IS_EVIL(ch))
    {
        send_to_char("The Gods of darkness do not grant such beneficial spells to their clerics.\n\r",ch);
        return;
    }
    sn_atrophy = skill_lookup("atrophy");
    sn_poison = skill_lookup("poison");
    if (is_affected(victim,sn_atrophy))
    {
    affect_strip(victim,sn_atrophy);
    send_to_char("Your body stops wasting away.\n\r",victim);
    act("$n's body stops wasting away.",victim,0,0,TO_ROOM);
    return;
    }
    if (is_affected(ch,sn_poison)
    || is_affected(ch,gsn_poison_dust))
    {
        act("$n looks better.",victim,0,0,TO_ROOM);
        send_to_char("You feel a warm sensation running through you.\n\r",victim);
        affect_strip(victim,sn_poison);
        affect_strip(victim,gsn_poison_dust);
    }
    if (is_affected(ch,gsn_plague))
    {
        act("The sores on $n's body disappear.",victim,0,0,TO_ROOM);
        send_to_char("The sores on your body disappear.\n\r",victim);
        affect_strip(victim,gsn_plague);
    }
    send_to_char("You feel better!\n\r",victim);
    victim->hit = UMIN(victim->hit + 100, victim->max_hit);
    if (victim != ch)
        send_to_char("Ok.\n\r",ch);

    return;
}

void spell_call_slaves(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    AFFECT_DATA af;
    CHAR_DATA *mob;
    CHAR_DATA *gch;
	int i;

    if (cabal_down(ch,CABAL_OUTLAW))
	return;

    if (ch->gold < 500)
    {
		send_to_char("You can't afford to buy slaves!\n\r",ch);
		return;
	}

    for (gch = char_list; gch != NULL; gch = gch->next)
	{
	if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch && gch->pIndexData->vnum == MOB_VNUM_SLAYER)
	{
	send_to_char("You cannot call more slaves yet!\n\r",ch);
	return;
	}
	}

    for (i=0;i < 2; i++)
    {
		mob = create_mobile(get_mob_index(MOB_VNUM_SLAYER));
    	char_to_room(mob,ch->in_room);
    	mob->leader = ch;
    	mob->master = ch;
    	mob->level = level;
    	mob->max_hit = level * 15;
    	mob->hit = mob->max_hit;
    	mob->damroll = level/2;
    	mob->armor[0] = -level*5;
    	mob->armor[1] = -level*5;
    	mob->armor[2] = -level*5;
    	mob->armor[3] = -level*3;
		SET_BIT(mob->affected_by, AFF_CHARM);
	}
    init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype    = AFT_POWER;
	af.level = level;
	af.location = 0;
	af.modifier = 0;
	af.duration = 5;
	af.bitvector = 0;
	af.type = sn;
    affect_to_char(ch,&af);
    act("$n calls and two slaves appear.",ch,0,0,TO_ROOM);
    send_to_char("You pay the slaves for their services.\n\r",ch);
    ch->gold -= 500;
    return;
}

void spell_hold_person( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    int chance;

    if (IS_IMMORTAL(victim))
    {
	act("$N shrugs off your spell.",ch,0,victim,TO_CHAR);
	act("You shrug off $n's spell.",ch,0,victim,TO_VICT);
	return;
    }

    if ( is_affected( victim, sn ))                                                       
    {
        send_to_char("They are are already as paralyzed as they can be.\n\r",ch);
        check_improve(ch,sn,FALSE,1);
	WAIT_STATE(ch,PULSE_VIOLENCE);
        return;
    }
    chance = 55+(victim->saving_throw*.5);
    if(chance<35)
	chance=35;
    if(IS_AFFECTED(victim,AFF_SLOW))
	chance*=1.5;
    if(IS_AFFECTED(victim,AFF_HASTE))
	chance*=.5;
    if(victim->position==POS_FIGHTING)
	chance-=13;
    if(is_affected(victim,skill_lookup("resist paralysis")))
	chance-=25;
    chance-=10;
    chance = UMIN(chance,35);
    if(chance<number_percent())
    {
	send_to_char("You failed.\n\r",ch);
	send_to_char("You feel a slight tightening of your muscles, but it goes away.\n\r",victim);
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
    }

    init_affect(&af);
    af.where     	= TO_AFFECTS;
    af.aftype     	= AFT_SPELL;
    af.type      	= gsn_hold_person;
    af.location  	= 0;
    af.modifier  	= 0;
    af.duration 	= 1;
    af.bitvector 	= 0;
    af.affect_list_msg  = str_dup("restricts movement");
    af.level     	= level;
    affect_to_char( victim, &af );

    send_to_char("Your body stiffens up, you can't do anything!\n\r", victim);
    act("$n suddenly stiffens up, $s face taut with agony.",victim,NULL,NULL,TO_ROOM);
    check_improve(ch,sn,TRUE,1);
    WAIT_STATE(ch,PULSE_VIOLENCE*2);
    return;
}

void spell_slave_of_purgatory( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
  CHAR_DATA *gch;
  CHAR_DATA *slave;
  AFFECT_DATA af;
  char buf[100];
  int i;

  if (!is_affected(ch,gsn_cloak_form))
    {
      send_to_char("You may only invoke that power while concealed.\n\r",ch);
      return;
    }

  if (is_affected(ch,sn))
    {
      send_to_char("You feel too weak to summon a Slave of Purgatory now.\n\r",
		   ch);
      return;
    }

  send_to_char("You attempt to summon a Slave of Purgatory.\n\r",ch);
  act("$n attempts to summon a Slave of Purgatory.",ch,NULL,NULL,TO_ROOM);

  for (gch = char_list; gch != NULL; gch = gch->next)
    {
      if (IS_NPC(gch) && IS_AFFECTED(gch,AFF_CHARM) && gch->master == ch &&
          gch->pIndexData->vnum == MOB_VNUM_SLAVE)
	{
          send_to_char("Two Slave of Purgatories are more than you can control!\n\r",ch);
	  return;
	}
    }

  slave = create_mobile( get_mob_index(MOB_VNUM_SLAVE) );

  for (i=0;i < MAX_STATS; i++)
    {
      slave->perm_stat[i] = ch->perm_stat[i];
    }

  slave->max_hit = IS_NPC(ch)? URANGE(ch->max_hit,1 * ch->max_hit,30000)
                : URANGE(ch->pcdata->perm_hit,1*ch->pcdata->perm_hit,30000);
  slave->hit = slave->max_hit;
  slave->max_mana = ch->max_mana;
  slave->mana = slave->max_mana;
  slave->level = ch->level;
  for (i=0; i < 3; i++)
    slave->armor[i] = interpolate(slave->level,100,-100);
  slave->armor[3] = interpolate(slave->level,100,0);
  slave->gold = 0;
  slave->timer = 0;
  slave->damage[DICE_NUMBER] = number_range(level/15, level/10);
  slave->damage[DICE_TYPE]   = number_range(level/3, level/2);
  slave->damage[DICE_BONUS]  = 0;

  char_to_room(slave,ch->in_room);
  send_to_char("A Slave of Purgatory rises from the shadows!\n\r",ch);
  act("A Slave of Purgatory rises from the shadows!",ch,NULL,NULL,TO_ROOM);
  sprintf(buf, "A Slave of Purgatory kneels before you.");
  send_to_char( buf, ch );
  act( "A Slave of Purgatory kneels before $n!", ch, NULL, NULL, TO_ROOM );

	init_affect(&af);
  af.where		= TO_AFFECTS;
	af.aftype    = AFT_POWER;
  af.type               = sn;
  af.level              = level;
  af.duration           = 18;
  af.bitvector          = 0;
  af.modifier           = 0;
  af.location           = APPLY_NONE;
  affect_to_char(ch, &af);

 SET_BIT(slave->affected_by, AFF_CHARM);
  slave->master = slave->leader = ch;

}


void spell_shroud( int sn, int level, CHAR_DATA *ch, void *vo, int target )
{
    AFFECT_DATA af;

    if (cabal_down(ch,CABAL_ANCIENT))
	return;
    
    if (is_affected(ch,sn))
    {
	send_to_char("You are still shrouded.\n\r",ch);
	return;
    }

    init_affect(&af);
    af.where     = TO_AFFECTS;
    af.aftype    = AFT_POWER;
    af.type      = sn;
    af.level     = level;
    af.duration  = 24;
/*    af.location  = APPLY_AC;
    af.modifier  = 3 * AC_PER_ONE_PERCENT_DECREASE_DAMAGE / 2;
    af.bitvector = 0;
    affect_to_char(ch,&af);*/
    af.location  = APPLY_DAMROLL;
    af.modifier  = 5;
    af.bitvector = 0;
    affect_to_char(ch,&af);
    af.location  = APPLY_HITROLL;
    af.modifier  = 5;
    af.bitvector = 0;
    affect_to_char(ch,&af);
    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = -5;
    af.bitvector = 0;
    affect_to_char(ch,&af);
	af.location = APPLY_NONE;
	af.modifier = 0;
    af.type      = skill_lookup("protection good");
	af.affect_list_msg = str_dup("reduces damage from good opponents by 25%");
    af.bitvector = AFF_PROTECT_GOOD;
    affect_to_char(ch,&af);
    send_to_char( "You become shrouded in darkness.\n\r", ch);
    return;
}

void spell_mental_knife(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = ch->level*4 + number_percent() + 75;
    if (check_spellcraft(ch,sn))
	dam += ch->level;

    if ( saves_spell( level, victim,DAM_MENTAL) )
	{
	dam /= 1.25;
	}
    damage_old( ch, victim, dam, sn, DAM_MENTAL,TRUE);
    return;
}

void spell_scrying(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim;
	ROOM_INDEX_DATA *ori_room;
    if (cabal_down(ch,CABAL_ARCANA))
                return;
	if ((victim = get_char_world(ch, target_name)) == NULL)
	  {
	send_to_char("You can find no such person with your magical powers.\n",ch);
	return;
	  }
	if (ch->in_room->vnum!=5805)
	  {
	send_to_char("You must be with the Seneschal and the Orb of Magic to peer into it!\n",ch);
	return;
	  }
	if (((victim->level > ch->level + 7) && saves_spell(level, victim, DAM_NONE)) || IS_EXPLORE(victim->in_room))
	  {
	send_to_char("You peer intently into the Orb of Magic, but cannot see who you search for.\n",ch);
	WAIT_STATE(ch,PULSE_VIOLENCE*4);
	return;
	  }

	if (ch==victim)
	{
	  send_to_char("The mists inside the Orb of Magic swirl about, slowly revealing what you seek.\n",ch);
	  do_look(ch, str_empty);
	}
	  else
      {
          send_to_char("You feel somebody is watching you.\n\r", victim);
	  ori_room = ch->in_room;
	  char_from_room(ch);
	  char_to_room(ch, victim->in_room);
	  do_look(ch, str_empty);
	  char_from_room(ch);
	  char_to_room(ch, ori_room);
	  WAIT_STATE(ch,PULSE_VIOLENCE*5);
	}
}
void spell_nova(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    char buf[MAX_STRING_LENGTH];
    int dam;
    int dambase;
    dambase=220;

    if (!IS_NPC(ch) && check_spellcraft(ch,sn))
    {
		dambase=350;
		level+=2;
    }

    act("With hands outstretched and face to the sky, $n opens $s mouth in a soundless scream as waves of heat roll out from $m.",ch,0,0,TO_ROOM);
    act("You utter the words 'iaza'.",ch,0,0,TO_CHAR);

    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {	

	vch_next = vch->next_in_room;

	if (is_same_group(vch,ch) || is_safe(ch,vch))
            continue;

	if (ch->cabal>0 && vch->cabal>0 && ch->cabal==vch->cabal)
	    continue;

    	if (!IS_NPC(vch) && (ch->fighting == NULL || vch->fighting == NULL))
	{
  	    sprintf(buf,"Die, %s, you sorcerous dog!",PERS(ch,vch));
	    do_myell(vch,buf);
	}

        dam=dambase+number_percent();
        dam+=number_percent()*1.3;
        
   	if (saves_spell(level+1,vch,DAM_FIRE))
            dam-=125;

     	act("$N is seared by the blistering heat of $n's spell.",ch,0,vch,TO_NOTVICT);
     	act("You are seared by the blistering heat of $n's spell.",ch,0,vch,TO_VICT);
     	act("$N is seared by the blistering heat of your spell.",ch,0,vch,TO_CHAR);
      	damage_old(ch,vch,dam,sn,DAM_FIRE,TRUE);
        
    }

    WAIT_STATE(ch,PULSE_VIOLENCE*2);
    return;
}

void spell_garble(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
        CHAR_DATA *victim = (CHAR_DATA *) vo;
        AFFECT_DATA af;

        if (ch == victim)
        {
        send_to_char("Garble whose speech?\n\r",ch);
        return;
        }
        if (is_affected(victim,sn))
        {
        act("$N's speech is already garbled.",ch,NULL,victim,TO_CHAR);
        return;
        }

        if (saves_spell(level,victim, DAM_MENTAL) && number_percent() <85)
        return;

	init_affect(&af);
	af.where     = TO_AFFECTS;
	af.aftype    = AFT_POWER;
	af.type      = sn;
	af.level     = level;
	af.duration  = 10;
	af.modifier  = 0;
	af.affect_list_msg = str_dup("garbles speech and prevents casting");
	af.location  = 0;
	af.bitvector = 0;
	affect_to_char(victim,&af);

	act("You have garbled $N's speech!",ch,NULL,victim,TO_CHAR);
	send_to_char("You feel your tongue contort.\n\r",victim);
}

void spell_unholy_bless( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
	AFFECT_DATA af,caf;
	AFFECT_DATA *af_check, *af_next;
	OBJ_DATA *obj;

	if((obj = get_obj_carry(ch,target_name,ch)) == NULL)
		return send_to_char("You are not carrying that.\n\r",ch);

	if(obj->item_type != ITEM_WEAPON)
		return send_to_char("You can only give weapons an unholy blessing.\n\r",ch);

        if (is_affected_obj(obj,gsn_unholy_bless))
	{
        	send_to_char("That weapon already contains a dark blessing.\n\r",ch);
        	return;
	}

        OBJ_DATA *cobj;
        OBJ_DATA *cobj_next;
		AFFECT_DATA *caff;

	for (cobj = object_list; cobj != NULL; cobj = cobj_next)
	{
		cobj_next = cobj->next;
		if (is_affected_obj(cobj,gsn_unholy_bless))
		{
			caff = affect_find_obj(cobj,skill_lookup("unholy bless"));
			CHAR_DATA *cch = find_char_by_name(caff->owner_name);
			if (cch) {
				if (!str_cmp(cch->original_name,ch->original_name)) {
					send_to_char("You already have an unholy weapon somewhere in Thera.\n\r",ch);
					return;
				}
			}
		}
	}

	for (af_check = obj->affected; af_check != NULL; af_check = af_next)
	{
		af_next = af_check->next;
	    free_affect(af_check);
	}
	obj->affected = NULL;
	obj->enchanted = TRUE;
	
	init_affect(&af);
	af.where 	= TO_OBJECT;
	af.owner_name 	= str_dup(ch->original_name);
	af.aftype 	= AFT_SPELL;
	af.level 	= level;
	af.duration 	= -1;
	af.type 	= gsn_unholy_bless;
	af.modifier 	= 1;
	af.location 	= APPLY_HITROLL;
	affect_to_obj(obj,&af);
	af.modifier 	= 1;
	af.location 	= APPLY_DAMROLL;
	affect_to_obj(obj,&af);
	af.modifier 	= 50;
	af.location 	= APPLY_HIT;
	affect_to_obj(obj,&af);
	af.modifier 	= 50;
	af.location 	= APPLY_MANA;
	affect_to_obj(obj,&af);

	init_affect(&caf);
	caf.where 	= TO_AFFECTS;
	caf.aftype 	= AFT_INVIS;
	caf.duration 	= -1;
	caf.type 	= sn;
	affect_to_char(ch,&caf);

	act("$p feels strangely alive in your hands as you utter a dark ritual.",ch,obj,0,TO_CHAR);
	return;
}

void spell_eyes_of_intrigue(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim;
      	AFFECT_DATA af;
	ROOM_INDEX_DATA *ori_room;

    	if (cabal_down(ch,CABAL_ANCIENT))
        	return;

	if (is_affected(ch, sn))
		return send_to_char("You cannot call upon your spy network again so soon.\n\r",ch);

	if ((victim = get_char_world(ch, target_name)) == NULL)
	{
		send_to_char("Your spy network reveals no such player.\n",ch);
		return;
	}

	if (((victim->level > ch->level + 7) && saves_spell(level, victim, DAM_NONE)) || IS_EXPLORE(victim->in_room))
	{
		send_to_char("Your spy network cannot find that player.\n",ch);
		WAIT_STATE(ch,PULSE_VIOLENCE*5);
		return;
	}

	if (is_safe(victim, ch))
		return;

	if (ch==victim)
		do_look(ch, str_empty);
	else
        {
	  	ori_room = ch->in_room;

	  	char_from_room(ch);
	  	char_to_room(ch, victim->in_room);

	  	do_look(ch, str_empty);

	  	char_from_room(ch);
	  	char_to_room(ch, ori_room);

		WAIT_STATE(ch,PULSE_VIOLENCE*4);

        	if (saves_spell(level-10, victim, DAM_NONE)) 
			send_to_char("You feel somebody is watching you.\n\r", victim);
	
		init_affect(&af);
        	af.where     	= TO_AFFECTS;
        	af.aftype 	= AFT_INVIS;
        	af.type      	= sn;
        	af.level     	= level;
        	af.duration  	= number_range(5,10);
		af.affect_list_msg = str_dup("restricts use of Eyes of Intrigue");
        	af.modifier  	= 0;
        	af.location  	= 0;
        	af.bitvector 	= 0;
        	affect_to_char(ch,&af);

	}
	return;
}

void spell_confuse(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	AFFECT_DATA af;
	CHAR_DATA *rch;
	int count=0;

        if (is_affected(victim, gsn_confuse))
        {
	act("$N is already thoroughly confused.",ch,NULL,victim,TO_CHAR);
	return;
	}

    	if (cabal_down(ch,CABAL_OUTLAW))
                return;


	if (saves_spell(level,victim, DAM_MENTAL) && number_percent() <90 )
     	{
		send_to_char("You failed.\n\r",victim);
		return;
	}

	init_affect(&af);
	af.where     	= TO_AFFECTS;
	af.aftype 	= AFT_POWER;
	af.type      	= sn;
	af.level     	= level;
	af.duration  	= 3;
	af.modifier  	= 0;
	af.affect_list_msg = str_dup("inflicts a confused state");
	af.location  	= 0;
	af.bitvector 	= 0;
	affect_to_char(victim,&af);

	send_to_char("You feel so confused, you don't know where to go.\n\r",victim);
        act("$N is very confused.",ch,NULL,victim,TO_CHAR);

	for (rch = ch->in_room->people; rch; rch = rch->next_in_room)
		if (rch == ch
		&&  !can_see(ch, rch))
			count++;

	for (rch = ch->in_room->people; rch; rch = rch->next_in_room)
        {

		if (rch != ch
		&&  can_see(ch, rch)
		&&  number_range(1, count) == 1)
			break;
	}

	if (rch)
	do_murder(victim, rch->name);
	do_murder(victim, ch->name);
}
void spell_crushing_hand(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
        CHAR_DATA *victim = (CHAR_DATA *) vo;
        int dam;
  	dam=160;
	
	if (check_spellcraft(ch,sn))
        {
		dam=200;
		level+=2;
       	}

	dam+=(number_percent())/2;
        
	if (saves_spell(level+1,victim,DAM_OTHER))
            	dam-=60;

        act("$n creates a huge spectral hand to brutally pummel $N!",ch,0,victim,TO_NOTVICT);
        act("$n creates a huge spectral hand to brutally pummel you!",ch,0,victim,TO_VICT);
        act("You create a huge spectral hand to brutally pummel $N!",ch,0,victim,TO_CHAR);

        if (number_percent()<60)
	{
	        act("$N appears to be stunned by the blow.",ch,0,victim,TO_NOTVICT);
	        act("You feel stunned by the brutal blow.",ch,0,victim,TO_VICT);
      	  	act("$N appears to be stunned by the blow.",ch,0,victim,TO_CHAR);
		WAIT_STATE(victim,PULSE_VIOLENCE*2);
	}
	damage_new(ch,victim,dam,sn,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"crushing hand");
        return;
}

void spell_deafen(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
        CHAR_DATA *victim = (CHAR_DATA *) vo;
        AFFECT_DATA af;

        if (ch == victim)
        {
        	send_to_char("Deafen who?\n\r",ch);
        	return;
        }

        if (is_affected(victim,sn))
        {
        	act("$N is already deafened.",ch,NULL,victim,TO_CHAR);
        	return;
        }

	if (is_affected(victim,gsn_shock_sphere))
	{
		act("$N is already deafened.",ch,NULL,victim,TO_CHAR);
		return;
	}

        if (saves_spell(level+10, victim, DAM_NONE))
	{
		act("You failed to deafen $N.",ch,NULL,victim,TO_CHAR);
		act("You feel a slight ringing in your ears, but it stops.",ch,NULL,victim,TO_VICT);
        	return;
	}

	init_affect(&af);
        af.where     	= TO_AFFECTS;
	af.aftype 	= AFT_SPELL;
        af.type      	= sn;
        af.level     	= level;
        af.duration  	= number_range(1,3);
        af.modifier  	= 0;
	af.affect_list_msg = str_dup("prevents casting and inflicts a deafened state");
        af.location  	= 0;
        af.bitvector 	= 0;
        affect_to_char(victim,&af);

        act("You have deafened $N!",ch,NULL,victim,TO_CHAR);
        send_to_char("A loud ringing fills your ears...you can't hear anything!\n\r",victim);
}

void spell_chaos_blade(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
        AFFECT_DATA af;
        OBJ_DATA *blade;

    if (is_affected(ch,gsn_chaos_blade))
    {
        send_to_char("You still have the blade!\n\r",ch);
        return;
    }
    if (ch->mana < 30)
    {
        send_to_char("You don't have the mana.\n\r",ch);
        return;
    }

        send_to_char("You create a blade of chaos!\n\r",ch);
        act("$n creates a blade of chaos!",ch,NULL,NULL,TO_ROOM);
        ch->mana -= 30;
        blade = create_object(get_obj_index(OBJ_VNUM_CHAOS_BLADE), level);
        blade->timer = 58;
        obj_to_char(blade,ch);

	init_affect(&af);
        af.where = TO_AFFECTS;
	af.aftype = AFT_POWER;
        af.type = gsn_chaos_blade;
        af.duration = 60;
        af.location = 0;
        af.bitvector = 0;
        af.modifier = 0;
        af.level = ch->level;
        affect_to_char(ch,&af);

        check_improve(ch,gsn_chaos_blade,TRUE,1);
        return;
}

void spell_incinerate(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
    AFFECT_DATA af;
    if (victim == ch)
        return;
    if (victim == NULL)
	  return;
    if (!IS_NPC(victim))
	  do_myell(victim,"Help! I'm being incinerated!");
    act("Chanting incantations, you step up to $N and trace an arcane rune in the air.",ch,0,victim,TO_CHAR);
    act("Chanting incantations, $n steps up to $N and traces an arcane rune in the air.",ch,0,victim,TO_NOTVICT);
    act("Chanting incantations, $n steps up to you and traces an arcane rune in the air.",ch,0,victim,TO_VICT);
    act("Concentrating your energies, you gesture to $N and will $M to burn!",ch,0,victim,TO_CHAR);
    act("Concentrating $s energies, $n wills $M to burn!",ch,0,victim,TO_NOTVICT);
    act("Concentrating $s energies, $n wills you to burn!",ch,0,victim,TO_VICT);
    dam=number_range(250,500);
    if (!saves_spell(level+number_range(1,8),victim,DAM_FIRE))
	{
	    dam=number_range(300,800);
	    /* Failed save #1. Yer burnin, sucker. */
	init_affect(&af);
          af.where = TO_AFFECTS;
	af.aftype = AFT_SPELL;
          af.type = gsn_incinerate;
          af.duration = ch->level/10;
          af.location = APPLY_STR;
          af.bitvector = AFF_SLOW;
          af.modifier = -20;
          af.level = ch->level;
          affect_to_char(victim,&af);
          act("$N bursts into a roaring conflagration of flames!",ch,0,victim,TO_NOTVICT);
          act("$N bursts into a roaring conflagration of flames!",ch,0,victim,TO_CHAR);
          act("You burst into a roaring conflagration of flames!",ch,0,victim,TO_VICT);
		if (!saves_spell(level,victim,DAM_FIRE) && !saves_spell(level,victim,DAM_FIRE))
		{
			/* Failed saves #2 and 3. Have a bad day. */
	            act("{RSeared beyond recognition by your spell, $N collapses into a pile of ash!{x",ch,0,victim,TO_CHAR);
          	      act("{RSeared beyond recognition by $n's spell, $N collapses into a pile of ash!{x",ch,0,victim,TO_NOTVICT);
	            act("{RSeared beyond recognition by $n's spell, you collapse into a pile of ash!{x",ch,0,victim,TO_VICT);
			raw_kill(ch,victim);
		}
 	}
    damage(ch,victim,dam,gsn_incinerate,DAM_FIRE,TRUE);
    ch->mana-=50;
    WAIT_STATE(ch,PULSE_VIOLENCE*4);
}

void spell_light_of_heaven(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = ch->level*4 + number_percent() + 75;
    if (check_spellcraft(ch,sn))
	dam += ch->level;

    if ( saves_spell( level, victim,DAM_HOLY) )
	{
	dam /= 1.25;
	}
    damage_old( ch, victim, dam, sn, DAM_HOLY,TRUE);
    return;
}

void spell_unholy_affinity(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
	AFFECT_DATA af;
	if(is_affected(ch,gsn_unholy_affinity))
		return send_to_char("You are already in tune with your unholy weapon.\n\r",ch);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.duration = 6;
	af.level = level;
	af.owner_name = str_dup(ch->original_name);
	af.aftype = AFT_SPELL;
	af.affect_list_msg = str_dup("grants immunity to most disarm attempts");
	af.type = gsn_unholy_affinity;
	affect_to_char(ch,&af);
	return send_to_char("You feel a strange affinity for your unholy weapon, as if you couldn't bear to drop it.\n\r",ch);
}

bool check_dispel( int dis_level, CHAR_DATA *victim, int sn)
{
    AFFECT_DATA *af;

    if (is_affected(victim, sn))
    {
        for ( af = victim->affected; af != NULL; af = af->next )
        {
            if ( af->type == sn )
            {
				if(!saves_dispel(dis_level,af->level,af->duration))
                {
                    affect_strip(victim,sn);
	        	    if ( skill_table[sn].msg_off )
	        	    {
	            		send_to_char( skill_table[sn].msg_off, victim );
	            		send_to_char( "\n\r", victim );
	        	    }
					return TRUE;
				}
            }
        }
    }
    return FALSE;
}

bool check_dispel_cancellation(int dis_level, CHAR_DATA *victim, CHAR_DATA *ch, int sn,bool commune)
{
    AFFECT_DATA *af;
    char buf[MAX_STRING_LENGTH];

    if (is_affected(victim, sn))
    {
        for ( af = victim->affected; af != NULL; af = af->next )
        {
            if ( af->type == sn )
            {
		if (af->aftype == AFT_SPELL) {
		if(!saves_dispel(dis_level,af->level,af->duration))
                {
		    sprintf(buf,"The magic of your %s spell unravels.\n\r",skill_table[sn].name);
                    affect_strip(victim,sn);
		    send_to_char(buf,victim);
		    sprintf(buf,"The magic of $n's %s spell unravels.",skill_table[sn].name);
		    act(buf,victim,0,0,TO_ROOM);
			return TRUE;
		}
		else
		{
                    sprintf(buf,"The magic of your %s spell is unaffected.\n\r",skill_table[sn].name);
                    send_to_char(buf,victim);   
                    sprintf(buf,"The magic of $n's %s spell is unaffected.",skill_table[sn].name);
		    if(victim->fighting)
                    	act(buf,victim,0,0,TO_ROOM);
		}
		} else if (af->aftype == AFT_COMMUNE && commune) {
		if(!saves_dispel(dis_level,af->level,af->duration))
                {
		    sprintf(buf,"The power of your %s supplication unravels.\n\r",skill_table[sn].name);
                    affect_strip(victim,sn);
		    send_to_char(buf,victim);
		    sprintf(buf,"The power of $n's %s supplication unravels.",skill_table[sn].name);
		    act(buf,victim,0,0,TO_ROOM);
			return TRUE;
		}
		else
		{
                    sprintf(buf,"The power of your %s supplication is unaffected.\n\r",skill_table[sn].name);
                    send_to_char(buf,victim);   
                    sprintf(buf,"The power of $n's %s supplication is unaffected.",skill_table[sn].name);
		    if(victim->fighting)
                    	act(buf,victim,0,0,TO_ROOM);
		}
		}
            }
        }
    }
    return FALSE;
}

void spell_dispel_magic( int sn, int level, CHAR_DATA *ch, void *vo,int target )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    bool found = FALSE;
	bool commune = FALSE;

    /* begin running through the spells */ 

	if (class_table[ch->class].ctype==CLASS_COMMUNER) commune = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("armor"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("bless"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("blindness"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("calm"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("change sex"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("chill"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("curse"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("detect evil"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("detect good"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("detect hidden"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("detect invis"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("detect magic"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("faerie fire"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("fly"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("frenzy"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("giant strength"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("haste"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("infravision"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("invis"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("protective shield"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("mass invis"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("pass door"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("protection"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("sanctuary"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("shield"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("sleep"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("slow"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("stone skin"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("wraithform"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("weaken"),commune))
        found = TRUE;
 
    if (check_dispel_cancellation(level,victim,ch,skill_lookup("righteousness"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("water breathing"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("shield of flames"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("shield of lightning"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("shield of bubbles"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("shield of dust"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("shield of frost"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("shield of wind"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("absorb"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("enlarge"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("reduce"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("shield of wind"),commune))
        found = TRUE;

    if (check_dispel_cancellation(level,victim,ch,skill_lookup("ward of diminution"),commune))
        found = TRUE;

	return;
}


void spell_hand_of_vengeance(int sn,int level,CHAR_DATA *ch,void *vo,int target)
{
        CHAR_DATA *victim = (CHAR_DATA *) vo;
        int dam;

        if (!IS_SET(victim->act,PLR_CRIMINAL) )
        {
        act("A huge clenched fist appears above $n but fades without striking.",victim,0,0,TO_ROOM);
        send_to_char("A huge clenched fist appears above you but fades without striking.\n\r",victim);
        send_to_char("The Immortal of Enforcer frowns upon the abuse of this power.\n\r",ch);
        return;
        }
        dam = dice(level,6) + dice(level,3);
        act("A huge clenched fist appears above $n and strikes down.",victim,0,0,TO_ROOM);
        send_to_char("A huge clenched fist appears above you and strikes down.\n\r",victim);
        if (saves_spell(level,victim,DAM_BASH) )
                dam /= 2;
        if (number_percent()>75 || (is_affected(victim,gsn_bloody_shackles) && number_percent()>45))
        {
        act("The blow hammers $n to the ground with savage force!",victim,0,0,TO_ROOM);
        send_to_char("The blow hammers you to the ground with savage force!\n\r",victim);
        dam += dice(level,4);
        WAIT_STATE(victim, 24);
        }
        damage_old(ch,victim,dam,sn,DAM_BASH,TRUE);
        return;
}

void spell_exude_health( int sn, int level, CHAR_DATA *ch, void *vo,int target)
{
	OBJ_DATA *obj = (OBJ_DATA *) vo;
	AFFECT_DATA af;
	float hpmod = 0;

	if (is_affected(ch, sn))
		return send_to_char("You can't exude another item yet.\n\r", ch);

	if (isCabalItem(obj) || IS_WEAPON_STAT(obj, WEAPON_LEADER))
		return send_to_char("You can't exude cabal items or leader weapons.\n\r", ch);

	if (obj->pIndexData->limtotal <= 0 || obj->pIndexData->limtotal > 12)
		return send_to_char("That item is not powerful enough to exude.\n\r", ch);

	if (!IS_OBJ_STAT(obj, ITEM_MAGIC))
		return send_to_char("You can only exude magical items.\n\r", ch);

	if (number_percent() > get_skill(ch, sn) - 5) {
		act("$p explodes into fragments as you attempt to cast a spell upon it.",ch,obj,NULL,TO_CHAR);
		act("$p explodes into fragments as $n attempts to cast a spell upon it.",ch,obj,NULL,TO_ROOM);
		extract_obj(obj);
		return;
	}

	hpmod = number_range(50, 100) + ((12 - obj->pIndexData->limtotal) * obj->level);

	if (IS_OBJ_STAT(obj, ITEM_MAGIC))
		hpmod *= 1.05;
	if (IS_OBJ_STAT(obj, ITEM_GLOW))
		hpmod *= 1.05;
	if (IS_OBJ_STAT(obj, ITEM_INVIS))
		hpmod *= 1.05;
	if (!str_cmp(obj->pIndexData->material->name, "energy"))
		hpmod *= 1.05;

	init_affect(&af);
	af.where		= TO_AFFECTS;
	af.aftype		= AFT_SPELL;
	af.type			= sn;
	af.level		= level;
	af.duration		= 1000 / 24;
	af.location		= APPLY_HIT;
	af.modifier		= (int)hpmod;
	af.bitvector		= 0;
	affect_to_char(ch,&af);

	act("$p vanishes in an eruption of mana as you siphon it's magical power.",ch,obj,NULL,TO_CHAR);
	act("$p vanishes in an eruption of mana as $n siphons it's magical power.",ch,obj,NULL,TO_ROOM);
	extract_obj(obj);
}

