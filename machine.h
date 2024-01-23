//
// Created by Satyansh Yeluri on 12/22/23.
//

// Machine Class

#ifndef DISTRIBUTED_SYSTEM_MACHINE_H
#define DISTRIBUTED_SYSTEM_MACHINE_H
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "membership.h"
#define INTRODUCER_PORT 5001
#define HEARTBEAT_PORT 5001
#define COMMUNICATION_PORT 5002

class Machine
{
private:
    uint32_t m_Id;
    bool m_IsIntroducer;
    //    sockaddr_in serverAddress;
    std::string m_Status;
    std::string m_MyIp;
    int m_Version;
    std::unordered_map<int, std::string> m_MachineMapIdToIp;
    std::string m_IntroducerIp;
    Membership *m_Membership;

public:
    Machine();
    Machine(int argc, char *argv[]);
    ~Machine();

    int Join(const char *ip);

    int SetUpIntroducer(int NumberOfConnections);
    int CreateSocket(const char *ip, int port, int protocol, int client);
    int SendMessage(int socket, const char *message);
    // #TODO: creates id in the form "machine number - ip - timestamp(version)"
    std::string CreateId(int num);
    virtual int HandleNewConnection(int fd, char *ip);
    int GetId() const
    {
        return m_Id;
    }

    void SetId(int id)
    {
        this->m_Id = id;
    }

    std::string GetIp() const
    {
        return this->m_MyIp;
    }

    std::string GetStatus() const
    {
        return this->m_Status;
    }

    void SetStatus(int status)
    {
        this->m_Status = status;
    }

    std::string GetMachineIp(int id)
    {
        return m_MachineMapIdToIp[id];
    }
    bool IsIntroducer(){
        return m_IsIntroducer;
    }
    std::string getIntroducerIp(){
        return m_IntroducerIp;
    }

    std::string toString();
    int ParseCommandArgs(int argc, char *argv[]);
};

#endif // DISTRIBUTED_SYSTEM_MACHINE_H
