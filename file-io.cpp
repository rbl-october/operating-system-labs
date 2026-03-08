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