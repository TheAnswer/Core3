#!/usr/bin/env bash
#
# swgemu - Control swgemu server on this machine
# Original Author: Lord Kator <lordkator@swgemu.com>
# Created: Sun Jan  3 13:30:33 UTC 2016

SCRIPTDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
REPODIR="$(dirname "$SCRIPTDIR")"

# always fail script if a cmd fails
set -eo pipefail

RUN_DIR="/app/MMOCoreORB/bin"
cd "$RUN_DIR"

main() {
    local ret=255
    local cmd=$(echo $1 | tr '[:upper:]' '[:lower:]')
    shift

    if get_lock; then
        :
    else
        return 252
    fi

    case $cmd in
        'send' )         server_send         "$@";;
        'start' )        server_start        "$@";;
        'status' )       server_status       "$@";;
        'shutdown' )     server_shutdown     "$@";;
        'stop' )         server_shutdown     "0";;  # Backwards compatibility stop => shutdown 0
        'help' | * ) echo "Usage: $0 [send|start|status|shutdown]"; return 1 ;;
    esac

    ret=$?

    release_lock

    return $ret
}

server_start() {

    if server_status -q; then
        echo "** Already running **"
        return 0
    fi

    if [ ! -x core3 ]; then
        echo "** Server executible not found! **"
        return 2;
    fi

    # Free lock
    release_lock

    # Create named pipe to facilitate commands coming from docker exec
    [ -p /tmp/swgemu-input ] && rm /tmp/swgemu-input
    mkfifo /tmp/swgemu-input

    trap 'graceful_shutdown;' SIGINT SIGTERM
    
    (sleep infinity > /tmp/swgemu-input) &
    (./core3 < /tmp/swgemu-input) &
    local core3pid=$!
    echo "** core3 started (pid: $core3pid) **"
    wait $core3pid
}

server_shutdown() {
    local delay=$1

    if [ -z "${delay}" ]; then
        delay=0
    fi

    if server_status -q; then
        :
    else
        echo "** Server not running: shutting down immediately **"
        return 0
    fi

    

    echo "** send 'shutdown ${delay}' command to server **"
    server_send "shutdown ${delay}"
}

server_send() {
    if server_status -q; then
        :
    else
        echo "** Server not running **"
        return 0
    fi

    echo "$*" > /tmp/swgemu-input
    return 0
}

server_status() {
    local pid=$(pgrep -x core3)
    local verbose=true

    if [ "X$1" = "X-q" ]; then
        verbose=false
    fi

    if [ -z "$pid" ]; then
        if $verbose; then
            echo "** Unable to get server status: Server is not running **"
        fi
        return 1
    fi

    local state=$(ps -p $pid -ho state)

    shopt -s nocasematch

    if [[ "${state}" != "S" && "${state}" != "R" && "${state}" != "D" ]]; then
        echo -e $(tput setab 3;tput setaf 0)"WARNING: Server state is ${state}, it is not running or sleeping, something is wrong!"$(tput sgr0)

        if $verbose; then
            return 2
        fi
    fi

    if $verbose; then
        read uptime <<< $(ps -p $pid -ho etime)

        echo "** Server running on $pid, uptime: $uptime **"
    fi

    return 0
}

get_lock() {
    ## Run LOCK
    export HAVELOCK=false
    local locktmp=$(mktemp /tmp/swgemu.lock-XXXXXX)
    export SWGEMU_LOCKFILE=/tmp/swgemu.lock

    echo "$$ "$(date +%s) > ${locktmp}

    if ln ${locktmp} ${SWGEMU_LOCKFILE} 2> /dev/null; then
        rm -f ${locktmp}
        export HAVELOCK=true
    else
        read pid tm_lock < ${SWGEMU_LOCKFILE}
        tm_now=$(date +%s)

        let "tm_delta=${tm_now} - ${tm_lock}"

        if kill -0 $pid; then
            echo -e ">> PID $pid HAS HAD LOCK FOR ${tm_delta} SECOND(S) RUNNING:\n\n\t"$(ps -p $pid -ho cmd)"\n\nAborting, can't continue until other command is done."
            return 1
        else
            echo ">> Stealing lock from PID $pid which has gone away, locked ${tm_delta} second(s) ago"
            if ln -f ${locktmp} ${SWGEMU_LOCKFILE}; then
                rm -f ${locktmp}
                read pid tm_lock < ${SWGEMU_LOCKFILE}
                if [ "$pid" -eq "$$" ]; then
                    echo ">> STOLE LOCK, PROCEEDING"
                else
                    echo ">> Can't steal lock, somone got in before us!? pid=${pid}"
                    return 2
                fi
            else
                rm -f ${locktmp}
                echo ">> Failed to steal lock, **ABORT**"
                return 1
            fi
        fi
    fi

    export HAVELOCK=true
    trap 'release_lock;' 0
}

release_lock() {
    if $HAVELOCK; then
        rm -f ${SWGEMU_LOCKFILE}
    fi
}

graceful_shutdown() {
    echo "** starting graceful shutdown proceedure **"
    if server_status -q; then
        :
    else
        echo "** Server not running, exiting immediately **"
        exit 0
    fi

    echo "** saving... **"
    swgemu send save

    echo "** issuing shutdown command **"
    swgemu shutdown 0

    local core3pid=$(pgrep -x core3)
    if [ -z "$core3pid" ]; then
        echo "** no core3 pid found: shutting down immediately **"
        return 1
    fi

    echo "** waiting for shutdown to complete **"
    wait $core3pid
}

# Cron and API calls won't have TERM set
if [ -z "$TERM" ]; then
    TERM=dumb
fi

export TERM

main "$@"

ret=$?
exit $ret