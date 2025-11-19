#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#include <robif2b/functions/psyonic_ability.h>

int main(void) {

    bool success = false;

    struct robif2b_psyonic_ability_nbx hand = {
        .conf.hand_addr = 0x50,
        .conf.baud_rate = 921600,
        .conf.port      = "",

        .success = &success,

        0,
    };

    robif2b_psyonic_ability_configure(&hand);
    if (!success) {
        printf("Error during Ability configure\n");
        goto shutdown;
    }

    robif2b_psyonic_ability_start(&hand);
    if (!success) {
        printf("Error during Ability configure\n");
        goto shutdown;
    }

    float cmd[6] = {30.0, 30.0, 30.0, 30.0, 30.0, -30.0};
    struct robif2b_ability_comm comm = {
        .cmd = *cmd,
        .ctrl_mode = ROBIF2B_CTRL_MODE_POSITION,
    };
    hand.comm = &comm;

    struct timespec ts_start;
    clock_gettime(CLOCK_MONOTONIC, &ts_start);
    double start_time = ts_start.tv_sec + ts_start.tv_nsec * 1e-9;

    for (size_t i = 0; i < 100000; i++) {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        double now = ts.tv_sec + ts.tv_nsec * 1e-9;
        double t = now - start_time;
        for (size_t j = 0; j < 6; j++) {
            double ft = t * 3.0 + j * (2.0 * 3.14159265359 / 12.0);
            comm.cmd[j] = (0.5 * sin(ft) + 0.5) * 45.0 +15.0;
        }
        comm.cmd[5] = -comm.cmd[5];

        robif2b_psyonic_ability_update(&hand);
        if (!success) {
            printf("Error during Ability update\n");
            goto shutdown;
        }
    }

shutdown:
    robif2b_psyonic_ability_shutdown(&hand);
    return 0;
}
