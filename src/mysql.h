MYSQL                   	conn;

#define SQL_SERVER      	"kbk-sql"
#define SQL_USER        	"kbkuser"
#define SQL_PWD         	"kbkpassword"
#define SQL_DB          	"kbkdatabase"

void    init_mysql			args( ( void ) );
void    close_db            args( ( void ) );
int     mysql_safe_query    args( (char *fmt, ...) );
void    login_log           args( (CHAR_DATA *ch, int type) );
void 	saveCharmed			args( (CHAR_DATA *ch) );
void 	loadCharmed			args( (CHAR_DATA *ch) );
void 	printCharmed		args( (CHAR_DATA *ch, char *name) );
void 	pruneDatabase		args( (void ) );
void 	updatePlayerAuth	args( (CHAR_DATA *ch) );
void	updatePlayerDb		args( (CHAR_DATA *ch) );

#define LTYPE_IN            0
#define LTYPE_OUT           1
#define LTYPE_NEW           2
#define LTYPE_AUTO			3
