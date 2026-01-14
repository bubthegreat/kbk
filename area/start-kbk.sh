#!/bin/bash

# Set the port number.
KBK_PORT=8989
KBK_LOG_INDEX=0
MYSQL_AVAIL=false
MAX_WAIT=60
WAIT_COUNT=0

# Enable core dumps
ulimit -c unlimited
echo "Core dumps enabled - ulimit -c: $(ulimit -c)"
echo "Core pattern: $(cat /proc/sys/kernel/core_pattern 2>/dev/null || echo 'unable to read')"
echo "Current directory: $(pwd)"

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

    # Check if GDB debugging is enabled via environment variable
    if [ "$ENABLE_GDB_DEBUGGING" = "true" ]; then
        echo "GDB debugging is ENABLED - running under gdb (performance will be slower)"
        echo ""

        # Run under gdb to catch crashes in real-time
        gdb -batch \
            -ex "set pagination off" \
            -ex "handle SIGSEGV stop print" \
            -ex "handle SIGABRT stop print" \
            -ex "handle SIGFPE stop print" \
            -ex "handle SIGILL stop print" \
            -ex "run $port" \
            -ex "thread apply all bt full" \
            -ex "quit" \
            ./pos2 2>&1 | tee /tmp/gdb_output.log

        # Extract exit code from gdb output
        if grep -q "exited normally" /tmp/gdb_output.log; then
            EXIT_CODE=0
        elif grep -q "SIGSEGV\|SIGABRT\|SIGFPE\|SIGILL" /tmp/gdb_output.log; then
            EXIT_CODE=139
            echo ""
            echo "========================================"
            echo "CRASH DETECTED - $(date)"
            echo "Stack trace captured above by gdb"
            echo "========================================"
        else
            EXIT_CODE=1
        fi

        rm -f /tmp/gdb_output.log
    else
        echo "GDB debugging is DISABLED - running normally for best performance"
        echo "(Set ENABLE_GDB_DEBUGGING=true in ConfigMap to enable crash debugging)"
        echo ""

        # Run normally
        ./pos2 $port # > $logfile
        EXIT_CODE=$?

        echo "pos2 exited with code: $EXIT_CODE"

        # If it crashed, re-run under gdb to get a stack trace
        # Exit codes 128+N indicate death by signal N
        # 139 = 128 + 11 (SIGSEGV), 134 = 128 + 6 (SIGABRT), etc.
        if [ $EXIT_CODE -gt 128 ]; then
            SIGNAL=$((EXIT_CODE - 128))
            echo ""
            echo "========================================"
            echo "CRASH DETECTED - $(date)"
            echo "Exit Code: $EXIT_CODE (Signal $SIGNAL)"
            echo "========================================"
            echo ""
            echo "Re-running under gdb to capture stack trace..."
            echo ""

            # Re-run the same command under gdb to reproduce the crash
            timeout 30 gdb -batch \
                -ex "set pagination off" \
                -ex "run $port" \
                -ex "thread apply all bt full" \
                -ex "quit" \
                ./pos2 2>&1

            echo ""
            echo "========================================"
            echo "Note: This was a reproduction run under gdb"
            echo "The actual crash occurred in the previous run"
            echo "========================================"
        fi
    fi

    # Restart, giving old connections a chance to die.
    if [ -f "shutdown.txt" ] ; then
        rm -f "shutdown.txt"
        exit 0
    fi
    sleep 10
done
