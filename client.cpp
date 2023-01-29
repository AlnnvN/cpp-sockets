/*Default includes*/
#include <iostream>
#include <string.h>
#include <math.h>

/*UNIX dependencies*/
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

class SocketException : std::exception{
    private:
    const char *description;
    public:
    SocketException(const char * description){
        this->description = description; 
    }

    const char *what(){
        return description;
    }
};

/*
unsigned short portStrToShort(const char *PORT){
    unsigned short socketPort;
    try{
        if(std::stoi(PORT) > pow(2,sizeof(unsigned short)*8)) //treatment for port numbers above 16 bits (out of range for unsigned short)
            throw std::out_of_range("stoi");
        socketPort = std::stoi(PORT); //std::invalid_argument or std::out_of_range

        std::cout << socketPort << std::endl;
    }

    catch(std::invalid_argument exception){
        //std::cout << exception.what() << std::endl;
        std::cout << "std::stoi() -> std::invalid_argument - Invalid port argument - '" << PORT << "' is not an integer" << std::endl;
        return -1;
    }

    catch(std::out_of_range exception){
        //std::cout << exception.what() << std::endl;
        std::cout << "std::stoi() -> std::out_of_range - Port number must be up to 65536 (16 bits)" << std::endl;
        return -1;
    }

    catch(std::exception exception){
        //std::cout << exception.what() << std::endl;
        std::cout << "std::exception - Default exception in openConnection()" << std::endl;
        return -1;
    }

    return socketPort;
}
*/

void openConnection(const char *HOST_NAME, const char *PORT){
    int getAddrStatus;
    int socketFileDescriptor;
    struct addrinfo hints, *serverInfo;
    

    try{

        memset(&hints, 0, sizeof(hints)); 
        hints.ai_family     = AF_UNSPEC;
        hints.ai_socktype   = SOCK_STREAM; //TCP

        //Getting server info from the initial addrinfo (hints), HOST/IP and port
        if((getAddrStatus = getaddrinfo(HOST_NAME, PORT, &hints, &serverInfo)) != 0){
            //tests may be needed for port length ---> gai_strerror() exceptions not clear
            throw SocketException(gai_strerror(getAddrStatus)); 
            return;
        }

        //File Descriptor/Socket Descriptor created based on the serverInfo 
        if((socketFileDescriptor = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol)) == -1){
            //maybe try to implement exception with strerror(errno), error string may be useful
            throw SocketException("Couldn't create file descriptor - socket()");
            return;
        }

        //Connects to remote host
        if(connect(socketFileDescriptor, serverInfo->ai_addr, serverInfo->ai_addrlen) < 0){
            //maybe try to implement exception with strerror(errno), error string may be useful
            throw SocketException("Couldn't connect to remote host - connect()");
        }

        freeaddrinfo(serverInfo); //getaddrinfo()'s linked tree, used in struct addrinfo *serverInfo, is freed;
    }
    catch(SocketException exception){
        std::cout << "SocketException - Error during socket creation/connection ---> " << exception.what() << std::endl;
    }
    catch(std::exception exception){
        std::cout << "std::exception - Default exception in openConnection()" << std::endl;
        return;
    }
    return;
}

int main(int argc, char *argv[]){
    try{
        openConnection(argv[1], argv[2]); 
    }
    catch(std::exception exception){
        //std::cout << exception.what() << std::endl;
        std::cout << "Default exception in main()" << std::endl;
        return 0;
    }
    return 0;
}