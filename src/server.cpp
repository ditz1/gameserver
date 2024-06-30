#include "../inc/server.hpp"
#include <iostream>
#include <memory>
#include <thread>


Server::Server(const std::string& address, unsigned short port)
    : _ioc(1), _acceptor(_ioc, {boost::asio::ip::make_address(address), port}) {
    DoAccept();
}

void Server::Start() {
    _ioc.run();
}

std::vector<SocketPtr> Server::CurrentClients() {
    return _clients;
}

void Server::ProcessResponse(unsigned char data[2], int clientId) {
    res_data res;
    res.bytes.b1 = data[0];
    res.bytes.b0 = data[1];
    
    if (res.bytes.b0 == 0x61 && res.bytes.b0 == 0x62) {
        std::string response = "Client " + std::to_string(clientId) + " sent ab!";
    } else if (res.bytes.b0 == 0xd && res.bytes.b1 == 0xa){
        std::string response = "Client " + std::to_string(clientId) + " send end of response ('0xd > 0xa')";
    } else {
        std::string response = "Client " + std::to_string(clientId) + " sent: ";
        printf("0x%x 0x%x\n", res.bytes.b0, res.bytes.b1);
        std::cout << response << std::endl;
    }
}

// server.cpp
void Server::DoAccept() {
    _acceptor.async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
        if (!ec) {
            int clientId = _nextClientId++;
            auto clientSocket = std::make_shared<boost::asio::ip::tcp::socket>(std::move(socket));
            _clients.push_back(clientSocket);
            std::thread([this, clientId, clientSocket]() {
                // Send a welcome message to the newly connected client
                SendToClient("You have connected!", clientId);
                HandleConnection(clientSocket, clientId);
            }).detach();
            printf("New client connected: %d\n", clientId);
        }
        DoAccept();
    });
}

// HANDLE CONNECTION WILL NOT SEND ANYTHING BACK TO THE CLIENT
void Server::HandleConnection(SocketPtr socket, int clientId) {
    try {
        while (true) {
            unsigned char data[2]; // data we want
            boost::asio::read(*socket, boost::asio::buffer(data, 2));
            ProcessResponse(data, clientId);
        }
    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void Server::SendToAll(const std::string& message) {
    for (auto& client : _clients) {
        boost::asio::write(*client, boost::asio::buffer(message + "\n"));
    }
}

void Server::SendToClient(const std::string& message, int clientId) {
    if (clientId >= 0 && clientId < static_cast<int>(_clients.size())) {
        try {
            boost::asio::write(*_clients[clientId], boost::asio::buffer(message + "\n"));
        } catch (std::exception const& e) {
            std::cerr << "Error sending message to client " << clientId << ": " << e.what() << std::endl;
            // Remove the disconnected client from the _clients vector
            _clients.erase(_clients.begin() + clientId);
        }
    }
}