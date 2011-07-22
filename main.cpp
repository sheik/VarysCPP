#include <cstdlib>
#include <deque>
#include <string>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "IRCMessage.hpp"
#include "IRCClient.hpp"


int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: irc_client <host> <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(argv[1], argv[2]);
        tcp::resolver::iterator iterator = resolver.resolve(query);

        IRCClient c(io_service, iterator);

        io_service.run();
        c.close();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
