#include <sys/mman.h>   // Para mmap() e memória compartilhada
#include <sys/stat.h>   // Para constantes como O_CREAT
#include <fcntl.h>      // Para shm_open()
#include <unistd.h>     // Para fork(), close(), ftruncate()
#include <iostream>     // Para std::cout e std::cerr
#include <cstring>      // Para strcpy()
#include <sys/wait.h>   // Para wait()

int main()
{
    const char* shm_name = "/meu_shm"; // Identificador da memória
    int shm_fd; // Descritor de arquivo, identificador do objeto de memória compartilhada
    char* shared_memory; // Ponteiro para a região de memória compartilhada que será mapeada
    
    shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666); // Cria/Abre memória compartilhada
    if (shm_fd == -1)
    {
        std::cerr << "Erro ao criar memória compartilhada\n";
        return 1;
    }
    
    ftruncate(shm_fd, 1024); // Define tamanho do objeto de memória compartilhada para 1024 bytes
    
    // mmap(): Mapeia o objeto de memória compartilhada no espaço de endereçamento do processo, 
    //        retornando um ponteiro (shared_memory) que podemos usar como um array.
    shared_memory = (char*)mmap(0, 1024, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shared_memory == MAP_FAILED)
    {
        std::cerr << "Erro ao mapear memória\n";
        return 1;
    }
    
    pid_t childpid = fork();
    if (childpid < 0)
    {
        std::cerr << "Erro ao criar filho\n";
        return 1;
    }
    if (childpid == 0)
    {
        std::cout << "Filho leu: " << shared_memory << "\n";
    }
    else
    {
        const char* msg = "Olá SUAS VADIAS SUJAS HAHAHHHA\n";
        strcpy(shared_memory, msg);
        wait(NULL);
    }
    
    // Limpeza
    munmap(shared_memory, 1024);
    close(shm_fd);
    shm_unlink(shm_name); // Remove objeto da memória compartilhada
    return 0;
}

