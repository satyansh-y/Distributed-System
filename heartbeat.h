//
// Created by Satyansh Yeluri on 12/22/23.
//

// Failure Detection Class

#ifndef DISTRIBUTED_SYSTEM_HEARTBEAT_H
#define DISTRIBUTED_SYSTEM_HEARTBEAT_H
#include "membership.h"
//TFailure
//TCleanup
//Suspected Nodes
//
class GossipStyleHeartBeat {
    
    private:
        //map of failed members to timestamp detected
        //failed members
        // 
        int m_TFailure = 10;
        int m_TCleanup = 10;
        std::unordered_map<int, std::string> suspected_nodes;

    public:
        GossipStyleHeartBeat();
          

};



#endif // #define DISTRIBUTED_SYSTEM_HEARTBEAT_H

