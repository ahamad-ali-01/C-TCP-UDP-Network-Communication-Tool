#!/usr/bin/env bash
set -euo pipefail
BIN="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)/build/netpulse"
"$BIN" benchmark --protocol tcp --host 127.0.0.1 --port "${PORT:-9000}" --count "${COUNT:-1000}" --size "${SIZE:-1024}" --output "${OUTPUT:-results/tcp-local}"
