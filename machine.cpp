//
// Created by Satyansh Yeluri on 12/22/23.
//
#include "machine.h"
// machine m_Id = "machine-m_Id-m_Version-timestamp-created"

/**
 * @brief Constructs a Machine object with the given parameters.
 *
 * @param id The ID of the machine.
 * @param ip The IP address of the machine.
 * @param status The m_Status of the machine.
 */
Machine::Machine(int argc, char *argv[])
{
    // get local ip address
    this->m_Status = "Online";
    this->m_Version = 0;

    ParseCommandArgs(argc, argv);
}
Machine::~Machine()
{
    return;
}
int Machine::Join(const char *ip)
{
    std::cout << ip << std::endl;
    int clientSocket = CreateSocket(ip, INTRODUCER_PORT, SOCK_STREAM, 1);

    if (clientSocket == -1)
    {
        std::cerr << "Error creating socket\n";
        return -1;
    }

    //    const char *message = "Hello from client!";
    //
    //    int sent_bytes = send(clientSocket, message, strlen(message), 0);
    //
    //    if (sent_bytes == -1)
    //    {
    //        std::cerr << "Error sending message\n";
    //        close(clientSocket);
    //        return -1;
    //    }
    char buffer[4000];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesReceived == -1)
    {
        std::cerr << "Error receiving data\n";
    }
    else if (bytesReceived == 0)
    {
        std::cout << "Connection closed by server\n";
    }
    else
    {
        // Process the received data
        std::cout << "Received data from server: " << buffer << std::endl;
    }

    close(clientSocket);

    return 0;
}

/**
 *
 * HELPER FUNCTIONS
 *
 * */

/**
 * Creates specified socket for the machine.
 *
 * @param ip The IP address to bind the socket to.
 * @param port The port number to bind the socket to.
 * @param protocol The protocol to use for the socket (TCP or UDP).
 * @param client 1 is connecting, 0 is receiving
 * @return The created socket or -1 if an error occurred.
 */
int Machine::CreateSocket(const char *ip, int port, int protocol, int client)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    int reuse = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
    {
        std::cerr << "Error setting socket options\n";
        close(sock);
        return -1;
    }
    if (sock == -1)
    {
        return -1;
    }

    sockaddr_in serverAddress = {};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    //    serverAddress.sin_addr.s_addr = strcmp(ip, "ANY") == 0 ? INADDR_ANY : inet_addr(ip);
    std::cout << (strcmp(ip, "ANY") == 0) << std::endl;
    if (client)
    {
        if (connect(sock, reinterpret_cast<struct sockaddr *>(&serverAddress), sizeof(serverAddress)) == -1)
        {
            std::cout << "here" << std::endl;
            close(sock);
            return -1;
        }
    }
    else
    {
        if (bind(sock, reinterpret_cast<struct sockaddr *>(&serverAddress), sizeof(serverAddress)) == -1)
        {
            close(sock);
            return -1;
        }
    }

    return sock;
}

/**
 * Sets up the introducer for the machine.
 * This function creates a socket and sets the server address for the introducer.
 *
 * @return 0 if successful, -1 if an error occurs.
 */
int Machine::SetUpIntroducer(int NumberOfConnections)
{

    int serverSocket = CreateSocket("ANY", INTRODUCER_PORT, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        std::cerr << "Error binding socket\n";
        return -1;
    }
    // Listen for incoming connections
    if (listen(serverSocket, NumberOfConnections) == -1)
    {
        std::cerr << "Error listening for connections\n";
        close(serverSocket);
        return -1;
    }

    while (NumberOfConnections > 0)
    {
        sockaddr_in clientAddress = {};
        socklen_t clientAddrSize = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddrSize);

        if (clientSocket == -1)
        {
            std::cerr << "Error accepting connection\n";
            continue;
        }

        std::cout << "Connection accepted from " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << std::endl;

        HandleNewConnection(clientSocket, inet_ntoa(clientAddress.sin_addr));

        // Close the client socket when done
        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}

int Machine::HandleNewConnection(int fd, char *ip)
{
    std::cout << "Handling new connection" << std::endl;
    std::string id = "1";
    this->m_Membership->AddMember(id, ip, fd);

    char *message = new char[4000];
    this->m_Membership->SerializeMembership(message);

    send(fd, message, strlen(message), 0);
};

/**
 * Sends a message over a socket.
 *
 * @param socket The socket to send the message on.
 * @param message The message to send.
 * @return 0 if the message is sent successfully, -1 otherwise.
 */
// int Machine::SendMessage(int socket, const char *message)
// {
//     if (send(socket, message, strlen(message), 0) == -1)
//     {
//         return -1;
//     }
//     return 0;
// }

int Machine::ParseCommandArgs(int argc, char *argv[])
{
    // TODO: error checking arguments
    // TODO: assign variables
    std::string introducer_ip;
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-I") == 0)
        {
            this->m_IsIntroducer = true;

            //            introducer_ip = argv[i + 1];

            // if (std::all_of(argv[i + 1], argv[i + 1] + strlen(argv[i + 1]), ::isdigit))
            // {
            //     std::cout << "IP: " << argv[i + 1] << std::endl;
            // }
            // else
            // {
            //     std::cerr << "Error: ID must be a number.\n";
            //     exit(1);
            // }

            //            i++;
            continue;
        }
        //        if (strcmp(argv[i], "-id") == 0)
        //        {
        //            if (std::all_of(argv[i + 1], argv[i + 1] + strlen(argv[i + 1]), ::isdigit))
        //            {
        //                std::cout << "ID: " << argv[i + 1] << std::endl;
        //            }
        //            else
        //            {
        //                std::cerr << "Error: ID must be a number.\n";
        //                exit(1);
        //            }
        //            i++;
        //            continue;
        //        }
        std::cerr << "Usage: ./machine <introducer_ip> <id>\n";
    }
    //    if (argc < 5)
    //    {
    //        std::cerr << "Usage: ./machine <introducer_ip> <id>\n";
    //        exit(1);
    //    }

    int arbitrary_size = 1024;
    char hostname[arbitrary_size];
    hostname[arbitrary_size - 1] = '\0';
    gethostname(hostname, arbitrary_size - 1);
    struct hostent *h;
    h = gethostbyname(hostname);
    std::string ip_address = inet_ntoa(*((struct in_addr *)h->h_addr_list[0]));
    std::cout << "IP: " << ip_address << std::endl;
    this->m_MyIp = ip_address;
    this->m_Id = 0;
    this->m_Membership = new Membership(reinterpret_cast<std::string &>(this->m_Id), reinterpret_cast<std::string &>(this->m_MyIp));
    this->m_IntroducerIp = "127.0.0.1";
    return 0;
}
