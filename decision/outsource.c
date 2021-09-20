#include "outsource.h"

/**
 * Send arguments to the external decision module.
 */
void send_to_predictor(const char* fmt, ...)
{
    char buffer[512];

    va_list va;
    va_start(va, fmt);
    int count = vsnprintf(buffer, sizeof(buffer) - 1, fmt, va);
    va_end(va);

    if (count < 0) {
        perror("LAGO: Failed to vsnprintf.");
        abort();
    }
    else {
        buffer[count++] = '\n';
        buffer[count] = '\0';
        int written = write(predictor_input_pipe, buffer, count);
        if (written == -1) {
            perror("LAGO: Failed to write to predictor.");
            abort();
        }
        else if (written != count) {
            fprintf(stderr, "LAGO: Count mismatch during send_to_predictor.\n");
            abort();
        }
    }
}

/**
 * Receive output external decision module.
 */
void recv_from_predictor(const char* fmt, ...)
{
    int count = 0;
    char buffer[512];

    while (count == 0 || buffer[count-1] != '\n') {
        int result = read(predictor_output_pipe, buffer, sizeof(buffer) - count);
        if (result <= 0) {
            perror("LAGO: Failed to read from predictor pipe.\n");
            abort();
        }
        count += result;
        assert(count < (int) sizeof(buffer) - 1);
    }

    va_list va;
    va_start(va, fmt);
    vsscanf(buffer, fmt, va);
    va_end(va);
}

/**
 * Spawn external decision module.
 */
int spawn_predictor(const char* command)
{
    int inpipefd[2] = {-1, -1};
    int outpipefd[2] = {-1, -1};

    if (pipe(inpipefd) == -1 || pipe(outpipefd) == -1) {
        perror("LAGO: Failed to create predictor pipes.");
        return 0;
    }

    int pid = fork();
    if (pid == -1) {
        perror("LAGO: Failed to fork predictor.");
        close(inpipefd[0]);
        close(inpipefd[1]);
        close(outpipefd[0]);
        close(outpipefd[1]);
        return 0;
    }
    else if (pid == 0) {
        dup2(outpipefd[0], STDIN_FILENO);
        dup2(inpipefd[1], STDOUT_FILENO);

        close(outpipefd[1]);
        close(inpipefd[0]);

        // receive SIGTERM once the parent process dies
        prctl(PR_SET_PDEATHSIG, SIGTERM);

        // execute command to executue Python script
        execl("/bin/sh", "sh", "-c", command, NULL);
        perror("LAGO: execl failed.");
        return 0;
    }
    else {
        close(outpipefd[0]);
        close(inpipefd[1]);
        predictor_pid = pid;
        predictor_input_pipe = outpipefd[1];
        predictor_output_pipe = inpipefd[0];
        return 1;
    }
}
