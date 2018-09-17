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

void do_clearbrush(CHAR_DATA *ch, char *argument)
{
	AREA_DATA *area = ch->in_room->area;
	ROOM_INDEX_DATA *room = ch->in_room, *nroom, *droom = get_room_index(1299);
	ROOM_AFFECT_DATA raf;
	AFFECT_DATA af;
	int dir = 0;

	if(is_affected(ch,skill_lookup("clear brush")))
		return send_to_char("You don't have the energy to clear more brush right now.\n\r",ch);
	
	if(is_affected_room(room,skill_lookup("clear brush")))
		return send_to_char("The brush here is too thin to clear.\n\r",ch);

	if(!isInWilderness(ch))
		return send_to_char("You must be in the wilderness to clear brush.\n\r",ch);

	if(argument[0] == '\0' || (str_prefix("n",argument) && str_prefix("e",argument) && str_prefix("s",argument) && str_prefix("w",argument)))
		return send_to_char("Invalid direction.\n\r",ch);

	if(!str_prefix("u",argument) || !str_prefix("d",argument))
		return send_to_char("You can't clear brush upwards or downwards.\n\r",ch);

	if(!str_prefix("e",argument))
		dir = DIR_EAST;

	if(!str_prefix("n",argument))
		dir = DIR_NORTH;
	
	if(!str_prefix("s",argument))
		dir = DIR_SOUTH;

	if(!str_prefix("w",argument))
		dir = DIR_WEST;

	if(room->exit[dir])
		return send_to_char("That way is already clear!\n\r",ch);

	nroom = new_room_index();
	nroom->description = str_dup(droom->description);
	nroom->vnum = ch->in_room->vnum + 10000;
	nroom->area = area;
	nroom->exit[rev_dir[dir]] = new_exit();
	nroom->exit[rev_dir[dir]]->u1.to_room = ch->in_room;
	nroom->name = str_dup(droom->name);
	nroom->sector_type = SECT_FOREST;
	SET_BIT(nroom->exit[rev_dir[dir]]->exit_info,EX_NONOBVIOUS);

	switch(dir) {
		case(DIR_EAST):
			ch->in_room->exit[dir] = new_exit();
			ch->in_room->exit[dir]->u1.to_room = nroom;
			room->exit[dir]->orig_door = dir;
			SET_BIT(room->exit[dir]->exit_info,EX_NONOBVIOUS);
			send_to_char("You clear the brush away to the east and step into the clearing.\n\r",ch);
			act("$n rustles the trees to the east, then disappears.",ch,0,0,TO_ROOM);
			break;
	        case(DIR_SOUTH):
                        ch->in_room->exit[dir] = new_exit();
                        ch->in_room->exit[dir]->u1.to_room = nroom;
			room->exit[dir]->orig_door = dir;                        
			SET_BIT(room->exit[dir]->exit_info,EX_NONOBVIOUS);
			act("$n rustles the trees to the south, then disappears.",ch,0,0,TO_ROOM);
                        send_to_char("You clear the brush away to the south and step into the clearing.\n\r",ch);
                        break;

                case(DIR_NORTH):
                        ch->in_room->exit[dir] = new_exit();
                        ch->in_room->exit[dir]->u1.to_room = nroom;
			room->exit[dir]->orig_door = dir;                        
			SET_BIT(room->exit[dir]->exit_info,EX_NONOBVIOUS);
                        send_to_char("You clear the brush away to the north and step into the clearing.\n\r",ch);
                        act("$n rustles the trees to the north, then disappears.",ch,0,0,TO_ROOM);
			break;

                case(DIR_WEST):
                        ch->in_room->exit[dir] = new_exit();
                        ch->in_room->exit[dir]->u1.to_room = nroom;
                        SET_BIT(room->exit[dir]->exit_info,EX_NONOBVIOUS);
			room->exit[dir]->orig_door = dir;
                        send_to_char("You clear the brush away to the west and step into the clearing.\n\r",ch);
                        act("$n rustles the trees to the west, then disappears.",ch,0,0,TO_ROOM);
			break;
	}

	char_from_room(ch);
	char_to_room(ch,nroom);
	do_look(ch,"auto");

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SKILL;
	af.type = skill_lookup("clear brush");
	af.duration = ch->level/12;
	affect_to_char(ch,&af);

	init_affect_room(&raf);
	raf.where = TO_ROOM_AFFECTS;
	raf.type = skill_lookup("clear brush");
	raf.duration = -1;
	affect_to_room(room,&raf);
	affect_to_room(nroom,&raf);

return;
}
/* Dragging snare...under construction */
void do_dragsnare(CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	ROOM_AFFECT_DATA raf;
	ROOM_INDEX_DATA *room = ch->in_room;
	int dir = 0;

	if(!isInWilderness(ch))
		return send_to_char("You must be in the wilds to set a dragging snare.\n\r",ch);
	if(is_affected(ch,gsn_dragsnare))
		return send_to_char("You already have a snare waiting for someone!\n\r",ch);
	if(is_affected_room(room,gsn_dragsnare))
		return send_to_char("This room can't be prepared for a dragsnare right now.\n\r",ch);
	if(argument[0] == '\0' || (str_prefix("n",argument) && str_prefix("s",argument) && str_prefix("e",argument) && str_prefix("w",argument)))
		return send_to_char("Invalid direction.\n\r",ch);

	if(!str_prefix("n",argument))
		dir = DIR_NORTH;
	if(!str_prefix("s",argument))
		dir = DIR_SOUTH;
	if(!str_prefix("e",argument))
		dir = DIR_EAST;
	if(!str_prefix("w",argument))
		dir = DIR_WEST;
	if(!str_prefix("u",argument))
		dir = DIR_UP;
	if(!str_prefix("d",argument))
		dir = DIR_DOWN;

	if(!room->exit[dir])
		return send_to_char("Alas, you cannot go that way.\n\r",ch);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = gsn_dragsnare;
	af.aftype = AFT_SKILL;
	af.duration = 6;
	af.owner_name = str_dup(ch->original_name);
	affect_to_char(ch,&af);
	init_affect_room(&raf);
	raf.where = TO_ROOM_AFFECTS;
	raf.type = gsn_dragsnare;
	raf.aftype = AFT_SKILL;
	raf.owner_name = str_dup(ch->original_name);
	raf.duration = 6;
	affect_to_room(room,&raf);
	act("You carefully lay down a snare to catch the foot of the next passerby and step back to catch them.",ch,0,0,TO_CHAR);
	char_from_room(ch);
	char_to_room(ch,room->exit[dir]->u1.to_room);
	do_look(ch,"auto");
return;
}

bool check_fam(CHAR_DATA *ch, char *attack)
{
	CHAR_DATA *victim;
	int chance;
	char buf[MAX_STRING_LENGTH];
	AFFECT_DATA af;

	if((victim = ch->fighting) == NULL)
		return FALSE;

	if ((chance = get_skill(ch,gsn_wilderness_fam)) == 0)
		return FALSE;

	chance = chance/number_range(5,10);

	if(number_percent() > chance) {
		switch(number_range(1,5)) {
			case(1):
				if(number_percent() > 50) {
				if(IS_AFFECTED(victim,AFF_BLIND))
					break;
				sprintf(buf,"You dive to the ground, avoiding $N's %s, grab a handful of dirt and fling it in $S eyes!",attack);
				act(buf,ch,0,victim,TO_CHAR);
				free_string(buf);
				sprintf(buf,"$n dives to the ground, avoiding your %s, and sends a handful of dirt at your eyes!",attack);
				act(buf,ch,0,victim,TO_VICT);
				free_string(buf);
				act("$N is blinded by the dirt in $S eyes!",ch,0,victim,TO_NOTVICT);
				send_to_char("You can't see a thing!\n\r",victim);
				damage_new(ch,victim,number_range(2,10),TYPE_UNDEFINED,DAM_NONE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"thrown dirt");

				init_affect(&af);
				af.where = TO_AFFECTS;
				af.type = skill_lookup("blindness");
				af.aftype = AFT_SKILL;
				af.location = APPLY_HITROLL;
				af.modifier = -4;
				af.duration = 0;
				af.owner_name = str_dup(ch->original_name);
				af.bitvector = AFF_BLIND;
				affect_to_char(victim,&af);
				break;
				}
				else
					break;
			case(2):
				sprintf(buf,"You use the trees to your advantage and avoid $N's %s.",attack);
				act(buf,ch,0,victim,TO_CHAR);
				free_string(buf);
				sprintf(buf,"$n uses the trees to avoid your %s.",attack);
				act(buf,ch,0,victim,TO_VICT);
				free_string(buf);
				break;
			case(3):
				if(number_percent() > 60) {
				sprintf(buf,"You dodge $N's %s and disappear into the trees momentarily.",attack);
				act(buf,ch,0,victim,TO_CHAR);
				free_string(buf);
				sprintf(buf,"$n dodges your %s and disappears into the trees.",attack);
				act(buf,ch,0,victim,TO_VICT);

				stop_fighting(ch,TRUE);
				do_camouflage(ch,"");
				if (!can_see(victim, ch)) {
					OBJ_DATA *weapon = get_eq_char(ch,WEAR_WIELD);
					
					damage_new(ch,victim,number_range(2*GET_DAMROLL(ch),3*GET_DAMROLL(ch)),TYPE_UNDEFINED,weapon ? weapon->value[3] : DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"ambush");
				}
				else {
					act("You attempt to ambush $N but $E spots you before you can strike!", ch, 0, victim, TO_CHAR);
					act("$n attempts to ambush you but you spot $m before $e can strike!", ch, 0, victim, TO_VICT);
					set_fighting(ch, victim);
				}
				break;
				}
				return FALSE;
			case(4):
			case(5):
				return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}
