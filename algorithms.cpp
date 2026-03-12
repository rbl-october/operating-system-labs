#include "file-io.cpp"

void Scheduler::run()
{
    // Bước 1: Khởi tạo

    // Sắp xếp các tiến trình theo thứ tự thời gian đến tăng dần
    sort(allProcesses.begin(), allProcesses.end(), [](Process *a, Process *b)
         {
    // Nếu arrivalTime bằng nhau, tiến trình nào nằm trước trong file input sẽ đứng trước
        if (a->arrivalTime == b->arrivalTime)
        {
            return false; // Giữ nguyên thứ tự ban đầu để đảm bảo tính ổn định (stable)
        }
    return a->arrivalTime < b->arrivalTime; });

    currentTime = 0;
    Process *prevProcess = nullptr;
    int completedProcess = 0;
    int nextProcessIdx = 0;
    int totalProcesses = allProcesses.size();

    int qIndex = 0;
    int timeCurrentQueue = 0;

    // Bước 2: Đồng hồ chung
    while (completedProcess < totalProcesses)
    {
        // 1. Xử lý tiến trình mới
        while (nextProcessIdx < totalProcesses && allProcesses[nextProcessIdx]->arrivalTime == currentTime)
        {
            Process *p = allProcesses[nextProcessIdx];
            for (int i = 0; i < queues.size(); i++)
            {
                if (queues[i].qid == p->queueID)
                {
                    queues[i].processes.push_back(p);
                    break;
                }
            }
            nextProcessIdx++;
        }
        // 2. Điều phối Round-robin & 2 thuật toán: Điền vào đây

        // 3. Tăng đồng hồ
        timeCurrentQueue++;
        currentTime++;
    }
}