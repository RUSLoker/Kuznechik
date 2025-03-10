#pragma once
#include <funcs.h>
#include <stdint.h>

typedef struct {
    block_t *data;
    uint64_t bit_size;
    uint64_t alloc_len;
} bit_vec_t;

typedef struct {
    bit_vec_t additional_data;
    bit_vec_t cyphered_text;
    bit_vec_t MAC;
} MGM_result;

#ifdef __cplusplus
extern "C" {
#endif

bit_vec_t make_bit_vec(uint64_t size);

void delete_bit_vec(bit_vec_t *bit_vec);

void bit_vec_append(bit_vec_t *bit_vec, block_t data, uint64_t size);

bit_vec_t bit_vec_copy(bit_vec_t bit_vec);

MGM_result MGM_Encrypt(key256_t key, block_t nonce, bit_vec_t additional_data, bit_vec_t plain_text, uint64_t MAC_size);

#ifdef __cplusplus
}
#endif
