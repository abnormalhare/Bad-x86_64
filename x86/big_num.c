#include "data.h"

void u128_set(u128 *dest, u128 *src) {
    u128_set64(dest, src->high, src->low);
}

void u128_set64(u128 *dest, u64 high, u64 low) {
    dest->high = high;
    dest->low = low;
}

// void u256_set(u256 *dest, u256 *src) {
//     u256_set64(dest, src->high.high, src->high.low, src->low.high, src->low.low);
// }

// void u256_set128(u256 *dest, u128 *high, u128 *low) {
//     u256_set64(dest, high->high, high->low, low->high, low->low);
// }

// void u256_set64(u256 *dest, u64 hh, u64 hl, u64 lh, u64 ll) {
//     dest->high.high = hh;
//     dest->high.low  = hl;
//     dest->low.high  = lh;
//     dest->low.low   = ll;
// }