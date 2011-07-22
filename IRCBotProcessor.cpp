/* 
 * File:   IRCBotProcessor.cpp
 * Author: enum
 * 
 * Created on July 22, 2011, 10:57 AM
 */

#include "IRCBotProcessor.hpp"

void IRCBotProcessor::populate_handlers(void) {
    this->irc_handlers_["PING"] = &IRCBotProcessor::ping_handler;
}

void IRCBotProcessor::ping_handler(IRCClient& irc, IRCMessage& msg) {
    std::cout << "PONG " + boost::join(msg.get_parameters(), " ") + "\r\n";
    irc.write("PONG " + boost::join(msg.get_parameters(), " ") + "\r\n");
}
