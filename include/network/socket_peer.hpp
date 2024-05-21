#ifndef NETWORKING_SOCKET_PEER_HPP
#define NETWORKING_SOCKET_PEER_HPP


#include "network/interface.hpp"

#include <string>
#include <vector>


namespace Koi::Network {

class SocketPeer final {
public:
    //


private:
    int _last_error = 0;


public:
    static int get_interfaces(std::vector<Interface>& out_interfaces);


    SocketPeer();
    ~SocketPeer();


private:
    //
};

}


#endif //NETWORKING_SOCKET_PEER_HPP
