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
    // int frame = 0;
    int buttonState[CTRLBTNNUM];
    int tp[2] ={0,0};
    // bool wifiDebugRequest = false;
    // bool wifiDebugSelf = false;
    int modeSelect = 0;
    bool exitRequest = false;
    bool runError = false;
    String errorString;

    // bool firstLoopF = true;
    
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
    static int l_tone(duk_context* ctx);
    static int l_spr(duk_context* ctx);
    static int l_bg(duk_context* ctx);
    static int l_pset(duk_context* ctx);
    static int l_gcn(duk_context* ctx);
    static int l_color(duk_context* ctx);
    static int l_text(duk_context* ctx);
    static int l_gtx(duk_context* ctx);
    static int l_opmode(duk_context* ctx);
    static int l_drawrect(duk_context* ctx);
    static int l_fillrect(duk_context* ctx);
    static int l_drawcircle(duk_context* ctx);
    static int l_fillcircle(duk_context* ctx);
    static int l_btn(duk_context* ctx);
    static int l_tp(duk_context* ctx);
    static int l_str(duk_context* ctx);
    static int l_tbtns(duk_context* ctx);
    static int l_rwtb(duk_context* ctx);

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

    static String getBitmapName(String s);
    static String getPngName(String s);
    
    void init();
    int run(int remainTime);
    void pause();
    void resume();
};

#endif
