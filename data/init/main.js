x = 112;
y = 64;
rad = 0;
bx = 0;
step =1;
oA=[[0,0],[0,0],[0,0]];
oB=[0,0];

modetitle = 0
modegame = 1
modeover = 2
mode = modetitle
score = 0;
life =100;

setup = function(){

}

//loop内はモードの切り替え
loop = function(){
    if (mode == modetitle)title()
    else if(mode == modegame)game()
    else if(mode == modeover)over()
}

//タイトル画面
title = function(){
  score = 0;
  life = 100;
  color(8)
  fillrect(0,0,128,128)
  color(1)
  text("「小惑星きたる！」", 27,42)
  text("[>]ホールドでグッと前に進むよ",6, 85)
  if(btn(1)==true){
      mode = modegame;
  }
}

//ゲームオーバー画面
over = function(){
    color(2)
    fillrect(0,0,128,128)
    color(7)
    text("", 108, 1)
    text("ゲームオーバー",32, 52)
    if(btn(1)==true){
        mode = modetitle;
        for(i =0;i<oA.length;i++){
        oA[i][0] = 0;
        }
    }
}

//ゲーム画面
game = function(){
    color(0,0,120)
    fillrect(0,0,128,128)

    //キャラの描画
    y = Math.sin(rad)*32 + 64;
    spr(x, y, 8, 8, 8*1, 8*0)

    rad+=6.28/180;//2度ずつ進みます
    rad%=6.28;
    
    for(i =0;i<oA.length;i++){

    if(btn(1)>0){
        
        oA[i][0] += step;
        // oB[0] += step;
    }
    oA[i][0]++;

    if((oA[i][0])>128){
        oA[i][1] = Math.random()*112;
        // oB[1] = Math.random()*112;
        oA[i][0] = (oA[i][0])%128;
    }

    spr(oA[i][0]%128, oA[i][1], 8, 8, 8*2, 8*3)//石
    // spr(oB[0]%128, oB[1], 8, 8, 8*0, 8*3)//メダル


    //キャラエリアに入ったかどうか
    if(oA[i][0] >= 104 && oA[i][0] <= 120){
        score+=1;
        if( y-8 <= oA[i][1] && y+8 >= oA[i][1] ){
        // mode = modeover;
        spr(oA[i][0]%128-8, oA[i][1]-8, 24, 24, 8*12, 8*13)//石を爆発
        life--;

        if(life<0)mode = modeover;
        }
    }
    
    }
    color(9);
    fillrect(0,4,life+1, 3); 
    text(String(life), life+2, 0);
    color(10);
    text(String(score), 4, 8);

}
