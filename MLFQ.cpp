#include <stdio.h>
#include <iostream>

using namespace std;
struct process {
    char name; // Tên tiến trình
    int arrivalTime, burstTime, waitingTime, turnAroundTime, remainingTime, completionTime;
} Q1[10], Q2[10], Q3[10]; // Ba hàng đợi

int n; // Số lượng tiến trình

// Hàm sắp xếp tiến trình theo thời gian đến
void sortByArrival() {
    struct process temp;
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (Q1[i].arrivalTime > Q1[j].arrivalTime) {
                temp = Q1[i];
                Q1[i] = Q1[j];
                Q1[j] = temp;
            }
        }
    }
}

int main() {
    int i, j, k = 0, r = 0, time = 0, tq1 = 5, tq2 = 8, flag = 0;
    char c;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (i = 0, c = 'A'; i < n; i++, c++) {
        Q1[i].name = c;
        printf("\nEnter the arrival time and burst time of process %c: ", Q1[i].name);
        scanf("%d%d", &Q1[i].arrivalTime, &Q1[i].burstTime);
        Q1[i].remainingTime = Q1[i].burstTime; // Khởi tạo thời gian còn lại bằng burst time
    }

    sortByArrival();
    time = Q1[0].arrivalTime;
    
    printf("\nProcesses in Queue 1 (RR with quantum = 5):\n");
    printf("Process\tQueue\t\tRemaining Time\tWaiting Time\tTurnaround Time\n");

    for (i = 0; i < n; i++) {
        if (Q1[i].remainingTime <= tq1) { // Nếu tiến trình hoàn tất trong Queue 1
            time += Q1[i].remainingTime;
            Q1[i].remainingTime = 0;
            Q1[i].waitingTime = time - Q1[i].arrivalTime - Q1[i].burstTime;
            Q1[i].turnAroundTime = time - Q1[i].arrivalTime;
            printf("%c\tQueue 1\t\t%d\t\t%d\t\t%d\n", Q1[i].name, Q1[i].burstTime, Q1[i].waitingTime, Q1[i].turnAroundTime);
        } else { // Nếu tiến trình chưa hoàn tất, chuyển sang Queue 2
            Q2[k].waitingTime = time;
            time += tq1;
            Q1[i].remainingTime -= tq1;
            Q2[k].burstTime = Q1[i].remainingTime;
            Q2[k].remainingTime = Q2[k].burstTime;
            Q2[k].name = Q1[i].name;
            printf("%c\tQueue 1\t\t%d\t\t-\t\t-\n", Q1[i].name, Q1[i].burstTime);
            k++;
            flag = 1;
        }
    }

    if (flag == 1) {
        printf("\nProcesses in Queue 2 (RR with quantum = 8):\n");
        printf("Process\tQueue\t\tRemaining Time\tWaiting Time\tTurnaround Time\n");
    }

    for (i = 0; i < k; i++) {
        if (Q2[i].remainingTime <= tq2) { // Nếu tiến trình hoàn tất trong Queue 2
            time += Q2[i].remainingTime;
            Q2[i].remainingTime = 0;
            Q2[i].waitingTime = time - Q1[i].arrivalTime - Q2[i].burstTime;
            Q2[i].turnAroundTime = time - Q1[i].arrivalTime;
            printf("%c\tQueue 2\t\t%d\t\t%d\t\t%d\n", Q2[i].name, Q2[i].burstTime, Q2[i].waitingTime, Q2[i].turnAroundTime);
        } else { // Nếu tiến trình chưa hoàn tất, chuyển sang Queue 3
            Q3[r].arrivalTime = time;
            time += tq2;
            Q2[i].remainingTime -= tq2;
            Q3[r].burstTime = Q2[i].remainingTime;
            Q3[r].remainingTime = Q3[r].burstTime;
            Q3[r].name = Q2[i].name;
            printf("%c\tQueue 2\t\t%d\t\t-\t\t-\n", Q2[i].name, Q2[i].burstTime);
            r++;
            flag = 2;
        }
    }
    cout<< flag;
    if (flag == 2) {
        printf("\nProcesses in Queue 3 (FCFS):\n");
        printf("Process\tQueue\t\tRemaining Time\tWaiting Time\tTurnaround Time\n");
    }

    for (i = 0; i < r; i++) {
        if (i == 0) {
            Q3[i].completionTime = Q3[i].burstTime + time - tq1 - tq2;
        } else {
            Q3[i].completionTime = Q3[i - 1].completionTime + Q3[i].burstTime;
        }
        Q3[i].turnAroundTime = Q3[i].completionTime;
        Q3[i].waitingTime = Q3[i].turnAroundTime - Q3[i].burstTime;
        printf("%c\tQueue 3\t\t%d\t\t%d\t\t%d\n", Q3[i].name, Q3[i].burstTime, Q3[i].waitingTime, Q3[i].turnAroundTime);
    }

    return 0;
}
