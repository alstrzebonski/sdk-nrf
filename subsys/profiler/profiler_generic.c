/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <stdio.h>
#include <kernel_structs.h>
#include <sys/printk.h>
#include <sys/util.h>
#include <sys/byteorder.h>
#include <zephyr.h>
#include <SEGGER_RTT.h>
#include <string.h>
#include <nrfx.h>

#include <profiler.h>
#include <profiler_backend.h>

/* By default, all events are profiled. */
static uint32_t profiler_enabled_events = 0xffffffff;

static uint8_t profiler_num_events;

uint32_t get_profiler_enabled_events()
{
	return profiler_enabled_events;
}

uint8_t get_profiler_num_events()
{
	return profiler_num_events;
}

int profiler_init_backend(void);

int profiler_init(void)
{
	static bool profiler_initialized;

	k_sched_lock();
	if (profiler_initialized) {
		k_sched_unlock();
		return 0;
	}
	int ret = profiler_init_backend();
	if (ret == 0) {
		profiler_initialized = true;
	}
	k_sched_unlock();
	return ret;
}

void profiler_term_backend(void);

void profiler_term(void)
{
	profiler_term_backend();
}

const char *profiler_get_event_descr_backend(uint16_t event_type_id);

const char *profiler_get_event_descr(uint16_t event_type_id)
{
	return profiler_get_event_descr_backend(event_type_id);
}

bool is_profiling_enabled(uint16_t event_type_id)
{
	__ASSERT_NO_MSG(profiler_event_id < CONFIG_MAX_NUMBER_OF_CUSTOM_EVENTS);
	return (profiler_enabled_events & BIT(event_type_id)) != 0;
}

uint16_t profiler_register_event_type_backend(const char *name, const char * const *args,
				   const enum profiler_arg *arg_types,
				   uint8_t arg_cnt);

uint16_t profiler_register_event_type(const char *name, const char * const *args,
				   const enum profiler_arg *arg_types,
				   uint8_t arg_cnt)
{
	uint16_t ret = profiler_register_event_type_backend(name, args, arg_types, arg_cnt);
	profiler_num_events++;
	return ret;
}

void profiler_log_start_backend(struct log_event_buf *buf);

bool profiler_log_start(struct log_event_buf *buf, uint16_t event_type_id)
{
	if (IS_ENABLED(CONFIG_PROFILER_DYNAMIC_EVENT_ENABLING)) {
		__ASSERT_NO_MSG(event_type_id < sizeof(profiler_enabled_events) * 8)
		if (!(profiler_enabled_events & BIT(event_type_id))) {
			return false;
		}
	}
	profiler_log_start_backend(buf);
	return true;
}

void profiler_log_encode_u32_backend(struct log_event_buf *buf, uint32_t data);

void profiler_log_encode_u32(struct log_event_buf *buf, uint32_t data)
{
	profiler_log_encode_u32_backend(buf, data);
}

void profiler_log_encode_string_backend(struct log_event_buf *buf, const char *string);

void profiler_log_encode_string(struct log_event_buf *buf, const char *string)
{
	profiler_log_encode_string_backend(buf, string);
}

void profiler_log_add_mem_address_backend(struct log_event_buf *buf,
				  const void *mem_address);

void profiler_log_add_mem_address(struct log_event_buf *buf,
				  const void *mem_address)
{
	profiler_log_add_mem_address_backend(buf, mem_address);
}

void profiler_log_send_backend(struct log_event_buf *buf, uint16_t event_type_id);

void profiler_log_send(struct log_event_buf *buf, uint16_t event_type_id)
{
	if (IS_ENABLED(CONFIG_PROFILER_DYNAMIC_EVENT_ENABLING)) {
		__ASSERT_NO_MSG(event_type_id < sizeof(profiler_enabled_events) * 8)
		if (!(profiler_enabled_events & BIT(event_type_id))) {
			return;
		}
	}
	profiler_log_send_backend(buf, event_type_id);
}

void profiler_log_enable(uint16_t event_type_id)
{
	if (IS_ENABLED(CONFIG_PROFILER_DYNAMIC_EVENT_ENABLING)) {
		__ASSERT_NO_MSG(event_type_id < sizeof(profiler_enabled_events) * 8);
		profiler_enabled_events |= BIT(event_type_id);
	}
}

void profiler_log_disable(uint16_t event_type_id)
{
	if (IS_ENABLED(CONFIG_PROFILER_DYNAMIC_EVENT_ENABLING)) {
		__ASSERT_NO_MSG(event_type_id < sizeof(profiler_enabled_events) * 8);
		profiler_enabled_events &= ~(BIT(event_type_id));
	}
}
