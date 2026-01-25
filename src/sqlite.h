/***************************************************************************
 *  SQLite3 Database Interface for KBK MUD                                 *
 *  Converted from MySQL to SQLite3 for simpler deployment                 *
 ***************************************************************************/

#ifndef SQLITE_H
#define SQLITE_H

#include <sqlite3.h>

/* Database configuration */
#define SQLITE_DB_PATH "../data/kbk.db"

/* SQLite3 result set wrapper to mimic MySQL behavior */
typedef struct sqlite_result_set SQL_RES;
typedef char** SQL_ROW;

struct sqlite_result_set
{
	char ***rows;          /* Array of rows, each row is array of strings */
	int num_rows;          /* Number of rows */
	int num_cols;          /* Number of columns */
	int current_row;       /* Current row index for fetching */
	sqlite3 *db;           /* Database connection (to be closed when done) */
};

/* Function declarations */
sqlite3 *get_sqlite_connection args((void));
void init_sqlite args((void));
void close_db args((void));
int sqlite_safe_query args((char *fmt, ...));
SQL_RES *sqlite_safe_query_with_result args((char *fmt, ...));
SQL_ROW sqlite_fetch_row args((SQL_RES *res));
int sqlite_num_rows args((SQL_RES *res));
void sqlite_free_result args((SQL_RES *res));
void login_log args((CHAR_DATA * ch, int type));
void saveCharmed args((CHAR_DATA * ch));
void loadCharmed args((CHAR_DATA * ch));
void printCharmed args((CHAR_DATA * ch, char *name));
void pruneDatabase args((void));
void updatePlayerAuth args((CHAR_DATA * ch));
void updatePlayerDb args((CHAR_DATA * ch));

/* Login type constants */
#define LTYPE_IN 0
#define LTYPE_OUT 1
#define LTYPE_NEW 2
#define LTYPE_AUTO 3

#endif /* SQLITE_H */

