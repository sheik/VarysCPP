/* 
 * File:   IRCClient.hpp
 * Author: enum
 *
 * Created on July 21, 2011, 8:41 PM
 */

#ifndef IRCCLIENT_HPP
#define	IRCCLIENT_HPP

#include <cstdlib>
#include <deque>
#include <string>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/unordered_map.hpp>
#include "IRCMessage.hpp"

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

using boost::asio::ip::tcp;
typedef std::deque<std::string> irc_message_queue;

class IRCClient {
public:
    IRCClient(boost::asio::io_service& io_service,
            tcp::resolver::iterator endpoint_iterator);
    void write(const std::string& msg);
    void close();
    
private:
    typedef void (IRCClient::* IRCHandler)(IRCMessage msg);
    typedef boost::unordered_map<std::string, IRCHandler> map;

    boost::asio::io_service& io_service_;
    tcp::socket socket_;
    IRCMessage _read_msg;
    boost::asio::streambuf _response;
    irc_message_queue write_messages_;
    map irc_handlers_;
    
    void populate_handlers(void);
    void handle_connect(const boost::system::error_code& error,
            tcp::resolver::iterator endpoint_iterator);
    void handle_read(const boost::system::error_code& error);
    void do_write(std::string msg);
    void handle_write(const boost::system::error_code& error);
    void do_close();
    void process_message(IRCMessage msg);
    void ping_handler(IRCMessage msg);
};



#endif	/* IRCCLIENT_HPP */

