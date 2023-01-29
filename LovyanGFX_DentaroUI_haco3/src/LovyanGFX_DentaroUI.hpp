#pragma once //インクルードガード

// #include "SD.h"
#include <SD.h>
#include <SPIFFS.h>
#include "LovyanGFX_DentaroUI.hpp"

#include "PhysicBtn.hpp"//物理ボタン用
#include "TouchBtn.hpp"//タッチボタン用
#include <deque>

#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

#include <limits.h>
#include <stddef.h>
#include <vector> 

//開発者表記
#define CHAR_1_BYTE_5 5//=EN 5文字
#define CHAR_1_BYTE_9 9//=EN 9文字
#define CHAR_3_BYTE_5 15//=JP 5文字
#define CHAR_3_BYTE_9 27//=JP 9文字

//開発者表記
#define CHAR_3_BYTE 0//=JP
#define CHAR_1_BYTE 2//=EN
#define NUMERIC 4
//ユーザー表記(Arduino側でしか使わない)
#define JP 0
#define EN 2

#define SHOW_NAMED 0
#define SHOW_ALL 1

#define TOUCH_MODE 0
#define PHYSICAL_MODE 1
#define ANIME_MODE 2

#define TOGGLE_MODE true

#define PARENT_LCD 0
#define PARENT_SPRITE 1

#define ROW_MODE false
#define COL_MODE true

#define SHIFT_NUM 5//シフトパネルの数
#define HENKAN_NUM 57

#define VISIBLE true
#define INVISIBLE false

#define BUF_PNG_NUM 9

// #define BUF_PNG_NUM 9

class MyTFT_eSPI : public LGFX {
  public:

    void drawObako(uint16_t* data){
    int _width = 128;
    int _height = 128;

    pushImage(0,0,128,128,data);
    }
};

class MyTFT_eSprite : public LGFX_Sprite {
  public:
    MyTFT_eSprite(MyTFT_eSPI* tft): LGFX_Sprite(tft){
      _mytft = tft;
    }

    // MyTFT_eSprite(MyTFT_eSprite* sprite): LGFX_Sprite(sprite){
    //   _mysprite = sprite;
    // }

    void myDrawRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color){
      drawFastHLine(x, y, w, color);
      drawFastHLine(x, y + h - 1, w, color);
      // Avoid drawing corner pixels twice
      drawFastVLine(x, y+1, h-2, color);
      drawFastVLine(x + w - 1, y+1, h-2, color);
    }

    void drawObako(){
      _mytft->drawObako(_img);
    }

    // void drawObakoTft(){
    //   _mytft->drawObako(_img);
    // }
    // void drawObakoSprite(){
    //   _mytft->drawObako(_img);
    // }
  private:
    uint16_t* _img;
    MyTFT_eSPI* _mytft;
    MyTFT_eSprite * _mysprite;
};

class RetClass {
public:
  int btnID = -1;
  int btnNo = -1;
  lgfx::v1::touch_point_t sp;
  
  void reset(){
    btnID = -1;
    btnNo = -1;
    sp.x = 0;
    sp.y = 0;
  }

  int setBtnID_ret(int _btnID){
    btnID = _btnID;
    return btnID;
  }

  int setBtnNO_ret(int _btnNo){
    btnNo = _btnNo;
    return btnNo;
  }
  
  void func01(int _btnID, int _x, int _y, int _eventState, int _runEventNo) {   // 関数プロトタイプ「void xxx(int,int,int,int);」
    btnID = _btnID;
    sp.x = _x;
    sp.y = _y;
//    if(runEventNo == eventState){//イベント状態が一致したら関数を実行
    if(_eventState!=-1&&_eventState!=0&&_eventState!=5&&_eventState!=6&&_eventState!=7&&_eventState!=8){//触れていないときNONEと、待機中WAIT、移動中MOVEのイベントは表示しない
      Serial.print("BUTTON_NO_"+String(_btnID));
      
      if(_eventState == 3)Serial.println(String("TAP"));
      if(_eventState == 4)Serial.println(String("W_TAP"));
      if(_eventState == 10)Serial.println(String("RIGHT"));
      if(_eventState == 12)Serial.println(String("UP"));
      if(_eventState == 14)Serial.println(String("LEFT"));
      if(_eventState == 16)Serial.println(String("DOWN"));

      if(_eventState == 11)Serial.println(String("RIGHT_UP"));
      if(_eventState == 13)Serial.println(String("LEFT_UP"));
      if(_eventState == 15)Serial.println(String("LEFT_DOWN"));
      if(_eventState == 17)Serial.println(String("RIGHT_DOWN"));
      
      if(_eventState == 2)Serial.println(String("DRAG"));
      
      //FrameEvent
      if(_eventState == 6)Serial.println(String("TOUCH"));
      if(_eventState == 7)Serial.println(String("WAIT"));
      if(_eventState == 8)Serial.println(String("MOVE"));
      if(_eventState == 9)Serial.println(String("RELEASE"));
      
      
    }
  }
};

class RetClass;

class UiContainer{
  public:
  int id = -1;
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;
  int row = 0;
  int col = 0;
  int b_sNo = 0;
  int b_num = 0;
  int eventNo = -1; //MULTI_EVENT=-1
  int parentID = 0;
  String label = "";
  //std::vector<int, int> uiPos = {0, 0};
  //std::vector<std::pair<int, int>> uiPos ={ {x, y}, {w, h} };
};

struct Vec2{
  double x;
  double y;

  Vec2() = default; // デフォルトコンストラクタ

  Vec2(double _x, double _y) // コンストラクタ
  : x(_x)
  , y(_y) {}
};

class FlickPanel
{
  // private:
  public:
  int b_No = -1;
  std::deque<String> text_list; //パネルテキスト配置用デック配列
  FlickPanel(){};
};

class MapTile
{
  // private:
  public:
  int  posId = -1;
  int  posNo = -1;
  bool readF = false;
  int  mapName = -1;
  int  mapNo = -1;
  int  preMapNo = 0;
  int objNo = -1;
  int MapNo = 0;
  int latPos, lonPos = 0;
  bool existF = false;
  // int xtile, ytile, ztile = 0;
  int xtilePos, ytilePos = 0;
  int xtileNo, ytileNo, ztileNo = 0;//IDで使う
  int preXtileNo, preYtileNo, preZtileNo = 0;//
  int addXTileNo, addYTileNo = 0;//中央からの相対的なタイルナンバー -1,1
  int preAddXTileNo, preAddYTileNo = 0;//中央からの相対的なタイルナンバー -1,1
  String m_url;
  LGFX_Sprite buff_sprite;
  LGFX_Sprite* buff_sprite_p;
  MapTile(){};

  void begin(int _objNo, int _xtile, int _ytile,int _ztile, String _m_url){
    // xtile = _xtile;
    // ytile = _ytile;
    // ztile = _ztile;
    objNo = _objNo;

    m_url = _m_url;
    
    buff_sprite.setPsram(USE_PSRAM);
    buff_sprite.setColorDepth(COL_DEPTH);//子スプライトの色深度
    buff_sprite.createSprite(256, 256);//子スプライトメモリ確保
    buff_sprite.setPaletteGrayscale();
    buff_sprite.drawPngFile(SD, m_url,
                            0, 0,
                            256, 256,
                            0, 0, 1.0, 1.0,
                            datum_t::top_left);
    buff_sprite_p = &buff_sprite;//オブジェクトのアドレスをコピーして、ポインタを格納

  }

  LGFX_Sprite* getSpritePtr(){return buff_sprite_p;}
  int getObjNo(){return objNo;}
  // void setXtile(int _xtile){xtile = _xtile;}
  // void setYtile(int _ytile){ytile = _ytile;}
  // int getXtile(){return xtile;}
  // int getYtile(){return ytile;}
  void setPreXtileNo(int _preXtileNo){preXtileNo = _preXtileNo;}
  void setPreYtileNo(int _preYtileNo){preYtileNo = _preYtileNo;}
  void setXtileNo(int _xtileNo){xtileNo = _xtileNo;}
  void setYtileNo(int _ytileNo){ytileNo = _ytileNo;}

  void setXtilePos(int _xtilePos){xtilePos = _xtilePos;}
  void setYtilePos(int _ytilePos){ytilePos = _ytilePos;}

  int getXtileNo(){return xtileNo;}
  int getYtileNo(){return ytileNo;}
  
  int getPreXtileNo(){return preXtileNo;}
  int getPreYtileNo(){return preYtileNo;}

  int getXtilePos(){return xtilePos;}
  int getYtilePos(){return ytilePos;}

  void setAddX( int _addXTileNo){addXTileNo = _addXTileNo;}
  void setAddY(int _addYTileNo){addYTileNo = _addYTileNo;}
  int getAddX(){return addXTileNo;}
  int getAddY(){return addYTileNo;}

  void setPreAddX( int _preAddXTileNo){preAddXTileNo = _preAddXTileNo;}
  void setPreAddY(int _preAddYTileNo){preAddYTileNo = _preAddYTileNo;}
  int getPreAddX(){return preAddXTileNo;}
  int getPreAddY(){return preAddYTileNo;}

  void setMapReadF( bool _readF){readF = _readF;}
  bool getMapReadF(){return readF;};
  // void setMapName( int _mapName){mapName = _mapName;}
  
  int getMapName(){return mapName;}
  void setMapNo(int _mapNo){mapNo = _mapNo;}
  int getMapNo(){return mapNo;}

  void setPreMapNo(int _preMapNo){preMapNo = _preMapNo;}
  int getPreMapNo(){return preMapNo;}


  void setPosNo(int _posNo){posNo = _posNo;}
  int getPosNo(){return posNo;}

  void setExistF(bool _existF){existF = _existF;}
  bool getExistF(){return existF;}
  

};

class LovyanGFX_DentaroUI {
  
   private:

    lgfx::v1::touch_point_t tp;
    lgfx::v1::touch_point_t sp;
    lgfx::v1::touch_point_t sp2;
    lgfx::v1::touch_point_t layoutSpritePos; //レイアウト用
    lgfx::v1::touch_point_t lcdPos;//レイアウト用
    uint32_t btnState = B00000000;
    uint16_t touchState = B00000000;
    unsigned long sTime = 0;
    unsigned long tappedTime = 0;
    
    int eventState = -1;
    int flickState = -1;
    int tap_flick_thre = 82000;//タップとフリックの閾値
    int dist_thre = 40;
    int uiID = -1;
    int btnID = 0;
    uint constantBtnID = 9999;
    bool constantGetF = false;
    int selectBtnID = -1;
    int selectBtnBoxID = -1;
    int runEventNo =  -1;
    int parentID = 0;//初期値0 =　PARENT_LCD
    bool availableF = false;
    int uiMode = TOUCH_MODE;
    std::deque<TouchBtn*> touch_btn_list; //ボタン配置用デック配列

    int timeCnt = 0;
    uint16_t clist[5] = {0,0,0,0,0};
    int preEventState = -1;
    int AngleCount = 0;
    int uiBoxes_num = 0;
    // std::deque<UiContainer> uiBoxes;
    UiContainer uiBoxes[20];
    int layoutSprite_w = 0;
    int layoutSprite_h = 0;
    bool toggle_mode = false;
    RetClass obj_ret;

    int sec, psec;
    int fps = 0;
    int frame_count = 0;
    bool touchCalibrationF = false;

    int touchZoom = 1;

  
    PhysicBtn phbs;

    // uint16_t calData[8] = {442,3556,432,322,3662,3523,3543,293};
    // uint16_t calData[8] = {3811,3853,345,3789,3711,403,1086,430};
    uint16_t calData[8] = {558,3648,447,396,3599,3622,3625,324};
    uint16_t calDataOK = 0;

public:
    LGFX_Sprite layoutSprite_list[BUF_PNG_NUM];
    LovyanGFX_DentaroUI( LGFX* _lcd );
    LGFX* lcd;
    LovyanGFX* lgfx;
    LGFX_Sprite flickUiSprite;//フリック展開パネル用
    void update( LGFX& _lcd );
    void begin( LGFX& _lcd, int _colBit, int _rotateNo, bool _calibF );
    void begin(LGFX& _lcd);
    void touchCalibration(bool _calibUseF);
    void showSavedCalData(LGFX& _lcd);
    // void runTouchCalibration(LGFX& _lcd);
    bool isAvailable(int _btnID);
    void setConstantGetF(bool _constantGetF);
    void addHandler(int _btnID, int _btnNo, DelegateBase2* _func, uint16_t _runEventNo, int parentID = 0, bool _constantGetF = false);
    void circle(LovyanGFX* _lgfx, uint16_t c, int fillF);
    void rect(LovyanGFX* _lgfx, uint16_t c, int fillF);
    float getAngle(lgfx::v1::touch_point_t a, lgfx::v1::touch_point_t b );
    float getDist(lgfx::v1::touch_point_t a, lgfx::v1::touch_point_t b );
    int getPreEvent();
    // int getTouchEvent();
    void updateSelectBtnID(int _selectBtnID);
    lgfx::v1::touch_point_t getStartPos();//タッチされたスタート地点を取得
    lgfx::v1::touch_point_t getPos();//タッチしている座標を取得
    void setPos(int _x, int _y);
    void setStartPos(int _x, int _y);
    DelegateBase2 *setBtnID_ret_DG = Delegate2<RetClass>::createDelegator2( &obj_ret, &RetClass::setBtnID_ret );//型式が違うプロトタイプ関数
    DelegateBase2 *setBtnNO_ret_DG = Delegate2<RetClass>::createDelegator2( &obj_ret, &RetClass::setBtnNO_ret );//型式が違うプロトタイプ関数
    void setLayoutPos( int _x, int _y );
    void createLayout( int _layoutSprite_x, int _layoutSprite_y, int _layoutSprite_w, int _layoutSprite_h, LGFX_Sprite& _layoutSprite, int _eventNo);
    void setLayoutSpritePos( int _LayoutSpritePosx, int _LayoutSpritePosy );
    void setLayoutPosToAllBtn( lgfx::v1::touch_point_t  _layoutPos );
    void setBtnName(int _btnNo, String _btnName);
    void setBtnName(int _btnNo, String _btnName, String _btnNameFalse);//toggle用
    void setQWERTY(int _uiID, String _btnsString, LGFX_Sprite& _sprite);
    void createBtns( int _uiSprite_x, int _uiSprite_y, int _w,int _h, int _row, int _col, int _eventNo);//縦方向に並ぶ等倍
    void createBtns( int _uiSprite_x, int _uiSprite_y, int _w,int _h, int _row, int _col, int _eventNo, int _touchZoom);//縦方向に並ぶ（2倍以上）
    void clearAddBtns(int _ctrlBtnNum);
    void createToggles( int _uiSprite_x, int _uiSprite_y, int _w,int _h, int _row, int _col, LGFX_Sprite& _uiSprite, int _eventNo);//縦方向に並ぶ
    void createSliders( int _x, int _y, int _w, int _h,  int _row, int _col, LGFX_Sprite& _uiSprite, int _visible_mode, int _eventNo);
    void drawBtns(int _uiID, LovyanGFX* _lgfx, LGFX_Sprite& _uiSprite);
    void drawBtns(int _uiID, LovyanGFX* _lgfx, LGFX_Sprite& _uiSprite, int _uiSprite_x, int _uiSprite_y);
    void drawToggles(int _uiID, LovyanGFX* _lgfx, LGFX_Sprite& _uiSprite, int _uiSprite_x, int _uiSprite_y);
    void drawToggles(int _uiID, LovyanGFX* _lgfx, LGFX_Sprite& _uiSprite);
    void drawSliders(int _uiID, LovyanGFX* _lgfx, LGFX_Sprite& _uiSprite, int _uiSprite_x, int _uiSprite_y);
    void drawSliders(int _uiID, LovyanGFX* _lgfx, LGFX_Sprite& _uiSprite);
    void drawLayOut(LGFX_Sprite& _layoutSprite);
    void drawSelectBtn(int _id);
    // int getTouchBtnNo();//タッチされたボタンオブジェクトの番号を取得
    int getTouchBtnNum();//現在のタッチボタンの数を取得
    //int getTouchBtnID();//タッチされたボタンオブジェクトのIDを取得
    int getTouchBtnID();
    int getEvent();
    int getFlickEvent();
    void showTouchEventInfo(LovyanGFX& _lgfx, int _x, int _y);
    lgfx::v1::touch_point_t getTouchPoint(int _x, int _y);
    float getSliderVal( int uiID, int _btnNo );
    float getSliderVal(int uiID,int _btnNo, int _xy);
    Vec2 getSliderVec2( int uiID, int _btnNo );
    void setSliderVal(int uiID, int _btnNo, float _x, float _y);
    bool getToggleVal(int _uiID, int _btnID);
    bool getToggleVal(int _btnID);
    bool switchToggleVal(int _btnID, int _tbmode);

    void setAllBtnAvailableF(int uiID, bool _available);
    void setAvailableF(int uiID, int _btnID, bool _available);
    void setAllBtnVisibleF(int uiID, bool _visible);
    void setVisibleF(int uiID, int _btnID, bool _visible);
    // void setCharMode(int _charMode);
    // int getCharMode();
    void changeBtnMode(int _uiID, int _btnID, int _btn_mode);
    int getUiFirstNo(int _uiID);
    int getUiBtnNum(int _uiID);
    int getUiBoxNum();
    int getAllBtnNum();
    int getParentID();
    int getUiID( const char* _uiLabel );
    void setBtnID(int _btnID);

    lgfx::v1::touch_point_t getBtnPos(int _btnID);
    void showInfo( LovyanGFX& _lgfx, int _infox, int _infoy);
    std::vector<std::string> split_mb(const char* src, const char* del);

    void setupPhBtns(int pfbNo0, int pfbNo1, int pfbNo2 );
    void updatePhBtns();
    const bits_btn_t*  getStack();
    std::uint32_t getHitValue();
    void drawPhBtns(LovyanGFX& _lcd, int _x, int _y);

};
