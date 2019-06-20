/**
 * File:   demo_main.c
 * Author: AWTK Develop Team
 * Brief:  demo main
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-02-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "mvvm/mvvm.h"
extern ret_t assets_init(void);
extern ret_t application_init(void);

#ifdef USE_GUI_MAIN
int gui_app_start(int lcd_w, int lcd_h) {
  tk_init(lcd_w, lcd_h, APP_MOBILE, NULL, NULL);
#elif defined(WIN32)
#include <windows.h>
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
  char app_root[MAX_PATH + 1];
  path_app_root(app_root);
  tk_init(320, 480, APP_SIMULATOR, NULL, app_root);
#elif defined(WIN32)
#else
int main(void) {
  char app_root[MAX_PATH + 1];
  path_app_root(app_root);
  tk_init(320, 480, APP_SIMULATOR, NULL, app_root);
#endif

//#define WITH_LCD_PORTRAIT 1
#if defined(USE_GUI_MAIN) && defined(WITH_LCD_PORTRAIT)
  if (lcd_w > lcd_h) {
    tk_set_lcd_orientation(LCD_ORIENTATION_90);
  }
#endif /*WITH_LCD_PORTRAIT*/

#ifdef WITH_LCD_LANDSCAPE
  if (lcd_w < lcd_h) {
    tk_set_lcd_orientation(LCD_ORIENTATION_90);
  }
#endif /*WITH_LCD_PORTRAIT*/

#if defined(WIN32)
#if !defined(NDEBUG)
  {
    AllocConsole();
    FILE* fp = NULL;
    freopen_s(&fp, "CONOUT$", "w+t", stdout);
  }
#endif /*NDEBUG*/
#endif /*WIN32*/

  tk_ext_widgets_init();

  assets_init();
  mvvm_init();
  application_init();

  tk_run();

  mvvm_deinit();

  return 0;
}
