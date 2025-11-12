#include <cassert>
#include <array>
#include <cstring>
#include <optional>

#include <robif2b/functions/psyonic_ability.h>
#include "wrapper.h"


static std::optional<AHWrapper> robif2b_ah_wrapper;

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
    robif2b_ah_wrapper.emplace(b->conf.hand_addr, b->conf.baud_rate);
    *b->success = true;
}

void robif2b_psyonic_ability_start(struct robif2b_psyonic_ability_nbx *b) {
    assert(robif2b_ah_wrapper);

    robif2b_ah_wrapper->connect(b->conf.port);

    *b->success = true;
}

void robif2b_psyonic_ability_update(struct robif2b_psyonic_ability_nbx *b) {
    assert(robif2b_ah_wrapper);
    assert(b->comm);

    // wrap command for ah_wrapper
    std::array<float, 6> cmd {};
    std::memcpy(cmd.data(), b->comm->cmd, 6*sizeof(float));
    const Command cmd_type { robif2b_to_ability_type(b->comm->ctrl_mode) };

    int ret = robif2b_ah_wrapper->read_write_once(cmd, cmd_type, 0);
    // stop on failed read/write
    if (ret != 0) {
        *b->success = false;
        return;
    }

    // copy feedback from ah_wrapper hand into robif2b struct
    std::memcpy(b->pos, robif2b_ah_wrapper->hand.pos.data(), 6*sizeof(float));
    std::memcpy(b->vel, robif2b_ah_wrapper->hand.vel.data(), 6*sizeof(float));
    std::memcpy(b->cur, robif2b_ah_wrapper->hand.cur.data(), 6*sizeof(float));
    std::memcpy(b->fsr, robif2b_ah_wrapper->hand.fsr.data(), 30*sizeof(uint16_t));

    *b->success = true;
}

void robif2b_psyonic_ability_shutdown(struct robif2b_psyonic_ability_nbx *b) {
    assert(robif2b_ah_wrapper);

    robif2b_ah_wrapper.reset();
}
