#include "runJsGame.h"
extern MyTFT_eSprite tft;
extern LGFX_Sprite sprite88_0;
extern LGFX_Sprite spriteRoi;
extern LGFX_Sprite spritebg[8];
extern String fileName;
extern void setFileName(String s);
extern void reboot();
extern Tunes tunes;
extern int pressedBtnID;
extern uint8_t charSpritex;
extern uint8_t charSpritey;
int bgSpriteNo = 0;
extern uint8_t mapArray[256][256];
extern int readMap(int mn);
extern void readMapFsw(bool b);
extern int readmapno;
extern bool mapready;
extern bool readMapF;

extern uint8_t mapsx;
extern uint8_t mapsy;
// extern TaskHandle_t taskHandle2;
extern volatile SemaphoreHandle_t semaphore;
extern int getMapNo();
// extern volatile SemaphoreHandle_t semaphore2;

// extern bool readMapF;

extern File fw;
extern File fr;
extern String readStr;
extern String wrfile;	// ④読み書きするファイル名を設定
extern String writeStr;

int premapsx = 60;
int premapsy = 100;
bool moveF = true;
int RunJsGame::loadSurface(File *fp, uint8_t* buf){
  return 0;
}

duk_ret_t RunJsGame::l_tone(duk_context* ctx){
  duk_push_global_object(ctx);          // push global
  duk_get_prop_string(ctx, -1, "obako");// push obako
  RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
  duk_pop_2(ctx); // obako, global

  int n = duk_get_int(ctx, 1);
  int f = duk_get_int(ctx, 2);

  portENTER_CRITICAL(&Tunes::timerMux);
  Tunes::d[n] = (uint16_t)(3.2768*f);
  portEXIT_CRITICAL(&Tunes::timerMux);
  return 0;
}

int RunJsGame::l_spr(duk_context* ctx){
  duk_push_global_object(ctx);          // push global
  duk_get_prop_string(ctx, -1, "obako");// push obako
  RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
  duk_pop_2(ctx); // obako, global

  int x = duk_get_int(ctx, 0);
  int y = duk_get_int(ctx, 1);
  int w = duk_get_int(ctx, 2);
  int h = duk_get_int(ctx, 3);
  int sx = duk_get_int(ctx, 4);
  int sy = duk_get_int(ctx, 5);

  //マップ座標からキャラの位置が0,0になるように計算しておく
  charSpritex = (mapsx + 8)%256;
  charSpritey = (mapsy + 7)%256;

  int sw = w, sh = h;
  if(duk_get_top(ctx) == 8){ // todo: is this bug?
    sw = duk_get_int(ctx, 6);
    sh = (duk_get_int, 7);
  }
  sprite88_0.pushSprite(&tft, x, y);

  return 0;
}

/*
int RunJsGame::l_pget(duk_context* ctx){
  RunJsGame* self = (RunJsGame*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);

  uint16_t c = tft.readPixel(x, y);

  uint8_t index = 0;
  for(unsigned int pi = 0; pi < 256; pi ++){
    if(self->palette[pi] == c){
      index = pi;
      break;
    }
  }
  uint8_t r = ((c >> 11) << 3); // 5bit
  uint8_t g = (((c >> 5) & 0b111111) << 2); // 6bit
  uint8_t b = ((c & 0b11111) << 3);       // 5bit

  lua_pushinteger(L, (lua_Integer)r);
  lua_pushinteger(L, (lua_Integer)g);
  lua_pushinteger(L, (lua_Integer)b);
  lua_pushinteger(L, (lua_Integer)index);
  return 4;
}
*/

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
  if(g == NULL&& b == NULL){

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

duk_ret_t RunJsGame::l_mapno(duk_context* ctx){
  duk_push_global_object(ctx);          // push global
  duk_get_prop_string(ctx, -1, "obako");// push obako
  RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
  duk_pop_2(ctx); // obako, global
  readmapno = duk_get_int(ctx, 0);
  
  readMapFsw(true);//一度だけtrueにして戻す
  xSemaphoreGiveFromISR(semaphore, NULL);//セマフォで合図
  return 1;//読み込んだら1をリターン
}

duk_ret_t RunJsGame::l_drawmap(duk_context* ctx){
  duk_push_global_object(ctx);          // push global
  duk_get_prop_string(ctx, -1, "obako");// push obako
  RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
  duk_pop_2(ctx); // obako, global

  readMapFsw(true);//一度だけtrueにして戻す
  xSemaphoreGiveFromISR(semaphore, NULL);//セマフォで合図
  return 1;//読み込んだら1をリターン
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

int RunJsGame::l_setupbg(duk_context* ctx){
  duk_push_global_object(ctx);
  duk_get_prop_string(ctx, -1, "obako");
  RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
  duk_pop_2(ctx); // pointer, obako, global

  int x0 = duk_get_int(ctx, 0);
  int y0 = duk_get_int(ctx, 1);
  int x1 = duk_get_int(ctx, 2);
  int y1 = duk_get_int(ctx, 3);

  spriteRoi.drawPngFile(SPIFFS, "/init/sprite.png", -161, -211);

  bgSpriteNo%=8;
  spritebg[bgSpriteNo].drawPngFile(SPIFFS, "/init/sprite.png", -8*x0, -8*y0);
  spritebg[bgSpriteNo+1].drawPngFile(SPIFFS, "/init/sprite.png", -8*x1, -8*y1);
  bgSpriteNo+=2;
  
  return 0;
}

int RunJsGame::l_bg(duk_context* ctx){
  duk_push_global_object(ctx);
  duk_get_prop_string(ctx, -1, "obako");
  RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
  duk_pop_2(ctx); // pointer, obako, global

  mapsx = duk_get_int(ctx, 0);//グローバル変数に渡しておく
  mapsy = duk_get_int(ctx, 1);

  int w = duk_get_int(ctx, 2);
  int h = duk_get_int(ctx, 3);

  if(mapready == true){
  for(int j = 0; j<15; j++){
      for(int i = 0; i<16; i++){
        int n = mapArray[(mapsx+i)%256][(mapsy+j)%256]; //0~7が入る
        spritebg[n].pushSprite( &tft, i*8, j*8 );
      }
    }
  }
  return 0;
}


/*
int RunJsGame::l_fillcircle(duk_context* ctx){
  RunJsGame* self = (RunJsGame*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  int r = lua_tointeger(L, 3);

  tft.fillCircle(x, y, r, rgb24to16(self->col[0], self->col[1], self->col[2]));
  return 0;
}
*/

duk_ret_t RunJsGame::l_btn(duk_context* ctx){
  duk_push_global_object(ctx);          // push global
  duk_get_prop_string(ctx, -1, "obako");// push obako
  RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
  duk_pop_2(ctx); // obako, global
  int n = duk_get_int(ctx, 0);
  duk_push_int(ctx, (lua_Integer)self->buttonState[n]);
  
  return 1;
}

// int RunJsGame::l_gcn(duk_context* ctx){
//   duk_push_global_object(ctx);          // push global
//   duk_get_prop_string(ctx, -1, "obako");// push obako
//   RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
//   duk_pop_2(ctx); // obako, global
//   int n = duk_get_int(ctx, 0);

//   return 0;
// }

// int RunJsGame::l_gcn(duk_context* ctx){
//   return 0;//読み込んだら1をリターン
//   // duk_push_global_object(ctx);          // push global
//   // duk_get_prop_string(ctx, -1, "obako");// push obako
//   // RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx,-1);
//   // duk_pop_2(ctx); // obako, global
//   // int n = duk_get_int(ctx, 0);
// //   // if(getMapNo() == 0)return 0;
// //   // else if(getMapNo() == 1)return 1;

// }

// int RunLuaGame::l_btn(lua_State* L){
// duk_ret_t RunJsGame::l_btn(duk_context* ctx)
// {
//   // RunJsGame* self = (RunJsGame*)lua_touserdata(ctx, lua_upvalueindex(1));
//   RunJsGame* self = (RunJsGame*)duk_get_pointer(ctx, lua_upvalueindex(1));
//   // int n = lua_tointeger(ctx, 1);
//   int n = duk_get_int(ctx, 1);
//   // lua_pushinteger(ctx, (lua_Integer)self->buttonState[n]);
//   duk_push_int(ctx, (lua_Integer)self->buttonState[n]);
//   return 1;
// }

String RunJsGame::getBitmapName(String s){
  int p = s.lastIndexOf("/");
  if(p == -1){
    p = 0;
  }
  return s.substring(0, p) + "/sprite.bmp";
}

void RunJsGame::init(){
  this->resume();
}
void RunJsGame::pause(){
  duk_destroy_heap(ctx);
}
void RunJsGame::resume(){
  char buf[MAX_CHAR];
  char str[100];

  Serial.println("resume JS");

  ctx = duk_create_heap_default();
  duk_push_global_object(ctx);

  duk_push_pointer(ctx, this);
  duk_put_prop_string(ctx, -2, "obako");

  duk_idx_t fidx = duk_push_c_function(ctx, RunJsGame::l_tone, 3);
  duk_put_prop_string(ctx, -2, "tone");

  fidx = duk_push_c_function(ctx, l_spr, 6);
  duk_put_prop_string(ctx, -2, "spr");
  

  fidx = duk_push_c_function(ctx, l_pset, 2);
  duk_put_prop_string(ctx, -2, "pset");

  fidx = duk_push_c_function(ctx, l_mapno, 1);
  duk_put_prop_string(ctx, -2, "mapno");

  fidx = duk_push_c_function(ctx, l_drawmap, 1);
  duk_put_prop_string(ctx, -2, "drawmap");

  fidx = duk_push_c_function(ctx, l_color, 3);
  duk_put_prop_string(ctx, -2, "color");

  // fidx = duk_push_c_function(ctx, l_color, 1);//後に書いたものが優先される。
  // duk_put_prop_string(ctx, -2, "color");

  fidx = duk_push_c_function(ctx, l_text, 3);
  duk_put_prop_string(ctx, -2, "text");

  fidx = duk_push_c_function(ctx, l_drawrect, 4);
  duk_put_prop_string(ctx, -2, "drawrect");

  fidx = duk_push_c_function(ctx, l_fillrect, 4);
  duk_put_prop_string(ctx, -2, "fillrect");

  fidx = duk_push_c_function(ctx, l_bg, 4);
  duk_put_prop_string(ctx, -2, "bg");

  fidx = duk_push_c_function(ctx, l_setupbg, 4);
  duk_put_prop_string(ctx, -2, "setupbg");

  fidx = duk_push_c_function(ctx, l_btn, 1);
  duk_put_prop_string(ctx, -2, "btn");

  // fidx = duk_push_c_function(ctx, l_gcn, 1);
  // duk_put_prop_string(ctx, -2, "gcn");
  

  duk_pop(ctx); // pop global

  SPIFFS.begin(true);
  Serial.println("SPIFFS begin");

  if(SPIFFS.exists(getBitmapName(fileName))){
    File bmpFile = SPIFFS.open(getBitmapName(fileName) , FILE_READ);
    Serial.println("bitmap load begin");

    // if(loadSurface(&bmpFile, (uint8_t*)surface) != 0){
    //   printf("bitmap load error");
    //   Serial.println("bitmap load error");
    //   runError = true;
    //   errorString = "bitmap load error fileName=" + getBitmapName(fileName);
    // }

    Serial.println("bitmap load end");
    bmpFile.close();
  }

  Serial.println("loaded bitmap");

  File fp = SPIFFS.open(fileName, FILE_READ);
  Serial.println("open file");

  tft.fillScreen(TFT_BLACK);

  while(fp.available()){
    fp.readStringUntil('\n').toCharArray(buf, MAX_CHAR);
    Serial.println(buf);
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

  Serial.println("finish");

  for(int i = 0; i < 7; i ++){
    buttonState[i] = 0;
  }

  tft.pushSprite(0, 0);
}

int RunJsGame::run(int remainTime){
  char str[100];
  char key;

  if(exitRequest){
    exitRequest = false;
    return 1; // exit
  }

  for(int i = 0; i < 7; i ++){//初期化
      buttonState[i] = false;
  }

if(pressedBtnID == 0){buttonState[0] = true;}//左
if(pressedBtnID == 1){buttonState[1] = true;}//右
if(pressedBtnID == 2){buttonState[2] = true;}//上
if(pressedBtnID == 3){buttonState[3] = true;}//下

if(pressedBtnID == 4){buttonState[4] = true;}//中央
if(pressedBtnID == 5){buttonState[5] = true;}//A
if(pressedBtnID == 6){buttonState[6] = true;}//B
pressedBtnID = -1;

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
        // setFileName("/init/main.lua"); // TODO: lua?
        setFileName("/init/main.js"); // TODO: lua?
        return 1;
      }
    }else{

      if(duk_peval_string(ctx, "loop()") != 0){
        Serial.print("loop() failed");
        Serial.println(duk_safe_to_string(ctx, -1));
      }
      //  Serial.printf("run error? %s\n", lua_tostring(L, -1));
      //  runError = true;
      //  errorString = lua_tostring(L, -1);
      duk_pop(ctx);

    }
  

  // show FPS
  sprintf(str, "%02dFPS", 1000/remainTime); // FPS
  
  // tft.setFont(&lgfxJapanGothicP_8);

  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setCursor(0, 127 - 18);
  tft.print(str);

  sprintf(str, "%02dms", remainTime); // ms
  tft.setCursor(40, 127 - 18);
  tft.print(str);

  int wait = 1000/30 - remainTime;
  if(wait > 0){
    delay(wait);
  }
  return 0;
}


