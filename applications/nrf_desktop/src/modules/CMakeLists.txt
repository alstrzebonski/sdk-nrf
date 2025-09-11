#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

target_sources_ifdef(CONFIG_DESKTOP_BLE_ADV_CTRL_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ble_adv_ctrl.c)

target_sources_ifdef(CONFIG_DESKTOP_BLE_BOND_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ble_bond.c)

target_sources_ifdef(CONFIG_DESKTOP_BLE_ENABLE_PASSKEY
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ble_passkey.c)

target_sources_ifdef(CONFIG_DESKTOP_BLE_LATENCY_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ble_latency.c)

target_sources_ifdef(CONFIG_DESKTOP_BLE_CONN_PARAMS_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ble_conn_params.c)

target_sources_ifdef(CONFIG_DESKTOP_BLE_DISCOVERY_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ble_discovery.c)

target_sources_ifdef(CONFIG_DESKTOP_BLE_SCAN_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ble_scan.c)

target_sources_ifdef(CONFIG_DESKTOP_BLE_QOS_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ble_qos.c)

target_sources_ifdef(CONFIG_DESKTOP_CONFIG_CHANNEL_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/info.c)

target_sources_ifdef(CONFIG_DESKTOP_FACTORY_RESET
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/factory_reset.c)

target_sources_ifdef(CONFIG_DESKTOP_FAST_PAIR
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/fast_pair.c)

target_sources_ifdef(CONFIG_DESKTOP_SWIFT_PAIR
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/swift_pair.c)

target_sources_ifdef(CONFIG_DESKTOP_HID_FORWARD_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/hid_forward.c)

target_sources_ifdef(CONFIG_DESKTOP_HID_STATE_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/hid_state.c)

target_sources_ifdef(CONFIG_DESKTOP_HID_REPORT_PROVIDER_CONSUMER_CTRL
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/hid_provider_consumer_ctrl.c)

target_sources_ifdef(CONFIG_DESKTOP_HID_REPORT_PROVIDER_MOUSE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/hid_provider_mouse.c)

target_sources_ifdef(CONFIG_DESKTOP_HID_REPORT_PROVIDER_KEYBOARD
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/hid_provider_keyboard.c)

target_sources_ifdef(CONFIG_DESKTOP_HID_REPORT_PROVIDER_SYSTEM_CTRL
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/hid_provider_system_ctrl.c)

target_sources_ifdef(CONFIG_DESKTOP_HID_STATE_PM_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/hid_state_pm.c)

target_sources_ifdef(CONFIG_DESKTOP_USB_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/usb_state.c)

target_sources_ifdef(CONFIG_DESKTOP_USB_PM_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/usb_state_pm.c)

target_sources_ifdef(CONFIG_CAF_LEDS
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/led_state.c)

target_sources_ifdef(CONFIG_DESKTOP_LED_STREAM_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/led_stream.c)

target_sources_ifdef(CONFIG_DESKTOP_CONFIG_CHANNEL_DFU_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dfu.c)

target_sources_ifdef(CONFIG_DESKTOP_DFU_MCUMGR_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dfu_mcumgr.c)

target_sources_ifdef(CONFIG_DESKTOP_FN_KEYS_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/fn_keys.c)

target_sources_ifdef(CONFIG_DESKTOP_WATCHDOG_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/watchdog.c)

target_sources_ifdef(CONFIG_DESKTOP_BAS_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/bas.c)

target_sources_ifdef(CONFIG_DESKTOP_QOS_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/qos.c)

target_sources_ifdef(CONFIG_DESKTOP_DEV_DESCR_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dev_descr.c)

target_sources_ifdef(CONFIG_DESKTOP_HIDS_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/hids.c)

target_sources_ifdef(CONFIG_DESKTOP_HFCLK_LOCK_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/hfclk_lock.c)

target_sources_ifdef(CONFIG_DESKTOP_CONSTLAT_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/constlat.c)

target_sources_ifdef(CONFIG_DESKTOP_FAILSAFE_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/failsafe.c)

target_sources_ifdef(CONFIG_DESKTOP_CPU_MEAS_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/cpu_meas.c)

target_sources_ifdef(CONFIG_DESKTOP_NRF_PROFILER_SYNC_GPIO_ENABLE
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/nrf_profiler_sync.c)

target_sources_ifdef(CONFIG_DESKTOP_DVFS
		     app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dvfs.c)

# Auto-generated zephyr_code_relocate commands to move all files to LocalCodeRAM_TEXT
# Generated from build.ninja analysis - relocating all compiled source files (excluding kernel)

# Relocate ALL application-specific files
zephyr_code_relocate(FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
zephyr_code_relocate(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../events/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
zephyr_code_relocate(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../hw_interface/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
zephyr_code_relocate(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../util/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
zephyr_code_relocate(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../main.c LOCATION LocalCodeRAM_TEXT NOKEEP)
zephyr_code_relocate(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../configuration/common/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)

# Relocate core Zephyr files (excluding kernel which can't be relocated)
#zephyr_code_relocate(FILES ${ZEPHYR_BASE}/arch/arm/core/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
#zephyr_code_relocate(FILES ${ZEPHYR_BASE}/arch/arm/core/cortex_m/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
#zephyr_code_relocate(FILES ${ZEPHYR_BASE}/arch/common/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
zephyr_code_relocate(FILES ${ZEPHYR_BASE}/drivers/*/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
zephyr_code_relocate(FILES ${ZEPHYR_BASE}/drivers/*/*/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
zephyr_code_relocate(FILES ${ZEPHYR_BASE}/subsys/*/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
zephyr_code_relocate(FILES ${ZEPHYR_BASE}/subsys/*/*/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
zephyr_code_relocate(FILES ${ZEPHYR_BASE}/subsys/*/*/*/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
#zephyr_code_relocate(FILES ${ZEPHYR_BASE}/soc/*/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
#zephyr_code_relocate(FILES ${ZEPHYR_BASE}/soc/*/*/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
zephyr_code_relocate(FILES ${ZEPHYR_BASE}/lib/*/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
zephyr_code_relocate(FILES ${ZEPHYR_BASE}/lib/*/*/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
#zephyr_code_relocate(FILES ${ZEPHYR_BASE}/misc/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
#zephyr_code_relocate(FILES ${ZEPHYR_BASE}/misc/generated/configs.c LOCATION LocalCodeRAM_TEXT NOKEEP)
zephyr_code_relocate(FILES ${ZEPHYR_BASE}/modules/hal_nordic/nrfx/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)

# Relocate build-generated files
#zephyr_code_relocate(FILES ${APPLICATION_BINARY_DIR}/zephyr/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
#zephyr_code_relocate(FILES ${APPLICATION_BINARY_DIR}/zephyr/code_relocation.c LOCATION LocalCodeRAM_TEXT NOKEEP)
#zephyr_code_relocate(FILES ${APPLICATION_BINARY_DIR}/zephyr/isr_tables.c LOCATION LocalCodeRAM_TEXT NOKEEP)
#zephyr_code_relocate(FILES ${APPLICATION_BINARY_DIR}/zephyr/misc/generated/configs.c LOCATION LocalCodeRAM_TEXT NOKEEP)

# Relocate nRF-specific files
zephyr_code_relocate(FILES ${NRF_DIR}/subsys/*/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
zephyr_code_relocate(FILES ${NRF_DIR}/subsys/*/*/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
zephyr_code_relocate(FILES ${NRF_DIR}/subsys/*/*/*/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
zephyr_code_relocate(FILES ${NRF_DIR}/subsys/*/*/*/*/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
zephyr_code_relocate(FILES ${NRF_DIR}/drivers/*/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
zephyr_code_relocate(FILES ${NRF_DIR}/lib/*/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)

# Relocate HAL drivers (excluding mdk)
zephyr_code_relocate(FILES ${NRFX_DIR}/drivers/src/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
zephyr_code_relocate(FILES ${NRFX_DIR}/helpers/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)

# Relocate crypto and security files using relative paths
#zephyr_code_relocate(FILES ${ARM_MBEDTLS_PATH}/library/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
#zephyr_code_relocate(FILES ${OBERON_PSA_CORE_PATH}/library/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)
#zephyr_code_relocate(FILES ${OBERON_PSA_CORE_PATH}/programs/ssl/library/*.c LOCATION LocalCodeRAM_TEXT NOKEEP)

# Relocate specific nRF security files (avoiding problematic wildcards)
#zephyr_code_relocate(FILES ${NRF_SECURITY_ROOT}/src/threading/threading_alt.c LOCATION LocalCodeRAM_TEXT NOKEEP)
#zephyr_code_relocate(FILES ${NRF_SECURITY_ROOT}/src/utils/nrf_security_events.c LOCATION LocalCodeRAM_TEXT NOKEEP)
#zephyr_code_relocate(FILES ${NRF_SECURITY_ROOT}/src/utils/nrf_security_mutexes.c LOCATION LocalCodeRAM_TEXT NOKEEP)
#zephyr_code_relocate(FILES ${NRF_SECURITY_ROOT}/src/zephyr/entropy_poll.c LOCATION LocalCodeRAM_TEXT NOKEEP)
#zephyr_code_relocate(FILES ${NRF_SECURITY_ROOT}/src/zephyr/mbedtls_heap.c LOCATION LocalCodeRAM_TEXT NOKEEP)
#zephyr_code_relocate(FILES ${NRF_SECURITY_ROOT}/src/zephyr/psa_crypto_init.c LOCATION LocalCodeRAM_TEXT NOKEEP)
#zephyr_code_relocate(FILES ${NRF_SECURITY_ROOT}/src/psa_crypto_driver_wrappers.c LOCATION LocalCodeRAM_TEXT NOKEEP)

# Relocate bootloader files (using correct relative path)
#zephyr_code_relocate(FILES ${ZEPHYR_BASE}/../bootloader/mcuboot/boot/bootutil/src/bootutil_public.c LOCATION LocalCodeRAM_TEXT NOKEEP)
