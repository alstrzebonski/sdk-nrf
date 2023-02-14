/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/ztest.h>
#include <zephyr/settings/settings.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/storage/flash_map.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(test_main);

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
	RESET_TEST_STAGE_INTERRUPTED,
};

enum reset_test_finished {
	RESET_TEST_FINISHED_SECTION_PLACEMENT_BIT_POS,
	RESET_TEST_FINISHED_STORE_BIT_POS,
	RESET_TEST_FINISHED_NORMAL_RESET_BIT_POS,
	RESET_TEST_FINISHED_INTERRUPTED_RESET_BEGIN_BIT_POS,

	RESET_TEST_FINISHED_COUNT,
};

static enum reset_test_stage test_stage = RESET_TEST_STAGE_NORMAL;
static uint8_t test_finished_mask;

BUILD_ASSERT(__CHAR_BIT__ * sizeof(test_finished_mask) >= RESET_TEST_FINISHED_COUNT);

static int settings_set_err;

static bool storage_reset_pass;


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

	if (!strncmp(name, SETTINGS_STAGE_KEY_NAME, sizeof(SETTINGS_STAGE_KEY_NAME))) {
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

static void mark_test_as_finished(enum reset_test_finished bit_pos)
{
	WRITE_BIT(test_finished_mask, bit_pos, 1);
}

static int test_fp_storage_reset_perform(void)
{
	if (storage_reset_pass) {
		mark_test_as_finished(RESET_TEST_FINISHED_INTERRUPTED_RESET_BEGIN_BIT_POS);
		storage_reset_pass = false;
		ztest_test_pass();
	}

	return 0;
}

static void test_fp_storage_reset_prepare(void) {}

/* The name starts with 'aaa' so that this module's struct would be first in section (linker sorts
 * entries by name) and its callbacks would be call before other's modules callbacks.
 */
FP_STORAGE_MANAGER_MODULE_REGISTER(_0_test_reset, test_fp_storage_reset_perform,
				   test_fp_storage_reset_prepare);

static void test_section_placement(void)
{
	struct fp_storage_manager_module *module;

	STRUCT_SECTION_GET(fp_storage_manager_module, 0, &module);

	zassert_equal_ptr(test_fp_storage_reset_perform, module->module_reset_perform,
			  "Invalid order of elements in section");
	zassert_equal_ptr(test_fp_storage_reset_prepare, module->module_reset_prepare,
			  "Invalid order of elements in section");

	mark_test_as_finished(RESET_TEST_FINISHED_SECTION_PLACEMENT_BIT_POS);
}

static int fp_settings_validate_delete_cb(const char *key, size_t len, settings_read_cb read_cb,
					  void *cb_arg, void *param)
{
	int *err = param;

	if (!strncmp(key, SETTINGS_RESET_TEST_STAGE_FULL_NAME,
		     sizeof(SETTINGS_RESET_TEST_STAGE_FULL_NAME))) {
		return 0;
	}

	*err = -EFAULT;
	return *err;
}

static int fp_settings_validate_delete(void)
{
	int err;
	int err_cb = 0;

	err = settings_load_subtree_direct(NULL, fp_settings_validate_delete_cb, &err_cb);
	if (err) {
		return err;
	}

	if (err_cb) {
		return err_cb;
	}

	return 0;
}

static int fp_data_nvs_validate_empty_cb(const char *key, size_t len, settings_read_cb read_cb,
					      void *cb_arg, void *param)
{
	int *err = param;

	if (!strncmp(key, SETTINGS_RESET_TEST_STAGE_FULL_NAME,
		     sizeof(SETTINGS_RESET_TEST_STAGE_FULL_NAME))) {
		return 0;
	}

	if (!strncmp(key, SETTINGS_RESET_IN_PROGRESS_FULL_NAME,
		     sizeof(SETTINGS_RESET_IN_PROGRESS_FULL_NAME))) {
		int rc;
		bool reset_in_progress;

		if (len > sizeof(reset_in_progress)) {
			*err = -EINVAL;
			return *err;
		}

		rc = read_cb(cb_arg, &reset_in_progress, len);
		if (rc < 0) {
			*err = rc;
			return *err;
		}

		if (rc != len) {
			*err = -EINVAL;
			return *err;
		}

		if (reset_in_progress) {
			*err = -EFAULT;
			return *err;
		}

		return 0;
	}

	*err = -EFAULT;
	return *err;
}

static int fp_data_nvs_validate_empty(void)
{
	int err;
	int err_cb = 0;

	err = settings_load_subtree_direct(NULL, fp_data_nvs_validate_empty_cb, &err_cb);
	if (err) {
		return err;
	}

	if (err_cb) {
		return err_cb;
	}

	return 0;
}

static int fp_settings_delete(void)
{
	int err;

	err = bt_fast_pair_factory_reset();
	if (err) {
		return err;
	}

	err = settings_delete(SETTINGS_RESET_IN_PROGRESS_FULL_NAME);
	if (err) {
		return err;
	}

	err = fp_settings_validate_delete();
	if (err) {
		return err;
	}

	return 0;
}

static void fp_ram_clear(void)
{
	fp_storage_ak_ram_clear();
	fp_storage_pn_ram_clear();
	fp_storage_manager_ram_clear();
}

static void setup_fn(void)
{
	int err;

	err = fp_settings_validate_delete();
	zassert_ok(err, "Settings have not been fully deleted");

	err = settings_load();
	zassert_ok(err, "Unexpected error in settings load");
}

static void teardown_fn(void)
{
	int err;

	err = fp_settings_delete();
	zassert_ok(err, "Unexpected error in settings delete");

	fp_ram_clear();
}

static int all_settings_delete(void)
{
	int err;
	const struct flash_area *flash_area;

	err = flash_area_open(FIXED_PARTITION_ID(storage_partition), &flash_area);
	if (err) {
		return err;
	}

	err = flash_area_erase(flash_area, 0, FIXED_PARTITION_SIZE(storage_partition));
	flash_area_close(flash_area);
	if (err) {
		return err;
	}

	return 0;
}

static void final_teardown_fn(void)
{
	int err;

	err = all_settings_delete();
	zassert_ok(err, "Unable to delete settings");
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

	err = fp_data_nvs_validate_empty();
	zassert_ok(err, "Non-volatile Fast Pair data storage is not empty after reset");
}

static void storage_validate(void)
{
	static const uint8_t seed = 5;
	static const uint8_t key_count = ACCOUNT_KEY_MAX_CNT;
	static const char *name = "storage_validate_ram";
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

static void test_store(void)
{
	storage_validate();

	mark_test_as_finished(RESET_TEST_FINISHED_STORE_BIT_POS);
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
	store_data_and_factory_reset();

	storage_validate();

	mark_test_as_finished(RESET_TEST_FINISHED_NORMAL_RESET_BIT_POS);
}

static void test_interrupted_reset_begin(void)
{
	storage_reset_pass = true;

	store_data_and_factory_reset();

	/* The recent instruction should trigger reboot so this code should never be entered. */
	ztest_test_fail();
}

static void test_interrupted_reset_finish_setup_fn(void)
{
	int err;

	err = settings_load();
	zassert_ok(err, "Unexpected error in settings load");
}

static void test_interrupted_reset_finish(void)
{
	storage_validate();
}

static void normal_test_stage_finalize(void)
{
	int err;

	for (size_t i = 0; i < RESET_TEST_FINISHED_COUNT; i++) {
		if (!(test_finished_mask & BIT(i))) {
			/* Do not reboot the device, clean up and abandon the test to
			 * properly report fail in recent tests.
			 */
			err = all_settings_delete();
			__ASSERT(!err, "Unable to delete settings (err %d)", err);

			return;
		}
	}

	test_stage = RESET_TEST_STAGE_INTERRUPTED;
	err = settings_save_one(SETTINGS_RESET_TEST_STAGE_FULL_NAME, &test_stage,
				sizeof(test_stage));
	__ASSERT(!err, "Unexpected error in test settings (err %d)", err);

	/* Reboot to finish the interrupted reset test. */
	//sys_reboot(SYS_REBOOT_COLD);
	sys_reboot(SYS_REBOOT_WARM);
}

void test_main(void)
{
	int err;

	err = settings_subsys_init();
	__ASSERT(!err, "Error while initializing settings subsys (err %d)", err);

	err = settings_load_subtree(SETTINGS_RESET_TEST_SUBTREE_NAME);
	__ASSERT(!err, "Error while loading test settings (err %d)", err);

	switch (test_stage) {
	case RESET_TEST_STAGE_NORMAL:
		;
		/* The test_interrupted_reset_begin test must be placed last in the suite, it
		 * triggers sys_reboot.
		 */
		ztest_test_suite(fast_pair_storage_factory_reset_normal_tests,
				 ztest_unit_test(test_section_placement),
				 ztest_unit_test_setup_teardown(test_store, setup_fn, teardown_fn),
				 ztest_unit_test_setup_teardown(test_normal_reset, setup_fn,
								teardown_fn),
				 ztest_unit_test_setup_teardown(
							test_interrupted_reset_begin,
							setup_fn,
							unit_test_noop)
				 );

		ztest_run_test_suite(fast_pair_storage_factory_reset_normal_tests);

		normal_test_stage_finalize();

		break;

	case RESET_TEST_STAGE_INTERRUPTED:
		;
		ztest_test_suite(fast_pair_storage_factory_reset_interrupted_tests_continue,
				 ztest_unit_test_setup_teardown(
							test_interrupted_reset_finish,
							test_interrupted_reset_finish_setup_fn,
							final_teardown_fn)
				 );

		ztest_run_test_suite(fast_pair_storage_factory_reset_interrupted_tests_continue);

		break;

	default:
		__ASSERT_NO_MSG(false);

		break;
	}
}
