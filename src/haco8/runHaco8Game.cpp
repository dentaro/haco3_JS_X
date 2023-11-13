#include "runHaco8Game.h"
extern MyTFT_eSprite tft;
// extern LGFX_Sprite sprite64;
// extern LGFX_Sprite sprite256[BUF_PNG_NUM];
extern LGFX_Sprite sprite88_roi;
extern LGFX_Sprite buffSprite;

extern String appfileName;
extern void startWifiDebug(bool isSelf);
extern void setFileName(String s);
extern void runFileName(String s);
extern bool isWifiDebug();
extern void reboot();
extern int getcno2tftc(uint8_t _cno);
// extern Tunes tunes;
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
extern float soundSpeed;
// extern bool toneflag;
extern void readMap();
extern uint64_t frame;
extern int xtile;
extern int ytile;
extern float ztile;
extern int xtileNo;
extern int ytileNo;
extern bool downloadF;
extern LGFX_Sprite sprref;

// extern float sinValues[90];// 0から89度までの91個の要素

// extern String newKeys[BUF_PNG_NUM];
extern String oldKeys[BUF_PNG_NUM];

extern Vector3<float> currentgpos;

extern Vector3<float> prePos;
extern Vector3<float> currentPos;
extern Vector3<float> diffPos;
extern int dirNos[9];

// extern uint8_t sprite64cnos[PNG_SPRITE_WIDTH*PNG_SPRITE_HEIGHT];//64*128
extern std::vector<uint8_t> sprite64cnos_vector;
// #include "MapDictionary.h"//直前で読み込まないとexternできない
// extern MapDictionary dict;

// extern std::map<String, LGFX_Sprite*> key2ptr;

extern uint8_t mapArray[MAPWH][MAPWH];

extern int8_t sprbits[128];//fgetでアクセスするスプライト属性を格納するための配列

extern int getDirNo(float _x, float _y);
extern Vector2<int> getSign(int dirno);
extern Vector2<int> getKey2Sign(String _currentKey, String _targetKey);
extern LGFX_Sprite mapTileSprites[1];

// extern SemaphoreHandle_t xSemaphore;
// extern SemaphoreHandle_t taskSemaphore;
// extern void downloadTask(void *pvParameters);
// extern void taskCompleteCallback();
extern void setAllKeys(String _key);

extern std::deque<int> buttonState;//ボタンの個数未定

String oldKey = "";

Intersection intersections[32];

std::vector<std::vector<float>> polygonData;

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
  lua_pushcclosure(L, l_srnd, 1);
  lua_setglobal(L, "srnd");

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
  lua_pushcclosure(L, l_distance, 1);
  lua_setglobal(L, "distance");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_sin, 1);
  lua_setglobal(L, "sin");

  // lua_pushlightuserdata(L, this);
  // lua_pushcclosure(L, l_gsin, 1);
  // lua_setglobal(L, "gsin");

  // lua_pushlightuserdata(L, this);
  // lua_pushcclosure(L, l_gcos, 1);
  // lua_setglobal(L, "gcos");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_cos, 1);
  lua_setglobal(L, "cos");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_atan2, 1);
  lua_setglobal(L, "atan2");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_band, 1);
  lua_setglobal(L, "band");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_bnot, 1);
  lua_setglobal(L, "bnot");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_bor, 1);
  lua_setglobal(L, "bor");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_bxor, 1);
  lua_setglobal(L, "bxor");

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

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_sgn, 1);
  lua_setglobal(L, "sgn");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_shl, 1);
  lua_setglobal(L, "shl");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_shr, 1);
  lua_setglobal(L, "shr");

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

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_t, 1);
  lua_setglobal(L, "t");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_getstl, 1);
  lua_setglobal(L, "getstl");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_setply, 1);
  lua_setglobal(L, "setply");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_rendr, 1);
  lua_setglobal(L, "rendr");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_drawstars, 1);
  lua_setglobal(L, "drawstars");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_starp, 1);
  lua_setglobal(L, "starp");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_creobj, 1);
  lua_setglobal(L, "creobj");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_lig, 1);
  lua_setglobal(L, "lig");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_cam, 1);
  lua_setglobal(L, "cam");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_trans, 1);
  lua_setglobal(L, "trans");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_wini, 1);
  lua_setglobal(L, "wini");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_wset, 1);
  lua_setglobal(L, "wset");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_wdraw, 1);
  lua_setglobal(L, "wdraw");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_gcol, 1);
  lua_setglobal(L, "gcol");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_sp3d, 1);
  lua_setglobal(L, "sp3d");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_spmap, 1);
  lua_setglobal(L, "spmap");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_drawpng, 1);
  lua_setglobal(L, "drawpng");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_initstars, 1);
  lua_setglobal(L, "initstars");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_drawstars2, 1);
  lua_setglobal(L, "drawstars2");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_drawmaps, 1);
  lua_setglobal(L, "drawmaps");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_getmappos, 1);
  lua_setglobal(L, "getmappos");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_getgpos, 1);
  lua_setglobal(L, "getgpos");
  
}

int RunHaco8Game::l_mget(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int celx = lua_tointeger(L, 1);
  int cely = lua_tointeger(L, 2);
  lua_pushinteger(L, mapArray[celx][cely]);
  return 1;
}

// int getcno2tftc(uint8_t _cno){
//   switch (_cno)
//   {
//   case 0:return RunHaco8Game::HACO3_C0;break;
//   case 1:return RunHaco8Game::HACO3_C1;break;
//   case 2:return RunHaco8Game::HACO3_C2;break;
//   case 3:return RunHaco8Game::HACO3_C3;break;
//   case 4:return RunHaco8Game::HACO3_C4;break;
//   case 5:return RunHaco8Game::HACO3_C5;break;
//   case 6:return RunHaco8Game::HACO3_C6;break;
//   case 7:return RunHaco8Game::HACO3_C7;break;
//   case 8:return RunHaco8Game::HACO3_C8;break;
//   case 9:return RunHaco8Game::HACO3_C9;break;
//   case 10:return RunHaco8Game::HACO3_C10;break;
//   case 11:return RunHaco8Game::HACO3_C11;break;
//   case 12:return RunHaco8Game::HACO3_C12;break;
//   case 13:return RunHaco8Game::HACO3_C13;break;
//   case 14:return RunHaco8Game::HACO3_C14;break;
//   case 15:return RunHaco8Game::HACO3_C15;break;

//   default:
//   return RunHaco8Game::HACO3_C0;
//     break;
//   }
// }

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

  // sprite64.pushRotateZoom(&tft, x, y, 0, 1, 1, TFT_BLACK);
  // sprite64.pushSprite(&tft, x, y);

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

//キャラスプライト
  for(int y=0;y<8;y++){
      for(int x=0;x<8;x++){
        uint8_t bit4;
        int sprpos;
        //sprite64cnos[(sy*8) * PNG_SPRITE_WIDTH + (sx*8) 取得スタート位置
        //y*PNG_SPRITE_WIDTH + xスプライトのピクセル取得
        sprpos = (sy*8*PNG_SPRITE_WIDTH+sx*8 + y*PNG_SPRITE_WIDTH + x)/2;//４ビット二つで８ビットに入れてるので1/2に
        bit4 = sprite64cnos_vector[sprpos];
        if(x%2 == 1)bit4 = (bit4 & 0b00001111);
        if(x%2 == 0)bit4 = (bit4 >> 4);
        sprite88_roi.drawPixel(x,y, getcno2tftc(bit4));
      }
  }

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
            // sprite64.pushSprite(&sprite88_roi, -8*(sx), -8*(sy));//128*128のpngデータを指定位置までずらすsprite64のスプライトデータ8*8で切り抜く

            for(int y=0;y<8;y++){
                for(int x=0;x<8;x++){
                  uint8_t bit4;
                  int sprpos;
                  //sprite64cnos[(sy*8) * PNG_SPRITE_WIDTH + (sx*8) 取得スタート位置
                  //y*PNG_SPRITE_WIDTH + xスプライトのピクセル取得
                  sprpos = (sy*8*PNG_SPRITE_WIDTH+sx*8 + y*PNG_SPRITE_WIDTH + x)/2;
                  bit4 = sprite64cnos_vector[sprpos];
                  if(x%2 == 1)bit4 = (bit4 & 0b00001111);
                  if(x%2 == 0)bit4 = (bit4 >> 4);
                  sprite88_roi.drawPixel(x,y, getcno2tftc(bit4));
                }
            }
            
            // for(int y=0;y<8;y++){
            //   for(int x=0;x<8;x++){
            //     //sprite64cnos[(sy*8) * PNG_SPRITE_WIDTH + (sx*8) 取得スタート位置
            //     //y*PNG_SPRITE_WIDTH + xスプライトのピクセル取得
            //     uint8_t bit4;
            //     if(sx%2 == 0){
            //       bit4 = sprite64cnos[(sy*8 * PNG_SPRITE_WIDTH + sx*8 + y*PNG_SPRITE_WIDTH + x)/2];
            //       //上位４ビットを０マスクする
            //       bit4 = (bit4 & 0b00001111);
            //     }else if(sx%2 == 1){
            //       bit4 = sprite64cnos[(sy*8 * PNG_SPRITE_WIDTH + sx*8 + y*PNG_SPRITE_WIDTH + (x-1))/2];
            //       //上位４ビットを右にビットシフトして上位を０で埋める
            //       bit4 = (bit4 >> 4);
            //     }
            //     sprite88_roi.drawPixel(x,y, getcno2tftc(bit4));
            //   }
            // } 

            sprite88_roi.pushSprite(&tft, roix+n*8, roiy+m*8);//4ずれない
            // sprite88_roi.pushRotateZoom(&tft, roix+n*8+4, roiy+m*8+4, 0, 1, 1, TFT_BLACK);//なぜか４を足さないとずれる要修正

      }
  }
  return 0;
}

int RunHaco8Game::l_drawpng(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  String pngname = lua_tostring(L, 1);
  int x = lua_tointeger(L, 2);
  int y = lua_tointeger(L, 3);

  return 0; 
}

extern std::vector<String> temporaryKeys;
extern std::vector<String> previousKeys;
extern std::vector<String> writableKeys;

extern std::vector<String> downloadKeys;
extern std::vector<String> predownloadKeys;
extern std::vector<String> allKeys;
extern std::vector<String> preallKeys;

std::vector<String> rundownloadKeys;

//preAllKeysをチェックしてなければ追加する。
void setDownloadKeys(String _key) {
        // _key は preallKeys に存在しない場合のみ追加
        downloadKeys.push_back(_key);
}

bool existDownloadKey(String _key) {
    return std::find(downloadKeys.begin(), downloadKeys.end(), _key) != downloadKeys.end();
}

bool existpreDownloadKey(String _key) {
    return std::find(predownloadKeys.begin(), predownloadKeys.end(), _key) != predownloadKeys.end();
}

void printpreallKeys() {
    Serial.println("preall Keys:");
    for (const auto& key : preallKeys) {
        Serial.print(key);
    }
    Serial.println("");
}

extern int shouldNo;
extern int downloadLimitNum;
bool alreadyDownlordedF= false;
String currentKey = "";
extern String targetKey;
extern float tileZoom;
extern int currentDirNo;
extern float bairitu;
bool collF=true;
int mx = 62;//64;//128;//ダウンロードを先読みする距離
int my = 58;//60;//120;//ダウンロードを先読みする距離
int ringBuffNo = 0;

Vector2<int> buffPos;
Vector2<float> buffPos2;

// extern void taskCompleteCallback();
Vector2<int> calculateVectorDifference(const String& Key1, const String& Key2) {
  if(Key2 !=NULL)
  {
    std::istringstream stream1(Key1.c_str());
    std::istringstream stream2(Key2.c_str());
    std::string token;

    // Key1 から x1 と y1 を抽出
    std::getline(stream1, token, '/');
    int x1 = std::stoi(token);
    std::getline(stream1, token);
    int y1 = std::stoi(token);

    // Key2 から x2 と y2 を抽出
    std::getline(stream2, token, '/');
    int x2 = std::stoi(token);
    std::getline(stream2, token);
    int y2 = std::stoi(token);

    // 差分を計算して Vector2<int> として返す
    return Vector2<int>(x2 - x1, y2 - y1);
    //  return Vector2<int>(x1 - x2, y1 - y2);
  }else{
    return Vector2<int>(0,0);
  }
}

// 文字列をデリミタで分割して Vector2<int> に変換する関数
Vector2<int> parseStringToVector2(const String& Key1) {
    std::istringstream stream(Key1.c_str());
    std::string token;
    std::getline(stream, token, '/');
    int x = std::stoi(token);
    std::getline(stream, token);
    int y = std::stoi(token);
    return Vector2<int>(x, y);
}

void getTilePos(float _lat, float _lon, int zoom_level)
{
  // 経度を0から2にスケーリングし、255倍してタイル座標に変換
  float n = pow(2, zoom_level);
  float lat_rad = _lat * (M_PI / 180);

  float x = (_lon + 180.0) / 360.0 * n;
  xtile = int(x * 256);

  // 緯度をスケーリングしてタイル座標に変換
  float y = (1.0 - log(tan(lat_rad) + (1 / cos(lat_rad))) / M_PI) / 2.0 * n;
  ytile = int(y * 256);

  ztile = zoom_level;

  // 現在地を含むタイル番号を計算
  xtileNo = int(x);
  ytileNo = int(y);
}

int RunHaco8Game::l_getgpos(lua_State* L) {
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int _posX = lua_tointeger(L, 1);
  int _posY = lua_tointeger(L, 2);
  float _tileZoom = lua_tonumber(L, 3);

  float bairitu = 1 - (0.5 - ((_tileZoom - int(floor(_tileZoom))) / 2));
  float zoom_level = int(floor(_tileZoom));

  // 調整後の座標を逆変換して緯度経度に戻す
  float n = pow(2, zoom_level);
  float lat_rad = asin(tanh(M_PI * (2 * ((static_cast<float>(_posY - TFT_HEIGHT_HALF)/256.0 ) / bairitu) / n)));
  float lon = (static_cast<float>(_posX - TFT_WIDTH_HALF)/256.0 ) / bairitu / n * 360.0;
  float lat = lat_rad * 180.0 / M_PI;

  // テーブルを作成して値をセット
  lua_newtable(L);

  lua_pushnumber(L, currentgpos.getX()-lat);
  lua_setfield(L, -2, "lat");

  lua_pushnumber(L, currentgpos.getY()+lon);
  lua_setfield(L, -2, "lon");

  lua_pushnumber(L, _tileZoom);
  lua_setfield(L, -2, "alt");

  // テーブルをスタックにプッシュ
  return 1;
}

int RunHaco8Game::l_getmappos(lua_State* L)
{
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  float _walkLatPos = lua_tonumber(L, 1);
  float _walkLonPos = lua_tonumber(L, 2);
  float _tileZoom = lua_tonumber(L, 3);

  float bairitu = 1 - (0.5 - ((_tileZoom - int(floor(_tileZoom))) / 2));
  float zoom_level = int(floor(_tileZoom));

  // 経度を0から2にスケーリングし、タイル座標に変換
  // float lon_rad = _walkLonPos * (M_PI / 180);
  float n = pow(2, zoom_level);
  float _x = (_walkLonPos + 180.0) / 360.0 * n;
  int _xtile = int(_x * 256);

  // 緯度をスケーリングしてタイル座標に変換
  float lat_rad = _walkLatPos * (M_PI / 180);
  float _y = (1.0 - log(tan(lat_rad) + 1 / cos(lat_rad)) / M_PI) / 2.0 * n;
  int _ytile = int(_y * 256);

  int _ztile = int(zoom_level);

  // 現在地を含むタイル番号を計算
  int _xtileNo = int(_x);
  int _ytileNo = int(_y);

  // 調整
  _xtile = int(_xtile + 256) % 256 - currentPos.x;
  _ytile = int(_ytile + 256) % 256 - currentPos.y;

  // 調整後の _xtile や _ytile を使用して調整
  _xtile = TFT_WIDTH_HALF + int(_xtile * bairitu);
  _ytile = TFT_HEIGHT_HALF + int(_ytile * bairitu);

  // テーブルを作成して値をセット
  lua_newtable(L);

  lua_pushinteger(L, int(_xtile + (_xtileNo - xtileNo) * 256));
  lua_setfield(L, -2, "x");

  lua_pushinteger(L, int(_ytile + (_ytileNo - ytileNo) * 256));
  lua_setfield(L, -2, "y");

  lua_pushinteger(L, _ztile);
  lua_setfield(L, -2, "z");

  // テーブルをスタックにプッシュ
  return 1;
}

// 時分秒を度に変換する関数
float hmsToDegrees(int hours, int minutes, float seconds) {
    return (hours * 15.0 + minutes + seconds /3600); // 1時間 = 15度, 1分 = 0.25度, 1秒 = 0.00416667度
}

// 度分秒を度に変換する関数
float dmsToDegrees(int degrees, int minutes, float seconds) {
    return degrees + minutes / 60.0 + seconds / 3600.0; // 1度 = 60分 = 3600秒
}

// 時分秒を度に変換する関数
// float hmsToRadian(int hours, int minutes, float seconds) {
//     return (hours * 15.0 + minutes + seconds * 0.00416667) * (M_PI / 180.0); // 1時間 = 15度, 1分 = 0.25度, 1秒 = 0.00416667度
// }
float hmsToRadian(int hours, int minutes, float seconds) {
    return (hours * 15.0 + minutes + seconds /3600) * RAD_ONE; // 1時間 = 15度, 1分 = 0.25度, 1秒 = 0.00416667度
}

// 度分秒を度に変換する関数
float dmsToRadian(int degrees, int minutes, float seconds) {
    return (degrees + minutes / 60.0 + seconds / 3600.0) * RAD_ONE;// 1度 = 60分 = 3600秒
}

bool firstFileOpenF = true;
File fr;
int BSTARBUFNUM = STARS_NUM;
float bsParamFloat[STARS_NUM][2];
uint8_t bsParamInt8t[STARS_NUM][1];
uint16_t bsParamInt16t[STARS_NUM][1];

int RunHaco8Game::l_initstars(lua_State* L) 
{
  float _asc_h = lua_tonumber(L, 1);
  float _asc_m = lua_tonumber(L, 2);
  float _asc_s = lua_tonumber(L, 3);
  float _dec_d  = lua_tonumber(L, 4);
  float _dec_m  = lua_tonumber(L, 5);
  float _dec_s  = lua_tonumber(L, 6);
  int zoom = lua_tointeger(L, 7);

  // 赤経の中心と幅を指定
  float centerRightAscension = hmsToRadian( _asc_h, _asc_m, _asc_s ); // 中心の赤経（時分秒をラジアンに変換する関数）
  float centerDeclination    = dmsToRadian( _dec_d, _dec_m, _dec_s );// 中心の赤緯（度分秒をラジアンに変換する関数）
  
  float halfWidthRightAscension = (M_PI / zoom); // 赤経の半幅（ラジアン）
  float halfHeightDeclination = (M_PI / zoom);// 赤緯の半幅（ラジアン）

  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));

  // ...（初期化などのコード）

  for(int i = 0; i < ui.getAllBtnNum(); i ++){//ボタンの強制リセット（しないと読み込み時にもイベントを取るのか2回読み込みが行われてしまう）
    buttonState[i] = 0;
  }

  if (SPIFFS.exists("/star/brightstar1000.csv")) {
    File fr = SPIFFS.open("/star/brightstar1000.csv", FILE_READ);
    String line;

    int currentIndex = 0;
    // 赤経と赤緯のラジアン変換
    float starRightAscension = 0;
    float starDeclination = 0;
    int starbrightness = 0;
    int colangle = -1;//無彩色を指定
    
    String noStr = "";
    String rightAscensionStr = "";
    String declinationStr = "";
    String brightnessStr = "";
    String spectraltypeStr = "";
    // String starData[6]; // 6つの要素を持つ配列Y

    int startLine = 1;//この行から読み込み描画を開始する
    int currentLine = 1;

    float startAscRadian = centerRightAscension + (M_PI/zoom);

    //赤経のラジアン角から読み込み描画のスタート行を決定する
         if(startAscRadian >=6.771955068 && startAscRadian <= 7.051396826){startLine = 9000;}
    else if(startAscRadian >=6.422898914 && startAscRadian < 6.771955068 ){startLine = 8750;}
    else if(startAscRadian >=6.195972174 && startAscRadian < 6.422898914 ){startLine = 8500;}
    else if(startAscRadian >=6.021395616 && startAscRadian < 6.195972174 ){startLine = 8250;}
    else if(startAscRadian >=5.881778972 && startAscRadian < 6.021395616 ){startLine = 8000;}
    else if(startAscRadian >=5.707464213 && startAscRadian < 5.881778972 ){startLine = 7750;}
    else if(startAscRadian >=5.567624555 && startAscRadian < 5.707464213 ){startLine = 7500;}
    else if(startAscRadian >=5.410525529 && startAscRadian < 5.567624555 ){startLine = 7250;}
    else if(startAscRadian >=5.253470137 && startAscRadian < 5.410525529 ){startLine = 7000;}
    else if(startAscRadian >=5.079169923 && startAscRadian < 5.253470137 ){startLine = 6750;}
    else if(startAscRadian >=4.921930301 && startAscRadian < 5.079169923 ){startLine = 6500;}
    else if(startAscRadian >=4.764840973 && startAscRadian < 4.921930301 ){startLine = 6250;}
    else if(startAscRadian >=4.590283807 && startAscRadian < 4.764840973 ){startLine = 6000;}
    else if(startAscRadian >=4.4157024   && startAscRadian < 4.590283807 ){startLine = 5750;}
    else if(startAscRadian >=4.223832538 && startAscRadian < 4.4157024   ){startLine = 5500;}
    else if(startAscRadian >=4.031904498 && startAscRadian < 4.223832538 ){startLine = 5250;}
    else if(startAscRadian >=3.839792228 && startAscRadian < 4.031904498 ){startLine = 5000;}
    else if(startAscRadian >=3.630415744 && startAscRadian < 3.839792228 ){startLine = 4750;}
    else if(startAscRadian >=3.438395589 && startAscRadian < 3.630415744 ){startLine = 4500;}
    else if(startAscRadian >=3.22900456  && startAscRadian < 3.438395589 ){startLine = 4250;}
    else if(startAscRadian >=3.01961838  && startAscRadian < 3.22900456  ){startLine = 4000;}
    else if(startAscRadian >=2.862533899 && startAscRadian < 3.01961838  ){startLine = 3750;}
    else if(startAscRadian >=2.705400937 && startAscRadian < 2.862533899 ){startLine = 3500;}
    else if(startAscRadian >=2.565638849 && startAscRadian < 2.705400937 ){startLine = 3250;}
    else if(startAscRadian >=2.426206434 && startAscRadian < 2.565638849 ){startLine = 3000;}
    else if(startAscRadian >=2.286449194 && startAscRadian < 2.426206434 ){startLine = 2750;}
    else if(startAscRadian >=2.146842246 && startAscRadian < 2.286449194 ){startLine = 2500;}
    else if(startAscRadian >=2.007303173 && startAscRadian < 2.146842246 ){startLine = 2250;}
    else if(startAscRadian >=1.885023466 && startAscRadian < 2.007303173 ){startLine = 2000;}
    else if(startAscRadian >=1.728065037 && startAscRadian < 1.885023466 ){startLine = 1750;}
    else if(startAscRadian >=1.571082367 && startAscRadian < 1.728065037 ){startLine = 1500;}
    else if(startAscRadian >=1.413774872 && startAscRadian < 1.571082367 ){startLine = 1250;}
    else if(startAscRadian >=1.239435872 && startAscRadian < 1.413774872 ){startLine = 1000;}
    else if(startAscRadian >=1.029865462 && startAscRadian < 1.239435872 ){startLine = 750;}
    else if(startAscRadian >=0.820416256 && startAscRadian < 1.029865462 ){startLine = 500;}
    else if(startAscRadian >=0.541212057 && startAscRadian < 0.820416256 ){startLine = 250;}
    else if(startAscRadian >=0 && startAscRadian < 0.541212057 ){startLine = 1;}

    // while (fr.available()) {
    // while (starData[1].toFloat() < centerRightAscension + (M_PI/zoom)) {//描画すべき赤経を超えるまで読み込む
    for(int i=0;i<BSTARBUFNUM;i++){
      line = fr.readStringUntil('\n');
        if (currentLine >= startLine) {
          if (!line.isEmpty()) {

            int currentIndex = 0;

            while (line.length() > 0) {
              int commaIndex = line.indexOf(',');
              if (commaIndex != -1) {
                String val = line.substring(0, commaIndex);
                // starData[currentIndex] = val;

                if(currentIndex==0)bsParamFloat[i][0] = val.toFloat();
                else if(currentIndex==1)bsParamFloat[i][1] = val.toFloat();
                else if(currentIndex==2)bsParamInt8t[i][0] = val.toInt();
                else if(currentIndex==3)bsParamInt16t[i][0] = val.toInt();

                currentIndex++;
                line = line.substring(commaIndex + 1);
              } else {
                // starData[currentIndex] = line;
                line = "";
              }
            }
            currentIndex++;
          }
        }
        currentLine++;
      }
    fr.close();
  }
  return 0;
}


// スケーリング関数
float scaleValue(float value, float inMin, float inMax, float outMin, float outMax) {
    return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

// // 天球儀上での座標を計算する関数
// void celestialToScreen(float celestialRA, float celestialDec,
//                        float centerRA, float centerDec,
//                        float halfWidthRA, float halfHeightDec,
//                        float screenWidth, float screenHeight,
//                        float &x, float &y) {
//     // 赤経をスケーリング
//     x = scaleValue(celestialRA,
//                    centerRA - halfWidthRA,
//                    centerRA + halfWidthRA,
//                    0, screenWidth);

//     // 赤緯をスケーリング
//     y = scaleValue(celestialDec,
//                    centerDec - halfHeightDec,
//                    centerDec + halfHeightDec,
//                    screenHeight, 0);
// }

void celestialToScreen(float starRightAscension, float starDeclination,
                       float centerRightAscension, float halfWidthRightAscension,
                       float centerDeclination, float halfHeightDeclination,
                       float pixelWidth, float pixelHeight,
                       float &x, float &y) {
    // 中心座標
    float centerX = pixelWidth / 2.0;
    float centerY = pixelHeight / 2.0;

    // 星の赤経と赤緯をラジアンから度に変換
    float ascDegree = (starRightAscension-centerRightAscension) * (180.0 / M_PI);
    float decDegree = (starDeclination-centerDeclination) * (180.0 / M_PI);

    // 赤経をx座標に変換（半径方向）
    float radius = 0.5 * pixelWidth;
    x = centerX + radius * cos(decDegree) * sin(ascDegree);

    // 赤緯をy座標に変換（垂直方向）
    float yRadius = 0.5 * pixelHeight;
    y = centerY - yRadius * sin(decDegree);

    // 座標がピクセル範囲内に収まるようにクリップ
    x = fmax(0, fmin(pixelWidth - 1, x));
    y = fmax(0, fmin(pixelHeight - 1, y));
}

int RunHaco8Game::l_drawstars2(lua_State* L) 
{
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  float _asc_h = lua_tonumber(L, 1);
  float _asc_m = lua_tonumber(L, 2);
  float _asc_s = lua_tonumber(L, 3);
  float _dec_d  = lua_tonumber(L, 4);
  float _dec_m  = lua_tonumber(L, 5);
  float _dec_s  = lua_tonumber(L, 6);
  int zoom = lua_tointeger(L, 7);

  // 赤経の中心と幅を指定
  float centerRightAscension = hmsToRadian( _asc_h, _asc_m, _asc_s ); // 中心の赤経（時分秒をラジアンに変換する関数）
  float centerDeclination    = dmsToRadian( _dec_d, _dec_m, _dec_s );// 中心の赤緯（度分秒をラジアンに変換する関数）
  
  float halfWidthRightAscension = (M_PI / zoom); // 赤経の半幅（ラジアン）
  float halfHeightDeclination = (M_PI / zoom);// 赤緯の半幅（ラジアン）

  // for(int i = 0; i < ui.getAllBtnNum(); i ++){//ボタンの強制リセット（しないと読み込み時にもイベントを取るのか2回読み込みが行われてしまう）
  //   buttonState[i] = 0;
  // }

    // 赤経と赤緯のラジアン変換
    float starRightAscension = 0;
    float starDeclination = 0;
    int starbrightness = 0;
    // int colangle = -1;//無彩色を指定
    int noInt = -1;
    // int spectraltype = -1;
    // int startLine = 1;//この行から読み込み描画を開始する
    // float startAscRadian = centerRightAscension - (M_PI/zoom);
    

    //赤経のラジアン角から読み込み描画のスタート行を決定する
    //      if(startAscRadian >=6.771955068 && startAscRadian <= 7.051396826){startLine = 9000;}
    // else if(startAscRadian >=6.422898914 && startAscRadian < 6.771955068 ){startLine = 8750;}
    // else if(startAscRadian >=6.195972174 && startAscRadian < 6.422898914 ){startLine = 8500;}
    // else if(startAscRadian >=6.021395616 && startAscRadian < 6.195972174 ){startLine = 8250;}
    // else if(startAscRadian >=5.881778972 && startAscRadian < 6.021395616 ){startLine = 8000;}
    // else if(startAscRadian >=5.707464213 && startAscRadian < 5.881778972 ){startLine = 7750;}
    // else if(startAscRadian >=5.567624555 && startAscRadian < 5.707464213 ){startLine = 7500;}
    // else if(startAscRadian >=5.410525529 && startAscRadian < 5.567624555 ){startLine = 7250;}
    // else if(startAscRadian >=5.253470137 && startAscRadian < 5.410525529 ){startLine = 7000;}
    // else if(startAscRadian >=5.079169923 && startAscRadian < 5.253470137 ){startLine = 6750;}
    // else if(startAscRadian >=4.921930301 && startAscRadian < 5.079169923 ){startLine = 6500;}
    // else if(startAscRadian >=4.764840973 && startAscRadian < 4.921930301 ){startLine = 6250;}
    // else if(startAscRadian >=4.590283807 && startAscRadian < 4.764840973 ){startLine = 6000;}
    // else if(startAscRadian >=4.4157024   && startAscRadian < 4.590283807 ){startLine = 5750;}
    // else if(startAscRadian >=4.223832538 && startAscRadian < 4.4157024   ){startLine = 5500;}
    // else if(startAscRadian >=4.031904498 && startAscRadian < 4.223832538 ){startLine = 5250;}
    // else if(startAscRadian >=3.839792228 && startAscRadian < 4.031904498 ){startLine = 5000;}
    // else if(startAscRadian >=3.630415744 && startAscRadian < 3.839792228 ){startLine = 4750;}
    // else if(startAscRadian >=3.438395589 && startAscRadian < 3.630415744 ){startLine = 4500;}
    // else if(startAscRadian >=3.22900456  && startAscRadian < 3.438395589 ){startLine = 4250;}
    // else if(startAscRadian >=3.01961838  && startAscRadian < 3.22900456  ){startLine = 4000;}
    // else if(startAscRadian >=2.862533899 && startAscRadian < 3.01961838  ){startLine = 3750;}
    // else if(startAscRadian >=2.705400937 && startAscRadian < 2.862533899 ){startLine = 3500;}
    // else if(startAscRadian >=2.565638849 && startAscRadian < 2.705400937 ){startLine = 3250;}
    // else if(startAscRadian >=2.426206434 && startAscRadian < 2.565638849 ){startLine = 3000;}
    // else if(startAscRadian >=2.286449194 && startAscRadian < 2.426206434 ){startLine = 2750;}
    // else if(startAscRadian >=2.146842246 && startAscRadian < 2.286449194 ){startLine = 2500;}
    // else if(startAscRadian >=2.007303173 && startAscRadian < 2.146842246 ){startLine = 2250;}
    // else if(startAscRadian >=1.885023466 && startAscRadian < 2.007303173 ){startLine = 2000;}
    // else if(startAscRadian >=1.728065037 && startAscRadian < 1.885023466 ){startLine = 1750;}
    // else if(startAscRadian >=1.571082367 && startAscRadian < 1.728065037 ){startLine = 1500;}
    // else if(startAscRadian >=1.413774872 && startAscRadian < 1.571082367 ){startLine = 1250;}
    // else if(startAscRadian >=1.239435872 && startAscRadian < 1.413774872 ){startLine = 1000;}
    // else if(startAscRadian >=1.029865462 && startAscRadian < 1.239435872 ){startLine = 750;}
    // else if(startAscRadian >=0.820416256 && startAscRadian < 1.029865462 ){startLine = 500;}
    // else if(startAscRadian >=0.541212057 && startAscRadian < 0.820416256 ){startLine = 250;}
    // else if(startAscRadian >=0 && startAscRadian < 0.541212057 ){startLine = 1;}

// int i = startLine;

// int i = 0;
//  while ( i < BSTARBUFNUM ) {
  for(int i =0; i< BSTARBUFNUM ;i++){
    // while (bsParamFloat[i][0]< centerRightAscension + (M_PI/zoom)) {//描画すべき赤経を超えるまで読み込む
            //読み込み描画処理をする赤経の範囲かどうかを判定
            // if(bsParamFloat[i][0]> centerRightAscension - (M_PI/zoom) && bsParamFloat[i][0] < centerRightAscension + (M_PI/zoom))
            // {
            //   //読み込み描画処理をする赤緯の範囲かどうかを判定
            //   if(bsParamFloat[i][1] >centerDeclination - (M_PI/zoom) && bsParamFloat[i][1] < centerDeclination + (M_PI/zoom))
            //   {
                

              // 赤経と赤緯のラジアン変換
              // noInt = bsParamInt8t[i][0];
              starRightAscension = bsParamFloat[i][0];
              starDeclination = bsParamFloat[i][1];
              starbrightness = bsParamInt8t[i][0];
              noInt = bsParamInt16t[i][0];
              // spectraltype = bsParamInt8t[i][1];

              // colangle = -1;//無彩色を指定
              //       if(spectraltype == 0){colangle = 240;}//O型 ≥ 30,000 K 青
              // else  if(spectraltype == 1){colangle = 180;}//B型 10,000–30,000 K
              // else  if(spectraltype == 2){colangle = -1;}//A型 7,500–10,000 K
              // else  if(spectraltype == 3){colangle = 70;}//F型 6,000–7,500 K
              // else  if(spectraltype == 4){colangle = 60;}//G型 5,200–6,000 K
              // else  if(spectraltype == 5){colangle = 40;}//K型 3,700–5,200 K
              // else  if(spectraltype == 6){colangle = 35;}//C型
              // else  if(spectraltype == 7){colangle = 30;}//M-S型 2,400–3,700 K

              // 赤経の範囲内の星を描画
              // if (starRightAscension >= (centerRightAscension - halfWidthRightAscension) &&
              //   starRightAscension <= (centerRightAscension + halfWidthRightAscension)) {

                  // 描画する座標の計算
                  // 赤経をスケーリング

                  // x = scaleValue(starRightAscension, 
                  //                     centerRightAscension - halfWidthRightAscension,
                  //                     centerRightAscension + halfWidthRightAscension, TFT_WIDTH, 0);

                  // // 赤緯をスケーリング
                  // y = scaleValue(starDeclination, 
                  //                     centerDeclination - halfHeightDeclination, 
                  //                     centerDeclination + halfHeightDeclination, TFT_HEIGHT, 0);

                  // float cy = starDeclination;
                  // float k = 1 -starRightAscension * cy;

                  // //天球座標に変換
                  // float x = cy * -starRightAscension / k;
                  // float y = starRightAscension / k;


                  

                  // celestialToScreen(starRightAscension, starDeclination,
                  //     centerRightAscension, centerDeclination,
                  //     halfWidthRightAscension, halfHeightDeclination,
                  //     TFT_WIDTH, TFT_HEIGHT,
                  //     x, y);

                  

                  // if(fmod((starRightAscension - centerRightAscension + TWO_PI),TWO_PI) < PI)
                  if (starRightAscension >= centerRightAscension - halfWidthRightAscension &&
                      starRightAscension <= centerRightAscension + halfWidthRightAscension &&
                      starRightAscension - centerRightAscension > -M_PI) {

                  float x, y;

                  // 天球座標をピクセル座標に変換
                  // celestialToScreen(starRightAscension, starDeclination, 
                  // centerRightAscension,halfWidthRightAscension,
                  // centerDeclination,halfHeightDeclination,
                  // TFT_WIDTH, TFT_HEIGHT,
                  // x, y);

                   celestialToScreen(starRightAscension, starDeclination, 
                      centerRightAscension, halfWidthRightAscension,
                      centerDeclination, halfHeightDeclination,
                      TFT_WIDTH, TFT_HEIGHT,
                      x, y);
                  

                  
                  // float x = scaleValue(starRightAscension, 
                  //                     0 - halfWidthRightAscension,
                  //                     0 + halfWidthRightAscension, TFT_WIDTH, 0);

                  // // 赤緯をスケーリング
                  // float y = scaleValue(starDeclination, 
                  //                     0 - halfHeightDeclination, 
                  //                     0 + halfHeightDeclination, TFT_HEIGHT, 0);

                  // tft.drawPixel(x, y, TFT_WHITE);

                  // tft.drawPixel(64, 60, TFT_RED);
                  
                  // tft.fillCircle(int(x), int(y), 1, TFT_WHITE); 

                  // if(starbrightness<=70){
                  //   tft.drawPixel(x, y, lua_rgb24to16(starbrightness, starbrightness, starbrightness)); 
                  // }
                  // else 

                  
                  if(starbrightness>70&&starbrightness<=110){//110=４等級
                    tft.fillCircle(x, y, 1, lua_rgb24to16(starbrightness, starbrightness, starbrightness)); 
                  }
                  else if(starbrightness>110&&starbrightness<184){
                    tft.fillCircle(x, y, 1, lua_rgb24to16(starbrightness, starbrightness, starbrightness));
                  }else if(starbrightness>=184){
                    tft.fillCircle(x, y, 2, lua_rgb24to16(starbrightness, starbrightness, starbrightness)); 
                    tft.setTextWrap(false);
                    tft.setCursor(x-4,y+4); 
                    tft.print(noInt);
                  }

                  tft.setTextWrap(false);
                  tft.setCursor(x-4,y+4);

                       if(noInt==2061){tft.print("ベテルギウス");}
                  else if(noInt==1713){tft.print("リゲル");}
                  else if(noInt==5191){tft.print("アルカイド");}
                  else if(noInt==4763){tft.print("ガクルックス");}
                  else if(noInt==4357){tft.print("ゾズマ");}
                  else if(noInt==264){tft.print("カシオペヤ");}
                  else if(noInt==2491){tft.print("シリウス");}
                  else if(noInt==2827){tft.print("アルドラ");}
                  else if(noInt==3165){tft.print("ナオス");}
                  else if(noInt==2990){tft.print("ポルックス");}
                  else if(noInt==5054){tft.print("ミザール");}
                  else if(noInt==5793){tft.print("アルフェッカ");}
                  else if(noInt==6527){tft.print("シャウラ");}
                  else if(noInt==7796){tft.print("サドル");}
                  else if(noInt==7924){tft.print("デネブ");}
                  else if(noInt==8162){tft.print("アルデラミン");}
                  else if(noInt==8775){tft.print("シェアト");}
                  else if(noInt==8308){tft.print("エニフ");}
                  else if(noInt==8728){tft.print("フォーマルハウト");}
                  else if(noInt==4301){tft.print("ドゥベー");}
                  else if(noInt==4905){tft.print("アリオト");}
                  else if(noInt==3982){tft.print("レグルス");}
                  else if(noInt==4534){tft.print("デネボラ");}
                  else if(noInt==4554){tft.print("フェクダ");}
                  else if(noInt==7001){tft.print("ベガ");}
                  else if(noInt==6705){tft.print("エルタニン");}
                  else if(noInt==3748){tft.print("アルファルド");}
                  else if(noInt==5056){tft.print("スピカ");}
                  else if(noInt==3634){tft.print("スハイル");}
                  else if(noInt==4853){tft.print("ミモザ");}
                  else if(noInt==1903){tft.print("アルニラム");}
                  else if(noInt==603){tft.print("アルマク");}
                  else if(noInt==168){tft.print("シェダル");}
                  else if(noInt==617){tft.print("ハマル");}
                  else if(noInt==936){tft.print("アルゴル");}
                  else if(noInt==1017){tft.print("ミルファク");}
                  else if(noInt==1457){tft.print("アルデバラン");}
                  else if(noInt==424){tft.print("ポラリス");}
                  else if(noInt==2326){tft.print("カノープス");}
                  else if(noInt==3685){tft.print("ミアプラキドゥス");}
                  else if(noInt==2943){tft.print("プロキオン");}
                  else if(noInt==99){tft.print("アンカー");}
                  else if(noInt==472){tft.print("アケルナル");}
                  else if(noInt==2249){tft.print("ミルザム");}
                  else if(noInt==1948){tft.print("アルニタク");}
                  else if(noInt==1790){tft.print("ベラトリクス");}//Bellatrix
                  else if(noInt==1791){tft.print("エルナト");}//Elnath
                  else if(noInt==2088){tft.print("メンカリナン");}//Menkalinan
                  else if(noInt==5953){tft.print("ジュバ");}//Dschubba
                  else if(noInt==6378){tft.print("サビク");}//Sabik
                  else if(noInt==6217){tft.print("アトリア");}//Atria
                  else if(noInt==6134){tft.print("アンタレス");}
                  else if(noInt==5459){tft.print("Rigil Kentaurus");}
                  else if(noInt==7790){tft.print("ピーコック");}

                  // else if(noInt==3485){tft.print("　");}
                  // else if(noInt==3734){tft.print("　");}
                  // else if(noInt==3207){tft.print("　");}//172
                  // else if(noInt==6580){tft.print("　");}
                  // else if(noInt==5460){tft.print("　");}//184
                  // else if(noInt==5440){tft.print("　");}
                  // else if(noInt==3301){tft.print("");}
                // }
        //       }
        }
        // }
    // i++;
    }
    // tft.drawPixel(64, 60, HACO3_C8); 
  return 0;
}

int RunHaco8Game::l_drawmaps(lua_State* L){
  // RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  // float _walkLatPos = lua_tonumber(L, 1);
  // float _walkLonPos = lua_tonumber(L, 2);
  // float _tileZoom = lua_tonumber(L, 3);

  // tileZoom = _tileZoom;
  // currentgpos = {_walkLatPos, _walkLonPos, tileZoom};

  // //tileZoom の少数点以降からbairituを計算する
  // float bairitu = 1-(0.5-((tileZoom - int(floor(tileZoom)))/2));

  // //経緯度からタイル座標を計算
  // getTilePos(_walkLatPos, _walkLonPos, tileZoom);

  // //現在のタイルマップのキーを保持
  // currentKey = String(int(xtileNo)) + "/" + String(int(ytileNo));

  // //現在位置の座標を計算して確定する（タイルマップの現在値をディスプレイの中央にずらすための座標）
  // currentPos.set(xtile % 256, ytile % 256, 0.0);

  // //進行方向をベクトルで取得
  // diffPos = currentPos.diff(prePos);//変位

  // //現在の進行方向を方向ベクトルから方向番号に変換して保持
  // int currentDirNo = dict.getTargetDirNo(diffPos);//進行方向番号を取得

  // // dict.setBuffIDNos(currentDirNo);//8方向すべてのBuffIDNoをセットする(以降変わらない)
  // dict.setDirNos(currentDirNo);//8方向すべてのdirNoをセットする
  // Vector2<int> currentTilePos = dict.getTilePos(currentDirNo);
  
  // //リングバッファに対する処理、実際にダウンロードする枚数を抽出する
  // for (int buffCounter = 0; buffCounter < downloadLimitNum; buffCounter++) 
  // {
  //   //方向番号を取得
  //   int targetDirNo = dict.getDirNo(buffCounter);
  //   Vector2<int> targetTilePos = dict.getTilePos(targetDirNo);
  //   targetKey = String(int(xtileNo + targetTilePos.getX())) + "/" + String(int(ytileNo + targetTilePos.getY()));
  //   //------------------------------------

  //   allKeys.push_back(targetKey);
  //   //主人公の位置が真ん中に来るようにバッファスプライトの位置を動かす
  //   buffPos.set(TFT_WIDTH_HALF  + ((targetTilePos.getX()*256)-currentPos.x) * bairitu,
  //               TFT_HEIGHT_HALF + ((targetTilePos.getY()*256)-currentPos.y) * bairitu);
    
  //   //バッファと関心領域との当たり判定を計算
  //   Rect2D cR(TFT_WIDTH_HALF -((TFT_WIDTH_HALF+mx)-(currentTilePos.getX()*mx))*bairitu,
  //           TFT_HEIGHT_HALF-((TFT_HEIGHT_HALF+my)-(currentTilePos.getY()*my))*bairitu,
  //           (TFT_WIDTH+mx*2)*bairitu,
  //           (TFT_HEIGHT+my*2)*bairitu);
  //   // Rect2D cR(TFT_WIDTH_HALF-((64+mx/2)-targetTilePos.getX()*mx)*bairitu,TFT_HEIGHT_HALF-((60+my/2)-targetTilePos.getY()*my)*bairitu,(128+mx)*bairitu,(120+my)*bairitu);//ダウンロードのタイミングを作るための当たり判定用の矩形
  //   Rect2D tR(buffPos.getX(),buffPos.getY(),256*bairitu,256*bairitu);

  //   collF=false;//初期化

  //   if (cR.x < tR.x + tR.w &&
  //       cR.x + cR.w > tR.x &&
  //       cR.y < tR.y + tR.h &&
  //       cR.y + cR.h > tR.y) 
  //       {
  //         collF = true;// 当たり判定あり
  //       } 

  //   if(collF){//当たったらダウンロード対象にする//preAllKeysをチェックしてなければ追加する。
  //     setDownloadKeys(targetKey);
  //     // dict.setCollF(buffCounter, true);
  //     dict.setKey2CollF(targetKey, true);
  //     if (!existpreDownloadKey(targetKey)){//実際にダウンロードするキーのみを抽出
  //       rundownloadKeys.push_back(targetKey);//実際にダウンロードするキーを登録,要素数が増える
  //     }
  //   }else{
  //     dict.setKey2CollF(targetKey, false);
  //   }
  // }
  // //現在地の地図があるかチェックなければ必ずダウンロードリストに入れる
  // if(!dict.existKey(currentKey)){
  //   rundownloadKeys.push_back(currentKey);
  // }

  // //ダウンロード枚数だけループしタグをつける
  
  // // readno順で、読み込み順はソートされている
  // for (const auto& rundownloadkey : rundownloadKeys) {//登録されたキーの要素数だけ繰り返す
  // // Serial.print(rundownloadkey);
  // // Serial.print("][");
  //   String filePath  = "/tokyo/"+String(int(floor(tileZoom)))+"/"+ rundownloadkey +".png";
  //   // String filePath  = "/tokyo/"+_tileZoom+"/"+ dict.getNewKey(shouldNo) +".png";
  //   if (SD.exists(filePath)) {
  //     // ダウンロード処理
  //     int tlimit = 0;
  //     //クロックを上げすぎているためか、描画完了のtrue信号に取りこぼしがあるよう。
  //     //一度脱調すると、もう取れなくなってしまうので、取りこぼしても一定の時間たったら処理をやめるようにした。
  //     while(!mapTileSprites[ringBuffNo].drawPngFile(SD, filePath, 0, 0)||tlimit>=500){//描画が終わったらFalseダウンロードし終わったらtrue？(勘）
  //       tlimit++;
  //     }

  //     dict.setNewKey(ringBuffNo, rundownloadkey);//先にキーを入れておかないとgetkey2ReadNoができない 
  //     dict.setSprptr(ringBuffNo, &mapTileSprites[ringBuffNo]);

  //     // Serial.println("書き終わった");
  //     // // ringBuffNo++; ringBuffNo = ringBuffNo%9;

  //     // Serial.print("-------------------------------------------------------------------------------------------------------");
  //     // Serial.println(ringBuffNo);
  //     ringBuffNo++; 
  //     ringBuffNo = ringBuffNo%9;
  //     // shouldNo = (shouldNo+1) % downloadLimitNum;
  //   }else{
  //     Serial.print("ファイルパスがない？");
  //     Serial.println(filePath);
  //   }
  // }

  // //描画処理
  // tft.fillScreen(TFT_BLACK);
  //   for (const auto& targetKey2 : allKeys) {

  //       //バッファの位置を更新し、再配置するための処理（currentKeyからの相対方向を更新）
  //       Vector2<int> result1 = calculateVectorDifference( currentKey, targetKey2 );
  //       // dict.setBuffPos(readBuffNo, result1);
  //       dict.setKey2BuffPos(targetKey2, result1);

  //       buffPos2.set(TFT_WIDTH_HALF + (dict.getkey2BuffPos(targetKey2).getX()*256-currentPos.x) * bairitu,
  //                   TFT_HEIGHT_HALF + (dict.getkey2BuffPos(targetKey2).getY()*256-currentPos.y) * bairitu);

  //       float matrix_map[6] = {
  //           bairitu,   //0 横2倍
  //           -0.0,      //1 横傾き
  //           buffPos2.getX(),      //2 X座標
  //           0.0,       //3 縦傾き
  //           bairitu,   //4 縦2倍
  //           buffPos2.getY()      //5 Y座標
  //       };
        
  //       if(dict.existKey(targetKey2)){
  //         if(dict.getKey2CollF(targetKey2)){
  //           dict.getKey2sprptr(targetKey2)->pushAffine(&tft, matrix_map);
  //           dict.getKey2sprptr(targetKey2)->drawRect(1, 1, 254, 254, TFT_RED);

  //           // Serial.print(currentPos.getX());
  //           // Serial.print(":");
  //           // Serial.println(currentPos.getX());
  //         }
  //       }

        

  //   // // mapTileSprites[i].fillRect(20*readBuffNo, 20*readBuffNo, 20,20, TFT_BLUE);
  //   // tft.setCursor(dict.getBuffPos(readBuffNo).getX(), dict.getBuffPos(readBuffNo).getY()+ (floor(readBuffNo/3)*8));
  //   // tft.print(String(readBuffNo));
  //   // tft.print(String(targetKey2));

  //   //過去から現在の進行方向ベクトルの線を引く
  //   tft.drawLine( TFT_WIDTH_HALF,
  //                 TFT_HEIGHT_HALF,
  //                 diffPos.x + TFT_WIDTH_HALF, 
  //                 diffPos.y + TFT_HEIGHT_HALF, TFT_BLUE);
  //   tft.setColor(HACO3_C8);
  //   tft.drawRect(TFT_WIDTH_HALF-64*bairitu, TFT_HEIGHT_HALF-60*bairitu, 128*bairitu, 120*bairitu);

  //   // if(collF){
  //   //   tft.drawLine(cR.x, cR.y,tR.x, tR.y,TFT_BLUE);
  //   //   tft.drawRect(tR.x, tR.y, 256*bairitu, 256*bairitu,TFT_BLUE);
  //   // }

  //   // xSemaphoreGive(xSemaphore); // セマフォを解放

  //   if(abs(currentPos.x-prePos.x)>TFT_WIDTH||abs(currentPos.y-prePos.y)>TFT_HEIGHT){
  //     // Serial.print("またぎました");
  //     // break; 
  //   }

  //   // Serial.print("[");
  //   // // Serial.print(dict.getNewKey(readBuffNo));
  //   // Serial.print(dict.getBuffPos(readBuffNo).getX());
  //   // Serial.print(":");
  //   // Serial.print(dict.getBuffPos(readBuffNo).getY());
  //   // Serial.print("] ");

  //   // readBuffNo++;

  // }

  // // Serial.println("");

  // //ダウンロードのタイミングを作るための当たり判定用の矩形

  // // Rect2D vR(TFT_WIDTH_HALF -(64-(mx))*bairitu,
  // //           TFT_HEIGHT_HALF-(60-(my))*bairitu,
  // //           (128+mx*2)*bairitu,
  // //           (120+my*2)*bairitu);
  // Rect2D vR(TFT_WIDTH_HALF -((64+mx)-(currentTilePos.getX()*mx))*bairitu,
  //           TFT_HEIGHT_HALF-((60+my)-(currentTilePos.getY()*my))*bairitu,
  //           (TFT_WIDTH+mx*2)*bairitu,
  //           (TFT_HEIGHT+my*2)*bairitu);
  // tft.drawRect(vR.x, vR.y, vR.w, vR.h,TFT_BLUE);

  // preallKeys = allKeys;
  // predownloadKeys = downloadKeys;
  
  // // Serial.println("");

  // allKeys.clear();
  // downloadKeys.clear();
  // rundownloadKeys.clear();

  // prePos = currentPos;
  // previousKeys = temporaryKeys;

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
  float spd = lua_tonumber(L, 2);
  soundNo = sn;
  soundSpeed = spd;
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
  float a = lua_tonumber(L, 1);
  float b = lua_tonumber(L, 2);
  if (a > b){
    lua_pushnumber(L, a);
  }else{
    lua_pushnumber(L, b);
  }
  return 1;
}

int RunHaco8Game::l_mid(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  float x = lua_tonumber(L, 1);
  float y = lua_tonumber(L, 2);
  float z = lua_tonumber(L, 3);

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
  float a = lua_tonumber(L, 1);
  float b = lua_tonumber(L, 2);
  if (a < b){
    lua_pushnumber(L, a);
  }else{
    lua_pushnumber(L, b);
  }
  return 1;
}


int RunHaco8Game::l_abs(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  float x = lua_tonumber(L, 1);
  if (x < 0) {
        lua_pushnumber(L, -x);
    } else {
        lua_pushnumber(L, x);
    }
  return 1;
}

int RunHaco8Game::l_sqrt(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  float x = lua_tonumber(L, 1);
  lua_pushnumber(L, sqrt(x));
  return 1;
}

int RunHaco8Game::l_distance(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  float x1 = lua_tonumber(L, 1);
  float y1 = lua_tonumber(L, 2);
  float x2 = lua_tonumber(L, 3);
  float y2 = lua_tonumber(L, 4);

  float dx = x2 - x1;
  float dy = y2 - y1;
  lua_pushnumber(L, sqrt(dx*dx + dy*dy));
  return 1;
} 

int normalizeAngle(float angle) {

    angle = int(fmod(angle,360)); // 360で割った余りを求める
    if (angle < 0) {
        angle += 360; // 負の角度を正の範囲に変換する
    }
    return angle;
}

// int RunHaco8Game::l_gsin(lua_State* L) {
//   RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
//   int x = lua_tointeger(L, 1);

//   // x を 0 から 359 の範囲に収める
//   x = (x % 360 + 360) % 360;

//   if (x >= 0 && x <= 89) {
//     lua_pushnumber(L, sinValues[x]);
//   } 
//   else if (x >= 90 && x <= 179) {
//     lua_pushnumber(L, sinValues[180 - x]);
//   } 
//   else if (x >= 180 && x <= 269) {
//     lua_pushnumber(L, -sinValues[x - 180]);
//   } 
//   else {
//     lua_pushnumber(L, -sinValues[360 - x]);
//   }
  
//   return 1;
// }

// int RunHaco8Game::l_gcos(lua_State* L) {
//   RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
//   int x = lua_tointeger(L, 1);
  
//   // x を 0 から 359 の範囲に収める
//   x = (x % 360 + 360 - 90) % 360;

//   if (x >= 0 && x <= 89) {
//     lua_pushnumber(L, sinValues[x]);
//   } 
//   else if (x >= 90 && x <= 179) {
//     lua_pushnumber(L, sinValues[180 - x]);
//   } 
//   else if (x >= 180 && x <= 269) {
//     lua_pushnumber(L, -sinValues[x - 180]);
//   } 
//   else {
//     lua_pushnumber(L, -sinValues[360 - x]);
//   }
  
//   return 1;
// }

float gsin(float angle)
{
    // int intangle = static_cast<int>(normalizeAngle(angle));

    // if (intangle >= 0 && intangle <= 89) {
    //     return sinValues[intangle];
    // } 
    // else if (intangle >= 90 && intangle <= 179) {
    //     return sinValues[180 - intangle];
    // } 
    // else if (intangle >= 180 && intangle <= 269) {
    //     return -sinValues[intangle - 180];
    // } 
    // else {
    //     return -sinValues[360 - intangle];
    // }
}

float gcos(float angle) {

    // int intangle = static_cast<int>(normalizeAngle(angle));
    // intangle = (intangle % 360 + 360 - 90) % 360;

    // if (intangle >= 0 && intangle <= 89) {
    //     return sinValues[intangle];
    // } 
    // else if (intangle >= 90 && intangle <= 179) {
    //     return sinValues[180 - intangle];
    // } 
    // else if (intangle >= 180 && intangle <= 269) {
    //     return -sinValues[intangle - 180];
    // } 
    // else {
    //     return -sinValues[360 - intangle];
    // }
}


int RunHaco8Game::l_sin(lua_State* L) {
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  float x = lua_tonumber(L, 1);
  lua_pushnumber(L, sin(x));
  return 1;
}

int RunHaco8Game::l_cos(lua_State* L) {
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  float x = lua_tonumber(L, 1);
  lua_pushnumber(L, cos(x));
  return 1;
}


int RunHaco8Game::l_atan2(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  float y = lua_tonumber(L, 1);//intに入れた時点でfloorされてる
  float x = lua_tonumber(L, 2);//intに入れた時点でfloorされてる
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

int RunHaco8Game::l_srnd(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int s = lua_tointeger(L, 1);
  srand(s);
  // lua_pushinteger(L, 0);
  return 0;
}

int RunHaco8Game::l_sgn(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  float x = lua_tonumber(L, 1);
  if (x > 0) {
    lua_pushinteger(L, 1);
  } else if (x < 0) {
    lua_pushinteger(L, -1);
  } else {
    lua_pushinteger(L, 0);
  }
  return 1;
}

int RunHaco8Game::l_shl(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  lua_pushinteger(L, x << y);
  return 1;
}

int RunHaco8Game::l_shr(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  lua_pushinteger(L, x >> y);
  return 1;
}

int RunHaco8Game::l_band(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  lua_pushinteger(L, x & y);
  return 1;
}

int RunHaco8Game::l_bnot(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  lua_pushinteger(L, ~x);
  return 1;
}

int RunHaco8Game::l_bor(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  lua_pushinteger(L, x | y);
  return 1;
}

int RunHaco8Game::l_bxor(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  lua_pushinteger(L, x ^ y);
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
    tft.setClipRect(0, 0, TFT_WIDTH, TFT_HEIGHT);
    //tft.setCursor(0,0);
  }else if(x != NULL && y != NULL && cn == NULL){//位置指定ありの場合
    tft.setTextColor(lua_rgb24to16(self->col[0], self->col[1], self->col[2]));//文字背景を消せる
    tft.setCursor(x,y);
    tft.setClipRect(x, y, TFT_WIDTH-x, TFT_HEIGHT-y);
  }else if(x != NULL && y != NULL && cn != NULL){//位置指定、、色指定ありの場合
    tft.setTextColor(lua_rgb24to16(self->col[0], self->col[1], self->col[2]));//文字背景を消せる
    tft.setCursor(x,y);
    tft.setClipRect(x, y, TFT_WIDTH-x, TFT_HEIGHT-y);
  }
  // tft.setCursor(x,y);
  // self->col[0] = self->clist[cn][0]; // 5bit
  // self->col[1] = self->clist[cn][1]; // 6bit
  // self->col[2] = self->clist[cn][2];       // 5bit
  // tft.setTextColor(lua_rgb24to16(self->col[0], self->col[1], self->col[2]));//文字背景を消せる
  tft.println(text);
  tft.clearClipRect();
  Serial.print(text);//シリアルにも出力
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

  float xa = lua_tonumber(L, 1);
  float ya = lua_tonumber(L, 2);
  float xb = lua_tonumber(L, 3);
  float yb = lua_tonumber(L, 4);
  int   cn = lua_tointeger(L, 5);
  int   cn2 = lua_tointeger(L, 6);
  int   cn3 = lua_tointeger(L, 7);
  if(cn != NULL)
  {
    self->col[0] = self->clist[cn][0]; // 5bit
    self->col[1] = self->clist[cn][1]; // 6bit
    self->col[2] = self->clist[cn][2]; // 5bit
  }
  if(cn3 != NULL)
  {
    self->col[0] = cn; // 5bit
    self->col[1] = cn2; // 6bit
    self->col[2] = cn3; // 5bit
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

int RunHaco8Game::l_cls(lua_State* L) {
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int n = lua_tointeger(L, 1);

  uint8_t r, g, b;
  if (n >= 0 && n < sizeof(clist) / sizeof(clist[0])) {
    r = self->clist[n][0];
    g = self->clist[n][1];
    b = self->clist[n][2];
  } else {
    r = g = b = 0; // Default to black color if n is out of range
  }

  self->col[0] = r; // 5bit
  self->col[1] = g; // 6bit
  self->col[2] = b; // 5bit

  tft.fillRect(0, 0, TFT_WIDTH, TFT_HEIGHT, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));

  return 0;
}

// int RunHaco8Game::l_cls(lua_State* L){
//   RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
//   int r,g,b;
//   int n = lua_tointeger(L, 1);
//   if(n == NULL){
//     n = 0;//黒の番号を入れて塗りつぶす
//   }

//   if(n == 0){ r = 0;     g = 0;    b = 0; }//0: 黒色
//   else if(n == 1){ r = 27;    g = 42;   b = 86; }//1: 暗い青色
//   else if(n == 2){ r = 137;   g = 24;   b = 84; }//2: 暗い紫色
//   else if(n == 3){ r = 0;     g = 139;  b = 75; }//3: 暗い緑色
//   else if(n == 4){ r = 183;   g = 76;   b = 45; }//4: 茶色
//   else if(n == 5){ r = 97;    g = 87;   b = 78; }//5: 暗い灰色
//   else if(n == 6){ r = 194;   g = 195;  b = 199; }//6: 明るい灰色
//   else if(n == 7){ r = 255;   g = 241;  b = 231; }//7: 白色

//   else if(n == 8){ r = 255;   g = 0;    b = 70; }//8: 赤色
//   else if(n == 9){ r = 255;   g = 160;  b = 0; }//9: オレンジ
//   else if(n == 10){ r = 255;  g = 238;  b = 0; }//10: 黄色
//   else if(n == 11){ r = 0;    g = 234;  b = 0; }//11: 緑色
//   else if(n == 12){ r = 0;    g = 173;  b = 255; }//12: 水色
//   else if(n == 13){ r = 134;  g = 116;  b = 159; }//13: 藍色
//   else if(n == 14){ r = 255;  g = 107;  b = 169; }//14: ピンク
//   else if(n == 15){ r = 255;  g = 202;  b = 165; }//15: 桃色

//   self->col[0] = r; // 5bit
//   self->col[1] = g; // 6bit
//   self->col[2] = b;       // 5bit

//   tft.fillRect(0,0,128,128,lua_rgb24to16(self->col[0], self->col[1], self->col[2]));

//   return 0;
// }

int RunHaco8Game::l_t(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  lua_pushinteger(L, frame);
  return 1;
}

//ここから3D関連
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
// #include <lua.hpp>
//3d関連
struct Vertex {
    float x;
    float y;
    float z;
};
struct Point2D {
    int x;
    int y;

    Point2D(int _x, int _y) : x(_x), y(_y) {}
};

// template <typename T>
// class Matrix4 {
// public:
//     T data[4][4];

//     Matrix4() {
//         // 行列を単位行列で初期化
//         loadIdentity();
//     }

//     void loadIdentity() {
//         for (int i = 0; i < 4; ++i) {
//             for (int j = 0; j < 4; ++j) {
//                 data[i][j] = (i == j) ? 1 : 0;
//             }
//         }
//     }

//     // ここに行列演算の関数を追加

//     // ビュー行列を計算する関数
//     static Matrix4 lookAt(const Vector3<T>& eye, const Vector3<T>& center, const Vector3<T>& up) {
//         Matrix4 viewMatrix;

//         Vector3<T> f = (center - eye).normalize();
//         Vector3<T> u = up.normalize();
//         Vector3<T> s = f.cross(u).normalize();
//         u = s.cross(f);

//         viewMatrix.data[0][0] = s.x;
//         viewMatrix.data[1][0] = s.y;
//         viewMatrix.data[2][0] = s.z;
//         viewMatrix.data[0][1] = u.x;
//         viewMatrix.data[1][1] = u.y;
//         viewMatrix.data[2][1] = u.z;
//         viewMatrix.data[0][2] = -f.x;
//         viewMatrix.data[1][2] = -f.y;
//         viewMatrix.data[2][2] = -f.z;
//         viewMatrix.data[3][0] = -s.dot(eye);
//         viewMatrix.data[3][1] = -u.dot(eye);
//         viewMatrix.data[3][2] = f.dot(eye);

//         return viewMatrix;
//     }

//     // その他の行列演算関数をここに追加

//     // 行列の乗算
//     Matrix4 operator*(const Matrix4& other) const {
//         Matrix4 result;

//         for (int i = 0; i < 4; ++i) {
//             for (int j = 0; j < 4; ++j) {
//                 result.data[i][j] = 0;
//                 for (int k = 0; k < 4; ++k) {
//                     result.data[i][j] += data[i][k] * other.data[k][j];
//                 }
//             }
//         }

//         return result;
//     }
// };

// Matrix4 calculateViewMatrix(const CameraObj& camera) {
//     // カメラの位置を (x, y, z) として、ビュー行列を計算する
//     Vector3<float> eye(camera.x, camera.y, camera.z);
//     Vector3<float> center(camera.x2, camera.y2, camera.z2);
//     Vector3<float> up(0.0f, 1.0f, 0.0f); // 上方向を指定（通常はY軸方向）

//     // LookAt行列を計算
//     Matrix4 viewMatrix = Matrix4::lookAt(eye, center, up);

//     return viewMatrix;
// }






static RunHaco8Game::CameraObj camera;
static RunHaco8Game::LightObj light;
static RunHaco8Game::CubeObj plyObj;

std::string convertToDecimalNotation(float value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << value;
    return oss.str();
}

std::vector<Vertex> parseSTLFile(const std::string& filename) {
    std::vector<Vertex> vertices;

    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return vertices;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);

        std::string keyword;
        iss >> keyword;

        if (keyword == "vertex") {
            Vertex vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
    }

    return vertices;
}


// ベクトルの長さを計算する関数
float RunHaco8Game::calculateLength(float x, float y, float z) {
  return std::sqrt(x * x + y * y + z * z);
}

// ベクトルの正規化を行う関数
Vector3<float> RunHaco8Game::normalize(float x, float y, float z) {
  float length = calculateLength(x, y, z);
  return {x / length, y / length, z / length};
}

// 2つのベクトルの内積を計算する関数
float RunHaco8Game::calculateDotProduct(const Vector3<float>& v1, const Vector3<float>& v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// 3つの頂点から法線ベクトルを計算する関数
Vector3<float> RunHaco8Game::calculateNormal(const Vector3<float>& v1, const Vector3<float>& v2, const Vector3<float>& v3) {
  float mx = v2.x - v1.x;
  float my = v2.y - v1.y;
  float uz = v2.z - v1.z;
  float vx = v3.x - v1.x;
  float vy = v3.y - v1.y;
  float vz = v3.z - v1.z;

  float nx = my * vz - uz * vy;
  float ny = uz * vx - mx * vz;
  float nz = mx * vy - my * vx;

  return normalize(nx, ny, nz);
}

// ポリゴンの明るさを計算する関数
// float RunHaco8Game::calculateBrightness(const Vector3<float>& v1, const Vector3<float>& v2, const Vector3<float>& v3, const RunHaco8Game::LightObj& light) {
//   // ポリゴンの法線ベクトルを計算
//   Vector3<float> normal = calculateNormal(v1, v2, v3);
//   // 光源ベクトルを計算
//   Vector3<float> lightVector = normalize(light.x, light.y, light.z);
//   // ポリゴンの法線ベクトルと光源ベクトルの角度を計算
//   float dotProduct = calculateDotProduct(normal, lightVector);
//   // 明るさを0~1の範囲に正規化
//   float brightness = (dotProduct + 1) / 2;
//   return brightness;
// }

// ポリゴンの明るさを計算する関数
float RunHaco8Game::calculateBrightness(const Vector3<float>& v1, const Vector3<float>& v2, const Vector3<float>& v3, const RunHaco8Game::LightObj& light) {
  // ポリゴンの法線ベクトルを計算
  Vector3<float> normal = calculateNormal(v1, v2, v3);
  // 光源ベクトルを計算して正規化
  Vector3<float> lightVector = normalize(light.x, light.y, light.z);
  
  // ポリゴンの法線ベクトルと光源ベクトルの角度を計算
  float dotProduct = calculateDotProduct(normal, lightVector);
  
  // 明るさを0~1の範囲に正規化（ここが修正点）
  // dotProductの範囲は-1から1ですが、角度の余弦の性質から0から1に正規化する必要があります
  float normalizedBrightness = (dotProduct + 1.0f) / 2.0f;
  
  return normalizedBrightness;
}


// テーブル内の要素のインデックス
const int VRTCS_TABLE_INDEX = 1;

// テーブル内の要素のキー
const char* V1_KEY = "v1";
const char* V2_KEY = "v2";
const char* V3_KEY = "v3";

inline float clamp(float value, float min, float max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    } else {
        return value;
    }
}

Vector2<float> projectToScreen(const Vector3<float>& vertex) {
    // パースペクティブ補正とズーム適用
    float distanceFromCamera = 64.0f - vertex.z;
    float scale = 640.0f / distanceFromCamera * camera.zoom;

    float scaledX = vertex.x * scale + 64.0f;
    float scaledY = vertex.y * scale + 64.0f;

    return Vector2<float>(static_cast<int>(floor(scaledX)), static_cast<int>(floor(scaledY)));
}

Vector3<float> transformToCameraSpace(const Vector3<float>& vertex) {
    // オブジェクト座標系での座標変換
    float x = vertex.x - plyObj.x;
    float y = vertex.y - plyObj.y;
    float z = vertex.z - plyObj.z;

    // オブジェクトの回転行列を適用
    float cosAngle = -sin((fmod((plyObj.angle + 90), 360)) * RAD_ONE);
    float sinAngle = sin((fmod(plyObj.angle, 360)) * RAD_ONE);

    float transformedX = x * cosAngle - z * sinAngle;
    float transformedZ = z * cosAngle + x * sinAngle;
    float transformedY = y;

    // カメラ座標系での座標変換
    float sinCamera = sin((fmod(camera.anglex, 360)) * RAD_ONE);
    float cosCamera = -sin((fmod((camera.anglex + 90), 360)) * RAD_ONE);

    float transformedXCamera = transformedX * cosCamera - transformedZ * sinCamera;
    float transformedZCamera = transformedZ * cosCamera + transformedX * sinCamera;
    float transformedYCamera = transformedY;

    return Vector3<float>(transformedXCamera, transformedYCamera, transformedZCamera);
}

void drawLine(const Vector3<float>& v1, const Vector3<float>& v2, int colangle) {
    // カメラ座標系への座標変換を行う
    Vector3<float> transformedV1 = transformToCameraSpace(v1);
    Vector3<float> transformedV2 = transformToCameraSpace(v2);

    // 3D線分を2Dスクリーン上に投影
    Vector2<float> projectedV1 = projectToScreen(transformedV1);
    Vector2<float> projectedV2 = projectToScreen(transformedV2);

    // 2Dスクリーン上に線を描画
    // draw2DLine(projectedV1, projectedV2, colangle);
}

// 点を指定した角度だけ回転する関数
Vector3<float> rotatePoint(const Vector3<float>& point, float angle) {
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);

    float rotatedX = point.getX() * cosAngle - point.getZ() * sinAngle;
    float rotatedY = point.getY();
    float rotatedZ = point.getZ() * cosAngle + point.getX() * sinAngle;

    return Vector3<float>(rotatedX, rotatedY, rotatedZ);
}

// 点を平行移動する関数
Vector3<float> transformPoint(const Vector3<float>& point, const Vector3<float>& translation) {
    float transformedX = point.getX() + translation.getX();
    float transformedY = point.getY() + translation.getY();
    float transformedZ = point.getZ() + translation.getZ();

    return Vector3<float>(transformedX, transformedY, transformedZ);
}

// カメラ座標系への変換を行う関数
Vector3<float> applyCoordinateTransform(const Vector3<float>& point) {
    float cosAnglex = std::cos(camera.anglex*PI/180);
    float sinAnglex = std::sin(camera.anglex*PI/180);
    float cosAngley = std::cos(camera.angley*PI/180);
    float sinAngley = std::sin(camera.angley*PI/180);
    float cosAnglez = std::cos(camera.anglez*PI/180);
    float sinAnglez = std::sin(camera.anglez*PI/180);

    float x = point.getX();
    float y = point.getY();
    float z = point.getZ();

    // float x1 = x * cosAngley * cosAnglez - z * cosAngley * sinAnglez;
    // float y1 = y * cosAnglex - (x * cosAngley * cosAnglez - z * cosAngley * sinAnglez) * sinAnglex;
    // float z1 = x * sinAngley * cosAnglez + z * sinAngley * sinAnglez;

    // float x2 = x1 * cosAnglex - z1 * sinAnglex + camera.x;
    // float y2 = y1 + camera.y;
    // float z2 = x1 * sinAnglex + z1 * cosAnglex + camera.z;

    

    // float x2 = x1 * cosAnglex - z1 * sinAnglex + camera.x;
    float x2 = x + cosAnglex * 60;
    float y2 = y - sinAngley * 60;
    float z2 = point.getZ();

    return Vector3<float>(x2, y2, z2);
}


Vector3<float> applyScale(const Vector3<float>& point, float scale) {
    float scaledX = point.x * scale;
    float scaledY = point.y * scale;
    float scaledZ = point.z * scale;

    return Vector3<float>(scaledX, scaledY, scaledZ);
}


// Vector3<float> rotatePoint(const Vector3<float>& point, float angle) {
//     float cosAngle = std::cos(angle);
//     float sinAngle = std::sin(angle);

//     float rotatedX = point.x * cosAngle - point.z * sinAngle;
//     float rotatedZ = point.z * cosAngle + point.x * sinAngle;
//     float rotatedY = point.y;

//     return Vector3<float>(rotatedX, rotatedY, rotatedZ);
// }

// Vector3<float> transformPoint(const Vector3<float>& point, const Vector3<float>& translation) {
//     float transformedX = point.x - translation.x;
//     float transformedY = point.y - translation.y;
//     float transformedZ = point.z - translation.z;

//     return Vector3<float>(transformedX, transformedY, transformedZ);
// }

// Vector3<float> applyCoordinateTransform(const Vector3<float>& point) {
//     float cosAnglex = std::cos(camera.anglex);
//     float sinAnglex = std::sin(camera.anglex);
//     float cosAngley = std::cos(camera.angley);
//     float sinAngley = std::sin(camera.angley);
//     float cosAnglez = std::cos(camera.anglez);
//     float sinAnglez = std::sin(camera.anglez);

//     float x = point.getX();
//     float y = point.getY();
//     float z = point.getZ();

//     // 座標変換
//     float x1 = x * cosAngley * cosAnglez - z * cosAngley * sinAnglez;
//     float y1 = y * cosAnglex - (x * cosAngley * cosAnglez - z * cosAngley * sinAnglez) * sinAnglex;
//     float z1 = x * sinAngley * cosAnglez + z * sinAngley * sinAnglez;

//     // カメラ座標系への座標変換
//     float cosCamera = std::cos((fmod(camera.anglex, 360)) * RAD_ONE);
//     float sinCamera = std::sin((fmod((camera.anglex + 90), 360)) * RAD_ONE);

//     float x2 = x1 * cosCamera - z1 * sinCamera + camera.x2;
//     float y2 = y1 + camera.y2;
//     float z2 = x1 * sinCamera + z1 * cosCamera + camera.z2;

//     return Vector3<float>(x2, y2, z2);
// }



void renderPolygon(const std::vector<std::vector<float>>& polygonData, int colangle) {
  // RunLuaGame gameInstance;

  // for (size_t i = 0; i < polygonData.size(); i += 3) {
  //   // 頂点座標を取得
  //   Vector3<float> v1(static_cast<float>(polygonData[i    ][0]), static_cast<float>(polygonData[i    ][1]), static_cast<float>(polygonData[i    ][2]));
  //   Vector3<float> v2(static_cast<float>(polygonData[i + 1][0]), static_cast<float>(polygonData[i + 1][1]), static_cast<float>(polygonData[i + 1][2]));
  //   Vector3<float> v3(static_cast<float>(polygonData[i + 2][0]), static_cast<float>(polygonData[i + 2][1]), static_cast<float>(polygonData[i + 2][2]));

  //   // オブジェクトの回転行列を計算
  //   float angle = plyObj.angle * 3.14159f / 180.0f; // オブジェクトの角度度
  //   Vector3<float> movV1 = rotatePoint(v1, angle);
  //   Vector3<float> movV2 = rotatePoint(v2, angle);
  //   Vector3<float> movV3 = rotatePoint(v3, angle);

  //   Vector3<float> translation = {plyObj.x, plyObj.y, plyObj.z};

  //   // オブジェクトの座標系での座標変換
  //   movV1 = transformPoint(movV1, translation);
  //   movV2 = transformPoint(movV2, translation);
  //   movV3 = transformPoint(movV3, translation);

    // カメラの座標系を更新し、座標変換行列を取得
    // camera.update();
    // Matrix4 viewMatrix = camera.getViewMatrix();

    // オブジェクトの座標系での座標変換を行う
    // movV1 = transformPoint(movV1, viewMatrix);
    // movV2 = transformPoint(movV2, viewMatrix);
    // movV3 = transformPoint(movV3, viewMatrix);



    // ポリゴンの描画処理
    for (size_t i = 0; i < polygonData.size(); i += 3) {
        // 頂点座標を取得
        Vector3<float> v1(static_cast<float>(polygonData[i    ][0]), static_cast<float>(polygonData[i    ][1]), static_cast<float>(polygonData[i    ][2]));
        Vector3<float> v2(static_cast<float>(polygonData[i + 1][0]), static_cast<float>(polygonData[i + 1][1]), static_cast<float>(polygonData[i + 1][2]));
        Vector3<float> v3(static_cast<float>(polygonData[i + 2][0]), static_cast<float>(polygonData[i + 2][1]), static_cast<float>(polygonData[i + 2][2]));
        

        //回転した座標に変換
        float objangle = plyObj.angle * 3.14159f / 180.0f; // オブジェクトの角度
        Vector3<float> movV1 = rotatePoint(v1, objangle);
        Vector3<float> movV2 = rotatePoint(v2, objangle);
        Vector3<float> movV3 = rotatePoint(v3, objangle);
        
        Vector3<float> translation = {plyObj.x, plyObj.y, plyObj.z};
        
        //移動した座標に変換
        movV1 = transformPoint(movV1, translation);
        movV2 = transformPoint(movV2, translation);
        movV3 = transformPoint(movV3, translation);

        

        // パースペクティブ補正とズーム適用
        float scale1 = 640.0f / (64.0f - movV1.getZ()) * camera.zoom;
        float scale2 = 640.0f / (64.0f - movV2.getZ()) * camera.zoom;
        float scale3 = 640.0f / (64.0f - movV3.getZ()) * camera.zoom;

        movV1 = applyScale(movV1, scale1);
        movV2 = applyScale(movV2, scale2);
        movV3 = applyScale(movV3, scale3);

        //カメラから見た座標に変換
        movV1 = applyCoordinateTransform(movV1);
        movV2 = applyCoordinateTransform(movV2);
        movV3 = applyCoordinateTransform(movV3);


        // camera.angley = camera.anglex;
        // camera.anglez = camera.anglex;


        // float cosAnglex = gcos(camera.anglex);
        // float sinAnglex = gsin(camera.anglex);
        // float cosAngley = gcos(camera.angley);
        // float sinAngley = gsin(camera.angley);
        // float cosAnglez = gcos(camera.anglez);
        // float sinAnglez = gsin(camera.anglez);

        // // 座標変換
        // movV1.set(
        //   (movV1.getX() * cosAngley * cosAnglez - movV1.getZ() * cosAngley * sinAnglez) * cosAnglex + movV1.getY() * sinAnglex,
        //   movV1.getY() * cosAnglex - (movV1.getX() * cosAngley * cosAnglez - movV1.getZ() * cosAngley * sinAnglez) * sinAnglex,
        //   (movV1.getX() * sinAngley * cosAnglez + movV1.getZ() * sinAngley * sinAnglez)
        // );

        // movV2.set(
        //   (movV2.getX() * cosAngley * cosAnglez - movV2.getZ() * cosAngley * sinAnglez) * cosAnglex + movV2.getY() * sinAnglex,
        //   movV2.getY() * cosAnglex - (movV2.getX() * cosAngley * cosAnglez - movV2.getZ() * cosAngley * sinAnglez) * sinAnglex,
        //   (movV2.getX() * sinAngley * cosAnglez + movV2.getZ() * sinAngley * sinAnglez)
        // );

        // movV3.set(
        //   (movV3.getX() * cosAngley * cosAnglez - movV3.getZ() * cosAngley * sinAnglez) * cosAnglex + movV3.getY() * sinAnglex,
        //   movV3.getY() * cosAnglex - (movV3.getX() * cosAngley * cosAnglez - movV3.getZ() * cosAngley * sinAnglez) * sinAnglex,
        //   (movV3.getX() * sinAngley * cosAnglez + movV3.getZ() * sinAngley * sinAnglez)
        // );

      //  // カメラ座標系での座標変換
      //   Vector3<float> normal = calculateNormal(movV1, movV2, movV3);

      //   Vector3<float> viewVector(
      //     camera.x2 - camera.x, 
      //     camera.y2 - camera.y, 
      //     camera.z2 - camera.z);

      //   viewVector = viewVector.normalize();
      //   float len = viewVector.length();

      //   float anglex = asin(viewVector.getY() / len);
      //   float angley = atan2(-viewVector.getX(), viewVector.getZ());
      //   float anglez = atan2(viewVector.getY(), viewVector.getX());

      //   float cosAnglex = cos(anglex);
      //   float sinAnglex = sin(anglex);
      //   float cosAngley = cos(angley);
      //   float sinAngley = sin(angley);
      //   float cosAnglez = cos(anglez);
      //   float sinAnglez = sin(anglez);

      //   // 座標変換
      //   movV1.set(
      //       (movV1.getX() * cosAngley * cosAnglez - movV1.getZ() * cosAngley * sinAnglez) * cosAnglex + movV1.getY() * sinAnglex,
      //       movV1.getY() * cosAnglex - (movV1.getX() * cosAngley * cosAnglez - movV1.getZ() * cosAngley * sinAnglez) * sinAnglex,
      //       (movV1.getX() * sinAngley * cosAnglez + movV1.getZ() * sinAngley * sinAnglez)
      //   );

      //   movV2.set(
      //       (movV2.getX() * cosAngley * cosAnglez - movV2.getZ() * cosAngley * sinAnglez) * cosAnglex + movV2.getY() * sinAnglex,
      //       movV2.getY() * cosAnglex - (movV2.getX() * cosAngley * cosAnglez - movV2.getZ() * cosAngley * sinAnglez) * sinAnglex,
      //       (movV2.getX() * sinAngley * cosAnglez + movV2.getZ() * sinAngley * sinAnglez)
      //   );

      //   movV3.set(
      //       (movV3.getX() * cosAngley * cosAnglez - movV3.getZ() * cosAngley * sinAnglez) * cosAnglex + movV3.getY() * sinAnglex,
      //       movV3.getY() * cosAnglex - (movV3.getX() * cosAngley * cosAnglez - movV3.getZ() * cosAngley * sinAnglez) * sinAnglex,
      //       (movV3.getX() * sinAngley * cosAnglez + movV3.getZ() * sinAngley * sinAnglez)
      //   );

        float sx1 = movV1.getX() + 64.0f;
        float sy1 = movV1.getY() + 64.0f;

        float sx2 = movV2.getX() + 64.0f;
        float sy2 = movV2.getY() + 64.0f;

        float sx3 = movV3.getX() + 64.0f;
        float sy3 = movV3.getY() + 64.0f;

        // // ポリゴンの法線ベクトルと視線ベクトルの角度を計算
        // float dotProduct = calculateDotProduct(normal, viewVector);
        // float angle1 = degrees(std::acos(dotProduct));

        // // ポリゴンの明るさを計算
        // float brightness = static_cast<float>(calculateBrightness(v1, v2, v3, light));

        // // 明るさに基づいてポリゴンの色を設定（0~255の範囲に収める）（最低値を0にせずまっ黒にならないように調整している）
        // int color = static_cast<int>(brightness * 200.0f + 55);
        // color = clamp(color, 0, 255);
        

        // // 角度が180度未満のポリゴンのみ描画
        // if (angle1 < 180.0f) {
        //   // 陰面消去を実施
        //   Point2D p1(static_cast<int>(floor(sx1)), static_cast<int>(floor(sy1)));
        //   Point2D p2(static_cast<int>(floor(sx2)), static_cast<int>(floor(sy2)));
        //   Point2D p3(static_cast<int>(floor(sx3)), static_cast<int>(floor(sy3)));

        //   // ポリゴンの面積を計算
        //   float area = (p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y);

        //   // 面積が正の場合はポリゴンを描画
        //   if (area > 0.0f) {
        //         // HSBからRGBに変換
        //     int r, g, b;
        //     gameInstance.hsbToRgb(plyObj.colangle, 127, color, r, g, b);

        //     byte col[3] = {0,0,0};

        //     // RGB値を設定
        //     col[0] = r; // Red
        //     col[1] = g; // Green
        //     col[2] = b; // Blue

        //     gameInstance.fillFastTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, lua_rgb24to16(col[0], col[1], col[2]));
        //       // gameInstance.fillFastTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, TFT_ORANGE); // 0xFFFFは描画色（白）を表します
        //   }
        // }

        Point2D p1(static_cast<int>(floor(sx1)), static_cast<int>(floor(sy1)));
        Point2D p2(static_cast<int>(floor(sx2)), static_cast<int>(floor(sy2)));
        Point2D p3(static_cast<int>(floor(sx3)), static_cast<int>(floor(sy3)));

        tft.drawLine(p1.x, p1.y, p2.x, p2.y, TFT_WHITE);
        tft.drawLine(p2.x, p2.y, p3.x, p3.y, TFT_WHITE);
        tft.drawLine(p3.x, p3.y, p1.x, p1.y, TFT_WHITE);
    }
}


// LuaのテーブルからC++のstd::vector<std::vector<float>>にデータを格納する関数
void getPolygonData(lua_State* L, std::vector<std::vector<float>>& polygonData) {
    // luaL_checktype(L, -1, LUA_TTABLE); // 引数のテーブルをチェック

    // lua_pushnil(L);  // テーブルの最初のキーを取得するためにnilをプッシュ
    // while (lua_next(L, -2) != 0) {
    //     luaL_checktype(L, -1, LUA_TTABLE); // テーブル内の要素がテーブルであることをチェック

    //     std::vector<float> vertex;
    //     lua_pushnil(L); // サブテーブルの最初のキーを取得するためにnilをプッシュ
    //     while (lua_next(L, -2) != 0) {
    //         luaL_checktype(L, -1, LUA_TNUMBER); // サブテーブル内の要素が数値であることをチェック

    //         float value = static_cast<float>(lua_tonumber(L, -1));
    //         vertex.push_back(value);

    //         lua_pop(L, 1); // サブテーブル内の値をポップ
    //     }
    //     polygonData.push_back(vertex);

    //     lua_pop(L, 1); // テーブル内の値をポップ
    // }
}


// std::vector<std::vector<float>> createIco() {
//     float phi = (1.0f + std::sqrt(5.0f)) / 2.0f;

//     std::vector<std::vector<float>> vertices = {
//         {-1.0f, phi, 0.0f}, {1.0f, phi, 0.0f}, {-1.0f, -phi, 0.0f}, {1.0f, -phi, 0.0f},
//         {0.0f, -1.0f, phi}, {0.0f, 1.0f, phi}, {0.0f, -1.0f, -phi}, {0.0f, 1.0f, -phi},
//         {phi, 0.0f, -1.0f}, {phi, 0.0f, 1.0f}, {-phi, 0.0f, -1.0f}, {-phi, 0.0f, 1.0f}
//     };

//     std::vector<std::vector<int>> faces = {
//         {0, 11, 5}, {0, 5, 1}, {0, 1, 7}, {0, 7, 10}, {0, 10, 11},
//         {1, 5, 9}, {5, 11, 4}, {11, 10, 2}, {10, 7, 6}, {7, 1, 8},
//         {3, 9, 4}, {3, 4, 2}, {3, 2, 6}, {3, 6, 8}, {3, 8, 9},
//         {4, 9, 5}, {2, 4, 11}, {6, 2, 10}, {8, 6, 7}, {9, 8, 1}
//     };

//     std::vector<std::vector<float>> polyVertices;

//     for (const auto& face : faces) {
//         Vector3<float> edge1(vertices[face[1]][0] - vertices[face[0]][0],
//                              vertices[face[1]][1] - vertices[face[0]][1],
//                              vertices[face[1]][2] - vertices[face[0]][2]);
//         Vector3<float> edge2(vertices[face[2]][0] - vertices[face[0]][0],
//                              vertices[face[2]][1] - vertices[face[0]][1],
//                              vertices[face[2]][2] - vertices[face[0]][2]);

//         // Calculate the face normal by cross product of two edges
//         Vector3<float> normal = edge1.cross(edge2).normalize();

//         std::vector<float> polyVertex;
//         for (int index : face) {
//             polyVertex.push_back(vertices[index][0]);
//             polyVertex.push_back(vertices[index][1]);
//             polyVertex.push_back(vertices[index][2]);
//         }

//         // Reverse vertices order if the normal points in the opposite direction
//         if (normal.z < 0.0f) {
//             std::reverse(polyVertex.begin(), polyVertex.end());
//         }

//         polyVertices.push_back(polyVertex);
//     }

//     return polyVertices;
// }


int RunHaco8Game::l_setply(lua_State* L) {
    RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));

    // polygonData = createIco();

  //   polygonData = {
  //   { 1.0f,  0.0f,  0.0f },//側面
  //   { 0.0f,  1.0f,  0.0f },
  //   { -1.0f,  -1.0f,  -1.0f },

  //   { 0.0f,  0.0f,  1.0f },//底面
  //   { 1.0f,  0.0f,  0.0f },
  //   { -1.0f,  -1.0f,  -1.0f },

  //   { 0.0f,  1.0f,  0.0f },//側面
  //   { 0.0f,  0.0f,  1.0f },
  //   { -1.0f,  -1.0f,  -1.0f },

  //   { 0.0f,  1.0f,  0.0f },//斜面
  //   { 1.0f,  0.0f,  0.0f },
  //   { 0.0f,  0.0f,  1.0f }
  // };

  


//   polygonData = {
//     { 1.0f,  1.0f,  1.0f },
//     {-1.0f,  1.0f, -1.0f },
//     { 1.0f, -1.0f, -1.0f },

//     {-1.0f, -1.0f,  1.0f },
//     { 1.0f, -1.0f, -1.0f },
//     {-1.0f,  1.0f, -1.0f },

//     { 1.0f, -1.0f, -1.0f },
//     {-1.0f, -1.0f,  1.0f },
//     {-1.0f,  1.0f, -1.0f },

//     {-1.0f, -1.0f,  1.0f },
//     { 1.0f,  1.0f,  1.0f },
//     { 1.0f, -1.0f, -1.0f }
// };

// polygonData = {
//     { 1.0f,  0.0f,  0.0f },
//     { 0.0f,  1.0f,  0.0f },
//     { 0.0f,  0.0f,  1.0f },

//     {-1.0f,  0.0f,  0.0f },
//     { 0.0f,  0.0f, -1.0f },
//     { 0.0f, -1.0f,  0.0f },

//     { 1.0f,  0.0f,  0.0f },
//     { 0.0f,  0.0f, -1.0f },
//     { 0.0f,  1.0f,  0.0f },

//     {-1.0f,  0.0f,  0.0f },
//     { 0.0f, -1.0f,  0.0f },
//     { 0.0f,  0.0f,  1.0f },

//     { 0.0f,  1.0f,  0.0f },
//     { 0.0f,  0.0f,  1.0f },
//     { 0.0f, -1.0f,  0.0f },

//     { 0.0f, -1.0f,  0.0f },
//     { 0.0f,  0.0f,  1.0f },
//     { 0.0f,  1.0f,  0.0f },

//     { 1.0f,  0.0f,  0.0f },
//     { 0.0f, -1.0f,  0.0f },
//     { 0.0f,  0.0f, -1.0f },

//     {-1.0f,  0.0f,  0.0f },
//     { 0.0f,  1.0f,  0.0f },
//     { 0.0f,  0.0f, -1.0f }
// };

  return 0;
}

// void RunHaco8Game::fill3DFastTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t c1){
  void RunHaco8Game::fill3DFastTriangle(int16_t x0, int16_t y0, int16_t z0,
                                        int16_t x1, int16_t y1, int16_t z1,
                                        int16_t x2, int16_t y2, int16_t z2,
                                        uint16_t c1) {
  tft.setColor(c1);
  int16_t a, b;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) { std::swap(y0, y1); std::swap(x0, x1); }
    if (y1 > y2) { std::swap(y2, y1); std::swap(x2, x1); }
    if (y0 > y1) { std::swap(y0, y1); std::swap(x0, x1); }

    if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
      a = b = x0;
      if (x1 < a)      a = x1;
      else if (x1 > b) b = x1;
      if (x2 < a)      a = x2;
      else if (x2 > b) b = x2;
      tft.drawFastHLine(a, y0, b - a + 1);
      return;
    }

    if ((x1-x0) * (y2-y0) == (x2-x0) * (y1-y0)) {
      tft.drawLine(x0,y0,x2,y2);
      return;
    }

    int16_t dy1 = y1 - y0;
    int16_t dy2 = y2 - y0;
    bool change = ((x1 - x0) * dy2 > (x2 - x0) * dy1);
    int16_t dx1 = abs(x1 - x0);
    int16_t dx2 = abs(x2 - x0);
    int16_t xstep1 = x1 < x0 ? -1 : 1;
    int16_t xstep2 = x2 < x0 ? -1 : 1;
    a = b = x0;
    if (change) {
      std::swap(dx1, dx2);
      std::swap(dy1, dy2);
      std::swap(xstep1, xstep2);
    }
    int16_t err1 = (std::max(dx1, dy1) >> 1)
                 + (xstep1 < 0
                   ? std::min(dx1, dy1)
                   : dx1);
    int16_t err2 = (std::max(dx2, dy2) >> 1)
                 + (xstep2 > 0
                   ? std::min(dx2, dy2)
                   : dx2);

    if (z0 < 0 || z0 > 1 || z1 < 0 || z1 > 1 || z2 < 0 || z2 > 1) {
        return;
    }

    //tft.startWrite();
    if (y0 != y1) {
      do {
        err1 -= dx1;
        while (err1 < 0) { err1 += dy1; a += xstep1; }
        err2 -= dx2;
        while (err2 < 0) { err2 += dy2; b += xstep2; }
        // if (y0 % 2 != 0) { // 奇数の行のみを処理
        // Zが０～１の時のみを処理
          if (z0 >= 0 && z0 <= 1 && z1 >= 0 && z1 <= 1 && z2 >= 0 && z2 <= 1) {
            tft.writeFastHLine(a, y0, b - a + 1);
          }
        // }
      } while (++y0 < y1);
    }

    if (change) {
      b = x1;
      xstep2 = x2 < x1 ? -1 : 1;
      dx2 = abs(x2 - x1);
      dy2 = y2 - y1;
      err2 = (std::max(dx2, dy2) >> 1)
           + (xstep2 > 0
             ? std::min(dx2, dy2)
             : dx2);
    } else {
      a = x1;
      dx1 = abs(x2 - x1);
      dy1 = y2 - y1;
      xstep1 = x2 < x1 ? -1 : 1;
      err1 = (std::max(dx1, dy1) >> 1)
           + (xstep1 < 0
             ? std::min(dx1, dy1)
             : dx1);
    }
    do {
      err1 -= dx1;
      while (err1 < 0) { err1 += dy1; if ((a += xstep1) == x2) break; }
      err2 -= dx2;
      while (err2 < 0) { err2 += dy2; if ((b += xstep2) == x2) break; }
      // if (y0 % 2 != 0) { // 奇数の行のみを処理
        tft.writeFastHLine(a, y0, b - a + 1);
      // }
    } while (++y0 <= y2);
    tft.endWrite();
}


void RunHaco8Game::fastPoint(const Vector3<float> v1, int starbrightness, int noInt) 
{

  uint16_t c1 = lua_rgb24to16(starbrightness, starbrightness, starbrightness);
  

  if(starbrightness>70 && starbrightness<=110){//110=４等級
    tft.startWrite();
    tft.writePixel(static_cast<int16_t>((v1.getX() + 1.0f) * TFT_WIDTH_HALF), 
      TFT_HEIGHT-static_cast<int16_t>((v1.getY() + 1.0f) * TFT_HEIGHT_HALF),
      c1);
    tft.endWrite();
  }
  else if(starbrightness>110 && starbrightness<184){
    tft.startWrite();
    tft.writePixel(static_cast<int16_t>((v1.getX() + 1.0f) * TFT_WIDTH_HALF), 
      TFT_HEIGHT-static_cast<int16_t>((v1.getY() + 1.0f) * TFT_HEIGHT_HALF),
      c1);
    tft.endWrite();
    
    // tft.fillCircle(static_cast<int16_t>((v1.getX() + 1.0f) * TFT_WIDTH_HALF), 
    // TFT_HEIGHT-static_cast<int16_t>((v1.getY() + 1.0f) * TFT_HEIGHT_HALF), 1, c1);
  }
  else if(starbrightness>=184){
    tft.fillCircle(static_cast<int16_t>((v1.getX() + 1.0f) * TFT_WIDTH_HALF), 
    TFT_HEIGHT-static_cast<int16_t>((v1.getY() + 1.0f) * TFT_HEIGHT_HALF), 1, c1); 

    
    
  }

  // tft.setTextWrap(false);
  // tft.setCursor(static_cast<int16_t>((v1.getX() + 1.0f) * TFT_WIDTH_HALF)+4, 
  // TFT_HEIGHT-static_cast<int16_t>((v1.getY() + 1.0f) * TFT_HEIGHT_HALF)+4);
  //       if(noInt==2061){tft.print("ベテルギウス");}
  //   else if(noInt==1713){tft.print("リゲル");}
  //   else if(noInt==5191){tft.print("アルカイド");}
  //   else if(noInt==4763){tft.print("ガクルックス");}
  //   else if(noInt==4357){tft.print("ゾズマ");}
  //   else if(noInt==264){tft.print("カシオペヤ");}
  //   else if(noInt==2491){tft.print("シリウス");}
  //   else if(noInt==2827){tft.print("アルドラ");}
  //   else if(noInt==3165){tft.print("ナオス");}
  //   else if(noInt==2990){tft.print("ポルックス");}
  //   else if(noInt==5054){tft.print("ミザール");}
  //   else if(noInt==5793){tft.print("アルフェッカ");}
  //   else if(noInt==6527){tft.print("シャウラ");}
  //   else if(noInt==7796){tft.print("サドル");}
  //   else if(noInt==7924){tft.print("デネブ");}
  //   else if(noInt==8162){tft.print("アルデラミン");}
  //   else if(noInt==8775){tft.print("シェアト");}
  //   else if(noInt==8308){tft.print("エニフ");}
  //   else if(noInt==8728){tft.print("フォーマルハウト");}
  //   else if(noInt==4301){tft.print("ドゥベー");}
  //   else if(noInt==4905){tft.print("アリオト");}
  //   else if(noInt==3982){tft.print("レグルス");}
  //   else if(noInt==4534){tft.print("デネボラ");}
  //   else if(noInt==4554){tft.print("フェクダ");}
  //   else if(noInt==7001){tft.print("ベガ");}
  //   else if(noInt==6705){tft.print("エルタニン");}
  //   else if(noInt==3748){tft.print("アルファルド");}
  //   else if(noInt==5056){tft.print("スピカ");}
  //   else if(noInt==3634){tft.print("スハイル");}
  //   else if(noInt==4853){tft.print("ミモザ");}
  //   else if(noInt==1903){tft.print("アルニラム");}
  //   else if(noInt==603){tft.print("アルマク");}
  //   else if(noInt==168){tft.print("シェダル");}
  //   else if(noInt==617){tft.print("ハマル");}
  //   else if(noInt==936){tft.print("アルゴル");}
  //   else if(noInt==1017){tft.print("ミルファク");}
  //   else if(noInt==1457){tft.print("アルデバラン");}
  //   else if(noInt==424){tft.print("ポラリス");}
  //   else if(noInt==2326){tft.print("カノープス");}
  //   else if(noInt==3685){tft.print("ミアプラキドゥス");}
  //   else if(noInt==2943){tft.print("プロキオン");}
  //   else if(noInt==99){tft.print("アンカー");}
  //   else if(noInt==472){tft.print("アケルナル");}
  //   else if(noInt==2249){tft.print("ミルザム");}
  //   else if(noInt==1948){tft.print("アルニタク");}
  //   else if(noInt==1790){tft.print("ベラトリクス");}//Bellatrix
  //   else if(noInt==1791){tft.print("エルナト");}//Elnath
  //   else if(noInt==2088){tft.print("メンカリナン");}//Menkalinan
  //   else if(noInt==5953){tft.print("ジュバ");}//Dschubba
  //   else if(noInt==6378){tft.print("サビク");}//Sabik
  //   else if(noInt==6217){tft.print("アトリア");}//Atria
  //   else if(noInt==6134){tft.print("アンタレス");}
  //   else if(noInt==5459){tft.print("Rigil Kentaurus");}
  //   else if(noInt==7790){tft.print("ピーコック");}
}

// void RunHaco8Game::point3D(const Vector3<float> v1, uint16_t c1) {
//   // tft.drawPixel((v1.getX() + 1.0f) * TFT_WIDTH_HALF, TFT_HEIGHT-(v1.getY() + 1.0f) * TFT_HEIGHT_HALF, c1);
//   fastPoint(static_cast<int16_t>(v1.getX()),
//             static_cast<int16_t>(v1.getY()),
//             static_cast<int16_t>(v1.getZ()),c1);
// }

int langle = 0;

void RunHaco8Game::triangle3D(const Vector3<float> v1,const Vector3<float> v2,const Vector3<float> v3) {
  // Vector3<int16_t> p1,p2,p3;

  // p1.set(static_cast<int16_t>((v1.getX() + 1.0f) * TFT_WIDTH_HALF),
  //         TFT_HEIGHT-static_cast<int16_t>((v1.getY() + 1.0f) * TFT_HEIGHT_HALF),
  //         v1.getZ());

  // p2.set(static_cast<int16_t>((v2.getX() + 1.0f) * TFT_WIDTH_HALF),
  //         TFT_HEIGHT-static_cast<int16_t>((v2.getY() + 1.0f) * TFT_HEIGHT_HALF),
  //         v2.getZ());

  // p3.set(static_cast<int16_t>((v3.getX() + 1.0f) * TFT_WIDTH_HALF),
  //         TFT_HEIGHT-static_cast<int16_t>((v3.getY() + 1.0f) * TFT_HEIGHT_HALF),
  //         v3.getZ());


        // カメラ座標系での座標変換
        Vector3<float> normal = calculateNormal(v1, v2, v3);

        Vector3<float> viewVector(
          camera.x2 - camera.x, 
          camera.y2 - camera.y, 
          camera.z2 - camera.z);

        viewVector = viewVector.normalize();
        float len = viewVector.length();

        // ポリゴンの法線ベクトルと視線ベクトルの角度を計算
        float dotProduct = calculateDotProduct(normal, viewVector);
        float angle1 = degrees(std::acos(dotProduct));

        

        // ポリゴンの明るさを計算
        float brightness = static_cast<float>(calculateBrightness(v1, v2, v3, light));

        // 明るさに基づいてポリゴンの色を設定（0~255の範囲に収める）（最低値を0にせずまっ黒にならないように調整している）
        int color = static_cast<int>(brightness * 200.0f + 55);
        color = clamp(color, 0, 255);

        // 角度が180度未満のポリゴンのみ描画
        if (angle1 < 180.0f) {
          // 陰面消去を実施
          Vector3<int16_t> p1,p2,p3;

          //画面に拡大表示
          p1.set(static_cast<int16_t>((v1.getX() + 1.0f) * TFT_WIDTH_HALF),
                  TFT_HEIGHT-static_cast<int16_t>((v1.getY() + 1.0f) * TFT_HEIGHT_HALF),
                  v1.getZ());

          p2.set(static_cast<int16_t>((v2.getX() + 1.0f) * TFT_WIDTH_HALF),
                  TFT_HEIGHT-static_cast<int16_t>((v2.getY() + 1.0f) * TFT_HEIGHT_HALF),
                  v2.getZ());

          p3.set(static_cast<int16_t>((v3.getX() + 1.0f) * TFT_WIDTH_HALF),
                  TFT_HEIGHT-static_cast<int16_t>((v3.getY() + 1.0f) * TFT_HEIGHT_HALF),
                  v3.getZ());

          // ポリゴンの面積を計算
          float area = (p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y);

          // 面積が正の場合はポリゴンを描画
          if (area > 0.0f) {
                // HSBからRGBに変換
            int r, g, b;
            hsbToRgb(plyObj.colangle, 127, color, r, g, b);

            byte col[3] = {0,0,0};

            // RGB値を設定
            col[0] = r; // Red
            col[1] = g; // Green
            col[2] = b; // Blue

            // gameInstance.fillFastTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, lua_rgb24to16(col[0], col[1], col[2]));
            fill3DFastTriangle(
              p1.getX(), p1.getY(), p1.getZ(), 
              p2.getX(), p2.getY(), p2.getZ(), 
              p3.getX(), p3.getY(), p3.getZ(), 
              lua_rgb24to16(col[0], col[1], col[2]));
            
            // Vector3<float> cp= {float(p1.getX()), float(p1.getY()), float(p1.getZ())};

            // point3D(cp,TFT_WHITE);
          }
        }
  
    // fill3DFastTriangle(
    //   p1.getX(), p1.getY(), p1.getZ(), 
    //   p2.getX(), p2.getY(), p2.getZ(), 
    //   p3.getX(), p3.getY(), p3.getZ(), 
    //   TFT_WHITE);

}


void RunHaco8Game::line3D(const Vector3<float> v1,const Vector3<float> v2,uint16_t c1) {
    tft.startWrite();
    tft.drawLine(static_cast<int16_t>((v1.getX() + 1.0f) * TFT_WIDTH_HALF), 
      TFT_HEIGHT-static_cast<int16_t>((v1.getY() + 1.0f) * TFT_HEIGHT_HALF),
                 static_cast<int16_t>((v2.getX() + 1.0f) * TFT_WIDTH_HALF), 
      TFT_HEIGHT-static_cast<int16_t>((v2.getY() + 1.0f) * TFT_HEIGHT_HALF),c1);
    tft.endWrite();
}

// void setSphere(struct POS* op, int numCorners, int numRings) {
//     float deg = 360.0f / numCorners;
//     for (int j = 0; j < numRings; j++) {
//         float r = sin(deg * j);
//         float z = cos(deg * j);
//         for (int i = 0; i < numCorners; i++) {
//             int k = j * numCorners + i;
//             op[k].x = sin(deg * i) * r;
//             op[k].y = cos(deg * i) * r;
//             op[k].z = z;
//         }
//     }
// }

// void setCylinder(struct POS* op, int numCorners, int numRings) {
//     float deg = 360.0f / numCorners;
// for(int i=0;i<numCorners; i++){
//       for (int j = 0; j < numRings; j++) {
//         int k = j * numCorners + i;
//         op[k].setX(gsin(cdeg*i));
//         op[k].setY(gcos(cdeg*i));
//         op[k].setZ(1.0f - 2.0/numRings*j);
//       }
//     }
// }

int RunHaco8Game::l_rendr(lua_State* L) {
  return 0;
}

  Vector3<float> myop;
  Vector3<float> myp;

std::vector<Vector3<float>> opg;
std::vector<Vector3<float>> pg;
std::vector<Vector3<float>> op;
std::vector<Vector3<float>> p;


int RunHaco8Game::l_starp(lua_State* L) {
   RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));

  int cam_angleX = lua_tointeger(L, 1);
  int cam_angleY = lua_tointeger(L, 2);
  int cam_angleZ = 0;
  float zoom = lua_tonumber(L, 3);
  camera.zoom = zoom;
  int degmode= lua_tointeger(L, 4);

  
  int d= lua_tointeger(L, 8);
  int m= lua_tointeger(L, 9);
  float s= lua_tonumber(L, 10);

  int h2= lua_tointeger(L, 5);
  int m2= lua_tointeger(L, 6);
  float s2= lua_tonumber(L, 7);

  // int d= lua_tointeger(L, 5);
  // int m= lua_tointeger(L, 6);
  // float s= lua_tonumber(L, 7);

  // int h2= lua_tointeger(L, 8);
  // int m2= lua_tointeger(L, 9);
  // float s2= lua_tonumber(L, 10);

  //   //星の座標を求めて格納
  myop.setX(cos(dmsToRadian(d,m,s)) * cos(hmsToRadian(h2,m2,s2)));
  myop.setZ(cos(dmsToRadian(d,m,s)) * sin(hmsToRadian(h2,m2,s2)));
  myop.setY(sin(dmsToRadian(d,m,s)));


  if (cam_angleX > 360 || cam_angleX <= 0 ) {
      // deg1 = fmod(deg1 , 360);
      cam_angleX%=360;
  }

  if (cam_angleY > 360 || cam_angleY < 0 ) {
      // deg2 = fmod(deg2 , 360);
      cam_angleY%=360;
  }

  if (cam_angleZ > 360 || cam_angleZ < 0 ) {
      cam_angleZ%=360;
  }

//   // 与えられた角度をラジアンに変換
//   float angleX = cam_angleX * RAD_ONE;
//   float angleY = cam_angleY * RAD_ONE;

//   // 各軸周りの回転行列を計算
//   float cosX = cos(angleX);
//   float sinX = sin(angleX);
//   float cosY = cos(angleY);
//   float sinY = sin(angleY);


//     if(drawmode == 0||drawmode == 1){

//     // 座標変換
//   for (int i = 0; i < BSTARBUFNUM; i++) {
//       float x = op[i].getX();
//       float y = op[i].getY();
//       float z = op[i].getZ();

//       // y軸周りの回転
//       float tempX1 = x * cosY - z * sinY;
//       float tempZ1 = x * sinY + z * cosY;

//       // x軸周りの回転
//       p[i].setX(tempX1);
//       p[i].setY(y * cosX + tempZ1 * sinX);
//       p[i].setZ(-y * sinX + tempZ1 * cosX);

//       //world座標変換
//       // if(sw == 0){
//       //   p[i].setX(cosX * op[i].getX() + -sinX * op[i].getY());
//       //   p[i].setY(sinX * op[i].getX() +  cosX * op[i].getY());
//       //   p[i].setZ(op[i].getZ());
//       // }
//       // else if(sw == 1){
//       //   p[i].setX(cosX * op[i].getX() + -sinX * op[i].getZ());
//       //   p[i].setY(op[i].getY());
//       //   p[i].setZ(sinX * op[i].getX() +  cosX * op[i].getZ());
//       // }
//       // else if(sw == 2){
//       //   p[i].setX(op[i].getX());
//       //   p[i].setY(cosY * op[i].getY() + -sinY * op[i].getZ());
//       //   p[i].setZ(sinY * op[i].getY() +  cosY * op[i].getZ());
//       // }

//       // カメラの移動
//       p[i].setZ(-p[i].getZ() - 1);//カメラの位置0だと表示されない
//       p[i].setZ(-p[i].getZ());//記号を反転させる
//       p[i].setX(p[i].getX() / p[i].getZ()/camera.zoom);
//       p[i].setY(p[i].getY() / p[i].getZ()/camera.zoom);
//       p[i].setZ(p[i].getZ() / 5);
//   }

// //星を描画
//     for(int i =0;i < BSTARBUFNUM; i++)
//     {

//       int starbrightness = bsParamInt8t[i][0];
//       int noInt = bsParamInt16t[i][0];
//       self->fastPoint(p[i], starbrightness, noInt);
                  
//     }
//     }


//     return 0;
// }

// int RunHaco8Game::l_starp(lua_State* L) {
//   RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
//   int cam_angleX = lua_tointeger(L, 1);
//   int cam_angleY = lua_tointeger(L, 2);
//   int cam_angleZ = 0;
//   float zoom = lua_tonumber(L, 3);
//   camera.zoom = zoom;
//   int degmode= lua_tointeger(L, 4);
//   // float rad1= lua_tonumber(L, 5);
//   // float rad2= lua_tonumber(L, 6);


//   // int cam_angleX = lua_tointeger(L, 1);
//   // int cam_angleY = lua_tointeger(L, 2);
//   // int cam_angleZ = 0;
//   // float zoom = lua_tonumber(L, 3);
//   // camera.zoom = zoom;
//   // int degmode= lua_tointeger(L, 4);

//   int h_d= lua_tointeger(L, 5);
//   int m= lua_tointeger(L, 6);
//   float s= lua_tonumber(L, 7);

//   float rad1 = 0.0f;
//   float rad2 = 0.0f;

//   if(degmode==0){//時分秒
//     rad1=hmsToRadian(h_d,m,s);
//     // rad1 =PI/2;
//   }else if(degmode==1){//度分秒
//     rad2=dmsToRadian(h_d,m,s);
//     // rad2 =PI/2;
//   }

//   // int h1= lua_tointeger(L, 5);
//   // int m1= lua_tointeger(L, 6);
//   // int s1= lua_tointeger(L, 7);

//   // int d2= lua_tointeger(L, 8);
//   // int m2= lua_tointeger(L, 9);
//   // int s2= lua_tointeger(L, 10);

//   // if(degmode==0){//時分秒
//   //   rad1=hmsToRadian(h1,m1,s1);
//   // }else if(degmode==1){//度分秒
//   //   rad2=dmsToRadian(d2,m2,s2);
//   // }

//   //星の座標を求めて格納
//   myop.setX(cos(rad2) * cos(rad1));
//   myop.setZ(cos(rad2) * sin(rad1));
//   myop.setY(sin(rad2));

//   if (cam_angleX > 360 || cam_angleX <= 0 ) {
//       // deg1 = fmod(deg1 , 360);
//       cam_angleX%=360;
//   }

//   if (cam_angleY > 360 || cam_angleY < 0 ) {
//       // deg2 = fmod(deg2 , 360);
//       cam_angleY%=360;
//   }

//   if (cam_angleZ > 360 || cam_angleZ < 0 ) {
//       cam_angleZ%=360;
//   }

  // 与えられた角度をラジアンに変換
  float angleX = cam_angleX * RAD_ONE;
  float angleY = cam_angleY * RAD_ONE;

  // 各軸周りの回転行列を計算
  float cosX = cos(angleX);
  float sinX = sin(angleX);
  float cosY = cos(angleY);
  float sinY = sin(angleY);


      float x = myop.getX();
      float y = myop.getY();
      float z = myop.getZ();

      
      // y軸周りの回転
      float tempX1 = x * cosY - z * sinY;
      float tempZ1 = x * sinY + z * cosY;

      // x軸周りの回転
      myp.setX(tempX1);
      myp.setY(y * cosX + tempZ1 * sinX);
      myp.setZ(-y * sinX + tempZ1 * cosX);


      // カメラの移動
      myp.setZ(-myp.getZ() - 1);//カメラの位置0だと表示されない
      myp.setZ(-myp.getZ());//記号を反転させる
      myp.setX(myp.getX() / myp.getZ()/camera.zoom);
      myp.setY(myp.getY() / myp.getZ()/camera.zoom);
      myp.setZ(myp.getZ() / 5);



  // float x = myop.getX();
  // float y = myop.getY();
  // float z = myop.getZ();

  // // // y軸周りの回転
  // float tempX1 = x * cosY - z * sinY;
  // float tempZ1 = x * sinY + z * cosY;

  // // x軸周りの回転
  // myp.setX(tempX1);
  // myp.setY(y * cosX + tempZ1 * sinX);
  // myp.setZ(-y * sinX + tempZ1 * cosX);

  
  
  // // カメラの移動
  // myp.setZ(-myp.getZ() - 1);//カメラの位置0だと表示されない
  // myp.setZ(-myp.getZ());//記号を反転させる
  // myp.setX(myp.getX() / myp.getZ()/camera.zoom);
  // myp.setY(myp.getY() / myp.getZ()/camera.zoom);
  // myp.setZ(myp.getZ() / 5);



  // // テーブルを作成して値をセット
  // lua_newtable(L);

  // lua_pushinteger(L, static_cast<int16_t>((myp.getX() + 1.0f) * TFT_WIDTH_HALF));
  // lua_setfield(L, -2, "x");

  // lua_pushinteger(L, TFT_HEIGHT-static_cast<int16_t>((myp.getY() + 1.0f) * TFT_HEIGHT_HALF));
  // lua_setfield(L, -2, "y");

  // // テーブルをスタックにプッシュ
  // return 1;



  // if(degmode == 0)
  // {
  //   lua_pushinteger(L, static_cast<int16_t>((myp.getX() + 1.0f) * TFT_WIDTH_HALF));
  // }else if(degmode == 1){
  //   lua_pushinteger(L, TFT_HEIGHT-static_cast<int16_t>((myp.getY() + 1.0f) * TFT_HEIGHT_HALF));
  // }

  if(degmode == 0)
  {
    lua_pushinteger(L, static_cast<int16_t>((myp.getX() + 1.0f) * TFT_WIDTH_HALF));
  }else if(degmode == 1){
    lua_pushinteger(L, TFT_HEIGHT-static_cast<int16_t>((myp.getY() + 1.0f) * TFT_HEIGHT_HALF));
  }

  return 1;
}


int numCorners = 24;
int numRings = numCorners / 2 + 1;
int numVertices = numCorners * numRings;
float deg = 0;
float cdeg = 360.0f/ float(numCorners);
// Vector3<float> opg[numVertices];
// Vector3<float> pg[numVertices];
// std::vector<Vector3<float>> opg;
// std::vector<Vector3<float>> pg;
// std::vector<Vector3<float>> op;
// std::vector<Vector3<float>> p;

int RunHaco8Game::l_drawstars(lua_State* L) {
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int cam_angleX = lua_tointeger(L, 1);
  int cam_angleY = lua_tointeger(L, 2);
  int cam_angleZ = 0;
  float zoom = lua_tonumber(L, 3);
  camera.zoom = zoom;
  int drawmode = lua_tointeger(L, 4);

  if (cam_angleX > 360 || cam_angleX <= 0 ) {
      // deg1 = fmod(deg1 , 360);
      cam_angleX%=360;
  }

  if (cam_angleY > 360 || cam_angleY < 0 ) {
      // deg2 = fmod(deg2 , 360);
      cam_angleY%=360;
  }

  if (cam_angleZ > 360 || cam_angleZ < 0 ) {
      cam_angleZ%=360;
  }

  // 与えられた角度をラジアンに変換
  float angleX = cam_angleX * RAD_ONE;
  float angleY = cam_angleY * RAD_ONE;

  // 各軸周りの回転行列を計算
  float cosX = cos(angleX);
  float sinX = sin(angleX);
  float cosY = cos(angleY);
  float sinY = sin(angleY);

    // int deg1 = lua_tointeger(L, 1);
    // int deg2 = lua_tointeger(L, 2);
    // int sw = lua_tointeger(L, 3);
    // float zoom = lua_tonumber(L, 4);
    // camera.zoom = zoom;

    // langle += 3;
    // langle %= 360;
    // // ライトのxyzは位置ではなくベクトル成分。多分。
    // light.x = gsin(langle);//cos(langle*RAD_ONE);
    // light.y = 1;//＋が上のよう
    // light.z = gcos(langle);//手前奥

    if(drawmode == 1||drawmode == 2){
  
    //グリッド線を引く
    for(int i =0;i < numVertices; i++)
    {

      float x = opg[i].getX();
      float y = opg[i].getY();
      float z = opg[i].getZ();

      
      // y軸周りの回転
      float tempX1 = x * cosY - z * sinY;
      float tempZ1 = x * sinY + z * cosY;

      // x軸周りの回転
      pg[i].setX(tempX1);
      pg[i].setY(y * cosX + tempZ1 * sinX);
      pg[i].setZ(-y * sinX + tempZ1 * cosX);

      //world座標変換
      // if(sw == 0){
      //   pg[i].setX(cosX * opg[i].getX() + -sinX * opg[i].getY());
      //   pg[i].setY(sinX * opg[i].getX() +  cosX * opg[i].getY());
      //   pg[i].setZ(opg[i].getZ());
      // }
      // else if(sw == 1){
      //   pg[i].setX(cosX * opg[i].getX() + -sinX * opg[i].getZ());
      //   pg[i].setY(opg[i].getY());
      //   pg[i].setZ(sinX * opg[i].getX() +  cosX * opg[i].getZ());
      // }
      // else if(sw == 2){
      //   pg[i].setX(opg[i].getX());
      //   pg[i].setY(cosY * opg[i].getY() + -sinY * opg[i].getZ());
      //   pg[i].setZ(sinY * opg[i].getY() +  cosY * opg[i].getZ());
      // }

      // カメラの移動
      pg[i].setZ(-pg[i].getZ() - 1);//カメラの位置0だと表示されない
      pg[i].setZ(-pg[i].getZ());//記号を反転させる
      pg[i].setX(pg[i].getX() / pg[i].getZ()/camera.zoom);
      pg[i].setY(pg[i].getY() / pg[i].getZ()/camera.zoom);
      pg[i].setZ(pg[i].getZ() / 5);
    }
    

    
    for(int i =0;i < numVertices; i++)
    {

    //   // self->triangle3D(pg[0], pg[1], pg[2]);
    //   // self->triangle3D(pg[0], pg[2], pg[3]);
    //   // self->triangle3D(pg[0], pg[2], pg[3]);

    //   //線を書く

    //   // self->line3D(p[i], pg[j], TFT_CYAN);
    //   // if(i < numVertices - numCorners){
    //   //   j = i + numCorners;
    //   //   self->line3D(pg[i], pg[j], TFT_BLUE);

    //   //self->line3D(pg[i], pg[j], TFT_CYAN);

      int j = i + 1;
    //   if(j % numCorners == 0) j -= numCorners;//一周したらリセット

      int k = i + numCorners;
      int l = j + numCorners;


      // k-l
      // |\|
      // i-j

      // ポリゴン生成
      // if(i < numVertices - numCorners){
      //   self->triangle3D(pg[i], pg[j], pg[k]);
      //   self->triangle3D(pg[j], pg[l], pg[k]);
      // }

      if( i < numVertices - numCorners){//赤経をひく
        if( 0 == i % numCorners){//0度
          self->line3D(pg[i], pg[k], HACO3_C8);
        }
        else{
          self->line3D(pg[i], pg[k], HACO3_C1);
        }
      }

      if( i < numVertices - 1){//赤緯をひく
        if( numCorners-1 == i % numCorners){
          j = i-(numCorners-1);
        }

        if( i >= numCorners*(numRings/2) && i < numCorners*(numRings/2+1) ){
        // if(i <= numCorners-1){
          self->line3D(pg[i], pg[j], HACO3_C8);//0度
        }else{
          self->line3D(pg[i], pg[j], HACO3_C1);
        }
      }
      
    //   //点を書く

    //   // if( 3 >= i % numCorners){
    //   //   if( i <= numCorners*4){
    //     // self->point3D(pg[i],TFT_WHITE);

    //     // self->fastPoint(pg[i],TFT_WHITE);
    //   //   }
    //   // }
    }

    }

    if(drawmode == 0||drawmode == 1){

    // 座標変換
  for (int i = 0; i < BSTARBUFNUM; i++) {
      float x = op[i].getX();
      float y = op[i].getY();
      float z = op[i].getZ();

      // y軸周りの回転
      float tempX1 = x * cosY - z * sinY;
      float tempZ1 = x * sinY + z * cosY;

      // x軸周りの回転
      p[i].setX(tempX1);
      p[i].setY(y * cosX + tempZ1 * sinX);
      p[i].setZ(-y * sinX + tempZ1 * cosX);

      //world座標変換
      // if(sw == 0){
      //   p[i].setX(cosX * op[i].getX() + -sinX * op[i].getY());
      //   p[i].setY(sinX * op[i].getX() +  cosX * op[i].getY());
      //   p[i].setZ(op[i].getZ());
      // }
      // else if(sw == 1){
      //   p[i].setX(cosX * op[i].getX() + -sinX * op[i].getZ());
      //   p[i].setY(op[i].getY());
      //   p[i].setZ(sinX * op[i].getX() +  cosX * op[i].getZ());
      // }
      // else if(sw == 2){
      //   p[i].setX(op[i].getX());
      //   p[i].setY(cosY * op[i].getY() + -sinY * op[i].getZ());
      //   p[i].setZ(sinY * op[i].getY() +  cosY * op[i].getZ());
      // }

      // カメラの移動
      p[i].setZ(-p[i].getZ() - 1);//カメラの位置0だと表示されない
      p[i].setZ(-p[i].getZ());//記号を反転させる
      p[i].setX(p[i].getX() / p[i].getZ()/camera.zoom);
      p[i].setY(p[i].getY() / p[i].getZ()/camera.zoom);
      p[i].setZ(p[i].getZ() / 5);
  }

//星を描画
    for(int i =0;i < BSTARBUFNUM; i++)
    {

      int starbrightness = bsParamInt8t[i][0];
      int noInt = bsParamInt16t[i][0];
      self->fastPoint(p[i], starbrightness, noInt);
                  
    }
    }


    return 0;
}



int RunHaco8Game::l_creobj(lua_State* L) {

    RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));

    //グリッド生成
    // for (int i = 0; i < numRings*numCorners; i++) //=numVertices
    // {   //ベクター配列を初期化
    //     Vector3<float> initp;
    //     opg.push_back(initp);
    //     pg.push_back(initp);

    // }

    //オブジェクトを生成
    for (int j = 0; j < numRings; j++) 
    {
      float r = sin(cdeg * j * RAD_ONE);
      float y = cos(cdeg * j * RAD_ONE);
      for(int i=0;i<numCorners; i++){

        int k = j * numCorners + i;

        //ベクター配列を初期化
        Vector3<float> initp;
        opg.push_back(initp);
        pg.push_back(initp);

        opg[k].setX(sin(i*cdeg*RAD_ONE) * r);
        opg[k].setZ(cos(i*cdeg*RAD_ONE) * r);
        opg[k].setY(y);
      }
    }

    for(int i=0; i<BSTARBUFNUM; i++){//星の数だけ

      Vector3<float> initp;
      op.push_back(initp);
      p.push_back(initp);
      
      //星の３次元座標を求めてオリジナルポジションに入れる
      op[i].setX(cos(bsParamFloat[i][1]) * cos(bsParamFloat[i][0]));
      op[i].setZ(cos(bsParamFloat[i][1]) * sin(bsParamFloat[i][0]));
      op[i].setY(sin(bsParamFloat[i][1]));

    }

    // for(int i=0; i<numVertices; i++){//グリッドの数だけ

    //   Vector3<float> initp;
    //   opg.push_back(initp);
    //   pg.push_back(initp);
      
    //   //星の３次元座標を求めてオリジナルポジションに入れる
    //   opg[i].setX(cos(bsParamFloat[i][1]) * cos(bsParamFloat[i][0]));
    //   opg[i].setZ(cos(bsParamFloat[i][1]) * sin(bsParamFloat[i][0]));
    //   opg[i].setY(sin(bsParamFloat[i][1]));

    // }

    return 0; // テーブルをスタックに返す
}

int RunHaco8Game::l_trans(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  float x = lua_tonumber(L, 1);
  float y = lua_tonumber(L, 2);
  float z = lua_tonumber(L, 3);
  float angle = lua_tonumber(L, 4);
  float size = lua_tonumber(L, 5);
  int objid = lua_tonumber(L, 6);//あとでオブジェクトを配列にするときに使うplyObj[2]のように
  int colangle = lua_tointeger(L, 7);
  plyObj.x = x;
  plyObj.y = y;
  plyObj.z = z;
  plyObj.angle = angle;
  plyObj.size = size;
  plyObj.colangle = colangle;
  plyObj.width = TFT_WIDTH;
  plyObj.height = TFT_HEIGHT;
  return 0;
}


int RunHaco8Game::l_lig(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  float x = lua_tonumber(L, 1);
  float y = lua_tonumber(L, 2);
  float z = lua_tonumber(L, 3);
  light.x = x;
  light.y = y;
  light.z = z;
  return 0;
}


int RunHaco8Game::l_cam(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  float x = lua_tonumber(L, 1);
  float y = lua_tonumber(L, 2);
  float z = lua_tonumber(L, 3);
  float x2 = lua_tonumber(L, 4);
  float y2 = lua_tonumber(L, 5);
  float z2 = lua_tonumber(L, 6);
  // float anglex = lua_tonumber(L, 7);
  // float zoom = lua_tonumber(L, 8);
  float anglex = lua_tonumber(L, 7);
  float angley = lua_tonumber(L, 8);
  float anglez = lua_tonumber(L, 9);
  float zoom = lua_tonumber(L, 10);
  camera.x = x;
  camera.y = y;
  camera.z = z;

  // camera.x2 = x2;//視線の方向ベクトル
  // camera.y2 = y2;
  // camera.z2 = z2;
  camera.anglex = anglex;
  camera.angley = angley;
  camera.anglez = anglez;
  camera.zoom = zoom;
  return 0;
}

int RunHaco8Game::l_wini(lua_State* L) {
  // const int ARRAY_SIZE = 32;  // 配列の要素数
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));

  int divnum = lua_tointeger(L, 1);

  //Intersection構造体の配列を作成
  std::array<Intersection, 32> intersections;
  // std::vector<Intersection> intersections(divnum);

    //配列の各要素を初期化
  for (int rayno = 0; rayno < divnum; ++rayno) {
      intersections[rayno].x = 999.0;
      intersections[rayno].y = 999.0;
      intersections[rayno].distance = 999.0;
      intersections[rayno].mapheight = 0.0;
  }

  return 0;
}

int RunHaco8Game::l_wset(lua_State* L) {
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));

  // Get the coordinates of the first line segment
  int pangle = lua_tointeger(L, 1);
  
  float x1 = lua_tonumber(L, 2);
  float y1 = lua_tonumber(L, 3);

  // wall line points
  float x3 = lua_tonumber(L, 4);
  float y3 = lua_tonumber(L, 5);
  float x4 = lua_tonumber(L, 6);
  float y4 = lua_tonumber(L, 7);
  int colangle = lua_tointeger(L, 8);
  
  int raylength = 64;
  int angleStep = 2;
  int centerno = 16;

  // int closestDistance=999;

  for(int rayno = 0;rayno<32; rayno++){

    float distance  = 999;
    
    int rayangle = (pangle + angleStep*(rayno-centerno) + 360) % 360;

    float sinVal = sin(rayangle * RAD_ONE);
    float cosVal = -sin(((rayangle + 90 + 360) % 360) * RAD_ONE);


    float x2 = x1 + raylength * cosVal; // レイの終点のx座標
    float y2 = y1 + raylength * sinVal; // レイの終点のy座標

    tft.drawLine(x1, y1, x2, y2, lua_rgb24to16(127, 127, 0));//光線を描画
    float denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    

    if (denominator == 0) {
      // 線分は平行なので、nilを返す
      
      // lua_pushnil(L);
      // return 1;
    } else {
      
      float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denominator;
      float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denominator;

      if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
        // 線分は交差しているので、交点座標を計算する
        float intersectionX = x1 + t * (x2 - x1);
        float intersectionY = y1 + t * (y2 - y1);
        distance = sqrt(pow((intersectionX - x1), 2) + pow((intersectionY - y1), 2));

        if(intersections[rayno].distance>distance){
          intersections[rayno].distance = distance;
          intersections[rayno].x = intersectionX;
          intersections[rayno].y = intersectionY;
          intersections[rayno].colangle = colangle;
        }

        tft.fillCircle(intersections[rayno].x, intersections[rayno].y, 3, lua_rgb24to16(255, 0, 0));

      } else {
        // 線分は交差していないので、nilを返す
        // lua_pushnil(L);
      }
    }
  }
  return 1;
}

int RunHaco8Game::l_wdraw(lua_State* L)
{
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int pangle = lua_tointeger(L, 1);

  int raylength = 64;
  int angleStep = 2;
  int centerno = 16;
  int centerangle = 32;

  for(int rayno = 0; rayno < 32; rayno++)
  {
    if(intersections[rayno].distance != 999)
    {
      int brightness = 255 - (intersections[rayno].distance / raylength) * 255;

      // HSBからRGBに変換
      int r, g, b;
      self->hsbToRgb(intersections[rayno].colangle, 127, brightness, r, g, b); //彩度は自動で決まるのでダミー

      // RGB値を設定
      self->col[0] = r; // Red
      self->col[1] = g; // Green
      self->col[2] = b; //

      int angleDiff = (2 * (rayno - centerno) + 360) % 360;
      int perpdist = intersections[rayno].distance * cos(angleDiff*RAD_ONE);
      int mapheight = ( 600.0 / perpdist );

      if(mapheight > 64.0){
        mapheight = 64.0;
      }

      int cing = 32 - mapheight / 2;

      tft.fillRect(TFT_WIDTH - rayno * 2, cing, 2, mapheight, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
      tft.fillCircle(intersections[rayno].x, intersections[rayno].y, 3, lua_rgb24to16(0, 255, 0));
    }

    // Reset intersection values
    intersections[rayno].x = 999;
    intersections[rayno].y = 999;
    intersections[rayno].distance = 999;
    intersections[rayno].mapheight = 0;
    intersections[rayno].colangle = 0;
  }

  return 0;
}

int RunHaco8Game::l_gcol(lua_State* L)
{
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);

  // 分解してRGBA値を取得
  uint16_t c = tft.readPixel(x, y);
  lua_pushinteger(L, c);

  return 1;  // スタックにプッシュした値の数を返す
}

int RunHaco8Game::l_sp3d(lua_State* L)
{
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  float x = lua_tonumber(L, 1);
  float y = lua_tonumber(L, 2);
  float w = lua_tonumber(L, 3);
  float h = lua_tonumber(L, 4);
  float x1 = lua_tonumber(L, 5);
  float y1 = lua_tonumber(L, 6);
  float angle = lua_tonumber(L, 7);
  float pheight = lua_tonumber(L, 8);
  pheight = pheight / 100;

  int bottom = y + h;

  float pcos = cos(angle*RAD_ONE);
  float psin = sin(angle*RAD_ONE);

  for (int wx = 0; wx < w; wx++) {
    for (int wy = 0; wy < h; wy++) {
        float hRatio = wx / (w - 1);
        float vRatio = pheight / (1 - (wy / (h - 1)));

        float sX = (1 - hRatio) * vRatio * w;
        float sY = hRatio * vRatio * h;

        float transformedX = sX * pcos - sY * psin + x1;
        float transformedY = sX * psin + sY * pcos + y1;

        uint16_t c = tft.readPixel(floor(transformedX), floor(transformedY));
        tft.drawPixel(x + wx, bottom - wy, c);
    }
  }
  return 0;
}

int RunHaco8Game::l_spmap(lua_State* L) {
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int mx = lua_tointeger(L, 1);
  int my = lua_tointeger(L, 2);
  float w = lua_tonumber(L, 3);
  float h = lua_tonumber(L, 4);
  int roix = lua_tointeger(L, 5);
  int roiy = lua_tointeger(L, 6);
  int roiW = lua_tointeger(L, 7);
  int roiH = lua_tointeger(L, 8);

  float x1 = lua_tonumber(L, 9);
  float y1 = lua_tonumber(L, 10);
  float angle = lua_tonumber(L, 11);
  float pheight = lua_tonumber(L, 12);
  pheight = pheight / 100;

  String fn = lua_tostring(L, 13);
  if (fn != NULL) {
    mapFileName = fn;
  }

//１ピクセルずつ走査して描画していく

  // int bottom = my + h;

  float pcos = cos(angle*RAD_ONE);
  float psin = sin(angle*RAD_ONE);

  float invW = 1.0 / (w - 1);
  float invH = 1.0 / (h - 1);

  for (int wx = 0; wx < w; wx++) {
    float hRatio = wx * invW;

    for (int wy = 0; wy < h; wy++) {
      float vRatio = pheight / (1 - (wy * invH));

      float sX = (1 - hRatio) * vRatio * w;
      float sY = hRatio * vRatio * h;

      float transformedX = sX * pcos - sY * psin + x1;
      float transformedY = sX * psin + sY * pcos + y1;

      if (transformedX < ((roix + roiW) << 3) && transformedY < ((roiy + roiH) << 3) && transformedX >= (roix << 3) && transformedY >= (roiy << 3)) {
        // uint16_t c = getmapcolor(static_cast<int>(transformedX), static_cast<int>(transformedY));
        int xx = static_cast<int>(transformedX);
        int yy = static_cast<int>(transformedY);

        int sprX = xx / 8;  // floorしている
        int sprY = yy / 8;  // floorしている

        // マップ配列から指定位置のスプライトを抽出する
        int sprno = mapArray[sprX][sprY];

        int sx = (sprno - 1) & 7;  // (sprno - 1) % spr8numX と同等
        int sy = (sprno - 1) >> 3; // (sprno - 1) / spr8numY と同等

        // スプライト内の座標から色を取得する
        // uint16_t color = sprite64.readPixel((sx << 3) + (xx & 7), (sy << 3) + (yy & 7));

        // tft.drawPixel(mx + wx, my + h - wy, color);
      }
    }
  }
  return 0;
}

int RunHaco8Game::l_getstl(lua_State* L)
{
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  const char* filename = lua_tostring(L, 1);

  // SPIFFSからファイルを開く
  File file = SPIFFS.open(filename, "r");
  if (!file) {
    return luaL_error(L, "failed to open file: %s", filename);
  }

  // 読み込み位置をファイルの先頭に設定
  file.seek(0);

  // ファイルからSTLヘッダーを読み込む
  char header[80];
  file.read((uint8_t*)header, 80);

  // ファイルから三角形の数を読み込む
  char poly_count_buf[4];
  file.read((uint8_t*)poly_count_buf, 4);
  unsigned int poly_count = *reinterpret_cast<unsigned int*>(poly_count_buf);
  const unsigned int vertices_count = poly_count * 3;//ポリゴンの頂点数

lua_newtable(L);  // テーブルを新しく作成

for (unsigned int i = 0; i < vertices_count; i += 3) {
  // ダミーデータを設定する
  float dummyData[3] = {1.0f+i, 2.0f+i, 3.0f+i};

  lua_newtable(L);  // テーブルを新しく作成

  for (int j = 0; j < 3; ++j) {
    lua_pushnumber(L, static_cast<lua_Number>(dummyData[j]));
    lua_rawseti(L, -2, j + 1);
  }

  lua_rawseti(L, -2, i / 3 + 1);  // テーブルへの追加位置を修正
}

// テーブルのサイズをスタックに積む
lua_pushinteger(L, static_cast<lua_Integer>(poly_count));

  return 2;
}

