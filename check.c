#include <unistd.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

struct passwd* get_pw() {
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid); 
    if (!pw) {
        perror("getpwuid");
        exit(1);
    }
    return pw;
}

int main() {
    int pfd[2];
    if (pipe(pfd) < 0) {
        perror("pipe");
        exit(1);
    }
    pid_t pid = fork();
    if (pid < -1) {
        perror("fork");
    } else if (pid == 0) {
        dup2(pfd[1], STDOUT_FILENO);
        close(pfd[0]);
        close(pfd[1]);

        struct passwd *pw = get_pw();
        char filter[1024];
        snprintf(filter, sizeof(filter), "name=%s", pw->pw_name);

        char *argv[] = {"docker", "ps", "-aq", "--filter", filter, 0};
        execvp(argv[0], argv);
        perror("execvp");
        exit(1);
    }
    close(pfd[1]);
    char buf[1024];
    int nread = read(pfd[0], buf, 1024);
    wait(NULL);
    if (nread == 0) {
        pid = fork();
        if (pid < - 1) {
            perror("fork");
        } else if (pid == 0) {
            struct passwd *pw = get_pw();
            char mount_dir[1024];
            snprintf(mount_dir, sizeof(mount_dir), "/tmp/%s:/tmp/test:rw", pw->pw_name);
            puts(mount_dir);
            char uid[16];
            snprintf(uid, sizeof(uid), "%d", geteuid());
            char *argv[] = {"docker", "run", "--name", pw->pw_name, "-dit", "-v", mount_dir, "--restart", "always", "ubuntu_socat", uid, 0};
            execvp(argv[0], argv);
            perror("execvp");
            exit(1);
        }
        int status;
        wait(&status);
        if (status != 0) {
            printf("container creation failed");
            exit(1);
        }
    }
}
