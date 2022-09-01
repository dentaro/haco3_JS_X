#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <FS.h>
#include <LovyanGFX_DentaroUI.hpp>

#include "SPIFFS.h"
#include "baseGame.h"
#include "Tunes.h"


// #include <Arduino.h>

// #include <WiFi.h>
// #include <WiFiClientSecure.h>
// #include <FS.h>
// #ifdef M5STACK
// #include "M5Stack.h"
// #else
// #include <TFT_eSPI.h>
// #endif


#include "SPIFFS.h"
#include "baseGame.h"
#include "Tunes.h"

// #include "runLuaGame.h"//lua_IntegerなどのCとの変数受け渡しのためだけに読み込んでる
#include "../lib/lua-5.3/lua.h"//メモリ足らんのでこっちでlua_Integer入ってるファイルだけ最低限読み込む

#include <duk_config.h>
#include <duktape.h>

#ifndef RUN_JS_GAME_H
#define RUN_JS_GAME_H

#define MAX_CHAR 256


//Luaと共通なもの（baseGameに持っていきたい）

inline uint16_t rgb24to16(uint8_t r, uint8_t g, uint8_t b) {
  uint16_t tmp = ((r>>3) << 11) | ((g>>2) << 5) | (b>>3);
  return tmp; //(tmp >> 8) | (tmp << 8);
}


class RunJsGame: public BaseGame
{
  public:
    enum WifiPhase{
      NONE,
      SELECT,
      SHOW,
      RUN
    };

    duk_context* ctx;
    byte col[3] = {0,0,0};
    int buttonState[7];
    uint8_t surface[128][128]; //16.3kb
    uint16_t palette[256];
    bool wifiDebugRequest = false;
    bool wifiDebugSelf = false;
    WifiPhase wifiMode = NONE;
    int modeSelect = 0;
    bool exitRequest = false;
    bool runError = false;
    String errorString;

    int loadSurface(File* fp, uint8_t* buf);
    static int l_tone(duk_context* ctx);
    static int l_spr(duk_context* ctx);
    static int l_pset(duk_context* ctx);
    /*
    static int l_pget(duk_context* ctx);
    */
    static int l_color(duk_context* ctx);
    static int l_text(duk_context* ctx);
    static int l_drawrect(duk_context* ctx);
    static int l_fillrect(duk_context* ctx);
    /*
u   static int l_fillcircle(duk_context* ctx);
    */
    static int l_btn(duk_context* ctx);
    /*
    static int l_getip(duk_context* ctx);
    static int l_iswifidebug(duk_context* ctx);
    static int l_wifiserve(duk_context* ctx);
    static int l_run(duk_context* ctx);
    static int l_list(duk_context* ctx);
    static int l_require(duk_context* ctx);
    static int l_httpsget(duk_context* ctx);
    static int l_httpsgetfile(duk_context* ctx);
    static int l_savebmp(duk_context* ctx);
    static int l_reboot(duk_context* ctx);
    static int l_debug(duk_context* ctx);
    */

    String getBitmapName(String s);
    void init();
    int run(int remainTime);
    void pause();
    void resume();
};

#endif
