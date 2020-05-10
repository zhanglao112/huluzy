//
// (nwlzee AT gmail DOT com)
//

#ifndef __HULUZY_PACKETPROCESS_H__
#define __HULUZY_PACKETPROCESS_H__

namespace huluzy {
class PacketProcess {
 public:
    virtual HandlePacket(Packet*, Connection*) = 0;
};

}

#endif 
