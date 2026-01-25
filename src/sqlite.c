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

/***************************************************************************
 *  SQLite3 Database Interface - Converted from MySQL                      *
 *  Provides simpler deployment with file-based database                   *
 ***************************************************************************/

#include "include.h"

/*
 * Get a new SQLite3 connection.
 * Caller is responsible for calling sqlite3_close() when done.
 * Returns NULL on failure.
 */
sqlite3 *get_sqlite_connection(void)
{
	sqlite3 *db = NULL;
	int rc;

	rc = sqlite3_open(SQLITE_DB_PATH, &db);

	if (rc != SQLITE_OK)
	{
		n_logf("get_sqlite_connection: Failed to open database");
		n_logf("  Error: %s", sqlite3_errmsg(db));
		if (db)
			sqlite3_close(db);
		return NULL;
	}

	// Enable foreign keys (off by default in SQLite)
	sqlite3_exec(db, "PRAGMA foreign_keys = ON;", NULL, NULL, NULL);

	return db;
}

void init_sqlite(void)
{
	sqlite3 *test_db = get_sqlite_connection();

	if (test_db)
	{
		log_string("init_sqlite: Successfully connected to SQLite database.");
		sqlite3_close(test_db);
	}
	else
	{
		// Fatal error - cannot start without database
		log_string("========================================");
		log_string("FATAL ERROR: Cannot connect to SQLite database!");
		log_string("Database path: " SQLITE_DB_PATH);
		log_string("Please check:");
		log_string("  1. Database file exists and is readable");
		log_string("  2. Directory has proper permissions");
		log_string("  3. Database is not corrupted");
		log_string("  4. Disk space is available");
		log_string("========================================");
		log_string("Server startup ABORTED.");
		exit(1);
	}
	return;
}

void close_db(void)
{
	// No-op since we don't keep persistent connections
	log_string("Close_db: No persistent connection to close.");
	return;
}

/*
 * Helper function to escape strings for SQLite.
 * SQLite uses single quotes, and escapes them by doubling.
 * Returns escaped string in static buffer.
 */
static char *sqlite_escape_string(const char *str)
{
	static char escaped[MAX_STRING_LENGTH * 2];
	char *out = escaped;
	const char *in = str;

	if (!str)
		return "";

	while (*in && (out - escaped) < (MAX_STRING_LENGTH * 2 - 2))
	{
		if (*in == '\'')
		{
			*out++ = '\'';
			*out++ = '\'';
		}
		else
		{
			*out++ = *in;
		}
		in++;
	}
	*out = '\0';
	return escaped;
}

/*
 * Execute a query that doesn't need results (INSERT, UPDATE, DELETE).
 * Opens connection, executes query, closes connection immediately.
 * Returns 0 on success, -1 on failure.
 */
int sqlite_safe_query(char *fmt, ...)
{
	va_list argp;
	int i = 0;
	double j = 0;
	long int l = 0;
	char *s = 0, *out = 0, *p = 0;
	char query[MAX_STRING_LENGTH];
	int result = -1;
	sqlite3 *db = NULL;
	char *err_msg = NULL;

	*query = '\0';

	// Get a fresh connection for this query
	db = get_sqlite_connection();
	if (!db)
	{
		n_logf("SQLite query aborted: could not establish connection");
		return -1;
	}

	va_start(argp, fmt);

	for (p = fmt, out = query; *p != '\0'; p++)
	{
		if (*p != '%')
		{
			*out++ = *p;
			continue;
		}
		switch (*++p)
		{
		case 'c':
			i = va_arg(argp, int);
			out += sprintf(out, "%c", i);
			break;
		case 's':
			s = va_arg(argp, char *);
			if (!s)
			{
				out += sprintf(out, " ");
				break;
			}
			out += sprintf(out, "%s", sqlite_escape_string(s));
			break;
		case 'd':
			i = va_arg(argp, int);
			out += sprintf(out, "%d", i);
			break;
		case 'f':
			j = va_arg(argp, double);
			out += sprintf(out, "%f", j);
			break;
		case 'l':
			// Check if next character is 'd' for %ld format
			if (*(p + 1) == 'd')
			{
				p++; // consume the 'd'
				l = va_arg(argp, long int);
				out += sprintf(out, "%ld", l);
			}
			else
			{
				// Just %l by itself (shouldn't happen, but handle it)
				l = va_arg(argp, long int);
				out += sprintf(out, "%ld", l);
			}
			break;
		case '%':
			out += sprintf(out, "%%");
			break;
		}
	}
	*out = '\0';

	va_end(argp);

	result = sqlite3_exec(db, query, NULL, NULL, &err_msg);

	if (result != SQLITE_OK)
	{
		n_logf("SQLite Error: %s\n\r--- Offending Query ---\n\r%s\n\r", err_msg, query);
		sqlite3_free(err_msg);
		sqlite3_close(db);
		return -1;
	}

	// Close the connection immediately after the query
	sqlite3_close(db);
	return 0;
}

/*
 * Fetch the next row from a result set.
 * Returns NULL when no more rows available.
 * Compatible with mysql_fetch_row().
 */
SQL_ROW sqlite_fetch_row(SQL_RES *res)
{
	if (!res || res->current_row >= res->num_rows)
		return NULL;

	return res->rows[res->current_row++];
}

/*
 * Get the number of rows in a result set.
 * Compatible with mysql_num_rows().
 */
int sqlite_num_rows(SQL_RES *res)
{
	if (!res)
		return 0;
	return res->num_rows;
}

/*
 * Free a result set and close the associated database connection.
 * Compatible with mysql_free_result().
 */
void sqlite_free_result(SQL_RES *res)
{
	int i, j;

	if (!res)
		return;

	// Free all row data
	if (res->rows)
	{
		for (i = 0; i < res->num_rows; i++)
		{
			if (res->rows[i])
			{
				for (j = 0; j < res->num_cols; j++)
				{
					if (res->rows[i][j])
						free(res->rows[i][j]);
				}
				free(res->rows[i]);
			}
		}
		free(res->rows);
	}

	// Close database connection
	if (res->db)
		sqlite3_close(res->db);

	free(res);
}

/*
 * Execute a query and return results in MySQL-compatible format.
 * Stores all results in memory, closes database connection.
 * Caller MUST call sqlite_free_result() when done.
 * Returns NULL on failure.
 */
SQL_RES *sqlite_safe_query_with_result(char *fmt, ...)
{
	va_list argp;
	int i = 0, row_idx = 0, col_idx = 0;
	double j = 0;
	long int l = 0;
	char *s = 0, *out = 0, *p = 0;
	char query[MAX_STRING_LENGTH];
	sqlite3_stmt *stmt = NULL;
	sqlite3 *db = NULL;
	SQL_RES *result = NULL;
	int rc, num_cols;
	const char *col_text;

	*query = '\0';

	// Get a fresh connection for this query
	db = get_sqlite_connection();
	if (!db)
	{
		n_logf("SQLite query aborted: could not establish connection");
		return NULL;
	}

	va_start(argp, fmt);

	// Build query string
	for (p = fmt, out = query; *p != '\0'; p++)
	{
		if (*p != '%')
		{
			*out++ = *p;
			continue;
		}
		switch (*++p)
		{
		case 'c':
			i = va_arg(argp, int);
			out += sprintf(out, "%c", i);
			break;
		case 's':
			s = va_arg(argp, char *);
			if (!s)
			{
				out += sprintf(out, " ");
				break;
			}
			out += sprintf(out, "%s", sqlite_escape_string(s));
			break;
		case 'd':
			i = va_arg(argp, int);
			out += sprintf(out, "%d", i);
			break;
		case 'f':
			j = va_arg(argp, double);
			out += sprintf(out, "%f", j);
			break;
		case 'l':
			if (*(p + 1) == 'd')
			{
				p++;
				l = va_arg(argp, long int);
				out += sprintf(out, "%ld", l);
			}
			else
			{
				l = va_arg(argp, long int);
				out += sprintf(out, "%ld", l);
			}
			break;
		case '%':
			out += sprintf(out, "%%");
			break;
		}
	}
	*out = '\0';
	va_end(argp);

	// Prepare statement
	rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
	if (rc != SQLITE_OK)
	{
		n_logf("SQLite Error: %s\n\r--- Offending Query ---\n\r%s\n\r", sqlite3_errmsg(db), query);
		sqlite3_close(db);
		return NULL;
	}

	// Allocate result structure
	result = (SQL_RES *)malloc(sizeof(SQL_RES));
	if (!result)
	{
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return NULL;
	}

	result->db = db;
	result->current_row = 0;
	result->num_cols = sqlite3_column_count(stmt);
	num_cols = result->num_cols;

	// Count rows first
	result->num_rows = 0;
	while (sqlite3_step(stmt) == SQLITE_ROW)
		result->num_rows++;

	// Reset statement to fetch data
	sqlite3_reset(stmt);

	// Allocate rows array
	result->rows = (char ***)malloc(sizeof(char **) * result->num_rows);
	if (!result->rows)
	{
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		free(result);
		return NULL;
	}

	// Fetch all rows
	row_idx = 0;
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		result->rows[row_idx] = (char **)malloc(sizeof(char *) * num_cols);
		if (!result->rows[row_idx])
		{
			sqlite_free_result(result);
			sqlite3_finalize(stmt);
			return NULL;
		}

		for (col_idx = 0; col_idx < num_cols; col_idx++)
		{
			col_text = (const char *)sqlite3_column_text(stmt, col_idx);
			if (col_text)
			{
				result->rows[row_idx][col_idx] = strdup(col_text);
			}
			else
			{
				result->rows[row_idx][col_idx] = strdup("");
			}
		}
		row_idx++;
	}

	sqlite3_finalize(stmt);
	// Note: db connection stays open, will be closed in sqlite_free_result()

	return result;
}

void do_pktrack(CHAR_DATA *ch, char *argument)
{
	SQL_ROW row;
	SQL_RES *res_set;
	BUFFER *buffer;
	char arg1[MSL];
	char buf[MSL];
	int i = 0;
	bool found = FALSE;

	if (!str_cmp(argument, ""))
	{
		return send_to_char("Syntax: pktrack <wins/losses/all/date/location> <value>\n\r", ch);
	}
	buffer = new_buf();
	argument = one_argument(argument, arg1);

	if (!str_cmp(arg1, "wins"))
		res_set = sqlite_safe_query_with_result("SELECT * FROM pklogs WHERE killer LIKE '%s'", argument);
	else if (!str_cmp(arg1, "losses"))
		res_set = sqlite_safe_query_with_result("SELECT * FROM pklogs WHERE dead LIKE '%s'", argument);
	else if (!str_cmp(arg1, "all"))
		res_set = sqlite_safe_query_with_result("SELECT * FROM pklogs WHERE killer LIKE '%s' OR dead LIKE '%s'", argument, argument);
	else if (!str_cmp(arg1, "date"))
		res_set = sqlite_safe_query_with_result("SELECT * FROM pklogs WHERE time LIKE '%s'", argument);
	else if (!str_cmp(arg1, "location"))
		res_set = sqlite_safe_query_with_result("SELECT * FROM pklogs WHERE room LIKE '%s'", argument);
	else
		return send_to_char("Invalid option.\n\r", ch);

	if (res_set == NULL)
	{
		send_to_char("Error accessing results.\n\r", ch);
		return;
	}
	else if (sqlite_num_rows(res_set) == 0)
	{
		send_to_char("No results found.\n\r", ch);
		sqlite_free_result(res_set);
		return;
	}
	else
	{
		while ((row = sqlite_fetch_row(res_set)) != NULL)
		{
			sprintf(buf, "%3d) %s killed %s at %s on %s",
					++i,
					row[1],
					row[0],
					row[2],
					row[3]);
			add_buf(buffer, buf);
			found = TRUE;
		}
		if (!found)
		{
			send_to_char("No matching results found.\n\r", ch);
		}
		else
		{
			page_to_char(buf_string(buffer), ch);
		}
		sqlite_free_result(res_set);
		free_buf(buffer);
	}
	return;
}

void login_log(CHAR_DATA *ch, int type)
{
	char vbuf[MSL] = "0";
	// HUH?: If the character doesn't have a description, don't log it?
	if (ch->desc == NULL)
	{
		return;
	}
	// If the character is in a room, use that room number in vbuf
	if (ch->in_room != NULL)
	{
		sprintf(vbuf, "%ld", ch->in_room->vnum);
	}
	// Insert the info into the traffic tables.
	// Note: IP stored as text in SQLite3 instead of MySQL's INET_ATON integer format
	sqlite_safe_query("INSERT INTO traffic VALUES('%s', '%s', '%s', datetime('now'), %d, '%s')",
					 ch->original_name,
					 type == LTYPE_AUTO ? ch->desc->ip : ch->desc->ip,
					 type == LTYPE_AUTO ? ch->desc->host : ch->desc->host,
					 type,
					 vbuf);
}

void saveCharmed(CHAR_DATA *ch)
{
	CHAR_DATA *charm;

	// Check if it's an NPC for some reason, and don't do it if it's an NPC
	if (IS_NPC(ch))
	{
		return;
	}
	// Clear out any previous charmies from the database
	if (sqlite_safe_query("DELETE FROM charmed WHERE owner='%s'", ch->original_name) != 0)
	{
		return;
	}
	// Save all the charmies that are into the room to the database.
	for (charm = char_list; charm != NULL; charm = charm->next)
	{
		if (charm->in_room == ch->in_room && charm->leader == ch && IS_NPC(charm))
		{
			sqlite_safe_query("INSERT INTO charmed VALUES('%s','%d','%s','%s','%s',%d,%d,%d,%d,%d,%d,%d)",
							 charm->leader->original_name,
							 charm->pIndexData->vnum,
							 charm->name,
							 charm->short_descr,
							 charm->long_descr,
							 charm->level,
							 charm->max_hit,
							 charm->hit,
							 charm->alignment,
							 charm->damage[DICE_NUMBER],
							 charm->damage[DICE_TYPE],
							 charm->damroll);
		}
	}
	return;
}

void loadCharmed(CHAR_DATA *ch)
{
	SQL_ROW row;
	SQL_RES *result;
	CHAR_DATA *mob;
	long vnum = 0;
	AFFECT_DATA af;

	// Get charmies from database
	result = sqlite_safe_query_with_result("SELECT * FROM charmed WHERE owner='%s'", ch->original_name);

	// Nothing to do if we have no results.
	if (!result || sqlite_num_rows(result) == 0)
	{
		if (result)
			sqlite_free_result(result);
		return;
	}

	// Load up them bad boys.
	while ((row = sqlite_fetch_row(result)) != NULL)
	{
		vnum = atoi(row[1]);
		// If it doesn't have a valid vnum, skip it.
		if (get_mob_index(vnum) == NULL)
		{
			bug("LoadCharmed: invalid vnum %ld.", vnum);
			continue;
		}
		mob = create_mobile(get_mob_index(vnum));
		// HUH?: Free up descriptions of the mob for some reason?
		free_string(mob->name);
		free_string(mob->short_descr);
		free_string(mob->long_descr);
		// Set all the mob info.
		mob->name = str_dup(row[2]);
		mob->short_descr = str_dup(row[3]);
		mob->long_descr = str_dup(row[4]);
		mob->level = atoi(row[5]);
		mob->max_hit = atoi(row[6]);
		mob->hit = atoi(row[7]);
		mob->alignment = atoi(row[8]);
		mob->damage[DICE_NUMBER] = atoi(row[9]);
		mob->damage[DICE_TYPE] = atoi(row[10]);
		mob->damroll = atoi(row[11]);
		// Put the mob in the room and have them follow the char.
		char_to_room(mob, ch->in_room);
		add_follower(mob, ch);
		mob->leader = ch;

		// Set the charm affect when they're brought back.
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.level = ch->level;
		af.aftype = AFT_SPELL;
		af.location = 0;
		af.modifier = 0;
		af.duration = -1;
		af.bitvector = AFF_CHARM;
		// Set to animate dead if it's a zombie, otherwise we'll assume it's charm person.
		if (mob->pIndexData->vnum == MOB_VNUM_ZOMBIE)
		{
			af.type = skill_lookup("animate dead");
		}
		else
		{
			af.type = skill_lookup("charm person");
		}
		affect_to_char(mob, &af);

		// Some cool gechoes for the mob creation.
		if (mob->pIndexData->vnum == MOB_VNUM_ZOMBIE)
		{
			act("$N rises from the earth and bows before $n, awaiting command.", ch, 0, mob, TO_ROOM);
			act("$N rises from the earth and bows before you, awaiting command.", ch, 0, mob, TO_CHAR);
		}
		else if (mob->pIndexData->vnum == MOB_VNUM_FIRE_ELEMENTAL)
		{
			act("$N blasts into the room in a burst of flames, ready to serve $n.", ch, 0, mob, TO_ROOM);
			act("$N blasts into the room in a burst of flames, ready to serve you.", ch, 0, mob, TO_CHAR);
		}
		else if (mob->pIndexData->vnum == MOB_VNUM_WATER_ELEMENTAL)
		{
			act("$N rises from a nearby puddle, ready to serve $n.", ch, 0, mob, TO_ROOM);
			act("$N rises from a nearby puddle, ready to serve you.", ch, 0, mob, TO_CHAR);
		}
		else if (mob->pIndexData->vnum == MOB_VNUM_AIR_ELEMENTAL)
		{
			act("$N emerges from a swirling cyclone, ready to serve $n.", ch, 0, mob, TO_ROOM);
			act("$N emerges from a swirling cyclone, ready to serve you.", ch, 0, mob, TO_CHAR);
		}
		else if (mob->pIndexData->vnum == MOB_VNUM_EARTH_ELEMENTAL)
		{
			act("The earth rumbles and $N emerges from the ground, ready to serve $n.", ch, 0, mob, TO_ROOM);
			act("The earth rumbles and $N emerges from the ground, ready to serve you.", ch, 0, mob, TO_CHAR);
		}
		else
		{
			act("$N materializes and nods loyally towards $n.", ch, 0, mob, TO_ROOM);
			act("$N materializes and nods loyally towards you.", ch, 0, mob, TO_CHAR);
		}
	}
	sqlite_free_result(result);
	return;
}

void printCharmed(CHAR_DATA *ch, char *name)
{
	SQL_ROW row;
	SQL_RES *result;
	BUFFER *buffer;
	int c = 0;
	char buf[MSL];

	// Select all the charmies of this person.
	result = sqlite_safe_query_with_result("SELECT * FROM charmed WHERE owner='%s'", name);

	buffer = new_buf();

	// If we have a non-empty result, print that bitch out.
	if (result && sqlite_num_rows(result))
	{
		send_to_char("Num Vnum     Charmed Name\n\r", ch);
		send_to_char("--------------------------------------------------\n\r", ch);
		// Go through and calculate how many of each charmie they have
		while ((row = sqlite_fetch_row(result)))
		{
			c++;
			sprintf(buf, "%-2d) %-6ld - %s\n\r", c, (long)atoi(row[1]), row[3]);
			add_buf(buffer, buf);
		}
		// Give them the result.
		page_to_char(buf_string(buffer), ch);
		// Free up memory.
		sqlite_free_result(result);
		free_buf(buffer);
	}
	else
	{
		printf_to_char(ch, "No charmed mobs have been recorded for %s.\n\r", name);
		if (result)
			sqlite_free_result(result);
	}
	free_buf(buffer);
	return;
}

void do_charmed(CHAR_DATA *ch, char *argument)
{
	if (IS_NPC(ch))
		return;

	if (argument[0] == '\0')
	{
		send_to_char("Syntax: charmed <player name>\n\r", ch);
		return;
	}

	printCharmed(ch, argument);
	return;
}

void pruneDatabase(void)
{
	// Notes
	if (sqlite_safe_query("DELETE FROM notes WHERE timestamp + 2592000 < %d", current_time) != 0)
	{
		n_logf("MySQL: Failed to clean notes table.");
	}
	else
	{
		n_logf("MySQL: Notes tables has been cleaned.");
	}

	// Logins
	if (sqlite_safe_query("DELETE FROM logins WHERE ctime + 5184000 < %d", current_time) != 0)
	{
		n_logf("MySQL: Failed to clean logins table.");
	}
	else
	{
		n_logf("MySQL: Login records have been cleaned.");
	}

	return;
}

void updatePlayerAuth(CHAR_DATA *ch)
{
	sqlite_safe_query("DELETE FROM player_auth WHERE name='%s'",
					 ch->original_name);
	sqlite_safe_query("INSERT INTO player_auth VALUES('%s','%s',%d)",
					 ch->original_name,
					 ch->pcdata->pwd,
					 ch->level);
	return;
}

void delete_char(char *name, bool save_pfile)
{
	char buf2[MSL];
	name = capitalize(name);

	// Wipe bounties
	if (sqlite_safe_query("DELETE FROM bounties WHERE victim='%s'", name))
	{
		n_logf("Delete_char: Wiped bounties for %s.", name);
	}
	else
	{
		n_logf("Delete_char: Failed to wipe bounties for %s.", name);
	}
	// Wipe charmies
	if (sqlite_safe_query("DELETE FROM charmed WHERE owner='%s'", name))
	{
		n_logf("Delete_char: wiped saved charmed mobs for %s.", name);
	}
	else
	{
		n_logf("Delete_char: Failed to wipe charmed mobs for %s.", name);
	}
	// Wipe pklogs
	if (sqlite_safe_query("DELETE FROM pklogs WHERE dead='%s' OR killer='%s'", name, name))
	{
		n_logf("Delete_char: wiping pklogs for %s.", name);
	}
	else
	{
		n_logf("Delete_char: Failed to wipe pklogs for %s.", name);
	}
	//
	if (save_pfile)
	{
		sprintf(buf2, "mv %s%s.plr %sdead_char/%s.plr", PLAYER_DIR, name, PLAYER_DIR, name);
	}
	else
	{
		sprintf(buf2, "rm %s%s.plr", PLAYER_DIR, name);
	}
	system(buf2);
}

void updatePlayerDb(CHAR_DATA *ch)
{

	if (IS_NPC(ch))
		return;
	// Clear out old info
	sqlite_safe_query("DELETE FROM player_data WHERE name='%s'", ch->original_name);
	// Add new info.
	sqlite_safe_query("INSERT INTO player_data VALUES('%s',%d,%d,%d,%d,%d,%d,%d,%d)",
					 ch->original_name,
					 ch->level,
					 ch->race,
					 ch->class,
					 ch->cabal,
					 ch->sex,
					 ch->alignment,
					 ch->pcdata->ethos,
					 current_time);
	return;
}

void do_cabalstat(CHAR_DATA *ch, char *argument)
{
	SQL_ROW row;
	SQL_RES *res;
	int cabal = 0, member_count = 0;
	char buf[MSL], time[255], arg[MIL];
	BUFFER *buffer;
	long ttime;

	argument = one_argument(argument, arg);

	// Make sure they're allowed to see the output and have valid syntax.
	if (IS_IMMORTAL(ch))
	{
		if (arg[0] == '\0')
		{
			if (ch->cabal != 0)
			{
				cabal = ch->cabal;
			}
			else
			{
				send_to_char("You must either specify a cabal or be a member of a cabal.\n\r", ch);
				return;
			}
		}
		else
		{
			cabal = cabal_lookup(arg);
			if (cabal == 0 || cabal > MAX_CABAL)
			{
				send_to_char("No such cabal.\n\r", ch);
				return;
			}
		}
	}
	else
	{
		if (ch->cabal == 0)
		{
			send_to_char("You aren't in a cabal.\n\r", ch);
			return;
		}

		if (ch->pcdata->induct != CABAL_LEADER)
		{
			send_to_char("You aren't the leader of your cabal.\n\r", ch);
			return;
		}
		cabal = ch->cabal;
	}

	buffer = new_buf();

	// If they've gotten past the gatekeepers, they should be allowed to see this.
	res = sqlite_safe_query_with_result("SELECT * FROM player_data WHERE cabal=%d AND level <= %d", cabal, ch->level);

	if (!res)
	{
		send_to_char("Error accessing member list.\n\r", ch);
		free_buf(buffer);
		return;
	}

	send_to_char("Member Name     Last Login\n\r-----------     ----------\n\r", ch);
	// Count the number of members for the cabals and print that shit out.
	if (res)
	{
		while ((row = sqlite_fetch_row(res)) != NULL)
		{
			member_count++;

			// Safety checks for NULL values
			// Schema: name(0), level(1), race(2), class(3), cabal(4), sex(5), alignment(6), ethos(7), ctime(8)
			if (row[0] == NULL)
				continue;

			// Show N/A if no login time available
			if (row[8] == NULL || row[8][0] == '\0' || atoi(row[8]) == 0)
			{
				sprintf(buf, "%-11s     N/A\n\r", row[0]);
			}
			else
			{
				ttime = atoi(row[8]);
				struct tm *timeinfo = localtime(&ttime);
				if (timeinfo == NULL)
				{
					sprintf(buf, "%-11s     N/A\n\r", row[0]);
				}
				else
				{
					strftime(time, 255, "%A %b %e", timeinfo);
					sprintf(buf, "%-11s     %-10s\n\r", row[0], time);
				}
			}
			add_buf(buffer, buf);
		}
		sqlite_free_result(res);
		page_to_char(buf_string(buffer), ch);
		free_buf(buffer);
	}
	else
	{
		send_to_char("DB Error.\n\r", ch);
	}

	printf_to_char(ch, "\n\rTotal Members Found: %d.\n\r", member_count);
	return;
}

void do_ltrack(CHAR_DATA *ch, char *argument)
{
	SQL_ROW row;
	SQL_RES *results;
	BUFFER *buffer;
	char arg1[MIL];
	char arg2[MIL];
	char buf[MSL];
	int type = -1;

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);
	// If you don't know how to use the command, you should feel bad son.
	if (arg1[0] == '\0')
	{
		send_to_char("Syntax: ltrack <name/date/hostname/ip/vnum> [type]\n\r", ch);
		send_to_char("        ltrack % [type]\n\r", ch);
		send_to_char("Type can be: new, in, or out\n\r", ch);
		send_to_char("Use '%' to show all recent login records (limit 100)\n\r", ch);
		return;
	}

	if (arg2[0] != '\0')
	{
		if (!str_cmp(arg2, "new"))
		{
			type = LTYPE_NEW;
		}
		else if (!str_cmp(arg2, "in"))
		{
			type = LTYPE_IN;
		}
		else if (!str_cmp(arg2, "out"))
		{
			type = LTYPE_OUT;
		}
	}

	buffer = new_buf();
	// For some reason we want to send the type to a buffer and use it instead of just
	// using the variable...?
	sprintf(buf, "%d", type);

	// Note: IP stored as text in SQLite3, no need for INET_NTOA conversion
	results = sqlite_safe_query_with_result("SELECT name, ip, hostname, time, type, vnum FROM traffic WHERE (name LIKE '%s' OR ip LIKE '%s' OR hostname LIKE '%%%s%%' or time LIKE '%s' or vnum like '%s') %s%s ORDER BY time ASC LIMIT 100",
					 arg1,
					 arg1,
					 arg1,
					 arg1,
					 arg1,
					 type > -1 ? "AND type=" : "",
					 type > -1 ? buf : "");

	if (results == NULL)
	{
		send_to_char("Database error encountered.\n\r", ch);
		free_buf(buffer);
		return;
	}
	else if (sqlite_num_rows(results) == 0)
	{
		send_to_char("No matching results were found.\n\r", ch);
		sqlite_free_result(results);
		free_buf(buffer);
		return;
	}
	else
	{
		while ((row = sqlite_fetch_row(results)) != NULL)
		{
			type = atoi(row[4]);
			sprintf(buf, "%s : %-8s [%-5ld] %s@%s (%s)\n\r",
					row[3],
					type == LTYPE_NEW ? "NEW CHAR" : type == LTYPE_IN ? "LOGIN"
												 : type == LTYPE_OUT  ? "LOG OUT"
																	  : "AUTOQUIT",
					atol(row[5]),
					row[0],
					row[2],
					row[1]);
			add_buf(buffer, buf);
		}
		sqlite_free_result(results);
		page_to_char(buf_string(buffer), ch);
		free_buf(buffer);
	}
	return;
}
