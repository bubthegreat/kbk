#!/bin/bash

# Set the port number.
KBK_PORT=8989
KBK_LOG_INDEX=0
MYSQL_AVAIL=false
MAX_WAIT=60
WAIT_COUNT=0

# Wait for mysql service to be ready:
echo "Waiting for MySQL to be ready..."
until $MYSQL_AVAIL; do
    # Simple TCP connection check - doesn't require authentication
    if timeout 1 bash -c "cat < /dev/null > /dev/tcp/kbk-sql/3306" 2>/dev/null; then
        echo 'MySQL port is open, waiting 2 more seconds for initialization...'
        sleep 2
        MYSQL_AVAIL=true
    else
        echo "MySQL is unavailable - sleeping (attempt $WAIT_COUNT/$MAX_WAIT)"
        sleep 1
        WAIT_COUNT=$((WAIT_COUNT + 1))
        if [ $WAIT_COUNT -ge $MAX_WAIT ]; then
            echo "ERROR: MySQL did not become available within $MAX_WAIT seconds"
            exit 1
        fi
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
