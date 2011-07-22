/* 
 * File:   IRCClient.cpp
 * Author: enum
 * 
 * Created on July 21, 2011, 8:41 PM
 */

#include "IRCClient.hpp"

IRCClient::IRCClient(boost::asio::io_service& io_service,
        tcp::resolver::iterator endpoint_iterator)
: io_service_(io_service),
socket_(io_service) {
    this->processor = IRCBotProcessor();
    this->processor.populate_handlers();
    tcp::endpoint endpoint = *endpoint_iterator;
    socket_.async_connect(endpoint,
            boost::bind(&IRCClient::handle_connect, this,
            boost::asio::placeholders::error, ++endpoint_iterator));
}

void IRCClient::write(const std::string& msg) {
    io_service_.post(boost::bind(&IRCClient::do_write, this, msg));
}

void IRCClient::close() {
    io_service_.post(boost::bind(&IRCClient::do_close, this));
}

void IRCClient::handle_connect(const boost::system::error_code& error,
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

void IRCClient::handle_read(const boost::system::error_code& error) {
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

void IRCClient::do_write(std::string msg) {
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

void IRCClient::handle_write(const boost::system::error_code& error) {
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

void IRCClient::do_close() {
    socket_.close();
}

void IRCClient::populate_handlers(void) {
    this->irc_handlers_["PING"] = &IRCClient::ping_handler;
}

void IRCClient::process_message(IRCMessage msg) {
    map::const_iterator iter = this->irc_handlers_.find(msg.get_command());
    std::cout << msg.get_message() << std::endl;
    std::cout << msg.get_command() << std::endl;
    if(this->irc_handlers_.find(msg.get_command()) != this->irc_handlers_.end()) {
        IRCHandler h = this->irc_handlers_[msg.get_command()];
        (this->*h)(msg);
    } else {
        std::cout << "Unknown command - \"" << msg.get_command() << "\"" <<  std::endl;
    }
}

void IRCClient::ping_handler(IRCMessage msg) {
    std::cout << "PONG " + boost::join(msg.get_parameters(), " ") + "\r\n";
    this->do_write("PONG " + boost::join(msg.get_parameters(), " ") + "\r\n");
}
