#ifndef BASE_GAME_H
#define BASE_GAME_H

#define CTRLBTNNUM 1//ゲームに使えるサイドボタン数を決めます9
#define CALIBRATION_FILE "/init/param/caldata.txt"
#define MAPSPRNOS_FILE "/init/param/mapsprnos.txt"//色番号に対応するスプライトを登録するファイル
#define WIFIPASS_FILE "/init/param/wifipass.txt"
#define SPRBITS_FILE "/init/param/sprbits.txt"
#define FAST_MODE 1
#define WIDE_MODE 2 //倍率になっています。

#define TFT_OFFSET_X 60
#define TFT_OFFSET_Y 0
#define RAD_ONE PI/180

#define PNG_SPRITE_WIDTH 64
#define PNG_SPRITE_HEIGHT 128

#define OUTPIN_0 16
#define INPIN_0  17

class BaseGame
{
  public:
    bool wifiDebugRequest = false;//外部ファイルから書き換えてWifiモードにできる
    bool wifiDebugSelf = false;

    void setWifiDebugRequest(bool _b){wifiDebugRequest = _b;};//外部ファイルから書き換えてWifiモードにできる
    void setWifiDebugSelf(bool _b){wifiDebugSelf = _b;};

  BaseGame(){}
  virtual void init(){};
  virtual int run(int remainTime){return 0;};
  virtual void pause(){};
  virtual void resume(){};

};


#endif


