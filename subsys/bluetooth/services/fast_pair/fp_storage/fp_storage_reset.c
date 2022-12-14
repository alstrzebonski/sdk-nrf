/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <errno.h>
#include <string.h>
#include <zephyr/settings/settings.h>
#include <zephyr/sys/reboot.h>
#include <bluetooth/services/fast_pair.h>

#include "fp_storage.h"
#include "fp_storage_pn.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(fp_storage_reset, CONFIG_FP_STORAGE_LOG_LEVEL);

#define SETTINGS_RESET_SUBTREE_NAME "fp_reset"
#define SETTINGS_RESET_IN_PROGRESS_NAME "reset_in_progress"
#define SETTINGS_NAME_SEPARATOR_STR "/"
#define SETTINGS_RESET_IN_PROGRESS_FULL_NAME \
	(SETTINGS_RESET_SUBTREE_NAME SETTINGS_NAME_SEPARATOR_STR SETTINGS_RESET_IN_PROGRESS_NAME)

static bool reset_in_progress;
static int settings_set_err;

static int fp_settings_load_reset_in_progress(size_t len, settings_read_cb read_cb, void *cb_arg)
{
	int rc;

	if (len != sizeof(reset_in_progress)) {
		return -EINVAL;
	}

	rc = read_cb(cb_arg, &reset_in_progress, len);
	if (rc < 0) {
		return rc;
	}

	if (rc != len) {
		return -EINVAL;
	}

	return 0;
}

static int fp_settings_set(const char *name, size_t len, settings_read_cb read_cb, void *cb_arg)
{
	int err = 0;
	const char *key = SETTINGS_RESET_IN_PROGRESS_NAME;

	if (!strncmp(name, key, sizeof(SETTINGS_RESET_IN_PROGRESS_NAME))) {
		err = fp_settings_load_reset_in_progress(len, read_cb, cb_arg);
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

static int fp_settings_commit(void)
{
	if (settings_set_err) {
		return settings_set_err;
	}

	int err;

	if (reset_in_progress) {
		LOG_WRN("Fast Pair factory reset has been interrupted or aborted. Retrying");
		err = bt_fast_pair_factory_reset();
		if (err) {
			LOG_ERR("Unable to perform Fast Pair factory reset (err %d)", err);
			return err;
		}
	}

	return 0;
}

SETTINGS_STATIC_HANDLER_DEFINE(fast_pair_storage_reset, SETTINGS_RESET_SUBTREE_NAME, NULL,
			       fp_settings_set, fp_settings_commit, NULL);

int bt_fast_pair_factory_reset(void)
{
	int err;
	bool in_progress;

	in_progress = true;
	err = settings_save_one(SETTINGS_RESET_IN_PROGRESS_FULL_NAME, &in_progress,
				sizeof(in_progress));
	if (err) {
		return err;
	}

	reset_in_progress = in_progress;

	err = fp_storage_delete();
	if (err) {
		LOG_ERR("Unable to delete record (err %d)", err);
		return err;
	}

	if (IS_ENABLED(CONFIG_BT_FAST_PAIR_STORAGE_EXT_PN)) {
		err = fp_storage_pn_delete();
		if (err) {
			LOG_ERR("Unable to delete record (err %d)", err);
			return err;
		}
	}

	in_progress = false;
	err = settings_save_one(SETTINGS_RESET_IN_PROGRESS_FULL_NAME, &in_progress,
				sizeof(in_progress));
	if (err) {
		return err;
	}

	reset_in_progress = in_progress;

	return 0;
}
