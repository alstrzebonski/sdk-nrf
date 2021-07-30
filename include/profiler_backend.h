/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef _PROFILER_BACKEND_H_
#define _PROFILER_BACKEND_H_

/**
 * @defgroup profiler Profiler
 * @brief Profiler
 *
 * @{
 */

/*
#include <zephyr/types.h>
#include <sys/util.h>
#include <sys/__assert.h>
*/


uint32_t get_profiler_enabled_events();

uint8_t get_profiler_num_events();

int profiler_init_backend(void);


void profiler_term_backend(void);

const char *profiler_get_event_descr_backend(uint16_t event_type_id);


uint16_t profiler_register_event_type_backend(const char *name, const char * const *args,
				   const enum profiler_arg *arg_types,
				   uint8_t arg_cnt);

void profiler_log_start_backend(struct log_event_buf *buf);

void profiler_log_encode_u32_backend(struct log_event_buf *buf, uint32_t data);

void profiler_log_encode_string_backend(struct log_event_buf *buf, const char *string);

void profiler_log_add_mem_address_backend(struct log_event_buf *buf,
				  const void *mem_address);

void profiler_log_send_backend(struct log_event_buf *buf, uint16_t event_type_id);

void profiler_log_enable(uint16_t event_type_id);

void profiler_log_disable(uint16_t event_type_id);


/**
 * @}
 */

#endif /* _PROFILER_BACKEND_H_ */
