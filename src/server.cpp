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

}

void Server::ProcessResponse(const std::string& data, int clientId) {
    int data_length = data.length();
    printf("data length: %d\n", data_length);
    printf("Received data from client %x: ", clientId);
    for (int i = 0; i < data_length; i++) {
        printf("%02x ", static_cast<unsigned char>(data[i]));
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
            // Read data until a newline character is encountered
            boost::asio::read_until(*socket, buffer, '\n');

            // Extract the data from the buffer
            std::string data = boost::asio::buffer_cast<const char*>(buffer.data());

            // Remove the data from the buffer
            buffer.consume(buffer.size());

            // Process the response
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