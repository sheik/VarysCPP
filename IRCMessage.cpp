/* 
 * File:   IRCMessage.cpp
 * Author: enum
 * 
 * Created on July 17, 2011, 11:32 PM
 */

#include <string.h>

#include "IRCMessage.hpp"

IRCMessage::IRCMessage() {
}

IRCMessage::IRCMessage(std::string raw) {
    this->message_ = raw;
    this->parse_message();
}

IRCMessage::IRCMessage(const char *raw) {
    this->message_ = std::string(raw);
    this->parse_message();
}

/* look up move / copy constructors 
IRCMessage::IRCMessage(const IRCMessage& orig) {
}*/

IRCMessage::~IRCMessage() {
}

std::string IRCMessage::get_message() { return this->message_; }
std::string IRCMessage::get_prefix() { return this->prefix_; }
std::string IRCMessage::get_command() { return this->command_; }
std::deque<std::string> IRCMessage::get_parameters() { return this->parameters_; }

void IRCMessage::parse_message() {
    int pfx_end = -1;
    std::string trailing;
    std::deque<std::string> cmd_param;

    // Try to remove any \r and \n
    boost::algorithm::erase_all(this->message_, "\n");
    boost::algorithm::erase_all(this->message_, "\r");
    
    // obtain prefix
    if(this->message_.find(':') == 0) {
        pfx_end = this->message_.find(' ');
        this->prefix_ = this->message_.substr(1, pfx_end-1);
    }
    
    // obtain trailing
    int trail_start = this->message_.find(" :");
    if(trail_start != std::string::npos)
        trailing = this->message_.substr(trail_start+2);
    else
        trail_start = this->message_.length();
    
    // split the remainder for command + params
    std::string params = this->message_.substr(pfx_end + 1, trail_start - pfx_end - 1);
    boost::split(cmd_param, params, boost::is_any_of(" "));
    
    // obtain command and remove it from params
    this->command_ = cmd_param.front();
    cmd_param.pop_front();
    
    // get the parameters
    this->parameters_ = cmd_param;
    
    if(!trailing.empty())
        this->parameters_.push_back(trailing);
}
