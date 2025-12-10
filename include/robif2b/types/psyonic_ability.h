//
#ifndef ROBIF2B_TYPES_PSYONIC_ABILITY_H
#define ROBIF2B_TYPES_PSYONIC_ABILITY_H

#include <stdbool.h>
#include <stdint.h>

#include <robif2b/types/control_mode.h>


#ifdef __cplusplus
extern "C" {
#endif

#define ROBIF2B_ABILITY_NR_JOINTS 6
#define ROBIF2B_ABILITY_NR_FSR 30

struct robif2b_psyonic_ability_config
{
    const uint8_t hand_addr;
    const uint32_t baud_rate;
    const char *port;
};

struct robif2b_ability_cmd {
    float *cmd; // * NR_JOINTS
    enum robif2b_ctrl_mode *ctrl_mode;
    uint8_t *reply_mode;
};

struct robif2b_ah_wrapper;

struct robif2b_psyonic_ability_nbx
{
    struct robif2b_psyonic_ability_config conf;

    struct robif2b_ability_cmd *cmd;
    float *pos;    // * NR_JOINTS
    float *vel;    // * NR_JOINTS
    float *cur;    // * NR_JOINTS
    uint16_t *fsr; // * NR_FSR
    bool *success;

    // Internal
    struct robif2b_ah_wrapper *ah_wrapper;
};


#ifdef __cplusplus
}
#endif

#endif // ROBIF2B_TYPES_PSYONIC_ABILITY_H
