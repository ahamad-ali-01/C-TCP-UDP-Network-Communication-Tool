#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <cstddef>
namespace netpulse {
constexpr std::size_t kHeaderSize=24, kMaxPayload=60000;
enum class MessageType: std::uint8_t { Echo=1, Ping=2, Pong=3, Error=4 };
struct Message { MessageType type{MessageType::Echo}; std::uint32_t sequence{0}; std::uint64_t timestamp_ns{0}; std::string payload; };
std::vector<std::uint8_t> encode(const Message& message);
bool decode(const std::uint8_t* data, std::size_t size, Message& out, std::string& error);
std::uint64_t now_ns();
}
