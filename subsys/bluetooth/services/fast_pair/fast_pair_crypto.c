/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <tinycrypt/constants.h>
#include <tinycrypt/sha256.h>
#include <tinycrypt/aes.h>
#include <tinycrypt/ecc_dh.h>
#include <bluetooth/services/fast_pair/fast_pair_crypto.h>

bool fp_sha256(uint8_t *out, const uint8_t *in, size_t datalen)
{
        struct tc_sha256_state_struct s;

        if (tc_sha256_init(&s) != TC_CRYPTO_SUCCESS) {
                return false;
        }
        if (tc_sha256_update(&s, in, datalen) != TC_CRYPTO_SUCCESS) {
                return false;
        }
        if (tc_sha256_final(out, &s) != TC_CRYPTO_SUCCESS) {
                return false;
        }
        return true;
}

bool fp_aes_encrypt(uint8_t *out, const uint8_t *in, const uint8_t *k)
{
        struct tc_aes_key_sched_struct s;
        
        if (tc_aes128_set_encrypt_key(&s, k) != TC_CRYPTO_SUCCESS) {
                return false;
        }
        if (tc_aes_encrypt(out, in, &s) != TC_CRYPTO_SUCCESS) {
                return false;
        }
        return true;
}

bool fp_aes_decrypt(uint8_t *out, const uint8_t *in, const uint8_t *k)
{
        struct tc_aes_key_sched_struct s;
        
        if (tc_aes128_set_encrypt_key(&s, k) != TC_CRYPTO_SUCCESS) {
                return false;
        }
        if (tc_aes_decrypt(out, in, &s) != TC_CRYPTO_SUCCESS) {
                return false;
        }
        return true;
}

bool fp_shared_secret(const uint8_t *public_key, const uint8_t *private_key, uint8_t *secret_key)
{
        const struct uECC_Curve_t *curve = uECC_secp256r1();

        if (uECC_shared_secret(public_key, private_key, secret_key, curve) != TC_CRYPTO_SUCCESS) {
                return false;
        }
        return true;
}
