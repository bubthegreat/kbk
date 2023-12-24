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
 *	    Russ Taylor (rtaylor@efn.org)				   *
 *	    Gabrielle Taylor						   *
 *	    Brian Moore (zump@rom.org)					   *
 *	By using this code, you have agreed to follow the terms of the	   *
 *	ROM license, in the file Tartarus/doc/rom.license                  *
 ***************************************************************************/

/***************************************************************************
 *       Tartarus code is copyright (C) 1997-1998 by Daniel Graham          *
 *	In using this code you agree to comply with the Tartarus license   *
 *       found in the file /Tartarus/doc/tartarus.doc                       *
 ***************************************************************************/

#include "include.h"

/* random room generation procedure */
ROOM_INDEX_DATA *get_random_room(CHAR_DATA *ch)
{
	ROOM_INDEX_DATA *room;

	for (;;)
	{
		room = get_room_index(number_range(0, 65535));
		if (room != NULL)
			if (can_see_room(ch, room) && !room_is_private(room) && !IS_SET(room->room_flags, ROOM_PRIVATE) && !IS_SET(room->room_flags, ROOM_SOLITARY) && !IS_SET(room->room_flags, ROOM_SAFE) && !IS_SET(room->room_flags, ROOM_NO_GATE) && (IS_NPC(ch) || IS_SET(ch->act, ACT_AGGRESSIVE) || !IS_SET(room->room_flags, ROOM_LAW)))
				break;
	}

	return room;
}

/* RT Enter portals */
void do_enter(CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *location;

	if (ch->fighting != NULL)
		return;

	/* nifty portal stuff */
	if (argument[0] != '\0')
	{
		ROOM_INDEX_DATA *old_room;
		OBJ_DATA *portal;
		CHAR_DATA *fch, *fch_next;

		old_room = ch->in_room;

		portal = get_obj_list(ch, argument, ch->in_room->contents);

		if (portal == NULL)
		{
			send_to_char("You don't see that here.\n\r", ch);
			return;
		}

		if (portal->item_type != ITEM_PORTAL || (IS_SET(portal->value[1], EX_CLOSED) && !IS_TRUSTED(ch, ANGEL)))
		{
			send_to_char("You can't seem to find a way in.\n\r", ch);
			return;
		}

		if (!IS_TRUSTED(ch, ANGEL) && !IS_SET(portal->value[2], GATE_NOCURSE) && (IS_AFFECTED(ch, AFF_CURSE) || IS_SET(old_room->room_flags, ROOM_NO_RECALL)))
		{
			send_to_char("Something prevents you from leaving...\n\r", ch);
			return;
		}

		if (IS_SET(portal->value[2], GATE_RANDOM) || portal->value[3] == -1)
		{
			location = get_random_room(ch);
			portal->value[3] = location->vnum; /* for record keeping :) */
		}
		else if (IS_SET(portal->value[2], GATE_BUGGY) && (number_percent() < 5))
			location = get_random_room(ch);
		else
			location = get_room_index(portal->value[3]);

		if (location == NULL || location == old_room || !can_see_room(ch, location) || (room_is_private(location) && !IS_TRUSTED(ch, IMPLEMENTOR)))
		{
			act("$p doesn't seem to go anywhere.", ch, portal, NULL, TO_CHAR);
			return;
		}

		if (IS_NPC(ch) && IS_SET(ch->act, ACT_AGGRESSIVE) && IS_SET(location->room_flags, ROOM_LAW))
		{
			send_to_char("Something prevents you from leaving...\n\r", ch);
			return;
		}

		act("$n steps into $p.", ch, portal, NULL, TO_ROOM);

		if (IS_SET(portal->value[2], GATE_NORMAL_EXIT))
			act("You enter $p.", ch, portal, NULL, TO_CHAR);
		else
			act("You walk through $p and find yourself somewhere else...",
				ch, portal, NULL, TO_CHAR);

		char_from_room(ch);
		char_to_room(ch, location);

		if (IS_SET(portal->value[2], GATE_GOWITH)) /* take the gate along */
		{
			obj_from_room(portal);
			obj_to_room(portal, location);
		}

		if (IS_SET(portal->value[2], GATE_NORMAL_EXIT))
			act("$n has arrived.", ch, portal, NULL, TO_ROOM);
		else
			act("$n has arrived through $p.", ch, portal, NULL, TO_ROOM);

		do_look(ch, "auto");

		/* charges */
		if (portal->value[0] > 0)
		{
			portal->value[0]--;
			if (portal->value[0] == 0)
				portal->value[0] = -1;
		}

		/* protect against circular follows */
		if (old_room == location)
			return;

		for (fch = old_room->people; fch != NULL; fch = fch_next)
		{
			fch_next = fch->next_in_room;

			if (portal == NULL || portal->value[0] == -1)
				/* no following through dead portals */
				continue;

			if (fch->master == ch && IS_AFFECTED(fch, AFF_CHARM) && fch->position < POS_STANDING)
				do_stand(fch, "");

			if (fch->master == ch && fch->position == POS_STANDING)
			{

				if (IS_SET(ch->in_room->room_flags, ROOM_LAW) && (IS_NPC(fch) && IS_SET(fch->act, ACT_AGGRESSIVE)))
				{
					act("You can't bring $N into the city.",
						ch, NULL, fch, TO_CHAR);
					act("You aren't allowed in the city.",
						fch, NULL, NULL, TO_CHAR);
					continue;
				}

				act("You follow $N.", fch, NULL, ch, TO_CHAR);
				do_enter(fch, argument);
			}
		}

		if (portal != NULL && portal->value[0] == -1)
		{
			act("$p fades out of existence.", ch, portal, NULL, TO_CHAR);
			if (ch->in_room == old_room)
				act("$p fades out of existence.", ch, portal, NULL, TO_ROOM);
			else if (old_room->people != NULL)
			{
				act("$p fades out of existence.",
					old_room->people, portal, NULL, TO_CHAR);
				act("$p fades out of existence.",
					old_room->people, portal, NULL, TO_ROOM);
			}
			extract_obj(portal);
		}

		return;
	}

	send_to_char("Nope, can't do it.\n\r", ch);
	return;
}

void do_path(CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *pRoomIndex, *queueIn, *queueOut, *source, *destination = NULL;
	int iHash, door, door2;
	EXIT_DATA *pexit, *pexit2;
	char buf[MAX_STRING_LENGTH], buf2[MAX_STRING_LENGTH];
	bool fArea = 0;
	CHAR_DATA *victim = NULL;
	AREA_DATA *area;

	if (argument[0] == '\0')
	{
		if (!IS_IMMORTAL(ch))
			send_to_char("<destination area>\n\r", ch);
		else
			send_to_char("<destination player, mob, or area>\n\r", ch);
		return;
	}

	if (IS_AFFECTED(ch, AFF_BLIND) && !IS_IMMORTAL(ch))
	{
		send_to_char("You can't see to get there.\n\r", ch);
		return;
	}

	if ((source = ch->in_room) == NULL)
	{
		send_to_char("You must be somewhere to go anywhere.\n\r", ch);
		return;
	}

	if ((area = area_lookup(argument)) != NULL)
	{
		destination = area_begin(area);
		fArea = 1;
	}

	else if (IS_IMMORTAL(ch) && (victim = get_char_world(ch, argument)) != NULL)
		destination = victim->in_room;

	if (destination == NULL)
	{
		send_to_char("No such destination.\n\r", ch);
		return;
	}

	if ((fArea && source->area == destination->area) || source == destination)
	{
		send_to_char("No need to walk to get there!\n\r", ch);
		return;
	}

	for (iHash = 0; iHash < MAX_KEY_HASH; iHash++)
	{
		for (pRoomIndex = room_index_hash[iHash]; pRoomIndex != NULL;
			 pRoomIndex = pRoomIndex->next)
		{
			pRoomIndex->distance_from_source = INT_MAX;
			pRoomIndex->shortest_from_room = NULL;
			pRoomIndex->shortest_next_room = NULL;
		}
	}
	source->distance_from_source = 0;
	queueIn = source;
	for (queueOut = source; queueOut; queueOut = queueOut->shortest_next_room)
	{
		for (door = 0; door < MAX_DIR; door++)
		{
			if ((pexit = queueOut->exit[door]) != NULL && pexit->u1.to_room != NULL)
			{
				if (pexit->u1.to_room->distance_from_source >
					queueOut->distance_from_source + 1)
				{
					pexit->u1.to_room->distance_from_source =
						queueOut->distance_from_source + 1;
					if (
						(fArea && pexit->u1.to_room->area == destination->area) || pexit->u1.to_room == destination)
					{
						int count = 1;
						char buf3[3];

						sprintf(buf3, "%c", dir_name[door][0]);
						sprintf(buf2, "%s", buf3);
						for (pRoomIndex = queueOut;
							 pRoomIndex->shortest_from_room;
							 pRoomIndex = pRoomIndex->shortest_from_room)
						{
							for (door2 = 0; door2 < MAX_DIR; door2++)
							{
								if (
									(pexit2 =
										 pRoomIndex->shortest_from_room->exit[door2]) != NULL &&
									pexit2->u1.to_room == pRoomIndex)
								{
									if (dir_name[door2][0] == buf3[0])
									{
										count++;
									}
									else
									{
										sprintf(buf, "%s", buf2);
										if (count > 1)
										{
											sprintf(buf2, "%c%d%s",
													dir_name[door2][0], count,
													buf);
										}
										else
										{
											sprintf(buf2, "%c%s",
													dir_name[door2][0], buf);
										}
										count = 1;
										sprintf(buf3, "%c",
												dir_name[door2][0]);
									}
								}
							}
						}
						if (count > 1)
						{
							sprintf(buf, "%s", buf2);
							sprintf(buf2, "%d%s", count, buf);
						}
						if (fArea)
						{
							sprintf(buf,
									"Shortest path to %s is %d steps: %s.\n\r",
									destination->area->name,
									pexit->u1.to_room->distance_from_source, buf2);
							send_to_char(buf, ch);
						}
						else if (victim)
						{
							sprintf(buf,
									"Shortest path to %s is %d steps: %s.\n\r",
									GetName(victim),
									pexit->u1.to_room->distance_from_source, buf2);
							send_to_char(buf, ch);
						}
						return;
					}
					pexit->u1.to_room->shortest_from_room = queueOut;
					queueIn->shortest_next_room = pexit->u1.to_room;
					queueIn = pexit->u1.to_room;
				}
			}
		}
	}
	send_to_char("No path to destination.\n\r", ch);
	return;
}
