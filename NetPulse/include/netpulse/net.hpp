#pragma once
#include <string>
#include <cstdint>
namespace netpulse {
struct Options { std::string host="127.0.0.1", protocol="tcp", output=""; int port=9000, count=100, size=64, duration=0, interval_ms=0, timeout_ms=1000, clients=1; };
int run_server(const Options& o);
int run_client(const Options& o);
int run_benchmark(const Options& o);
}
