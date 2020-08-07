#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return getPID(); }
//

int Process::getPID(){ return pid;}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return getCPUUtil(); }

float Process::getCPUUtil() {
  long total_time = LinuxParser::ActiveJiffies(pid);
  long uptime = LinuxParser::UpTime();
  long starttime = LinuxParser::UpTime(pid);
  long HERTZ = sysconf(_SC_CLK_TCK);
  float seconds = uptime - (starttime / HERTZ);
  cpu_util = ((total_time / HERTZ) / seconds);
  return cpu_util;
}

// TODO: Return the command that generated this process
string Process::Command() { return getCommand(); }

string Process::getCommand(){return LinuxParser::Command(pid); }

// TODO: Return this process's memory utilization
string Process::Ram() { return getRam(); }
//

string Process::getRam(){ ram = LinuxParser::Ram(pid); return ram;}

// TODO: Return the user (name) that generated this process
string Process::User() { return getUser(); }

string Process::getUser(){ return LinuxParser::User(pid);}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return getUpTime(); }

long int Process::getUpTime() {
  uptime = LinuxParser::UpTime(pid);
  return uptime;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { return this->cpu_util > a.cpu_util; }

void Process::setPID(int p){ pid = p;}

void Process::setUser(){user = std::string(); user = LinuxParser::User(pid);}

void Process::setCommand(){command = std::string(); command = LinuxParser::Command(pid);}

Process::Process(int p){
  setPID(p);
  setUser();
  setCommand();
  getCPUUtil();
  getRam();
  getUpTime();
}