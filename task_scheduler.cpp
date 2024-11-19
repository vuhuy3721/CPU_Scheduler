#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

// Cấu trúc mô tả mỗi tiến trình
struct Process {
    int id;           // ID của tiến trình
    int burstTime;    // Thời gian xử lý (CPU Burst)
    int arrivalTime;  // Thời gian đến
    int priority;     // Độ ưu tiên (dùng cho Priority Scheduling)
    int remainingTime; // Thời gian còn lại
    int turnaroundTime; // Thời gian quay vòng
    int waitingTime;    // Thời gian chờ
    int completionTime; // Thời gian hoàn thành
};

// Hàm sắp xếp các tiến trình theo FCFS
void FCFS_Scheduler(vector<Process> processes) {
    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
        return a.arrivalTime < b.arrivalTime;
    });

    cout << "FCFS Scheduling:\n";
    int currentTime = 0;
    int totalWaitingTime = 0;
    for (const auto& p : processes) {

        cout << "Process " << p.id << " starts at " << currentTime << " and finishes at " << currentTime + p.burstTime << endl;
        currentTime += p.burstTime;
        int turnaroundTime = currentTime - p.arrivalTime;
        int waitingTime = max(0, turnaroundTime - p.burstTime);
        totalWaitingTime += waitingTime;
    }
    cout << "Average Waiting Time: " << (double)totalWaitingTime / processes.size() << endl << endl;
}

// Hàm sắp xếp các tiến trình theo SJF
void SJF_Scheduler(vector<Process> processes) {
    cout << "SJF Scheduling:\n";
    int currentTime = 0;
    int totalWaitingTime = 0;
    vector<Process> readyQueue;
    
    while (!processes.empty() || !readyQueue.empty()) {
        for (auto it = processes.begin(); it != processes.end(); ) {
            if (it->arrivalTime <= currentTime) {
                readyQueue.push_back(*it);
                it = processes.erase(it);
            } else {
                ++it;
            }
        }

        if (!readyQueue.empty()) {
            sort(readyQueue.begin(), readyQueue.end(), [](const Process &a, const Process &b) {
                return a.burstTime < b.burstTime;
            });

            Process p = readyQueue.front();
            readyQueue.erase(readyQueue.begin());
            int waitingTime = max(0, currentTime - p.arrivalTime);
            totalWaitingTime += waitingTime;
            cout << "Process " << p.id << " starts at " << currentTime << " and finishes at " << currentTime + p.burstTime << endl;
            currentTime += p.burstTime;
        } else {
            currentTime++;
        }
    }
    cout << "Average Waiting Time: " << (double)totalWaitingTime / processes.size() << endl << endl;
}
void RoundRobin_Scheduler(queue<Process>& queue, int timeQuantum) {
    int currentTime = 0;
    cout << "Processing High Priority Queue (Round Robin):\n";
    while (!queue.empty()) {
        Process currentProcess = queue.front();
        queue.pop();

        if (currentTime < currentProcess.arrivalTime) {
            currentTime = currentProcess.arrivalTime; // Chờ tiến trình đến
        }

        while (currentProcess.remainingTime > 0) {
            int executionTime = min(timeQuantum, currentProcess.remainingTime);
            cout << "Process " << currentProcess.id
                 << " runs from " << currentTime
                 << " to " << currentTime + executionTime << endl;

            currentTime += executionTime;
            currentProcess.remainingTime -= executionTime;

            // Kiểm tra nếu hàng đợi rỗng hoặc tiến trình tiếp theo chưa đến
            if (!queue.empty() && currentTime >= queue.front().arrivalTime) {
                break; // Kết thúc vòng lặp nếu có tiến trình khác sẵn sàng
            }
        }

        if (currentProcess.remainingTime > 0) {
            queue.push(currentProcess); // Tiến trình chưa hoàn thành, đẩy lại hàng đợi
        } else {
            currentProcess.completionTime = currentTime;
            currentProcess.turnaroundTime = currentProcess.completionTime - currentProcess.arrivalTime;
            currentProcess.waitingTime = currentProcess.turnaroundTime - currentProcess.burstTime;
            cout << "Process " << currentProcess.id
                 << " finished at " << currentProcess.completionTime << endl;
        }
    }
    cout << endl;

    // Calculate and display averages
    //cout << "Average Waiting Time: " << totalWaitingTime / processes.size() << endl;
    //cout << "Average Turnaround Time: " << totalTurnaroundTime / processes.size() << endl << endl;
}

// Hàm sắp xếp các tiến trình theo Priority Scheduling
void Priority_Scheduler(vector<Process> processes) {
    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
        return a.priority < b.priority;
    });

    cout << "Priority Scheduling:\n";
    int currentTime = 0;
    int totalWaitingTime = 0;
    for (const auto& p : processes) {
        int waitingTime = max(0, currentTime - p.arrivalTime);
        totalWaitingTime += waitingTime;
        cout << "Process " << p.id << " starts at " << currentTime << " and finishes at " << currentTime + p.burstTime << " (Priority " << p.priority << ")\n";
        currentTime += p.burstTime;
    }
    cout << "Average Waiting Time: " << (double)totalWaitingTime / processes.size() << endl << endl;
}
// Hàm sắp xếp các tiến trình theo Shortest Remaining Time First (SRTF)
void SRTF_Scheduler(vector<Process> processes) {
    cout << "SRTF Scheduling:\n";
    int currentTime = 0;
    int totalWaitingTime = 0;
    int completedProcesses = 0;
    vector<Process> readyQueue;
    int n = processes.size();
    vector<int> waitingTimes(n, 0);

    // Copy burst time into remaining time for each process
    for (auto& p : processes) {
        p.remainingTime = p.burstTime;
    }

    while (completedProcesses < n) {
        for (auto it = processes.begin(); it != processes.end(); ) {
            if (it->arrivalTime <= currentTime) {
                readyQueue.push_back(*it);
                it = processes.erase(it);
            } else {
                ++it;
            }
        }

        if (!readyQueue.empty()) {
            sort(readyQueue.begin(), readyQueue.end(), [](const Process &a, const Process &b) {
                return a.remainingTime < b.remainingTime;
            });

            Process &p = readyQueue.front();
            int execTime = 1;
            cout << "Process " << p.id << " runs from " << currentTime << " to " << currentTime + execTime << endl;

            p.remainingTime -= execTime;
            if (p.remainingTime == 0) {
                completedProcesses++;
                totalWaitingTime += currentTime + execTime - p.arrivalTime - p.burstTime;
                readyQueue.erase(readyQueue.begin());
            }

            currentTime += execTime;
        } else {
            currentTime++;
        }
    }
    cout << "Average Waiting Time: " << (double)totalWaitingTime / n << endl << endl;
}

int main() {
    vector<Process> processes = {
        {1, 6, 0, 1},  // ID: 1, Burst Time: 6, Arrival Time: 0, Priority: 2
        {2, 8, 1, 2},  // ID: 2, Burst Time: 8, Arrival Time: 1, Priority: 1
        {3, 7, 2, 4},  // ID: 3, Burst Time: 7, Arrival Time: 2, Priority: 4
        {4, 3, 3, 3}   // ID: 4, Burst Time: 3, Arrival Time: 3, Priority: 3
    };
    cout << "Number Processes:" << processes.size() << endl;
    cout << "ProcessID\tBurstTime\tArrivalTime\tPriority\n";
    for (int i = 0; i < processes.size(); i++)
    {
        cout << processes[i].id << "\t\t" << processes[i].burstTime << "\t\t" << processes[i].arrivalTime << "\t\t" << processes[i].priority << endl;
    }
    
    FCFS_Scheduler(processes);              // First-Come, First-Served
    SJF_Scheduler(processes);               // Shortest Job First
    Priority_Scheduler(processes);          // Priority Scheduling
    SRTF_Scheduler(processes);              // Shortest Remaining Time First
    return 0;
}
