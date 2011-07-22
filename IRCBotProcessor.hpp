/* 
 * File:   IRCBotProcessor.hpp
 * Author: enum
 *
 * Created on July 22, 2011, 10:57 AM
 */

#ifndef IRCBOTPROCESSOR_HPP
#define	IRCBOTPROCESSOR_HPP

#include <boost/unordered_map.hpp>
#include <string>
#include "IRCProcessor.hpp"
#include "IRCClient.hpp"
class IRCClient;

class IRCBotProcessor  {
public:
    void populate_handlers(void);
    void ping_handler(IRCMessage& msg);
    typedef void (IRCBotProcessor::* IRCHandler)(IRCClient& irc, IRCMessage& msg);
    boost::unordered_map<std::string, IRCHandler> irc_handlers_;
};

#endif	/* IRCBOTPROCESSOR_HPP */

