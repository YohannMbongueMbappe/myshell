#include <stdio.h>

int main() {
    char buffer[1024];

    while (1) {
        printf("myshell> ");
        fflush(stdout);

        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strcmp(buffer, "exit") == 0) {
            break;
    }

    return 0;
}