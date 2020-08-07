#ifndef PROCESS_H
#define PROCESS_H

#include "linux_parser.h"
#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  
  Process(int p=0); 
  
  // Mutator functions 
  void setPID(int);
  void setUser();
  void setCommand();
  
  //Accessor functions 
  int getPID();
  std::string getUser();
  std::string getCommand();
  std::string getRam();
  float getCPUUtil();
  long int getUpTime();
  
  // TODO: Declare any necessary private members
 private: 
  int pid;
  float cpu_util;
  std::string command, user, ram;
  long int uptime;
};

#endif