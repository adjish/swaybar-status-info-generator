#include <ifaddrs.h>
#include <net/if.h>

extern inline void get_current_network_name(void)
{
    struct ifaddrs *ifaddr;

    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (const struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;

        if (ifa->ifa_addr->sa_family == AF_INET && !(ifa->ifa_flags & IFF_LOOPBACK))
        {
            fputs(ifa->ifa_name, stdout);
            freeifaddrs(ifaddr);
            return;
        }
    }

    freeifaddrs(ifaddr);

    fputs("Disconnected", stdout);
}
