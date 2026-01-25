#!/bin/bash
#
# Migrate data from MySQL to SQLite3
# Usage: ./migrate_mysql_to_sqlite.sh
#

set -e

# MySQL connection details
MYSQL_HOST="${MYSQL_HOST:-kbk-sql}"
MYSQL_USER="${MYSQL_USER:-kbkuser}"
MYSQL_PASS="${MYSQL_PASS:-kbkpassword}"
MYSQL_DB="${MYSQL_DB:-kbkdatabase}"

# SQLite database path
SQLITE_DB="./kbk.db"

echo "=========================================="
echo "MySQL to SQLite3 Migration Script"
echo "=========================================="
echo ""
echo "MySQL Host: $MYSQL_HOST"
echo "MySQL Database: $MYSQL_DB"
echo "SQLite Database: $SQLITE_DB"
echo ""

# Check if MySQL is accessible
if ! mysql -h "$MYSQL_HOST" -u "$MYSQL_USER" -p"$MYSQL_PASS" -e "USE $MYSQL_DB" 2>/dev/null; then
    echo "ERROR: Cannot connect to MySQL database"
    echo "Please check your MySQL connection settings"
    exit 1
fi

echo "✓ MySQL connection successful"
echo ""

# Initialize SQLite database
echo "Initializing SQLite database..."
if [ -f "$SQLITE_DB" ]; then
    echo "WARNING: $SQLITE_DB already exists"
    read -p "Do you want to overwrite it? (yes/no): " confirm
    if [ "$confirm" != "yes" ]; then
        echo "Migration cancelled"
        exit 0
    fi
    rm -f "$SQLITE_DB"
fi

sqlite3 "$SQLITE_DB" < init_sqlite.sql
echo "✓ SQLite database initialized"
echo ""

# Function to migrate a table
migrate_table() {
    local table=$1
    local transform=$2
    
    echo "Migrating table: $table"
    
    # Export from MySQL
    mysql -h "$MYSQL_HOST" -u "$MYSQL_USER" -p"$MYSQL_PASS" "$MYSQL_DB" \
        -e "SELECT * FROM $table" -B -N > /tmp/${table}.tsv
    
    # Count rows
    local row_count=$(wc -l < /tmp/${table}.tsv)
    echo "  Found $row_count rows"
    
    if [ $row_count -eq 0 ]; then
        echo "  (empty table, skipping)"
        rm -f /tmp/${table}.tsv
        return
    fi
    
    # Transform and import to SQLite
    if [ -n "$transform" ]; then
        eval "$transform" < /tmp/${table}.tsv | sqlite3 "$SQLITE_DB"
    else
        cat /tmp/${table}.tsv | sqlite3 "$SQLITE_DB" ".import /dev/stdin $table"
    fi
    
    rm -f /tmp/${table}.tsv
    echo "  ✓ Migrated $row_count rows"
}

# Migrate each table
echo "Starting data migration..."
echo ""

# bounties table
migrate_table "bounties" "awk -F'\t' '{printf \"INSERT INTO bounties VALUES(%s,'\''%s'\'','\''%s'\'',%s);\\n\", \$1, \$2, \$3, \$4}'"

# notes table  
migrate_table "notes" "awk -F'\t' '{gsub(/'\''/, \"'\''\'\'''\", \$2); gsub(/'\''/, \"'\''\'\'''\", \$3); gsub(/'\''/, \"'\''\'\'''\", \$4); gsub(/'\''/, \"'\''\'\'''\", \$5); gsub(/'\''/, \"'\''\'\'''\", \$6); printf \"INSERT INTO notes VALUES(%s,'\''%s'\'','\''%s'\'','\''%s'\'','\''%s'\'','\''%s'\'',%s);\\n\", \$1, \$2, \$3, \$4, \$5, \$6, \$7}'"

# pklogs table
migrate_table "pklogs" "awk -F'\t' '{gsub(/'\''/, \"'\''\'\'''\", \$1); gsub(/'\''/, \"'\''\'\'''\", \$2); gsub(/'\''/, \"'\''\'\'''\", \$3); gsub(/'\''/, \"'\''\'\'''\", \$4); printf \"INSERT INTO pklogs VALUES('\''%s'\'','\''%s'\'','\''%s'\'','\''%s'\'');\\n\", \$1, \$2, \$3, \$4}'"

# traffic table (convert INET_ATON to text)
migrate_table "traffic" "awk -F'\t' 'BEGIN {cmd=\"mysql -h $MYSQL_HOST -u $MYSQL_USER -p$MYSQL_PASS $MYSQL_DB -B -N\"} {print \"SELECT INET_NTOA(\" \$2 \");\" | cmd; cmd | getline ip; close(cmd); gsub(/'\''/, \"'\''\'\'''\", \$1); gsub(/'\''/, \"'\''\'\'''\", \$3); gsub(/'\''/, \"'\''\'\'''\", \$4); gsub(/'\''/, \"'\''\'\'''\", \$6); printf \"INSERT INTO traffic VALUES('\''%s'\'','\''%s'\'','\''%s'\'','\''%s'\'',%s,'\''%s'\'');\\n\", \$1, ip, \$3, \$4, \$5, \$6}'"

# player_data table
migrate_table "player_data" "awk -F'\t' '{gsub(/'\''/, \"'\''\'\'''\", \$1); printf \"INSERT INTO player_data VALUES('\''%s'\'',%s,%s,%s,%s,%s,%s,%s,%s);\\n\", \$1, \$2, \$3, \$4, \$5, \$6, \$7, \$8, \$9}'"

# player_auth table
migrate_table "player_auth" "awk -F'\t' '{gsub(/'\''/, \"'\''\'\'''\", \$1); gsub(/'\''/, \"'\''\'\'''\", \$2); printf \"INSERT INTO player_auth VALUES('\''%s'\'','\''%s'\'',%s);\\n\", \$1, \$2, \$3}'"

# charmed table
migrate_table "charmed" "awk -F'\t' '{gsub(/'\''/, \"'\''\'\'''\", \$1); gsub(/'\''/, \"'\''\'\'''\", \$2); gsub(/'\''/, \"'\''\'\'''\", \$3); gsub(/'\''/, \"'\''\'\'''\", \$4); gsub(/'\''/, \"'\''\'\'''\", \$5); printf \"INSERT INTO charmed VALUES('\''%s'\'',%s,'\''%s'\'','\''%s'\'','\''%s'\'',%s,%s,%s,%s,%s,%s,%s);\\n\", \$1, \$2, \$3, \$4, \$5, \$6, \$7, \$8, \$9, \$10, \$11, \$12}'"

# logins and helpfiles tables are typically empty or unused, skip them

echo ""
echo "=========================================="
echo "Migration Complete!"
echo "=========================================="
echo ""
echo "SQLite database created at: $SQLITE_DB"
echo ""
echo "Verification:"
sqlite3 "$SQLITE_DB" "SELECT name, COUNT(*) as count FROM (
    SELECT 'bounties' as name, COUNT(*) as count FROM bounties
    UNION ALL SELECT 'notes', COUNT(*) FROM notes
    UNION ALL SELECT 'pklogs', COUNT(*) FROM pklogs
    UNION ALL SELECT 'traffic', COUNT(*) FROM traffic
    UNION ALL SELECT 'player_data', COUNT(*) FROM player_data
    UNION ALL SELECT 'player_auth', COUNT(*) FROM player_auth
    UNION ALL SELECT 'charmed', COUNT(*) FROM charmed
) GROUP BY name;"

echo ""
echo "Migration successful!"

