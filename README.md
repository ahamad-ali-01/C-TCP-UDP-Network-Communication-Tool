# NetPulse — C++ TCP/UDP Network Communication and Performance Analyzer

NetPulse is a C++17 Linux command-line utility for TCP/UDP echo and ping/pong exchanges, concurrent TCP clients, and application-level round-trip benchmarking. It uses POSIX sockets and has no third-party runtime dependencies.

> **Scope:** educational and diagnostic software, not a hardened production service. Benchmark results describe this application's observed exchanges, not physical link capacity or general network-wide loss. Use loopback for initial testing and do not expose the service publicly without a security review.

## Features
- TCP server with per-client managed worker threads, framed messages, clean SIGINT/SIGTERM shutdown, and multiple simultaneous clients.
- UDP echo server, sequence numbers, response timeout, and estimated loss based on the benchmark's observed responses.
- Versioned binary protocol with a 24-byte header, big-endian fixed-width fields, and a 60,000-byte payload cap.
- Interactive client (`/quit`), RTT measurement, min/average/median/p95/p99 summary, throughput and exchange rate.
- CSV and JSON export, CMake, tests, helper scripts, sanitizer build preset, and GitHub Actions CI.

## Build (Ubuntu 22.04/24.04 or WSL2 Ubuntu)
```bash
sudo apt update
sudo apt install -y build-essential cmake git zip unzip
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```
Or run `./scripts/build.sh` and `./scripts/run_tests.sh`. Optional sanitizer build: `cmake --preset debug-sanitize && cmake --build --preset debug-sanitize` (configure/build preset support depends on installed CMake; alternatively pass `-DNETPULSE_SANITIZERS=ON`).

## Quick start
Open two terminal windows in the project directory.

TCP server:
```bash
./build/netpulse server --protocol tcp --host 127.0.0.1 --port 9000
```
TCP interactive client:
```bash
./build/netpulse client --protocol tcp --host 127.0.0.1 --port 9000
```
Type a message; `/quit` exits.

UDP server (separate terminal):
```bash
./build/netpulse server --protocol udp --host 127.0.0.1 --port 9001
```
UDP interactive client:
```bash
./build/netpulse client --protocol udp --host 127.0.0.1 --port 9001 --timeout-ms 1000
```

## Benchmarks
Keep the matching server running first.
```bash
./build/netpulse benchmark --protocol tcp --host 127.0.0.1 --port 9000 --count 10000 --size 1024 --output results/tcp-local
./build/netpulse benchmark --protocol udp --host 127.0.0.1 --port 9001 --count 10000 --size 512 --timeout-ms 1000 --interval-ms 1 --output results/udp-local
```
`--output results/tcp-local` writes `results/tcp-local.csv` and `results/tcp-local.json`. For consistency, use a path without an extension. Current benchmark supports one client (`--clients 1`); `--duration` is accepted for forward-compatible CLI shape but count controls the number of exchanges. A single UDP benchmark waits for each response before sending the next packet, so it measures request/response behavior rather than maximum-rate UDP saturation.

### Metric definitions
- RTT is the elapsed client-side time from request send to response receive, using `steady_clock`; it is not one-way latency.
- Min/average/median/p95/p99 are computed from successful RTT samples. Percentiles use linear interpolation between sorted samples.
- Throughput is successfully received encoded application bytes divided by wall-clock benchmark duration; MiB/s uses 1,048,576 bytes per MiB. It excludes link-layer overhead and does not represent physical bandwidth.
- UDP estimated loss is `(sent - unique responses received) / sent * 100`. This is the observed response loss for this run, and may include server/client scheduling or timeout effects. UDP does not guarantee delivery, ordering, or duplicate suppression.
- Sent bytes and received bytes are reported separately in exports.

## CLI reference
```text
netpulse server --protocol tcp|udp [--host 127.0.0.1] [--port 9000]
netpulse client --protocol tcp|udp --host 127.0.0.1 --port 9000 [--timeout-ms 1000]
netpulse benchmark --protocol tcp|udp --host 127.0.0.1 --port 9000 [--count 100] [--size 64] [--interval-ms 0] [--timeout-ms 1000] [--clients 1] [--duration 0] [--output PATH]
```
`--host` is a numeric IPv4 address. Server defaults to loopback. Use `--host 0.0.0.0` only to bind all IPv4 interfaces; **0.0.0.0 is not a destination address**. Ports must be 1–65535, payloads 0–60000 bytes, count 1–10,000,000. Help: `./build/netpulse --help`.

## Tests
```bash
ctest --test-dir build --output-on-failure
```
Unit tests cover protocol serialization/validation and statistics. Integration tests start local TCP and UDP server processes, exercise interactive TCP echo, benchmark both protocols and verify CSV/JSON exports, and launch three concurrent TCP clients. The test script uses ports 19450/19451; ensure they are free.

## WSL2 and LAN
See [`docs/WINDOWS_WSL2.md`](docs/WINDOWS_WSL2.md). For initial validation use `127.0.0.1` in both terminals. For remote LAN clients, bind server to `0.0.0.0`, use the server's reachable LAN/WSL address as the client's `--host`, and allow the selected TCP/UDP port through the relevant firewall. WSL2 NAT networking can require Windows port forwarding for inbound connections; behavior differs by WSL version and networking mode.

## Project layout
```text
include/netpulse/   public protocol, network, and statistics interfaces
src/                CLI, TCP/UDP sockets, protocol and metrics implementation
tests/              lightweight unit tests
scripts/            build, test, demo, integration, cleanup helpers
docs/               architecture, wire protocol, metrics, troubleshooting, WSL2
config/             example reference configuration
results/            example output guidance
```

## Limitations and future work
IPv4 only; no TLS/authentication; TCP uses one thread per connected client; benchmark currently uses one client and sequential request/response exchanges; duration-based scheduling and configurable log levels are not implemented. Potential extensions include epoll-based scaling, rate-controlled asynchronous UDP, one-way clock-synchronized measurements, TLS, richer configuration parsing, and stronger load tests.
