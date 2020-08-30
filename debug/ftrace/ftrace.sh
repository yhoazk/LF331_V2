#!/usr/bin/sh

set -eux 

trace_path="/sys/kernel/debug/tracing/"

# stop tracing
echo 0 > "${trace_path}/tracing_on"
# clear previous traces
echo > "${trace_path}/trace"
# Disable tracing functions. show only system call events
echo nop > "${trace_path}/current_tracer"
# start tracing
echo 1 > "${trace_path}/tracing_on"
# enable all events
echo *:* > "${trace_path}/set_event"
# execute given binary
exec $1 &
echo $! > "${trace_path}/set_event_pid"
echo 1 > "${trace_path}/events/enable"
sleep 3
cat "${trace_path}/trace" > "trace.trc"
# stop tracing
echo 0 > "${trace_path}/tracing_on"