#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

struct Process
{
    int processID;          // ID của tiến trình
    int arrivalTime;        // Thời gian đến
    int burstTime;          // Thời gian xử lý
    int remainingBurstTime; // Thời gian xử lý còn lại
    int finishTime;         // Thời gian hoàn thành
    int waitingTime;        // Thời gian chờ
    int turnaroundTime;     // Thời gian quay vòng
};

queue<Process> Queue;


void RoundRobin(queue<Process> &queue, int timeQuantum, int &currentTime)
{
    cout << "Processing High Priority Queue (Round Robin):\n";
    while (!queue.empty())
    {
        Process currentProcess = queue.front();
        queue.pop();

        if (currentTime < currentProcess.arrivalTime)
        {
            currentTime = currentProcess.arrivalTime; // Chờ tiến trình đến
        }

        while (currentProcess.remainingBurstTime > 0)
        {
            int executionTime = min(timeQuantum, currentProcess.remainingBurstTime);
            cout << "Process " << currentProcess.processID
                 << " runs from " << currentTime
                 << " to " << currentTime + executionTime << endl;

            currentTime += executionTime;
            currentProcess.remainingBurstTime -= executionTime;

            // Kiểm tra nếu hàng đợi rỗng hoặc tiến trình tiếp theo chưa đến
            if (!queue.empty() && currentTime >= queue.front().arrivalTime)
            {
                break; // Kết thúc vòng lặp nếu có tiến trình khác sẵn sàng
            }
        }

        if (currentProcess.remainingBurstTime > 0)
        {
            queue.push(currentProcess); // Tiến trình chưa hoàn thành, đẩy lại hàng đợi
        }
        else
        {
            currentProcess.finishTime = currentTime;
            currentProcess.turnaroundTime = currentProcess.finishTime - currentProcess.arrivalTime;
            currentProcess.waitingTime = currentProcess.turnaroundTime - currentProcess.burstTime;
            cout << "Process " << currentProcess.processID
                 << " finished at " << currentProcess.finishTime << endl;
        }
    }
    cout << endl;
}



int main()
{
    vector<Process> processes = {
        {1, 0, 6, 6}, 
        {2, 1, 8, 8}, 
        {3, 2, 7, 7}, 
        {4, 3, 3, 3}, 
       
    };
    // in ra các tiến trình
    for (auto &process : processes)
    {
        cout << "Process: " << process.processID
             << " start: " << process.arrivalTime
             << " burst: " << process.burstTime
             << "\n";
    }

    // Phân loại tiến trình vào các hàng đợi
    for (auto &process : processes)
    {
        Queue.push(process);
    }

    int currentTime = 0;

    // Thực hiện xử lý lần lượt các hàng đợi
    RoundRobin(Queue, 4, currentTime); // Quantum = 4
}
