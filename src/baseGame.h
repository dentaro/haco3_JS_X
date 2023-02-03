#ifndef BASE_GAME_H
#define BASE_GAME_H

#define CTRLBTNNUM 5//ゲームに使える再度ボタン数を決めます
#define CALIBRATION_FILE "/init/caldata.txt"
#define FAST_MODE 1
#define WIDE_MODE 2 //倍率になっています。

#define TFT_OFFSET_X 64
#define TFT_OFFSET_Y 60

class BaseGame
{
  public:
  BaseGame(){}
  virtual void init(){};
  virtual int run(int remainTime){return 0;};
  virtual void pause(){};
  virtual void resume(){};

};


#endif
