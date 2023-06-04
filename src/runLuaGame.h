#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <FS.h>
#include <LovyanGFX_DentaroUI.hpp>

#include "SPIFFS.h"
#include "baseGame.h"
#include "Tunes.h"

#include <bitset>
#include <iostream>
#include <fstream>

extern "C"{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#ifndef RUN_LUA_GAME_H
#define RUN_LUA_GAME_H

#define MAX_CHAR 1024

// #define LUA_BUFFERSIZE 1024
// #define LUA_BUFFERSIZE_PS (LUA_BUFFERSIZE * 2)

// #define LUA_BUFSIZE 2048

struct LoadF{
  File f;
  char buf[MAX_CHAR];
};

inline uint16_t lua_rgb24to16(uint8_t r, uint8_t g, uint8_t b) {
  uint16_t tmp = ((r>>3) << 11) | ((g>>2) << 5) | (b>>3);
  return tmp; //(tmp >> 8) | (tmp << 8);
}

class RunLuaGame: public BaseGame
{//クラスはデフォルトではprivata

  public:
    enum WifiPhase{
      NONE,
      SELECT,
      SHOW,
      RUN
    };

    lua_State* L;
    luaL_Buffer b;
    byte col[3] = {0,0,0};
    // int frame = 0;
    int buttonState[CTRLBTNNUM];//ボタンの個数未定
    int tp[2] ={0,0};
    uint16_t palette[256];

    // bool wifiDebugRequest = true;//外部ファイルから書き換えテifiモードにできる
    // bool wifiDebugSelf = false;

    WifiPhase wifiMode = NONE;
    int modeSelect = 0;
    bool exitRequest = false;
    bool runError = false;
    String errorString;

    // int gameState = 0;

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

    int loadSurface(File* fp, uint8_t* buf);
    static int l_tp(lua_State* L);
    static int l_tstat(lua_State* L);
    static int l_tone(lua_State* L);
    static int l_spr(lua_State* L);
    static int l_scroll(lua_State* L);
    static int l_pset(lua_State* L);
    static int l_pget(lua_State* L);
    static int l_color(lua_State* L);
    static int l_text(lua_State* L);
    static int l_opmode(lua_State* L);
    static int l_drawrect(lua_State* L);
    static int l_fillrect(lua_State* L);
    static int l_fillcircle(lua_State* L);
    static int l_drawcircle(lua_State* L);
    static int l_drawtri(lua_State* L);
    static int l_filltri(lua_State* L);
    static int l_btn(lua_State* L);
    static int l_btnp(lua_State* L);
    static int l_sldr(lua_State* L);
    static int l_getip(lua_State* L);
    static int l_iswifidebug(lua_State* L);
    static int l_wifiserve(lua_State* L);
    static int l_run(lua_State* L);
    static int l_list(lua_State* L);
    static int l_require(lua_State* L);
    static int l_httpsget(lua_State* L);
    static int l_httpsgetfile(lua_State* L);
    static int l_savebmp(lua_State* L);
    static int l_reboot(lua_State* L);
    static int l_debug(lua_State* L);

    String getBitmapName(String s);
    String getPngName(String s);
    void hsbToRgb(float angle, float si, float br, int& r, int& g, int& b);
    void fillFastTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint16_t c1);

    //継承先の関数を優先するものにはvirtual
    virtual void haco8resume(){};//派生クラスに書き換えられるダミー関数
    //派生クラスでのみ実行されるダミー関数（このクラスでは何の処理もしていない）

    void resume();
    void init();
    int run(int _remainTime);
    void pause();
  
    protected://継承先でも使えるもの
};

#endif
