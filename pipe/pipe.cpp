#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

int main()
{
    int fd[2]; // Array para armazenar descritores de pipe (escrita e leitura)
    
    // 0 = leitura      1 = escrita
    
    pid_t childpid;
    
    if(pipe(fd) == -1) // Pipe passa array fd como parâmetro
    { // Verificação para pipe se criado corretamente
        std::cerr << "Erro ao criar pipe\n";
        return 1;
    }
    
    childpid = fork();
    if(childpid < 0)
    {
        std::cerr <<"Erro no fork\n";
        return 1;
    } 
    if(childpid == 0)
    {
        close(fd[1]); // Fecha extremidade de escrita, apenas lê
        char buffer[100];
        read(fd[0], buffer, sizeof(buffer)); //lê o pipe
        std::cout << "Filho recebeu: " << buffer << "\n" << std::endl;
        close(fd[0]);
    }
    else{
        close(fd[0]);
        const char* msg = "COE RAPAZIADAAAAA";
        write(fd[1], msg, strlen(msg) + 1);
        close(fd[1]);
    }
    return 0;
}