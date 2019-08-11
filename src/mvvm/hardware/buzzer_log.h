﻿/**
 * File:   buzzer_log.h
 * Author: AWTK Develop Team
 * Brief:  buzzer_log
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2019-07-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BUZZER_LOG_H
#define TK_BUZZER_LOG_H

#include "tkc/object.h"
#include "mvvm/hardware/buzzer.h"

BEGIN_C_DECLS

struct _buzzer_log_t;
typedef struct _buzzer_log_t buzzer_log_t;

/**
 * @class buzzer_log_t
 * @parent object_t
 *
 */
struct _buzzer_log_t {
  buzzer_t buzzer;
};

/**
 * @method buzzer_log_create
 * 创建buzzer对象。
 *
 * @return {object_t*} 返回object对象。
 */
object_t* buzzer_log_create(const char* args);

#define BUZZER_LOG(object) ((buzzer_log_t*)(object))

END_C_DECLS

#endif /*TK_BUZZER_LOG_H*/
