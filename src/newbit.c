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
//
//   Infinate bitmask system.
//
//   Code by Runter.

#include "include.h"

int remove_bit(BITMASK *mask, sh_int bit) 
{
	BMlist *pBlist, *last = 0;

    	if (!is_set(mask, bit)) // nothing doing. It isn't set.
        	return FALSE;
    	--bit;

    	for(pBlist = mask->int_list;pBlist;pBlist = pBlist->next) 
	{
        	if (pBlist->set == bit / 32) 
		{
            		pBlist->tar_mask &= ~(1 << (bit % 32)); // remove it.
            		
			--mask->bits;
            		if (pBlist->tar_mask == 0) 
			{
                		if (last)
                    			last->next = pBlist->next;
                		else
                    			mask->int_list = pBlist->next;
                		free(pBlist);
                		--mask->masks;
            		}
           		return TRUE;
        	}
        	last = pBlist;
    	}
    	return FALSE; 
}

int set_bit(BITMASK *mask, sh_int bit) 
{
    	BMlist *pBlist;

    	--bit;

    	for(pBlist = mask->int_list;pBlist;pBlist = pBlist->next) 
		{
        	if (pBlist->set == bit / 32) 
			{ 
            		if (pBlist->tar_mask & (1 << (bit % 32)))
                		return FALSE;

            		pBlist->tar_mask |= 1 << (bit % 32); 
            		++mask->bits;
            		return 1; 
        	}
    	}

    	pBlist = malloc(sizeof(BMlist));
    	++mask->masks;
    	pBlist->next = mask->int_list;
    	mask->int_list = pBlist;
    	pBlist->tar_mask = 0;
    	pBlist->set = bit / 32;
    	pBlist->tar_mask |= 1 << (bit % 32);
    	++mask->bits;
    	return 2; 
}

int is_set(BITMASK *mask, sh_int bit) 
{
    	BMlist *pBlist;

    	--bit;

    	for(pBlist = mask->int_list;pBlist;pBlist = pBlist->next)
	{
        	if (pBlist->set == bit / 32) 
		{
            		if (pBlist->tar_mask & 1 << (bit % 32))
                		return TRUE;
            		else
                		return FALSE;
        	}
	}
    	return FALSE;
}

int *serialize_bitmask(BITMASK *mask, int *len) 
{
	BMlist *pBlist;
	
	if (mask->bits <= 0) 
		return NULL;
		
	int *ilist = (int *)malloc(sizeof(int) * mask->bits), i = 0, z;
	*len = mask->bits;
	
	if (!ilist) return NULL;
	
	ilist[mask->bits - 1] = 0;

	for(pBlist = mask->int_list;pBlist;pBlist = pBlist->next) 
	{
		for(z = 0;z < 32;++z) 
		{
			if (i > mask->bits) 	
			{
					break;
			}

			if (pBlist->tar_mask & 1 << z)
				ilist[i++] = pBlist->set * 32 + z + 1;
		}
	}

	if (i < mask->bits + 1) 
	{
		// Problem
	}

	return ilist;
}

void free_bitmask(BITMASK *pBmask) 
{
    	BMlist *pBMlist, *next;
    	int found = 1;
    
	for(pBMlist = pBmask->int_list;pBMlist;pBMlist = next) 
	{
        	next = pBMlist->next;
        	free_mem(pBMlist, sizeof(pBMlist) );
        	found = 2;
    	}
    	return;
}

BITMASK init_bitmask(BITMASK *bm) 
{
    	static BITMASK bmzero;
    	if (bm == 0)
        	return bmzero;

    	*bm = bmzero;
    	return bmzero;
}

void load_bitmask(BITMASK *pBmask, FILE *fp) 
{
    	int i;
    	BMlist *pBMlist;

    	pBmask->masks = fread_number(fp);
    	pBmask->bits = fread_number(fp);

    	for(i = 0;i < pBmask->masks;i++) 
		{
        	pBMlist = malloc(sizeof(BMlist));
        	pBMlist->set = fread_number(fp);
        	pBMlist->tar_mask = fread_number(fp);
		
			//if (pBmask->int_list)
				pBMlist->next = pBmask->int_list;
			//else
			//	pBMlist->next = NULL;
				
			pBmask->int_list = pBMlist;
    	}
		
		
}

void save_bitmask(BITMASK *pBmask, FILE *fp) 
{
    	BMlist *pBMlist = pBmask->int_list;
		int i;

    	fprintf(fp, "%ld %ld", pBmask->masks, pBmask->bits);
		
		for(i = 0;i < pBmask->masks;i++) {
			fprintf(fp, " %ld %ld", pBMlist->set, pBMlist->tar_mask);
			pBMlist = pBMlist->next;
		}
		/*//Bug fix - prevent next = self which was causing pfile bitmask duplications
    	for(pBMlist = pBmask->int_list;pBMlist && pBMlist != last;pBMlist = pBMlist->next) {
			last = pBMlist;
        	
		}*/

    	fprintf(fp, "\n");
}

void load_old_bits(BITMASK *pBmask, FILE *fp)
{
	long temp;
	int i = 0;

	temp = fread_flag( fp );
	
	for( i = 0; i < 32 ; i++ )
	{	if(IS_SET( temp, 1 << i ) )
			set_bit(pBmask, i+1);
	}
	
	return;
}

void multi_set_bit(BITMASK *bm, int bits)
{
     	int i;

	for(i = 0; i < 32;i++)
       		if (IS_SET(bits, 1 << i))
			set_bit(bm, i+1);
     
	return;
}

BITMASK dup_bitmask(BITMASK *bm)
{
	int ilist_len;
	int *ilist = (int *)serialize_bitmask(bm, &ilist_len),i;
	
	BITMASK dup = init_bitmask(NULL);
	
	if (!ilist) 
		return dup;
	
	for(i = 0; i < ilist_len; i++)
	{
		set_bit(&dup, ilist[i]);
	}

	free(ilist);
	return dup;
}

char * bitmask_string( BITMASK *bm, const struct flag_type *flags )
{	int i;
	static char string[MSL];
	string[0] = '\0';

	for( i = 0 ; flags[i].name != NULL ; i++ )
	{	
		if(is_set(bm, flags[i].bit) )
		{	
			strcat(string, flags[i].name);
			strcat(string, " ");
		}
	}
	return string;
}

