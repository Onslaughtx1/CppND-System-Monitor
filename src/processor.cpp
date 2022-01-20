#include "processor.h"
#include "linux_parser.h"

#include <vector>
#include <string>

using std::vector;
using std::string;

// DONE: Return the aggregate CPU utilization

 float Processor::Utilization() {

     std::vector <string> cpu_U = LinuxParser::CpuUtilization();
     const long user = std::stol(cpu_U[0]);
     const long nice = std::stol(cpu_U[1]);
     const long system = std::stol(cpu_U[2]);
     const long idle = std::stol(cpu_U[3]);
     const long iowait = std::stol(cpu_U[4]);
     const long irq = std::stol(cpu_U[5]);
     const long sirq = std::stol(cpu_U[6]);
     const long steal = std::stol(cpu_U[7]);

     const long idleT = idle + iowait;
     const long total = user + nice + system + idle + iowait + irq + sirq + steal;

     if(total != 0){
         float cpuPer = (float) (total - idleT) / total;
         return cpuPer;
     }
     return 0.0;
 }