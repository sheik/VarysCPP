/* 
 * File:   IRCMessage.hpp
 * Author: enum
 *
 * Created on July 17, 2011, 11:32 PM
 */

#ifndef IRCMESSAGE_HPP
#define	IRCMESSAGE_HPP

#include <boost/array.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include <deque>

#define DEBUG 1

class IRCMessage {
public:
    IRCMessage();
    IRCMessage(std::string raw);
    IRCMessage(const char *raw);
    virtual ~IRCMessage();
    std::string get_message();
    std::string get_prefix();
    std::string get_command();
    std::deque<std::string> get_parameters();
private:
    std::string message_;
    std::string prefix_;
    std::string command_;
    std::deque<std::string> parameters_;
    
    void parse_message();
};

#endif	/* IRCMESSAGE_HPP */

