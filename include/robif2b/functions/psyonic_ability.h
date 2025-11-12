//
#ifndef ROBIF2B_FUNCTIONS_PSYONIC_ABILITY_H
#define ROBIF2B_FUNCTIONS_PSYONIC_ABILITY_H

#include <robif2b/types/psyonic_ability.h>


#ifdef __cplusplus
extern "C" {
#endif

void robif2b_psyonic_ability_configure(struct robif2b_psyonic_ability_nbx *b);

void robif2b_psyonic_ability_start(struct robif2b_psyonic_ability_nbx *b);

void robif2b_psyonic_ability_update(struct robif2b_psyonic_ability_nbx *b);

void robif2b_psyonic_ability_shutdown(struct robif2b_psyonic_ability_nbx *b);

#ifdef __cplusplus
}
#endif

#endif // ROBIF2B_FUNCTIONS_PSYONIC_ABILITY_H
