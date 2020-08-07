#include "processor.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    std::vector<std::string> parsed_data = LinuxParser::CpuUtilization();

    std::vector<long> long_data{};
    for(auto& ele: parsed_data){
        long_data.push_back(std::stol(ele));
    }

    long idleTime = long_data[3] + long_data[4];
    long nonIdleTime = long_data[0] + long_data[1] + long_data[2] + long_data[5] + long_data[6] + long_data[7];
    long totalTime = idleTime + nonIdleTime;
    //std::cout<<totalTime<<" "<<totalTime - idleTime<<"\n";

    return (float)(totalTime-idleTime)/totalTime;
}