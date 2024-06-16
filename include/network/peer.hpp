//
// Created by rfish on 6/15/2024.
//

#ifndef KOI_NETWORK_PEER_HPP
#define KOI_NETWORK_PEER_HPP


#include "network/typedefs.hpp"
#include "network/enums.hpp"


namespace Koi { namespace Network {

class Peer {
public:
    //

protected:
    Socket _handle = INVALID_SOCKET;

public:
    explicit Peer(int select_flags);

    virtual ~Peer() = default;
    virtual bool is_ready_to_send() const = 0;
    virtual bool is_data_ready_to_receive() const = 0;
    virtual bool is_exception_active() const = 0;
};

}
}


#endif //KOI_NETWORK_PEER_HPP
