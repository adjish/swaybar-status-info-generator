#include "audio.h"
#include "network.h"

#define SLEEP_TIME 100000

int main(void)
{
    time_t now;
    char day_of_week[4];

    while (1)
    {
        const struct tm *timeinfo;

        get_current_network_name();

        get_alsa_master_volume();

        time(&now);
        timeinfo = localtime(&now);

        if (strftime(day_of_week, sizeof(day_of_week), "%a", timeinfo) == 0)
        {
            fprintf(stderr, "strftime returned 0");
            exit(EXIT_FAILURE);
        }

        printf("| %02d:%02d %s %02d/%02d/%02d\n", timeinfo->tm_hour, timeinfo->tm_min, day_of_week, timeinfo->tm_mday,
               timeinfo->tm_mon + 1, timeinfo->tm_year % 100);

        if (fflush(stdout) == EOF)
        {
            fprintf(stderr, "flush error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        usleep(SLEEP_TIME);
    }

    return EXIT_SUCCESS;
}
