#include <stdio.h>
#include <time.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if.h>

#include "network.h"
#include "audio.h"

#define SLEEP_TIME 100000

int main(void)
{
  audio result;
  time_t now;
  char day_of_week[4];

  while (1)
  {
    const struct tm * timeinfo;

    networking();

    GetAlsaMasterVolume(&result);

    printf(" | %.0f%% ", result.percentage);

    if (!result.muted)
    {
      fputs("MUTED ", stdout);
    }

    time(&now);
    timeinfo = localtime(&now);

    strftime(day_of_week, sizeof(day_of_week), "%a", timeinfo);

    printf("| %02d:%02d %s %02d/%02d/%02d\n", timeinfo->tm_hour, timeinfo->tm_min, day_of_week, timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year % 100);

    fflush(stdout);
    usleep(SLEEP_TIME);
  }

  return EXIT_SUCCESS;
}
