/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <ztest.h>
#include <bluetooth/services/fast_pair/fast_pair_crypto.h>

static void test_sha256(void)
{
	const uint8_t input_data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};

	const uint8_t hashed_result[] = {0xBB, 0x00, 0x0D, 0xDD, 0x92, 0xA0, 0xA2, 0xA3, 0x46, 0xF0,
					 0xB5, 0x31, 0xF2, 0x78, 0xAF, 0x06, 0xE3, 0x70, 0xF8, 0x69,
					 0x32, 0xCC, 0xAF, 0xCC, 0xC8, 0x92, 0xD6, 0x8D, 0x35, 0x0F,
					 0x80, 0xF8};

	uint8_t result_buf[32];

	zassert_true(fp_sha256(result_buf, input_data, sizeof(input_data)),
		     "Error during hashing data.");
	zassert_mem_equal(result_buf, hashed_result, sizeof(hashed_result),
			  "Invalid hashing result");
}

static void test_aes(void)
{
	const uint8_t plaintext[] = {0xF3, 0x0F, 0x4E, 0x78, 0x6C, 0x59, 0xA7, 0xBB, 0xF3, 0x87,
				     0x3B, 0x5A, 0x49, 0xBA, 0x97, 0xEA};

	const uint8_t key[] = {0xA0, 0xBA, 0xF0, 0xBB, 0x95, 0x1F, 0xF7, 0xB6, 0xCF, 0x5E, 0x3F,
			       0x45, 0x61, 0xC3, 0x32, 0x1D};

	const uint8_t ciphertext[] = {0xAC, 0x9A, 0x16, 0xF0, 0x95, 0x3A, 0x3F, 0x22, 0x3D, 0xD1,
				      0x0C, 0xF5, 0x36, 0xE0, 0x9E, 0x9C};

	uint8_t result_buf[16];

	zassert_true(fp_aes_encrypt(result_buf, plaintext, key), "Error during value encryption.");
	zassert_mem_equal(result_buf, ciphertext, sizeof(ciphertext), "Invalid encryption result.");

	zassert_true(fp_aes_decrypt(result_buf, ciphertext, key), "Error during value decryption.");
	zassert_mem_equal(result_buf, plaintext, sizeof(plaintext), "Invalid decryption result.");
}

void test_main(void)
{
	ztest_test_suite(fast_pair_crypto_tests,
			 ztest_unit_test(test_sha256),
			 ztest_unit_test(test_aes)
			 );

	ztest_run_test_suite(fast_pair_crypto_tests);
}
