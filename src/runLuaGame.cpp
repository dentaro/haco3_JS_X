#include "runLuaGame.h"
extern MyTFT_eSprite tft;
extern LGFX_Sprite sprite64;
extern LGFX_Sprite sprite88_roi;
extern String appfileName;
extern void startWifiDebug(bool isSelf);
extern void setFileName(String s);
extern bool isWifiDebug();
extern void reboot();
extern Tunes tunes;
extern int pressedBtnID;
extern LovyanGFX_DentaroUI ui;
extern int outputMode;
extern int mapsprnos[16];

// extern int oskF;

// int system(const char* c){
//   //none
// }

int cursor = 0;

extern "C" {
  void gprint(char* s){
    tft.setCursor(3, cursor);
    tft.setTextColor(TFT_WHITE, TFT_BLUE);
    tft.print(s);
    cursor += 4;
    if(cursor > 128){
      cursor = 0;
    }
  }

  const char *getF(lua_State *L, void *ud, size_t *size){
    struct LoadF *lf = (struct LoadF *)ud;
    (void)L; /* not used */
    char* ret = NULL;

    if(!lf->f.available()){
      *size = 0;
      return NULL;
    }

    lf->f.readStringUntil('\n').toCharArray(lf->buf, MAX_CHAR);
    ret = lf->buf;
    int len = strnlen(ret, MAX_CHAR);
    ret[len] = '\n'; // todo n, n+1 > MAX_CHAR ?
    ret[len + 1] = 0;

    *size = len + 1;
    Serial.print("");
    Serial.println(ret);
    Serial.println(*size);
    return ret;
  }
}

int RunLuaGame::loadSurface(File *fp, uint8_t* buf){
  uint8_t c;
  unsigned long offset;
  unsigned long width, height;
  unsigned long biSize;
  uint16_t bitCount;

  Serial.println("pre read");
  fp->read(&c, 1);
  Serial.println("read1");
  if(c != 'B'){
    printf("header error 0");
    Serial.print(c);
    Serial.println("unknown header");
    return -1;
  }
  fp->read(&c, 1);
  Serial.println("read2");
  if(c != 'M'){
    printf("header error 1");
    return -1;
  }
  Serial.println("pre seek");
  fp->seek(4 + 2 + 2, SeekCur); // size, resv1, resv2
  fp->read((uint8_t*)&offset, 4);

  fp->read((uint8_t*)&biSize, 4);
  fp->read((uint8_t*)&width, 4);
  fp->read((uint8_t*)&height, 4);
  fp->seek(2, SeekCur); // skip biPlanes
  fp->read((uint8_t*)&bitCount, 2);


  Serial.println("pre check");
  if(width != 128){
    printf("invalid width:%d\n", width);
    return -1;
  }
  if(height != 128){
    printf("invalid height:%d\n", height);
    return -1;
  }
  if(bitCount != 8){
    printf("invalid bitCount:%x\n", bitCount);
    return -1;
  }

  fp->seek(biSize - (4 + 4 + 4 + 2 + 2), SeekCur);
  uint8_t r, g, b;
  for(unsigned int i = 0; i < 256; i ++){
    fp->read(&b, 1);
    fp->read(&g, 1);
    fp->read(&r, 1);
    fp->seek(1, SeekCur);
    palette[i] = lua_rgb24to16(r, g, b);
    Serial.print("palette");
    Serial.println(i);
    Serial.print(r);
    Serial.print(g);
    Serial.print(b);
  }

  Serial.println("pre seek");
  fp->seek(offset, SeekSet); // go to bmp data section

  for(unsigned int i = 0; i < width * height; i ++){
    uint8_t d;
    fp->read(&d, 1);
    *buf = d;
    buf ++;
  }
  return 0;
}

int RunLuaGame::l_tp(lua_State* L)
{
  RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int n = lua_tointeger(L, 1);
  self->tp[0] = ui.getPos().x/2;
  self->tp[1] = ui.getPos().y/2;
  lua_pushinteger(L, (lua_Integer)self->tp[n]);
  return 1;
}

// int RunLuaGame::l_tp(lua_State* L){
//   RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
//   int n = lua_tointeger(L, 1);
//   self->tp[0] = ui.getPos().x/2;
//   self->tp[1] = ui.getPos().y/2;
//   lua_pushinteger(L, (lua_Integer)self->tp[n]);//JSに値をリターンできる
//   return 1;//１にしないといけない（duk_pushでJSに値をリターンできる
// }

int RunLuaGame::l_tone(lua_State* L){
  RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int n = lua_tointeger(L, 1);
  int f = lua_tointeger(L, 2);

  portENTER_CRITICAL(&Tunes::timerMux);
  Tunes::d[n] = (uint16_t)(3.2768*f);
  portEXIT_CRITICAL(&Tunes::timerMux);
  return 0;
}

int RunLuaGame::l_spr(lua_State* L){  
  RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));

  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  int w = lua_tointeger(L, 3);
  int h = lua_tointeger(L, 4);
  int sx = lua_tointeger(L, 5);
  int sy = lua_tointeger(L, 6);

  for(int j=0;j<h/8;j++){
      for(int i=0;i<w/8;i++){
      sprite64.pushSprite(&sprite88_roi, -(sx+(i*8)), -(sy+(j*8)));//128*128のpngデータを指定位置までずらす
      sprite88_roi.pushSprite(&tft, x+(i*8), y+(j*8), TFT_BLACK);//16*16で切り抜き&tftに書き出し：黒を透明に
      }
  }
  return 0;
}
int RunLuaGame::l_scroll(lua_State* L){
  RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);

  tft.scroll(x, y);
  return 0;
}

int RunLuaGame::l_pset(lua_State* L){
  RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  int cn = lua_tointeger(L, 3);
  if(cn != NULL){
    self->col[0] = self->clist[cn][0]; // 5bit
    self->col[1] = self->clist[cn][1]; // 6bit
    self->col[2] = self->clist[cn][2]; // 5bit
  }
    tft.drawPixel(x, y, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  return 0;
}

int RunLuaGame::l_pget(lua_State* L){
  RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
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

int RunLuaGame::l_color(lua_State* L){
  RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int r,g,b;
  r = lua_tointeger(L, 1);
  g = lua_tointeger(L, 2);
  b = lua_tointeger(L, 3);
    //とにかく一回格納する
  self->col[0] = r;
  self->col[1] = g;
  self->col[2] = b;
  //色番号だったら上書き
  if(g == NULL && b == NULL){
    int cn = lua_tointeger(L, 1);
    self->col[0] = self->clist[cn][0]; // 5bit
    self->col[1] = self->clist[cn][1]; // 6bit
    self->col[2] = self->clist[cn][2]; // 5bit
  }
  return 0;
}

int RunLuaGame::l_text(lua_State* L){
  RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  const char* text = lua_tostring(L, 1);
  int x = lua_tointeger(L, 2);
  int y = lua_tointeger(L, 3);

  tft.setCursor(x,y);
  tft.setTextColor(lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  tft.print(text);
  return 0;
}

int RunLuaGame::l_opmode(lua_State* L){//FAST,WIDE
  RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int _n = lua_tointeger(L, 1);
  outputMode = _n;
  return 0;
}

int RunLuaGame::l_drawrect(lua_State* L){
  RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  int w = lua_tointeger(L, 3);
  int h = lua_tointeger(L, 4);

  tft.drawRect(x, y, w, h, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  return 0;
}

int RunLuaGame::l_fillrect(lua_State* L){
  RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  int w = lua_tointeger(L, 3);
  int h = lua_tointeger(L, 4);

  tft.fillRect(x, y, w, h, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  return 0;
}

int RunLuaGame::l_fillcircle(lua_State* L){
  RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int x = lua_tointeger(L, 1);
  int y = lua_tointeger(L, 2);
  int r = lua_tointeger(L, 3);

  tft.fillCircle(x, y, r, lua_rgb24to16(self->col[0], self->col[1], self->col[2]));
  return 0;
}

int RunLuaGame::l_btn(lua_State* L){
  RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  int n = lua_tointeger(L, 1);
  // Serial.println((lua_Integer)self->buttonState[n]);
  lua_pushinteger(L, (lua_Integer)self->buttonState[n]);
  return 1;
}

int RunLuaGame::l_iswifidebug(lua_State* L){
  RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));

  lua_pushboolean(L, isWifiDebug()?1:0);
  return 1;
}
int RunLuaGame::l_getip(lua_State* L){
  RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  char buf[16];
  IPAddress ip = WiFi.localIP();
  if(self->wifiDebugSelf){
    ip = WiFi.softAPIP();
  }
  sprintf(buf, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3] );

  lua_pushstring(L, buf);
  return 1;
}


int RunLuaGame::l_wifiserve(lua_State* L){
  RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  self->wifiDebugSelf = false;
  if(lua_gettop(L) == 1){
    const char* text = lua_tostring(L, 1);
    if(strncmp(text, "ap", 3) == 0){
      self->wifiDebugSelf = true;
    }
  }
  self->wifiDebugRequest = true;
  return 0;
}
int RunLuaGame::l_run(lua_State* L){//ファイル名を取得して、そのファイルを実行runする

  RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  const char* file = lua_tostring(L, 1);

  setFileName(file);//次のゲームのパスをセット
  self->exitRequest = true;//次のゲームを立ち上げるフラグを立てる
  return 0;
}

int RunLuaGame::l_list(lua_State* L){//
  RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  File f;

  lua_newtable(L);

  File root = SPIFFS.open("/");
  f = root.openNextFile();
  int i = 0;
  while(f){
    Serial.println(f.path());
    lua_pushstring(L, f.path());//パスを文字列にしてリターン
    lua_rawseti(L, -2, i);
    f = root.openNextFile();
    i ++;
  }
  return 1;
}
int RunLuaGame::l_require(lua_State* L){
  bool loadError = false;
  RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  const char* fname = lua_tostring(L, 1);
  File fp = SPIFFS.open(fname, FILE_READ);

  struct LoadF lf;
  lf.f = fp;
  char cFileName[32];
  appfileName.toCharArray(cFileName, 32);
  if(lua_load(L, getF, &lf, cFileName, NULL)){
    printf("error? %s\n", lua_tostring(L, -1));
    Serial.printf("error? %s\n", lua_tostring(L, -1));
    //runError = true;
    //errorString = lua_tostring(L, -1);
    loadError = true;
  }
  fp.close();

  if(loadError == false){
    if(lua_pcall(L, 0, 1, 0)){
      Serial.printf("init error? %s\n", lua_tostring(L, -1));
      //runError = true;
      //errorString = lua_tostring(L, -1);
    }
  }

  Serial.println("finish require");


  return 1;
}

int RunLuaGame::l_httpsget(lua_State* L){
  RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  const char* host = lua_tostring(L, 1);
  const char* path = lua_tostring(L, 2);
  WiFiClientSecure client;
  const int httpsPort = 443;
  Serial.println(host);
  Serial.println(path);
  if(!client.connect(host, httpsPort)){
    // connection failed
    Serial.println("connect failed");
  }
  client.print(String("GET ") + path + " HTTP/1.1\r\n" +
    "Host: " + host + "\r\n" +
    "User-Agent: o-bako\r\n" +
    "Connection: close\r\n\r\n"
  );
  String line;
  while(client.connected()){
    line = client.readStringUntil('\n');
    if(line == "\r"){
      // headers recieved
      Serial.println("headers recieved");
      break;
    }
  }
  line = client.readString();
  int lineLength = line.length();
  const char *lineChar = line.c_str();

  lua_pushstring(L, lineChar);
  Serial.println("done");
  return 1;
}

int RunLuaGame::l_httpsgetfile(lua_State* L){
  RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  const char* host = lua_tostring(L, 1);
  const char* path = lua_tostring(L, 2);
  const char* filePath = lua_tostring(L, 3);
  WiFiClientSecure client;
  const int httpsPort = 443;
  if(!client.connect(host, httpsPort)){
    // connection failed
    Serial.println("connect failed");
  }
  client.print(String("GET ") + path + " HTTP/1.1\r\n" +
    "Host: " + host + "\r\n" +
    "User-Agent: o-bako\r\n" +
    "Connection: close\r\n\r\n"
  );
  String line;
  int len = 0;
  while(client.connected()){
    line = client.readStringUntil('\n');
    if(line == "\r"){
      // headers recieved
      Serial.println("headers recieved");
      break;
    }
    if(line.startsWith("Content-Length: ")){
      len = line.substring(16).toInt();
    }

  }

  tunes.pause();
  File f = SPIFFS.open(filePath, FILE_WRITE);
  while(client.available() && len > 0){
    char c = client.read();
    f.write(c);
    len --;
  }
  f.close();
  tunes.resume();
  return 0;
}

int RunLuaGame::l_savebmp(lua_State* L){
  RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  const char* path = lua_tostring(L, 1);
  uint8_t buf[4];
  long* ltmp = (long*) buf;
  uint16_t* tmp2 = (uint16_t*)buf;
  uint8_t tmp1;

  tunes.pause();
  File f = SPIFFS.open(path, FILE_WRITE);
  f.write('B');
  f.write('M');
  *ltmp = 14 + 40 + 4 * 256;
  f.write(buf, 4); // file size
  *tmp2 = 0;
  f.write(buf, 2); // reserved1
  f.write(buf, 2); // reserved2
  *ltmp = 14 + 40 + 4 * 256;
  f.write(buf, 4); // header size

  // BITMAPCOREHEADER
  *ltmp = 40;
  f.write(buf, 4); // bc size
  *ltmp = 128;
  f.write(buf, 4); // width
  f.write(buf, 4); // height
  *tmp2 = 1;
  f.write(buf, 2); // planes
  *tmp2 = 8;
  f.write(buf, 2); // bitcount
  *ltmp = 0;
  f.write(buf,4); // compression
  *ltmp = 0;
  f.write(buf,4); // size image
  *ltmp = 0;
  f.write(buf,4); // horizon dot/m
  *ltmp = 0;
  f.write(buf,4); // vertical dot/m
  *ltmp = 0;
  f.write(buf,4); // cir used
  *ltmp = 0;
  f.write(buf,4); // cir important

  uint8_t r,g,b;
  for(unsigned int i = 0; i < 256; i ++){
    r = ((self->palette[i] >> 11) << 3);
    g = (((self->palette[i] >> 5) & 0b111111) << 2);
    b = ((self->palette[i] & 0b11111) << 3);
    f.write(b);
    f.write(g);
    f.write(r);
    f.write(0); // reserved
  }
  int x = 0,y = 127;
  for(unsigned int i = 0; i < 128*128; i ++){
    uint16_t d = tft.readPixel(x, y);
    uint8_t index = 0;
    for(unsigned int pi = 0; pi < 256; pi ++){
      if(self->palette[pi] == d){
        index = pi;
        break;
      }
    }
    f.write(index);
    x ++;
    if(x == 128){
      x = 0;
      y --;
    }
  }
  f.close();
  tunes.resume();
  return 0;
}

int RunLuaGame::l_reboot(lua_State* L){
  RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));

  reboot();
  return 0;
}

int RunLuaGame::l_debug(lua_State* L){
  RunLuaGame* self = (RunLuaGame*)lua_touserdata(L, lua_upvalueindex(1));
  const char* text = lua_tostring(L, 1);

  Serial.println(text);
  return 0;
}

String RunLuaGame::getBitmapName(String s){
  int p = s.lastIndexOf("/");
  if(p == -1){
    p = 0;
  }
  return s.substring(0, p) + "/sprite.bmp";
}

String RunLuaGame::getPngName(String s){
  int p = s.lastIndexOf("/");
  if(p == -1){
    p = 0;
  }
  return s.substring(0, p) + "/initspr.png";
}

void RunLuaGame::init(){
  this->resume();
}

void RunLuaGame::pause(){
  lua_close(L);
}

void RunLuaGame::resume(){//ゲーム起動時のみ一回だけ走る処理（setupのようなもの)

  char buf[MAX_CHAR];
  char str[100];

  L = luaL_newstate();
  luaL_openlibs(L);

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_tp, 1);
  lua_setglobal(L, "tp");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_tone, 1);
  lua_setglobal(L, "tone");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_spr, 1);
  lua_setglobal(L, "spr");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_scroll, 1);
  lua_setglobal(L, "scroll");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_pset, 1);
  lua_setglobal(L, "pset");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_pget, 1);
  lua_setglobal(L, "pget");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_color, 1);
  lua_setglobal(L, "color");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_text, 1);
  lua_setglobal(L, "text");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_opmode, 1);
  lua_setglobal(L, "opmode");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_drawrect, 1);
  lua_setglobal(L, "drawrect");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_fillrect, 1);
  lua_setglobal(L, "fillrect");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_fillrect, 1);
  lua_setglobal(L, "fillrect");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_fillcircle, 1);
  lua_setglobal(L, "fillcircle");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_btn, 1);
  lua_setglobal(L, "btn");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_getip, 1);
  lua_setglobal(L, "getip");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_iswifidebug, 1);
  lua_setglobal(L, "iswifidebug");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_wifiserve, 1);
  lua_setglobal(L, "wifiserve");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_run, 1);
  lua_setglobal(L, "run");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_list, 1);
  lua_setglobal(L, "list");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_reboot, 1);
  lua_setglobal(L, "reboot");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_debug, 1);
  lua_setglobal(L, "debug");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_require, 1);
  lua_setglobal(L, "require");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_httpsget, 1);
  lua_setglobal(L, "httpsget");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_httpsgetfile, 1);
  lua_setglobal(L, "httpsgetfile");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_savebmp, 1);
  lua_setglobal(L, "savebmp");

  haco8resume();//派生クラスでのみこの位置で実行されるダミー関数

  SPIFFS.begin(true);//SPIFFSを利用可能にする

  if(SPIFFS.exists(getPngName(appfileName))){
    sprite64.drawPngFile(SPIFFS, appfileName, 0, 0);
  }
  //後でSDからもファイルを読めるようにする

  File fp = SPIFFS.open(appfileName, FILE_READ);

  tft.fillScreen(TFT_BLACK);

  struct LoadF lf;
  lf.f = fp;

  char cFileName[32];
  appfileName.toCharArray(cFileName, 32);//char変換
  
  if(lua_load(L, getF, &lf, cFileName, NULL)){//Luaに渡してファイル読み込みに成功したかチェック（成功すると0）
    printf("error? %s\n", lua_tostring(L, -1));
    Serial.printf("error? %s\n", lua_tostring(L, -1));
    runError = true;//エラーが発生
    errorString = lua_tostring(L, -1);
  }

  fp.close();

  if(runError == false){//エラーが発生していなくても
    if(lua_pcall(L, 0, 0,0)){//LUAの関数呼び出しに成功したかチェック（成功すると0）
      Serial.printf("init error? %s\n", lua_tostring(L, -1));
      runError = true;//エラーが発生
      errorString = lua_tostring(L, -1);
    }
  }

  Serial.println("lua chack finish");

  for(int i = 0; i < CTRLBTNNUM; i ++){//初期化
      buttonState[i] = false;
  }

  tft.pushSprite(0, 0);
}

int RunLuaGame::run(int _remainTime){
  char str[12];
  // char key;
  // for(int i = 0; i < CTRLBTNNUM; i ++){//初期化
  //     buttonState[i] = false;
  //     if(pressedBtnID ==  i){buttonState[i]  = true;}//左
  // }
// pressedBtnID = -1;

  if(wifiDebugRequest){
    startWifiDebug(wifiDebugSelf);
    wifiMode = SHOW;
    wifiDebugRequest = false;
  }

  if(exitRequest){//次のゲームを起動するフラグがたったら
    exitRequest = false;//フラグをリセットして、
    return 1; // exit(1をリターンすることで、main.cppの変数modeを１にする)
  }

  //ボタンを押してからの経過フレームを返すための処理
  // for(int i = 0; i < CTRLBTNNUM; i ++){
  //   if(ui.getEvent()==NO_EVENT){
  //     buttonState[i] = 0;
  //   }else{
  //     buttonState[i] ++;
  //   }
  // }

  //ボタンを押してからの経過時間を返すための処理
  for(int i = 0; i < CTRLBTNNUM; i ++){
    if(ui.getEvent()==NO_EVENT){
      buttonState[i] = 0;
    }else{
      if(pressedBtnID ==  i){//押されたものだけの値をあげる
      buttonState[i] ++;
      }
    }
  }

  if(wifiMode == NONE || wifiMode == RUN){
    if(runError){
      tft.setTextSize(1);
      tft.setTextColor(TFT_WHITE, TFT_RED);
      tft.setCursor(0, 0);
      tft.setTextWrap(true);
      tft.print(errorString);
      tft.setTextWrap(false);

      // if(buttonState[2]){ // reload
      //   return 1;
      // }
      // if(buttonState[1]){ // reload
      //   setFileName("/init/main.lua");
      //   return 1;
      // }
      // if(buttonState[0]){ // reload
      //   wifiMode = SELECT;
      // }
    }else{
      // if(oskF==0){//オンスクリーンキーボード使用中はゲームループを止める

        if(luaL_dostring(L, "loop()")){
          Serial.printf("run error? %s\n", lua_tostring(L, -1));
          runError = true;
          errorString = lua_tostring(L, -1);
        }

      // }

      // if(buttonState[10]){ // ブルーメニューを開く
      //   wifiMode = SELECT;
      // }
    }
  }else if(wifiMode == SELECT){
    tft.fillRect(0, 0, 128, 64, lua_rgb24to16(64,64,64));
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLUE);
    tft.setCursor(0, 0);
    tft.print("pause");
    tft.setCursor(0, 8);
    tft.print("  WiFi AP");
    tft.setCursor(0, 16);
    tft.print("  WiFi STA");
    tft.setCursor(0, 24);
    tft.print("  load /init/main.lua");
    tft.setCursor(0, (modeSelect + 1) * 8);
    tft.print(">");

    // if(buttonState[6] && modeSelect > 0){//ブルーメニュー選択
    //   modeSelect -= 1;
    // }
    // if(buttonState[7] || buttonState[3]){//ブルーメニュー選択
    //   modeSelect += 1;
    //   modeSelect = modeSelect%3;
    // }
    // if(buttonState[9]){//ブルーメニュー決定
    //   switch(modeSelect){
    //     case 0:
    //       wifiDebugRequest = true;
    //       wifiDebugSelf = true;

    //     break;
    //     case 1:
    //       wifiDebugRequest = true;
    //       wifiDebugSelf = false;

    //     break;
    //     case 2:
    //       setFileName("/init/main.lua");
    //       return 1;
    //   }
      
    // }
  }else if(wifiMode == SHOW){
    // if(buttonState[9]){ // reload//ブルーメニューをとじてwifionのまま戻る
    //   wifiMode = RUN;

    // }
  }

  // show FPS
  sprintf(str, "%02dFPS", 1000/_remainTime); // FPS

  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setCursor(90, 127 - 16);
  tft.print(str);

  sprintf(str, "%02dms", _remainTime); // ms
  tft.setCursor(90, 127 - 8);
  tft.print(str);

  

  int wait = 1000/30 - _remainTime;
  if(wait > 0){
    delay(wait);
  }
  return 0;
}



