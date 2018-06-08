#define SDL_MAIN_HANDLED

#ifdef __linux
#include <SDL2/SDL_net.h>
#endif

#ifdef _WIN32
    #include <SDL_net.h>
#endif

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include<ctime>

int main() {

    //Get the IP of the server
    std::string line;
    std::ifstream IPFile;
    int offset;
    char* search0 = (char*)"IPv4 Address. . . . . . . . . . . :";

    system("ipconfig > ip.txt");

    IPFile.open ("ip.txt");
    if(IPFile.is_open()){
       while(!IPFile.eof()) {
           std::getline(IPFile,line);
           if((offset = line.find(search0, 0)) != std::string::npos) {
               line.erase(0,39);
               std::cout << "[SERVER] Server IP address is: " << line << '\n';
               IPFile.close();
           }
       }
    }

    SDLNet_Init();

    SDLNet_SocketSet socketSet;
    std::vector<TCPsocket> socketList;
    std::vector<bool> socketFreeList;
    std::vector<std::string> addresses;
    socketList.resize(16);
    socketFreeList.resize(16);
    int client_count = 0;
    int max_clients = 15;

    for(auto& i : socketList) {i = nullptr;}
    for(int i = 0; i < socketFreeList.size(); i++){
        socketFreeList[i] = true;
    }

    char* msg = (char*)"Connected!";
    int len = 37;

    char* msg2 = (char*)"Server_Full!";
    int len2 = 13;

    char data[14];

    IPaddress ipAddress;
	TCPsocket tcpSocket;
    TCPsocket client;
    int port = 52239;

    int success = SDLNet_ResolveHost(&ipAddress, nullptr, port);

	if(success == -1) {
		std::cout << "[SERVER] Failed to open port: " << port << '\n';
	}else{
        std::cout << "[SERVER] Host resolved" << '\n';
    }

    std::cout << "[SERVER] Opening connection on port: " << port << '\n';

	tcpSocket = SDLNet_TCP_Open(&ipAddress);

	if(tcpSocket == nullptr) {
		std::cout << "[SERVER] Failed to open port: " << port <<" \n\tError: " << SDLNet_GetError() << '\n';
	}else{
        std::cout << "[SERVER] Success! Port opened for listening!" << '\n';
    }

    socketSet = SDLNet_AllocSocketSet(10);

    SDLNet_TCP_AddSocket(socketSet, tcpSocket);

    std::cout << "[SERVER] Waiting for clients..." << '\n';

    std::string charList = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    srand(time(0));

    while(true){
        //Check for new connections.....

        int numActiveSockets = SDLNet_CheckSockets(socketSet, 20);
        int serverSocketActivity = SDLNet_SocketReady(tcpSocket);

        if (serverSocketActivity != 0) {
    		if (client_count < max_clients) {
    			int freeSpot = -99;
    			for (unsigned int loop = 0; loop < max_clients; loop++) {
    				if (socketFreeList[loop] == true) {
    					socketFreeList[loop] = false;
    					freeSpot = loop;
    					break;
    				}
    			}
    			socketList[freeSpot] = SDLNet_TCP_Accept(tcpSocket);

    			SDLNet_TCP_AddSocket(socketSet, socketList[freeSpot]);

                std::cout << "[SERVER] Client accepted!" << '\n';

    			client_count++;

                std::string address = "";


                addressCreation:
                for(int i = 0; i < 3; i++) {address += charList [rand() % charList.size()];}

                bool exists = false;
                for(int i = 0; i < addresses.size(); i++) {
                    if(addresses[i] == address){
                        exists = true;
                        break;
                    }
                }

                if(exists) {
                    goto addressCreation;
                }

                addresses.push_back(address);

                std::cout << "[SERVER] New client address is: " << (int)address[0] << "." << (int)address[1] << "." << (int)address[2] << '\n';

    			SDLNet_TCP_Send(socketList[freeSpot], address.c_str(), 4);

    		}else{
    			std::cout << "[SERVER] Client rejected, server full!" << '\n';

    			TCPsocket tempSock = SDLNet_TCP_Accept(tcpSocket);

    			SDLNet_TCP_Send(tempSock, msg2, len2);

    			SDLNet_TCP_Close(tempSock);
    		}
        }

        //Respond to clients...

    	for (unsigned int clientNumber = 0; clientNumber < max_clients; clientNumber++) {

    		int clientSocketActivity = SDLNet_SocketReady(socketList[clientNumber]);

    		if (clientSocketActivity != 0) {

    			int receivedByteCount = SDLNet_TCP_Recv(socketList[clientNumber], data, 13);

    			if (receivedByteCount <= 0) {
    				std::cout << "[SERVER] Client #" << clientNumber << " disconnected." << '\n';

    				SDLNet_TCP_DelSocket(socketSet, socketList[clientNumber]);
    				SDLNet_TCP_Close(socketList[clientNumber]);
    				socketList[clientNumber] = NULL;

    				socketFreeList[clientNumber] = true;
                    addresses.erase(addresses.begin() + clientNumber);

    				client_count--;

    				std::cout << "[SERVER] Server is now connected to: " << client_count << " client(s)." << '\n';
    			}else{
                    if(data[0] == '0'){
                        std::cout << "[SERVER] Message recieved from client #" << clientNumber << ", address: " << (int)data[1] << "." << (int)data[2] << "." << (int)data[3] <<
                                                                                                       ", to: " << (int)data[4] << "." << (int)data[5] << "." << (int)data[6] <<
                                                                                                     ", data: " << data[7] << data[8] << data[9] << data[10] << data[11] <<'\n';

                        std::string address2;
                        address2.push_back(data[4]);
                        address2.push_back(data[5]);
                        address2.push_back(data[6]);
                        bool exists = false;
                        int id = 0;
                        for(int i = 0; i < addresses.size(); i++) {if(addresses[i] == address2) {exists = true; id = i;}}
                        if(exists){
                            std::string sender;
                            sender.push_back(data[1]);
                            sender.push_back(data[2]);
                            sender.push_back(data[3]);
                            std::string dataS;
                            dataS.push_back(data[7]);
                            dataS.push_back(data[8]);
                            dataS.push_back(data[9]);
                            dataS.push_back(data[10]);
                            dataS.push_back(data[11]);
                            std::string finalM = sender+dataS;
                            SDLNet_TCP_Send(socketList[id], finalM.c_str(), 9);
                            std::cout << "[SERVER] Data forwarded from: " << (int)sender[0] << "." << (int)sender[1] << "." << (int)sender[2] << ", to: " << (int)address2[0] << "." << (int)address2[1] << "." << (int)address2[2] << ", data: " << dataS << '\n';
                        }else{
                            std::cout << "[SERVER] Unknown target address: " << (int)address2[0] << "." << (int)address2[1] << "." << (int)address2[2] << '\n';
                        }

                    }else{
                        std::cout << "[SERVER] Unknown message recieved from client #" << clientNumber << '\n';
                    }

    			}
    		}

    	}
    }

    SDLNet_TCP_Close(tcpSocket);
    SDLNet_FreeSocketSet(socketSet);

    return 0;
}
