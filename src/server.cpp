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

PlayerData Server::DecodePlayerData(const std::string& data, int clientId) {
    // Extract the data
    PlayerData player_data;
    uint8_t player_id = data[0];
    uint8_t state = data[1];
    uint8_t mv_dir = data[2];
    uint8_t atk_dir = data[3];
    float pos_x = *reinterpret_cast<const float*>(&data[4]);
    float pos_y = *reinterpret_cast<const float*>(&data[8]);
    std::string name(data.begin() + 12, data.end());

    // Process the data as needed
    std::cout << "Received from client " << clientId << ": "
              << "Player ID: " << static_cast<int>(player_id) << ", "
              << "State: " << static_cast<int>(state) << ", "
              << "Move Dir: " << static_cast<int>(mv_dir) << ", "
              << "Attack Dir: " << static_cast<int>(atk_dir) << ", "
              << "Pos X: " << pos_x << ", "
              << "Pos Y: " << pos_y << ", "
              << "Name: " << name << std::endl;

    return player_data;
}

void Server::ProcessResponse(const char* data, int clientId, int len) {
    for (int i = 0; i < len; i++) {
        printf("%x | ", data[i]);
    }
    printf("\n");    

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
        boost::asio::streambuf buffer;
        while (true) {
            boost::system::error_code error;
        
            // Read data until the delimiter byte (0xa) is found
            size_t len = boost::asio::read_until(*socket, buffer, '\n', error);
        
            // Copy buffer to data
            std::string data(boost::asio::buffer_cast<const char*>(buffer.data()), len);
            std::cout << "Received " << len << " bytes from client " << clientId << std::endl;
            
            buffer.consume(len);
        
            if (error) {
                if (error == boost::asio::error::eof) {
                    std::cout << "Client " << clientId << " disconnected." << std::endl;
                } else {
                    throw boost::system::system_error(error);
                }
                break;
            }
        
            // Process the response
            ProcessResponse(data.data(), clientId, len);
        }
    } catch (std::exception const& e) {
        std::cerr << "Error handling client " << clientId << ": " << e.what() << std::endl;
    }

    std::cout << "Connection handler for client " << clientId << " exiting." << std::endl;
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