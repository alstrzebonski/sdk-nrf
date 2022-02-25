/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef _FAST_PAIR_CRYPTO_H_
#define _FAST_PAIR_CRYPTO_H_

/**
 * @defgroup fast_pair_crypto Fast Pair Crypto
 * @brief Fast Pair Crypto
 *
 * @{
 */

/** @brief Hash value using SHA-256
 *
 *  @param out Buffer to receive hashed result.
 *  @param in Input data.
 *  @param datalen Length of input data.
 *
 * @return True on success and False on failure.
 */
bool fp_sha256(uint8_t *out, const uint8_t *in, size_t datalen);

/** @brief Encrypt message using AES-128
 * 
 *  @param out Buffer to receive encrypted message.
 *  @param in Plaintext message.
 *  @param k AES key.
 * 
 * @return True on success and False on failure.
 */
bool fp_aes_encrypt(uint8_t *out, const uint8_t *in, const uint8_t *k);

/** @brief Decrypt message using AES-128
 * 
 *  @param out Buffer to receive plaintext message.
 *  @param in Ciphertext message.
 *  @param k AES key.
 * 
 * @return True on success and False on failure.
 */
bool fp_aes_decrypt(uint8_t *out, const uint8_t *in, const uint8_t *k);

/** @brief Compute a shared secret given your secret key and someone else's public key.
 *         Keys are assumed to be secp256r1 elliptic curve keys.
 * 
 *  @param public_key Someone else's public key.
 *  @param private_key Your secret key.
 *  @param secret_key Buffer to receive shared secret key.
 * 
 * @return True on success and False on failure.
 */
bool fp_shared_secret(const uint8_t *public_key, const uint8_t *private_key, uint8_t *secret_key);

/**
 * @}
 */

#endif /* _FAST_PAIR_CRYPTO_H_ */
