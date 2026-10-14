#!/usr/bin/env bash
set -euo pipefail
BIN="${1:-./build/netpulse}"
TMP="$(mktemp -d)"
TCP_PID=""; UDP_PID=""
cleanup(){ [[ -n "$TCP_PID" ]] && kill -TERM "$TCP_PID" 2>/dev/null || true; [[ -n "$UDP_PID" ]] && kill -TERM "$UDP_PID" 2>/dev/null || true; [[ -n "$TCP_PID" ]] && wait "$TCP_PID" 2>/dev/null || true; [[ -n "$UDP_PID" ]] && wait "$UDP_PID" 2>/dev/null || true; rm -rf "$TMP"; }
trap cleanup EXIT
"$BIN" server --protocol tcp --host 127.0.0.1 --port 19450 >"$TMP/tcp.log" 2>&1 & TCP_PID=$!
sleep .25
printf 'integration tcp\n/quit\n' | "$BIN" client --protocol tcp --host 127.0.0.1 --port 19450 --timeout-ms 1000 >"$TMP/tcp-client.log"
grep -q 'integration tcp' "$TMP/tcp-client.log"
"$BIN" benchmark --protocol tcp --host 127.0.0.1 --port 19450 --count 10 --size 32 --output "$TMP/tcp-bench" >"$TMP/tcp-bench.log"
grep -q 'Successful: 10' "$TMP/tcp-bench.log"
test -s "$TMP/tcp-bench.csv"; test -s "$TMP/tcp-bench.json"
"$BIN" server --protocol udp --host 127.0.0.1 --port 19451 >"$TMP/udp.log" 2>&1 & UDP_PID=$!
sleep .25
"$BIN" benchmark --protocol udp --host 127.0.0.1 --port 19451 --count 10 --size 32 --timeout-ms 500 --output "$TMP/udp-bench" >"$TMP/udp-bench.log"
grep -q 'Sent: 10' "$TMP/udp-bench.log"
test -s "$TMP/udp-bench.csv"; test -s "$TMP/udp-bench.json"
# Three concurrent TCP clients each exercise an independent server worker.
PIDS=()
for i in 1 2 3; do (printf 'parallel-%s\n/quit\n' "$i" | "$BIN" client --protocol tcp --host 127.0.0.1 --port 19450 >"$TMP/client-$i.log") & PIDS+=("$!"); done
for pid in "${PIDS[@]}"; do wait "$pid"; done
grep -q parallel-1 "$TMP/client-1.log"; grep -q parallel-2 "$TMP/client-2.log"; grep -q parallel-3 "$TMP/client-3.log"
echo 'TCP/UDP local integration, benchmark exports, and concurrent TCP client checks passed'
