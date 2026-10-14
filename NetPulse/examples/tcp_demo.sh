#!/usr/bin/env bash
set -euo pipefail
./build/netpulse server --protocol tcp --host 127.0.0.1 --port 9000
