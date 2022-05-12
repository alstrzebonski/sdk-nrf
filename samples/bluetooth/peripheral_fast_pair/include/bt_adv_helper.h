/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef BT_ADV_HELPER_H_
#define BT_ADV_HELPER_H_

#include <bluetooth/bluetooth.h>

/**
 * @defgroup fast_pair_sample_adv_helper Fast Pair sample advertising helper API
 * @brief Fast Pair sample advertising helper API
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/** Start Fast Pair sample advertising
 *
 * Include: TX power and Fast Pair advertising data into an advertising packet and start
 * advertising. Device name is added to scan response data. The advertising is not resumed after
 * Bluetooth connection is established.
 *
 * The function handles also periodic RPA rotation and Fast Pair advertising data update.
 *
 * Device must be Fast Pair not discoverable and the Account Key List must be not empty for
 * show/hide UI indication option to be used.
 *
 * @param[in] fp_discoverable	    Indicate if the device should be Fast Pair discoverable.
 * @param[in] fp_show_ui_indication Boolean indicating if UI indication are to be shown on Fast Pair
 *                                  Seeker device.
 *
 * @return 0 if the operation was successful. Otherwise, a (negative) error code is returned.
 */
int bt_adv_helper_adv_start(bool fp_discoverable, bool fp_show_ui_indication);

/** Stop Fast Pair sample advertising
 *
 * @return 0 if the operation was successful. Otherwise, a (negative) error code is returned.
 */
int bt_adv_helper_adv_stop(void);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* BT_ADV_HELPER_H_ */
