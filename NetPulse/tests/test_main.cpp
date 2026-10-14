#include "netpulse/protocol.hpp"
#include "netpulse/statistics.hpp"
#include <iostream>
#include <cmath>
#include <vector>
using namespace netpulse;
static int failures=0;
#define CHECK(x) do{if(!(x)){std::cerr<<"FAIL "<<__FILE__<<":"<<__LINE__<<" "<<#x<<"\n";++failures;}}while(0)
int main(){Message in{MessageType::Ping,42,123456,"hello world"},out;auto b=encode(in);std::string err;CHECK(decode(b.data(),b.size(),out,err));CHECK(out.sequence==42);CHECK(out.payload=="hello world");CHECK(out.type==MessageType::Ping);CHECK(!decode(b.data(),4,out,err));auto bad=b;bad[0]='X';CHECK(!decode(bad.data(),bad.size(),out,err));bad=b;bad[12]=0;bad[13]=0;bad[14]=0;bad[15]=99;CHECK(!decode(bad.data(),bad.size(),out,err));Message huge{MessageType::Echo,0,0,std::string(kMaxPayload+1,'x')};CHECK(encode(huge).empty());Summary s;s.duration_s=2;s.sent=4;s.received=3;s.success=3;s.bytes_received=2048;s.rtt_ms={4,1,3,2};auto m=calculate(s);CHECK(std::abs(m.min_ms-1)<1e-9);CHECK(std::abs(m.avg_ms-2.5)<1e-9);CHECK(std::abs(m.median_ms-2.5)<1e-9);CHECK(std::abs(m.bytes_per_sec-1024)<1e-9);CHECK(std::abs(m.messages_per_sec-1.5)<1e-9);if(failures){std::cerr<<failures<<" test(s) failed\n";return 1;}std::cout<<"All protocol/statistics tests passed\n";return 0;}
