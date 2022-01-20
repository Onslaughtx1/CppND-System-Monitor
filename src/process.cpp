#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid){
  pid_ = pid; 
  command_ = LinuxParser::Command(pid);
  userName_ = LinuxParser::User(pid);
  ram_ = LinuxParser::Ram(pid);
  upTime_ = LinuxParser::UpTime(pid);
  cpu_ = LinuxParser::CpuUtilization(pid);

}

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {
long totalJiffies = LinuxParser::ActiveJiffies(pid_);
cpu_ = float(totalJiffies) / float(upTime_);

return cpu_;
//return 0; }
}

// DONE: Return the command that generated this process
string Process::Command() { return command_; }

// DONE: Return this process's memory utilization
string Process::Ram() { return ram_; }

// DONE: Return the user (name) that generated this process
string Process::User() { return userName_; }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return upTime_; }

// DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return (cpu_ > a.cpu_);
    //return (ram_ > a.ram_);
    //return true; 
    }