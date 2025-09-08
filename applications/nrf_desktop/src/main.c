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

#define PROFILE_CACHE 1
#if PROFILE_CACHE
#include <hal/nrf_cache.h>
static uint32_t ihits;
static uint32_t imisses;
#endif
#define CACHE_INVALIDATE 0

static void cache_profiling_init(void)
{
#if PROFILE_CACHE
	nrf_cache_profiling_set(NRF_ICACHE, 1);
#endif
}

static void cache_profiling_clear(void)
{
#if PROFILE_CACHE
	nrf_cache_profiling_counters_clear(NRF_ICACHE);
#endif
}

static void cache_stats_update(void)
{	
#if PROFILE_CACHE
	ihits = nrf_cache_data_hit_counter_get(NRF_ICACHE, 0);
	imisses = nrf_cache_data_miss_counter_get(NRF_ICACHE, 0);
#endif
}

static void cache_stats_print(void)
{
#if PROFILE_CACHE
	printk("\tInstr cache hits: %u\n", ihits);
	printk("\tInstr cache misses: %u\n", imisses);
#endif
}


int main(void)
{
	#if 0
	nrf_rramc_power_t rram_power;
	nrf_rramc_power_config_get(NRF_RRAMC, &rram_power);
	LOG_INF("RRAM power access timeout %d", rram_power.access_timeout);
	rram_power.access_timeout = 0xFFFF;//8192; // 8192 * 31.25 ns = 256 us
	nrf_rramc_power_config_set(NRF_RRAMC, &rram_power);
	LOG_INF("RRAM power access timeout set to %d", rram_power.access_timeout);
	#endif
	#if 1
	unsigned int lowpower_conf = NRF_RRAMC->POWER.LOWPOWERCONFIG;
	//NRF_RRAMC->POWER.LOWPOWERCONFIG = (0 << 6) | 0x1; /* Mode:Standby + AXI wake up - speed up 13 us */
	NRF_RRAMC->POWER.LOWPOWERCONFIG = (1 << 6) | 0x1; /* Mode:Standby + CPU wake up - speed up additionally 2us similarly to skipping WFI */
	LOG_INF("RRAM power config was %08x and is set to %08x", lowpower_conf, NRF_RRAMC->POWER.LOWPOWERCONFIG);
	#endif

	if (app_event_manager_init()) {
		LOG_ERR("Application Event Manager not initialized");
	} else {
		module_set_state(MODULE_STATE_READY);
	}
	#if PROFILE_CACHE
	cache_profiling_init();
	while(1) {
		cache_stats_update();
		cache_stats_print();
		cache_profiling_clear();
		k_sleep(K_SECONDS(5));
	}
	#endif
	return 0;
}
