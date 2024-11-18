#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Process {
    char name;
    int ArrivalTime, BurstTime, WaitingTime, TurnaroundTime, RemainingTime, CompletionTime;
};

vector<Process> Queue1, Queue2, Queue3;

void sortByArrival(vector<Process>& queue) {
    sort(queue.begin(), queue.end(), [](const Process& a, const Process& b) {
        return a.ArrivalTime < b.ArrivalTime;
    });
}

int main() {
    int n, tq1 = 5, tq2 = 8, time = 0, k = 0, r = 0, flag = 0;
    char c;

    cout << "Enter the number of processes: ";
    cin >> n;

    // Nhập thông tin các process cho Queue1
    for (int i = 0; i < n; i++) {
        Process p;
        p.name = 'A' + i;
        cout << "\nEnter the arrival time and burst time of process " << p.name << ": ";
        cin >> p.ArrivalTime >> p.BurstTime;
        p.RemainingTime = p.BurstTime;
        Queue1.push_back(p);
    }

    // Sắp xếp các process theo ArrivalTime
    sortByArrival(Queue1);

    // Xử lý Queue1 (Round Robin với tq = 5)
    time = Queue1[0].ArrivalTime;
    cout << "Processes in first queue (RR with qt = 5):\n";
    cout << "Process\tRemainingTime\tWaitingTime\tTurnaroundTime\n";

    for (size_t i = 0; i < Queue1.size(); i++) {
        if (Queue1[i].RemainingTime <= tq1) {
            time += Queue1[i].RemainingTime;
            Queue1[i].RemainingTime = 0;
            Queue1[i].CompletionTime = time;
            Queue1[i].TurnaroundTime = Queue1[i].CompletionTime - Queue1[i].ArrivalTime;
            Queue1[i].WaitingTime = Queue1[i].TurnaroundTime - Queue1[i].BurstTime;
            cout << Queue1[i].name << "\t" << Queue1[i].BurstTime << "\t\t" 
                 << Queue1[i].WaitingTime << "\t\t" << Queue1[i].TurnaroundTime << "\t\t"
                 << Queue1[i].CompletionTime <<"\n";
        } else {
            Process temp = Queue1[i];
            time += tq1;
            temp.RemainingTime -= tq1;
            temp.WaitingTime = time;
            Queue2.push_back(temp);
            flag = 1;
        }
    }

    // Xử lý Queue2 (Round Robin với tq = 8)
    if (flag == 1) {
        cout << "\nProcesses in second queue (RR with qt = 8):\n";
        cout << "Process\tRemainingTime\tWaitingTime\tTurnaroundTime\tCompletionTime\n";

        for (size_t i = 0; i < Queue2.size(); i++) {
            if (Queue2[i].RemainingTime <= tq2) {
                time += Queue2[i].RemainingTime;
                Queue2[i].RemainingTime = 0;
                Queue2[i].CompletionTime = time;
                Queue2[i].TurnaroundTime = Queue2[i].CompletionTime - Queue2[i].ArrivalTime;
                Queue2[i].WaitingTime = Queue2[i].TurnaroundTime - Queue2[i].BurstTime;
                cout << Queue2[i].name << "\t" << Queue2[i].BurstTime << "\t\t" 
                     << Queue2[i].WaitingTime << "\t\t" << Queue2[i].TurnaroundTime << "\t\t"
                     << Queue2[i].CompletionTime << "\n";
            } else {
                Process temp = Queue2[i];
                time += tq2;
                temp.RemainingTime -= tq2;
                Queue3.push_back(temp);
                flag = 2;
            }
        }
    }

    // Xử lý Queue3 (FCFS)
    if (flag == 2) {
        cout << "\nProcesses in third queue (FCFS):\n";
        cout << "Process\tBurstTime\tWaitingTime\tTurnaroundTime\tCompletionTime\n";

        for (size_t i = 0; i < Queue3.size(); i++) {
            if (i == 0)
                Queue3[i].CompletionTime = time + Queue3[i].RemainingTime;
            else
                Queue3[i].CompletionTime = Queue3[i - 1].CompletionTime + Queue3[i].RemainingTime;

            Queue3[i].TurnaroundTime = Queue3[i].CompletionTime - Queue3[i].ArrivalTime;
            Queue3[i].WaitingTime = Queue3[i].TurnaroundTime - Queue3[i].BurstTime;

            cout << Queue3[i].name << "\t" << Queue3[i].BurstTime << "\t\t" 
                 << Queue3[i].WaitingTime << "\t\t" << Queue3[i].TurnaroundTime << "\t\t"
                 << Queue3[i].CompletionTime << "\n";
        }
    }

    return 0;
}
