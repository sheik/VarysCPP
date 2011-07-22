/* 
 * File:   IRCBotProcessor.hpp
 * Author: enum
 *
 * Created on July 22, 2011, 10:57 AM
 */

#ifndef IRCBOTPROCESSOR_HPP
#define	IRCBOTPROCESSOR_HPP

#include "IRCProcessor.hpp"

class IRCBotProcessor : public IRCProcessor  {
public:
    void populate_handlers(void);
    typedef void (IRCProcessor::* IRCHandler)(IRCMessage msg);
};

#endif	/* IRCBOTPROCESSOR_HPP */

