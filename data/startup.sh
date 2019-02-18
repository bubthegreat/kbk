#!/usr/bin/env bash

# Have to do some shenanigans due to a bug - we have to touch the files so
# all the file permissions/attributes are in place prior to trying to start
# the mysql service. https://github.com/dixudx/keystone-docker/issues/1
find /var/lib/mysql -type f -exec touch {} \;
CLOSED_CLEAN=false
while ! $CLOSED_CLEAN; do 
    echo "Attempting to start mysql service..."
    bad_close=$(service mysql start | grep 'not closed cleanly.')
    if [ -z "$bad_close" ]; then
        CLOSED_CLEAN=true
        echo "Successfully started mysql service..."
    fi
done

# Make database tables if they don't exist
mysql -vv -u root -h '127.0.0.1' -e "CREATE DATABASE IF NOT EXISTS kbkdatabase;"

# Configure tables if they don't exist
mysql -vv -u root -h '127.0.0.1' kbkdatabase < /kbk/data/configure_database.sql

cd /kbk/area && ./startup
