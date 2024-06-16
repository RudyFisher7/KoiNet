//
// Created by rfish on 6/15/2024.
//

#ifndef KOI_NETWORK_TCP_SERVER_HPP
#define KOI_NETWORK_TCP_SERVER_HPP


#include "network/peer.hpp"

#include "network/typedefs.hpp"

#include <set>


namespace Koi { namespace Network {

class TcpServer : Peer {
public:
    //

protected:
    std::set<Socket> _remote_handles;

public:
    explicit TcpServer(int select_flags, int connection_queue_size);

    bool is_ready_to_send() const override;
    bool is_data_ready_to_receive() const override;
    bool is_exception_active() const override;
    bool is_new_connection_ready() const;

    Socket accept_new_connection(int select_flags);
};

}
}


#endif //KOI_NETWORK_TCP_SERVER_HPP
