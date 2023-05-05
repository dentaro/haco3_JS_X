#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <FS.h>
#include <LovyanGFX_DentaroUI.hpp>

#include "SPIFFS.h"
#include "baseGame.h"
#include "runLuaGame.h"
#include "Tunes.h"

extern "C"{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#ifndef RUN_HACO8_GAME_H
#define RUN_HACO8_GAME_H

#define MAPWH 16//マップのpixelサイズ

// #define MAX_CHAR 256

// struct LoadF{
//   File f;
//   char buf[MAX_CHAR];
// };

// inline uint16_t lua_rgb24to16(uint8_t r, uint8_t g, uint8_t b) {
//   uint16_t tmp = ((r>>3) << 11) | ((g>>2) << 5) | (b>>3);
//   return tmp; //(tmp >> 8) | (tmp << 8);
// }

class RunHaco8Game: public RunLuaGame
{
  public:

//24bitRGB
  uint8_t clist[16][3] =
  {
  { 0,0,0},//0: 黒色
  { 27,42,86 },//1: 暗い青色
  { 137,24,84 },//2: 暗い紫色
  { 0,139,75 },//3: 暗い緑色
  { 183,76,45 },//4: 茶色
  { 97,87,78 },//5: 暗い灰色
  { 194,195,199 },//6: 明るい灰色
  { 255,241,231 },//7: 白色
  { 255,0,70 },//8: 赤色
  { 255,160,0 },//9: オレンジ
  { 255,238,0 },//10: 黄色
  { 0,234,0 },//11: 緑色
  { 0,173,255 },//12: 水色
  { 134,116,159 },//13: 藍色
  { 255,107,169 },//14: ピンク
  { 255,202,165}//15: 桃色
  };

  float matrix_spr[6] = {2.0,   // 横2倍
                     -0.0,  // 横傾き
                     0.0,   // X座標
                     0.0,   // 縦傾き
                     2.0,   // 縦2倍
                     0.0    // Y座標
                    };

  RunHaco8Game(int _gameState, String _mn);//ゲームの状態を立ち上げ時に渡す

  static int l_cls(lua_State* L);
  static int l_rnd(lua_State* L);
  static int l_flr(lua_State* L);
  static int l_max(lua_State* L);
  static int l_min(lua_State* L);
  static int l_go2(lua_State* L);
  static int l_gstat(lua_State* L);
  static int l_ceil(lua_State* L);
  static int l_fget(lua_State* L);
  static int l_fset(lua_State* L);
  static int l_sfx(lua_State* L);
  static int l_music(lua_State* L);
  static int l_circfill(lua_State* L);
  static int l_circ(lua_State* L);
  static int l_rectfill(lua_State* L);
  static int l_rect(lua_State* L);
  static int l_line(lua_State* L);
  static int l_print(lua_State* L);
  static int l_fillcircle2(lua_State* L);
  static int l_map(lua_State* L);
  static int l_mget(lua_State* L);
  static int l_spr8(lua_State* L);
  // static int l_add(lua_State* L);
  // static int l_del(lua_State* L);
  
  uint16_t getColor(int _cn, int _rgb);
  void haco8resume();//この関数で基底クラスのダミー関数を置き換えることで、派生クラスの処理を挿入する
  
};

#endif
