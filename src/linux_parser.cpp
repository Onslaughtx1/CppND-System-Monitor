#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float totalMem{0.0};
  float freeMem{0.0};
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::remove(line.begin(), line.end(), ' ');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if (key == "MemTotal"){
          totalMem = std::stof(value);
          break;
        }  
          else if (key == "MemFree"){
          freeMem = std::stof(value);
          break;
        }
      }
    }
  }
  return ((totalMem - freeMem) / totalMem); 
 }

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
    string line;
    long uptimeSeconds{0};
    std::ifstream filestream(kProcDirectory + kUptimeFilename);
      if(filestream.is_open()){
        while(std::getline(filestream, line)){
          std::istringstream linestream(line);
          linestream >> uptimeSeconds; 
        }
      }
  return uptimeSeconds; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
    long active, idle;
  active = LinuxParser::ActiveJiffies();
  idle = LinuxParser::IdleJiffies();
  return active + idle;
  //return 0; 
  }

// DONE: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
    vector <string> values;
  string line, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while (linestream >> value){
        values.push_back(value);
      }
    }
  }
  long value13 = std::stol(values[13]);
  long value14 = std::stol(values[14]);
  long sysconfx = sysconf(_SC_CLK_TCK);
  long total = (value13 + value14) / sysconfx;
  //return std::stol(values[13] + values[14]) / sysconf(_SC_CLK_TCK);
  return total;
 }

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
    vector<string> cpuU = CpuUtilization();
  return stol(cpuU[CPUStates::kUser_]) + stol(cpuU[CPUStates::kNice_]) +
         stol(cpuU[CPUStates::kSystem_]) + stol(cpuU[CPUStates::kIRQ_]) +
         stol(cpuU[CPUStates::kSoftIRQ_]) + stol(cpuU[CPUStates::kSteal_]); 
         //return 0; 
         }

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> cpuU = CpuUtilization();

  return stol(cpuU[CPUStates::kIdle_]) + stol(cpuU[CPUStates::kIOwait_]);
  //return 0; 
  }

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector <string> values;
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key;
      if(key == "cpu"){
      while (linestream >> value){
        values.push_back(value); 
      }
        return values;
      }
    }
  }
  return {};
 }

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int totalProcesses;
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if (key == "processes"){
          totalProcesses = std::stoi(value);
          return totalProcesses;
        }  
      }
    }
  }
  return totalProcesses; 
 }

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
    int runningProcesses;
    string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
        while(linestream >> key >> value){
        if (key == "procs_running"){
          runningProcesses = std::stoi(value);
          return runningProcesses;
        }  
      }
    }
  }
  return runningProcesses; 
  }

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line, command;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while (linestream >> command){
        return command;
      }
    }
  }
  return "";
 }

// DONE: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
    string line, key, value, ramV;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if (key == "VmRSS"){
          ramV = std::to_string(int(std::stoi(value)/1024));
          return ramV;
        }  
      }
    }
  }
  return ""; 
 }

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if (key == "Uid"){
          return value;
        }  
      }
    }
  }
  return string(); 
 }

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
    string uid = Uid(pid);
    string line, key, userName{"user"}, a;
    std::ifstream filestream(kPasswordPath);
    if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> userName >> a >> key)
      if(key == uid){
        return userName;
      }
      }
    }
  return userName;
 }

// DONE: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
    string line, value;
  vector<string> values;
  long upTime = LinuxParser::UpTime();
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> value){
        values.push_back(value); 
      }
    }
  }
  long pidUptime = upTime - (stol(values[21]) / sysconf(_SC_CLK_TCK));

  return pidUptime;
 }

// Read the Cpu utilization of a process
float LinuxParser::CpuUtilization(int pid){
  vector<string> v;
  string line, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 22; i++){
      linestream >> value;
      v.push_back(value);
    }
    long uT, sT, startT, totalT;
    float cpuUt, secs;
    uT = stol(v[13]);
    sT = stol(v[14]);
    startT = stol(v[21]);
    totalT = uT + sT;
    long sysUpTime = LinuxParser::UpTime();
    secs = (float) ( sysUpTime - (startT/sysconf(_SC_CLK_TCK)) );
    if (secs){
      cpuUt = (float) ( (totalT/sysconf(_SC_CLK_TCK)) / secs );
      return cpuUt;
    }
  }
  return 0.0;
}

