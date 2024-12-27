#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <climits>
using namespace std;

struct Process {
    int pid;            // Process ID
    int arrivalTime;    // Arrival time
    int burstTime;      // CPU burst time
    int remainingTime;  // Remaining time for SRT and RR
    int finishTime;     // Completion time
    int waitingTime;    // Waiting time
    int turnaroundTime; // Turnaround time
};

// Function to simulate First-Come, First-Served (FCFS) scheduling
void FCFS_Scheduling(vector<Process>& processes) {
    int currentTime = 0;
    int totalIdleTime = 0;
    double cpuUtilization;

    for (auto& process : processes) {
        if (currentTime < process.arrivalTime) {
            // CPU is idle until the next process arrives
            totalIdleTime += (process.arrivalTime - currentTime);
            currentTime = process.arrivalTime;
        }
        process.finishTime = currentTime + process.burstTime;
        process.turnaroundTime = process.finishTime - process.arrivalTime;
        process.waitingTime = process.turnaroundTime - process.burstTime;
        currentTime = process.finishTime;
    }

    cpuUtilization = (static_cast<double>(currentTime - totalIdleTime) / currentTime) * 100;

    // Display results
    cout << "\nFirst-Come, First-Served (FCFS) Scheduling:\n";
    cout << "Process\tArrival\tBurst\tFinish\tWaiting\tTurnaround\n";
    for (const auto& process : processes) {
        cout << "P" << process.pid << "\t" << process.arrivalTime << "\t"
            << process.burstTime << "\t" << process.finishTime << "\t"
            << process.waitingTime << "\t" << process.turnaroundTime << "\n";
    }
    cout << "CPU Utilization: " << fixed << setprecision(2) << cpuUtilization << "%\n";
}

// Function to simulate Shortest Remaining Time (SRT) scheduling
void SRT_Scheduling(vector<Process>& processes) {
    int n = processes.size();
    int currentTime = 0, completed = 0;
    int totalIdleTime = 0;
    double cpuUtilization;

    while (completed < n) {
        // Find process with the shortest remaining time among arrived processes
        int idx = -1;
        int minRemainingTime = INT_MAX;
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) {
                if (processes[i].remainingTime < minRemainingTime) {
                    minRemainingTime = processes[i].remainingTime;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            // No process is ready, CPU is idle
            currentTime++;
            totalIdleTime++;
        }
        else {
            // Process the selected process for 1 unit of time
            processes[idx].remainingTime--;
            currentTime++;

            if (processes[idx].remainingTime == 0) {
                // Process completed
                processes[idx].finishTime = currentTime;
                processes[idx].turnaroundTime = processes[idx].finishTime - processes[idx].arrivalTime;
                processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
                completed++;
            }
        }
    }

    cpuUtilization = (static_cast<double>(currentTime - totalIdleTime) / currentTime) * 100;

    // Display results
    cout << "\nShortest Remaining Time (SRT) Scheduling:\n";
    cout << "Process\tArrival\tBurst\tFinish\tWaiting\tTurnaround\n";
    for (const auto& process : processes) {
        cout << "P" << process.pid << "\t" << process.arrivalTime << "\t"
            << process.burstTime << "\t" << process.finishTime << "\t"
            << process.waitingTime << "\t" << process.turnaroundTime << "\n";
    }
    cout << "CPU Utilization: " << fixed << setprecision(2) << cpuUtilization << "%\n";
}

// Function to simulate Round-Robin (RR) scheduling
void RR_Scheduling(vector<Process>& processes, int timeQuantum) {
    int n = processes.size();
    int currentTime = 0, completed = 0;
    int totalIdleTime = 0;
    double cpuUtilization;

    queue<int> readyQueue;

    // Enqueue the first arriving processes
    for (int i = 0; i < n; ++i) {
        if (processes[i].arrivalTime == currentTime) {
            readyQueue.push(i);
        }
    }

    while (!readyQueue.empty() || completed < n) {
        if (readyQueue.empty()) {
            // No process is ready, CPU is idle
            currentTime++;
            totalIdleTime++;
            for (int i = 0; i < n; ++i) {
                if (processes[i].arrivalTime == currentTime) {
                    readyQueue.push(i);
                }
            }
            continue;
        }

        int idx = readyQueue.front();
        readyQueue.pop();

        // Process the selected process for up to the time quantum
        int timeToProcess = min(timeQuantum, processes[idx].remainingTime);
        processes[idx].remainingTime -= timeToProcess;
        currentTime += timeToProcess;

        // Enqueue newly arrived processes
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrivalTime > currentTime - timeToProcess &&
                processes[i].arrivalTime <= currentTime) {
                readyQueue.push(i);
            }
        }

        if (processes[idx].remainingTime > 0) {
            // Process is not yet complete, re-enqueue it
            readyQueue.push(idx);
        }
        else {
            // Process completed
            processes[idx].finishTime = currentTime;
            processes[idx].turnaroundTime = processes[idx].finishTime - processes[idx].arrivalTime;
            processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
            completed++;
        }
    }

    cpuUtilization = (static_cast<double>(currentTime - totalIdleTime) / currentTime) * 100;

    // Display results
    cout << "\nRound-Robin (RR) Scheduling (Q = " << timeQuantum << "):\n";
    cout << "Process\tArrival\tBurst\tFinish\tWaiting\tTurnaround\n";
    for (const auto& process : processes) {
        cout << "P" << process.pid << "\t" << process.arrivalTime << "\t"
            << process.burstTime << "\t" << process.finishTime << "\t"
            << process.waitingTime << "\t" << process.turnaroundTime << "\n";
    }
    cout << "CPU Utilization: " << fixed << setprecision(2) << cpuUtilization << "%\n";
}

int main() {
    int n, choice, timeQuantum;

    // Input number of processes
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    // Input process details
    for (int i = 0; i < n; ++i) {
        processes[i].pid = i + 1;
        cout << "Enter arrival time for Process P" << processes[i].pid << ": ";
        cin >> processes[i].arrivalTime;
        cout << "Enter burst time for Process P" << processes[i].pid << ": ";
        cin >> processes[i].burstTime;
        processes[i].remainingTime = processes[i].burstTime; // Set remaining time for RR and SRT
    }

    // Menu to choose scheduling algorithm
    cout << "Choose the scheduling algorithm:\n";
    cout << "1. FCFS\n2. Round-Robin (RR)\n3. Shortest Remaining Time (SRT)\n";
    cin >> choice;

    switch (choice) {
    case 1:
        FCFS_Scheduling(processes);
        break;
    case 2:
        cout << "Enter time quantum for Round-Robin (RR): ";
        cin >> timeQuantum;
        RR_Scheduling(processes, timeQuantum);
        break;
    case 3:
        SRT_Scheduling(processes);
        break;
    default:
        cout << "Invalid choice!" << endl;
    }

    return 0;
}
