#!/bin/bash

# Set the port number.
KBK_PORT=8989
KBK_LOG_INDEX=0

# Enable core dumps
ulimit -c unlimited
echo "Core dumps enabled - ulimit -c: $(ulimit -c)"
echo "Core pattern: $(cat /proc/sys/kernel/core_pattern 2>/dev/null || echo 'unable to read')"
echo "Current directory: $(pwd)"

# Note: MySQL connection check is now handled by the MUD server itself
# The server will exit cleanly with a proper error message if it can't connect
echo "Starting KBK MUD server..."

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
        elif grep -q "exited with code 1" /tmp/gdb_output.log; then
            EXIT_CODE=1
            echo ""
            echo "========================================"
            echo "FATAL STARTUP ERROR - $(date)"
            echo "Check logs above for details"
            echo "Container will exit - fix the issue and restart"
            echo "========================================"
            rm -f /tmp/gdb_output.log
            exit 1
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

        # Exit code 1 = fatal startup error (e.g., MySQL connection failure)
        # Don't restart in this case - let the container exit so it can be debugged
        if [ $EXIT_CODE -eq 1 ]; then
            echo ""
            echo "========================================"
            echo "FATAL STARTUP ERROR - $(date)"
            echo "Server exited with code 1"
            echo "Check logs above for details"
            echo "Container will exit - fix the issue and restart"
            echo "========================================"
            exit 1
        fi

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
