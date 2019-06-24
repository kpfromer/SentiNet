#ifndef RKS_PROCESSES_PROCESS_CONTROL_BLOCK_HPP
#define RKS_PROCESSES_PROCESS_CONTROL_BLOCK_HPP

namespace rks
{
namespace process
{

class ProcessControlBlock
{
  

  private:
    RKS_POINTER
    RKS_PROCESS_STATE state;
    RKS_PID pid;
    RKS_PNUM 
