#include "audio.h"
#include "network.h"

#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SLEEP_TIME_NS 100000000

int main(void)
{
    struct timespec ts = {0, SLEEP_TIME_NS};
    const char *locale = setlocale(LC_ALL, "");

    if (locale == NULL)
    {
        fputs("Warning: cannot set user locale; falling back to \"C\"", stderr);

        if (errno != 0)
            fprintf(stderr, ": %s", strerror(errno));

        fputc('\n', stderr);

        locale = setlocale(LC_ALL, "C");

        if (locale == NULL)
        {
            fputs("Error: cannot set fallback locale \"C\"\n", stderr);
            return EXIT_FAILURE;
        }
    }

    while (1)
    {
        char day_of_week[32];
        time_t now = time(NULL);

        if (now == (time_t)-1)
        {
            perror("time() failed");
            return EXIT_FAILURE;
        }

        const struct tm *timeinfo = localtime(&now);

        if (!timeinfo)
        {
            perror("localtime() failed");
            return EXIT_FAILURE;
        }

        if (strftime(day_of_week, sizeof(day_of_week), "%a", timeinfo) == 0)
        {
            fputs("strftime() returned 0\n", stderr);
            return EXIT_FAILURE;
        }

        get_current_network_name();
        get_alsa_master_volume();

        printf("| %02d:%02d %s %02d/%02d/%02d\n", timeinfo->tm_hour, timeinfo->tm_min, day_of_week, timeinfo->tm_mday,
               timeinfo->tm_mon + 1, timeinfo->tm_year % 100);

        if (fflush(stdout) == EOF)
        {
            perror("fflush");
            return EXIT_FAILURE;
        }

        while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
            ;
    }

    return EXIT_SUCCESS;
}
