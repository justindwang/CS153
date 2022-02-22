
#include "types.h"
#include "user.h"

int main(int argc, char *argv[])
{
    int PScheduler(void);
    int donateTest(void);
    printf(1, "\n This program tests the correctness of your lab#2\n");
    if (atoi(argv[1]) == 1)
        PScheduler();
    else if (atoi(argv[1]) == 2)
        donateTest();
    else 
        printf(1, "\ntype \"lab_2 1\" to test scheduler and \"lab_2 2\" to test priority donation\n");
   // End of test
    return 0;
}


int PScheduler(void){
    // use this part to test the priority scheduler. Assuming that the priorities range between range between 0 to 31
    // 0 is the highest priority and 31 is the lowest priority.
    int pid1;
    int pid;
    int priority;
    int j,k;
    

    printf(1, "\n  Testing the priority scheduler and setPriority(int priority) system call:\n");
    printf(1, "\n  Assuming that the priorities range between range between 0 to 31\n");
    printf(1, "\n  0 is the highest priority. All processes have a default priority of 10\n");
    printf(1, "\n  The parent processes will switch to priority 0\n");
    setpriority(1);

    pid1 = fork();

    // first child process
    if (pid1 == 0){
        priority = 30;
        setpriority(priority);
        printf(1, "\n child# %d has priority %d before starting its work", getpid(), priority);

        pid = fork();

        // second child process
        if(pid == 0){
            priority = 20;
            setpriority(priority);
            printf(1, "\n child# %d has priority %d before starting its work", getpid(), priority);

            pid = fork();

            // third child process
            if(pid == 0){
                priority = 10;
                setpriority(priority);
                printf(1, "\n child# %d has priority %d before starting its work\n", getpid(), priority);
            }
        }
    }

    // if one of child processes
    if( pid1 == 0) {
        for (j=0;j<50000;j++) {
            for(k=0;k<10000;k++) {
                asm("nop");
            }
        }
		printf(1, "\n child# %d has priority %d after finishing its work", getpid(), getpriority());
        printf(1, "\n child# %d with original priority %d has finished! \n", getpid(), priority);
        wait(0);
        exit(0);   
    }
    
    
    printf(1,"\n if processes with highest priority finished first then its correct \n");
    wait(0);
    exit(0);
}

int donateTest(void){
    int pid;
    int ppid = getpid();
    setpriority(30);
    printf(1, "\n This is parent# %d with starting priority %d", ppid, 30);
    pid = fork();
    if( pid == 0){
        setpriority(2);
        printf(1, "\n This is child# %d with starting priority %d before donation", getpid(), 2);
        printf(1, "\n Calling donateprio() to donate my priority to my parent..");
        donateprio(ppid);
        printf(1, "\n This is child# %d with starting priority %d and I finished donating", getpid(), 2);
        printf(1, "\n This is child# %d with current priority %d due to aging", getpid(), getpriority());
        exit(0);
    }
    wait(0);
    printf(1, "\n This is parent# %d with current priority %d after receiving a donation and running for a bit", ppid, getpriority());
    exit(0);
}

