#!/bin/bash

# Set the port number.
KBK_PORT=8989
KBK_LOG_INDEX=0
MYSQL_AVAIL=false

# Wait for mysql service to be ready:
until $MYSQL_AVAIL; do
    echo "MySQL is unavailable - sleeping"
    # Try to get a connection to the mysql server
    mysql -h kbk-sql -u kbkuser -pkbkpassword --execute '\q'
    LAST_RETURN=$?
    if [[ $LAST_RETURN == 0 ]]; then
        echo 'Found mysql server'
        MYSQL_AVAIL=true
    else
        sleep 1
    fi
done

# Set limits.
if [ -f "shutdown.txt" ] ; then
    rm "shutdown.txt"
fi

while true; do
    while true; do
        # Go through and loop until we find a log file number that doesn't exist already.
        KBK_LOGFILE=/kbk/log/$KBK_LOG_INDEX.log
        echo $KBK_LOGFILE
    	if [ ! -f "$KBK_LOGFILE" ]; then
            break
        fi
    	let "KBK_LOG_INDEX++"
    done

    # Start our high CPU killer
    /kbk/high_cpu_kill &

    # Run posMUD 2.
    ./pos2 $port # > $logfile

    # Restart, giving old connections a chance to die.
    if [ -f "shutdown.txt" ] ; then
        rm -f "shutdown.txt"
        exit 0
    fi
    sleep 10
done
