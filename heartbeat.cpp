#include "heartbeat.h"


std::mutex Membership::m_MembershipMutex;

void SuspectFailures(std::unordered_map<std::string, Member *> &membershipList)
{
    m_MembershipMutex.lock();
    for(auto member: membershipList){
        if(member)
    }
    m_MembershipMutex.unlock();
}
void DetectFailures()
{
    m_MembershipMutex.lock();
    m_MembershipMutex.unlock();
}