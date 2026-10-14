# Benchmarking

Run a server in one terminal and benchmark from another. Use loopback for a sanity check, then test a controlled LAN. Save output with `--output PATH` (without an extension) to create CSV and JSON files.

RTT is measured client-side using `std::chrono::steady_clock`; it includes application scheduling and server response time. Throughput is successful encoded response bytes divided by complete test wall time. Percentiles are linear-interpolated from sorted RTT samples. UDP loss is calculated from the number of unique valid response sequence numbers received. This sequential request/response loop deliberately prioritizes interpretable RTT/loss behavior over line-rate traffic generation. Do not interpret the result as maximum network capacity or network-wide packet loss.

For meaningful comparisons, repeat tests, document host load, interface and payload size, avoid competing traffic, and compare like-for-like runs. The first test is often affected by warm-up and scheduling noise. No benchmark values are included as measured results in this source archive.
