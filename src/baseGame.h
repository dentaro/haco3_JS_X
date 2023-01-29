#ifndef BASE_GAME_H
#define BASE_GAME_H

#define CTRLBTNNUM 5//ゲームに使える再度ボタン数を決めます

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
