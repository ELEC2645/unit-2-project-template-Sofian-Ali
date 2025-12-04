#ifndef INPUT_GUARD_H
#define INPUT_GUARD_H

/* Get a validated number from the user.
 * - out_value: where the result goes
 * - allow_zero: 1 = zero allowed, 0 = zero not allowed (e.g. divisors)
 * Returns 0 on success.
 */

int get_valid_number(double *out_value, int allow_zero);

void wait_for_enter(void);

#endif