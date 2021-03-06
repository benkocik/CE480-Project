// Author : Allison Hurley & Ben Kocik
// Purpose : Dice roller client
//
//
//
//  Citation : Based off of sample code found at https://www.binarytides.com/winsock-socket-programming-tutorial/

#include <iostream>
#include <regex>
#include "portable_socket.h"
#include "simple_args.h"


int main(int argc, const char** argv)
try {
    simple_args args(argc, argv);
    //default value 60481 otherwise inputted port
    in_port_t port = stoi(args("p", "60481"));
    //default value 127.0.0.1 unless inputted address
    std::string ipaddr = args("a", "127.0.0.1");

    //connect to server
    client_socket client(ipaddr, port);
    bool running = true;
    std::string cmd;
    std::cout << "Connected to address: " << ipaddr << " port: " << port << "\n";

    std::regex cmd_pattern("\\s*(\\d+)\\s*[dD]\\s*(\\d+)\\s*");

    while (running) {
        std::cout << "Enter the number of dice to roll along with the number of sides (ex: 3 d 6) or Q to quit: ";
        std::getline(std::cin, cmd);
        std::smatch match;

        if(cmd == "Q" || cmd == "q"){
            running = false; 
        } else if (std::regex_match(cmd, match, cmd_pattern)) {
            int die_count = std::stoi(match[1]);
            for(int i = 1; i <= die_count; i++){
                // sends string to server
                client.send(match[2]);
                // gets string back from server and prints
                std::cout << "Die " << i << " rolls " << client.recv() << "\n";
            }
    
        }else {
            std::cout << "Unknown command: " << cmd <<  "\n";
        }
    }
    // ends program if general socket error or connection error 
} catch(socket_error& err) {
    std::cerr << "Err: " << err.what() << "\n";
} catch(std::exception&) {
    std::cerr << "Connection to server closed\n";
}

