#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <queue>

struct Process
{
    string id;         
    int arrivalTime;   
    int burstTime;      
    int remainingTime; // (SRTN) 
    string queueID;    // id hàng đợi 

    // Đầu ra 
    int completionTime; 
    int turnaroundTime; 
    int waitingTime;    

    bool isCompleted = false;

    Process() : completionTime(0), turnaroundTime(0), waitingTime(0) {}
};

struct CPUQueue
{
    string qid;                 
    int timeSlice;               
    Policy policy;    // (SJF hoặc SRTN)
    vector<Process *> processes; 
};

class Scheduler
{
private:
    vector<CPUQueue> queues;      
    vector<Process> allProcesses; 
    int currentTime = 0;

struct GanttEntry
    {
        int start, end;
        string qid, pid;
    };
    vector<GanttEntry> ganttChart;

public:
    
    void loadInput(string filename);
    // Thuật toán : Round Robin giữa các Queue 
    void run();
    void exportOutput(string filename);
    void calculateStatistics();
};