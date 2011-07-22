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
#include "IRCMessage.hpp"

using boost::asio::ip::tcp;
typedef std::deque<std::string> irc_message_queue;

class IRCClient {
public:

    IRCClient(boost::asio::io_service& io_service,
            tcp::resolver::iterator endpoint_iterator)
    : io_service_(io_service),
    socket_(io_service) {
        tcp::endpoint endpoint = *endpoint_iterator;
        socket_.async_connect(endpoint,
                boost::bind(&IRCClient::handle_connect, this,
                boost::asio::placeholders::error, ++endpoint_iterator));
    }
    
    void write(const std::string& msg) {
        io_service_.post(boost::bind(&IRCClient::do_write, this, msg));
    }

    void close() {
        io_service_.post(boost::bind(&IRCClient::do_close, this));
    }

private:

    void handle_connect(const boost::system::error_code& error,
            tcp::resolver::iterator endpoint_iterator) {
        if (!error) {
            do_write("USER test test test :test\r\n");
            do_write("NICK test\r\n");
            boost::asio::async_read_until(socket_, _response, "\r\n",
                    boost::bind(&IRCClient::handle_read, this,
                    boost::asio::placeholders::error));
        } else if (endpoint_iterator != tcp::resolver::iterator()) {
            socket_.close();
            tcp::endpoint endpoint = *endpoint_iterator;
            socket_.async_connect(endpoint,
                    boost::bind(&IRCClient::handle_connect, this,
                    boost::asio::placeholders::error, ++endpoint_iterator));
        }
    }

    void handle_read(const boost::system::error_code& error) {
        std::istream recv_stream(&_response);
        std::string line;
        if (!error) {
            while (std::getline(recv_stream, line)) {
                this->process_message(IRCMessage(line));
            }
            boost::asio::async_read_until(socket_, _response, "\r\n",
                    boost::bind(&IRCClient::handle_read, this,
                    boost::asio::placeholders::error));
        } else {
            do_close();
        }
    }

    void do_write(std::string msg) {
        bool write_in_progress = !write_messages_.empty();
        this->write_messages_.push_back(msg);

        if (!write_in_progress) {
            boost::asio::async_write(socket_,
                    boost::asio::buffer(write_messages_.front(),
                    write_messages_.front().length()),
                    boost::bind(&IRCClient::handle_write, this,
                    boost::asio::placeholders::error));
        }
    }

    void handle_write(const boost::system::error_code& error) {
        if (!error) {
            write_messages_.pop_front();
            if (!write_messages_.empty()) {
                boost::asio::async_write(socket_,
                        boost::asio::buffer(write_messages_.front(),
                        write_messages_.front().length()),
                        boost::bind(&IRCClient::handle_write, this,
                        boost::asio::placeholders::error));
            }
        } else {
            do_close();
        }
    }

    void do_close() {
        socket_.close();
    }

private:
    boost::asio::io_service& io_service_;
    tcp::socket socket_;
    IRCMessage _read_msg;

    irc_message_queue write_messages_;
    boost::asio::streambuf _response;
    
    void process_message(IRCMessage msg) {
        std::cout << msg.get_message() << std::endl;
        std::cout << msg.get_command() << std::endl;
    }
};


#endif	/* IRCCLIENT_HPP */

