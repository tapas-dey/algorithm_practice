#include <vector>
#include <iostream>
using namespace std;

void calculateWaitingTime(vector<int>& wait_time, 
                          const vector<int>& burst_time,
                          int num_process, 
                          int time_quanta){
    wait_time.resize(num_process, 0);                        
    std::vector<int> copy_burst_time;
    copy_burst_time.resize(burst_time.size(), 0);
    std::copy(burst_time.begin(), burst_time.end(), copy_burst_time.begin());
    //start the clock
    int cur_time = 0;
    while(1){
        bool all_process_done = true;
        for (int ii = 0; ii < num_process; ++ii){
            //Check if the process still needs some time execute
            int rem_time = copy_burst_time[ii];
            if (rem_time > 0){
                all_process_done = false; // Found a process that needs some time allocation
                //The process needs some more time with CPU
                if (rem_time > time_quanta){
                    //we can't give the process full time it needs
                    //we can only allocate it the time quanta amount of time
                    cur_time += time_quanta;//Move the current time since the process will take up that time
                    rem_time -= time_quanta;
                    copy_burst_time[ii] = rem_time;
                }
                else{
                    //Let process finish its task
                    cur_time = cur_time + rem_time;
                    //Let's figure out how much time the process has waited
                    wait_time[ii] = cur_time - burst_time[ii];
                    //no time is needed by the process
                    copy_burst_time[ii] = 0;
                }
            }
        }
        // Check if all processes are done
		if (all_process_done == true) 
		    break;
    }                        

}

int main(){
    //Ref: 
    //1. https://www.studytonight.com/cpp-programs/cpp-program-for-round-robin-scheduling-algorithm
    //2. https://www.geeksforgeeks.org/program-for-round-robin-scheduling-for-the-same-arrival-time/
    //In round robin algorithm for OS, each program gets a fixed amount of CPU time and after that time expires
    //each process has to go in waiting mode for next turn.
    //Burst time: The total time a process needs to finish its task.
    //Waiting time: The total time a process has to wait due to time share
    //Turn around time: The total time a process takes to finish its task given the CPU is doing round robin algorithm
    //                  to decide on context switching and task execution
    // Time quanta: Time allocated for each process to continue its execution at a stretch

    //Assume we've three process
    int num_process = 3;
    //The first process needs 10 time units, second process takes 5 time units etc.
    vector<int> burst_time {10, 5, 8}; 
    //Assume CPU has 2 units as Time qunata
    int time_quanta = 2;
    vector<int> wait_time;
    calculateWaitingTime(wait_time, 
                         burst_time,
                         num_process, 
                         time_quanta);
    //Find turn around time which is burst_time + wait_time
    vector<int> turn_around_time;
    turn_around_time.resize(num_process, 0);
    for (int ii = 0; ii < num_process; ++ii){
        turn_around_time[ii] = wait_time[ii] + burst_time[ii];
    }
    //Now we can find average waiting time and average turn around time
    int total_waiting_time = 0;
    int total_turn_around_time = 0;
    cout << "Process ID || Burst Time || Wait Time || Turn Around Time" << endl; 
    for (int ii = 0; ii < num_process; ++ii){
        total_waiting_time += wait_time[ii];
        total_turn_around_time += turn_around_time[ii];
        cout << ii << " || " << burst_time[ii] <<" || " << wait_time[ii] << " || " << turn_around_time[ii] << endl;
    }
    double avg_waiting_time = (double)total_waiting_time/num_process;
    double avg_turn_around_time = (double)total_turn_around_time/ num_process;
    cout << "Average waiting time : " << avg_waiting_time << endl;
    cout <<  "Average turn around time : " << avg_turn_around_time << endl;
}