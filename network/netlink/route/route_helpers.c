#include "route_helpers.h"

void show_ndmsg(struct ndmsg* msg){
    printf("--- Print ndmsg ---\n");
    printf("ndm_familiy: %d\n", msg->ndm_family);
    printf("ndm_ifindex: %d : Interface index\n", msg->ndm_ifindex);
    printf("ndm_state: ");
    switch (msg->ndm_state){
    case NUD_INCOMPLETE:
        printf("NUD_INCOMPLETE\n");
        break;
    case NUD_REACHABLE:
        printf("NUD_REACHABLE\n");
        break;
    case NUD_STALE:
        printf("NUD_STALE\n");
        break;
    case NUD_DELAY:
        printf("NUD_DELAY\n");
        break;
    case NUD_PROBE:
        printf("NUD_PROBE\n");
        break;
    case NUD_FAILED:
        printf("NUD_FAILED\n");
        break;
    case NUD_NOARP:
        printf("NUD_NOARP\n");
        break;
    case NUD_PERMANENT:
        printf("NUD_PERMANENT\n");
        break;
    default:
        break;
    }
    printf("ndm_flags: ");
    switch (msg->ndm_flags){
    case NTF_PROXY:
        printf("NTF_PROXY\n");
        break;
    case NTF_ROUTER:
        printf("NTF_ROUTER\n");
        break;
    default:
        break;
    }
    printf("ndm_type: %d\n", msg->ndm_type);
    printf("--- /Print ndmsg ---\n");
}