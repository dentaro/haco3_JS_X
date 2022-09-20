#include <Arduino.h>
#include <FS.h>
#include <SPI.h>
#include "SD.h"
#include "SPIFFS.h"
#include <LovyanGFX_DentaroUI.hpp>
#include "Tunes.h"
#include "runJsGame.h"

uint8_t xpos, ypos = 0;
// int bgSpriteNo = 0;
uint8_t colValR = 0;
uint8_t colValG = 0;
uint8_t colValB = 0;

uint8_t charSpritex = 0;
uint8_t charSpritey = 0;

uint8_t mapsx = 0;
uint8_t mapsy = 0;

String mapFileName = "";
int readmapno = 0;

int divnum = 128;
bool readMapF = true;
//divnumが大きいほど少ない領域で展開できる(2の乗数)

File fw;
File fr;
String readStr = "";
String wrfile;	// ④読み書きするファイル名を設定
String writeStr;

#define LOG_FILE_NAME "/config.csv"
static File s_myFile;

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
float matrix_bg[6] = {2.0,   // 横2倍
                     -0.0,  // 横傾き
                     0.0,   // X座標
                     0.0,   // 縦傾き
                     2.0,   // 縦2倍
                     0.0    // Y座標
                    };

LGFX screen;//LGFXを継承

LovyanGFX_DentaroUI ui(&screen);

static LGFX_Sprite sideSprite( &screen );//背景スプライトはディスプレイに出力
LGFX_Sprite layoutSprite(&screen);//LGFX_Spriteを継承 2倍するため
LGFX_Sprite tft = LGFX_Sprite(&layoutSprite);//LGFX_Spriteを継承

LGFX_Sprite spriteRoi = LGFX_Sprite(&tft);

LGFX_Sprite sprite88_0 = LGFX_Sprite(&tft);

LGFX_Sprite spritebg[16];//16種類のスプライトを背景で使えるようにする
LGFX_Sprite spriteMap;

uint8_t mapArray[256][256];
// RGBColor mapArray[256][256];

bool mapready = false;

// RunLuaGame*　game;
BaseGame* game;
// String fileName = "/init/main.lua";
String fileName = "/init/main.js";
// WifiGame* wifiGame = NULL;
Tunes tunes;


hw_timer_t * timerA = NULL;//スピーカー用
// extern void onTimerA();
volatile static boolean timer_flag = false;
//高速
void IRAM_ATTR onTimerA() {
  timer_flag != timer_flag;
}

void startWifiDebug(bool isSelf){
  // tunes.pause();
  // wifiGame = new WifiGame();
  // wifiGame->init(isSelf);
  // tunes.resume();
}
char buf[50];
char *A;
bool flip = false;

uint32_t preTime;
void setFileName(String s){
  fileName = s;
}
// bool isWifiDebug(){
//   return wifiGame != NULL;
// }
void reboot(){
//   wifiGame->pause();
  ESP.restart();
}

// タイマー
hw_timer_t * timer = NULL;

// 画面描画タスクハンドル
TaskHandle_t taskHandle;
//描画待ち用セマフォ
volatile SemaphoreHandle_t semaphore;

//ボタンイベント用
// TaskHandle_t taskHandle2;
volatile SemaphoreHandle_t semaphore2;


int readMap(int mn)
{

  spriteMap.setPsram(false );
  spriteMap.setColorDepth(16);//子スプライトの色深度
  spriteMap.createSprite(256, 256/divnum);//マップ展開用スプライトメモリ確保

  mapready = false;
  readmapno = mn;

  if(readmapno == 0)mapFileName = "/init/town.png";
  else if(readmapno == 1)mapFileName = "/init/map.png";
  
// 外マップ＆スプライト---------------------------------------------
// divnum回に分けて読み込む

  for(int n = 0; n<divnum; n++){

    spriteMap.drawPngFile( SPIFFS, mapFileName, 0, (int32_t)(-256*n/divnum) );

    for(int j = 0; j<256/divnum; j++){
      for(int i = 0; i<256; i++){

        int k = j+(256/divnum)*(n);//マップ下部
        colValR = spriteMap.readPixelRGB(i,j).R8();
        colValG = spriteMap.readPixelRGB(i,j).G8();
        colValB = spriteMap.readPixelRGB(i,j).B8();

        if(colValR==0&&colValG==174&&colValB==255){//水色＝0海
          mapArray[i][k] = 0;//rand() % 7 + 1;
        }else if(colValR==0&&colValG==235&&colValB==0){//緑＝1草
          mapArray[i][k] = 1;
        }else if(colValR==132&&colValG==117&&colValB==156){//紫＝2森
          mapArray[i][k] = 2;
        }else if(colValR==0&&colValG==138&&colValB==74){//濃い緑＝3しげみ
          mapArray[i][k] = 3;
        }else if(colValR==255&&colValG==239&&colValB==0){//黄色＝4砂地
          mapArray[i][k] = 4;
        }else if(colValR==255&&colValG==162&&colValB==0){//オレンジ＝5山
          mapArray[i][k] = 5;
        }else if(colValR==99&&colValG==85&&colValB==74){//灰＝6岩山
          mapArray[i][k] = 6;
        }else if(colValR==255&&colValG==243&&colValB==231){//白＝7氷
          mapArray[i][k] = 7;
        }
        // if(i==255 && k==255){mapready = true;return 1;}//読み込み終わったら1をリターン
      }
    }
  }
      
  spriteMap.deleteSprite();//メモリに格納したら解放する

  mapready = true;
  return 1;

}

void readMapFsw(bool b){
  readMapF = b;
}

// 画面描画タスク
void dispTask(void *pvParameters) {
  while (1) {
  xSemaphoreTake(semaphore, portMAX_DELAY);
  
  pressedBtnID = -1;//リセット
  ui.updatePhBtns();//物理ボタンの状態を更新
  ui.update(screen);//タッチイベントを取るので、LGFXが基底クラスでないといけない
  if( ui.getEvent() != NO_EVENT ){//何かイベントがあれば
    if( ui.getEvent() == TOUCH ){//TOUCHの時だけ
    }
    if(ui.getEvent() == MOVE){
      pressedBtnID = ui.getTouchBtnID();
    }
    if( ui.getEvent() == RELEASE ){//RELEASEの時だけ
      pressedBtnID = ui.getTouchBtnID();
    }
  }


    if(readMapF == true){

      fr = SPIFFS.open(wrfile.c_str(), "r");// ⑩ファイルを読み込みモードで開く
      // while(fr == NULL){};
      readStr = fr.readStringUntil('\n');// ⑪改行まで１行読み出し
      fr.close();	// ⑫	ファイルを閉じる
      // readmapno = atoi(readStr.c_str());

      
      //マップ番号が変わっている時だけよみこむ
      if(readmapno != atoi(readStr.c_str()))
      {

      fw = SPIFFS.open(wrfile.c_str(), FILE_WRITE);// ⑥ファイルを書き込みモードで開く
      // while(fw == NULL){};
      fw.println( String(readmapno) );	// ⑦ファイルに書き込み
      fw.close();	// ⑧ファイルを閉じる

      tunes.pause();//一旦とめてないとできない
      game->pause();
      free(game);
      game = new RunJsGame();//JS版リセットしてからだと通る
      readMap(readmapno);
      game->init();
      tunes.resume();//再開
      }
    }

     //ゲーム
    uint32_t now = millis();
    uint32_t remainTime = (now - preTime);

    preTime = now;

    // if(readMapF == false){
      tunes.run();
      int mode = game->run(remainTime);
    // }

    //違うゲームが選ばれた時はこの処理を通る？
    // if(mode != 0){
      
    //   tunes.pause();//一旦とめて
    //   game->pause();
    //   free(game);
    //   game = new RunJsGame();//新しいゲームオブジェクトを作る
    //   game->init();//初期化して
    //   tunes.resume();//再開
    // }


      if(flip){
        tft.setTextSize(1);
        tft.setFont(&lgfxJapanGothicP_8);
        tft.setTextColor( TFT_WHITE , TFT_BLACK );
        tft.setCursor( 0,0 );

        tft.print(charSpritex);
        tft.print(":");
        tft.println(charSpritey);
        
        tft.print(colValR);
        tft.print(":");
        tft.print(colValG);
        tft.print(":");
        tft.println(colValB);

        tft.print("SPIFFS Read:");	// ⑬シリアルモニタにEEPROM内容表示
        tft.println(readStr);

        tft.pushSprite(&layoutSprite,0,0);

        // if(readMapF == true){
        //   readMap(1);
        //   readMapF = false;
        // }

        ui.showFPS( layoutSprite, 0, 127 - 28);//タッチイベントを視覚化する
        // ui.showTouchEventInfo( layoutSprite, 120, 0 );//タッチイベントを視覚化する
        // ui.showInfo( layoutSprite, 0, 0 );//ボタン情報、フレームレート情報などを表示します。
        // ui.drawPhBtns( layoutSprite );//物理ボタンの状態を表示
        layoutSprite.pushAffine(matrix_bg);
        readMapFsw(false);//描画が終わったら必ずfalse
      }else{

      }
      flip = !flip;//描画は2フレにつき１回行う
    }
    
    delay(1);
}

// タイマー割り込み
void IRAM_ATTR onTimer() {
  xTaskNotifyFromISR(taskHandle, 0, eIncrement, NULL);
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

  // バイナリセマフォ作成
  semaphore = xSemaphoreCreateBinary();

  // 描画用タスク作成APP_CPU
  xTaskCreateUniversal(
    dispTask,
    "dispTask",
    8192,
    NULL,
    2,
    &taskHandle,
    APP_CPU_NUM
  );

  timerA = timerBegin(0, 80, true);//カウント時間は1マイクロ秒//hw_timer_t*オブジェクト(タイマーハンドラ）がかえってくる
  timerAttachInterrupt(timerA, &onTimerA, true);//タイマー割り込みが発生したときに実行する関数を登録する。timerA =フレームタイマー
  timerAlarmWrite(timerA, 1000000, true);//タイマーの設定値(割り込みのタイミング)を設定する。1/30秒　＝ 33333uSec　1/15秒　＝ 66666uSec
  timerAlarmEnable(timerA);
  delay(10);

  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  wrfile = "/config.txt";	// ④読み書きするファイル名を設定
  // writeStr = "1";	// ⑤書き込み文字列を設定

  fr = SPIFFS.open(wrfile.c_str(), "r");// ⑩ファイルを読み込みモードで開く
  // while(fr == NULL){};  
  readStr = fr.readStringUntil('\n');// ⑪改行まで１行読み出し
  fr.close();	// ⑫	ファイルを閉じる
  readmapno = atoi(readStr.c_str());
  

  // fw = SPIFFS.open(wrfile.c_str(), FILE_WRITE);// ⑥ファイルを書き込みモードで開く
  // // while(fw == NULL){};
  // fw.println( String(readmapno) );	// ⑦ファイルに書き込み
  // fw.close();	// ⑧ファイルを閉じる

  // Serial.print("SPIFFS Write:");	// ⑨書き込み完了をモニタに表示
  // Serial.println(writeStr);
  // delay(500);

  // tft.print("SPIFFS Read:");	// ⑬シリアルモニタにEEPROM内容表示
  // tft.println(readStr);

  //screenは16ビットカラー,回転方向１,タッチキャリブレーションtrue
  ui.begin( screen, 16, 1, true);

  sprite88_0.setPsram(false );
  sprite88_0.setColorDepth(16);//子スプライトの色深度
  sprite88_0.createSprite(8, 8);//ゲーム画面用スプライトメモリ確保
  sprite88_0.drawPngFile(SPIFFS, "/init/sprite.png", -8*1, -8*0);

  //maproi
  spriteRoi.setPsram(false );
  spriteRoi.setColorDepth(16);//子スプライトの色深度
  spriteRoi.createSprite(16, 15);//ゲーム画面用スプライトメモリ確保
  // spriteRoi.drawPngFile(SPIFFS, "/init/map.png", -161, -211);

  //外マップスプライトセット
  // // setupbg( 5,10, 11,10)//0海、1草
  // // setupbg(13,10, 10,10)//2森、3茂み
  // // setupbg( 3,10, 11,11)//4砂、5山
  // // setupbg(13,11,  9,10)//6岩山、7氷 @
  // int sx_[8] = { 5,11,13,10, 3,11,13, 9};
  // int sy_[8] = {10,10,10,10,10,11,11,10};

//町マップスプライトセット
  // setupbg( 5,10, 11,10)//0水、1草
  // setupbg(12,11, 10,10)//2木、3茂み
  // setupbg( 3,10, 12,10)//4砂、5花
  // setupbg( 2,10,  9,10)//6壁、7なし 
  // setupbg( 9,11,  9,10)//8道、9マスク 

  int sx_[10] = { 5,11,12,10, 3,12, 2, 9, 9, 9};
  int sy_[10] = {10,10,11,10,10,10,10,10,11,10};

  for(int i=0;i<sizeof(sx_)/sizeof(sx_[0]);i++)
  {
    spritebg[i].setPsram( false );
    spritebg[i].setColorDepth(16);//子スプライトの色深度
    spritebg[i].createSprite(8, 8);//ゲーム画面用スプライトメモリ確保
    spritebg[i].drawPngFile(SPIFFS, "/init/sprite.png", -8*sx_[i], -8*sy_[i]);
  }

  // spriteMap.setPsram(false );
  // spriteMap.setColorDepth(16);//子スプライトの色深度
  // spriteMap.createSprite(256, 256/divnum);//マップ展開用スプライトメモリ確保

  // readMap(0);

  sideSprite.setPsram(false );
  sideSprite.setColorDepth(16);//子スプライトの色深度
  sideSprite.createSprite(31, 120);//ゲーム画面用スプライトメモリ確保
  sideSprite.fillScreen(TFT_RED);
// //  //SDからの読み込みは４Gb以下じゃないとうまく動作しないかも
// //PNGをバッファに書いて2倍出力
  sideSprite.drawPngFile(SPIFFS, "/haco3/util/side.png", 0, 0);//4ボタン
  // sideSprite.drawPngFile(SPIFFS, "/haco3/util/side0.png", 0, 0);//
  sideSprite.pushAffine(matrix_side);
  sideSprite.deleteSprite();//描画したらメモリを解放する

  //sprite（bg1)のボタン配置の時
  ui.createBtns( 130,  9,  30, 111,  1, 4, TOUCH, 2);//コントローラー4ボタン
  
  //sprite（bg1)のボタン配置の時
  // ui.createBtns( 130,  9,  30, 44,  3, 4, TOUCH, 2);//コントローラー
  // ui.createBtns( 130, 53,  30, 66,  2, 6, TOUCH, 2);//メインメニュー

  // ui.createBtns( 3,   66,  66, 11,  6, 1, TOUCH, 2);//最後の引数でタッチエリアを2倍している
  // ui.createBtns( 97,  66,  32, 11,  4, 1, TOUCH, 2);//最後の引数でタッチエリアを2倍している

  // ui.createBtns( 1, 77,    128, 32, 16, 4, TOUCH, 2);//最後の引数でタッチエリアを2倍している
  // ui.createBtns( 1, 1,     64, 64,  8, 8, TOUCH, 2);//最後の引数でタッチエリアを2倍している

  // ui.createBtns( 69, 6,    40, 40,  4, 4, TOUCH, 2);//最後の引数でタッチエリアを2倍している
  // ui.createBtns( 109, 3,   20, 46,  2, 5, TOUCH, 2);//最後の引数でタッチエリアを2倍している
  // ui.createBtns( 66, 48,   64, 18,  8, 1, TOUCH, 2);//最後の引数でタッチエリアを2倍している

  delay(100);

  layoutSprite.setPsram( false );//DMA利用のためPSRAMは切る
  layoutSprite.createSprite( 128, 128 );
  tft.setPsram( false );//DMA利用のためPSRAMは切る
  tft.createSprite( 128, 128 );
  // screen.startWrite();//CSアサート開始
  layoutSprite.startWrite();//CSアサート開始
  // tft.startWrite();//CSアサート開始

  fw = SPIFFS.open(wrfile.c_str(), FILE_WRITE);// ⑥ファイルを書き込みモードで開く
  while(fw == NULL){};
  fw.println( String( readmapno ) );  // ⑦ファイルに書き込み
  fw.close(); // ⑧ファイルを閉じる

  game =  new BaseGame();//これしか通らない
  game->pause();
  free(game);
  game = new RunJsGame();//JS版リセットしてからだと通る
  
  readMap(readmapno);
  game->init();
  tunes.init();

  // ui.setupPhBtns( screen, 33, 39, 36);//物理ボタンをセットアップ
}

void loop()
{
  // pressedBtnID = -1;//リセット
  // ui.updatePhBtns();//物理ボタンの状態を更新
  // ui.update(screen);//タッチイベントを取るので、LGFXが基底クラスでないといけない
  // if( ui.getEvent() != NO_EVENT ){//何かイベントがあれば
  //   if( ui.getEvent() == TOUCH ){//TOUCHの時だけ
  //   }
  //   if(ui.getEvent() == MOVE){
  //     pressedBtnID = ui.getTouchBtnID();
  //   }
  //   if( ui.getEvent() == RELEASE ){//RELEASEの時だけ
  //     pressedBtnID = ui.getTouchBtnID();
  //   }
  // }

  switch (ui.getHitValue())
  {
  default:  layoutSprite.println("--"); break;
  case 101: layoutSprite.println("A click");pressedBtnID = 2; break;
  case 102: layoutSprite.println("B click");pressedBtnID = 0; break;
  case 103: layoutSprite.println("C click");pressedBtnID = 3; break;
  case 111: layoutSprite.println("A hold");pressedBtnID = 2; break;
  case 112: layoutSprite.println("B hold");pressedBtnID = 1; break;
  case 113: layoutSprite.println("C hold");pressedBtnID = 3; break;
  case 121: layoutSprite.println("A double click");pressedBtnID = 2; break;
  case 122: layoutSprite.println("B double click");pressedBtnID = 1; break;
  case 123: layoutSprite.println("C double click");pressedBtnID = 3; break;
  case 201: layoutSprite.println("AB hold"); break;
  case 202: layoutSprite.println("AC hold"); break;
  case 203: layoutSprite.println("BC hold"); break;
  case 204: layoutSprite.println("ABC hold"); break;
  case 301: layoutSprite.println("A->B->C"); break;
  }


  xSemaphoreGiveFromISR(semaphore, NULL);
}
