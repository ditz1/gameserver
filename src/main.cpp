#include "../inc/server.hpp"
#include <iostream>

int main() {
    try {
        std::string address = "192.168.1.42";
        unsigned short port = 8080;

        Server server(address, port);
        std::cout << "death machine spinning up -- " << address << ":" << port << std::endl;

        // Start the server
        server.Start();
        
        


        

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}