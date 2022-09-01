#include "TouchBtn.hpp"

TouchBtn::TouchBtn(LGFX* _lcd)
{
  lcd = _lcd;
}

void TouchBtn::setlayoutSpritePos(lgfx::v1::touch_point_t _layoutSpritePos)
{
  layoutSpritePos = _layoutSpritePos;
}

void TouchBtn::initBtn(int _btnID, String _btnIDlabel, int _b_x, int _b_y, int _b_w, int _b_h, String _btn_name,
lgfx::v1::touch_point_t _layoutSpritePos,
lgfx::v1::touch_point_t _uiSpritePos,
int _btn_mode)
{
  btnID = _btnID;
  btnIDlabel = _btnIDlabel;
  uiSpritePos = _uiSpritePos;
  b_x = _b_x;
  b_y = _b_y;
  b_w = _b_w;
  b_h = _b_h;
  b_hw = b_w/2;
  b_hh = b_h/2;
  btn_name = _btn_name;
  btn_mode = _btn_mode;
  layoutSpritePos = _layoutSpritePos;
}

void TouchBtn::initSlider( int _btnID, int _s_x, int _s_y, int _s_w, int _s_h, String _btn_name, 
lgfx::v1::touch_point_t _layoutSpritePos,
lgfx::v1::touch_point_t _uiSpritePos,
LGFX_Sprite& _sprite,
int _xy_mode )
{
  btnID = _btnID;
  btn_mode = TOUCH_SLIDER_MODE;
  uiSpritePos = _uiSpritePos;
  s_x = _s_x; 
  s_y = _s_y;
  s_w = _s_w; 
  s_h = _s_h;
  s_hw = s_w/2; 
  s_hh = s_h/2;
  btn_name = _btn_name;
  xy_mode = _xy_mode;
  sliderPosx = _s_x + (_s_w/2);
  sliderPosy = _s_x + (_s_h/2);
  layoutSpritePos = _layoutSpritePos;
}

void TouchBtn::setBtnNameFalse(String _btnNameFalse)
{
  btn_nameFalse = _btnNameFalse;
}

void TouchBtn::btnDraw(LGFX_Sprite& _uiSprite)
{
  _uiSprite.setTextSize(1);
  _uiSprite.setFont(&lgfxJapanGothicP_20);
  String drawName = "";
  
  if(this->btn_mode == TOUCH_TOGGLE_MODE)//トグルボタンの時
  {
    if(this->toggleVal == true){
      _uiSprite.fillRoundRect(this->b_x, this->b_y, this->b_w, this->b_h, 10, TFT4_WHITE);
      _uiSprite.setTextColor(TFT4_BLACK);
      drawName = btn_name;
      }
    else if(this->toggleVal == false){
      _uiSprite.fillRoundRect(this->b_x, this->b_y, this->b_w, this->b_h, 10, TFT4_BLACK);
      _uiSprite.setTextColor(TFT4_WHITE);
      drawName = btn_nameFalse;
      }
    b_str_hw = _uiSprite.textWidth(drawName)/2;
    _uiSprite.drawString(drawName, b_x + b_hw - b_str_hw , b_y + b_hh - 8);
  }
  else if(this->btn_mode == TOUCH_BTN_MODE)//普通のボタンの時
  {
    if( visibleF == true ){
      _uiSprite.fillRoundRect(this->b_x, this->b_y, this->b_w, this->b_h, 10, TFT4_WHITE);
      _uiSprite.setTextColor(TFT4_BLACK);
      //_uiSprite.setFont(&lgfxJapanGothicP_20);
      // drawName = btn_name;
      b_str_hw = _uiSprite.textWidth(btn_name)/2;
      _uiSprite.drawString(btn_name, b_x + b_hw - b_str_hw , b_y + b_hh - 8);
    }
  }
  else if( this->btn_mode == TOUCH_FLICK_MODE )//フリックボタンの時
  {
    if( visibleF == true )
    {
      _uiSprite.fillRoundRect( this->b_x, this->b_y, this->b_w, this->b_h, 10, TFT4_WHITE );
      _uiSprite.setTextColor( TFT4_BLACK );
      //_uiSprite.setFont(&lgfxJapanGothicP_20);
      // drawName = btnIDlabel;//btn_name;
      b_str_hw = _uiSprite.textWidth( btn_name ) / 2;
      _uiSprite.drawString( btn_name, b_x + b_hw - b_str_hw , b_y + b_hh - 8 );
    }
  }

  if(selectBtnF){
    _uiSprite.drawRoundRect(this->b_x, this->b_y, this->b_w, this->b_h, 10, TFT4_WHITE);
  }else{
    _uiSprite.drawRoundRect(this->b_x, this->b_y, this->b_w, this->b_h, 10, TFT4_BLACK);
  }
}

void TouchBtn::sliderDraw(LGFX_Sprite& _uiSprite, lgfx::v1::touch_point_t _tp)
{
  _uiSprite.fillRect(this->s_x, this->s_y, this->s_w, this->s_h, TFT4_BLACK);
  _uiSprite.setTextSize(1);
  _uiSprite.setTextColor(TFT4_WHITE);
  this->sliderPosx = this->s_x + sliderValx * this->s_w;
  this->sliderPosy = this->s_y + sliderValy * this->s_h;
    
  _uiSprite.fillCircle(this->sliderPosx, 
                         this->sliderPosy, 5, TFT4_WHITE);

  _uiSprite.drawRoundRect(this->s_x, this->s_y, this->s_w, this->s_h, 10, TFT4_WHITE);
  if( xy_mode == Y_VAL||xy_mode==XY_VAL ){
    //_uiSprite.drawString(String(sliderValy), this->s_x + 4 , this->s_y + 16);
    _uiSprite.drawLine(this->s_x, 
                      this->sliderPosy, 
                      this->s_x + this->s_w,
                      this->sliderPosy, TFT4_WHITE);
  }
  if( xy_mode == X_VAL||xy_mode==XY_VAL ){
    //_uiSprite.drawString(String(sliderValx), this->s_x + 4 , this->s_y + 4);
    _uiSprite.drawLine(sliderPosx, 
                      this->s_y, 
                      sliderPosx, 
                      this->s_y + this->s_h , TFT4_WHITE);
  }
  //_uiSprite.drawLine(this->s_x, this->s_x, this->s_x + this->s_w, this->s_x, TFT4_WHITE);
  
  if( btn_mode == TOUCH_BTN_MODE ){
    b_str_hw = _uiSprite.textWidth(btn_name)/2;
    _uiSprite.drawString(btn_name, this->s_x + this->s_hw - b_str_hw , this->s_y + this->s_hh -4);
  }
}

void TouchBtn::setSelectBtnF(bool _selectBtnF){
  selectBtnF = _selectBtnF;
}


void TouchBtn::setAvailableF(bool _availableF){
  availableF = _availableF;
}

bool TouchBtn::getAvailableF(){
  return availableF;
}

void TouchBtn::setVisibleF(bool _visibleF){
  visibleF = _visibleF;
}

bool TouchBtn::getVisibleF(){
  return visibleF;
}

int TouchBtn::getBtnMode(){
  return btn_mode;
}

void TouchBtn::addHandler(DelegateBase2* _func){
  lim2.push_back(_func);
}

void TouchBtn::delHandlers2(){
      list<DelegateBase2*>::iterator ite2 =  lim2.begin();
      while( ite2 != lim2.end() )
      {
        lim2.pop_front();
      }
}

void TouchBtn::run2(int _btnID, int _btnNo, lgfx::v1::touch_point_t _sp, lgfx::v1::touch_point_t _tp, int _eventState, int _runEventNo)
{
  run2(_btnID, _btnNo, _sp.x, _sp.y, _tp.x, _tp.y, _eventState, _runEventNo);
}

void TouchBtn::run2(int _btnID, int _btnNo, int _sx, int _sy, int _tx, int _ty, int _eventState, int _runEventNo){
      sp.x = _sx;
      sp.y = _sy;
      tp.x = _tx;
      tp.y = _ty;

      eventState = _eventState;
      runEventNo = _runEventNo;

      list<DelegateBase2*>::iterator ite2 =  lim2.begin();
      while( ite2 != lim2.end() )
      {
        DelegateBase2 *ptr = (*ite2);

        if(btn_mode == TOUCH_TILE_MODE){
          if(sp.x > b_x 
          && sp.x < b_x + b_w 
          && sp.y > b_y 
          && sp.y < b_y + b_h){//ボタンの領域内に入っていれば
            (*ptr)( _btnID );    // 関数を実行！
          }
        }
        else if(btn_mode == TOUCH_BTN_MODE||btn_mode == TOUCH_TOGGLE_MODE)
        {//普通のボタン用の判定
          if(sp.x > b_x     + layoutSpritePos.x + this->uiSpritePos.x 
          && sp.x < b_x+b_w + layoutSpritePos.x + this->uiSpritePos.x
          && sp.y > b_y     + layoutSpritePos.y + this->uiSpritePos.y
          && sp.y < b_y+b_h + layoutSpritePos.y + this->uiSpritePos.y)
          {//ボタンの領域内に入っていれば
          if(availableF == true){
            (*ptr)( _btnID );    // 関数を実行！
          }
            // if(eventState == runEventNo)this->switchToggleVal();
            if(eventState == runEventNo && eventState != NO_EVENT)this->switchToggleVal();
          }
        }
        else if(btn_mode == TOUCH_FLICK_MODE)//FLICK_MODEもこちらで処理
        {//普通のボタン用の判定
          if(sp.x > b_x     + layoutSpritePos.x + this->uiSpritePos.x 
          && sp.x < b_x+b_w + layoutSpritePos.x + this->uiSpritePos.x
          && sp.y > b_y     + layoutSpritePos.y + this->uiSpritePos.y
          && sp.y < b_y+b_h + layoutSpritePos.y + this->uiSpritePos.y)
          {//ボタンの領域内に入っていれば
          if(availableF == true){
            (*ptr)( _btnID );    // 関数を実行！
          }
            // if(eventState == runEventNo)this->switchToggleVal();
            if(eventState == runEventNo && eventState != NO_EVENT)this->switchToggleVal();
          }
        }
        else if(btn_mode == TOUCH_SLIDER_MODE)
        {//スライダボタン用の判定
          if(sp.x > this->s_x + this->uiSpritePos.x
          && sp.x < this->s_x + this->uiSpritePos.x + this->s_w
          && sp.y > this->s_y + this->uiSpritePos.y
          && sp.y < this->s_y + this->uiSpritePos.y + this->s_h)
          {//ボタンの領域内に入っていれば
            (*ptr)( _btnID );// 関数を実行！
            if( eventState == runEventNo||runEventNo == MULTI_EVENT ){
                this->sliderPosx = tp.x - uiSpritePos.x;
                this->sliderPosy = tp.y - uiSpritePos.y;

                this->sliderValx = float( tp.x - s_x - uiSpritePos.x )/s_w;
                this->sliderValy = float( tp.y - s_y - uiSpritePos.y )/s_h;
            }
            if( (tp.x - uiSpritePos.x) >= s_x && (tp.x - uiSpritePos.x) <= s_x + s_w ){
              if( (tp.y - uiSpritePos.y) >= s_y && (tp.y - uiSpritePos.y) <= s_y + s_h ){
                this->sliderPosx = tp.x - uiSpritePos.x;
                this->sliderPosy = tp.y - uiSpritePos.y;

                this->sliderValx = float(tp.x - s_x - uiSpritePos.x)/s_w;
                this->sliderValy = float(tp.y - s_y - uiSpritePos.y)/s_h;
              }
            }
          }
        }
        ite2++;
        lim2.pop_front();//メソッドを実行したら削除
      }
}

int TouchBtn::getBtnID(){
    return btnID;
}

void TouchBtn::setColor(uint16_t _color){
  color = _color;
}
void TouchBtn::setBgColorIndex(uint16_t _bgColorIndex){
  bgColorIndex = _bgColorIndex;
}

lgfx::v1::touch_point_t TouchBtn::getTouchPoint(int _x, int _y){
  lgfx::v1::touch_point_t tp;
  tp.x = _x;
  tp.y = _y;
  return tp;
}

void TouchBtn::switchToggleVal(){
  this->toggleVal = !this->toggleVal;
}

float TouchBtn::getSliderValx(){
  return this->sliderValx;
}

float TouchBtn::getSliderValy(){
  return this->sliderValy;
}

void TouchBtn::setSliderVal(float _x, float _y){
  this->sliderValx = _x;
  this->sliderValy = _y;
}

bool TouchBtn::getToggleVal(){
  return this->toggleVal;
}

int TouchBtn::get_xy_mode(){
  return xy_mode;
}

lgfx::v1::touch_point_t TouchBtn::getBtnPos(){
  return getTouchPoint(this->b_x, this->b_y);
}


void TouchBtn::setBtnName(String _btn_name){
  btn_name = _btn_name;
}

String TouchBtn::getBtnName()
{
  return btn_name;
}

void TouchBtn::setDrawFinishF(bool _drawFinishF){
  drawFinishF = _drawFinishF;
}

bool TouchBtn::getDrawFinishF(){
  return drawFinishF;
}