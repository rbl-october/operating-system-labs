#include "file-io.cpp"

// Dành riêng cho SJF & SRTN

Process *Scheduler::selectSJF(CPUQueue &currentQ, Process *prevProcess)
{
    if (prevProcess != nullptr && prevProcess->queueID == currentQ.qid && !prevProcess->isCompleted)
    {
        return prevProcess;
    }

    Process *selected = nullptr;
    int minBurst = 1e6;
    for (Process *p : currentQ.processes)
    {
        if (!p->isCompleted && p->burstTime < minBurst)
        {
            minBurst = p->burstTime;
            selected = p;
        }
    }

    return selected;
}

Process *Scheduler::selectSRTN(CPUQueue &currentQ)
{
    Process *selected = nullptr;
    int minRemaining = 1e6;

    for (Process *p : currentQ.processes)
    {
        if (!p->isCompleted && p->remainingTime < minRemaining)
        {
            minRemaining = p->remainingTime;
            selected = p;
        }
    }

    return selected;
}

void Scheduler::run()
{
    // Bước 1: Khởi tạo

    // Sắp xếp các tiến trình theo thứ tự thời gian đến tăng dần
    stable_sort(allProcesses.begin(), allProcesses.end(), [](Process *a, Process *b)
                { return a->arrivalTime < b->arrivalTime; });

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
        // 2. Điều phối Round-robin & 2 thuật toán
        CPUQueue *currentQ = &queues[qIndex];
        Process *selectedProcess = nullptr;

        if (currentQ->policy == Policy::SJF)
        {
            selectedProcess = selectSJF(*currentQ, prevProcess);
        }
        else
        {
            selectedProcess = selectSRTN(*currentQ);
        }

        if (selectedProcess == nullptr || timeCurrentQueue >= currentQ->timeSlice)
        {

            // Duyệt vòng quanh các hàng đợi để tìm hàng đợi có việc để làm
            for (int i = 0; i < queues.size(); i++)
            {
                qIndex = (qIndex + 1) % queues.size(); // Chuyển sang hàng đợi kế tiếp
                currentQ = &queues[qIndex];
                timeCurrentQueue = 0; // Reset lại bộ đếm thời gian cho hàng đợi mới

                // Thử chọn tiến trình ở hàng đợi mới
                if (currentQ->policy == Policy::SJF)
                {
                    selectedProcess = selectSJF(*currentQ, prevProcess);
                }
                else
                {
                    selectedProcess = selectSRTN(*currentQ);
                }

                // Nếu tìm thấy tiến trình để chạy, thoát khỏi vòng lặp tìm kiếm
                if (selectedProcess != nullptr)
                {
                    break;
                }
            }
        }

        // Ghi nhận biểu đồ Gantt
        // Nếu tiến trình hiện tại khác với giây trước đó (xảy ra chuyển ngữ cảnh)
        if (selectedProcess != prevProcess)
        {

            // 1. Chốt thời gian kết thúc cho tiến trình cũ (nếu có)
            if (prevProcess != nullptr && !ganttChart.empty())
            {
                ganttChart.back().end = currentTime;
            }

            // 2. Mở một entry mới cho tiến trình vừa được chọn (nếu CPU không rảnh rỗi)
            if (selectedProcess != nullptr)
            {
                GanttEntry newEntry;
                newEntry.start = currentTime;
                newEntry.end = currentTime + 1; // Sẽ được cập nhật đè lên ở lần chuyển ngữ cảnh tiếp theo
                newEntry.qid = currentQ->qid;
                newEntry.pid = selectedProcess->id;
                ganttChart.push_back(newEntry);
            }
        }

        if (selectedProcess != nullptr)
        {
            selectedProcess->remainingTime--;

            // Nếu tiến trình vừa chạy xong ở giây này
            if (selectedProcess->remainingTime == 0)
            {
                selectedProcess->isCompleted = true;
                selectedProcess->completionTime = currentTime + 1;
                completedProcess++;
            }
        }

        // Cập nhật vết tiến trình cho nhịp đồng hồ tiếp theo
        prevProcess = selectedProcess;
        // 3. Tăng đồng hồ
        timeCurrentQueue++;
        currentTime++;
    }
}