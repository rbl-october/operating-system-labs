#include "algorithms.cpp"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Loi cu phap! Dung: MSSV1_MSSV2.exe <INPUT_FILE> <OUTPUT_FILE>\n";
        return 1;
    }

    Scheduler scheduler;

    // Chuỗi hành động cốt lõi
    scheduler.loadInput(argv[1]);    // Đọc file
    scheduler.run();                 // Chạy mô phỏng (RR, SJF, SRTN)
    scheduler.calculateStatistics(); // Tính toán số liệu
    scheduler.exportOutput(argv[2]); // Ghi ra file

    return 0;
}