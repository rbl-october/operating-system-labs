#include "file.cpp"

void Scheduler::loadInput(string filename)
{
    ifstream inFile(filename);
    if (!inFile.is_open()) // File có đọc được không
    {
        cerr << "Loi: Khong the mo file " << filename << endl;
        exit(1);
    }
    int N;
    if (!(inFile >> N))
    {
        cerr << "Loi: Khong doc duoc N - loi dinh dang hoac rong";
        exit(1);
    }
    for (int i = 0; i < N; i++)
    {
        CPUQueue q;
        string policyStr;

        inFile >> q.qid >> q.timeSlice >> policyStr;

        if (policyStr == "SJF")
            q.policy = Policy::SJF;
        else if (policyStr == "SRTN")
            q.policy = Policy::SRTN;

        queues.push_back(q);
    }
    string pid, qid;
    int aTime, bTime; // aTime = time of arrival, bTime = burst time

    while (inFile >> pid >> aTime >> bTime >> qid)
    {
        Process *p = new Process;
        p->id = pid;
        p->arrivalTime = aTime;
        p->burstTime = bTime;

        p->remainingTime = bTime;
        p->queueID = qid;
        allProcesses.push_back(p);
    }

    inFile.close();
}

void Scheduler::calculateStatistics()
{
    for (Process *p : allProcesses)
    {

        // Thời gian lưu lại hệ thống = Thời điểm hoàn thành - Thời điểm đến
        p->turnaroundTime = p->completionTime - p->arrivalTime;

        // Thời gian chờ = Thời gian lưu lại - Thời gian thực thi (Burst Time)
        p->waitingTime = p->turnaroundTime - p->burstTime;
    }
}

void Scheduler::exportOutput(string filename)
{
    ofstream outFile(filename);
    if (!outFile.is_open())
    {
        cerr << "Loi: Khong the xuat ket qua ra " << filename << "\n";
        return;
    }
    outFile << "================== CPU SCHEDULING DIAGRAM ==================\n";
    outFile << left << setw(20) << "[Start-End]"
            << setw(15) << "Queue"
            << setw(15) << "Process" << "\n";
    outFile << "---------------------------------------------------------------------\n";
    for (const auto &GanttEntry : ganttChart)
    {
        string timeRange = "[" + to_string(GanttEntry.start) + "-" + to_string(GanttEntry.end) + "]";
        outFile << left << setw(20) << timeRange
                << setw(15) << GanttEntry.qid
                << setw(15) << GanttEntry.pid << "\n";
    }
    stable_sort(allProcesses.begin(), allProcesses.end(), [](Process *a, Process *b)
                {
                    // Ưu tiên độ dài chuỗi trước (để P10 không bị xếp trước P2)
                    if (a->id.length() != b->id.length())
                    {
                        return a->id.length() < b->id.length();
                    }
                    return a->id < b->id; // Sau đó mới so sánh theo bảng chữ cái
                });
    outFile << "================ PROCESS STATISTICS ================\n";
    outFile << left << setw(10) << "Process"
            << setw(10) << "Arrival"
            << setw(10) << "Burst"
            << setw(15) << "Completion"
            << setw(15) << "Turnaround"
            << setw(10) << "Waiting" << "\n";
    outFile << "---------------------------------------------------------------------\n";

    double totalTurnaround = 0;
    double totalWaiting = 0;

    for (Process *p : allProcesses)
    {
        outFile << left << setw(10) << p->id
                << setw(10) << p->arrivalTime
                << setw(10) << p->burstTime
                << setw(15) << p->completionTime
                << setw(15) << p->turnaroundTime
                << setw(10) << p->waitingTime << "\n";

        // Cộng dồn để tính số trung bình
        totalTurnaround += p->turnaroundTime;
        totalWaiting += p->waitingTime;
    }

    // Xuất các thông số trung bình
    if (!allProcesses.empty())
    {
        double avgTurnaround = totalTurnaround / allProcesses.size();
        double avgWaiting = totalWaiting / allProcesses.size();

        outFile << "Average Turnaround Time: " << avgTurnaround << "\n";
        outFile << "Average Waiting Time: " << avgWaiting << "\n";
    }

    outFile.close();
}