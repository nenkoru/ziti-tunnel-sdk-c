#ifndef ZITI_TUNNELER_SDK_UTUN_H
#define ZITI_TUNNELER_SDK_UTUN_H

#include <net/if.h>
#include "nf/netif_driver.h"

struct netif_handle_s {
    int  fd;
    char name[IFNAMSIZ];
};

extern netif_driver utun_open(char *error, size_t error_len);

#endif //ZITI_TUNNELER_SDK_UTUN_H
