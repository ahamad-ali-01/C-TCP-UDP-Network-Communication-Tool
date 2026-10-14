#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
"$ROOT/scripts/build.sh"
"$ROOT/build/netpulse" server --protocol tcp --host 127.0.0.1 --port 9000
