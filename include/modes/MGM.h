#pragma once
#include <funcs.h>

typedef struct {
    block_t *data;
    size_t bit_size;
    size_t alloc_len;
} bit_vec_t;

bit_vec_t make_bit_vec(size_t size);

void delete_bit_vec(bit_vec_t *bit_vec);

void bit_vec_append(bit_vec_t *bit_vec, block_t data, size_t size);

bit_vec_t bit_vec_copy(bit_vec_t bit_vec);

typedef struct {
    bit_vec_t additional_data;
    bit_vec_t cyphered_text;
    bit_vec_t MAC;
} MGM_result;

MGM_result MGM_Encrypt(key256_t key, block_t nonce, bit_vec_t additional_data, bit_vec_t plain_text, size_t MAC_size);

