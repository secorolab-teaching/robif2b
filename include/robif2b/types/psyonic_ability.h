//
#ifndef ROBIF2B_TYPES_PSYONIC_ABILITY_H
#define ROBIF2B_TYPES_PSYONIC_ABILITY_H

#include <stdbool.h>
#include <stdint.h>

#include <robif2b/types/control_mode.h>


#ifdef __cplusplus
extern "C" {
#endif

struct robif2b_psyonic_ability_config
{
    const uint8_t hand_addr;
    const uint32_t baud_rate;
    const char* port;
};

struct robif2b_ability_comm {
    float cmd[6];
    enum robif2b_ctrl_mode ctrl_mode;
};

struct robif2b_psyonic_ability_nbx
{
    struct robif2b_psyonic_ability_config conf;
    float pos[6];
    float vel[6];
    float cur[6];
    uint16_t fsr[30];
    bool* success;
    struct robif2b_ability_comm *comm;
};


#ifdef __cplusplus
}
#endif

#endif // ROBIF2B_TYPES_PSYONIC_ABILITY_H
