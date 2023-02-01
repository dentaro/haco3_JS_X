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

#include <JPEGDecoder.h>

#include <Arduino.h>
#include <FS.h>
#include "SPIFFS.h"
#include "runJsGame.h"
#include "runLuaGame.h"

#include "Tunes.h"
#include "runJsGame.h"
#include "wifiGame.h"

#define FORMAT_SPIFFS_IF_FAILED true

WifiGame* wifiGame = NULL;
Tunes tunes;

uint8_t xpos, ypos = 0;
uint8_t colValR = 0;
uint8_t colValG = 0;
uint8_t colValB = 0;

uint8_t charSpritex = 0;
uint8_t charSpritey = 0;
int pressedBtnID = -1;//この値をタッチボタン、物理ボタンの両方から操作してbtnStateを間接的に操作している

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
// LGFX_Sprite tft(&screen);
// LGFX_Sprite layoutSprite(&screen);//LGFX_Spriteを継承 2倍するため
// LGFX_Sprite tft = LGFX_Sprite(&layoutSprite);//LGFX_Spriteを継承
// LGFX_Sprite spriteRoi = LGFX_Sprite(&tft);
LGFX_Sprite sprite88_0 = LGFX_Sprite(&tft);
// LGFX_Sprite spritebg[16];//16種類のスプライトを背景で使えるようにする
BaseGame* game;
// String appfileName = "/init/main.js";
// String appfileName = "/init/main.lua";//最初に実行されるファイル名
String appfileName = "/soundjs/main.js";//最初に実行されるファイル名

String caldatafile = "/init/caldata.txt";
String txtName = "/init/txt/sample.txt";//実行されるファイル名

// Tunes tunes;
// bool constantGetF = false;

char buf[MAX_CHAR];
char str[100];//情報表示用

// #define BUZZER_PIN 25
//音階名と周波数の対応

float TONES[14] = 
{ 261.6//C4
, 277.18//C_4
, 293.665//D4
, 311.127//D_4
, 329.63//E4
, 349.228//F4
, 369.994//F_4
, 391.995//G4
, 415.305//G_4
, 440//A4
, 466.164//A_4
, 493.883//B4
, 523.251//C5
, 0//NOTONE
};

enum struct FileType {
  LUA,
  JS,
  BMP,
  PNG,
  TXT,
  OTHER
};

void drawLogo(){
  logoSprite.setPsram(false);
  logoSprite.setColorDepth(16);                // 子スプライトの色深度
  logoSprite.createSprite(30, 10); // ゲーム画面用スプライトメモリ確保
  //  //SDからの読み込みは４Gb以下じゃないとうまく動作しないかも
  //PNGをバッファに書いて2倍出力
  logoSprite.drawPngFile(SPIFFS, "/init/logo.png", 0, 0); // 4ボタン
  // logoSprite.drawPngFile(SPIFFS, "/haco3/util/side0.png", 0, 0);//
  logoSprite.pushAffine(matrix_side);
  logoSprite.deleteSprite(); // 描画したらメモリを解放する

  screen.setTextSize(2);
  // screen.setFont(&lgfxJapanGothicP_8);
  screen.setTextColor( TFT_WHITE , TFT_DARKGRAY );
  screen.setCursor( 258,0 );
  
  for(int i=0;i<CTRLBTNNUM-1;i++){
    screen.fillRoundRect(258,55*i+20,60,53,2,TFT_DARKGRAY);
    // screen.fillTriangle()
    // screen.println("<");
    // screen.println("^");
    // screen.println("V");
  }
}

bool isWifiDebug(){
  return wifiGame != NULL;
}
void reboot(){
  wifiGame->pause();
  ESP.restart();
}

FileType detectFileType(String *appfileName){
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

BaseGame* nextGameObject(String* _appfileName){
  switch(detectFileType(_appfileName)){
    case FileType::JS:  game = new RunJsGame(); break;
    case FileType::LUA: game = new RunLuaGame(); break;
    case FileType::TXT: 
      game = new RunJsGame(); 
      //ファイル名がもし/init/caldata.txtなら
      if(*_appfileName == CALIBRATION_FILE){
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

void tone(int _toneNo, int _tonelength){
  ledcWriteTone(1,TONES[_toneNo]);
  delay(_tonelength);
  ledcWriteTone(1, 0);    // ならしたら最後に消音
}

hw_timer_t * timerA = NULL;//スピーカー用
// extern void onTimerA();
volatile static boolean timer_flag = false;
//高速
void IRAM_ATTR onTimerA() {
  timer_flag != timer_flag;
}

// char buf[50];
char *A;
bool flip = true;

uint32_t preTime;
void setFileName(String s){
  appfileName = s;
}

// void reboot(){
//   ESP.restart();
// }

// タイマー
hw_timer_t * timer = NULL;

// 画面描画タスクハンドル
TaskHandle_t taskHandle;
//描画待ち用セマフォ
// volatile SemaphoreHandle_t semaphore;

//ボタンイベント用
// TaskHandle_t taskHandle2;
// volatile SemaphoreHandle_t semaphore2;

// 画面描画タスク
// void dispTask(void *pvParameters) {
//   while (1) {
//   xSemaphoreTake(semaphore, portMAX_DELAY);

//   // playmusic();
//     }
//     // delay(1);
// }

// タイマー割り込み
void IRAM_ATTR onTimer() {
  xTaskNotifyFromISR(taskHandle, 0, eIncrement, NULL);
}

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

void setup()
{
  Serial.begin(115200);
  delay(50);
  // タイマー作成(33.333ms)
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);

  // ledcSetup(1,12000, 8);
  // ledcAttachPin(BUZZER_PIN,1);

  // バイナリセマフォ作成
  // semaphore = xSemaphoreCreateBinary();

  // 描画用タスク作成APP_CPU
  // xTaskCreateUniversal(
  //   dispTask,
  //   "dispTask",
  //   8192,
  //   NULL,
  //   2,
  //   &taskHandle,
  //   APP_CPU_NUM
  // );

  timerA = timerBegin(0, 80, true);//カウント時間は1マイクロ秒//hw_timer_t*オブジェクト(タイマーハンドラ）がかえってくる
  timerAttachInterrupt(timerA, &onTimerA, true);//タイマー割り込みが発生したときに実行する関数を登録する。timerA =フレームタイマー
  timerAlarmWrite(timerA, 1000000, true);//タイマーの設定値(割り込みのタイミング)を設定する。1ui.be/30秒　＝ 33333uSec　1/15秒　＝ 66666uSec
  timerAlarmEnable(timerA);
  delay(10);

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
  appfileName = rFirstAppName("/init/firstAppName.txt");//最初に立ち上げるゲームのパスをSPIFFSのファイルfirstAppName.txtから読み込む

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
  sprite88_0.drawPngFile(SPIFFS, "/init/sprite.png", -8*1, -8*0);

  sprite64.setPsram(false );
  sprite64.setColorDepth(16);//子スプライトの色深度
  sprite64.createSprite(64, 64);//ゲーム画面用スプライトメモリ確保
  sprite64.drawPngFile(SPIFFS, "/init/initspr.png", 0, 0);
  
  sprite88_roi.setPsram(false );
  sprite88_roi.setColorDepth(16);//子スプライトの色深度
  sprite88_roi.createSprite(8, 8);//ゲーム画面用スプライトメモリ確保

  //sprite（bg1)のボタン配置の時
  ui.createBtns( 130,  0,  30,   8,  1, 1, TOUCH, 2);//ホームボタン
  ui.createBtns( 130,  9,  30, 111,  1, 4, TOUCH, 2);//コントローラー4ボタン
  delay(100);

  tft.setPsram( false );//DMA利用のためPSRAMは切る
  tft.createSprite( 128, 128 );
  tft.startWrite();//CSアサート開始
  
  tft.setTextSize(2);
  tft.setFont(&lgfxJapanGothicP_8);
  tft.setTextColor( TFT_WHITE , TFT_BLACK );
  tft.setCursor( 0,0 );

  game = nextGameObject(&appfileName);//ホームゲームを立ち上げる
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

    // if(ui.getTouchBtnID() == RELEASE){//リリースされたら
    //   pressedBtnID = -1;
    // }

  // tft.setTextSize(1);
  // tft.setTextColor(TFT_WHITE, TFT_RED);
  // tft.setCursor(0, 120);
  // tft.setTextWrap(true);
  // tft.print(pressedBtnID);
  // tft.setTextWrap(false);

  tft.setTextSize(1);//元に戻す
  
  uint32_t now = millis();
  uint32_t remainTime= (now - preTime);
  preTime = now;

  // == wifi task ==
  if(wifiGame){ // debug mode
    int r = wifiGame->run(remainTime);
    if(r != 0){ // reload request

      tunes.pause();
      game->pause();
      free(game);
      txtName = appfileName;
      game = nextGameObject(&appfileName);
      game->init();
      tunes.resume();
    }
  }
  // == tune task ==
  tunes.run();

  // == game task ==
  int mode = game->run(remainTime);//exitは1が返ってくる　mode=１ 次のゲームを起動

  //0ボタンで強制終了
  if (pressedBtnID == 0)
  { // reload
    ui.setConstantGetF(false);//初期化処理 タッチポイントの常時取得を切る
    appfileName = "/init/main.lua";
    mode = 1;//exit
  }

  if(mode != 0){ // exit request//次のゲームを立ち上げるフラグがた値、modeが１次のゲームを起動であれば
    tunes.pause();
    game->pause();
    free(game);
    txtName = appfileName;
    game = nextGameObject(&appfileName);//ファイルの種類を判別して適したゲームオブジェクトを生成
    game->init();//resume()（再開処理）を呼び出し、ゲームで利用する関数などを準備
    tunes.resume();
  }

  // == display update ==
  tft.setCursor(0,120);
  tft.setTextColor(0xffff);
  tft.print(ESP.getFreeHeap());

     //ゲーム
    // uint32_t now = millis();
    // uint32_t remainTime = (now - preTime);

    // preTime = now;
    // tunes.run();
    // int mode = game->run(remainTime);

    //違うゲームが選ばれた時はこの処理を通る？
    // if(mode != 0){
    //   tunes.pause();//一旦とめて
    //   game->pause();
    //   free(game);
    //   game = new RunJsGame();//新しいゲームオブジェクトを作る
    //   game->init();//初期化して
    //   tunes.resume();//再開
    // }

      // if(flip){
        // tft.setTextSize(1);
        // tft.setFont(&lgfxJapanGothicP_8);
        // tft.setTextColor( TFT_WHITE , TFT_BLACK );
        // tft.setCursor( 0,0 );

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

        // tft.pushSprite(&screen,0,0);//ゲーム画面を描画する

        tft.pushAffine(matrix_game);//ゲーム画面を最終描画する

        // delay(4);//120FPS スプライトが少ない、速度の速いモード描画ぶん待つ
        delay(10);//30FPS メニュー、パズルなど

  // int wait = 1000/60 - remainTime;//フレームレートを60FPS合わせる
  // if(wait > 0){
  //   delay(wait);
  // }
  // xSemaphoreGiveFromISR(semaphore, NULL);

}

// #include <LovyanGFX_DentaroUI.hpp>

// static LGFX tft;
// LovyanGFX_DentaroUI ui(&tft);
// static LGFX_Sprite sideSprite( &tft );//背景スプライトはディスプレイに出力
// static LGFX_Sprite canvas(&tft);
// static bool isCanvasCreated = false;
// static int canvasX;
// static int canvasY;

// void setup() {

//   ui.begin( tft, 16, 1, true);
//   ui.createBtns( 130,  9,  30, 111,  1, 4, TOUCH, 2);//コントローラー4ボタン

// }

// void loop() { 
//   ui.update(tft);
   
//    ui.showTouchEventInfo( tft, 0, 100 );//タッチイベントを視覚化する
//    ui.showInfo( tft, 0, 100+8 );//ボタン情報、フレームレート情報などを表示します。
  
//   // delay(1000); 
//   }