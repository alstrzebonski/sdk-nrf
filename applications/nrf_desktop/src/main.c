/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <app_event_manager.h>

#define MODULE main
#include <caf/events/module_state_event.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(MODULE);


#include <hal/nrf_rramc.h>


int main(void)
{
	// #if 1
	// nrf_rramc_power_t rram_power;
	// nrf_rramc_power_config_get(NRF_RRAMC, &rram_power);
	// LOG_INF("RRAM power access timeout %d", rram_power.access_timeout);
	// rram_power.access_timeout = 0xFFFF;//8192; // 8192 * 31.25 ns = 256 us
	// nrf_rramc_power_config_set(NRF_RRAMC, &rram_power);
	// LOG_INF("RRAM power access timeout set to %d", rram_power.access_timeout);
	// #endif
	// #if 1
	// unsigned int lowpower_conf = NRF_RRAMC->POWER.LOWPOWERCONFIG;
	// //NRF_RRAMC->POWER.LOWPOWERCONFIG = (0 << 6) | 0x1; /* Mode:Standby + AXI wake up - speed up 13 us */
	// NRF_RRAMC->POWER.LOWPOWERCONFIG = (1 << 6) | 0x1; /* Mode:Standby + CPU wake up - speed up additionally 2us similarly to skipping WFI */
	// LOG_INF("RRAM power config was %08x and is set to %08x", lowpower_conf, NRF_RRAMC->POWER.LOWPOWERCONFIG);
	// #endif

	if (app_event_manager_init()) {
		LOG_ERR("Application Event Manager not initialized");
	} else {
		module_set_state(MODULE_STATE_READY);
	}
	// while(1) {}
	return 0;
}
