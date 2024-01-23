#include <iostream>
#include "machine.h"

int main(int argc, char *argv[])
{
    
    for (int i = 0; i < argc; ++i)
    {
        std::cout << "Argument " << i << ": " << argv[i] << std::endl;
    }
    Machine machine = Machine(argc, argv);
    if (machine.IsIntroducer())
    {
        machine.SetUpIntroducer(5);
    }
    else
    {
        std::string IntroducerIp = machine.getIntroducerIp();
        machine.Join(IntroducerIp.c_str());
    }

    // parse text file and create map of m_Id number to ips

    return 0;
}
