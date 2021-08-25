/*
Copyright 2019 Netfoundry, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

https://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "model/dtos.h"
#include <ziti/ziti_log.h>
#include "ziti/sys/queue.h"
#include <time.h>

struct tnl_identity_s {
    tunnel_identity *id;
    LIST_ENTRY(tnl_identity_s) _next;
};

static LIST_HEAD(tnl_identities, tnl_identity_s) tnl_identity_list = LIST_HEAD_INITIALIZER(&tnl_identity_list);

tunnel_identity *get_tunnel_identity(char* identifier) {
    struct tnl_identity_s *tnl_id;
    LIST_FOREACH(tnl_id, &tnl_identity_list, _next) {
        if (strcmp(identifier, tnl_id->id->Identifier) == 0) break;
    }
    if (tnl_id != NULL) {
        return tnl_id->id;
    } else {
        tnl_id = malloc(sizeof(struct tnl_identity_s));
        tnl_id->id = calloc(1, sizeof(struct tunnel_identity_s));
        tnl_id->id->Identifier = identifier;
        LIST_INSERT_HEAD(&tnl_identity_list, tnl_id, _next);
        return tnl_id->id;
    }
}

tunnel_identity *find_tunnel_identity(char* identifier) {
    struct tnl_identity_s *tnl_id;
    LIST_FOREACH(tnl_id, &tnl_identity_list, _next) {
        if (strcmp(identifier, tnl_id->id->Identifier) == 0) break;
    }
    if (tnl_id != NULL) {
        return tnl_id->id;
    } else {
        ZITI_LOG(WARN, "ztx[%s] is not found. It may not be active/connected", identifier);
        return NULL;
    }
}

void set_mfa_status(char* identifier, bool mfa_enabled, bool mfa_needed) {
    tunnel_identity *tnl_id = find_tunnel_identity(identifier);
    if (tnl_id != NULL) {
        tnl_id->MfaEnabled = mfa_enabled;
        tnl_id->MfaNeeded = mfa_needed;
        ZITI_LOG(DEBUG, "ztx[%s] mfa enabled : %d, mfa needed : %d ", identifier, mfa_enabled, mfa_needed);
    }
}

void update_mfa_time(char* identifier) {
    tunnel_identity *tnl_id = find_tunnel_identity(identifier);
    if (tnl_id != NULL) {
        uv_timeval64_t now;
        uv_gettimeofday(&now);

        if (tnl_id->MfaLastUpdatedTime == NULL) {
            tnl_id->MfaLastUpdatedTime = malloc(sizeof(timestamp));
        }
        tnl_id->MfaLastUpdatedTime->tv_sec = now.tv_sec;
        tnl_id->MfaLastUpdatedTime->tv_usec = now.tv_usec;
    }
}

// ************** TUNNEL BROADCAST MESSAGES
IMPL_MODEL(tunnel_identity, TUNNEL_IDENTITY)
IMPL_MODEL(tunnel_config, TUNNEL_CONFIG)
IMPL_MODEL(tunnel_metrics, TUNNEL_METRICS)
IMPL_MODEL(tunnel_address, TUNNEL_ADDRESS)
IMPL_MODEL(tunnel_port_range, TUNNEL_PORT_RANGE)
IMPL_MODEL(tunnel_posture_check, TUNNEL_POSTURE_CHECK)
IMPL_MODEL(tunnel_service, TUNNEL_SERVICE)

