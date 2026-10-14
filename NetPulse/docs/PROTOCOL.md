# NetPulse wire protocol v1

All multi-byte fields are unsigned big-endian integers. Do not send native C++ structs directly.

| Offset | Bytes | Field |
|---:|---:|---|
| 0 | 4 | ASCII magic `NPLS` |
| 4 | 1 | Version, currently `1` |
| 5 | 1 | Type: 1 Echo, 2 Ping, 3 Pong, 4 Error |
| 6 | 2 | Reserved, set to zero |
| 8 | 4 | Sequence number |
| 12 | 4 | Payload length |
| 16 | 8 | Sender's monotonic timestamp in nanoseconds (informational; clocks are not comparable across hosts) |
| 24 | variable | Payload bytes |

Header length is exactly 24 bytes; maximum payload is 60,000 bytes. TCP transports a header and its payload as a stream; receivers must read exactly the header, validate the declared length, then read exactly that payload. UDP requires exactly one complete header+payload in each datagram. Type 2 Ping receives type 3 Pong; Echo receives Echo. Malformed input is dropped by the UDP server and closes the TCP client session when decoding fails.
