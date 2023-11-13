#include <Arduino.h>

// #include <WiFi.h>
// #include <WiFiClientSecure.h>
#include <FS.h>
#include <LovyanGFX_DentaroUI.hpp>

#include "SPIFFS.h"
#include "baseGame.h"
#include "runLuaGame.h"
// #include "Tunes.h"

#include <string>
#include <sstream>

#include <map>

extern "C"{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#ifndef RUN_HACO8_GAME_H
#define RUN_HACO8_GAME_H

#define MAPWH 16//マップのpixelサイズ

#define BUF_PNG_NUM 0

#define TFT_WIDTH 160
#define TFT_HEIGHT 128
#define TFT_WIDTH_HALF 80
#define TFT_HEIGHT_HALF 64

#define VEC_FRAME_COUNT 10

#define STARS_NUM 1

struct Intersection {
    float x;
    float y;
    float distance;
    float mapheight;
    int colangle;
};

class RunHaco8Game: public RunLuaGame
{
  public:

  void fillFastTriangle(float x0, float y0, float x1, float y1, float x2, float y2, uint16_t c1);

    
  struct Rect2D {
    int x;
    int y;
    int w;
    int h;
    Rect2D(int _x, int _y, int _w, int _h) : x(_x), y(_y), w(_w), h(_h) {}
  };

  struct CameraObj {
    float x;
    float y;
    float z;
    float x2;
    float y2;
    float z2;
    float anglex;
    float angley;
    float anglez;
    float zoom;

    // コンストラクタで初期値を設定
    CameraObj()
        : x(0), y(0), z(0), x2(0), y2(0), z2(0), anglex(0), angley(0), anglez(0), zoom(1.0)
    {
    }
  };

  struct LightObj {
    float x;
    float y;
    float z;

    // コンストラクタで初期値を設定
    LightObj()
        : x(0), y(0), z(0)
    {
    }
  };

  struct CubeObj {
    float x;
    float y;
    float z;
    float angle;
    float size;
    int colangle;
    int width;
    int height;
    // コンストラクタで初期値を設定
    CubeObj()
        : x(0), y(0), z(0), angle(0), size(1.0),colangle(120), width(10), height(10)
    {
    }
};

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

  static constexpr int HACO3_C0    = 0x0000;
  static constexpr int HACO3_C1    = 6474;//27,42,86 
  static constexpr int HACO3_C2    = 35018;
  static constexpr int HACO3_C3    = 1097;
  static constexpr int HACO3_C4    = 45669;
  static constexpr int HACO3_C5    = 25257;
  static constexpr int HACO3_C6    = 50712;
  static constexpr int HACO3_C7    = 65436;
  static constexpr int HACO3_C8    = 63496;//0xF802;
  static constexpr int HACO3_C9    = 64768;
  static constexpr int HACO3_C10    = 65376;
  static constexpr int HACO3_C11    = 1856;
  static constexpr int HACO3_C12    = 1407;
  static constexpr int HACO3_C13    = 33715;
  static constexpr int HACO3_C14    = 64341;
  static constexpr int HACO3_C15    = 65108;


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
  static int l_srnd(lua_State* L);
  static int l_flr(lua_State* L);
  static int l_max(lua_State* L);
  static int l_mid(lua_State* L);
  static int l_min(lua_State* L);
  static int l_abs(lua_State* L);
  static int l_sqrt(lua_State* L);
  static int l_distance(lua_State* L);
  static int l_sin(lua_State* L);
  static int l_cos(lua_State* L);
  static int l_atan2(lua_State* L);
  
  // static int l_gsin(lua_State* L);
  // static int l_gcos(lua_State* L);

  static int l_band(lua_State* L);
  static int l_bnot(lua_State* L);
  static int l_bor(lua_State* L);
  static int l_bxor(lua_State* L);

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
  static int l_t(lua_State* L);
  static int l_fillcircle2(lua_State* L);
  static int l_map(lua_State* L);
  static int l_mget(lua_State* L);
  static int l_spr8(lua_State* L);

  // static int l_del(lua_State* L);
  static int l_sgn(lua_State* L);
  static int l_shl(lua_State* L);
  static int l_shr(lua_State* L);

  //3D関連
  static int l_getstl(lua_State* L);
  static int l_setply(lua_State* L);
  static int l_rendr(lua_State* L);

  static int l_drawstars(lua_State* L);
  static int l_starp(lua_State* L);

  static int l_creobj(lua_State* L);
  static int l_lig(lua_State* L);
  static int l_cam(lua_State* L);
  static int l_trans(lua_State* L);

  //fps関連
  static int l_wini(lua_State* L);
  static int l_wset(lua_State* L);
  static int l_wdraw(lua_State* L);

  static int l_gcol(lua_State* L);
  static int l_sp3d(lua_State* L);
  static int l_spmap(lua_State* L);
  static int l_drawpng(lua_State* L);
  static int l_initstars(lua_State* L);

  static int l_drawstars2(lua_State* L);
  static int l_drawmaps(lua_State* L);
  static int l_getmappos(lua_State* L);
  static int l_getgpos(lua_State* L);
  
  // ベクトルの長さを計算する関数
  float calculateLength(float x, float y, float z);

  // ベクトルの正規化を行う関数
  Vector3<float> normalize(float x, float y, float z);

  // 2つのベクトルの内積を計算する関数
  float calculateDotProduct(const Vector3<float>& v1, const Vector3<float>& v2);

  // 3つの頂点から法線ベクトルを計算する関数
  Vector3<float> calculateNormal(const Vector3<float>& v1, const Vector3<float>& v2, const Vector3<float>& v3);

  // ポリゴンの明るさを計算する関数
  float calculateBrightness(const Vector3<float>& v1, const Vector3<float>& v2, const Vector3<float>& v3, const LightObj& light);

  // void renderPolygon(lua_State* L, const std::vector<std::vector<float>>& polygonData);
  // void renderPolygon(const std::vector<std::vector<float>>& polygonData, int colangle);
  
  // void renderPolygon(lua_State* L, const std::vector<std::vector<float>>& polygonData);

  // void getVertices(lua_State* L, int tableIndex, Vertex& v1, Vertex& v2, Vertex& v3);
  // void fill3DFastTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t c1);
  // void fastPoint(const Vector3<float> v1, uint16_t c1);
  void fastPoint(const Vector3<float> v1, int starbrightness, int noInt);

  void fill3DFastTriangle(int16_t x0, int16_t y0, int16_t z0,int16_t x1, int16_t y1, int16_t z1,int16_t x2, int16_t y2, int16_t z2,uint16_t c1);
  void triangle3D(const Vector3<float> v1,const Vector3<float> v2,const Vector3<float> v3);
  // void point3D(const Vector3<float> v1,uint16_t c1);
  void line3D(const Vector3<float> v1,const Vector3<float> v2,uint16_t c1);
  

  // uint16_t getColor(int _cn, int _rgb);

  void haco8resume();//この関数で基底クラスのダミー関数を置き換えることで、派生クラスの処理を挿入する
  
};

#endif
