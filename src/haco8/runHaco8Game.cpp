#include "runHaco8Game.h"
extern MyTFT_eSprite tft;
extern LGFX_Sprite sprite64;
// extern LGFX_Sprite sprite256[BUF_PNG_NUM];
extern LGFX_Sprite sprite88_roi;
extern LGFX_Sprite buffSprite;

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
extern float soundSpeed;
// extern bool toneflag;
extern void readMap();
extern uint64_t frame;
extern float radone;
extern int xtile;
extern int ytile;
extern double ztile;
extern int xtileNo;
extern int ytileNo;
extern bool downloadF;
extern LGFX_Sprite sprref;

// extern String newKeys[BUF_PNG_NUM];
extern String oldKeys[BUF_PNG_NUM];

extern Vector3<double> currentgpos;

extern Vector3<double> prePos;
extern Vector3<double> currentPos;
extern Vector3<double> diffPos;
extern int dirNos[9];

#include "MapDictionary.h"//直前で読み込まないとexternできない
extern MapDictionary dict;


// extern std::map<String, LGFX_Sprite*> key2ptr;

extern uint8_t mapArray[MAPWH][MAPWH];

extern int8_t sprbits[128];//fgetでアクセスするスプライト属性を格納するための配列

extern int getDirNo(double _x, double _y);
extern Vector2<int> getSign(int dirno);
extern Vector2<int> getKey2Sign(String _currentKey, String _targetKey);
extern LGFX_Sprite mapTileSprites[9];

extern SemaphoreHandle_t xSemaphore;
// extern SemaphoreHandle_t taskSemaphore;
extern void downloadTask(void *pvParameters);
extern void taskCompleteCallback();
extern void setAllKeys(String _key);

String oldKey = "";

Intersection intersections[32];

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
  lua_pushcclosure(L, l_rendr, 1);
  lua_setglobal(L, "rendr");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_creobj, 1);
  lua_setglobal(L, "creobj");

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
            sprite64.pushSprite(&sprite88_roi, -8*(sx), -8*(sy));//128*128のpngデータを指定位置までずらすsprite64のスプライトデータ8*8で切り抜く
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
extern double tileZoom;
extern int currentDirNo;
extern float bairitu;
bool collF=true;
int mx = 62;//64;//128;//ダウンロードを先読みする距離
int my = 58;//60;//120;//ダウンロードを先読みする距離
int ringBuffNo = 0;

Vector2<int> buffPos;
Vector2<float> buffPos2;

extern void taskCompleteCallback();
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

void getTilePos(double _lat, double _lon, int zoom_level)
{
  // 経度を0から2にスケーリングし、255倍してタイル座標に変換
  double n = pow(2, zoom_level);
  double lat_rad = _lat * (M_PI / 180);

  double x = (_lon + 180.0) / 360.0 * n;
  xtile = int(x * 256);

  // 緯度をスケーリングしてタイル座標に変換
  double y = (1.0 - log(tan(lat_rad) + (1 / cos(lat_rad))) / M_PI) / 2.0 * n;
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
  double _tileZoom = lua_tonumber(L, 3);

  double bairitu = 1 - (0.5 - ((_tileZoom - int(floor(_tileZoom))) / 2));
  double zoom_level = int(floor(_tileZoom));

  // 調整後の座標を逆変換して緯度経度に戻す
  double n = pow(2, zoom_level);
  double lat_rad = asin(tanh(M_PI * (2 * ((static_cast<double>(_posY - TFT_HEIGHT_HALF)/256.0 ) / bairitu) / n)));
  double lon = (static_cast<double>(_posX - TFT_WIDTH_HALF)/256.0 ) / bairitu / n * 360.0;
  double lat = lat_rad * 180.0 / M_PI;

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

int RunHaco8Game::l_getmappos(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  double _walkLatPos = lua_tonumber(L, 1);
  double _walkLonPos = lua_tonumber(L, 2);
  double _tileZoom = lua_tonumber(L, 3);

  double bairitu = 1 - (0.5 - ((_tileZoom - int(floor(_tileZoom))) / 2));
  double zoom_level = int(floor(_tileZoom));

  // 経度を0から2にスケーリングし、タイル座標に変換
  // double lon_rad = _walkLonPos * (M_PI / 180);
  double n = pow(2, zoom_level);
  double _x = (_walkLonPos + 180.0) / 360.0 * n;
  int _xtile = int(_x * 256);

  // 緯度をスケーリングしてタイル座標に変換
  double lat_rad = _walkLatPos * (M_PI / 180);
  double _y = (1.0 - log(tan(lat_rad) + 1 / cos(lat_rad)) / M_PI) / 2.0 * n;
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

int RunHaco8Game::l_drawmaps(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  double _walkLatPos = lua_tonumber(L, 1);
  double _walkLonPos = lua_tonumber(L, 2);
  double _tileZoom = lua_tonumber(L, 3);

  tileZoom = _tileZoom;
  currentgpos = {_walkLatPos, _walkLonPos, tileZoom};

  //tileZoom の少数点以降からbairituを計算する
  double bairitu = 1-(0.5-((tileZoom - int(floor(tileZoom)))/2));

  //経緯度からタイル座標を計算
  getTilePos(_walkLatPos, _walkLonPos, tileZoom);

  //現在のタイルマップのキーを保持
  currentKey = String(int(xtileNo)) + "/" + String(int(ytileNo));

  //現在位置の座標を計算して確定する（タイルマップの現在値をディスプレイの中央にずらすための座標）
  currentPos.set(xtile % 256, ytile % 256, 0.0);

  //進行方向をベクトルで取得
  diffPos = currentPos.diff(prePos);//変位

  //現在の進行方向を方向ベクトルから方向番号に変換して保持
  int currentDirNo = dict.getTargetDirNo(diffPos);//進行方向番号を取得

  // dict.setBuffIDNos(currentDirNo);//8方向すべてのBuffIDNoをセットする(以降変わらない)
  dict.setDirNos(currentDirNo);//8方向すべてのdirNoをセットする
  Vector2<int> currentTilePos = dict.getTilePos(currentDirNo);
  
  //リングバッファに対する処理、実際にダウンロードする枚数を抽出する
  for (int buffCounter = 0; buffCounter < downloadLimitNum; buffCounter++) 
  {
    //方向番号を取得
    int targetDirNo = dict.getDirNo(buffCounter);
    Vector2<int> targetTilePos = dict.getTilePos(targetDirNo);
    targetKey = String(int(xtileNo + targetTilePos.getX())) + "/" + String(int(ytileNo + targetTilePos.getY()));
    //------------------------------------

    allKeys.push_back(targetKey);
    //主人公の位置が真ん中に来るようにバッファスプライトの位置を動かす
    buffPos.set(TFT_WIDTH_HALF  + ((targetTilePos.getX()*256)-currentPos.x) * bairitu,
                TFT_HEIGHT_HALF + ((targetTilePos.getY()*256)-currentPos.y) * bairitu);
    
    //バッファと関心領域との当たり判定を計算
    Rect2D cR(TFT_WIDTH_HALF -((64+mx)-(currentTilePos.getX()*mx))*bairitu,
            TFT_HEIGHT_HALF-((60+my)-(currentTilePos.getY()*my))*bairitu,
            (128+mx*2)*bairitu,
            (120+my*2)*bairitu);
    // Rect2D cR(TFT_WIDTH_HALF-((64+mx/2)-targetTilePos.getX()*mx)*bairitu,TFT_HEIGHT_HALF-((60+my/2)-targetTilePos.getY()*my)*bairitu,(128+mx)*bairitu,(120+my)*bairitu);//ダウンロードのタイミングを作るための当たり判定用の矩形
    Rect2D tR(buffPos.getX(),buffPos.getY(),256*bairitu,256*bairitu);

    collF=false;//初期化

    if (cR.x < tR.x + tR.w &&
        cR.x + cR.w > tR.x &&
        cR.y < tR.y + tR.h &&
        cR.y + cR.h > tR.y) 
        {
          collF = true;// 当たり判定あり
        } 

    if(collF){//当たったらダウンロード対象にする//preAllKeysをチェックしてなければ追加する。
      setDownloadKeys(targetKey);
      // dict.setCollF(buffCounter, true);
      dict.setKey2CollF(targetKey, true);
      if (!existpreDownloadKey(targetKey)){//実際にダウンロードするキーのみを抽出
        rundownloadKeys.push_back(targetKey);//実際にダウンロードするキーを登録,要素数が増える
      }
    }else{
      dict.setKey2CollF(targetKey, false);
    }
  }
  //現在地の地図があるかチェックなければ必ずダウンロードリストに入れる
  if(!dict.existKey(currentKey)){
    rundownloadKeys.push_back(currentKey);
  }

  //ダウンロード枚数だけループしタグをつける
  
  // readno順で、読み込み順はソートされている
  for (const auto& rundownloadkey : rundownloadKeys) {//登録されたキーの要素数だけ繰り返す
  // Serial.print(rundownloadkey);
  // Serial.print("][");
    String filePath  = "/tokyo/"+String(int(floor(tileZoom)))+"/"+ rundownloadkey +".png";
    // String filePath  = "/tokyo/"+_tileZoom+"/"+ dict.getNewKey(shouldNo) +".png";
    if (SD.exists(filePath)) {
      // ダウンロード処理
      int tlimit = 0;
      //クロックを上げすぎているためか、描画完了のtrue信号に取りこぼしがあるよう。
      //一度脱調すると、もう取れなくなってしまうので、取りこぼしても一定の時間たったら処理をやめるようにした。
      while(!mapTileSprites[ringBuffNo].drawPngFile(SD, filePath, 0, 0)||tlimit>=500){//描画が終わったらFalseダウンロードし終わったらtrue？(勘）
        tlimit++;
      }

      dict.setNewKey(ringBuffNo, rundownloadkey);//先にキーを入れておかないとgetkey2ReadNoができない 
      dict.setSprptr(ringBuffNo, &mapTileSprites[ringBuffNo]);

      // Serial.println("書き終わった");
      // // ringBuffNo++; ringBuffNo = ringBuffNo%9;

      // Serial.print("-------------------------------------------------------------------------------------------------------");
      // Serial.println(ringBuffNo);
      ringBuffNo++; 
      ringBuffNo = ringBuffNo%9;
      // shouldNo = (shouldNo+1) % downloadLimitNum;
    }else{
      Serial.print("ファイルパスがない？");
      Serial.println(filePath);
    }
  }

  //描画処理
  tft.fillScreen(TFT_BLACK);
    for (const auto& targetKey2 : allKeys) {

        //バッファの位置を更新し、再配置するための処理（currentKeyからの相対方向を更新）
        Vector2<int> result1 = calculateVectorDifference( currentKey, targetKey2 );
        // dict.setBuffPos(readBuffNo, result1);
        dict.setKey2BuffPos(targetKey2, result1);

        buffPos2.set(TFT_WIDTH_HALF + (dict.getkey2BuffPos(targetKey2).getX()*256-currentPos.x) * bairitu,
                    TFT_HEIGHT_HALF + (dict.getkey2BuffPos(targetKey2).getY()*256-currentPos.y) * bairitu);

        float matrix_map[6] = {
            bairitu,   //0 横2倍
            -0.0,      //1 横傾き
            buffPos2.getX(),      //2 X座標
            0.0,       //3 縦傾き
            bairitu,   //4 縦2倍
            buffPos2.getY()      //5 Y座標
        };
        
        if(dict.existKey(targetKey2)){
          if(dict.getKey2CollF(targetKey2)){
            dict.getKey2sprptr(targetKey2)->pushAffine(&tft, matrix_map);
            dict.getKey2sprptr(targetKey2)->drawRect(1, 1, 254, 254, TFT_RED);

            // Serial.print(currentPos.getX());
            // Serial.print(":");
            // Serial.println(currentPos.getX());
          }
        }

        

    // // mapTileSprites[i].fillRect(20*readBuffNo, 20*readBuffNo, 20,20, TFT_BLUE);
    // tft.setCursor(dict.getBuffPos(readBuffNo).getX(), dict.getBuffPos(readBuffNo).getY()+ (floor(readBuffNo/3)*8));
    // tft.print(String(readBuffNo));
    // tft.print(String(targetKey2));

    //過去から現在の進行方向ベクトルの線を引く
    tft.drawLine( TFT_WIDTH_HALF,
                  TFT_HEIGHT_HALF,
                  diffPos.x + TFT_WIDTH_HALF, 
                  diffPos.y + TFT_HEIGHT_HALF, TFT_BLUE);
    tft.setColor(TFT_RED);
    tft.drawRect(TFT_WIDTH_HALF-64*bairitu, TFT_HEIGHT_HALF-60*bairitu, 128*bairitu, 120*bairitu);

    // if(collF){
    //   tft.drawLine(cR.x, cR.y,tR.x, tR.y,TFT_BLUE);
    //   tft.drawRect(tR.x, tR.y, 256*bairitu, 256*bairitu,TFT_BLUE);
    // }

    // xSemaphoreGive(xSemaphore); // セマフォを解放

    if(abs(currentPos.x-prePos.x)>128||abs(currentPos.y-prePos.y)>128){
      // Serial.print("またぎました");
      // break; 
    }

    // Serial.print("[");
    // // Serial.print(dict.getNewKey(readBuffNo));
    // Serial.print(dict.getBuffPos(readBuffNo).getX());
    // Serial.print(":");
    // Serial.print(dict.getBuffPos(readBuffNo).getY());
    // Serial.print("] ");

    // readBuffNo++;

  }

  // Serial.println("");

  //ダウンロードのタイミングを作るための当たり判定用の矩形

  // Rect2D vR(TFT_WIDTH_HALF -(64-(mx))*bairitu,
  //           TFT_HEIGHT_HALF-(60-(my))*bairitu,
  //           (128+mx*2)*bairitu,
  //           (120+my*2)*bairitu);
  Rect2D vR(TFT_WIDTH_HALF -((64+mx)-(currentTilePos.getX()*mx))*bairitu,
            TFT_HEIGHT_HALF-((60+my)-(currentTilePos.getY()*my))*bairitu,
            (128+mx*2)*bairitu,
            (120+my*2)*bairitu);
  tft.drawRect(vR.x, vR.y, vR.w, vR.h,TFT_BLUE);

  preallKeys = allKeys;
  predownloadKeys = downloadKeys;
  
  // Serial.println("");

  allKeys.clear();
  downloadKeys.clear();
  rundownloadKeys.clear();

  prePos = currentPos;
  previousKeys = temporaryKeys;

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

int RunHaco8Game::l_distance(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  double x1 = lua_tonumber(L, 1);
  double y1 = lua_tonumber(L, 2);
  double x2 = lua_tonumber(L, 3);
  double y2 = lua_tonumber(L, 4);

  double dx = x2 - x1;
  double dy = y2 - y1;
  lua_pushnumber(L, sqrt(dx*dx + dy*dy));
  return 1;
} 

int normalizeAngle(int angle) {
    angle = angle % 360; // 360で割った余りを求める
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

// double gsin(int angle)
// {
//     angle = static_cast<int>(normalizeAngle(angle));

//     if (angle >= 0 && angle <= 89) {
//         return sinValues[angle];
//     } 
//     else if (angle >= 90 && angle <= 179) {
//         return sinValues[180 - angle];
//     } 
//     else if (angle >= 180 && angle <= 269) {
//         return -sinValues[angle - 180];
//     } 
//     else {
//         return -sinValues[360 - angle];
//     }
// }

// double gcos(int angle) {
//     angle = static_cast<int>(normalizeAngle(angle));
//     // angle = -(angle + 90+360) % 360;

//     angle = (angle % 360 + 360 - 90) % 360;

//     if (angle >= 0 && angle <= 89) {
//         return sinValues[angle];
//     } 
//     else if (angle >= 90 && angle <= 179) {
//         return sinValues[180 - angle];
//     } 
//     else if (angle >= 180 && angle <= 269) {
//         return -sinValues[angle - 180];
//     } 
//     else {
//         return -sinValues[360 - angle];
//     }
// }


int RunHaco8Game::l_sin(lua_State* L) {
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  double x = lua_tonumber(L, 1);
  lua_pushnumber(L, sin(x));
  return 1;
}

int RunHaco8Game::l_cos(lua_State* L) {
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

int RunHaco8Game::l_srnd(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int s = lua_tointeger(L, 1);
  srand(s);
  // lua_pushinteger(L, 0);
  return 0;
}

int RunHaco8Game::l_sgn(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  double x = lua_tonumber(L, 1);
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

  tft.fillRect(0, 0, 128, 128, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));

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
    double x;
    double y;
    double z;
};
struct Point2D {
    int x;
    int y;

    Point2D(int _x, int _y) : x(_x), y(_y) {}
};



static RunHaco8Game::CameraObj camera;
static RunHaco8Game::LightObj light;
static RunHaco8Game::CubeObj cube;

 
std::string convertToDecimalNotation(double value) {
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
double RunHaco8Game::calculateLength(double x, double y, double z) {
  return std::sqrt(x * x + y * y + z * z);
}

// ベクトルの正規化を行う関数
Vector3<double> RunHaco8Game::normalize(double x, double y, double z) {
  double length = calculateLength(x, y, z);
  return {x / length, y / length, z / length};
}

// 2つのベクトルの内積を計算する関数
double RunHaco8Game::calculateDotProduct(const Vector3<double>& v1, const Vector3<double>& v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// 3つの頂点から法線ベクトルを計算する関数
Vector3<double> RunHaco8Game::calculateNormal(const Vector3<double>& v1, const Vector3<double>& v2, const Vector3<double>& v3) {
  double mx = v2.x - v1.x;
  double my = v2.y - v1.y;
  double uz = v2.z - v1.z;
  double vx = v3.x - v1.x;
  double vy = v3.y - v1.y;
  double vz = v3.z - v1.z;

  double nx = my * vz - uz * vy;
  double ny = uz * vx - mx * vz;
  double nz = mx * vy - my * vx;

  return normalize(nx, ny, nz);
}

// ポリゴンの明るさを計算する関数
double RunHaco8Game::calculateBrightness(const Vector3<double>& v1, const Vector3<double>& v2, const Vector3<double>& v3, const LightObj& light) {
  // ポリゴンの法線ベクトルを計算
  Vector3<double> normal = calculateNormal(v1, v2, v3);
  // 光源ベクトルを計算
  Vector3<double> lightVector = normalize(light.x, light.y, light.z);
  // ポリゴンの法線ベクトルと光源ベクトルの角度を計算
  double dotProduct = calculateDotProduct(normal, lightVector);
  // 明るさを0~1の範囲に正規化
  double brightness = (dotProduct + 1) / 2;
  return brightness;
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

void RunHaco8Game::renderPolygon(const std::vector<std::vector<float>>& polygonData, int colangle) {

  // オブジェクトの回転行列を計算
    // float rotationAngle = static_cast<float>(cube.angle) * M_PI / 180.0f;
    float cosAngle = -sin(((cube.angle + 90) % 360)*radone);
    float sinAngle =  sin((cube.angle % 360)*radone);

    // ポリゴンの描画処理
    for (size_t i = 0; i < polygonData.size(); i += 3) {
            // 頂点座標を取得
    Vector3<double> v1(static_cast<double>(polygonData[i][0]), static_cast<double>(polygonData[i][1]), static_cast<double>(polygonData[i][2]));
    Vector3<double> v2(static_cast<double>(polygonData[i + 1][0]), static_cast<double>(polygonData[i + 1][1]), static_cast<double>(polygonData[i + 1][2]));
    Vector3<double> v3(static_cast<double>(polygonData[i + 2][0]), static_cast<double>(polygonData[i + 2][1]), static_cast<double>(polygonData[i + 2][2]));

    // オブジェクト座標系での座標変換
    float x1 = v1.x - cube.x;
    float y1 = v1.y - cube.y;
    float z1 = v1.z - cube.z;

    float x2 = v2.x - cube.x;
    float y2 = v2.y - cube.y;
    float z2 = v2.z - cube.z;

    float x3 = v3.x - cube.x;
    float y3 = v3.y - cube.y;
    float z3 = v3.z - cube.z;

    // オブジェクトの回転行列を適用
    float transformedX1 = x1 * cosAngle - z1 * sinAngle;
    float transformedZ1 = z1 * cosAngle + x1 * sinAngle;
    float transformedY1 = y1;

    float transformedX2 = x2 * cosAngle - z2 * sinAngle;
    float transformedZ2 = z2 * cosAngle + x2 * sinAngle;
    float transformedY2 = y2;

    float transformedX3 = x3 * cosAngle - z3 * sinAngle;
    float transformedZ3 = z3 * cosAngle + x3 * sinAngle;
    float transformedY3 = y3;

    // カメラ座標系での座標変換
    float sinCamera = sin((camera.angle % 360)*radone);
    float cosCamera = -sin(((camera.angle + 90) % 360)*radone);

    float transformedX1Camera = transformedX1 * cosCamera - transformedZ1 * sinCamera;
    float transformedZ1Camera = transformedZ1 * cosCamera + transformedX1 * sinCamera;
    float transformedY1Camera = transformedY1;

    float transformedX2Camera = transformedX2 * cosCamera - transformedZ2 * sinCamera;
    float transformedZ2Camera = transformedZ2 * cosCamera + transformedX2 * sinCamera;
    float transformedY2Camera = transformedY2;

    float transformedX3Camera = transformedX3 * cosCamera - transformedZ3 * sinCamera;
    float transformedZ3Camera = transformedZ3 * cosCamera + transformedX3 * sinCamera;
    float transformedY3Camera = transformedY3;

    // パースペクティブ補正とズーム適用
    float scale1 = 640.0f / (64.0f - transformedZ1Camera) * camera.zoom;
    float scale2 = 640.0f / (64.0f - transformedZ2Camera) * camera.zoom;
    float scale3 = 640.0f / (64.0f - transformedZ3Camera) * camera.zoom;

    float sx1 = transformedX1Camera * scale1 + 64.0f;
    float sy1 = transformedY1Camera * scale1 + 64.0f;

    float sx2 = transformedX2Camera * scale2 + 64.0f;
    float sy2 = transformedY2Camera * scale2 + 64.0f;

    float sx3 = transformedX3Camera * scale3 + 64.0f;
    float sy3 = transformedY3Camera * scale3 + 64.0f;

        // ポリゴンの法線ベクトルを計算
        Vector3<double> normal = calculateNormal(v1, v2, v3);

        // カメラの視線ベクトル
        Vector3<double> viewVector = normalize(camera.x, camera.y, camera.z);

        // ポリゴンの法線ベクトルと視線ベクトルの角度を計算
        float dotProduct = calculateDotProduct(normal, viewVector);
        float angle = degrees(std::acos(dotProduct));

        // ポリゴンの明るさを計算
        float brightness = static_cast<float>(calculateBrightness(v1, v2, v3, light));

        // 明るさに基づいてポリゴンの色を設定（0~255の範囲に収める）
        int color = static_cast<int>(brightness * 255.0f);
        color = clamp(color, 0, 255);

        // 角度が180度未満のポリゴンのみ描画
        if (angle < 180.0f) {
            // 陰面消去を実施
            Point2D p1(static_cast<int>(floor(sx1)), static_cast<int>(floor(sy1)));
            Point2D p2(static_cast<int>(floor(sx2)), static_cast<int>(floor(sy2)));
            Point2D p3(static_cast<int>(floor(sx3)), static_cast<int>(floor(sy3)));

            // ポリゴンの面積を計算
            float area = (p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y);

            // 面積が正の場合はポリゴンを描画
            if (area > 0.0f) {
                  // HSBからRGBに変換
              int r, g, b;
              hsbToRgb(cube.colangle, 127, color, r, g, b);

              // RGB値を設定
              col[0] = r; // Red
              col[1] = g; // Green
              col[2] = b; // Blue

              fillFastTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, lua_rgb24to16(col[0], col[1], col[2]));
              // fillFastTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color); // 0xFFFFは描画色（白）を表します
            }
        }
    }
}

// LuaのテーブルからC++のstd::vector<std::vector<float>>にデータを格納する関数
void getPolygonData(lua_State* L, std::vector<std::vector<float>>& polygonData) {
    luaL_checktype(L, -1, LUA_TTABLE); // 引数のテーブルをチェック

    lua_pushnil(L);  // テーブルの最初のキーを取得するためにnilをプッシュ
    while (lua_next(L, -2) != 0) {
        luaL_checktype(L, -1, LUA_TTABLE); // テーブル内の要素がテーブルであることをチェック

        std::vector<float> vertex;
        lua_pushnil(L); // サブテーブルの最初のキーを取得するためにnilをプッシュ
        while (lua_next(L, -2) != 0) {
            luaL_checktype(L, -1, LUA_TNUMBER); // サブテーブル内の要素が数値であることをチェック

            float value = static_cast<float>(lua_tonumber(L, -1));
            vertex.push_back(value);

            lua_pop(L, 1); // サブテーブル内の値をポップ
        }
        polygonData.push_back(vertex);

        lua_pop(L, 1); // テーブル内の値をポップ
    }
}

int RunHaco8Game::l_rendr(lua_State* L) {
    RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));

    // ポリゴンデータを取得
    std::vector<std::vector<float>> polygonData;
    getPolygonData(L, polygonData);

    // ポリゴンを描画
    self->renderPolygon(polygonData,120);

    return 0;
}

int RunHaco8Game::l_creobj(lua_State* L) {

    RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
    int objectType = lua_tointeger(L, 1);

    switch (objectType) {
        case 0: // カメラ
            camera = CameraObj(); // デフォルトの初期値を使用
            camera.x = 0;
            camera.y = 0;
            camera.z = 0;
            camera.angle = 0;
            camera.zoom = 1.0;
            break;

        case 1: // 光源
            light = LightObj(); // デフォルトの初期値を使用
            light.x = 1;
            light.y = 3;
            light.z = 10;
            break;

        case 2: // cube1
            cube = CubeObj(); // デフォルトの初期値を使用
            cube.x = 0;
            cube.y = 0;
            cube.z = 0;
            cube.angle = 0;
            cube.size = 1.0;
            cube.width = 128;
            cube.height = 120;
            break;
    }
    
    lua_newtable(L); // テーブルを作成

    switch (objectType) {
        case 0: // カメラ
            lua_pushstring(L, "x");
            lua_pushinteger(L, camera.x);
            lua_settable(L, -3);

            lua_pushstring(L, "y");
            lua_pushinteger(L, camera.y);
            lua_settable(L, -3);

            lua_pushstring(L, "z");
            lua_pushinteger(L, camera.z);
            lua_settable(L, -3);

            lua_pushstring(L, "angle");
            lua_pushinteger(L, camera.angle);
            lua_settable(L, -3);

            lua_pushstring(L, "zoom");
            lua_pushnumber(L, camera.zoom);
            lua_settable(L, -3);
            break;

        case 1: // 光源
            lua_pushstring(L, "x");
            lua_pushinteger(L, light.x);
            lua_settable(L, -3);

            lua_pushstring(L, "y");
            lua_pushinteger(L, light.y);
            lua_settable(L, -3);

            lua_pushstring(L, "z");
            lua_pushinteger(L, light.z);
            lua_settable(L, -3);
            break;

        case 2: // cube1
            lua_pushstring(L, "x");
            lua_pushinteger(L, cube.x);
            lua_settable(L, -3);

            lua_pushstring(L, "y");
            lua_pushinteger(L, cube.y);
            lua_settable(L, -3);

            lua_pushstring(L, "z");
            lua_pushinteger(L, cube.z);
            lua_settable(L, -3);

            lua_pushstring(L, "angle");
            lua_pushinteger(L, cube.angle);
            lua_settable(L, -3);

            lua_pushstring(L, "size");
            lua_pushnumber(L, cube.size);
            lua_settable(L, -3);

            lua_pushstring(L, "width");
            lua_pushinteger(L, cube.width);
            lua_settable(L, -3);

            lua_pushstring(L, "height");
            lua_pushinteger(L, cube.height);
            lua_settable(L, -3);
            break;
    }

    return 1; // テーブルをスタックに返す
}

int RunHaco8Game::l_trans(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  int z = lua_tointeger(L, 3);
  int angle = lua_tointeger(L, 4);
  double size = lua_tonumber(L, 5);
  int objid = lua_tointeger(L, 6);//あとでオブジェクトを配列にするときに使うcube[2]のように
  int colangle = lua_tointeger(L, 7);
  cube.x = x;
  cube.y = y;
  cube.z = z;
  cube.angle = angle;
  cube.size = size;
  cube.colangle = colangle;
  cube.width = 128;
  cube.height = 120;
  return 0;
}

int RunHaco8Game::l_cam(lua_State* L){
  RunHaco8Game* self = (RunHaco8Game*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  int z = lua_tointeger(L, 3);
  int angle = lua_tointeger(L, 4);
  double zoom = lua_tonumber(L, 5);
  camera.x = x;
  camera.y = y;
  camera.z = z;
  camera.angle = angle;
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
  
  double x1 = lua_tonumber(L, 2);
  double y1 = lua_tonumber(L, 3);

  // wall line points
  double x3 = lua_tonumber(L, 4);
  double y3 = lua_tonumber(L, 5);
  double x4 = lua_tonumber(L, 6);
  double y4 = lua_tonumber(L, 7);
  int colangle = lua_tointeger(L, 8);
  
  int raylength = 64;
  int angleStep = 2;
  int centerno = 16;

  // int closestDistance=999;

  for(int rayno = 0;rayno<32; rayno++){

    double distance  = 999;
    
    int rayangle = (pangle + angleStep*(rayno-centerno) + 360) % 360;

    double sinVal = sin(rayangle * radone);
    double cosVal = -sin(((rayangle + 90 + 360) % 360) * radone);


    double x2 = x1 + raylength * cosVal; // レイの終点のx座標
    double y2 = y1 + raylength * sinVal; // レイの終点のy座標

    tft.drawLine(x1, y1, x2, y2, lua_rgb24to16(127, 127, 0));//光線を描画
    double denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    

    if (denominator == 0) {
      // 線分は平行なので、nilを返す
      
      // lua_pushnil(L);
      // return 1;
    } else {
      
      double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denominator;
      double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denominator;

      if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
        // 線分は交差しているので、交点座標を計算する
        double intersectionX = x1 + t * (x2 - x1);
        double intersectionY = y1 + t * (y2 - y1);
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
      int perpdist = intersections[rayno].distance * cos(angleDiff*radone);
      int mapheight = ( 600.0 / perpdist );

      if(mapheight > 64.0){
        mapheight = 64.0;
      }

      int cing = 32 - mapheight / 2;

      tft.fillRect(128 - rayno * 2, cing, 2, mapheight, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
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
  double x = lua_tonumber(L, 1);
  double y = lua_tonumber(L, 2);
  double w = lua_tonumber(L, 3);
  double h = lua_tonumber(L, 4);
  double x1 = lua_tonumber(L, 5);
  double y1 = lua_tonumber(L, 6);
  double angle = lua_tonumber(L, 7);
  double pheight = lua_tonumber(L, 8);
  pheight = pheight / 100;

  int bottom = y + h;

  double pcos = cos(angle*radone);
  double psin = sin(angle*radone);

  for (int wx = 0; wx < w; wx++) {
    for (int wy = 0; wy < h; wy++) {
        double hRatio = wx / (w - 1);
        double vRatio = pheight / (1 - (wy / (h - 1)));

        double sX = (1 - hRatio) * vRatio * w;
        double sY = hRatio * vRatio * h;

        double transformedX = sX * pcos - sY * psin + x1;
        double transformedY = sX * psin + sY * pcos + y1;

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
  double w = lua_tonumber(L, 3);
  double h = lua_tonumber(L, 4);
  int roix = lua_tointeger(L, 5);
  int roiy = lua_tointeger(L, 6);
  int roiW = lua_tointeger(L, 7);
  int roiH = lua_tointeger(L, 8);

  double x1 = lua_tonumber(L, 9);
  double y1 = lua_tonumber(L, 10);
  double angle = lua_tonumber(L, 11);
  double pheight = lua_tonumber(L, 12);
  pheight = pheight / 100;

  String fn = lua_tostring(L, 13);
  if (fn != NULL) {
    mapFileName = fn;
  }

//１ピクセルずつ走査して描画していく

  // int bottom = my + h;

  double pcos = cos(angle*radone);
  double psin = sin(angle*radone);

  double invW = 1.0 / (w - 1);
  double invH = 1.0 / (h - 1);

  for (int wx = 0; wx < w; wx++) {
    double hRatio = wx * invW;

    for (int wy = 0; wy < h; wy++) {
      double vRatio = pheight / (1 - (wy * invH));

      double sX = (1 - hRatio) * vRatio * w;
      double sY = hRatio * vRatio * h;

      double transformedX = sX * pcos - sY * psin + x1;
      double transformedY = sX * psin + sY * pcos + y1;

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
        uint16_t color = sprite64.readPixel((sx << 3) + (xx & 7), (sy << 3) + (yy & 7));

        tft.drawPixel(mx + wx, my + h - wy, color);
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

