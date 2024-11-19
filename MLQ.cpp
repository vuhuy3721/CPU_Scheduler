#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

struct Process {
    int processID;           // ID của tiến trình
    int arrivalTime;         // Thời gian đến
    int burstTime;           // Thời gian xử lý
    int remainingBurstTime;  // Thời gian xử lý còn lại
    int priorityLevel;       // Độ ưu tiên (1: cao, 2: trung bình, 3: thấp)
    int finishTime;          // Thời gian hoàn thành
    int waitingTime;         // Thời gian chờ
    int turnaroundTime;      // Thời gian quay vòng
};

// Hàng đợi tiến trình
queue<Process> highPriorityQueue;       // Hàng đợi 1: Round Robin
queue<Process> mediumPriorityQueue;     // Hàng đợi 2: FCFS
vector<Process> lowPriorityQueue;       // Hàng đợi 3: SJF

void RoundRobin(queue<Process>& queue, int timeQuantum, int& currentTime) {
    cout << "Processing High Priority Queue (Round Robin):\n";
    while (!queue.empty()) {
        Process currentProcess = queue.front();
        queue.pop();

        if (currentTime < currentProcess.arrivalTime) {
            currentTime = currentProcess.arrivalTime; // Chờ tiến trình đến
        }

        while (currentProcess.remainingBurstTime > 0) {
            int executionTime = min(timeQuantum, currentProcess.remainingBurstTime);
            cout << "Process " << currentProcess.processID
                 << " runs from " << currentTime
                 << " to " << currentTime + executionTime << endl;

            currentTime += executionTime;
            currentProcess.remainingBurstTime -= executionTime;

            // Kiểm tra nếu hàng đợi rỗng hoặc tiến trình tiếp theo chưa đến
            if (!queue.empty() && currentTime >= queue.front().arrivalTime) {
                break; // Kết thúc vòng lặp nếu có tiến trình khác sẵn sàng
            }
        }

        if (currentProcess.remainingBurstTime > 0) {
            queue.push(currentProcess); // Tiến trình chưa hoàn thành, đẩy lại hàng đợi
        } else {
            currentProcess.finishTime = currentTime;
            currentProcess.turnaroundTime = currentProcess.finishTime - currentProcess.arrivalTime;
            currentProcess.waitingTime = currentProcess.turnaroundTime - currentProcess.burstTime;
            cout << "Process " << currentProcess.processID
                 << " finished at " << currentProcess.finishTime << endl;
        }
    }
    cout << endl;
}


// Hàm thực hiện FCFS cho hàng đợi 2
void FirstComeFirstServe(queue<Process>& queue, int& currentTime) {
    cout << "Processing Medium Priority Queue (FCFS):\n";
    while (!queue.empty()) {
        Process currentProcess = queue.front();
        queue.pop();

        if (currentTime < currentProcess.arrivalTime) {
            currentTime = currentProcess.arrivalTime; // Chờ tiến trình đến
        }

        cout << "Process " << currentProcess.processID 
             << " starts at " << currentTime 
             << " and finishes at " << currentTime + currentProcess.burstTime << endl;

        currentTime += currentProcess.burstTime;
        currentProcess.finishTime = currentTime;
        currentProcess.turnaroundTime = currentProcess.finishTime - currentProcess.arrivalTime;
        currentProcess.waitingTime = currentProcess.turnaroundTime - currentProcess.burstTime;
    }
    cout << endl;
}

// Hàm thực hiện SJF cho hàng đợi 3
void ShortestJobFirst(vector<Process>& queue, int& currentTime) {
    cout << "Processing Low Priority Queue (SJF):\n";
    sort(queue.begin(), queue.end(), [](const Process& a, const Process& b) {
        return a.burstTime < b.burstTime;
    });

    for (auto& currentProcess : queue) {
        if (currentTime < currentProcess.arrivalTime) {
            currentTime = currentProcess.arrivalTime; // Chờ tiến trình đến
        }

        cout << "Process " << currentProcess.processID 
             << " starts at " << currentTime 
             << " and finishes at " << currentTime + currentProcess.burstTime << endl;

        currentTime += currentProcess.burstTime;
        currentProcess.finishTime = currentTime;
        currentProcess.turnaroundTime = currentProcess.finishTime - currentProcess.arrivalTime;
        currentProcess.waitingTime = currentProcess.turnaroundTime - currentProcess.burstTime;
    }
    cout << endl;
}

int main() {
    vector<Process> processes = {
        {1, 0, 6, 6, 1}, // Tiến trình 1, ưu tiên cao
        {2, 2, 8, 8, 2}, // Tiến trình 2, ưu tiên trung bình
        {3, 4, 7, 7, 3}, // Tiến trình 3, ưu tiên thấp
        {4, 6, 3, 3, 1}, // Tiến trình 4, ưu tiên cao
        {5, 8, 5, 5, 2}  // Tiến trình 5, ưu tiên trung bình
    };
    // in ra các tiến trình 
    for (auto& process : processes) {
        cout << "Process: " << process.processID 
            << " start: " << process.arrivalTime
            << " burst: " << process.burstTime 
            << " priority: " << process.priorityLevel
            << "\n";
    }

    // Phân loại tiến trình vào các hàng đợi
    for (auto& process : processes) {
        if (process.priorityLevel == 1) {
            highPriorityQueue.push(process);
        } else if (process.priorityLevel == 2) {
            mediumPriorityQueue.push(process);
        } else if (process.priorityLevel == 3) {
            lowPriorityQueue.push_back(process);
        }
    }

    int currentTime = 0;

    // Thực hiện xử lý lần lượt các hàng đợi
    RoundRobin(highPriorityQueue, 4, currentTime); // Quantum = 4
    FirstComeFirstServe(mediumPriorityQueue, currentTime);
    ShortestJobFirst(lowPriorityQueue, currentTime);

    return 0;
}
