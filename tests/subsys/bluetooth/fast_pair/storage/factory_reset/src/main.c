/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/ztest.h>
#include <zephyr/settings/settings.h>
#include <zephyr/sys/reboot.h>

#include <bluetooth/services/fast_pair.h>

#include "fp_storage_ak.h"
#include "fp_storage_ak_priv.h"
#include "fp_storage_pn.h"
#include "fp_storage_pn_priv.h"
#include "fp_storage_manager.h"
#include "fp_storage_manager_priv.h"

#include "common_utils.h"


#define ACCOUNT_KEY_MAX_CNT CONFIG_BT_FAST_PAIR_STORAGE_ACCOUNT_KEY_MAX

#define SETTINGS_RESET_TEST_SUBTREE_NAME "fp_reset_test"
#define SETTINGS_STAGE_KEY_NAME "stage"
#define SETTINGS_NAME_SEPARATOR_STR "/"
#define SETTINGS_RESET_TEST_STAGE_FULL_NAME \
	(SETTINGS_RESET_TEST_SUBTREE_NAME SETTINGS_NAME_SEPARATOR_STR SETTINGS_STAGE_KEY_NAME)

BUILD_ASSERT(SETTINGS_NAME_SEPARATOR == '/');

enum reset_test_stage {
	RESET_TEST_STAGE_NORMAL,
	RESET_TEST_STAGE_INTERRUPT,
	RESET_TEST_STAGE_INTERRUPT_FLOW_2,
};

static enum reset_test_stage test_stage = RESET_TEST_STAGE_NORMAL;

static int settings_set_err;

static bool storage_reset_reboot;


static int settings_load_stage(size_t len, settings_read_cb read_cb, void *cb_arg)
{
	int rc;

	if (len > sizeof(test_stage)) {
		return -EINVAL;
	}

	rc = read_cb(cb_arg, &test_stage, len);
	if (rc < 0) {
		return rc;
	}

	if (rc != len) {
		return -EINVAL;
	}

	return 0;
}

static int fp_reset_test_settings_set(const char *name, size_t len, settings_read_cb read_cb,
				      void *cb_arg)
{
	int err = 0;
	static const char *key = SETTINGS_STAGE_KEY_NAME;

	if (!strncmp(name, key, sizeof(SETTINGS_STAGE_KEY_NAME))) {
		err = settings_load_stage(len, read_cb, cb_arg);
	} else {
		err = -ENOENT;
	}

	/* The first reported settings set error will be remembered by the module.
	 * The error will then be propagated by the commit callback.
	 * Errors returned in the settings set callback are not propagated further.
	 */
	if (err && !settings_set_err) {
		settings_set_err = err;
	}

	return err;
}

static int fp_reset_test_settings_commit(void)
{
	if (settings_set_err) {
		return settings_set_err;
	}

	return 0;
}

SETTINGS_STATIC_HANDLER_DEFINE(fp_reset_test, SETTINGS_RESET_TEST_SUBTREE_NAME, NULL,
			       fp_reset_test_settings_set, fp_reset_test_settings_commit, NULL);

static int test_fp_storage_reset_perform(void)
{
	if (storage_reset_reboot) {
		sys_reboot(SYS_REBOOT_WARM);
	}

	return 0;
}

static void test_fp_storage_reset_prepare(void) {}

/* The name starts with 'aaa' so that this module's struct would be first in section (linker sorts
 * entries by name) and its callbacks would be call before other's modules callbacks.
 */
FP_STORAGE_MANAGER_MODULE_REGISTER(aaa_test_reset, test_fp_storage_reset_perform,
				   test_fp_storage_reset_prepare);

static void test_section_placement(void)
{
	struct fp_storage_manager_module *module;

	if (test_stage != RESET_TEST_STAGE_NORMAL) {
		ztest_test_pass();
	}

	STRUCT_SECTION_GET(fp_storage_manager_module, 0, &module);

	zassert_equal_ptr(test_fp_storage_reset_perform, module->module_reset_perform,
			  "Invalid order of elements in section");
	zassert_equal_ptr(test_fp_storage_reset_prepare, module->module_reset_prepare,
			  "Invalid order of elements in section");
}

static int fast_pair_settings_delete(void)
{
	int err;

	err = settings_delete(SETTINGS_RESET_IN_PROGRESS_FULL_NAME);
	if (err) {
		return err;
	}

	err = settings_delete(SETTINGS_PN_FULL_NAME);
	if (err) {
		return err;
	}

	for (uint8_t index = 0; index < ACCOUNT_KEY_CNT; index++) {
		err = fp_storage_ak_delete(index);
		if (err) {
			return err;
		}
	}

	return 0;
}

static void fp_storage_clear(void)
{
	int err;

	err = fast_pair_settings_delete();
	zassert_ok(err, "Unexpected error in settings delete");

	fp_storage_ak_ram_clear();
	fp_storage_pn_ram_clear();
	fp_storage_manager_ram_clear();
}

static void setup_fn(void)
{
	int err;

	fp_storage_clear();

	err = settings_load();
	zassert_ok(err, "Unexpected error in settings load");
}

static void teardown_fn(void)
{
	fp_storage_clear();
}

static void personalized_name_store(const char *name)
{
	int err;

	err = fp_storage_pn_save(name);
	zassert_ok(err, "Unexpected error in name save");
}

static void personalized_name_validate_loaded(const char *name)
{
	int err;
	char read_name[FP_STORAGE_PN_BUF_LEN];

	err = fp_storage_pn_get(read_name);
	zassert_ok(err, "Unexpected error in name get");
	err = strcmp(read_name, name);
	zassert_ok(err, "Invalid Personalized Name");
}

static void storage_validate_empty(void)
{
	int err;
	char read_name[FP_STORAGE_PN_BUF_LEN];
	int read_count;
	struct fp_account_key account_key_list[ACCOUNT_KEY_MAX_CNT];

	read_count = fp_storage_ak_count();
	zassert_equal(read_count, 0, "Invalid Account Key count");

	err = fp_storage_ak_get(account_key_list, &read_count);
	zassert_ok(err, "Unexpected error in Account Key get");
	zassert_equal(read_count, 0, "Invalid Account Key count");

	err = fp_storage_pn_get(read_name);
	zassert_ok(err, "Unexpected error in name get");
	err = strcmp(read_name, "");
	zassert_ok(err, "Expected saved name to be empty");
}

static void storage_validate(void)
{
	static const uint8_t seed = 5;
	static const uint8_t key_count = ACCOUNT_KEY_MAX_CNT;
	static const char *name = "storage_validate";
	int err;

	storage_validate_empty();

	cu_account_keys_generate_and_store(seed, key_count);
	personalized_name_store(name);

	cu_account_keys_validate_loaded(seed, key_count);
	personalized_name_validate_loaded(name);

	fp_storage_ak_ram_clear();
	fp_storage_pn_ram_clear();
	err = settings_load();
	zassert_ok(err, "Unexpected error in settings load");

	cu_account_keys_validate_loaded(seed, key_count);
	personalized_name_validate_loaded(name);
}

static void storage_validate_flow_2(void)
{
	int err;

	storage_validate_empty();

	fp_storage_ak_ram_clear();
	fp_storage_pn_ram_clear();
	err = settings_load();
	zassert_ok(err, "Unexpected error in settings load");

	storage_validate();
}

static void test_store(void)
{
	if (test_stage != RESET_TEST_STAGE_NORMAL) {
		ztest_test_pass();
	}

	storage_validate();
}

static void test_store_flow_2(void)
{
	if (test_stage != RESET_TEST_STAGE_NORMAL) {
		ztest_test_pass();
	}

	storage_validate_flow_2();
}

static void store_data_and_factory_reset(void)
{
	static const uint8_t seed = 50;
	static const uint8_t key_count = ACCOUNT_KEY_MAX_CNT;
	static const char *name = "store_data_and_factory_reset";
	int err;

	cu_account_keys_generate_and_store(seed, key_count);
	personalized_name_store(name);

	err = bt_fast_pair_factory_reset();
	zassert_ok(err, "Unexpected error in factory reset");
}

static void test_normal_reset(void)
{
	if (test_stage != RESET_TEST_STAGE_NORMAL) {
		ztest_test_pass();
	}

	store_data_and_factory_reset();

	storage_validate();
}

static void test_normal_reset_flow_2(void)
{
	if (test_stage != RESET_TEST_STAGE_NORMAL) {
		ztest_test_pass();
	}

	store_data_and_factory_reset();

	storage_validate_flow_2();
}

static void test_interrupted_reset(void)
{
	int err;

	switch (test_stage) {
	case RESET_TEST_STAGE_NORMAL:
		storage_reset_reboot = true;

		test_stage = RESET_TEST_STAGE_INTERRUPT;
		err = settings_save_one(SETTINGS_RESET_TEST_STAGE_FULL_NAME, &test_stage,
					sizeof(test_stage));
		zassert_ok(err, "Unexpected error in test settings");

		store_data_and_factory_reset();

		/* The recent instruction should trigger reboot so this code should never be
		 * entered.
		 */
		ztest_test_fail();

		break;

	case RESET_TEST_STAGE_INTERRUPT:
		storage_validate();

		break;

	case RESET_TEST_STAGE_INTERRUPT_FLOW_2:
		ztest_test_pass();

		break;

	default:
		ztest_test_fail();
		break;
	}
}

static void test_interrupted_reset_flow_2(void)
{
	int err;

	switch (test_stage) {
	case RESET_TEST_STAGE_NORMAL:
		/* This shouldn't happen as the test_interrupted_reset test should change
		 * the test_stage variable.
		 */
		ztest_test_fail();

		break;

	case RESET_TEST_STAGE_INTERRUPT:
		storage_reset_reboot = true;

		test_stage = RESET_TEST_STAGE_INTERRUPT_FLOW_2;
		err = settings_save_one(SETTINGS_RESET_TEST_STAGE_FULL_NAME, &test_stage,
					sizeof(test_stage));
		zassert_ok(err, "Unexpected error in test settings");

		store_data_and_factory_reset();

		/* The recent instruction should trigger reboot so this code should never be
		 * entered.
		 */
		ztest_test_fail();

		break;

	case RESET_TEST_STAGE_INTERRUPT_FLOW_2:
		storage_validate_flow_2();

		break;

	default:
		ztest_test_fail();
		break;
	}
}

void test_main(void)
{
	int err;

	err = settings_subsys_init();
	zassert_ok(err, "Error while initializing settings subsys");

	/* The test_interrupted_reset and the test_interrupted_reset_flow_2 tests must be placed in
	 * the suite in this exact places, because of dependencies during handling the test_stage
	 * variable.
	 */
	ztest_test_suite(fast_pair_storage_factory_reset_tests,
			 ztest_unit_test(test_section_placement),
			 ztest_unit_test_setup_teardown(test_store, setup_fn, teardown_fn),
			 ztest_unit_test_setup_teardown(test_store_flow_2, setup_fn, teardown_fn),
			 ztest_unit_test_setup_teardown(test_normal_reset, setup_fn, teardown_fn),
			 ztest_unit_test_setup_teardown(test_normal_reset_flow_2, setup_fn,
							teardown_fn),
			 ztest_unit_test_setup_teardown(test_interrupted_reset, setup_fn,
							teardown_fn),
			 ztest_unit_test_setup_teardown(test_interrupted_reset_flow_2, setup_fn,
							teardown_fn)
			 );

	ztest_run_test_suite(fast_pair_storage_factory_reset_tests);

	/* Non-volatile data of the test must be cleared at exit. */
	err = settings_delete(SETTINGS_RESET_TEST_STAGE_FULL_NAME);
	zassert_ok(err, "Error while cleaning up the test non-volatile storage");
}
