//
// Created by Satyansh Yeluri on 12/22/23.
//

#include "membership.h"

// Initialize the static mutex
std::mutex Membership::m_MembershipMutex;

Membership::Membership(std::string &id, std::string &ip)
{
    m_MembershipMutex.lock();
    // this->AddMember(id, ip);
    this->m_MembershipList[ip] = new Member(id, ip, 0);
    m_MembershipMutex.unlock();
}
Membership::~Membership()
{
    for (auto mem : this->m_MembershipList)
    {
        delete mem.second;
    }
}
void Membership::AddMember(const std::string &id, const std::string &ip, int socketDescriptor)
{
    m_MembershipMutex.lock();
    this->m_MembershipList[ip] = new Member(id, ip, socketDescriptor);
    m_MembershipMutex.unlock();
}
/*
 *Updates the heartbeat count and timestamp of the member
 *USED AS A HELPER FUNCTION, does not acquire lock
 */
void Membership::UpdateMember(const std::string &id, int heartbeatCount)
{
    this->m_MembershipList[id]->SetHeartbeat(heartbeatCount);
    this->m_MembershipList[id]->SetTimestamp();
}
void Membership::RemoveMember(const std::string &ip)
{
    m_MembershipMutex.lock();
    delete this->m_MembershipList[ip];
    m_MembershipMutex.unlock();
}

// Add a function to serialize Member object to a string
std::string Membership::SerializeMember(Member *member)
{
    std::ostringstream oss;
    oss << "{\"status\":\"" << member->GetStatus() << "\",\"id\":\"" << member->GetId() << "\",\"ip\":\"" << member->GetIp() << "\",\"heartbeat\":" << member->GetHeartbeat() << "}";
    return oss.str();
}

// Add a function to deserialize a string to a Member object
void Membership::DeserializeMember(const std::string &id, const std::string &serializedData)
{
    std::istringstream iss(serializedData);

    bool newMember = false;
    if (this->m_MembershipList.find(id) == this->m_MembershipList.end())
    {
        newMember = true;
    }

    char ch;
    std::string key, value;
    std::string status;
    std::string ip;
    int heartbeatCount;
    std::string timestamp;

    // Extract member attributes
    while (iss >> ch)
    {
        if (ch == '"' || ch == ',')
        {
            // Extract the key
            std::getline(iss, key, '"');
            iss >> ch; // Skip the ':' character

            // Extract the value
            std::getline(iss, value, ',');

            if (key == "status")
                status = value;
            else if (key == "id")
                continue;
            else if (key == "ip")
                ip = value;
            else if (key == "heartbeat")
                heartbeatCount = std::stoi(value);
        }
    }
    if (newMember)
    {
        this->AddMember(id, ip, 0);
    }
    else if (ShouldMemberUpdate(id, heartbeatCount))
    {
        this->UpdateMember(id, heartbeatCount);
    }
}
bool Membership::ShouldMemberUpdate(const std::string &id, const int heartbeatCount)
{
    Member *currMember = this->m_MembershipList[id];
    if (currMember->GetHeartbeat() < heartbeatCount)
    {
        return true;
    }
    return false;
}
// Serialize unordered_map of Members to a string
void Membership::SerializeMembership(char *buffer)
{
    std::ostringstream oss;
    oss << "{";
    for (const auto &entry : this->m_MembershipList)
    {
        oss << "\"" << entry.first << "\":" << SerializeMember(entry.second) << ",";
    }
    if (!this->m_MembershipList.empty())
    {
        oss.seekp(-1, std::ios_base::end); // Remove the trailing comma
    }
    oss << "}";
    std::strcpy(buffer, oss.str().c_str());
}

const char *Membership::DeserializeMembership(const std::string &serializedData)
{

    std::istringstream iss(serializedData);
    char ch;
    std::string key, value;

    // Assuming the buffer is a JSON-like string
    while (iss >> ch && ch != '}')
    {
        if (ch == '"' || ch == ',')
        {
            // Extract the key
            std::getline(iss, key, '"');
            iss >> ch; // Skip the ':' character

            // Extract the value (serialized Member object)
            std::getline(iss, value, ',');
            value = value.substr(0, value.rfind('}'));

            // Deserialize the member and add to the list
            DeserializeMember(key, value);
        }
    }
}
