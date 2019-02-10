﻿/**
 * File:   command_binding.c
 * Author: AWTK Develop Team
 * Brief:  command binding rule
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
 * 2019-01-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_COMMAND_BINDING_H
#define TK_COMMAND_BINDING_H

#include "mvvm/base/binding_rule.h"

BEGIN_C_DECLS

/**
 * @class command_binding_t
 * @parent binding_rule_t
 * 命令绑定规则。
 *
 */
typedef struct _command_binding_t {
  binding_rule_t binding_rule;
  object_t* props;

  /**
   * @property {bool_t} close_window
   * @annotation ["readable"]
   * 执行命令之后，是否关闭当前窗口。
   */
  bool_t close_window;

  /**
   * @property {bool_t} update_model
   * @annotation ["readable"]
   * 执行命令之前，是否更新数据到模型。
   */
  bool_t update_model;

  /**
   * @property {char*} command
   * @annotation ["readable"]
   * 命令名称。
   */
  char* command;

  /**
   * @property {char*} args
   * @annotation ["readable"]
   * 命令参数。
   */
  char* args;

  /**
   * @property {char*} event
   * @annotation ["readable"]
   * 事件名称。
   */
  char* event;

  /**
   * @property {char*} event_args
   * @annotation ["readable"]
   * 事件参数(主要用于按键事件)。
   */
  char* event_args;
} command_binding_t;

/**
 * @method command_binding_create
 * 创建数据绑定对象。
 * @annotation ["constructor"]
 *
 * @return {binding_rule_t*} 返回数据绑定对象。
 */
command_binding_t* command_binding_create(void);

/**
 * @method command_binding_can_exec
 * 检查当前的命令是否可以执行。
 *
 * @param {command_binding_t*} rule 绑定规则对象。
 *
 * @return {bool_t} 返回TRUE表示可以执行，否则表示不可以执行。
 */
bool_t command_binding_can_exec(command_binding_t* rule);

/**
 * @method command_binding_exec
 * 执行当前的命令。
 *
 * @param {command_binding_t*} rule 绑定规则对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t command_binding_exec(command_binding_t* rule);

#define COMMAND_BINDING(rule) ((command_binding_t*)rule);

#define COMMAND_BINDING_CMD_NOTHING "nothing"
#define COMMAND_BINDING_CMD_NAVIGATE "navigate"

#define COMMAND_BINDING_ARGS "Args"
#define COMMAND_BINDING_EVENT "Event"
#define COMMAND_BINDING_COMMAND "Command"
#define COMMAND_BINDING_EVENT_ARGS "EventArgs"
#define COMMAND_BINDING_CLOSE_WINDOW "CloseWindow"
#define COMMAND_BINDING_UPDATE_MODEL "UpdateModel"

END_C_DECLS

#endif /*TK_COMMAND_BINDING_H*/
