//
// Created by rfish on 6/17/2024.
//

#ifndef KOI_NETWORK_LOG_HPP
#define KOI_NETWORK_LOG_HPP

#define ENABLE_KOI_NET_LOG 1

#ifdef ENABLE_KOI_NET_LOG

#include <iostream>
#include <string>


#define KOI_NET_LOG(message) std::cout << "KoiNet Log:\n\t" << message << "\n\t\t\t"\
    << __FILE__ << ":" << __LINE__ << std::endl

#else

#define KOI_NET_LOG(message)

#endif

#endif //KOI_NETWORK_LOG_HPP
