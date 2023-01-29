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

class SocketException : std::exception
{
private:
    const char *description;

public:
    SocketException(const char *description)
    {
        this->description = description;
    }

    const char *what()
    {
        return description;
    }
};

void openConnection(const char *HOST_NAME, const char *PORT)
{
    int getAddrStatus;
    int socketFileDescriptor;
    struct addrinfo hints, *serverInfo;

    try
    {
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM; // TCP

        // Getting server info from the initial addrinfo (hints), HOST/IP and port - getaddrinfo()
        if ((getAddrStatus = getaddrinfo(HOST_NAME, PORT, &hints, &serverInfo)) != 0)
        {
            // tests may be needed for port length ---> gai_strerror() exceptions not clear
            throw SocketException(gai_strerror(getAddrStatus));
            return;
        }

        // File Descriptor/Socket Descriptor created based on the serverInfo - socket()
        if ((socketFileDescriptor = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol)) < 0)
        {
            // maybe try to implement exception with strerror(errno), error string may be useful
            throw SocketException("Couldn't create file descriptor - socket()");
            return;
        }

        // Connects to remote host - connect()
        if (connect(socketFileDescriptor, serverInfo->ai_addr, serverInfo->ai_addrlen) < 0)
        {
            // maybe try to implement exception with strerror(errno), error string may be useful
            throw SocketException("Couldn't connect to remote host - connect()");
            return;
        }

        //********* Taking user input for the message - temporary and socket-wise unrelated. TESTING PURPOSES ONLY **********
        char *message = new char[256];
        memset(message, 0, sizeof(message));
        std::cout << "Enter a message: ";
        std::cin.getline(message, 256);
        if (message == 0)
        {
            std::cout << "Error reading message";
            return;
        }
        ///***********************************************************************************************************///

        // Sends message to the stored socket descriptor - send()
        if (send(socketFileDescriptor, message, strlen(message), 0) < 0)
        {
            // maybe try to implement exception with strerror(errno), error string may be useful
            throw SocketException("Couldn't send the message to the server - send()");
            return;
        }

        delete (message); // TESTING PURPOSES ONLY

        close(socketFileDescriptor);

        freeaddrinfo(serverInfo); // getaddrinfo()'s linked tree, used in struct addrinfo *serverInfo, is freed;
    }

    catch (SocketException exception)
    {
        std::cout << "SocketException - Error during socket creation/connection ---> " << exception.what() << std::endl;
    }

    catch (std::exception exception)
    {
        std::cout << "std::exception - Default exception at openConnection()" << std::endl;
        return;
    }

    return;
}

int main(int argc, char *argv[])
{
    try
    {
        openConnection(argv[1], argv[2]);
    }
    catch (std::exception exception)
    {
        std::cout << "std::exception - Default exception at main()" << std::endl;
        return 0;
    }
    return 0;
}