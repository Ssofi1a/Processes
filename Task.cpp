#include <iostream>
#include <unistd.h>
#include <cerrno>
#include <sys/wait.h>

int main() {
    int pipe_fd[2];
    pid_t child1_pid, child2_pid;
    
    if (pipe(pipe_fd) != 0) {
        std::cout << "Can't create a pipe.\n";
        exit(errno);
    }

    child1_pid = fork();
    if (child1_pid == -1) {
        std::cout << "Can't create a first child process.\n";
        exit(errno);
    } else if (child1_pid == 0) {
        close(pipe_fd[0]);
        pid_t pid1 = getpid();
        write(pipe_fd[1], &pid1, sizeof(pid1));
        close(pipe_fd[1]);
        exit(0);
    }
    std::cout << "First child pid is: " << child1_pid << std::endl;

    child2_pid = fork();
    if (child2_pid == -1) {
       std::cout << "Can't create a second child process.\n";
       exit(errno);
    } else if (child2_pid == 0) {
        close(pipe_fd[1]);
        pid_t pid2 = getpid();
        write(pipe_fd[1], &pid2, sizeof(pid2));
        close(pipe_fd[1]);
        exit(0);
    }
    std::cout << "Second child pid is: " << child2_pid << std::endl;
    waitpid(child1_pid, nullptr, 0);
    waitpid(child2_pid, nullptr, 0);
    std::cout << "Fisrt child pid is: " << child2_pid << std::endl;
    std::cout << "Second child pid is: " << child1_pid << std::endl;
    return 0;
}
