#include "runJsGame.h"
// #include <stdio.h>
extern MyTFT_eSprite tft;
extern LGFX_Sprite sprite64;
extern LGFX_Sprite sprite88_roi;
extern String appfileName;
extern String txtName;
extern void setFileName(String s);
extern void reboot();
extern void tone(int _toneNo , int _tonelength);
extern void wCalData(String _wrfile);
extern String rCalData(String _wrfile);
extern Tunes tunes;
extern int pressedBtnID;
extern uint8_t charSpritex;
extern uint8_t charSpritey;
extern volatile SemaphoreHandle_t semaphore;

extern File fw;
extern File fr;
extern String readStr;
extern String wrfile;	// ④読み書きするファイル名を設定
extern String writeStr;
bool moveF = true;

extern char buf[MAX_CHAR];
extern char str[100];

extern LGFX screen;
extern LovyanGFX_DentaroUI ui;
extern String *targetfileName;

extern int outputMode;

extern int mapsprnos[16];
extern int8_t sprbits[128];//8*16

char chr;
char numStr[4];
char fileStr[100];
bool tbtnSetupF = true;
int prebtnid = -1;

int RunJsGame::loadSurface(File *fp, uint8_t* buf){
  return 0;
}

duk_ret_t RunJsGame::l_tone(duk_context* ctx){
  duk_push_global_object(ctx);          // push global
  duk_get_prop_string(ctx, -1, "obako");// push obako
  RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
  duk_pop_2(ctx); // obako, global

  int n = duk_get_int(ctx, 0);
  int f = duk_get_int(ctx, 1);

  portENTER_CRITICAL(&Tunes::timerMux);
  Tunes::d[n] = (uint16_t)(3.2768*f);
  portEXIT_CRITICAL(&Tunes::timerMux);

  // portENTER_CRITICAL(&Tunes::timerMux);
  // Tunes::d[n] = (uint16_t)(3.2768*f);
  // portEXIT_CRITICAL(&Tunes::timerMux);

  return 0;
}

int RunJsGame::l_spr(duk_context* ctx){
  duk_push_global_object(ctx);          // push global
  duk_get_prop_string(ctx, -1, "obako");// push obako
  RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
  duk_pop_2(ctx); // obako, global

  int x = duk_get_int(ctx, 0);
  int y = duk_get_int(ctx, 1);
  int _w = duk_get_int(ctx, 2);
  int _h = duk_get_int(ctx, 3);
  int sx = duk_get_int(ctx, 4);
  int sy = duk_get_int(ctx, 5);

  for(int j=0;j<_h/8;j++){
      for(int i=0;i<_w/8;i++){
      sprite64.pushSprite(&sprite88_roi, -(sx+(i*8)), -(sy+(j*8)));//128*128のpngデータを指定位置までずらす
      sprite88_roi.pushSprite(&tft, x+(i*8), y+(j*8), TFT_BLACK);//16*16で切り抜き&tftに書き出し：黒を透明に
      }
  }
  return 0;
}

duk_ret_t RunJsGame::l_color(duk_context* ctx){
  duk_push_global_object(ctx);          // push global
  duk_get_prop_string(ctx, -1, "obako");// push obako
  RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
  duk_pop_2(ctx); // obako, global

  int r,g,b;

  if(duk_get_top(ctx) == 3){ // from rgb

    r = duk_get_int(ctx, 0);
    g = duk_get_int(ctx, 1);
    b = duk_get_int(ctx, 2);
    //とにかく一回格納する
    self->col[0] = r;
    self->col[1] = g;
    self->col[2] = b;

    //色番号だったら上書き
  if(g == NULL && b == NULL){

      int n = duk_get_int(ctx, 0);

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

    //   uint16_t col16 = rgb24to16(r, g, b);
    
    // self->col[0] = ((col16 >> 11) << 3); // 5bit
    // self->col[1] = (((col16  >> 5) & 0b111111) << 2); // 6bit
    // self->col[2] = ((col16  & 0b11111) << 3);       // 5bit

    self->col[0] = r; // 5bit
    self->col[1] = g; // 6bit
    self->col[2] = b;       // 5bit
    }

  }
  return 0;
}

duk_ret_t RunJsGame::l_pset(duk_context* ctx){
  duk_push_global_object(ctx);          // push global
  duk_get_prop_string(ctx, -1, "obako");// push obako
  RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
  duk_pop_2(ctx); // obako, global

  int x = duk_get_int(ctx, 0);
  int y = duk_get_int(ctx, 1);

  tft.drawPixel(x, y, rgb24to16(self->col[0], self->col[1], self->col[2]));
  return 0;
}

duk_ret_t RunJsGame::l_text(duk_context* ctx){

  duk_push_global_object(ctx);
  duk_get_prop_string(ctx, -1, "obako");
  RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
  duk_pop_2(ctx); // pointer, obako, global

  duk_size_t len;
  const char* text = duk_get_lstring(ctx, 0, &len);
  int x = duk_get_int(ctx, 1);
  int y = duk_get_int(ctx, 2);

  tft.setCursor(x,y);

  tft.setFont(&lgfxJapanGothicP_8);//日本語に対応する
  tft.setTextColor(rgb24to16(self->col[0], self->col[1], self->col[2]));
  tft.print(text);
  return 0;
}

int RunJsGame::l_drawcircle(duk_context* ctx){
  duk_push_global_object(ctx);
  duk_get_prop_string(ctx, -1, "obako");
  RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
  duk_pop_2(ctx); // pointer, obako, global

  int x = duk_get_int(ctx, 0);
  int y = duk_get_int(ctx, 1);
  int r = duk_get_int(ctx, 2);

  tft.drawCircle(x, y, r, rgb24to16(self->col[0], self->col[1], self->col[2]));
  return 0;
}

int RunJsGame::l_fillcircle(duk_context* ctx){
  duk_push_global_object(ctx);
  duk_get_prop_string(ctx, -1, "obako");
  RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
  duk_pop_2(ctx); // pointer, obako, global

  int x = duk_get_int(ctx, 0);
  int y = duk_get_int(ctx, 1);
  int r = duk_get_int(ctx, 2);

  tft.fillCircle(x, y, r, rgb24to16(self->col[0], self->col[1], self->col[2]));
  return 0;
}


int RunJsGame::l_opmode(duk_context* ctx){
  duk_push_global_object(ctx);
  duk_get_prop_string(ctx, -1, "obako");
  RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
  duk_pop_2(ctx); // pointer, obako, global
  outputMode = duk_get_int(ctx, 0);
  return 0;
}

int RunJsGame::l_drawrect(duk_context* ctx){
  duk_push_global_object(ctx);
  duk_get_prop_string(ctx, -1, "obako");
  RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
  duk_pop_2(ctx); // pointer, obako, global

  int x = duk_get_int(ctx, 0);
  int y = duk_get_int(ctx, 1);
  int w = duk_get_int(ctx, 2);
  int h = duk_get_int(ctx, 3);

  tft.myDrawRect(x, y, w, h, rgb24to16(self->col[0], self->col[1], self->col[2]));
  return 0;
}

duk_ret_t RunJsGame::l_tbtns(duk_context* ctx){
  duk_push_global_object(ctx);
  duk_get_prop_string(ctx, -1, "obako");
  RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
  duk_pop_2(ctx); // pointer, obako, global

  int x = duk_get_int(ctx, 0);
  int y = duk_get_int(ctx, 1);
  int w = duk_get_int(ctx, 2);
  int h = duk_get_int(ctx, 3);
  int nx = duk_get_int(ctx, 4);
  int ny = duk_get_int(ctx, 5);

  int uw = int(w/nx);
  int uh = int(h/ny);
  bool congetF = duk_get_boolean(ctx, 6);

  if(tbtnSetupF == true){//resume時に一回だけ処理
         if(outputMode == WIDE_MODE)ui.createBtns( x, y, w, h, nx, ny, TOUCH, 2);//ゲーム画面を最終描画した時のタッチの位置　最後の引数で2倍面積4倍
    else if(outputMode == FAST_MODE)ui.createBtns( x+TFT_OFFSET_X, y+TFT_OFFSET_Y, w, h, nx, ny, TOUCH);//ゲーム画面を小さく高速描画した時のタッチの位置
    // ui.createBtns( x, y, w, h, nx, ny, TOUCH, 2);
    // tft.drawRect((rand() % 10)*12,60,10,10,TFT_BLUE);
    tbtnSetupF = false;
  }

  // for(int j = 0; j < ny; j++){
  //   for(int i = 0; i < nx; i++){
  //     tft.drawRoundRect(int(uw*i + x), int(uh*j + y), uw, uh, 4, TFT_DARKGRAY);
  //   }
  // }

  if(congetF != NULL){
    ui.setConstantGetF(congetF);//trueだとタッチポイントのボタンIDを連続取得するモード
  }else{
    ui.setConstantGetF(false);//タッチスタートポイント通常モード
  }
  duk_push_uint(ctx, (lua_Integer)ui.getTouchBtnID());//TouchBtnIDをリターン
  
  return 1;
}

duk_ret_t RunJsGame::l_rwtb(duk_context* ctx){
  duk_push_global_object(ctx);
  duk_get_prop_string(ctx, -1, "obako");
  RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
  duk_pop_2(ctx); // pointer, obako, global
  int btnid = duk_get_int(ctx, 0);
  int tbmode = duk_get_int(ctx, 1);
  
      if(tbmode != NULL){
        duk_push_boolean(ctx, (duk_bool_t)ui.switchToggleVal(btnid,tbmode));//TouchBtnIDをリターン
      }else{
        duk_push_boolean(ctx, (duk_bool_t)ui.getToggleVal(btnid));
      }

  return 1;
}

int RunJsGame::l_fillrect(duk_context* ctx){
  duk_push_global_object(ctx);
  duk_get_prop_string(ctx, -1, "obako");
  RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
  duk_pop_2(ctx); // pointer, obako, global

  int x = duk_get_int(ctx, 0);
  int y = duk_get_int(ctx, 1);
  int w = duk_get_int(ctx, 2);
  int h = duk_get_int(ctx, 3);

  tft.fillRect(x, y, w, h, rgb24to16(self->col[0], self->col[1], self->col[2]));
  return 0;
}

int RunJsGame::l_btn(duk_context* ctx){
  duk_push_global_object(ctx);          // push global
  duk_get_prop_string(ctx, -1, "obako");// push obako
  RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
  duk_pop_2(ctx); // obako, global
  int n = duk_get_int(ctx, 0);
  // Serial.println((lua_Integer)self->buttonState[n]);
  duk_push_int(ctx, (lua_Integer)self->buttonState[n]);
  
  return 1;
}

int RunJsGame::l_tp(duk_context* ctx){
  duk_push_global_object(ctx);          // push global
  duk_get_prop_string(ctx, -1, "obako");// push obako
  RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
  duk_pop_2(ctx); // obako, global
  
  int n = duk_get_int(ctx, 0);
  self->tp[0] = ui.getPos().x/2;
  self->tp[1] = ui.getPos().y/2;
  // tft.fillRect(self->tp[0], self->tp[1], 5, 5, TFT_GREEN);
  duk_push_uint(ctx, (lua_Integer)self->tp[n]);//duk_pushでJSに値をリターンできる
  return 1;//１にしないといけない（duk_pushでJSに値をリターンできる
}

int RunJsGame::l_gtx(duk_context* ctx){
  duk_push_global_object(ctx);          // push global
  duk_get_prop_string(ctx, -1, "obako");// push obako
  RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
  duk_pop_2(ctx); // obako, global
  int n = duk_get_int(ctx, 0);

  if(appfileName == CALIBRATION_FILE){
    wCalData(CALIBRATION_FILE);//
    String readStr = rCalData(CALIBRATION_FILE);
    const char* text = readStr.c_str();
    duk_push_string(ctx, text);//duk_pushでJSに値をリターンできる
  }else{
    String readStr = rCalData(txtName);
    const char* text = readStr.c_str();
    duk_push_string(ctx, text);//duk_pushでJSに値をリターンできる
  }
  return 1;
}

int RunJsGame::l_str(duk_context* ctx){
  duk_push_global_object(ctx);          // push global
  duk_get_prop_string(ctx, -1, "obako");// push obako
  RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
  duk_pop_2(ctx); // obako, global
  int n = duk_get_int(ctx, 0);
  // chr = (char)n;
  sprintf(numStr, "%d", n);
  duk_push_string(ctx, numStr);//duk_pushでJSに値をリターンできる
  return 1;
}

String RunJsGame::getBitmapName(String s){
  int p = s.lastIndexOf("/");
  if(p == -1){
    p = 0;
  }
  return s.substring(0, p) + "/sprite.bmp";
}

String RunJsGame::getPngName(String s){
  int p = s.lastIndexOf("/");
  if(p == -1){
    p = 0;
  }
  return s.substring(0, p) + "/initspr.png";
}

int RunJsGame::l_bg(duk_context* ctx){
  duk_push_global_object(ctx);          // push global
  duk_get_prop_string(ctx, -1, "obako");// push obako
  RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
  duk_pop_2(ctx); // obako, global

  int x = duk_get_int(ctx, 0);
  int y = duk_get_int(ctx, 1);

  duk_size_t len;
  const char* src = duk_get_lstring(ctx, 2, &len);

  String bgName = src;

  // String bgname = duk_get_int(ctx, 1);

  // txtName = "/init/logo.png";

  if(src != NULL){
    // if(SPIFFS.exists(txtName)){
      tft.drawPngFile(SPIFFS, txtName, 0, 0);
    // }
  }else{
    // if(SPIFFS.exists(bgName)){
      tft.drawPngFile(SPIFFS, bgName, 0, 0);
    // }
  }

  // sprite64.pushSprite(&tft, x, y);

  return 0;
}

void RunJsGame::init(){
  this->resume();
}
void RunJsGame::pause(){
  duk_destroy_heap(ctx);
}
void RunJsGame::resume(){
  
  // char buf[MAX_CHAR];
  // char str[100];

  Serial.println("resume JS");

  ctx = duk_create_heap_default();
  duk_push_global_object(ctx);

  duk_push_pointer(ctx, this);
  duk_put_prop_string(ctx, -2, "obako");

  duk_idx_t fidx = duk_push_c_function(ctx, RunJsGame::l_tone, 2);
  duk_put_prop_string(ctx, -2, "tone");

  fidx = duk_push_c_function(ctx, l_spr, 6);
  duk_put_prop_string(ctx, -2, "spr");

  fidx = duk_push_c_function(ctx, l_bg, 3);
  duk_put_prop_string(ctx, -2, "bg");
  
  fidx = duk_push_c_function(ctx, l_pset, 2);
  duk_put_prop_string(ctx, -2, "pset");

  fidx = duk_push_c_function(ctx, l_color, 3);
  duk_put_prop_string(ctx, -2, "color");

  fidx = duk_push_c_function(ctx, l_text, 3);
  duk_put_prop_string(ctx, -2, "text");

  fidx = duk_push_c_function(ctx, l_drawrect, 4);
  duk_put_prop_string(ctx, -2, "drawrect");

  fidx = duk_push_c_function(ctx, l_fillrect, 4);
  duk_put_prop_string(ctx, -2, "fillrect");

  fidx = duk_push_c_function(ctx, l_drawcircle, 3);
  duk_put_prop_string(ctx, -2, "drawcircle");

  fidx = duk_push_c_function(ctx, l_fillcircle, 3);
  duk_put_prop_string(ctx, -2, "fillcircle");

  fidx = duk_push_c_function(ctx, l_tbtns, 7);
  duk_put_prop_string(ctx, -2, "tbtns");

  fidx = duk_push_c_function(ctx, l_rwtb, 2);
  duk_put_prop_string(ctx, -2, "rwtb");

  fidx = duk_push_c_function(ctx, l_btn, 1);
  duk_put_prop_string(ctx, -2, "btn");

  fidx = duk_push_c_function(ctx, l_tp, 1);
  duk_put_prop_string(ctx, -2, "tp");

  fidx = duk_push_c_function(ctx, l_str, 1);
  duk_put_prop_string(ctx, -2, "str");

  fidx = duk_push_c_function(ctx, l_gtx, 1);
  duk_put_prop_string(ctx, -2, "gtx");

  duk_pop(ctx); // pop global

  
  SPIFFS.begin(true);//SPIFFSを利用可能にする
  if(SPIFFS.exists(getPngName(appfileName))){
    sprite64.drawPngFile(SPIFFS, appfileName, 0, 0);
  }
  // Serial.println("SPIFFS begin");

  // if(SPIFFS.exists(getBitmapName(appappfileName))){
  //   File bmpFile = SPIFFS.open(getBitmapName(appfileName) , FILE_READ);
  //   Serial.println("bitmap load begin");
  //   Serial.println("bitmap load end");
  //   bmpFile.close();
  // }

  // Serial.println("loaded bitmap");

  // if(SPIFFS.exists(getPngName(appfileName))){
  //   sprite64.drawPngFile(SPIFFS, appfileName, 0, 0);
  // }
  //   Serial.println("loaded png");

  File fp = SPIFFS.open(appfileName, FILE_READ);
  // Serial.println("open file");

  tft.fillScreen(TFT_BLACK);

  while(fp.available()){
    fp.readStringUntil('\n').toCharArray(buf, MAX_CHAR);
    // Serial.println(buf);
    duk_push_string(ctx, buf);
  }
  duk_push_string(ctx, "\n");
  duk_insert(ctx, 0);
  Serial.println("pre join");
  duk_join(ctx, duk_get_top(ctx) - 1);
  Serial.println("join file");
  //Serial.println(duk_safe_to_string(ctx, -1));
  if(duk_peval(ctx) != 0){
    Serial.println("eval failed");
    Serial.println(duk_safe_to_string(ctx, -1));
  }
  Serial.println("eval success");
  duk_size_t len;
  const char* result = duk_get_lstring(ctx, 1, &len);
  Serial.println(result);

  duk_pop(ctx); // remove result

  Serial.println("js check finish");

  for(int i = 0; i < CTRLBTNNUM; i ++){//初期化
    buttonState[i] = 0;
  }

  tbtnSetupF = true;//初期化

  tft.pushSprite(0, 0);
  
}

int RunJsGame::run(int _remainTime){
    

  char str[100];
  char key;
  if(ui.getTouchBtnNum()>CTRLBTNNUM){
    // ui.clearAddBtns();//最初に生成する4つのコントロールボタン以外は消して初期化する
  }

  if(exitRequest){//次のゲームを起動するフラグがたったら
    exitRequest = false;//フラグをリセットして
    ui.clearAddBtns();
    return 1; // exit(1をリターンすることで、main.cppの変数modeを１にする)
  }

  // for(int i = 0; i <CTRLBTNNUM; i ++){//初期化
  //     buttonState[i] = false;
  //     if(pressedBtnID ==  i){
  //       buttonState[i]  = true;
  //     }
  // }

  //ボタンを押してからの経過フレームを返すための処理
  for(int i = 0; i < CTRLBTNNUM; i ++){
    if(ui.getEvent()==NO_EVENT){
      buttonState[i] = 0;
    }else{
      if(pressedBtnID ==  i){//押されたものだけの値をあげる
      buttonState[i] ++;
      }
    }
  }

// pressedBtnID = -1;

  if(runError){
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLUE);
    tft.setCursor(0, 0);
    tft.setTextWrap(true);
    tft.print(errorString);
    tft.setTextWrap(false);

    if(buttonState[5] == 10){ // reload
      return 1;
    }
    
    if(buttonState[6] == 10){ // reload
      setFileName("/init/main.js"); // TODO: lua?
      return 1;
    }
  }else{
    if(duk_peval_string(ctx, "loop()") != 0){
      Serial.print("loop() failed");
      Serial.println(duk_safe_to_string(ctx, -1));
    }
    duk_pop(ctx);
  }

  // int ccx = 98;
  // int ccy = 64;
  // int cr = 24;
  // tft.setColor(TFT_WHITE);
  // tft.fillCircle(ccx,ccy, cr);
  // tft.setColor(TFT_BLUE);
  // tft.drawCircle(ccx,ccy, cr);

  // if(abs(2222 - jsy_value)>100||abs(2190 - jsx_value)>100)//どれだけ動いたかを判定
  // {
  //   float radian = atan2(jsy_value-2222, 2190-jsx_value);
  //   float u = (PI/8);

  //   if(radian >= 0 && radian < u){
  //     pressedBtnID = 16;
  //   }else if(radian >= u && radian < 3*u){
  //     pressedBtnID = 20;
  //   }else if(radian >= 3*u && radian < 5*u){
  //     pressedBtnID = 18;
  //   }else if(radian >= 5*u && radian < 7*u){
  //     pressedBtnID = 22;
  //   }else if(radian >= 7*u && radian < 8*u){
  //     pressedBtnID = 15;
  //   }
  //   else if(radian <= 0 && radian > -u){
  //     pressedBtnID = 16;
  //   }else if(radian <= -u && radian > 3*-u){
  //     pressedBtnID = 21;
  //   }else if(radian <= 3*-u && radian > 5*-u){
  //     pressedBtnID = 17;
  //   }else if(radian <= 5*-u && radian > 7*-u){
  //     pressedBtnID = 19;
  //   }else if(radian <= 7*-u && radian > 8*-u){
  //     pressedBtnID = 15;
  //   }

  //   tft.drawLine(ccx,ccy,ccx+cos(radian)*cr,ccy+sin(radian)*cr);
  //   tft.setColor(TFT_RED);
  //   tft.fillCircle(ccx + (2190-jsx_value)/2190*cr, 
  //                  ccy + (jsy_value-2222)/2222*cr, 5);
  // }else{
  //   pressedBtnID = -1;
  //   tft.setColor(TFT_BLUE);
  //   tft.fillCircle(ccx,ccy, 5);
  // }

  // sprintf(str, "%02dx", jsx_value); // x
  // tft.setCursor(0, 127 - 32);
  // tft.print(str);

  // sprintf(str, "%02dy", jsy_value); // y
  // tft.setCursor(0, 127 - 24);
  // tft.print(str);

  tft.setTextSize(1);
  tft.setFont(&lgfxJapanGothicP_8);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  
  // show FPS
  sprintf(str, "%02dFPS", 1000/_remainTime); // FPS
  
  // tft.setFont(&lgfxJapanGothicP_8);

  tft.setCursor(72, 0);
  tft.print(str);

  sprintf(str, "%02dms", _remainTime); // ms
  tft.setCursor(72, 8);
  tft.print(str);

  // sprintf(str, "%02dv", vol_value); // vol
  // tft.setCursor(72, 16);
  // tft.print(str);

  return 0;
}

