#include <cassert>
#include <array>
#include <cstring>
#include <optional>

#include <robif2b/functions/psyonic_ability.h>
#include "wrapper.h"


#define CMD_SIZE 6*sizeof(float)
#define FSR_SIZE 30*sizeof(uint16_t)


struct robif2b_ah_wrapper {
    AHWrapper impl;
};

Command robif2b_to_ability_type(enum robif2b_ctrl_mode ctrl_mode) {
    switch (ctrl_mode) {
    case ROBIF2B_CTRL_MODE_POSITION:
        return POSITION;
    case ROBIF2B_CTRL_MODE_VELOCITY:
        return VELOCITY;
    case ROBIF2B_CTRL_MODE_FORCE:
        return DUTY;
    case ROBIF2B_CTRL_MODE_CURRENT:
        return CURRENT;
    default:
        assert(false);
    }
}

void robif2b_psyonic_ability_configure(struct robif2b_psyonic_ability_nbx *b) {
    b->ah_wrapper = new robif2b_ah_wrapper {
        .impl = AHWrapper(b->conf.hand_addr, b->conf.baud_rate)
    };
    *b->success = true;
}

void robif2b_psyonic_ability_start(struct robif2b_psyonic_ability_nbx *b) {
    assert(b->ah_wrapper);

    int ret = b->ah_wrapper->impl.connect(b->conf.port);

    *b->success = ret == 0;
}

void robif2b_psyonic_ability_update(struct robif2b_psyonic_ability_nbx *b) {
    assert(b->ah_wrapper);
    assert(b->comm);

    // wrap command for ah_wrapper
    std::array<float, 6> cmd {};
    std::memcpy(cmd.data(), b->comm->cmd, CMD_SIZE);
    const Command cmd_type { robif2b_to_ability_type(b->comm->ctrl_mode) };

    int ret = b->ah_wrapper->impl.read_write_once(cmd, cmd_type, 0);
    // stop on failed read/write
    if (ret != 0) {
        *b->success = false;
        return;
    }

    // copy feedback from ah_wrapper hand into robif2b struct
    std::memcpy(b->pos, b->ah_wrapper->impl.hand.pos.data(), CMD_SIZE);
    std::memcpy(b->vel, b->ah_wrapper->impl.hand.vel.data(), CMD_SIZE);
    std::memcpy(b->cur, b->ah_wrapper->impl.hand.cur.data(), CMD_SIZE);
    std::memcpy(b->fsr, b->ah_wrapper->impl.hand.fsr.data(), FSR_SIZE);

    *b->success = true;
}

void robif2b_psyonic_ability_shutdown(struct robif2b_psyonic_ability_nbx *b) {
    assert(b->ah_wrapper);

    delete b->ah_wrapper;
}
