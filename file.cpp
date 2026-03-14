#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <queue>
#include <iomanip>

using namespace std;

enum class Policy
{
    SJF,
    SRTN
}; // Danh sách các thuật toán điều phối
   // Sử dụng enum class để đạt được ba mục đích: tối ưu hóa hiệu suất + tránh lỗi không đáng có; an toàn dữ liệu; xác định phạm vi sử dụng của biến

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

    Process() : arrivalTime(0), burstTime(0), remainingTime(0), completionTime(0), turnaroundTime(0), waitingTime(0) {}
    // Khởi tạo một constructor mặc định
};

struct CPUQueue
{
    string qid;
    int timeSlice;
    Policy policy; // (SJF hoặc SRTN)
    vector<Process *> processes;
};

class Scheduler
{
private:
    vector<CPUQueue> queues;
    vector<Process *> allProcesses; // Dùng Process* để tránh tái phân bổ vùng nhớ
    int currentTime = 0;
    Process *selectSJF(CPUQueue &currentQ, Process *prevProcess);
    Process *selectSRTN(CPUQueue &currentQ);
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

    ~Scheduler() // Xóa hàng đợi để tránh memory leak
    {
        for (Process *p : allProcesses)
        {
            delete p;
        }
    }
};