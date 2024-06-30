// main.cpp
#include "../inc/server.hpp"
#include <iostream>
#include <thread>

void ServerLogic(Server& server) {
    while (true) {
        // Perform server-side logic here
        // For example, you can send messages to clients periodically
        server.SendToAll("Server heartbeat message");
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

int main() {
    try {
        std::string address = "192.168.1.42";
        unsigned short port = 8080;

        Server server(address, port);
        std::cout << "Death machine spinning up -- " << address << ":" << port << std::endl;

        // Start the server in a separate thread
        std::thread serverThread([&server]() {
            server.Start();
        });

        // Run server-side logic in the main thread
        ServerLogic(server);

        // Wait for the server thread to finish (optional)
        serverThread.join();

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}