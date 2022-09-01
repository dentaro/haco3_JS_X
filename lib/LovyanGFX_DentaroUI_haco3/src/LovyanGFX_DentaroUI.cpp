#include "LovyanGFX_DentaroUI.hpp"
using namespace std;

#define FLICK_DIST 3

LovyanGFX_DentaroUI::LovyanGFX_DentaroUI(LGFX* _lcd)
{
  lcd = _lcd;
}

void LovyanGFX_DentaroUI::setupPhBtns(LovyanGFX& _lcd, int pfbNo0, int pfbNo1, int pfbNo2 ){
  phbs.setup(pfbNo0, pfbNo1, pfbNo2);

}

std::uint32_t LovyanGFX_DentaroUI::getHitValue(){
  // コマンド判定結果を取得
  return  phbs.getValue();
}

void LovyanGFX_DentaroUI::updatePhBtns()
{
  delay(1);
  // 入力状態の更新（変化がなければ終了）
  if (!phbs.loop()) return;

  // コマンド判定結果を取得
  static std::uint32_t hitvalue = ~0U;
  std::uint32_t prev = hitvalue;
  hitvalue = phbs.getValue();

  // 以前の値と同じ場合は除外
  if (prev == hitvalue) return;

}
const bits_btn_t*  LovyanGFX_DentaroUI::getStack()
{
  return phbs.getStack();
}

//上のupdateを視覚化しただけの処理（やってることは同じ）
void LovyanGFX_DentaroUI::drawPhBtns(LovyanGFX& _lcd)
{

  // ボタン入力履歴を画面に描画
        // auto stack = ui.getStack();
        // for (std::size_t idx = 0; idx < BTN_STACK_MAX; ++idx)
        // {
        //   layoutSprite.fillRect(idx * 10, 30, 9, 9, (stack[idx] & bit_btn_a) ? TFT_RED   : TFT_DARKGREY);
        //   layoutSprite.fillRect(idx * 10, 40, 9, 9, (stack[idx] & bit_btn_b) ? TFT_GREEN : TFT_DARKGREY);
        //   layoutSprite.fillRect(idx * 10, 50, 9, 9, (stack[idx] & bit_btn_c) ? TFT_BLUE  : TFT_DARKGREY);
        // }
        
  // phbs.loop();

  delay(1);
  // 入力状態の更新（変化がなければ終了）
  // if (!phbs.loop()) return;

  // ボタン入力履歴を画面に描画
  auto stack = phbs.getStack();
  for (std::size_t idx = 0; idx < BTN_STACK_MAX; ++idx)
  {
    _lcd.fillRect(idx * 5, 100, 4, 4, (stack[idx] & bit_btn_a) ? TFT_RED   : TFT_DARKGREY);
    _lcd.fillRect(idx * 5, 105, 4, 4, (stack[idx] & bit_btn_b) ? TFT_GREEN : TFT_DARKGREY);
    _lcd.fillRect(idx * 5, 110, 4, 4, (stack[idx] & bit_btn_c) ? TFT_BLUE  : TFT_DARKGREY);
  }

  // // コマンド判定結果を取得
  // static std::uint32_t hitvalue = ~0U;
  // std::uint32_t prev = hitvalue;
  // hitvalue = phbs.getValue();

  // // 以前の値と同じ場合は除外
  // if (prev == hitvalue) return;

  // // 入力内容を画面とシリアルに出力
  // switch (hitvalue)
  // {
  // default:  _lcd.println("--"); break;
  // case 101: _lcd.println("A click"); break;
  // case 102: _lcd.println("B click"); break;
  // case 103: _lcd.println("C click"); break;
  // case 111: _lcd.println("A hold"); break;
  // case 112: _lcd.println("B hold"); break;
  // case 113: _lcd.println("C hold"); break;
  // case 121: _lcd.println("A double click"); break;
  // case 122: _lcd.println("B double click"); break;
  // case 123: _lcd.println("C double click"); break;
  // case 201: _lcd.println("AB hold"); break;
  // case 202: _lcd.println("AC hold"); break;
  // case 203: _lcd.println("BC hold"); break;
  // case 204: _lcd.println("ABC hold"); break;
  // case 301: _lcd.println("A->B->C"); break;
  // }
}


void LovyanGFX_DentaroUI::begin( LGFX& _lcd, int _colBit, int _rotateNo, bool _calibF )
{
  _lcd.init();
  _lcd.begin();
  _lcd.startWrite();//CSアサート開始
  _lcd.setColorDepth( _colBit );
  touchCalibrationF = _calibF;
  begin(_lcd);
  _lcd.setRotation( _rotateNo );
    for(int i = 0; i < BUF_PNG_NUM; i++)
  {
    createLayout( 0, 0, 256, 256, layoutSprite_list[i], MULTI_EVENT );//レイアウト用のスプライトを作る
  }
  // for(int objId = 0; objId < BUF_PNG_NUM; objId++){
  //   preReadXtileNo[objId] = -1;
  //   preReadYtileNo[objId] = -1;
  // }
}

void LovyanGFX_DentaroUI::begin( LGFX& _lcd)
{
  lcdPos.x = 0;
  lcdPos.y = 0;
  if(uiMode == TOUCH_MODE){
  }else if(uiMode == PHYSICAL_MODE){
    selectBtnID = 0;
  }
  // タッチが使用可能な場合のキャリブレーションを行います。（省略可）
  if(touchCalibrationF == true){
    if (_lcd.touch())
    {
      if (_lcd.width() < _lcd.height()) _lcd.setRotation(_lcd.getRotation() ^ 1);

      // 画面に案内文章を描画します。
      _lcd.setTextDatum(textdatum_t::middle_center);
      _lcd.drawString("touch the arrow marker.", _lcd.width()>>1, _lcd.height() >> 1);
      _lcd.setTextDatum(textdatum_t::top_left);

      // タッチを使用する場合、キャリブレーションを行います。画面の四隅に表示される矢印の先端を順にタッチしてください。
      uint16_t fg = TFT_WHITE;
      uint16_t bg = TFT_BLACK;

      if (_lcd.isEPD()) swap(fg, bg);
      _lcd.calibrateTouch(nullptr, fg, bg, max(_lcd.width(), _lcd.height()) >> 3);

      _lcd.fillScreen(TFT_BLACK);
      _lcd.setColorDepth(COL_DEPTH);

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
  //heap_caps_get_info()multi_heap_info_t上記の関数からの情報に加えて、ヒープ固有の追加データ（割り当て数など）を含む構造体を返します。
  //Skip
  // heap_caps_print_heap_info()が返す情報の要約をstdoutに出力しheap_caps_get_info()ます。
  //Serial.printf("heap_caps_print_heap_info(MALLOC_CAP_INTERNAL) :\n");
  //heap_caps_print_heap_info(MALLOC_CAP_INTERNAL);
  // heap_caps_dump()そしてheap_caps_dump_all()意志出力は、ヒープ内の各ブロックの構造に関する情報を詳述します。これは大量の出力になる可能性があることに注意してください。
  //Serial.printf("heap_caps_dump() :\n");
  //heap_caps_dump(MALLOC_CAP_INTERNAL);
          // Serial.printf("heap_caps_get_free_size(MALLOC_CAP_SPIRAM)            : %6d\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM) );
          // Serial.printf("heap_caps_get_free_size(MALLOC_CAP_DMA):%d\n", heap_caps_get_free_size(MALLOC_CAP_DMA) );
          // Serial.printf("heap_caps_get_largest_free_block(MALLOC_CAP_DMA):%d\n", heap_caps_get_largest_free_block(MALLOC_CAP_DMA) );
          // Serial.printf("Width:%d, Height:%d\n", 256, 256);
      //    void *p = sprite1.createSprite(256, 256);
      //    if ( p == NULL ) {
      //      Serial.println("メモリが足りなくて確保できない");
      //    }
    }

  }
  Serial.println("");
  Serial.println("[UI_ID information]");
}

void LovyanGFX_DentaroUI::runTouchCalibration(LGFX& _lcd){

    if (_lcd.touch())
    {
      //renameFile(SPIFFS, "/config.txt", fileName);
      if (_lcd.width() < _lcd.height()) _lcd.setRotation(_lcd.getRotation() ^ 1);

      // 画面に案内文章を描画します。
      _lcd.setTextDatum(textdatum_t::middle_center);
      _lcd.drawString("touch the arrow marker.", _lcd.width()>>1, _lcd.height() >> 1);
      _lcd.setTextDatum(textdatum_t::top_left);

      // タッチを使用する場合、キャリブレーションを行います。画面の四隅に表示される矢印の先端を順にタッチしてください。
      uint16_t fg = TFT_WHITE;
      uint16_t bg = TFT_BLACK;

      if (_lcd.isEPD()) swap(fg, bg);
      _lcd.calibrateTouch(nullptr, fg, bg, max(_lcd.width(), _lcd.height()) >> 3);

    }
  }

void LovyanGFX_DentaroUI::update( LGFX& _lcd )
{
  _lcd.getTouch(&tp);
  for(int i = 0; i < 4; i++)
  {
    clist[i+1] = clist[i];
  }
  if(eventState != NO_EVENT) preEventState = eventState;//前のイベント状態を保持

  //以下イベント取得処理
  eventState = NO_EVENT;//イベント初期化
  flickState = NO_EVENT;//-1で初期化
  obj_ret.reset();

  //1ビット左にシフト
  touchState = touchState<<1;

  clist[0] = TFT_WHITE;

  if ( _lcd.getTouch( &tp ) )
  {
    bitSet(touchState, 0);//右０ビット目を１（Set）に
  }else{
    bitClear(touchState, 0);//右０ビット目を0（Clear）に
  }
  //下位２ビットをみてイベントを振り分け
  touchState &= B00000011;
  if (touchState == B_TOUCH){
    sp = tp; sTime =  micros();
    eventState = TOUCH;//6
  }
  else if (touchState == B_MOVE) {
    clist[0] = TFT_GREEN;
    unsigned long touchedTime = micros() - sTime;
    eventState = WAIT;
    if(touchedTime > 160000){
      clist[0] = TFT_YELLOW;
      eventState = MOVE;//8
    }
  }
  else if (touchState == B_RELEASE)
  {
    clist[0] = TFT_BLUE;
    unsigned long touchedTime = micros() - sTime;
    float dist = sqrt( pow((tp.x - sp.x),2) + pow((tp.y - sp.y),2) );

    if(touchedTime > 120000&&touchedTime <= 160000){
      if(dist <= FLICK_DIST){
        eventState = WAIT;//5
      }
    }
    else if(touchedTime > 160000)
    {
      eventState = DRAG;//2
    }
    else if(dist <= FLICK_DIST){
      eventState = TAPPED;//4

      float dist2 = sqrt( pow((tp.x - sp2.x),2) + pow((tp.y - sp2.y),2) );
      if(sTime - tappedTime < 200000 && dist2 < FLICK_DIST ){//ダブルタップの間の時間調整, 位置判定
        eventState = WTAPPED;//5
      }
      tappedTime = micros();
      sp2 = tp;
    }
    else if(dist > FLICK_DIST)
    {
       if(touchedTime <= 160000){
        float angle = getAngle(sp, tp);

          if(angle <= 22.5 || angle > 337.5){
            eventState = RIGHT_FLICK;//0
            flickState = RIGHT_FLICK;//0
          }else if(angle <= 67.5 && angle > 22.5){
            eventState = R_D_FLICK;//7
            flickState = R_D_FLICK;//7
          }else if(angle <= 112.5 && angle > 67.5){
            eventState = DOWN_FLICK;//6
            flickState = DOWN_FLICK;//6
          }else if(angle <= 157.5 && angle > 112.5){
            eventState = D_L_FLICK;//5
            flickState = D_L_FLICK;//5
          }else if(angle <= 202.5 && angle > 157.5){
            eventState = LEFT_FLICK;//4
            flickState = LEFT_FLICK;//4
          }else if(angle <= 247.5 && angle > 202.5){
            eventState = L_U_FLICK;//3
            flickState = L_U_FLICK;//3
          }else if(angle <= 292.5 && angle > 247.5){
            eventState = UP_FLICK;//2
            flickState = UP_FLICK;//2
          }else if(angle <= 337.5 && angle > 292.5){
            eventState = U_R_FLICK;//1
            flickState = U_R_FLICK;//1
          }

       }
    }
    eventState = RELEASE;//9
  }else if (touchState == B_UNTOUCH) {
    clist[0] = TFT_DARKGREEN;
  }

  for(int i = 0; i < uiBoxes_num; i++){
    for(int j = 0; j<uiBoxes[i].b_num; j++){
      addHandler(uiBoxes[i].b_sNo + j, j, ret0_DG, uiBoxes[i].eventNo, uiBoxes[i].parentID);
      addHandler(uiBoxes[i].b_sNo + j, j, ret1_DG, uiBoxes[i].eventNo, uiBoxes[i].parentID);
    }
  }
}

void LovyanGFX_DentaroUI::updateSelectBtnID(int _selectBtnID){
 selectBtnID = _selectBtnID;
}

void LovyanGFX_DentaroUI::showTouchEventInfo(LovyanGFX& _lgfx, int _x, int _y){
  for(int i = 0; i < 1; i++){
    _lgfx.fillRect(_x + 1 + 10*i, _y, 9,9,clist[0]);
  }
}

void LovyanGFX_DentaroUI::createToggles(int _x, int _y, int _w,int _h, int _row, int _col, LGFX_Sprite& _uiSprite, int _eventNo){
  toggle_mode = true;
  createBtns( _x, _y, _w, _h, _row, _col, _eventNo);
  toggle_mode = false;
}


void LovyanGFX_DentaroUI::setLayoutSpritePos(int _LayoutSpritePosx, int _LayoutSpritePosy){
  layoutSpritePos = getTouchPoint(_LayoutSpritePosx, _LayoutSpritePosy);
}

void LovyanGFX_DentaroUI::createBtns(//修正中
  int _x, int _y,
  int _w,int _h,
  int _row, int _col,
  int _eventNo)
  {//縦方向に並ぶ
    touchZoom = 1;
    createBtns(_x, _y, _w, _h, _row, _col, _eventNo, touchZoom);
  }

void LovyanGFX_DentaroUI::createBtns(
  int _x, int _y,
  int _w,int _h,
  int _row, int _col,
  int _eventNo, int _touchZoom)
  {//縦方向に並ぶ
  touchZoom = _touchZoom;

uiBoxes[uiID] = *new UiContainer;
// uiBoxes.push_back(*new UiContainer);

//   // if(uiID<128){
    uiBoxes[uiID].label = "BTN_" + String(uiID);
    Serial.println("BTN_" + String(uiID)  + "=[" + String(uiID) + "]" + String(btnID)+"~");
//
    int _startId = btnID;//スタート時のボタンIDをセット
    uiBoxes[uiID].b_sNo = btnID;
    uiBoxes[uiID].id  = uiID;
    uiBoxes[uiID].x   = _x*touchZoom;
    uiBoxes[uiID].y   = _y*touchZoom;
    uiBoxes[uiID].w   = _w*touchZoom;
    uiBoxes[uiID].h   = _h*touchZoom;
    uiBoxes[uiID].row = _row;
    uiBoxes[uiID].col = _col;
    uiBoxes[uiID].eventNo = _eventNo;
    int b_w = int(uiBoxes[uiID].w/uiBoxes[uiID].row);
    int b_h = int(uiBoxes[uiID].h/uiBoxes[uiID].col);

    Serial.print(uiBoxes[uiID].row);
    Serial.print(":");
    Serial.println(uiBoxes[uiID].col);

for(int j= 0; j < uiBoxes[uiID].col; j++)
{
    for(int i= 0; i < uiBoxes[uiID].row; i++)
      {
        int p_btnID =  _startId + j * uiBoxes[uiID].row + i;

        touch_btn_list.push_back(new TouchBtn( lcd ));//ボタン数分増やしていく

          touch_btn_list[p_btnID]->initBtn(p_btnID, String(p_btnID),
          i*b_w,
          j*b_h,
          b_w,
          b_h,
          String(p_btnID),
          layoutSpritePos,
          getTouchPoint(uiBoxes[uiID].x, uiBoxes[uiID].y),
          TOUCH_BTN_MODE);

        touch_btn_list[ p_btnID ]->setVisibleF( true );
        touch_btn_list[ p_btnID ]->setAvailableF( true );

        Serial.print("[");
        Serial.print(p_btnID);
        Serial.print("]");
        btnID++;//ボタンを更新
      }
  }

  uiBoxes[uiID].b_num =  btnID - uiBoxes[uiID].b_sNo;//UIのボタン数をセット
  Serial.println("e");

  uiBoxes_num++;
  uiID++;
}

void LovyanGFX_DentaroUI::setAvailableF(int uiID, int _btnID, bool _available){
  int _startId = uiBoxes[uiID].b_sNo;//スタート時のボタンIDをセット
  touch_btn_list[_startId + _btnID]->setAvailableF(_available);
}

void LovyanGFX_DentaroUI::setAllBtnAvailableF(int uiID, bool _available){
  int _startId = uiBoxes[uiID].b_sNo;//スタート時のボタンIDをセット
  for(int i= 0; i < uiBoxes[uiID].row; i++)
    {
      for(int j= 0; j < uiBoxes[uiID].col; j++)
      {
        touch_btn_list[_startId + j*uiBoxes[uiID].row + i]->setAvailableF(_available);
      }
    }
}

void LovyanGFX_DentaroUI::setVisibleF(int uiID, int _btnID, bool _visible){
  int _startId = uiBoxes[uiID].b_sNo;//スタート時のボタンIDをセット
  touch_btn_list[_startId + _btnID]->setVisibleF(_visible);
}

void LovyanGFX_DentaroUI::setAllBtnVisibleF(int uiID, bool _visible){
  int _startId = uiBoxes[uiID].b_sNo;//スタート時のボタンIDをセット
  for(int i= 0; i < uiBoxes[uiID].row; i++)
    {
      for(int j= 0; j < uiBoxes[uiID].col; j++)
      {
        touch_btn_list[_startId + j*uiBoxes[uiID].row + i]->setVisibleF(_visible);
      }
    }
}

void LovyanGFX_DentaroUI::setLayoutPos( int _x, int _y )
{
  layoutSpritePos.x = _x;//表示用レイアウト座標
  layoutSpritePos.y = _y;
  layoutSprite_w =240;
  layoutSprite_h =240;

  setLayoutPosToAllBtn(getTouchPoint(_x, _y ));
}


void LovyanGFX_DentaroUI::createLayout( int _x, int _y, int _w, int _h, LGFX_Sprite& _layoutSprite, int _eventNo = MULTI_EVENT){
    uiBoxes_num++;
    uiID++;
    uiBoxes[uiID] = *new UiContainer;

    Serial.println("LAYOUT_" + String(uiID)  + "=[" + String(uiID) + "]" + String(btnID)+"~");
    uiBoxes[uiID].label = "LAYOUT_" + String(uiID);
    uiBoxes[uiID].b_sNo = 0;
    uiBoxes[uiID].id  = uiID;
    uiBoxes[uiID].x   = _x;
    uiBoxes[uiID].y   = _y;
    uiBoxes[uiID].w   = _w;
    uiBoxes[uiID].h   = _h;

    uiBoxes[uiID].eventNo = _eventNo;

    layoutSpritePos.x = _x;
    layoutSpritePos.y = _y;
    layoutSprite_w = _w;
    layoutSprite_h = _h;

    _layoutSprite.setPsram( false );
    _layoutSprite.setColorDepth(16);//親スプライトの色深度１６
    _layoutSprite.createSprite(layoutSprite_w, layoutSprite_h);
    _layoutSprite.setPaletteGrayscale();
}

void LovyanGFX_DentaroUI::createSliders(int _x, int _y, int _w, int _h,  int _row, int _col,
LGFX_Sprite& _uiSprite,
int _xy_mode,
int _eventNo)
{
  uiBoxes[uiID] = *new UiContainer;
  uiBoxes_num++;
  uiID++;
  uiBoxes[uiID].label = "SLIDER_" + String(uiID);
  Serial.println("SLIDER_" + String(uiID) + "=[" + String(uiID) + "]");
  int _startId = btnID;
  uiBoxes[uiID].b_sNo = btnID;
  uiBoxes[uiID].id  = uiID;
  uiBoxes[uiID].x   = _x;
  uiBoxes[uiID].y   = _y;
  uiBoxes[uiID].w   = _w;
  uiBoxes[uiID].h   = _h;
  uiBoxes[uiID].row = _row;
  uiBoxes[uiID].col = _col;
  uiBoxes[uiID].eventNo = _eventNo;

  _uiSprite.setPsram( false );
  _uiSprite.setColorDepth( COL_DEPTH );
  _uiSprite.createSprite( _w, _h );

  int s_w = int( _w/_row );
  int s_h = int( _h/_col );

  for( int i= 0; i < _row; i++ )
    {
    for( int j= 0; j < _col; j++ )
    {
    btnID = _startId + j*_row + i;
    touch_btn_list[_startId + j*_row + i] = NULL;
    touch_btn_list[_startId + j*_row + i] = new TouchBtn( lcd );
    touch_btn_list[_startId + j*_row + i]->initSlider( _startId + j*_row + i,
    i*s_w,
    j*s_h,
    s_w,
    s_h,
    String( _startId + j*_row + i ),
    layoutSpritePos,
    getTouchPoint(_x, _y),
    _uiSprite,
    _xy_mode);

    btnID++;
    }
  }
  uiBoxes[uiID].b_num =  btnID - uiBoxes[uiID].b_sNo;
}



void LovyanGFX_DentaroUI::setQWERTY(int uiID, String _btnsString, LGFX_Sprite& _sprite)
{
  int charsNum = _btnsString.length();
  const char* btnChars = _btnsString.c_str();
  int _id = uiBoxes[uiID].b_sNo;

  for(int i=0; i<charsNum; i++){
    touch_btn_list[_id + i]->setVisibleF(true);
    touch_btn_list[_id + i]->setBtnName(String(btnChars[i]));
  }
}

void LovyanGFX_DentaroUI::setBtnName( int _btnNo, String _btnName, String _btnNameFalse){
  int _btnId = _btnNo;
  touch_btn_list[_btnId]->setBtnName(_btnName);
  touch_btn_list[_btnId]->setBtnNameFalse(_btnNameFalse);//トグルのfalse状態の時のラベルを設定
}

void LovyanGFX_DentaroUI::setBtnName( int _btnNo, String _btnName)
{
  int _btnId = _btnNo;
  touch_btn_list[_btnId]->setAvailableF(true);
  touch_btn_list[_btnId]->setVisibleF(true);
  touch_btn_list[_btnId]->setBtnName(_btnName);
}

void LovyanGFX_DentaroUI::drawBtns( int uiID, LovyanGFX* _lgfx, LGFX_Sprite& _uiSprite, int _x, int _y)
{
  lgfx = _lgfx;
  if( getEvent() != NO_EVENT ){
  if(getEvent() == uiBoxes[uiID].eventNo || uiBoxes[uiID].eventNo == MULTI_EVENT)
  {
    int _id = uiBoxes[uiID].b_sNo;
    int _btnNum = uiBoxes[uiID].b_num;
    _uiSprite.setPivot( 0, 0 );//setPivot()で回転する場合の原点を指定します。初期値は左上の(0, 0)だと思います

    for(int i= _id; i < _id + _btnNum; i++)
    {
      touch_btn_list[i]->setSelectBtnF(false);

      if(selectBtnID == i)touch_btn_list[i]->setSelectBtnF(true);
      else touch_btn_list[i]->setSelectBtnF(false);
      touch_btn_list[i]->btnDraw(_uiSprite);
    }
    _uiSprite.pushSprite(_lgfx, uiBoxes[uiID].x, uiBoxes[uiID].y);

    }
  }
}

void LovyanGFX_DentaroUI::drawSliders(int _uiID, LovyanGFX* _lgfx, LGFX_Sprite& _uiSprite)
{
  drawSliders( _uiID, _lgfx, _uiSprite, uiBoxes[_uiID].x, uiBoxes[_uiID].y);
}

void LovyanGFX_DentaroUI::drawSliders(int uiID, LovyanGFX* _lgfx, LGFX_Sprite& _uiSprite, int _x, int _y)
{
  if( getEvent() != NO_EVENT ){
    if( getEvent() == uiBoxes[uiID].eventNo || uiBoxes[uiID].eventNo == MULTI_EVENT)
    {
    int _id = uiBoxes[uiID].b_sNo;
    int _btnNum = uiBoxes[uiID].b_num;
    _uiSprite.setPivot(0, 0);//setPivot()で回転する場合の原点を指定します。初期値は左上の(0, 0)だと思います

    for(int i = _id; i < _id + _btnNum; i++)
    {
      touch_btn_list[i]->sliderDraw(_uiSprite, tp);
    }
    _uiSprite.pushSprite(_lgfx, _x, _y);
    }
  }
}

void LovyanGFX_DentaroUI::drawBtns(int _uiID, LovyanGFX* _lgfx, LGFX_Sprite& _uiSprite){
  toggle_mode = false;
  drawBtns( _uiID, _lgfx, _uiSprite, uiBoxes[_uiID].x, uiBoxes[_uiID].y);
}

void LovyanGFX_DentaroUI::drawToggles(int _uiID, LovyanGFX* _lgfx, LGFX_Sprite& _uiSprite){
  drawToggles(_uiID, _lgfx, _uiSprite, uiBoxes[_uiID].x, uiBoxes[_uiID].y);
}

void LovyanGFX_DentaroUI::drawToggles(int _uiID, LovyanGFX* _lgfx, LGFX_Sprite& _uiSprite, int _x, int _y){
  toggle_mode = true;
  drawBtns(_uiID, _lgfx, _uiSprite, _x, _y);
  toggle_mode = false;
}


void LovyanGFX_DentaroUI::drawLayOut(LGFX_Sprite& _layoutSprite){
    _layoutSprite.pushSprite(layoutSpritePos.x, layoutSpritePos.y);//最終的な出力
}

bool LovyanGFX_DentaroUI::isAvailable( int _btnID ){
  return touch_btn_list[_btnID]->getAvailableF();
}

void LovyanGFX_DentaroUI::addHandler( int _btnID, int _btnNo, DelegateBase2* _func, uint16_t _runEventNo, int _parentID){
  runEventNo = _runEventNo;
  _parentID = 0;//ベータ版ではとりあえず強制的にLCDのみのイベントをとる

  touch_btn_list[_btnID]->addHandler(_func);

  if( parentID == PARENT_LCD ) touch_btn_list[_btnID]->run2( _btnID, _btnNo, sp, tp, eventState, runEventNo);//ボタン内のイベントかチェック
  else if( parentID == PARENT_SPRITE ) touch_btn_list[_btnID]->run2( _btnID, _btnNo, sp, tp, eventState, runEventNo);//ボタン内のイベントかチェック

  touch_btn_list[_btnID]->delHandlers2();
}

float LovyanGFX_DentaroUI::getAngle(lgfx::v1::touch_point_t a, lgfx::v1::touch_point_t b ){

  float r = atan2( b.y - a.y, b.x - a.x );
  if (r < 0) {
        r = r + 2 * M_PI;
    }
    return r * 360 / (2 * M_PI);
}

float LovyanGFX_DentaroUI::getDist(lgfx::v1::touch_point_t a, lgfx::v1::touch_point_t b ){
   float d = sqrt((b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y));
    return d;
}

int LovyanGFX_DentaroUI::getPreEvent()
{
  return preEventState;
}

void LovyanGFX_DentaroUI::circle(LovyanGFX* _lgfx,  uint16_t c, int fillF)
{
  if(fillF){
    _lgfx->fillCircle(tp.x, tp.y, 10, c);
    _lgfx->fillCircle(tp.x, tp.y, 10, c);
  }else{
    _lgfx->drawCircle(tp.x, tp.y, 10, c);
    _lgfx->drawCircle(tp.x, tp.y, 10, c);
  }
}

void LovyanGFX_DentaroUI::rect(LovyanGFX* _lgfx, uint16_t c, int fillF)
{
  if(fillF){
    _lgfx->fillRect(tp.x, tp.y, 10, 10, c);
    _lgfx->fillRect(tp.x, tp.y, 10, 10, c);
  }else{
    _lgfx->drawRect(tp.x, tp.y, 10, 10, c);
    _lgfx->drawRect(tp.x, tp.y, 10, 10, c);
  }
}

lgfx::v1::touch_point_t LovyanGFX_DentaroUI::getPos()
{
  return tp;
}

lgfx::v1::touch_point_t LovyanGFX_DentaroUI::getStartPos()
{
  return sp;
}

void LovyanGFX_DentaroUI::setPos(int _x, int _y){
  tp.x = _x;
  tp.y = _y;
}

void LovyanGFX_DentaroUI::setStartPos(int _x, int _y){
  sp.x = _x;
  sp.y = _y;
}

int LovyanGFX_DentaroUI::getTouchBtnID(){
  return obj_ret.btnID;
}

int LovyanGFX_DentaroUI::getTouchBtnNo(){
  // return getTouchBtnID() - getUiFirstNo(FlickUiID);
}

int LovyanGFX_DentaroUI::getEvent(){
  return eventState;
}

int LovyanGFX_DentaroUI::getFlickEvent(){
  return flickState;
}

lgfx::v1::touch_point_t LovyanGFX_DentaroUI::getTouchPoint(int _x, int _y){
  lgfx::v1::touch_point_t tp;
  tp.x = _x;
  tp.y = _y;
  return tp;
}

float LovyanGFX_DentaroUI::getSliderVal(int uiID, int _btnNo){
  float retVal = 1.0;
  int _btnID = uiBoxes[uiID].b_sNo + _btnNo;
  if(touch_btn_list[_btnID]->get_xy_mode() == X_VAL){retVal = touch_btn_list[_btnID]->getSliderValx();}
  else if(touch_btn_list[_btnID]->get_xy_mode() == Y_VAL){retVal = touch_btn_list[_btnID]->getSliderValy();}
  return retVal;
}

float LovyanGFX_DentaroUI::getSliderVal(int uiID,int _btnNo, int _xy){
  float retVal = 1.0;
  int _btnID = uiBoxes[uiID].b_sNo + _btnNo;
  if(_xy == X_VAL){retVal = touch_btn_list[_btnID]->getSliderValx();}
  else if(_xy == Y_VAL){retVal = touch_btn_list[_btnID]->getSliderValy();}
  return retVal;
}

Vec2 LovyanGFX_DentaroUI::getSliderVec2(int uiID, int _btnNo){//XYを一気に取得
  Vec2  retPos;
  int _btnID = uiBoxes[uiID].b_sNo + _btnNo;
  if(touch_btn_list[_btnID]->get_xy_mode() == XY_VAL){
    retPos.x = touch_btn_list[_btnID]->getSliderValx();
    retPos.y = touch_btn_list[_btnID]->getSliderValy();
  }
  return retPos;
}

void LovyanGFX_DentaroUI::setSliderVal(int uiID, int _btnNo, float _x, float _y){
  int _btnID = uiBoxes[uiID].b_sNo + _btnNo;
  touch_btn_list[_btnID]->setSliderVal(_x, _y);
}


// void LovyanGFX_DentaroUI::setCharMode(int _charMode){
//   charMode = _charMode;
// }

// int LovyanGFX_DentaroUI::getCharMode(){
//   return charMode;
// }

bool LovyanGFX_DentaroUI::getToggleVal(int _uiID, int _btnNo){
  int _btnID = uiBoxes[uiID].b_sNo + _btnNo;
  return touch_btn_list[_btnID]->getToggleVal();
}

int LovyanGFX_DentaroUI::getUiFirstNo(int uiID){
  return uiBoxes[uiID].b_sNo;
}

int LovyanGFX_DentaroUI::getUiBtnNum(int uiID){
  return uiBoxes[uiID].b_num;
}

int LovyanGFX_DentaroUI::getUiBoxNum(){
  return uiBoxes_num;
}

int LovyanGFX_DentaroUI::getAllBtnNum(){
  return btnID;
}

int LovyanGFX_DentaroUI::getParentID(){
  return parentID;
}

int LovyanGFX_DentaroUI::getUiID( const char* _uiLabel){
  int i = 0;
  while(uiBoxes[i].label != String(_uiLabel)){
    i++;
  }
  return i;
}

lgfx::v1::touch_point_t LovyanGFX_DentaroUI::getBtnPos(int _btnID){
  return getTouchPoint(touch_btn_list[_btnID]->getBtnPos().x, touch_btn_list[_btnID]->getBtnPos().y);
}

void LovyanGFX_DentaroUI::showInfo(LovyanGFX& _lgfx , int _infox, int _infoy){
  //フレームレート情報などを表示します。
  _lgfx.setTextSize(1);
  _lgfx.setFont(&lgfxJapanGothicP_8);
  // _lgfx.fillRect( _infox, _infoy, 150, 10, TFT_BLACK );
  _lgfx.setTextColor( TFT_WHITE , TFT_BLACK );
  _lgfx.setCursor( _infox,_infoy );
  _lgfx.print( fps );
  _lgfx.print( ":" );
  _lgfx.print( String( getEvent() ) );
  _lgfx.print( "[" );
  _lgfx.print( String( getPreEvent() ) );
  _lgfx.print( "]:BTN" );
  _lgfx.print( "[" );
  _lgfx.print( String( getFlickEvent() ) );
  _lgfx.print( "]:BTN" );
  _lgfx.println( String( getTouchBtnID() ) );
  ++frame_count;sec = millis() / 1000;
  if ( psec != sec ) {
    psec = sec; fps = frame_count;
    frame_count = 0;
    _lgfx.setAddrWindow( 0, 0, _lgfx.width(), _lgfx.height() );
  }
}

void LovyanGFX_DentaroUI::setLayoutPosToAllBtn( lgfx::v1::touch_point_t  _layoutPos ){
  for(int i=0; i < getAllBtnNum(); i++){
    touch_btn_list[ i ]->setlayoutSpritePos(_layoutPos);
  }
}

