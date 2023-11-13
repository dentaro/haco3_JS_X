#include <LovyanGFX_DentaroUI.hpp>
// #include <SPI.h>
// #include <Wire.h>
#include <map>
#include <SD.h>
// #include <JPEGDecoder.h>

#include <Arduino.h>
#include <FS.h>
#include "SPIFFS.h"
// // #include "runLuaGame.h"
// #include "haco8/runHaco8Game.h"

// #include "Tunes.h"
// #include "runJsGame.h"
// #include "wifiGame.h"
// MapTile構造体の定義
#define BUF_PNG_NUM 1

class MapTile {
  private:
    LGFX_Sprite* sprptr;
    Vector2<int> tileNo;
    Vector2<int> tilePos;
    Vector2<int> buffPos;
    String filePath;
    String newKey;
    // String targetKey;
    String oldKey;
    int dirno;
    int buffIDno;
    bool collF;
    int readNo;
    int oldreadNo;
  public:
  // 他のメンバ変数やメソッドの定義
    MapTile()
    : tileNo(0,0), sprptr(nullptr), dirno(0), filePath(""), newKey(""), oldKey("")
    {
    }

    
    // 初期化メソッドなどを追加する場合もあります
    void init() {
        // 初期化処理を記述
    }
    
    void drawMap2(LovyanGFX& _buffSprite, int _bufx, int _bufy) {
      // if (sprptr != nullptr) {
      //   sprptr->setColor(TFT_RED);
      //   sprptr->fillRect(0, 0, 100, 50);
      // }
        
        // 登録したポインタから対応するスプライトを取得
        // LGFX_Sprite* spritePtr = key2ptr[maptils[no].getNewKey2()];
        // _targetKey = "/tokyo/15/29100/12903";
        // String filePath = "/tokyo/15/29099/12903.png";
            // if (SD.exists(filePath)) {
            //     _buffSprite.drawPngFile(SD, filePath, 20, 20);
            // }
        
       if (sprptr != nullptr) {
            // 登録されたスプライトを描画
            sprptr->pushSprite(&_buffSprite, _bufx, _bufy);
            // ここに _buffSprite に対して地図＝sprptr を描く処理
            // String filePath = "/tokyo/15/29099/12903.png";
            // if (SD.exists(filePath)) {
            //     _buffSprite.drawPngFile(SD, filePath, 20, 20);
            // }
        }
    }

    // void drawMap(LovyanGFX& _buffSprite, float _bufx, float _bufy) {
    //     _buffSprite.setColor(TFT_RED);
    //     _buffSprite.fillRect(0, 0, 100, 50);
    //     //ここに_buffSpriteに対して地図＝spriteを書く処理
    //     String filePath = "/tokyo/15/29099/12903.png";
    //     if (SD.exists(filePath)) {
    //       _buffSprite.drawPngFile(SD,filePath, 20, 20);
    //     }
    // }

    void setOldReadNo2(int _oldreadNo){
      oldreadNo = _oldreadNo;
    }

    int getOldReadNo2(){
      return oldreadNo;
    }

    void setReadNo2(int _readNo){
      readNo = _readNo;
    }

    int getReadNo2(){
      return readNo;
    }

    void setTilePos2(Vector2<int> _tilePos){
      tilePos = _tilePos;
    }

    Vector2<int> getTilePos2(){
      return tilePos;
    }
    
    void setBuffIDNo2( int _buffIDno ){
      buffIDno = _buffIDno;
    }

    void setDirNo2( int _dirno ){
      dirno = _dirno;
    }

    int getDirNo2(){
      return dirno;
    }

    int getBuffIDNo2(){
      return buffIDno;
    }

    LGFX_Sprite* getSprptr2() {
        return sprptr;
    }

    // メンバ変数をセットするメソッド
    void setNewKey2(const String& _Key) {
      // Serial.println(_Key);
      newKey = _Key;
    }

    void setTileNo2(Vector2<int> _tileNo){
      tileNo = _tileNo;
    }

    // メンバ変数をゲットするメソッド
    const String getNewKey2(){
        return newKey;
    }

    void setSprptr2(LGFX_Sprite* _sprptr){
     sprptr = _sprptr; // メンバ変数にスプライトへのポインタを代入
    }

    void setCollF2(bool _collF){
      collF = _collF;
    }

    bool getCollF2(){
      return collF;
    }
    
    void setBuffPos2(Vector2<int> _buffPos) {
        buffPos =  _buffPos;
    }

    Vector2<int> getBuffPos2() {
        return buffPos;
    }


};

// MapDictionaryクラスの定義
//マップの検索を行う
class MapDictionary {
private:
    MapTile maptils[BUF_PNG_NUM];
    std::map<String, LGFX_Sprite*> key2ptr;
    // std::vector<String> tempotaryKeys;
    

    String newKeys[BUF_PNG_NUM];
    String oldKeys[BUF_PNG_NUM];
    int dirNos[BUF_PNG_NUM];

    std::vector<float> angleList; // 角度の平均を出すための値を保持するベクター

public:
    static MapDictionary& getInstance() {
      static MapDictionary instance;
      return instance;
    }

    LGFX_Sprite* getSprptr(int no){
      return maptils[no].getSprptr2();
    } 

    void setSprptr(int no, LGFX_Sprite* _sprptr){
      maptils[no].setSprptr2(_sprptr);
    }

    void setNewKeySprptr(const String& _Key, LGFX_Sprite* sprptr) {
      auto it = key2ptr.find(_Key);
      if (it != key2ptr.end()) {
          it->second = sprptr; // 対応する値（LGFX_Sprite*）を更新
      }
    }

    void copy2buff(LovyanGFX& _buffSprite, LGFX_Sprite* _sprptr, int no) {
        _buffSprite.fillRect(20*no, 20*no, 20,20, TFT_BLUE);
    }

    void setOldReadNo(int no, int _oldreadNo){
      maptils[no].setOldReadNo2(_oldreadNo);
    }

    int getOldReadNo(int no){
      return maptils[no].getOldReadNo2();
    }

    void setCollF(int no, bool _collF){
      maptils[no].setCollF2(_collF);
    }

    void setKey2CollF(const String& _Key, bool _collF){

      for(int i = 0; i<BUF_PNG_NUM; i++){
        if(maptils[i].getNewKey2() == _Key)
        return  maptils[i].setCollF2(_collF);
      }
      
    }

    // bool getCollF(const String& _Key){
    //   for(int i = 0; i<BUF_PNG_NUM; i++){
    //     if(maptils[i].getNewKey2() == _Key)
    //     return maptils[i].getCollF2();
    //   }

    // }


    bool getCollF(int no){

        return maptils[no].getCollF2();
      
    }


    void setDirNo( int no, int _dirno){
      maptils[no].setDirNo2( _dirno );
    }


    void setReadNo(int no, int _readno){
      maptils[no].setReadNo2( _readno );
    }

    int getkey2ReadNo(const String& _Key){
      for(int i = 0; i<BUF_PNG_NUM; i++){
        if(maptils[i].getNewKey2() == _Key)
        return maptils[i].getReadNo2();
      }
    }

    int getKey2OldReadNo(const String& _Key){
      for (int i = 0; i < BUF_PNG_NUM; i++) {
        if (maptils[i].getNewKey2() == _Key) {
            return maptils[i].getOldReadNo2();
        }
      }
    }

    bool getKey2CollF(const String& _Key){
      for(int i = 0; i<BUF_PNG_NUM; i++){
        if(maptils[i].getNewKey2() == _Key)
        return maptils[i].getCollF2();
      }
    }

    void setKey2BuffPos(const String& _Key, Vector2<int> _buffPos){

      for (int i = 0; i < BUF_PNG_NUM; i++) {
        if (maptils[i].getNewKey2() == _Key) {
            maptils[i].setBuffPos2(_buffPos);
        }
      }

    }

    bool existKey(const String& _Key){
      for(int i = 0; i<BUF_PNG_NUM; i++){
        if(maptils[i].getNewKey2() == _Key)
        return true;
      }
      return false;

      // LGFX_Sprite* spritePtr = getKey2sprptr(_Key);

      // if (spritePtr) {
      //     return true;
      // } else {
      //     return false;
      // }
    }

    void setTileInfo(int no, LGFX_Sprite* _sprptr,bool _collF){
      // setTileInfo(targetKey, tilepos, &mapTileSprites[dict.getkey2ReadNo(rundownloadkey)]);
      maptils[no].setReadNo2(no);
      // maptils[no].setBuffPos2(_buffPos);
      maptils[no].setSprptr2(_sprptr);
      maptils[no].setOldReadNo2(_collF);
    }

    void setTilePos(int no, Vector2<int> _tilePos){
      maptils[no].setTilePos2( _tilePos );
    }

    Vector2<int> getkey2BuffPos(const String& _Key) {
    for (int i = 0; i < BUF_PNG_NUM; i++) {
        if (maptils[i].getNewKey2() == _Key) {
          return maptils[i].getBuffPos2();
          break;
        }
    }
    return {9999,9999};
    // 適切な値を返すか、エラー処理を行う必要があります
    // 例えば、デフォルトの座標を返すなど

    }



    void setBuffIDNos( int _dirno)
  {

    if(_dirno == -1){//止まっていたら
      _dirno = 0;//ひとまず方向０として順番を決める
    }
    maptils[0].setBuffIDNo2( -1 );//現在位置
    // 進行方向を中心にした読み込み順を決める
    maptils[1].setBuffIDNo2(  0 );//進行方向
    maptils[2].setBuffIDNo2( (8 - 1) % 8);
    maptils[3].setBuffIDNo2( (8 + 1) % 8);
    maptils[4].setBuffIDNo2( (8 - 2) % 8);
    maptils[5].setBuffIDNo2( (8 + 2) % 8);
    maptils[6].setBuffIDNo2( (8 - 3) % 8);
    maptils[7].setBuffIDNo2( (8 + 3) % 8);
    maptils[8].setBuffIDNo2( (8 - 4) % 8);
  
}

  void setDirNos( int _dirno)
  {
    if(_dirno == -1){//止まっていたら
      _dirno = 0;//ひとまず方向０として順番を決める
    }
    maptils[0].setDirNo2( -1 );//現在位置
    // 進行方向を中心にした読み込み順を決める
    maptils[1].setDirNo2(  0 );//進行方向
    maptils[2].setDirNo2( (8 - 1) % 8);
    maptils[3].setDirNo2( (8 + 1) % 8);
    maptils[4].setDirNo2( (8 - 2) % 8);
    maptils[5].setDirNo2( (8 + 2) % 8);
    maptils[6].setDirNo2( (8 - 3) % 8);
    maptils[7].setDirNo2( (8 + 3) % 8);
    maptils[8].setDirNo2( (8 - 4) % 8);

    // if(_dirno == -1){//止まっていたら
    //   _dirno = 0;//ひとまず方向０として順番を決める
    // }

    // maptils[0].setDirNo2( -1 );//現在位置
    // // 進行方向を中心にした読み込み順を決める
    // maptils[1].setDirNo2(  _dirno );//進行方向
    // maptils[2].setDirNo2( (_dirno + 8 - 1) % 8);
    // maptils[3].setDirNo2( (_dirno + 8 + 1) % 8);
    // maptils[4].setDirNo2( (_dirno + 8 - 2) % 8);
    // maptils[5].setDirNo2( (_dirno + 8 + 2) % 8);
    // maptils[6].setDirNo2( (_dirno + 8 - 3) % 8);
    // maptils[7].setDirNo2( (_dirno + 8 + 3) % 8);
    // maptils[8].setDirNo2( (_dirno + 8 - 4) % 8);
  
}

int getDirNo(int _dirno, int no){
  return maptils[no].getDirNo2();
}

// int getReadNo(int _dirno, int no){
//       if(no == 0)( 0 );//現在位置
//       // 進行方向を中心にした読み込み順を決める
//       else if(no == 1)return(  _dirno );//進行方向
//       else if(no == 2)return( (_dirno + 8 - 1) % 8);
//       else if(no == 3)return( (_dirno + 8 + 1) % 8);
//       else if(no == 4)return( (_dirno + 8 - 2) % 8);
//       else if(no == 5)return( (_dirno + 8 + 2) % 8);
//       else if(no == 6)return( (_dirno + 8 - 3) % 8);
//       else if(no == 7)return( (_dirno + 8 + 3) % 8);
//       else if(no == 8)return( (_dirno + 8 - 4) % 8);
// }

int getDirNo(int no) {
    return maptils[no].getDirNo2(); 
}

int getBuffIDNo(int no) {
    return maptils[no].getBuffIDNo2(); 
}

int getBuffID2DirNo(int buffIDno){
  for(int i = 0; i<BUF_PNG_NUM; i++){
    if(buffIDno = maptils[i].getBuffIDNo2()){
    return maptils[i].getDirNo2();break;
    }
  }
}

  Vector2<int> getTilePos(int _dirNo) {
    Vector2<int> directions[] = {//ベクトル { X, Y, DirNo}
        { 0,  0},  //-1
        { 1,  0},  // 0
        { 1,  1},  // 1
        { 0,  1},  // 2
        {-1,  1},  // 3
        {-1,  0},  // 4
        {-1, -1},  // 5
        { 0, -1},  // 6
        { 1, -1}   // 7
    };
    
    if (_dirNo >= -1 && _dirNo <= 7) {
        return directions[_dirNo + 1]; // 配列のインデックスは-1から始まるため、+1する
    } else {
        return {0, 0}; // デフォルト
    }
  }

  int getTargetDirNo(float _x, float _y){
    float angle = atan2(_y, _x) + 2 * M_PI;
      return fmod(angle / (M_PI / 4), 8);
  }

  float calculateAverageAngle(float _angle, int averageLimitNum) {
    if (angleList.size() >= averageLimitNum) {
        angleList.erase(angleList.begin()); // 最も古い要素を削除
    }

    float oldestAngle = 0.0;
    if (!angleList.empty()) {
        oldestAngle = angleList.front();
    }

    angleList.push_back(_angle);

    float sum = 0.0; // 角度の合計を保持する変数

    for (float angle : angleList) {
        sum += angle;
    }

    float average = sum / angleList.size();
    
    if (!angleList.empty()) {
        average -= oldestAngle / angleList.size(); // 最古の要素を除く
    }
    
    return average;
  }




  int getTargetDirNo(Vector3<float> _pos){
    float angle = atan2(_pos.getY(), _pos.getX()) + 2 * M_PI;

    angle = calculateAverageAngle(angle,10);
      return fmod(angle / (M_PI / 4), 8);
  }

  MapTile getMapTile(int no){
    return maptils[no];//ディクショナリー内から
  }

  void setNewKeyloop(int no, const String& _Key, LGFX_Sprite* _sprptr) {

    Serial.print(_Key);
    newKeys[no] = _Key;//ディクショナリー内にキーを登録

    maptils[no].setTileNo2(getKey2tileNo(_Key));

    if (no >= 0 && no < BUF_PNG_NUM) {
      maptils[no].setNewKey2(_Key);  // //マップタイル自体にキーを登録する
      maptils[no].setSprptr2(_sprptr);

      // if (key2ptr.count(_Key) == 0) {
      //     key2ptr[_Key] = getSprptr(no);  // キーをポインタに紐付ける
      // } else {
      //     // 既にキーが存在する場合の処理
      //     // 例えば、上書きするかどうかの判断など
      // }
    }
  }

  void setNewKey(int no, const String& _Key) {
    maptils[no].setNewKey2(_Key);  // //マップタイル自体に登録する

    // newKeys[no] = _Key;//ディクショナリー内に登録

    // maptils[no].setTileNo2(getKey2tileNo(_Key));

    // if (no >= 0 && no < BUF_PNG_NUM) {
    //   maptils[no].setNewKey2(_Key);  // //マップタイル自体に登録する

    //   if (key2ptr.count(_Key) == 0) {
    //       key2ptr[_Key] = getSprptr(no);  // キーをポインタに紐付ける
    //   } else {
    //       // 既にキーが存在する場合の処理
    //       // 例えば、上書きするかどうかの判断など
    //   }
    // }
  }

  void showKeyInfo(const String& _Key){
    if(key2ptr.count(_Key)>0){
      Serial.print(_Key+"の数：");
      Serial.println(key2ptr.count(_Key));
    }
  }


  void setNewKeySprptr(const String& _Key, const String& _newKey, LGFX_Sprite* sprptr) {
    auto it = key2ptr.find(_Key);
    if (it != key2ptr.end()) {
        key2ptr.erase(it);  // 古いキーのエントリを削除
        key2ptr[_newKey] = sprptr;  // 新しいキーでエントリを追加
    }
  }



  LGFX_Sprite* getKey2sprptr(const String& _Key) {

    for (int i = 0; i < BUF_PNG_NUM; i++) {
      if (maptils[i].getNewKey2() == _Key) {
        return maptils[i].getSprptr2();
      }
    }

    // if (key2ptr.count(_Key) > 0) {
      
    //     auto it = key2ptr.find(_Key);
    //     return it->second;
    // } else {
    //     // キーが見つからない場合はnullptrを返す
    //     return nullptr;
    // }
  }

  Vector2<int> getKey2tileNo(const String& _Key) {
    if (key2ptr.count(_Key) > 0) {
        int slashPos = _Key.indexOf('/'); // '/'の位置を取得
        if (slashPos != -1) { // '/'が見つかった場合
            String numA_str = _Key.substring(0, slashPos); // '/'より前の部分を取得
            String numB_str = _Key.substring(slashPos + 1); // '/'より後の部分を取得
            int numA = numA_str.toInt(); // 数字に変換
            int numB = numB_str.toInt(); // 数字に変換
            return {numA, numB}; // Vector2<int>を返す
        }
    }
    // キーが見つからない場合や'/'が見つからない場合はデフォルトのVector2<int>を返す
    return {-1, -1};
}


  String getNewKey(int no) {
      if (no >= 0 && no < BUF_PNG_NUM) {
          return maptils[no].getNewKey2();
      }
      return ""; // エラー時は空文字列を返す
  }

  bool getAlreadyDownlordedF(const String& _Key){
    if (key2ptr.count(_Key) > 0) return true;
    else return false;

  }

  Vector2<int> getBuffPos(int no) {
    return maptils[no].getBuffPos2();
  }

  // void setBuffPos(int no, Vector2<int> _buffPos) {
  //   maptils[no].setBuffPos2(_buffPos);
  // }


  void setBuffPos(int no, Vector2<int> _buffPos) {
    maptils[no].setBuffPos2(_buffPos);
  }

  // void setBuffPos(const String& _Key, Vector2<int> _buffPos) {
  //   for (int i = 0; i < BUF_PNG_NUM; i++) {
  //     if (maptils[i].getNewKey2() == _Key) {
  //       return maptils[i].setBuffPos2(_buffPos);
  //     }
  //   }
  // }

};
