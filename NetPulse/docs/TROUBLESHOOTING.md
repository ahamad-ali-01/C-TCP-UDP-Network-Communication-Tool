# Troubleshooting

- `bind: Address already in use`: choose another port or stop the existing process (`ss -ltnup`).
- `connect: Connection refused`: verify the server is running, protocol matches, host/port are correct, and the bind address permits the client.
- Timeout: inspect firewall rules, WSL2 forwarding, server logs, and the configured `--timeout-ms`.
- Invalid IPv4 address: use numeric IPv4 such as `127.0.0.1` or `192.168.1.20`; DNS names are not resolved by this version.
- UDP results show loss: UDP is best-effort; also check host load, timeout, firewall and packet size. Loss percentage here is observed missing responses, not proof of network-wide loss.
- Permission denied: normal ports above 1024 do not require root; do not run the application as root to work around a port/configuration problem.
- Test port conflict: integration tests use TCP 19450 and UDP 19451.
- Build error: verify `g++ --version` and `cmake --version`; install `build-essential cmake`.
