/* 
 * File:   IRCProcessor.hpp
 * Author: enum
 *
 * Created on July 22, 2011, 10:39 AM
 */

#ifndef IRCPROCESSOR_HPP
#define	IRCPROCESSOR_HPP

#include <boost/unordered_map.hpp>
#include "IRCMessage.hpp"

class IRCProcessor {
public:
    virtual ~IRCProcessor() {};
    virtual void populate_handlers(void);
    typedef void (IRCProcessor::* IRCHandler)(IRCMessage msg);
    boost::unordered_map<std::string, IRCHandler> irc_handlers_;
};

#endif	/* IRCPROCESSOR_HPP */

