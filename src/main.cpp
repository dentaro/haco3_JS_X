#include <LovyanGFX_DentaroUI.hpp>
#include <SPI.h>
#include <Wire.h>
// #include <SD.h>

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
using namespace std;

#define FORMAT_SPIFFS_IF_FAILED true
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
LGFX_Sprite sprite88_roi = LGFX_Sprite(&tft);
LGFX_Sprite sprite64 = LGFX_Sprite();
static LGFX_Sprite sideSprite( &screen );//背景スプライトはディスプレイに出力
static LGFX_Sprite logoSprite( &screen );//背景スプライトはディスプレイに出力
LGFX_Sprite sprite88_0 = LGFX_Sprite(&tft);
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
LGFX_Sprite spriteMap;

uint8_t mapArray[MAPWH][MAPWH];
bool mapready = false;

int8_t sprbits[128];//fgetでアクセスするスプライト属性を格納するための配列

char buf[MAX_CHAR];
char str[100];//情報表示用
int mode = 0;//記号モード //0はrun 1はexit
int gameState = 0;
String appNameStr = "init";
int soundNo = -1;
int musicNo = -1;
bool musicflag = false;
bool sfxflag = false;
float sliderval[2] = {0,0};
bool optionuiflag = false;

enum struct FileType {
  LUA,
  JS,
  BMP,
  PNG,
  TXT,
  OTHER
};

void showRam(){
  
  Serial.printf("===============================================================\n");
  Serial.printf("Mem Test\n");
  Serial.printf("===============================================================\n");
  Serial.printf("esp_get_free_heap_size()                              : %6d\n", esp_get_free_heap_size() );
  Serial.printf("esp_get_minimum_free_heap_size()                      : %6d\n", esp_get_minimum_free_heap_size() );
  //xPortGetFreeHeapSize()（データメモリ）ヒープの空きバイト数を返すFreeRTOS関数です。これはを呼び出すのと同じheap_caps_get_free_size(MALLOC_CAP_8BIT)です。
  Serial.printf("xPortGetFreeHeapSize()                                : %6d\n", xPortGetFreeHeapSize() );
  //xPortGetMinimumEverFreeHeapSize()また、関連heap_caps_get_minimum_free_size()するものを使用して、ブート以降のヒープの「最低水準点」を追跡できます。
  Serial.printf("xPortGetMinimumEverFreeHeapSize()                     : %6d\n", xPortGetMinimumEverFreeHeapSize() );
  //heap_caps_get_free_size() さまざまなメモリ機能の現在の空きメモリを返すためにも使用できます。
  Serial.printf("heap_caps_get_free_size(MALLOC_CAP_EXEC)              : %6d\n", heap_caps_get_free_size(MALLOC_CAP_EXEC) );
  Serial.printf("heap_caps_get_free_size(MALLOC_CAP_32BIT)             : %6d\n", heap_caps_get_free_size(MALLOC_CAP_32BIT) );
  Serial.printf("heap_caps_get_free_size(MALLOC_CAP_8BIT)              : %6d\n", heap_caps_get_free_size(MALLOC_CAP_8BIT) );
  Serial.printf("heap_caps_get_free_size(MALLOC_CAP_DMA)               : %6d\n", heap_caps_get_free_size(MALLOC_CAP_DMA) );
  Serial.printf("heap_caps_get_free_size(MALLOC_CAP_PID2)              : %6d\n", heap_caps_get_free_size(MALLOC_CAP_PID2) );
  Serial.printf("heap_caps_get_free_size(MALLOC_CAP_PID3)              : %6d\n", heap_caps_get_free_size(MALLOC_CAP_PID3) );
  Serial.printf("heap_caps_get_free_size(MALLOC_CAP_PID3)              : %6d\n", heap_caps_get_free_size(MALLOC_CAP_PID4) );
  Serial.printf("heap_caps_get_free_size(MALLOC_CAP_PID4)              : %6d\n", heap_caps_get_free_size(MALLOC_CAP_PID5) );
  Serial.printf("heap_caps_get_free_size(MALLOC_CAP_PID5)              : %6d\n", heap_caps_get_free_size(MALLOC_CAP_PID6) );
  Serial.printf("heap_caps_get_free_size(MALLOC_CAP_PID6)              : %6d\n", heap_caps_get_free_size(MALLOC_CAP_PID7) );
  Serial.printf("heap_caps_get_free_size(MALLOC_CAP_PID7)              : %6d\n", heap_caps_get_free_size(MALLOC_CAP_PID3) );
  Serial.printf("heap_caps_get_free_size(MALLOC_CAP_SPIRAM)            : %6d\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM) );
  Serial.printf("heap_caps_get_free_size(MALLOC_CAP_INTERNAL)          : %6d\n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL) );
  Serial.printf("heap_caps_get_free_size(MALLOC_CAP_DEFAULT)           : %6d\n", heap_caps_get_free_size(MALLOC_CAP_DEFAULT) );
  //Serial.printf("heap_caps_get_free_size(MALLOC_CAP_IRAM_8BIT)         : %6d\n", heap_caps_get_free_size(MALLOC_CAP_IRAM_8BIT) );
  Serial.printf("heap_caps_get_free_size(MALLOC_CAP_INVALID)           : %6d\n", heap_caps_get_free_size(MALLOC_CAP_INVALID) );
  //heap_caps_get_largest_free_block()ヒープ内の最大の空きブロックを返すために使用できます。これは、現在可能な最大の単一割り当てです。この値を追跡し、合計空きヒープと比較すると、ヒープの断片化を検出できます。
  Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_EXEC)     : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_EXEC) );
  Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_32BIT)    : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_32BIT) );
  Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_8BIT)     : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_8BIT) );
  Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_DMA)      : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_DMA) );
  Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_PID2)     : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_PID2) );
  Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_PID3)     : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_PID3) );
  Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_PID3)     : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_PID4) );
  Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_PID4)     : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_PID5) );
  Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_PID5)     : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_PID6) );
  Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_PID6)     : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_PID7) );
  Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_PID7)     : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_PID3) );
  Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM)   : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM) );
  Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL) : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL) );
  Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT)  : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT) );
  //Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_IRAM_8BIT): %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_IRAM_8BIT) );
  Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_INVALID)  : %6d\n", heap_caps_get_largest_free_block(MALLOC_CAP_INVALID) );
  //heap_caps_get_minimum_free_size()指定された機能を持つすべての領域の合計最小空きメモリを取得します。
  Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_EXEC)      : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_EXEC) );
  Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_32BIT)     : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_32BIT) );
  Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT)      : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT) );
  Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_DMA)       : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_DMA) );
  Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_PID2)      : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_PID2) );
  Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_PID3)      : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_PID3) );
  Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_PID3)      : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_PID4) );
  Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_PID4)      : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_PID5) );
  Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_PID5)      : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_PID6) );
  Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_PID6)      : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_PID7) );
  Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_PID7)      : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_PID3) );
  Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_SPIRAM)    : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_SPIRAM) );
  Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL)  : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL) );
  Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_DEFAULT)   : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_DEFAULT) );
  //Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_IRAM_8BIT) : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_IRAM_8BIT) );
  Serial.printf("heap_caps_get_minimum_free_size(MALLOC_CAP_INVALID)   : %6d\n", heap_caps_get_minimum_free_size(MALLOC_CAP_INVALID) );
}

void drawLogo()
{
  logoSprite.setPsram(false);
  logoSprite.setColorDepth(16);                // 子スプライトの色深度
  logoSprite.createSprite(30, 10); // ゲーム画面用スプライトメモリ確保
  //  //SDからの読み込みは４Gb以下じゃないとうまく動作しないかも
  //PNGをバッファに書いて2倍出力
  logoSprite.drawPngFile(SPIFFS, "/init/logo.png", 0, 0); // 4ボタン
  logoSprite.pushAffine(matrix_side);
  logoSprite.deleteSprite(); // 描画したらメモリを解放する
  
  // for(int i=0;i<CTRLBTNNUM-1;i++){
  //   screen.fillRoundRect(258,55*i+20,60,53,2,TFT_DARKGRAY);
  // }

  for(int j=0;j<2;j++){
    for(int i=0;i<2;i++){
    screen.fillRoundRect(30*i+258,52*j+20,28,50,2,TFT_DARKGRAY);
    }
  }

  screen.fillRoundRect(258,124,58,38,2,TFT_LIGHTGRAY);

  screen.fillRoundRect(258,   164,28,38,2,TFT_LIGHTGRAY);
  screen.fillRoundRect(258+30,164,28,38,2,TFT_LIGHTGRAY);

  screen.fillRoundRect(258,   204,58,38,2,TFT_LIGHTGRAY);
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
  // readmapno = mn;

  // if(readmapno == 0)mapFileName = "/init/map/0.png";
  // if(readmapno == 1)mapFileName = "/init/map/1.png";

  // mapFileName = "/init/map/0.png";

// 外マップ＆スプライト---------------------------------------------
// divnum回に分けて読み込む

// for(int i= 0;i<16;i++){//マップを描くときに使うスプライト番号リストを読み込む
//   Serial.print("/");
//   Serial.print(mapsprnos[i]);
// }
// Serial.println("");

  for(int n = 0; n<divnum; n++)
  {
    spriteMap.drawPngFile( SPIFFS, mapFileName, 0, (int32_t)(-MAPWH*n/divnum) );
    for(int j = 0; j<MAPWH/divnum; j++){
      for(int i = 0; i<MAPWH; i++){

        int k = j+(MAPWH/divnum)*(n);//マップ下部
        colValR = spriteMap.readPixelRGB(i,j).R8();
        colValG = spriteMap.readPixelRGB(i,j).G8();
        colValB = spriteMap.readPixelRGB(i,j).B8();

        Serial.print(colValR);
        Serial.print(":");
        Serial.print(colValG);
        Serial.print(":");
        Serial.println(colValB);
//24ビットRGB
// { 0,0,0},//0: 黒色
// { 27,42,86 },//1: 暗い青色
// { 137,24,84 },//2: 暗い紫色
// { 0,139,75 },//3: 暗い緑色
// { 183,76,45 },//4: 茶色
// { 97,87,78 },//5: 暗い灰色
// { 194,195,199 },//6: 明るい灰色
// { 255,241,231 },//7: 白色
// { 255,0,70 },//8: 赤色
// { 255,160,0 },//9: オレンジ
// { 255,238,0 },//10: 黄色
// { 0,234,0 },//11: 緑色
// { 0,173,255 },//12: 水色
// { 134,116,159 },//13: 藍色
// { 255,107,169 },//14: ピンク
// { 255,202,165}//15: 桃色
// mapsprno.txt
// 20,11,32,44,53,49,54,32,52,41,46,42,45,50,43,38

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
    // mapready = true;
    // return 1;
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
  Serial.begin(115200);
  delay(50);
  // タイマー作成(33.333ms)
  // timer = timerBegin(0, 80, true);
  // timerAttachInterrupt(timer, &onTimer, true);
  // timerAlarmWrite(timer, 1000000, true);
  // timerAlarmEnable(timer);

  // timerA = timerBegin(0, 80, true);//カウント時間は1マイクロ秒//hw_timer_t*オブジェクト(タイマーハンドラ）がかえってくる
  // timerAttachInterrupt(timerA, &onTimerA, true);//タイマー割り込みが発生したときに実行する関数を登録する。timerA =フレームタイマー
  // timerAlarmWrite(timerA, 1000000, true);//タイマーの設定値(割り込みのタイミング)を設定する。1ui.be/30秒　＝ 33333uSec　1/15秒　＝ 66666uSec
  // timerAlarmEnable(timerA);
  // delay(10);

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

  char text[32];

  sprintf(text, "%d", ui.getCalData(0));sprintf(text, "%d", ",");
  sprintf(text, "%d", ui.getCalData(1));sprintf(text, "%d", ",");
  sprintf(text, "%d", ui.getCalData(2));sprintf(text, "%d", ",");
  sprintf(text, "%d", ui.getCalData(3));sprintf(text, "%d", ",");
  sprintf(text, "%d", ui.getCalData(4));sprintf(text, "%d", ",");
  sprintf(text, "%d", ui.getCalData(5));sprintf(text, "%d", ",");
  sprintf(text, "%d", ui.getCalData(6));sprintf(text, "%d", ",");
  sprintf(text, "%d", ui.getCalData(7));sprintf(text, "%d", ",");

  // writeFile(SPIFFS, "/init/caldata.txt",text);

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
  
  sprite88_roi.setPsram(false );
  sprite88_roi.setColorDepth(16);//子スプライトの色深度
  sprite88_roi.createSprite(8, 8);//ゲーム画面用スプライトメモリ確保

  spriteMap.setPsram(false );
  spriteMap.setColorDepth(16);//子スプライトの色深度
  spriteMap.createSprite(MAPWH, MAPWH/divnum);//マップ展開用スプライトメモリ確保

  //sprite（bg1)のボタン配置の時
  ui.createBtns( 130,  0,  30,   8,  1, 1, TOUCH, 2);//ホームボタン
  // ui.createBtns( 130,  9,  30, 111,  1, 4, TOUCH, 2);//コントローラー4ボタン
  ui.createBtns( 130,  81,  30, 20,  2, 1, TOUCH, 2);//コントローラー4ボタン
  ui.createBtns( 130,  61,  30, 20,  1, 1, TOUCH, 2);//コントローラー4ボタン
  ui.createBtns( 130,  101, 30, 20,  1, 1, TOUCH, 2);//コントローラー4ボタン
  ui.createBtns( 130,  9,   30, 52,  2, 2, TOUCH, 2);//コントローラー4ボタン

    // if(optionuiflag == true){
    // ui.createSliders( 0, 100, 128, 20, 1, 1, sliderSprite, XY_VAL, MULTI_EVENT, 2);
    // sliderSprite.fillScreen(TFT_RED);
    // ui.setBtnName( ui.getUiID("SLIDER_5"), "2DSlider0" );
  // }

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
}

void loop()
{

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

  if(mode != 0){ // exit request//次のゲームを立ち上げるフラグがた値、modeが１次のゲームを起動であれば
    tunes.pause();
    game->pause();
    free(game);
    txtName = appfileName;
    game = nextGameObject(&appfileName, gameState, mapFileName);//ファイルの種類を判別して適したゲームオブジェクトを生成
    game->init();//resume()（再開処理）を呼び出し、ゲームで利用する関数などを準備
    tunes.resume();
  }

  if(optionuiflag == true){
  // tft.fillRect(int(sliderval[0]*128), 90, 10,10,TFT_RED); 
  // ui.drawSliders( ui.getUiID("SLIDER_5"), tft, sliderSprite, 0, 100);
  }

  // == display update ==
  // tft.setCursor(0,120);
  // tft.setTextColor(0xffff);
  // tft.print(ESP.getFreeHeap());

     //ゲーム
    // uint32_t now = millis();
    // uint32_t remainTime = (now - preTime);

    // preTime = now;
    // tunes.run();
    // int mode = game->run(remainTime);



      // if(flip){

        // tft.print(charSpritex);
        // tft.print(":");
        // tft.println(charSpritey);
        
        // tft.print(colValR);
        // tft.print(":");
        // tft.print(colValG);
        // tft.print(":");
        // tft.println(colValB);

        // tft.print("SPIFFS Read:");	// ⑬シリアルモニタにEEPROM内容表示
        // tft.println(readStr);

        // tft.pushSprite(&layoutSprite,0,0);

        // ui.showFPS( tft, 0, 127 - 28);//タッチイベントを視覚化する
        ui.showTouchEventInfo( tft, 0, 100 );//タッチイベントを視覚化する
        ui.showInfo( tft, 0, 100+8 );//ボタン情報、フレームレート情報などを表示します。
        //ui.drawPhBtns( tft, 0, 90+16 );//物理ボタンの状態を表示

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

}
