#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main(){
    const int nChildren = 24;
    pid_t pid;
    
    for (int i = 0; i < nChildren ; i++){
        pid = fork();
        
        if (pid < 0){
            cerr << "Erro ao criar filho" << endl;
            exit(1);
        }
        else if (pid == 0){
            srand(time(NULL)+getpid()); // faz seed diferente mesmo se dois programas terminam ao mesmo tempo, imita realismo apenas
            int sleep_time = rand() % 10 + 1;
            sleep(sleep_time);
            exit(sleep_time);
        }
    }
    
    int status;
    pid_t terminated_pid;
    
    while ((terminated_pid = wait(&status)) > 0) {
        if (WIFEXITED(status)){
           cout << "No processo pai: processo filho: " << terminated_pid << ", status de saída do filho: " << WEXITSTATUS(status) << endl;
        }
    }
    
    return 0;
}

