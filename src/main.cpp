#include <LovyanGFX_DentaroUI.hpp>
#include <SPI.h>
#include <Wire.h>
#include <map>
#include <SD.h>

#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiSTA.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiUdp.h>

// #include <JPEGDecoder.h>

#include <Arduino.h>
#include <FS.h>
#include "SPIFFS.h"
// #include "runLuaGame.h"
#include "haco8/runHaco8Game.h"

#include "Tunes.h"
#include "runJsGame.h"
#include "wifiGame.h"

//esp-idfのライブラリを使う！
//https://qiita.com/norippy_i/items/0ed46e06427a1d574625
#include <driver/adc.h>//アナログボタンはこのヘッダファイルを忘れないように！！


// どこかでMapDictionaryのインスタンスを取得しておく
// MapDictionary& dict = MapDictionary::getInstance();

using namespace std;

#define MAX_CHAR 1
#define FORMAT_SPIFFS_IF_FAILED true

#define BUF_PNG_NUM 9

// #define TFT_WIDTH 128
// #define TFT_HEIGHT 120

// #define TFT_WIDTH_HALF 64
// #define TFT_HEIGHT_HALF 60

// #define MAPWH 16//マップのpixelサイズ

//outputmode最終描画の仕方
// int outputMode = FAST_MODE;//50FPS程度128*128 速いけど小さい画面　速度が必要なモード
int outputMode = WIDE_MODE;//20FPS程度240*240 遅いけれどタッチしやすい画面　パズルなど

WifiGame* wifiGame = NULL;

uint8_t xpos, ypos = 0;
uint8_t colValR = 0;
uint8_t colValG = 0;
uint8_t colValB = 0;

uint8_t charSpritex = 0;
uint8_t charSpritey = 0;
int pressedBtnID = -1;//この値をタッチボタン、物理ボタンの両方から操作してbtnStateを間接的に操作している

int mapsprnos[16];
// int mapsprnos[16] = { 20, 11, 32, 44, 53, 49, 54, 32, 52, 41, 46, 42, 45, 50, 43, 38 };

const uint8_t RGBValues[][3] PROGMEM = {//16bit用
  {0, 0, 0},     // 0: 黒色=なし
  {24, 40, 82},  // 1: 暗い青色
  {140, 24, 82}, // 2: 暗い紫色
  {0, 138, 74},  // 3: 暗い緑色
  {181, 77, 41}, // 4: 茶色 
  {99, 85, 74},  // 5: 暗い灰色
  {198, 195, 198}, // 6: 明るい灰色
  {255, 243, 231}, // 7: 白色
  {255, 0, 66},  // 8: 赤色
  {255, 162, 0}, // 9: オレンジ
  {255, 239, 0}, // 10: 黄色
  {0, 235, 0},   // 11: 緑色
  {0, 174, 255}, // 12: 水色
  {132, 117, 156}, // 13: 藍色
  {255, 105, 173}, // 14: ピンク
  {255, 203, 165}  // 15: 桃色
};

//2倍拡大表示用のパラメータ
float matrix_side[6] = {2.0,   // 横2倍
                     -0.0,  // 横傾き
                     258.0,   // X座標
                     0.0,   // 縦傾き
                     2.0,   // 縦2倍
                     0.0    // Y座標
                    };

//2倍拡大表示用のパラメータ
float matrix_game[6] = {2.0,   // 横2倍
                     -0.0,  // 横傾き
                     0.0,   // X座標
                     0.0,   // 縦傾き
                     2.0,   // 縦2倍
                     0.0    // Y座標
                    };

LGFX screen;//LGFXを継承

LovyanGFX_DentaroUI ui(&screen);
LGFX_Sprite tft(&screen);

#include "MapDictionary.h"
MapDictionary& dict = MapDictionary::getInstance();

LGFX_Sprite sprite88_roi = LGFX_Sprite(&tft);
LGFX_Sprite sprite11_roi = LGFX_Sprite(&tft);
LGFX_Sprite sprite64 = LGFX_Sprite();

LGFX_Sprite buffSprite = LGFX_Sprite(&tft);

static LGFX_Sprite sideSprite( &screen );//背景スプライトはディスプレイに出力
static LGFX_Sprite logoSprite( &screen );//背景スプライトはディスプレイに出力
LGFX_Sprite sprite88_0 = LGFX_Sprite(&tft);

LGFX_Sprite mapTileSprites[9];
// static LGFX_Sprite sliderSprite( &tft );//スライダ用

BaseGame* game;
Tunes tunes;
String appfileName = "";//最初に実行されるアプリ名
String txtName = "/init/txt/sample.txt";//実行されるファイル名

uint8_t mapsx = 0;
uint8_t mapsy = 0;
String mapFileName = "/init/map/0.png";
int readmapno = 0;
int divnum = 1;
bool readMapF = false;
//divnumが大きいほど少ない領域で展開できる(2の乗数)
LGFX_Sprite spritebg[16];//16種類のスプライトを背景で使えるようにする
LGFX_Sprite spriteMap;//地図用スプライト

uint8_t mapArray[MAPWH][MAPWH];
bool mapready = false;

int8_t sprbits[128];//fgetでアクセスするスプライト属性を格納するための配列

char buf[MAX_CHAR];
char str[100];//情報表示用
int mode = 0;//記号モード //0はrun 1はexit
int gameState = 0;
String appNameStr = "init";
int soundNo = -1;
double soundSpeed = 1.0;
int musicNo = -1;
bool musicflag = false;
bool sfxflag = false;
bool toneflag = false;
bool firstLoopF = true;

float sliderval[2] = {0,0};
bool optionuiflag = false;
uint64_t frame = 0;
double radone = PI/180;

// double sinValues[90];// 0から89度までの91個の要素

int addUiNum[4];
int allAddUiNum = 0;

bool downloadF = true;
bool isCardMounted = false; // SDカードがマウントされているかのフラグ

int xtile = 0;
int ytile = 0;
double ztile = 0.0;

int xtileNo = 29100;
int ytileNo = 12909;

LGFX_Sprite sprref;
String oldKeys[BUF_PNG_NUM];
SemaphoreHandle_t xSemaphore = NULL;
// タスクが終了したことを示すセマフォ
// SemaphoreHandle_t taskSemaphore = NULL;

int vol_value; //analog値を代入する変数を定義
int statebtn_value; //analog値を代入する変数を定義
int jsx_value; //analog値を代入する変数を定義
int jsy_value; //analog値を代入する変数を定義

int phbtnState[4];

enum struct FileType {
  LUA,
  JS,
  BMP,
  PNG,
  TXT,
  OTHER
};

// int getTargetDirNo(double _x, double _y){
//   double angle = atan2(_y, _x) + 2 * M_PI;
//     return fmod(angle / (M_PI / 4), 8);
// }

// getSign関数をMapDictionaryクラス外に移動
Vector2<int> getSign(int dirno) {
    if (dirno == -1) {
        // 方向を持たない場合、(0.0, 0.0, 0.0)を返す
        return {0, 0};
    } else {
        double dx = (dirno == 0 || dirno == 1 || dirno == 7) ? 1.0 : ((dirno == 3 || dirno == 4 || dirno == 5) ? -1.0 : 0.0);
        double dy = (dirno == 1 || dirno == 2 || dirno == 3) ? 1.0 : ((dirno == 5 || dirno == 6 || dirno == 7) ? -1.0 : 0.0);
        return {int(dx), int(dy)};
    }
}

Vector2<int> getKey2Sign(String _currentKey, String _targetKey) {
#include <LovyanGFX_DentaroUI.hpp>
// #include <SPI.h>
// #include <Wire.h>
#include <map>
#include <SD.h>
// #include <JPEGDecoder.h>

#include <Arduino.h>
#include <FS.h>
#include "SPIFFS.h"
// // #include "runLuaGame.h"
// #include "haco8/runHaco8Game.h"

// #include "Tunes.h"
// #include "runJsGame.h"
// #include "wifiGame.h"
// MapTile構造体の定義
#define BUF_PNG_NUM 9

class MapTile {
  private:
    LGFX_Sprite* sprptr;
    Vector2<int> tileNo;
    // int xtileNo;
    // int ytileNo;
    String filePath;
    String newKey;
    String targetKey;
    String oldKey;
    int dirno;
    bool writableF;
    int oldreadNo;
  public:
  // 他のメンバ変数やメソッドの定義
    MapTile()
    : tileNo(0,0), sprptr(nullptr), dirno(0), filePath(""), newKey(""), oldKey(""), writableF(false), oldreadNo(-1)
    {
    }

    // 初期化メソッドなどを追加する場合もあります
    void init() {
        // 初期化処理を記述
    }
    
    void drawMap2(LovyanGFX& _buffSprite, int _bufx, int _bufy) {

       if (sprptr != nullptr) {
            // 登録されたスプライトを描画
            sprptr->pushSprite(&_buffSprite, _bufx, _bufy);

        }
    }

    void setOldreadNo(int _oldreadNo){
      oldreadNo = _oldreadNo;
    }

    int getOldreadNo(){
      return oldreadNo;
    }

    void setWritableF2(bool _writableF){
      writableF = _writableF;
    }

    bool getWritableF2(){
      return writableF;
    }
    
    
    void setDirNo2( int _dirno ){
      dirno = _dirno;
    }

    int getDirNo2(){
      return dirno;
    }
    LGFX_Sprite* getSprptr2() {
        return sprptr;
    }

    // メンバ変数をセットするメソッド
    void setNewKey2(const String& _Key) {
      // Serial.println(_Key);
      newKey = _Key;
    }

    void setTileNo2(Vector2<int> _tileNo){
      tileNo = _tileNo;
    }

    // メンバ変数をゲットするメソッド
    const String getNewKey2() const {
        return newKey;
    }

    void setSprptr2(LGFX_Sprite* _sprptr) {
     sprptr = _sprptr; // メンバ変数にスプライトへのポインタを代入
    }

};

// MapDictionaryクラスの定義
//マップの検索を行う
class MapDictionary {
private:
    MapTile maptils[BUF_PNG_NUM];
    std::map<String, LGFX_Sprite*> key2ptr;

    String newKeys[BUF_PNG_NUM];
    String oldKeys[BUF_PNG_NUM];
    int dirNos[BUF_PNG_NUM];

public:
    static MapDictionary& getInstance() {
      static MapDictionary instance;
      return instance;
    }

    LGFX_Sprite* getSprptr(int no){
      return maptils[no].getSprptr2();
    } 

    void setSprptr(int no, LGFX_Sprite* _sprptr){
      maptils[no].setSprptr2(_sprptr);
    }

    void copy2buff(LovyanGFX& _buffSprite, LGFX_Sprite* _sprptr, int no) {
        // maptils[no].setSprptr2(_sprptr);
        // setSprptr(no, _sprptr);
        // maptils[no].drawMap2(_buffSprite,0,0);

        // _buffSprite.
        _buffSprite.fillRect(20*no, 20*no, 20,20, TFT_BLUE);

        // _sprptr->pushSprite(&_buffSprite, 0,0);
    }

    // //LovyanGFX&であれば LGFX(タッチポイント取れる）LGFX_Spriteのどちらにも対応できる（だがタッチポイントはとれない）
    // void drawMap( LovyanGFX& _buffSprite, String& _targetKey, int no, float _bufx, float _bufy) {

    //   maptils[no].setNewKey2(_targetKey);

    //   //すでにダウンロード済みかどうかをチェック
    //   // bool downloaddrawflag = false;
    //   //   for (int i = 0; i < BUF_PNG_NUM; i++) {
    //   //       if (maptils[i].getNewKey2() == _targetKey) {
    //   //           downloaddrawflag = true;
    //   //       }
    //   //   }
      
    //   // //なければダウンロード描画のフラグをオンして表示
    //   // if(downloaddrawflag){
    //   //   maptils[no].drawMap2(_buffSprite, _bufx,  _bufy);
    //   // }
    //   //あれば表示のみ
    // }

    // // void drawMap( LovyanGFX& _buffSprite, String& _targetKey, int no, float _bufx, float _bufy) {
    // //   //すでにダウンロード済みかどうかをチェック
      

    // //   bool downloaddrawflag = false;
    // //     for (int i = 0; i < BUF_PNG_NUM; i++) {
    // //         if (newKeys[i] != _targetKey) {
    // //             downloaddrawflag = true;
    // //         }
    // //     }

    // //     newKeys[no] = _targetKey;
      
    // //   //なければダウンロード描画のフラグをオンして表示
    // //   if(downloaddrawflag){
    // //     maptils[no].drawMap2(_buffSprite, _bufx,  _bufy);
    // //   }
    // //   //あれば表示のみ
    // // }

    // void setOldreadNo(int no, int _oldreadNo){
    //   maptils[no].setOldReadNo2(_oldreadNo);
    // }

    // int getOldreadNo(int no){
    //   return maptils[no].getOldReadNo2();
    // }

    void setWritableF(int no, bool _writableF){
      maptils[no].setWritableF2(_writableF);
    }

    bool getWritableF(int no){
      return maptils[no].getWritableF2();
    }


    void setDirNo( int no, int _dirno){
      maptils[no].setDirNo2( _dirno );
    }

  void setReadNos( int _dirno){
    if(_dirno == -1){//止まっていたら
      for(int i=0;i<9;i++){
        maptils[i].setDirNo2( i - 1 );//ひとまず0方向と同じ読み込み順にする
      }
    }else{

      maptils[0].setDirNo2( -1 );//現在位置
      // 進行方向を中心にした読み込み順を決める
      maptils[1].setDirNo2(  _dirno );//進行方向
      maptils[2].setDirNo2( (_dirno + 8 - 1) % 8);
      maptils[3].setDirNo2( (_dirno + 8 + 1) % 8);
      maptils[4].setDirNo2( (_dirno + 8 - 2) % 8);
      maptils[5].setDirNo2( (_dirno + 8 + 2) % 8);
      maptils[6].setDirNo2( (_dirno + 8 - 3) % 8);
      maptils[7].setDirNo2( (_dirno + 8 + 3) % 8);
      maptils[8].setDirNo2( (_dirno + 8 - 4) % 8);
    }
    maptils[0].setDirNo2( -1 );//現在位置
  }

int getDirNo(int no) {
    return maptils[no].getDirNo2(); 
}

  Vector2<int> getTilePos(int _dirNo) {
    Vector2<int> directions[] = {//ベクトル { X, Y, DirNo}
        { 0,  0},  //-1
        { 1,  0},  // 0
        { 1,  1},  // 1
        { 0,  1},  // 2
        {-1,  1},  // 3
        {-1,  0},  // 4
        {-1, -1},  // 5
        { 0, -1},  // 6
        { 1, -1}   // 7
    };
    
    if (_dirNo >= -1 && _dirNo <= 7) {
        return directions[_dirNo + 1]; // 配列のインデックスは-1から始まるため、+1する
    } else {
        return {0, 0}; // デフォルト
    }
  }

  int getTargetDirNo(double _x, double _y){
    double angle = atan2(_y, _x) + 2 * M_PI;
      return fmod(angle / (M_PI / 4), 8);
  }

  int getTargetDirNo(Vector3<double> _pos){
    double angle = atan2(_pos.getY(), _pos.getX()) + 2 * M_PI;
      return fmod(angle / (M_PI / 4), 8);
  }

  MapTile getMapTile(int no){
    return maptils[no];//ディクショナリー内から
  }

  // void setNewKeyloop(int no, String& _Key, LGFX_Sprite* _sprptr) {

  //   Serial.print(_Key);
  //   newKeys[no] = _Key;//ディクショナリー内にキーを登録

  //   maptils[no].setTileNo2(getKey2tileNo(_Key));

  //   if (no >= 0 && no < BUF_PNG_NUM) {
  //     maptils[no].setNewKey2(_Key);  // //マップタイル自体にキーを登録する
  //     maptils[no].setSprptr2(_sprptr);

  //     // if (key2ptr.count(_Key) == 0) {
  //     //     key2ptr[_Key] = getSprptr(no);  // キーをポインタに紐付ける
  //     // } else {
  //     //     // 既にキーが存在する場合の処理
  //     //     // 例えば、上書きするかどうかの判断など
  //     // }
  //   }
  // }

  void setNewKey(int no, String& _Key) {
    newKeys[no] = _Key;//ディクショナリー内に登録

    maptils[no].setTileNo2(getKey2tileNo(_Key));

    if (no >= 0 && no < BUF_PNG_NUM) {
      maptils[no].setNewKey2(_Key);  // //マップタイル自体に登録する

      if (key2ptr.count(_Key) == 0) {
          key2ptr[_Key] = getSprptr(no);  // キーをポインタに紐付ける
      } else {
          // 既にキーが存在する場合の処理
          // 例えば、上書きするかどうかの判断など
      }
    }
  }

  void showKeyInfo(String& _Key){
    if(key2ptr.count(_Key)>0){
      Serial.print(_Key+"の数：");
      Serial.println(key2ptr.count(_Key));
    }
  }


  LGFX_Sprite* getKey2sprptr(const String& _Key) {

    if (key2ptr.count(_Key) > 0) {
      
        auto it = key2ptr.find(_Key);
        return it->second;
    } else {
        // キーが見つからない場合はnullptrを返す
        return nullptr;
    }
  }

  Vector2<int> getKey2tileNo(String& _Key) {
    if (key2ptr.count(_Key) > 0) {
        int slashPos = _Key.indexOf('/'); // '/'の位置を取得
        if (slashPos != -1) { // '/'が見つかった場合
            String numA_str = _Key.substring(0, slashPos); // '/'より前の部分を取得
            String numB_str = _Key.substring(slashPos + 1); // '/'より後の部分を取得
            int numA = numA_str.toInt(); // 数字に変換
            int numB = numB_str.toInt(); // 数字に変換
            return {numA, numB}; // Vector2<int>を返す
        }
    }
    // キーが見つからない場合や'/'が見つからない場合はデフォルトのVector2<int>を返す
    return {-1, -1};
}


  String getNewKey(int no) {
      if (no >= 0 && no < BUF_PNG_NUM) {
          return maptils[no].getNewKey2();
      }
      return ""; // エラー時は空文字列を返す
  }

  bool getAlreadyDownlordedF(String& _Key){
    if (key2ptr.count(_Key) > 0) return true;
    else return false;

  }

};
    int slashPos = _currentKey.indexOf('/'); // '/'の位置を取得
    if (slashPos != -1) { // '/'が見つかった場合
        String numA_str = _currentKey.substring(0, slashPos); // '/'より前の部分を取得
        String numB_str = _currentKey.substring(slashPos + 1); // '/'より後の部分を取得
        int numA_current = numA_str.toInt(); // 数字に変換
        int numB_current = numB_str.toInt(); // 数字に変換
        
        slashPos = _targetKey.indexOf('/'); // '/'の位置を取得
        if (slashPos != -1) { // '/'が見つかった場合
            numA_str = _targetKey.substring(0, slashPos); // '/'より前の部分を取得
            numB_str = _targetKey.substring(slashPos + 1); // '/'より後の部分を取得
            int numA_target = numA_str.toInt(); // 数字に変換
            int numB_target = numB_str.toInt(); // 数字に変換
            
            int dx = numA_target - numA_current;
            int dy = numB_target - numB_current;
            
            return {dx, dy};
        }
    }
    
    return {0, 0}; // デフォルトの値
}

Vector3<double> currentgpos = {0,0,0};;

Vector3<double> prePos= {0.0, 0.0, 0.0};
Vector3<double> currentPos = {0,0,0};
Vector3<double> diffPos = {0.0,0.0,0.0};
int dirNos[9];

int shouldNo = 0;
int downloadLimitNum = 9;
String targetKey = "";
double tileZoom = 15.0;

float bairitu = 1.0;
// Vector2<int> buffPos;

std::vector<String> temporaryKeys;
std::vector<String> previousKeys;
std::vector<String> writableKeys;

std::vector<String> downloadKeys;
std::vector<String> predownloadKeys;
std::vector<String> allKeys;
std::vector<String> preallKeys;





// void downloadTask(void *pvParameters) {
//   int i = *((int *)pvParameters);  // iを受け取る
//   if(i==shouldNo)//同期した番号のみしか受け入れない。
//   {
//     // Serial.print("うけとった？");
//     // Serial.println(i);
//     // String filePath  = "/tokyo/15/"+ String( xtileNo + getSign(dict.getDirNo(i)).getX()) + "/" + String(ytileNo + getSign(dict.getDirNo(i)).getY()) +".png";
//     String filePath  = "/tokyo/15/"+ targetKey +".png";
//     if (SD.exists(filePath)) {
//         // ダウンロード処理
//         if(mapTileSprites[shouldNo].drawPngFile(SD, filePath, 0, 0)){//ダウンロードし終わったらtrue？(勘）
//         // mapTileSprites[shouldNo].drawPngFile(SD, filePath, 0, 0);
//           Serial.println("書き終わった");
//           shouldNo = (shouldNo+1) % downloadLimitNum;
          
//         }
//     }else{
//       Serial.print("ファイルパスがない？");
//       Serial.println(filePath);
//     }
//     delay(50); // 適切な待ち時間を設定する（例えば100ms）
//     // taskCompleteCallback();
//     xSemaphoreGive(xSemaphore); // セマフォを解放ループ描画処理が実行される
//     // vTaskDelete(NULL); // タスクを終了
//   }
//   // タスクが完了したことを示すコールバックを呼び出す
//   // taskCompleteCallback();
//   vTaskDelete(NULL); // タスクを終了
// }

// void setAllKeys(String _key) {
//     if (std::find(allKeys.begin(), allKeys.end(), _key) == allKeys.end()) {
//         // _key は downloadKeys に存在しない場合のみ追加
//         allKeys.push_back(_key);
//     }
// }


void printDownloadKeys() {
    Serial.println("Download Keys:");
    for (const auto& key : downloadKeys) {
        Serial.print(key);
    }

    Serial.println("");
}


void taskCompleteCallback() {
    xSemaphoreGive(xSemaphore);
}
void downloadTask(void *pvParameters) {
 
  int i = *((int *)pvParameters);  // iを受け取る
  if(i==shouldNo)//同期した番号のみしか受け入れない。
  {
    String filePath  = "/tokyo/"+String(int(floor(tileZoom)))+"/"+ targetKey +".png";
    // String filePath  = "/tokyo/15/"+ dict.getNewKey(shouldNo) +".png";
    if (SD.exists(filePath)) {
        // ダウンロード処理
        while(!mapTileSprites[shouldNo].drawPngFile(SD, filePath, 0, 0)){//描画が終わったらFalseダウンロードし終わったらtrue？(勘）
          
        }

        delay(500);

        // mapTileSprites[shouldNo].drawPngFile(SD, filePath, 0, 0);
          Serial.println("書き終わった");
          shouldNo = (shouldNo+1) % downloadLimitNum;
          // xSemaphoreGive(xSemaphore); // セマフォを解放ループ描画処理が実行される
          taskCompleteCallback();

    }else{
      Serial.print("ファイルパスがない？");
      Serial.println(filePath);
    }
    // delay(50); // 適切な待ち時間を設定する（例えば100ms）
    // taskCompleteCallback();
    
    // vTaskDelete(NULL); // タスクを終了
      // }
  }
  // タスクが完了したことを示すコールバックを呼び出す
  // taskCompleteCallback();
  vTaskDelete(NULL); // タスクを終了
}


void showRam(){
  
  // Serial.printf("===============================================================\n");
  // Serial.printf("Mem Test\n");
  // Serial.printf("===============================================================\n");
  // Serial.printf("esp_get_free_heap_size()                              : %6d\n", esp_get_free_heap_size() );
  // Serial.printf("esp_get_minimum_free_heap_size()                      : %6d\n", esp_get_minimum_free_heap_size() );
  // //xPortGetFreeHeapSize()（データメモリ）ヒープの空きバイト数を返すFreeRTOS関数です。これはを呼び出すのと同じheap_caps_get_free_size(MALLOC_CAP_8BIT)です。
  // Serial.printf("xPortGetFreeHeapSize()                                : %6d\n", xPortGetFreeHeapSize() );
  // //xPortGetMinimumEverFreeHeapSize()また、関連heap_caps_get_minimum_free_size()するものを使用して、ブート以降のヒープの「最低水準点」を追跡できます。
  // Serial.printf("xPortGetMinimumEverFreeHeapSize()                     : %6d\n", xPortGetMinimumEverFreeHeapSize() );
  // //heap_caps_get_free_size() さまざまなメモリ機能の現在の空きメモリを返すためにも使用できます。
  // Serial.printf("heap_caps_get_free_size(MALLOC_CAP_EXEC)              : %6d\n", heap_caps_get_free_size(MALLOC_CAP_EXEC) );
  // Serial.printf("heap_caps_get_free_size(MALLOC_CAP_32BIT)             : %6d\n", heap_caps_get_free_size(MALLOC_CAP_32BIT) );
  // Serial.printf("heap_caps_get_free_size(MALLOC_CAP_8BIT)              : %6d\n", heap_caps_get_free_size(MALLOC_CAP_8BIT) );
  // Serial.printf("heap_caps_get_free_size(MALLOC_CAP_DMA)               : %6d\n", heap_caps_get_free_size(MALLOC_CAP_DMA) );
  // Serial.printf("heap_caps_get_free_size(MALLOC_CAP_PID2)              : %6d\n", heap_caps_get_free_size(MALLOC_CAP_PID2) );
  // Serial.printf("heap_caps_get_free_size(MALLOC_CAP_PID3)              : %6d\n", heap_caps_get_free_size(MALLOC_CAP_PID3) );
  // Serial.printf("heap_caps_get_free_size(MALLOC_CAP_PID3)              : %6d\n", heap_caps_get_free_size(MALLOC_CAP_PID4) );
  // Serial.printf("heap_caps_get_free_size(MALLOC_CAP_PID4)              : %6d\n", heap_caps_get_free_size(MALLOC_CAP_PID5) );
  // Serial.printf("heap_caps_get_free_size(MALLOC_CAP_PID5)              : %6d\n", heap_caps_get_free_size(MALLOC_CAP_PID6) );
  // Serial.printf("heap_caps_get_free_size(MALLOC_CAP_PID6)              : %6d\n", heap_caps_get_free_size(MALLOC_CAP_PID7) );
  // Serial.printf("heap_caps_get_free_size(MALLOC_CAP_PID7)              : %6d\n", heap_caps_get_free_size(MALLOC_CAP_PID3) );
  // Serial.printf("heap_caps_get_free_size(MALLOC_CAP_SPIRAM)            : %6d\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM) );
  // Serial.printf("heap_caps_get_free_size(MALLOC_CAP_INTERNAL)          : %6d\n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL) );
  // Serial.printf("heap_caps_get_free_size(MALLOC_CAP_DEFAULT)           : %6d\n", heap_caps_get_free_size(MALLOC_CAP_DEFAULT) );
  // //Serial.printf("heap_caps_get_free_size(MALLOC_CAP_IRAM_8BIT)         : %6d\n", heap_caps_get_free_size(MALLOC_CAP_IRAM_8BIT) );
  // Serial.printf("heap_caps_get_free_size(MALLOC_CAP_INVALID)           : %6d\n", heap_caps_get_free_size(MALLOC_CAP_INVALID) );
  // //heap_caps_get_largest_free_block()ヒープ内の最大の空きブロックを返すために使用できます。これは、現在可能な最大の単一割り当てです。この値を追跡し、合計空きヒープと比較すると、ヒープの断片化を検出できます。
  // Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_EXEC)     : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_EXEC) );
  // Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_32BIT)    : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_32BIT) );
  // Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_8BIT)     : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_8BIT) );
  // Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_DMA)      : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_DMA) );
  // Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_PID2)     : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_PID2) );
  // Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_PID3)     : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_PID3) );
  // Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_PID3)     : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_PID4) );
  // Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_PID4)     : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_PID5) );
  // Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_PID5)     : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_PID6) );
  // Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_PID6)     : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_PID7) );
  // Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_PID7)     : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_PID3) );
  // Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM)   : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM) );
  // Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL) : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL) );
  // Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT)  : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT) );
  // //Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_IRAM_8BIT): %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_IRAM_8BIT) );
  // Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_INVALID)  : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_INVALID) );
  // //heap_caps_get_minimum_free_size()指定された機能を持つすべての領域の合計最小空きメモリを取得します。
  // Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_EXEC)      : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_EXEC) );
  // Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_32BIT)     : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_32BIT) );
  // Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT)      : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT) );
  // Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_DMA)       : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_DMA) );
  // Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_PID2)      : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_PID2) );
  // Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_PID3)      : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_PID3) );
  // Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_PID3)      : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_PID4) );
  // Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_PID4)      : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_PID5) );
  // Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_PID5)      : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_PID6) );
  // Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_PID6)      : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_PID7) );
  // Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_PID7)      : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_PID3) );
  // Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_SPIRAM)    : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_SPIRAM) );
  // Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL)  : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL) );
  // Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_DEFAULT)   : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_DEFAULT) );
  // //Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_IRAM_8BIT) : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_IRAM_8BIT) );
  // Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_INVALID)   : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_INVALID) );
}

void drawLogo()
{
  logoSprite.setPsram(false);
  logoSprite.setColorDepth(16);    // 子スプライトの色深度
  logoSprite.createSprite(30, 10); // ゲーム画面用スプライトメモリ確保

  // PNGをバッファに書いて2倍出力
  logoSprite.drawPngFile(SPIFFS, "/init/logo.png", 0, 0); // 4ボタン
  logoSprite.pushAffine(matrix_side);

  // メモリを解放するのは後で行うこと

  // fillRoundRectの最適化
  uint16_t x1 = 258;
  uint16_t y1 = 20;
  uint16_t x2 = 28;
  uint16_t y2 = 50;
  uint16_t cornerRadius = 2;
  uint16_t color = TFT_DARKGRAY;

  for (int j = 0; j < 2; j++) {
      for (int i = 0; i < 2; i++) {
          screen.fillRoundRect(x1 + 30 * i, y1 + 52 * j, x2, y2, cornerRadius, color);
      }
  }

  screen.fillRoundRect(x1, 124, x2 + 30, 38, cornerRadius, TFT_LIGHTGRAY);

  screen.fillRoundRect(x1, 164, x2, 38, cornerRadius, TFT_LIGHTGRAY);
  screen.fillRoundRect(x1 + 30, 164, x2, 38, cornerRadius, TFT_LIGHTGRAY);

  screen.fillRoundRect(x1, 204, x2 + 30, 38, cornerRadius, TFT_LIGHTGRAY);

  // スプライトの解放
  logoSprite.deleteSprite();

}

bool isWifiDebug(){
  return wifiGame != NULL;
}

void reboot()
{
  wifiGame->pause();
  ESP.restart();
}

FileType detectFileType(String *appfileName)
{
  if(appfileName->endsWith(".js")){
    return FileType::JS;
  }else if(appfileName->endsWith(".lua")){
    return FileType::LUA;
  }else if(appfileName->endsWith(".bmp")){
    return FileType::BMP;
  }else if(appfileName->endsWith(".png")){
    return FileType::PNG;
  }else if(appfileName->endsWith(".txt")){
    return FileType::TXT;
  }
  return FileType::OTHER;
}

String *targetfileName;
BaseGame* nextGameObject(String* _appfileName, int _gameState, String _mn)
{
  // gameState = _gameState;
  switch(detectFileType(_appfileName)){
    case FileType::JS:  
      game = new RunJsGame(); 
      break;
    case FileType::LUA: 
      game = new RunHaco8Game(_gameState, _mn);
      break;
    case FileType::TXT: 
      game = new RunJsGame(); 
      //ファイル名がもし/init/param/caldata.txtなら
      if(*_appfileName == CALIBRATION_FILE)
      {
        ui.calibrationRun(screen);//キャリブレーション実行してcaldata.txtファイルを更新して
        drawLogo();//サイドボタンを書き直して
      }
      appfileName = "/init/txt/main.js";//txtエディタで開く
      break; //txteditorを立ち上げてtxtを開く
    case FileType::BMP: // todo: error
      game = NULL;
      break;
    case FileType::PNG: // todo: error
      game = new RunJsGame(); 
      appfileName = "/init/png/main.js";//pngエディタで開く
      break;
    case FileType::OTHER: // todo: error
      game = NULL;
      break;
  }
  return game;
}

void startWifiDebug(bool isSelf){
  tunes.pause();
  wifiGame = new WifiGame();
  wifiGame->init(isSelf);
  tunes.resume();
}

// void tone(int _toneNo, int _tonelength){
//   ledcWriteTone(1,TONES[_toneNo]);
//   delay(_tonelength);
//   ledcWriteTone(1, 0);    // ならしたら最後に消音
// }

// hw_timer_t * timerA = NULL;//スピーカー用
// volatile static boolean timer_flag = false;
//高速
// void IRAM_ATTR onTimerA() {
//   timer_flag != timer_flag;
// }

char *A;
// bool flip = true;

uint32_t preTime;
void setFileName(String s){
  appfileName = s;
}

void runFileName(String s){
  
  ui.setConstantGetF(false);//初期化処理 タッチポイントの常時取得を切る
  
  appfileName = s;
  mode = 1;//exit to run

}

// タイマー
hw_timer_t * timer = NULL;

// 画面描画タスクハンドル
// TaskHandle_t taskHandle;

// タイマー割り込み
// void IRAM_ATTR onTimer() {
//   xTaskNotifyFromISR(taskHandle, 0, eIncrement, NULL);
// }

//ファイル書き込み
void writeFile(fs::FS &fs, const char * path, const char * message){
    File file = fs.open(path, FILE_WRITE);
    if(!file){
        return;
    }
    file.print(message);
}

void wCalData(String _wrfile){
  char numStr[64];
  sprintf(numStr, "%d,%d,%d,%d,%d,%d,%d,%d", 
    ui.getCalData(0),ui.getCalData(1),ui.getCalData(2),ui.getCalData(3),
    ui.getCalData(4),ui.getCalData(5),ui.getCalData(6),ui.getCalData(7)
  );
  String writeStr = numStr;	// ⑤書き込み文字列を設定
  File fw = SPIFFS.open(_wrfile.c_str(), "w");// ⑥ファイルを書き込みモードで開く
  fw.println( writeStr );	// ⑦ファイルに書き込み
  fw.close();	// ⑧ファイルを閉じる
}

String rCalData(String _wrfile){
  File fr = SPIFFS.open(_wrfile.c_str(), "r");// ⑩ファイルを読み込みモードで開く
  String _readStr = fr.readStringUntil('\n');// ⑪改行まで１行読み出し
  fr.close();	// ⑫	ファイルを閉じる
  return _readStr;
}

String rFirstAppName(String _wrfile){
  File fr = SPIFFS.open(_wrfile.c_str(), "r");// ⑩ファイルを読み込みモードで開く
  String _readStr = fr.readStringUntil('\n');// ⑪改行まで１行読み出し
  fr.close();	// ⑫	ファイルを閉じる
  return _readStr;
}

int readMap()
{
  mapready = false;

  for(int n = 0; n<divnum; n++)
  {
    spriteMap.drawPngFile( SPIFFS, mapFileName, 0, (int32_t)(-MAPWH*n/divnum) );
    for(int j = 0; j<MAPWH/divnum; j++){
      for(int i = 0; i<MAPWH; i++){

        int k = j+(MAPWH/divnum)*(n);//マップ下部
        colValR = spriteMap.readPixelRGB(i,j).R8();
        colValG = spriteMap.readPixelRGB(i,j).G8();
        colValB = spriteMap.readPixelRGB(i,j).B8();

  //16ビットRGB（24ビットRGB）
        if(colValR==0&&colValG==0&&colValB==0){//0: 黒色=なし
          mapArray[i][k] = mapsprnos[0];//20;
        }else if(colValR==24&&colValG==40&&colValB==82){//{ 27,42,86 },//1: 暗い青色
          mapArray[i][k] = mapsprnos[1];//11;//5*8+5;
        }else if(colValR==140&&colValG==24&&colValB==82){//{ 137,24,84 },//2: 暗い紫色
          mapArray[i][k] = mapsprnos[2];//32;//5*8+5;
        }else if(colValR==0&&colValG==138&&colValB==74){//{ 0,139,75 },//3: 暗い緑色
          mapArray[i][k] = mapsprnos[3];//44;//5*8+5;
        }else if(colValR==181&&colValG==77&&colValB==41){//{ 183,76,45 },//4: 茶色 
          mapArray[i][k] = mapsprnos[4];//53;//5*8+5;
        }else if(colValR==99&&colValG==85&&colValB==74){//{ 97,87,78 },//5: 暗い灰色
          mapArray[i][k] = mapsprnos[5];//49;
        }else if(colValR==198&&colValG==195&&colValB==198){//{ 194,195,199 },//6: 明るい灰色
          mapArray[i][k] = mapsprnos[6];//54;//5*8+5;
        }else if(colValR==255&&colValG==243&&colValB==231){//{ 255,241,231 },//7: 白色
          mapArray[i][k] = mapsprnos[7];//32;
        }else if(colValR==255&&colValG==0&&colValB==66){//{ 255,0,70 },//8: 赤色
          mapArray[i][k] = mapsprnos[8];//52;
        }else if(colValR==255&&colValG==162&&colValB==0){//{ 255,160,0 },//9: オレンジ
          mapArray[i][k] = mapsprnos[9];//41;//5*8+5;
        }else if(colValR==255&&colValG==239&&colValB==0){//{ 255,238,0 },//10: 黄色
          mapArray[i][k] = mapsprnos[10];//46;
        }else if(colValR==0&&colValG==235&&colValB==0){//{ 0,234,0 },//11: 緑色
          mapArray[i][k] = mapsprnos[11];//42;
        }else if(colValR==0&&colValG==174&&colValB==255){//{ 0,173,255 },//12: 水色
          mapArray[i][k] = mapsprnos[12];//45;//5*8+5;
        }else if(colValR==132&&colValG==117&&colValB==156){//{ 134,116,159 },//13: 藍色
          mapArray[i][k] = mapsprnos[13];//50;
        }else if(colValR==255&&colValG==105&&colValB==173){//{ 255,107,169 },//14: ピンク
          mapArray[i][k] = mapsprnos[14];//43;//5*8+5;
        }else if(colValR==255&&colValG==203&&colValB==165){//{ 255,202,165}//15: 桃色
          mapArray[i][k] = mapsprnos[15];//38;//5*8+5;
        }
        if(i==MAPWH-1 && k==MAPWH-1){mapready = true;return 1;}//読み込み終わったら1をリターン
      }
    }
  }
  // spriteMap.deleteSprite();//メモリに格納したら解放する
  return 1;
}

using namespace std;
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <cmath>

using namespace std;

vector<string> split(string& input, char delimiter)
{
    istringstream stream(input);
    string field;
    vector<string> result;
    while (getline(stream, field, delimiter)) {
        result.push_back(field);
    }
    return result;
}

void setup()
{

  adc1_config_width(ADC_WIDTH_BIT_12);
  // //何ビットのADCを使うか設定する。今回は12bitにします。
  // //adc1の場合はこのように使うチャンネル全体の設定をするコマンドが用意されている。

  adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11);//39pin　4つのボタン
  adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_11);//33pin　ボリューム
  adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);//34pin　ジョイスティックX
  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);//35pin　ジョイスティックY

  // xSemaphore = xSemaphoreCreateMutex();
  // セマフォを作成
  
  xSemaphore = xSemaphoreCreateBinary();
   if (xSemaphore == NULL) {
    // セマフォの作成に失敗した場合の処理
    // 通常、メモリ不足などの原因が考えられます
    while (1);
  }

  
  // タスクを作成
  // xTaskCreatePinnedToCore(downloadTask, "DownloadTask", 4096, NULL, 1, NULL, 0);

  // xTaskCreatePinnedToCore(drawTask, "DrawTask", 4096, NULL, 1, NULL, 0);



  Serial.begin(115200);
  delay(50);

  //sin 0~90度をメモリに保持する

  // for (int i = 0; i < 90; ++i) {
  //   double radians = i * M_PI / 180.0;
  //   sinValues[i] = sin(radians);
  // }
  // sinValues[90] = 1.0;
  // sinValues[270] = -1.0;

  // xTaskCreateUniversal(//マルチタスクでバックグラウンド読み込み処理(RTOS)
  //   task2,        //pxTaskCode  作成するタスク関数
  //   "task2",      //pcName  表示用タスク名
  //   8192,         //usStackDepth スタックメモリ量
  //   NULL,         //pvParameters 起動パラメータ
  //   1,            //uxPriority  優先度
  //   NULL,         //pxCreatedTask タスクハンドル
  //   PRO_CPU_NUM   //xCoreID 実行するコア
  // );


  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  if(rCalData(CALIBRATION_FILE) == NULL){//タッチキャリブレーションデータがなければ
    ui.begin( screen, 16, 1, true);//立ち上げ時にキャリブレーションする
  }else{
    ui.begin( screen, 16, 1, false);
  }
  wCalData(CALIBRATION_FILE);//SPIFFSのファイルにキャリブレーションデータを書き込む

  appfileName = rFirstAppName("/init/param/firstAppName.txt");//最初に立ち上げるゲームのパスをSPIFFSのファイルfirstAppName.txtから読み込む

  // ui.setupPhBtns(36, 39, 34);//物理ボタンをセットアップ


  //-------------------------------------------------------------
  String host ="tokyo";//ルートディレクトリ名です。
  
   SD.end();
  // SDカードがマウントされているかの確認

  if(SD.begin(SDCARD_SS_PIN)){//custom設定のCPUのクロック周波数の定数分の1でないとタイミングが合わず、動かなくなるもよう
    Serial.println("Card Mount Successful");
    isCardMounted = true;
    delay(500);
  } else {
    Serial.println("Card Mount Failed");
    // SDカードのマウントに失敗した場合は、フラグをfalseに設定し、後で再試行する
    isCardMounted = false;
    // while (1) {}
  }

  if (SD.begin(SDCARD_SS_PIN)) {
    Serial.println("SD card initialization successful!");
    uint8_t cardType = SD.cardType();

    switch (cardType) {
      case CARD_NONE:
        Serial.println("No SD card");
        break;
      case CARD_MMC:
        Serial.println("MMC card");
        break;
      case CARD_SD:
        Serial.println("SD card");
        break;
      case CARD_SDHC:
        Serial.println("SDHC card");
        break;
      case CARD_UNKNOWN:
      default:
        Serial.println("Unknown card type");
        break;
    }
  } else {
    Serial.println("SD card initialization failed!");
  }

  sprintf(str, "%d", ui.getCalData(0));sprintf(str, "%d", ",");
  sprintf(str, "%d", ui.getCalData(1));sprintf(str, "%d", ",");
  sprintf(str, "%d", ui.getCalData(2));sprintf(str, "%d", ",");
  sprintf(str, "%d", ui.getCalData(3));sprintf(str, "%d", ",");
  sprintf(str, "%d", ui.getCalData(4));sprintf(str, "%d", ",");
  sprintf(str, "%d", ui.getCalData(5));sprintf(str, "%d", ",");
  sprintf(str, "%d", ui.getCalData(6));sprintf(str, "%d", ",");
  sprintf(str, "%d", ui.getCalData(7));sprintf(str, "%d", ",");

  ui.setTouchZoom(2);//2倍表示

  drawLogo();//ロゴを表示
  // ui.begin( screen, 16, 1, true);

  sprite88_0.setPsram(false );
  sprite88_0.setColorDepth(16);//子スプライトの色深度
  sprite88_0.createSprite(8, 8);//ゲーム画面用スプライトメモリ確保
  //sprite88_0.drawPngFile(SPIFFS, "/init/sprite.png", -8*1, -8*0);

  sprite64.setPsram(false );
  sprite64.setColorDepth(16);//子スプライトの色深度
  sprite64.createSprite(64, 128);//ゲーム画面用スプライトメモリ確保

  sprite64.drawPngFile(SPIFFS, "/init/initspr.png", 0, 0);
  
  buffSprite.setPsram( true );
  buffSprite.setColorDepth(16);//子スプライトの色深度
  buffSprite.createSprite(256, 256);//ゲーム画面用スプライトメモリ確保

  for( int i = 0; i < BUF_PNG_NUM; i++ ){
    mapTileSprites[i].setPsram(true);
    mapTileSprites[i].setColorDepth(16);
    mapTileSprites[i].createSprite(256,256);
    // MapTile クラスをインスタンス化し、スプライトに描画して返す
    dict.copy2buff(buffSprite, &mapTileSprites[i], i);
  }

  //キーと紐づけ、初期設定のキー0~9と値のペアを適当に登録しておく
  for(int j = 0; j<3; j++){
    for(int i = 0; i<3; i++){
      dict.setSprptr(i*3+j, &mapTileSprites[i]);
      dict.setNewKey(i*3+j, String(xtileNo+i) + "/" + String(ytileNo+j));
      dict.showKeyInfo(String(xtileNo+i) + "/" + String(ytileNo+j));
    }
  }

  sprite88_roi.setPsram(false );
  sprite88_roi.setColorDepth(16);//子スプライトの色深度
  sprite88_roi.createSprite(8, 8);//ゲーム画面用スプライトメモリ確保

  sprite11_roi.setPsram(false );
  sprite11_roi.setColorDepth(16);//子スプライトの色深度
  sprite11_roi.createSprite(1, 1);//ゲーム画面用スプライトメモリ確保

  spriteMap.setPsram(false );
  spriteMap.setColorDepth(16);//子スプライトの色深度
  spriteMap.createSprite(MAPWH, MAPWH/divnum);//マップ展開用スプライトメモリ確保

File fr = SPIFFS.open("/init/param/uiinfo.txt", "r");
String line;

while (fr.available()) {
  line = fr.readStringUntil('\n');
  if (!line.isEmpty()) {
    int commaIndex = line.indexOf(',');
    if (commaIndex != -1) {
      String val = line.substring(0, commaIndex);
      addUiNum[0] = val.toInt();

      if(addUiNum[0]!=-1){//-1の時は生成しない

        for (int i = 1; i < 6; i++) {
          int nextCommaIndex = line.indexOf(',', commaIndex + 1);
          if (nextCommaIndex != -1) {
            val = line.substring(commaIndex + 1, nextCommaIndex);
            addUiNum[i] = val.toInt();
            commaIndex = nextCommaIndex;
          }
        }
        ui.createPanel( addUiNum[0], addUiNum[1], addUiNum[2], addUiNum[3], addUiNum[4], addUiNum[5], TOUCH, ui.getTouchZoom());//ホームボタン
        allAddUiNum++;
      }
    }
  }
}

fr.close();
  delay(100);

  tft.setPsram( false );//DMA利用のためPSRAMは切る
  tft.createSprite( 128, 128 );
  tft.startWrite();//CSアサート開始
  
  tft.setTextSize(1);
  tft.setFont(&lgfxJapanGothicP_8);
  tft.setTextColor( TFT_WHITE , TFT_BLACK );
  tft.setCursor( 0,0 );

  // mapready = false;
  // while(mapready == false){
  //   readMap(1);
  // };//マップ番号を指定し、読み込み終わるまで待機

  mapFileName = "/init/map/0.png";
  readMap();

  delay(1000);

  game = nextGameObject(&appfileName, gameState, mapFileName);//ホームゲームを立ち上げる
  game->init();
  tunes.init();

  showRam();
  frame=0;

  // ui.setupPhBtns(36, 25, 22);//物理ボタンをセットアップ  
}

void loop()
{

  //AD1の場合
  jsx_value = 0;//34pin　ジョイスティックX
  jsy_value = 0;//35pin　ジョイスティックY
  vol_value = 0;//33pin　ボリューム
  statebtn_value = 0;//39pin　4つのボタン

  jsx_value = adc1_get_raw(ADC1_CHANNEL_7);//34pin　ジョイスティックX
  jsy_value = adc1_get_raw(ADC1_CHANNEL_6);//35pin　ジョイスティックY
  vol_value = adc1_get_raw(ADC1_CHANNEL_5);//33pin　ボリューム
  statebtn_value = adc1_get_raw(ADC1_CHANNEL_3);//39pin　4つのボタン
  
  phbtnState[0] = jsx_value;
  phbtnState[1] = jsy_value;
  phbtnState[2] = statebtn_value;
  phbtnState[3] = vol_value;
  //取得したいチャンネルを指定したらOK！

  Serial.print("joyStick x[0]:");
  Serial.print(jsx_value);
  Serial.print(" y[1]:");
  Serial.print(jsy_value);
  Serial.print("  button[2]:");
  Serial.print(statebtn_value);
  Serial.print("  volume[3]:");
  Serial.print(vol_value);
  
  Serial.println(":");



  //  ui.updatePhBtns();//物理ボタンの状態を更新
  //  uint32_t hitvalue = ui.getHitValue();
  //  // 入力内容を画面とシリアルに出力
  //       switch (hitvalue)
  //       {
  //       default:  Serial.println("--"); break;
  //       case 101: Serial.println("A click"); break;
  //       case 102: Serial.println("B click"); break;
  //       case 103: Serial.println("C click"); break;
  //       case 111: Serial.println("A hold"); break;
  //       case 112: Serial.println("B hold"); break;
  //       case 113: Serial.println("C hold"); break;
  //       case 121: Serial.println("A double click"); break;
  //       case 122: Serial.println("B double click"); break;
  //       case 123: Serial.println("C double click"); break;
  //       case 201: Serial.println("AB hold"); break;
  //       case 202: Serial.println("AC hold"); break;
  //       case 203: Serial.println("BC hold"); break;
  //       case 204: Serial.println("ABC hold"); break;
  //       case 301: Serial.println("A->B->C"); break;
  //       }

  // ui.setConstantGetF(true);//trueだとタッチポイントのボタンIDを連続取得するモード
  ui.update(screen);//タッチイベントを取るので、LGFXが基底クラスでないといけない

  if( ui.getEvent() != NO_EVENT ){//何かイベントがあれば

    if( ui.getEvent() == TOUCH ){//TOUCHの時だけ
      
    }
    if(ui.getEvent() == MOVE){
      pressedBtnID = ui.getTouchBtnID();
    }
    if( ui.getEvent() == RELEASE ){//RELEASEの時だけ
      // ui.setBtnID(-1);//タッチボタンIDをリセット
      // pressedBtnID = ui.getTouchBtnID()+12;//12個分の物理ボタンをタッチボタンIDに足す
      pressedBtnID = -1;//リセット
    }
  }

      if(optionuiflag == true){

      if( ui.getTouchBtnID() == ui.getUiFirstNo( ui.getUiID("SLIDER_5") )  )
      {
        // sliderval[0] = int( ui.getSliderVal( ui.getUiID("SLIDER_5"), 0, X_VAL)*128 ); 
        sliderval[0] = ui.getSliderVal( ui.getUiID("SLIDER_5"), 0, X_VAL ); 
        sliderval[1] = ui.getSliderVal( ui.getUiID("SLIDER_5"), 0, Y_VAL ); 
        // Serial.println(ui.getSliderVal( ui.getUiID("SLIDER_5"), 0, X_VAL));
      }
    }

    // if(ui.getTouchBtnID() == RELEASE){//リリースされたら
    //   pressedBtnID = -1;
    // }
  
  uint32_t now = millis();
  uint32_t remainTime= (now - preTime);
  preTime = now;

  //ゲーム内のprint時の文字設定をしておく
  tft.setTextSize(1);//サイズ
  tft.setFont(&lgfxJapanGothicP_8);//日本語可
  tft.setCursor(0, 0);//位置
  tft.setTextWrap(true);

  // == wifi task ==
  if(wifiGame){ // debug mode
    int r = wifiGame->run(remainTime);
    if(r != 0){ // reload request

      tunes.pause();
      game->pause();
      // ui.clearAddBtns();//個別のゲーム内で追加したボタンを消去する
      free(game);
      txtName = appfileName;
      game = nextGameObject(&appfileName, gameState, mapFileName);
      game->init();
      tunes.resume();
    }
  }
  // == tune task ==
  tunes.run();

  // == game task ==
  
  mode = game->run(remainTime);//exitは1が返ってくる　mode=１ 次のゲームを起動

  //0ボタンで強制終了
  if (pressedBtnID == 0)
  { // reload
    ui.setConstantGetF(false);//初期化処理 タッチポイントの常時取得を切る
    appfileName = "/init/main.lua";
    
    // game->setWifiDebugRequest(false);//外部ファイルから書き換えてWifiモードにできる
    // game->setWifiDebugSelf(false);
    mode = 1;//exit
  }

  if (pressedBtnID == 9999)
  { // reload
    ui.setConstantGetF(false);//初期化処理 タッチポイントの常時取得を切る
    mode = 1;//exit
    pressedBtnID = -1;
  }

  if(mode != 0){ // exit request//次のゲームを立ち上げるフラグがた値、modeが１＝次のゲームを起動であれば
    tunes.pause();
    game->pause();
    // ui.clearAddBtns();//個別のゲーム内で追加したタッチボタンを消去する
    free(game);
    firstLoopF = true;
    toneflag = false;
    sfxflag = false;
    musicflag = false;
    txtName = appfileName;
    game = nextGameObject(&appfileName, gameState, mapFileName);//ファイルの種類を判別して適したゲームオブジェクトを生成
    game->init();//resume()（再開処理）を呼び出し、ゲームで利用する関数などを準備
    tunes.resume();
    
  }

        ui.showTouchEventInfo( tft, 0, 100 );//タッチイベントを視覚化する
        ui.showInfo( tft, 0, 100+8 );//ボタン情報、フレームレート情報などを表示します。
        // ui.drawPhBtns( tft, 0, 90+16 );//物理ボタンの状態を表示

        
        
        //SD利用可能かどうか
        tft.setTextSize(1);
        tft.setTextColor( TFT_WHITE , TFT_BLACK );
        tft.setCursor( 100, 0 );
        if(isCardMounted){
          tft.print("SDアリ");
        }else{
          tft.print("SDナシ");
        }

        if(outputMode == WIDE_MODE)
        {
          tft.pushAffine(matrix_game);//ゲーム画面を最終描画する
          //Affineを使わない書き方
          // tft.setPivot(0, 0);
          // tft.pushRotateZoom(&screen, 0, 0, 0, 2, 2);
        }
        else if(outputMode == FAST_MODE){
          tft.pushSprite(&screen,TFT_OFFSET_X,TFT_OFFSET_Y);//ゲーム画面を小さく高速描画する
        }
        
        // delay(4);//120FPS スプライトが少ない、速度の速いモード描画ぶん待つ
        // delay(10);//30FPS メニュー、パズルなど

  // int wait = 1000/60 - remainTime;//フレームレートを60FPS合わせる
  // if(wait > 0){
  //   delay(wait);
  // }
  // xSemaphoreGiveFromISR(semaphore, NULL);

  frame++;

  if(frame > 18446744073709551615)frame = 0;

  delay(1);
}