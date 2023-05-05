#include "runHaco8Game.h"
extern MyTFT_eSprite tft;
extern LGFX_Sprite sprite64;
extern LGFX_Sprite sprite88_roi;
extern String appfileName;
extern void startWifiDebug(bool isSelf);
extern void setFileName(String s);
extern void runFileName(String s);
extern bool isWifiDebug();
extern void reboot();
extern Tunes tunes;
extern int pressedBtnID;
extern LovyanGFX_DentaroUI ui;
extern int outputMode;
extern int mode;
extern int gameState;
extern String mapFileName;
extern int soundNo;
extern int musicNo;
extern bool musicflag;
extern bool sfxflag;
extern void readMap();


extern uint8_t mapArray[MAPWH][MAPWH];

extern int8_t sprbits[128];//fgetでアクセスするスプライト属性を格納するための配列

RunHaco8Game::RunHaco8Game(int _gameState, String _mn){
  gameState = _gameState;
  mapFileName = _mn;
}

void RunHaco8Game::haco8resume()
{
  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_cls, 1);
  lua_setglobal(L, "cls");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_rnd, 1);
  lua_setglobal(L, "rnd");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_flr, 1);
  lua_setglobal(L, "flr");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_min, 1);
  lua_setglobal(L, "min");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_mid, 1);
  lua_setglobal(L, "mid");


  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_max, 1);
  lua_setglobal(L, "max");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_min, 1);
  lua_setglobal(L, "min");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_abs, 1);
  lua_setglobal(L, "abs");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_sqrt, 1);
  lua_setglobal(L, "sqrt");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_sin, 1);
  lua_setglobal(L, "sin");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_cos, 1);
  lua_setglobal(L, "cos");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_atan2, 1);
  lua_setglobal(L, "atan2");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_go2, 1);
  lua_setglobal(L, "go2");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_gstat, 1);
  lua_setglobal(L, "gstat");
  
  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_ceil, 1);
  lua_setglobal(L, "ceil");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_fget, 1);
  lua_setglobal(L, "fget");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_fset, 1);
  lua_setglobal(L, "fset");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_sfx, 1);
  lua_setglobal(L, "sfx");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_music, 1);
  lua_setglobal(L, "music");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_map, 1);
  lua_setglobal(L, "map");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_mget, 1);
  lua_setglobal(L, "mget");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_spr8, 1);
  lua_setglobal(L, "spr8");

  // lua_pushlightuserdata(L, this);
  // lua_pushcclosure(L, l_add, 1);
  // lua_setglobal(L, "add");

  // lua_pushlightuserdata(L, this);
  // lua_pushcclosure(L, l_del, 1);
  // lua_setglobal(L, "del");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_circfill, 1);
  lua_setglobal(L, "circfill");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_circ, 1);
  lua_setglobal(L, "circ");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_rectfill, 1);
  lua_setglobal(L, "rectfill");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_rect, 1);
  lua_setglobal(L, "rect");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_line, 1);
  lua_setglobal(L, "line");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_fillcircle2, 1);
  lua_setglobal(L, "fillcircle2");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_print, 1);
  lua_setglobal(L, "print");

  // gameState = _gameState;

  //スプライト属性を設定する
  // sprbits[20] = 0b11111111;//黒
  // sprbits[42] = 0b00000000;//草原
  // sprbits[49] = 0b00000001;//壁上
  // sprbits[50] = 0b00000001;//壁横
  
}

int RunHaco8Game::l_mget(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int celx = lua_tointeger(L, 1);
  int cely = lua_tointeger(L, 2);
  lua_pushinteger(L, mapArray[celx][cely]);
  return 1;
}

int RunHaco8Game::l_spr8(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int n = lua_tointeger(L, 1);
  int x = lua_tointeger(L, 2);
  int y = lua_tointeger(L, 3);
  int gw = lua_tointeger(L, 4);
  int gh = lua_tointeger(L, 5);
  int scalex = lua_tointeger(L, 6);
  int scaley = lua_tointeger(L, 7);
  int angle = lua_tointeger(L, 8);

  if( gw==NULL && gh==NULL )
  {
    gw = 1;
    gh = 1;
  }

  int w = gw*8;
  int h = gh*8;
  
  sprite88_roi.clear();//指定の大きさにスプライトを作り直す
  sprite88_roi.createSprite(w,h);

  int spr8numX = 8;//スプライトシートに並ぶｘ、ｙの個数
  int spr8numY = 8;

  int sx = ((n-1)%spr8numX); //0~7
  int sy = ((n-1)/spr8numY); //整数の割り算はintにいれると切り捨てされる

  sprite64.pushSprite(&sprite88_roi, -(sx*8), -(sy*8));//64*64のpngデータを指定位置までずらす

  sprite88_roi.setPivot(w/2.0, h/2.0);

  if(scalex == NULL && scaley==NULL && angle == NULL){
    sprite88_roi.pushRotateZoom(&tft, x, y, 0, 1, 1, TFT_BLACK);
  }else  if(scalex != NULL && scaley!=NULL && angle == NULL){
    sprite88_roi.pushRotateZoom(&tft, x, y, 0, scalex, scaley, TFT_BLACK);
  }else  if(scalex != NULL && scaley!=NULL && angle != NULL){
    sprite88_roi.pushRotateZoom(&tft, x, y, angle, scalex, scaley, TFT_BLACK);
  }
  return 0;
}

//luaテーブル操作
// int RunHaco8Game::l_add(lua_State* L){
//   RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
//   // int t = lua_tointeger(L, 1);
//   int v = lua_tointeger(L, 2);
//   int n = lua_rawlen(L, -1); // テーブルの要素数を取得
//   lua_pushinteger(L, v); // 値をスタックにプッシュ
//   lua_rawseti(L, -2, n + 1); // 値をテーブルに追加
//   lua_pop(L, 1); // テーブルをポップ
    
//   return 1;
// }



// int RunHaco8Game::l_del(lua_State* L, int index, lua_Integer v){

//   return 1;
// }

// int RunHaco8Game::l_add(lua_State* L) {
  // int num_tables = 2;//2つのテーブルの結合
  // RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  //   int total_len = 0;
  //   for (int i = 1; i <= num_tables; i++) {
  //       if (!lua_istable(L, i)) {
  //           luaL_error(L, "Argument #%d is not a table", i);
  //       }
  //       total_len += lua_rawlen(L, i);
  //   }

  //   lua_createtable(L, total_len, 0);
  //   int table_index = 1;

  //   for (int i = 1; i <= num_tables; i++) {
  //       lua_pushvalue(L, i);
  //       lua_pushstring(L, "");
  //       int sep_index = lua_gettop(L);
  //       int table_len = lua_rawlen(L, i);

  //       for (int j = 1; j <= table_len; j++) {
  //           lua_rawgeti(L, i, j);
  //           lua_pushvalue(L, sep_index);
  //           lua_call(L, 2, 1);
  //           const char* str = lua_tostring(L, -1);
  //           lua_pop(L, 1);
  //           lua_pushstring(L, str);
  //           lua_rawseti(L, -2, table_index++);
  //       }
  //   }

  //   return 1;
// }


int RunHaco8Game::l_map(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int mx = lua_tointeger(L, 1);
  int my = lua_tointeger(L, 2);
  int roix = lua_tointeger(L, 3);
  int roiy = lua_tointeger(L, 4);
  int roiW = lua_tointeger(L, 5);
  int roiH = lua_tointeger(L, 6);

  String fn = lua_tostring(L, 7);
  if(fn != NULL){
    mapFileName = fn;
  }

  sprite88_roi.clear();//指定の大きさにスプライトを作り直す
  sprite88_roi.createSprite(8,8);

  int spr8numX = 8;//スプライトシートに並ぶ８＊８スプライトの個数
  int spr8numY = 8;

  for(int m=0;m<roiH;m++){
      for(int n=0;n<roiW;n++){
          int sprno = mapArray[my+n][mx+m];
          
            int sx = ((sprno-1)%spr8numX); //0~7
            int sy = ((sprno-1)/spr8numY); //整数の割り算は自動で切り捨てされる
            
            sprite64.pushSprite(&sprite88_roi, -8*(sx), -8*(sy));//128*128のpngデータを指定位置までずらす
            sprite88_roi.pushRotateZoom(&tft, roix+n*8+4, roiy+m*8+4, 0, 1, 1, TFT_BLACK);//なぜか４を足さないとずれる要修正
      }
  }
  return 0;
}

int RunHaco8Game::l_fget(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int sprno = lua_tointeger(L, 1);
  int fbitno = lua_tointeger(L, 2);
  bool b = (sprbits[sprno]>>fbitno)&1;//fbitnoビットシフトしてから，最下位ビットのみを取り出す
  lua_pushboolean(L, b);
  return 1;
}

int RunHaco8Game::l_fset(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int8_t sprno = lua_tointeger(L, 1);
  int8_t fbitno = lua_tointeger(L, 2);
  int8_t val = lua_tointeger(L, 3);
  int8_t bitfilter = 0b00000001<<fbitno;
       if(val == 0)sprbits[sprno] &= ~bitfilter;//スプライト番号sprnoのfbitno番目を0に
  else if(val == 1)sprbits[sprno] |=  bitfilter;//スプライト番号sprnoのfbitno番目を1に
  return 0;
}

int RunHaco8Game::l_sfx(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  sfxflag = true;
  int8_t sn = lua_tointeger(L, 1);
  soundNo = sn;
  return 0;
}

int RunHaco8Game::l_music(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  
  int8_t mn = lua_tointeger(L, 1);
  if(mn == -1){
    musicflag = false;
  }else{
    musicflag = true;
  }
  musicNo = mn;
  return 0;
}

int RunHaco8Game::l_ceil(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  float n = lua_tonumber(L, 1);//
  int in =  (int)ceil(n);
  lua_pushinteger(L, in);
  return 1;
}

int RunHaco8Game::l_flr(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int n = lua_tonumber(L, 1);//intに入れた時点でfloorされてる
  // n =  (int)floor(n);
  lua_pushinteger(L, n);
  return 1;
}

int RunHaco8Game::l_max(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  double a = lua_tonumber(L, 1);
  double b = lua_tonumber(L, 2);
  if (a > b){
    lua_pushnumber(L, a);
  }else{
    lua_pushnumber(L, b);
  }
  return 1;
}

int RunHaco8Game::l_mid(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  double x = lua_tonumber(L, 1);
  double y = lua_tonumber(L, 2);
  double z = lua_tonumber(L, 3);

      if ((x <= y && y <= z) || (z <= y && y <= x)) {
          lua_pushnumber(L, y);
      }
      else if ((y <= x && x <= z) || (z <= x && x <= y)) {
          lua_pushnumber(L, x);
      }
      else {
          lua_pushnumber(L, z);
      }
  return 1;
}


int RunHaco8Game::l_min(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  double a = lua_tonumber(L, 1);
  double b = lua_tonumber(L, 2);
  if (a < b){
    lua_pushnumber(L, a);
  }else{
    lua_pushnumber(L, b);
  }
  return 1;
}


int RunHaco8Game::l_abs(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  double x = lua_tonumber(L, 1);
  if (x < 0) {
        lua_pushnumber(L, -x);
    } else {
        lua_pushnumber(L, x);
    }
  return 1;
}

int RunHaco8Game::l_sqrt(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  double x = lua_tonumber(L, 1);
  lua_pushnumber(L, sqrt(x));
  return 1;
}

int RunHaco8Game::l_sin(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  double x = lua_tonumber(L, 1);
  lua_pushnumber(L, sin(x));
  return 1;
}

int RunHaco8Game::l_cos(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  double x = lua_tonumber(L, 1);
  lua_pushnumber(L, cos(x));
  return 1;
}

int RunHaco8Game::l_atan2(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  double y = lua_tonumber(L, 1);//intに入れた時点でfloorされてる
  double x = lua_tonumber(L, 2);//intに入れた時点でfloorされてる
  lua_pushnumber(L, atan2(y, x));
  return 1;
}

int RunHaco8Game::l_go2(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  appfileName = lua_tostring(L, 1);
  int gs = lua_tointeger(L, 2);
  gameState = 0;
  if(gs != NULL){
    gameState = gs;
  }
  // appfileName = (String)text;
  // setFileName(appfileName);
  // appfileName = text;
  // runFileName(appfileName);
  // mode = 1;//appfileNameのゲームを起動させるために1モードを1に
  // pressedBtnID = 100;
  pressedBtnID = 9999;
  return 1;
}

int RunHaco8Game::l_gstat(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  lua_pushinteger(L, gameState);
  return 1;
}

int RunHaco8Game::l_rnd(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int n = lua_tointeger(L, 1);
  n =  rand() % n;
  lua_pushinteger(L, n);
  return 1;
}

// uint8_t clist[16][3] =
//   {
//   { 0,0,0},//0: 黒色
//   { 27,42,86 },//1: 暗い青色
//   { 137,24,84 },//2: 暗い紫色
//   { 0,139,75 },//3: 暗い緑色
//   { 183,76,45 },//4: 茶色
//   { 97,87,78 },//5: 暗い灰色
//   { 194,195,199 },//6: 明るい灰色
//   { 255,241,231 },//7: 白色
//   { 255,0,70 },//8: 赤色
//   { 255,160,0 },//9: オレンジ
//   { 255,238,0 },//10: 黄色
//   { 0,234,0 },//11: 緑色
//   { 0,173,255 },//12: 水色
//   { 134,116,159 },//13: 藍色
//   { 255,107,169 },//14: ピンク
//   { 255,202,165}//15: 桃色
//   };

// int RunHaco8Game::getColor(int _cn, int _rgb){
//   self->col[0] = clist[_cn][0]; // 5bit
//   self->col[1] = clist[_cn][1]; // 6bit
//   self->col[2] = clist[_cn][2];       // 5bit
// }

int RunHaco8Game::l_print(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  const char* text = lua_tostring(L, 1);
  int x = lua_tointeger(L, 2);
  int y = lua_tointeger(L, 3);
  int cn = lua_tointeger(L, 4);

  if(cn != NULL){
    self->col[0] = self->clist[cn][0]; // 5bit
    self->col[1] = self->clist[cn][1]; // 6bit
    self->col[2] = self->clist[cn][2]; 
  }

  if(x == NULL && y == NULL && cn == NULL){//位置指定なしの場合
    // tft.setTextColor(lua_rgb24to16(self->col[0], self->col[1], self->col[2]),TFT_BLACK);//文字背景あり
    tft.setTextColor(TFT_WHITE,TFT_BLACK);//文字背景あり
    tft.setClipRect(0, 0, 128, 120);
    //tft.setCursor(0,0);
  }else if(x != NULL && y != NULL && cn == NULL){//位置指定ありの場合
    tft.setTextColor(lua_rgb24to16(self->col[0], self->col[1], self->col[2]));//文字背景を消せる
    tft.setCursor(x,y);
    tft.setClipRect(x, y, 128-x, 128-y);
  }else if(x != NULL && y != NULL && cn != NULL){//位置指定、、色指定ありの場合
    tft.setTextColor(lua_rgb24to16(self->col[0], self->col[1], self->col[2]));//文字背景を消せる
    tft.setCursor(x,y);
    tft.setClipRect(x, y, 128-x, 128-y);
  }
  // tft.setCursor(x,y);
  // self->col[0] = self->clist[cn][0]; // 5bit
  // self->col[1] = self->clist[cn][1]; // 6bit
  // self->col[2] = self->clist[cn][2];       // 5bit
  // tft.setTextColor(lua_rgb24to16(self->col[0], self->col[1], self->col[2]));//文字背景を消せる
  tft.println(text);
  tft.clearClipRect();
  return 0;
}

// uint8_t clist[16][3] =
//   {
//   { 0,0,0},//0: 黒色
//   { 27,42,86 },//1: 暗い青色
//   { 137,24,84 },//2: 暗い紫色
//   { 0,139,75 },//3: 暗い緑色
//   { 183,76,45 },//4: 茶色
//   { 97,87,78 },//5: 暗い灰色
//   { 194,195,199 },//6: 明るい灰色
//   { 255,241,231 },//7: 白色
//   { 255,0,70 },//8: 赤色
//   { 255,160,0 },//9: オレンジ
//   { 255,238,0 },//10: 黄色
//   { 0,234,0 },//11: 緑色
//   { 0,173,255 },//12: 水色
//   { 134,116,159 },//13: 藍色
//   { 255,107,169 },//14: ピンク
//   { 255,202,165}//15: 桃色
//   };

int RunHaco8Game::l_line(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));

  double xa = lua_tonumber(L, 1);
  double ya = lua_tonumber(L, 2);
  double xb = lua_tonumber(L, 3);
  double yb = lua_tonumber(L, 4);
  int    cn = lua_tointeger(L, 5);
  
  // int xa = lua_tointeger(L, 1);
  // int ya = lua_tointeger(L, 2);
  // int xb = lua_tointeger(L, 3);
  // int yb = lua_tointeger(L, 4);
  // int cn = lua_tointeger(L, 5);

  if(cn != NULL)
  {
    self->col[0] = self->clist[cn][0]; // 5bit
    self->col[1] = self->clist[cn][1]; // 6bit
    self->col[2] = self->clist[cn][2]; // 5bit
  }
  tft.drawLine(xa,ya,xb,yb, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  return 0;
}

int RunHaco8Game::l_rect(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int xa = lua_tointeger(L, 1);
  int ya = lua_tointeger(L, 2);
  int xb = lua_tointeger(L, 3);
  int yb = lua_tointeger(L, 4);
  int cn = lua_tointeger(L, 5);
  if(cn != NULL)
  {
  self->col[0] = self->clist[cn][0]; // 5bit
  self->col[1] = self->clist[cn][1]; // 6bit
  self->col[2] = self->clist[cn][2]; // 5bit
  }
  tft.drawRect(xa, ya, xb, yb, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  return 0;
}

int RunHaco8Game::l_rectfill(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int xa = lua_tointeger(L, 1);
  int ya = lua_tointeger(L, 2);
  int xb = lua_tointeger(L, 3);
  int yb = lua_tointeger(L, 4);
  int cn = lua_tointeger(L, 5);
  if(cn != NULL)
  {
  self->col[0] = self->clist[cn][0]; // 5bit
  self->col[1] = self->clist[cn][1]; // 6bit
  self->col[2] = self->clist[cn][2]; // 5bit
  }
  tft.fillRect(xa, ya, xb, yb, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  return 0;
}

int RunHaco8Game::l_circ(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  int rr = lua_tointeger(L, 3);
  int cn = lua_tointeger(L, 4);
  if(cn != NULL)
  {
  self->col[0] = self->clist[cn][0]; // 5bit
  self->col[1] = self->clist[cn][1]; // 6bit
  self->col[2] = self->clist[cn][2]; // 5bit
  }
  tft.drawCircle(x, y, rr, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  return 0;
}

int RunHaco8Game::l_circfill(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  int rr = lua_tointeger(L, 3);
  int cn = lua_tointeger(L, 4);
  if(cn != NULL)
  {
  self->col[0] = self->clist[cn][0]; // 5bit
  self->col[1] = self->clist[cn][1]; // 6bit
  self->col[2] = self->clist[cn][2]; // 5bit
  }
  tft.fillCircle(x, y, rr, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  return 0;
}

int RunHaco8Game::l_fillcircle2(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  int r = lua_tointeger(L, 3);
  // tft.fillCircle(x, y, r, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  tft.fillCircle(x, y, r, TFT_YELLOW);
  return 0;
}

int RunHaco8Game::l_cls(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int r,g,b;
  int n = lua_tointeger(L, 1);
  if(n == NULL){
    n = 0;//黒の番号を入れて塗りつぶす
  }

  if(n == 0){ r = 0;     g = 0;    b = 0; }//0: 黒色
  else if(n == 1){ r = 27;    g = 42;   b = 86; }//1: 暗い青色
  else if(n == 2){ r = 137;   g = 24;   b = 84; }//2: 暗い紫色
  else if(n == 3){ r = 0;     g = 139;  b = 75; }//3: 暗い緑色
  else if(n == 4){ r = 183;   g = 76;   b = 45; }//4: 茶色
  else if(n == 5){ r = 97;    g = 87;   b = 78; }//5: 暗い灰色
  else if(n == 6){ r = 194;   g = 195;  b = 199; }//6: 明るい灰色
  else if(n == 7){ r = 255;   g = 241;  b = 231; }//7: 白色

  else if(n == 8){ r = 255;   g = 0;    b = 70; }//8: 赤色
  else if(n == 9){ r = 255;   g = 160;  b = 0; }//9: オレンジ
  else if(n == 10){ r = 255;  g = 238;  b = 0; }//10: 黄色
  else if(n == 11){ r = 0;    g = 234;  b = 0; }//11: 緑色
  else if(n == 12){ r = 0;    g = 173;  b = 255; }//12: 水色
  else if(n == 13){ r = 134;  g = 116;  b = 159; }//13: 藍色
  else if(n == 14){ r = 255;  g = 107;  b = 169; }//14: ピンク
  else if(n == 15){ r = 255;  g = 202;  b = 165; }//15: 桃色

  self->col[0] = r; // 5bit
  self->col[1] = g; // 6bit
  self->col[2] = b;       // 5bit

  tft.fillRect(0,0,128,128,lua_rgb24to16(self->col[0], self->col[1], self->col[2]));

  return 0;
}
