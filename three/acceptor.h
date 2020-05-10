//
// (nwlzee AT gmail DOT com)
//

#ifndef __HULUZY_ACCEPTOR_H__
#define __HULUZY_ACCEPTOR_H__

#include "iohandler.h"

namespace huluzy {
class Acceptor : public IOHandler {
 public:
    Acceptor();
    ~Acceptor();
    void Init(char *ipaddr,int port);
    void Listen(int backlog = 5);
    void HandleRead();
    void HandleWrite();
    virtual void want_write() {
        // nothing to do
    }
 private:
    Acceptor(const Acceptor&);
    const Acceptor &operator = (const Aceeptor&);
    
};

}


#endif
