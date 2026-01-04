#!/bin/bash

# Set the port number.
KBK_PORT=8989
KBK_LOG_INDEX=0
MYSQL_AVAIL=false

# Wait for mysql service to be ready:
echo "Waiting for MySQL to be ready..."
until $MYSQL_AVAIL; do
    # Simple TCP connection check - doesn't require authentication
    if timeout 1 bash -c "cat < /dev/null > /dev/tcp/kbk-sql/3306" 2>/dev/null; then
        echo 'MySQL port is open, waiting 5 more seconds for initialization...'
        sleep 5
        MYSQL_AVAIL=true
    else
        echo "MySQL is unavailable - sleeping"
        sleep 2
    fi
done
echo "MySQL is ready!"

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

    # Run posMUD 2.
    ./pos2 $port # > $logfile

    # Restart, giving old connections a chance to die.
    if [ -f "shutdown.txt" ] ; then
        rm -f "shutdown.txt"
        exit 0
    fi
    sleep 10
done
