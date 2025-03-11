#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <signal.h>

volatile sig_atomic_t stop = 0;

void handle_signal(int sig) {
    stop = 1;
}

int main(int argc, char **argv) {
    struct winsize ws;

    // Install signal handlers for termination signals
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGHUP, &sa, NULL);

    if (isatty(STDIN_FILENO)) {
        // Wait until a valid TTY size is available or termination is requested
        while (!stop) {
            if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) != -1 && ws.ws_row > 0 && ws.ws_col > 0) {
                break;
            }
            usleep(100000);  // sleep 100ms
        }
        if (stop) {
            fprintf(stderr, "Terminated while waiting for TTY size\n");
            return EXIT_FAILURE;
        }
    }

    execv("/usr/bin/htop", argv);
    perror("execv failed");
    return EXIT_FAILURE;
}
