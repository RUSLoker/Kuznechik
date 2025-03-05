#include "modes/MGM.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>


#define BLOCK_BIT_LEN (sizeof(block_t) * 8)

#define MIN(a, b) ((a) < (b) ? (a) : (b))


void MGM_apply_cypher(key256_t key, block_t nonce, bit_vec_t plain_text, bit_vec_t *cyphered_text);

block_t MGM_make_MAC(key256_t key, block_t nonce, bit_vec_t additional_data, bit_vec_t cyphered_text);

block_t incr_l(block_t block);

block_t incr_r(block_t block);

block_t MSB(size_t n, block_t block);

bit_vec_t make_bit_vec(size_t size) {
    if (size == 0) {
        const bit_vec_t result = {malloc(sizeof(block_t)), 0};
        return result;
    }

    const size_t alloc_len = size / BLOCK_BIT_LEN
                             // extra space for the last incomplete block
                             + (size % BLOCK_BIT_LEN != 0);

    const bit_vec_t result = {
        malloc(alloc_len * sizeof(block_t)),
        size,
        alloc_len
    };

    memset(result.data, 0, alloc_len);

    return result;
}

void delete_bit_vec(bit_vec_t *bit_vec) {
    free(bit_vec->data);
    bit_vec->bit_size = 0;
    bit_vec->alloc_len = 0;
    bit_vec->data = NULL;
}

void bit_vec_append(bit_vec_t *bit_vec, const block_t data, const size_t size) {
    const size_t new_size = bit_vec->bit_size + size;
    const size_t new_alloc_len = new_size / BLOCK_BIT_LEN
                                 // extra space for the last incomplete block
                                 + (new_size % BLOCK_BIT_LEN != 0);
    if (bit_vec->alloc_len < new_alloc_len) {
        block_t *new_data = realloc(bit_vec->data, new_alloc_len * sizeof(block_t));
        assert(new_data != NULL);

        bit_vec->data = new_data;
        bit_vec->alloc_len = new_alloc_len;
    }

    // appending data

    // size of the last incomplete block in bit_vec
    const size_t last_incomplete_size = bit_vec->bit_size % BLOCK_BIT_LEN;

    const size_t last_incomplete_idx = bit_vec->bit_size / BLOCK_BIT_LEN;

    block_t shifted_data = data;
    size_t data_remained_size = size;

    if (last_incomplete_size != 0) {
        const size_t last_block_free_size = BLOCK_BIT_LEN - last_incomplete_size;

        // size of the part less than the size of a block need to be taken from data
        const size_t first_incomplete_size = MIN(last_block_free_size, size);

        data_remained_size = size - first_incomplete_size;

        block_t last_block_converted = bit_vec->data[last_incomplete_idx];

        last_block_converted = last_block_converted | (MSB(first_incomplete_size, data) >> last_incomplete_size);

        // append data to the last incomplete byte in bit_vec
        bit_vec->data[last_incomplete_idx] = last_block_converted;

        bit_vec->bit_size += first_incomplete_size;

        shifted_data = data << first_incomplete_size;
    }

    if (data_remained_size > 0) {
        const size_t next_block_idx = bit_vec->bit_size / BLOCK_BIT_LEN;

        bit_vec->data[next_block_idx] = shifted_data;
        bit_vec->bit_size += data_remained_size;
    }
}

bit_vec_t bit_vec_copy(bit_vec_t bit_vec) {
    bit_vec_t result = make_bit_vec(bit_vec.bit_size);
    memcpy_s(result.data, result.alloc_len * sizeof(block_t),
             bit_vec.data, result.alloc_len * sizeof(block_t));
    return result;
}

MGM_result MGM_Encrypt(key256_t key, block_t nonce, bit_vec_t additional_data, bit_vec_t plain_text,
                       size_t MAC_size) {
    MGM_result result = {
        .additional_data = bit_vec_copy(additional_data),
        .cyphered_text = make_bit_vec(plain_text.bit_size),
        .MAC = make_bit_vec(MAC_size)
    };

    MGM_apply_cypher(key, nonce, plain_text, &result.cyphered_text);

    // append additional data and cyphered text to complete blocks by zeros
    result.additional_data.bit_size = ((result.additional_data.bit_size / BLOCK_BIT_LEN)
                                       + (result.additional_data.bit_size % BLOCK_BIT_LEN != 0)) * BLOCK_BIT_LEN;
    result.cyphered_text.bit_size = ((result.cyphered_text.bit_size / BLOCK_BIT_LEN)
                                     + (result.cyphered_text.bit_size % BLOCK_BIT_LEN != 0)) * BLOCK_BIT_LEN;

    result.MAC.data[0] = MSB(MAC_size, MGM_make_MAC(key, nonce, result.additional_data, result.cyphered_text));
}

void MGM_apply_cypher(key256_t key, block_t nonce, bit_vec_t plain_text, bit_vec_t *cyphered_text) {
    block_t Y = encrypt(key, nonce >> 1);
    const block_t *P = plain_text.data;
    block_t *C = cyphered_text->data;
    for (size_t remain_size = plain_text.bit_size; remain_size / BLOCK_BIT_LEN != 0; remain_size -= BLOCK_BIT_LEN) {
        *C = *P ^ encrypt(key, Y);
        Y = incr_r(Y);
        P++;
        C++;
    }

    const size_t incomplete_size = plain_text.bit_size % BLOCK_BIT_LEN;
    if (incomplete_size != 0) {
        *C = *P ^ MSB(incomplete_size, encrypt(key, Y));
    }
}

block_t MGM_make_MAC(key256_t key, block_t nonce, bit_vec_t additional_data, bit_vec_t cyphered_text) {

}


block_t incr_l(block_t block) {
    const size_t block_half_size = sizeof(block_t) / 2;
    const block_t left_one = (block_t) 1 << block_half_size;

    return block + left_one;
}

block_t incr_r(block_t block) {
    const size_t block_half_size = sizeof(block_t) / 2;
    const block_t r_half_mask = ((block_t) 1 << block_half_size) - 1;
    const block_t l_half_mask = r_half_mask << block_half_size;

    return (block & l_half_mask) | (((block & r_half_mask) + 1) & r_half_mask);
}

block_t MSB(size_t n, block_t block) {
    return block >> (sizeof(block_t) * 8 - n) << (sizeof(block_t) * 8 - n);
}
