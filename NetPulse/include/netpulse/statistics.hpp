#pragma once
#include <string>
#include <vector>
namespace netpulse {
struct Summary { std::size_t sent=0, received=0, success=0, errors=0; double duration_s=0, bytes_sent=0, bytes_received=0; std::vector<double> rtt_ms; };
struct Metrics { double min_ms=0, avg_ms=0, median_ms=0, p95_ms=0, p99_ms=0, bytes_per_sec=0, mib_per_sec=0, messages_per_sec=0; };
Metrics calculate(const Summary& s);
bool export_results(const Summary& s, const Metrics& m, const std::string& path, std::string& error);
}
