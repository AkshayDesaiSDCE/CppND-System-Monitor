#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

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
    if (file->d_type == DT_DIR) {
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = std::stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  unsigned int mem_tot, mem_free; 
  //mem_avail;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        //std::cout<<80<<"\n";
        if (key == "MemTotal:") mem_tot = std::stoi(value);
        else if(key == "MemFree:") mem_free = std::stoi(value);
        //else if(key == "MemAvailable:") mem_avail = stoi(value);
      }
    }
  }
  return ((float)mem_tot - (float)mem_free)/mem_tot;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  string key;
  long uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
   if (stream.is_open()) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> key;
      uptime = stof(key);
   }
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  long int uptime = LinuxParser::UpTime();
  return uptime*sysconf(_SC_CLK_TCK);
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    string line;
    vector<string> val_vec;
    int count = 0;
    std::getline(filestream,line);
    std::istringstream linestream(line);
    while(linestream.good()){
      if(count >= 22) break;
      string val;
      linestream >> val;
      val_vec.push_back(val);
      count++;
    }
    long uTime = std::stol(val_vec[13]);
    long sTime = std::stol(val_vec[14]);
    long cuTime = std::stol(val_vec[15]);
    long csTime = std::stol(val_vec[16]);

    long Total = uTime + sTime + cuTime + csTime;
    return Total;
  }
  return 0;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  string line;
  string val;
  long int sum=0;
  vector<string> val_vec;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line) && val_vec.size()<11) {
      std::istringstream linestream(line);
      while (linestream >> val) {
          val_vec.push_back(val);
      }
      //std::cout<<145<<"\n";
      if(val_vec.size()>=2) sum+=std::stoi(val_vec.back());
    }
  }
  return sum;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string line;
  string val;
  vector<string> val_vec;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line) && val_vec.size()<6) {
      std::istringstream linestream(line);
      while (linestream >> val) {
          val_vec.push_back(val);
      }
    }
  }
  //std::cout<<166<<"\n";
  return std::stoi(val_vec[5]) + std::stoi(val_vec[4]);
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string field, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  vector<string> val_vec = {};
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> field >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    val_vec = { user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice};
  }
  return val_vec;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        //std::cout<<194<<"\n";
        if (key == "processes") return std::stoi(value);
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  unsigned int proc;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        //std::cout<<213<<"\n";
        if (key == "procs_running") proc = std::stoi(value);
      }
    }
  }
  return proc;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line, res;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
      std::getline(stream, line);
      res = line;
   }
  return res;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while(filestream.good()){
    string line;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    string key, value;
    linestream >> key >> value;
    if(key == "VmSize:"){
        int ram = std::stoi(value);
        ram /= 1000;
        return to_string(ram);
    }
  }
}
  return string();
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key, value, uid;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") uid = value;
      }
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line;
  string user, letter, uid; 
  string res;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> letter >> uid) {
        if (uid == LinuxParser::Uid(pid)) {
          res = user;
        }
      }
    }
  }
  return res;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line;
  vector<string> val_vec;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    string elem;
    std::getline(filestream, line);
    std::istringstream linestream(line);
      while (val_vec.size() < 22) {
          linestream >> elem;
          val_vec.push_back(elem);
      }
    
  long upTime = (std::stol(val_vec[val_vec.size()-1]));
  return upTime;
  }
  return 0;
}