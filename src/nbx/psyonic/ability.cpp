#include <cassert>
#include <array>
#include <cstring>
#include <optional>

#include <robif2b/functions/psyonic_ability.h>
#include "wrapper.h"



#define CMD_SIZE ROBIF2B_ABILITY_NR_JOINTS*sizeof(float)
#define FSR_SIZE ROBIF2B_ABILITY_NR_FSR*sizeof(uint16_t)


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

void refresh_hand_pointers(struct robif2b_psyonic_ability_nbx *b) {
    b->pos = b->ah_wrapper->impl.hand.pos.data();
    b->vel = b->ah_wrapper->impl.hand.vel.data();
    b->cur = b->ah_wrapper->impl.hand.cur.data();
    b->fsr = b->ah_wrapper->impl.hand.fsr.data();
}

void robif2b_psyonic_ability_configure(struct robif2b_psyonic_ability_nbx *b) {
    b->ah_wrapper = new robif2b_ah_wrapper {
        .impl = AHWrapper(b->conf.hand_addr, b->conf.baud_rate)
    };
    *b->success = true;
}

void robif2b_psyonic_ability_start(struct robif2b_psyonic_ability_nbx *b) {
    assert("Ability Hand has not been configured." && b->ah_wrapper);

    int ret = b->ah_wrapper->impl.connect(b->conf.port);
    refresh_hand_pointers(b);

    *b->success = ret == 0;
}

void robif2b_psyonic_ability_update(struct robif2b_psyonic_ability_nbx *b) {
    assert("Ability Hand has not been configured." && b->ah_wrapper);
    assert("Command is not initialized." && b->cmd);

    std::array<float, ROBIF2B_ABILITY_NR_JOINTS> cmd{};
    std::memcpy(cmd.data(), b->cmd->cmd, CMD_SIZE);
    const Command cmd_type { robif2b_to_ability_type(*b->cmd->ctrl_mode) };
    int ret = b->ah_wrapper->impl.read_write_once(cmd, cmd_type, *b->cmd->reply_mode);
    // stop on failed read/write
    if (ret != 0) {
        *b->success = false;
        return;
    }

    *b->success = true;
}

void robif2b_psyonic_ability_shutdown(struct robif2b_psyonic_ability_nbx *b) {
    assert("Ability Hand has not been configured." && b->ah_wrapper);
    assert("Command is not initialized." && b->cmd);

    delete b->ah_wrapper;

    *b->success = true;
}
