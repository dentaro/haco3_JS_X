#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <FS.h>
#include <LovyanGFX_DentaroUI.hpp>

//esp-idfのライブラリを使う！
//https://qiita.com/norippy_i/items/0ed46e06427a1d574625
#include <driver/adc.h>//このヘッダファイルを忘れないように！！

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
    duk_context* ctx;
    byte col[3] = {0,0,0};
    int buttonState[CTRLBTNNUM];
    int tp[2] ={0,0};
    // bool wifiDebugRequest = false;
    // bool wifiDebugSelf = false;
    int modeSelect = 0;
    bool exitRequest = false;
    bool runError = false;
    String errorString;
    

    int loadSurface(File* fp, uint8_t* buf);
    static int l_tone(duk_context* ctx);
    static int l_spr(duk_context* ctx);
    static int l_pset(duk_context* ctx);
    static int l_gcn(duk_context* ctx);
    static int l_color(duk_context* ctx);
    static int l_text(duk_context* ctx);
    static int l_drawrect(duk_context* ctx);
    static int l_fillrect(duk_context* ctx);
    static int l_drawcircle(duk_context* ctx);
    static int l_fillcircle(duk_context* ctx);
    static int l_btn(duk_context* ctx);
    static int l_tp(duk_context* ctx);
    static int l_str(duk_context* ctx);
    static int l_tbtns(duk_context* ctx);
    static int l_rwtb(duk_context* ctx);
    static int l_tbstate(duk_context* ctx);
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

   //haco3Lib
  //  String l_intToString(int _val);
   //haco3Lib

    String getBitmapName(String s);
    String getPngName(String s);
    void init();
    int run(int remainTime);
    void pause();
    void resume();
};

#endif
