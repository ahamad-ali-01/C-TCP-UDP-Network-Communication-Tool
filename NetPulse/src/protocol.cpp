#include "netpulse/protocol.hpp"
#include <chrono>
#include <cstring>
namespace netpulse {
static void put32(std::uint8_t* p,std::uint32_t v){p[0]=static_cast<std::uint8_t>(v>>24);p[1]=static_cast<std::uint8_t>(v>>16);p[2]=static_cast<std::uint8_t>(v>>8);p[3]=static_cast<std::uint8_t>(v);}
static std::uint32_t get32(const std::uint8_t*p){return (std::uint32_t(p[0])<<24)|(std::uint32_t(p[1])<<16)|(std::uint32_t(p[2])<<8)|p[3];}
static void put64(std::uint8_t*p,std::uint64_t v){for(int i=7;i>=0;--i){p[i]=static_cast<std::uint8_t>(v);v>>=8;}}
static std::uint64_t get64(const std::uint8_t*p){std::uint64_t v=0;for(int i=0;i<8;++i)v=(v<<8)|p[i];return v;}
std::uint64_t now_ns(){return static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());}
std::vector<std::uint8_t> encode(const Message&m){if(m.payload.size()>kMaxPayload)return {};std::vector<std::uint8_t>b(kHeaderSize+m.payload.size());b[0]='N';b[1]='P';b[2]='L';b[3]='S';b[4]=1;b[5]=static_cast<std::uint8_t>(m.type);b[6]=b[7]=0;put32(&b[8],m.sequence);put32(&b[12],static_cast<std::uint32_t>(m.payload.size()));put64(&b[16],m.timestamp_ns);if(!m.payload.empty())std::memcpy(b.data()+kHeaderSize,m.payload.data(),m.payload.size());return b;}
bool decode(const std::uint8_t*d,std::size_t n,Message&o,std::string&e){if(n<kHeaderSize){e="message shorter than header";return false;}if(d[0]!='N'||d[1]!='P'||d[2]!='L'||d[3]!='S'){e="invalid magic";return false;}if(d[4]!=1){e="unsupported protocol version";return false;}if(d[5]<1||d[5]>4){e="invalid message type";return false;}auto len=get32(d+12);if(len>kMaxPayload){e="payload exceeds limit";return false;}if(n!=kHeaderSize+len){e="payload length mismatch";return false;}o.type=static_cast<MessageType>(d[5]);o.sequence=get32(d+8);o.timestamp_ns=get64(d+16);o.payload.assign(reinterpret_cast<const char*>(d+kHeaderSize),len);return true;}
}
