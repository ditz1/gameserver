#pragma once
#include <boost/asio.hpp>
#include <string>
#include <vector>
#include "player.hpp" // has game protocol

typedef std::shared_ptr<boost::asio::ip::tcp::socket> SocketPtr;

typedef union res_data {
    uint16_t d;
    struct {
        unsigned char b1;
        unsigned char b0;
    } bytes;
} res_data;

class Server {
public:
    Server(const std::string& address, unsigned short port);
    void Start();
    void SendToAll(const std::string& message);
    void SendToClient(const std::string& message, int clientId);
    std::vector<SocketPtr> CurrentClients();
    void ProcessResponse(const char* data, int clientId, int len_response);
    PlayerData DecodePlayerData(const std::string& data, int clientId); 
    inline int ClientCount() { return _clients.size(); }

private:
    void DoAccept();
    void HandleConnection(SocketPtr socket, int clientId);
    boost::asio::io_context _ioc;
    boost::asio::ip::tcp::acceptor _acceptor;
    std::vector<SocketPtr> _clients;    
    int _nextClientId = 0;
};