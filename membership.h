//
// Created by Satyansh Yeluri on 12/22/23.
//

// Membership Class
// virtual update methods that can be overwritten with own failure detection methods

#ifndef DISTRIBUTED_SYSTEM_MEMBERSHIP_H
#define DISTRIBUTED_SYSTEM_MEMBERSHIP_H
#include <mutex>
#include <sstream>
#include <iostream>
#include <cstring>
#include <unordered_map>
// std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
class Member
{
public:
    Member();
    Member(std::string id, std::string ip, int socketDescriptor) : m_Status("Active"),
                                                                   m_Id(id),
                                                                   m_Ip(ip),
                                                                   m_HeartbeatCount(0),
                                                                   m_Timestamp(""),
                                                                   m_Failed(false),
                                                                   m_SocketDescriptor(0) {}

    ~Member()
    {
        return;
    }
    bool operator()(const Member &member1, const Member &member2) const
    {
        return member1.GetHeartbeat() < member2.GetHeartbeat();
    }
    std::string GetStatus()
    {
        return this->m_Status;
    }
    void SetStatus(std::string &status)
    {
        this->m_Status = status;
    }
    std::string GetIp()
    {
        return this->m_Ip;
    }
    std::string GetId()
    {
        return this->m_Ip;
    }
    int GetHeartbeat() const
    {
        return this->m_HeartbeatCount;
    }
    void UpdateHeartbeat()
    {
        this->m_HeartbeatCount += 1;
    }
    void SetHeartbeat(int HeartbeatCount)
    {
        this->m_HeartbeatCount = HeartbeatCount;
    }
    void SetTimestamp()
    {
        this->m_Timestamp = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    }

private:
    std::string m_Status;
    std::string m_Id;
    std::string m_Ip;
    int m_HeartbeatCount;
    std::string m_Timestamp;
    int m_SocketDescriptor;
    bool m_Failed;

    // failed
    // id
    // ip
    // heartbeat
    // last_timestamp
};

class Membership
{
private:
    static std::mutex m_MembershipMutex;

    // map of members
    // failed members
    //

public:
    std::unordered_map<std::string, Member *> m_MembershipList;
    Membership();
    Membership(std::string &id, std::string &ip);
    ~Membership();
    virtual void AddMember(const std::string &id, const std::string &ip, int socketDescriptor);
    virtual void UpdateMember(const std::string &id, int heartbeatCount);
    virtual void RemoveMember(const std::string &id);
    void SerializeMembership(char *buffer);
    const char *DeserializeMembership(const std::string &serializedData);
    static std::string SerializeMember(Member *member);
    void DeserializeMember(const std::string &id, const std::string &serializedData);
    bool ShouldMemberUpdate(const std::string &id, const int heartbeatCount);
};

#endif // #define DISTRIBUTED_SYSTEM_MEMBERSHIP_H
