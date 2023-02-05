
px = 64
py = 112
pstep =3
tx = 0
ty = 0
tvx =1
tvy =1
modetitle = 0
modegame = 1
modeover = 2
mode = modetitle

oA=[[24,8,0],[36,16,0],[48,8,0],[56,16,0],[24,16,0],[36,24,0],[48,16,0],[56,24,0]];
setup = function(){
    color(1)
    fillrect(0,0,128,128)
}

//loop内はモードの切り替え
loop = function(){
    if (mode == modetitle)title()
    else if(mode == modegame)game()
    else if(mode == modeover)over()
}

//タイトル画面
title = function(){
  color(225,200,54)
  fillrect(0,0,128,128)
  color(44,90,0)
  text("「レンガを破壊せよ！」", 27,42)
  text("[ｖ]ホールドでパドルが動くよ",6, 85)
  tvx = Math.random()*2
  tvy = Math.random()*2
  if(btn(4)==true){
      mode = modegame;
  }
}

//ゲームオーバー画面
over = function(){
    color(7)
    text("クリア！",46,48);
    if(btn(4)==true){
        mode = modetitle;
        
        for(i =0;i<oA.length;i++){
        oA[i][2] = 0;
        
        }
    }
}
//ゲーム画面
game = function(){
    color(1)
    fillrect(0,0,128,128)
    if(px>=0){
    if(btn(3)>0){px -= pstep;}//>
    }else{
    px=0
    }
    if(px<=104){
    if(btn(4)>0){px += pstep;}//<
    }else{
    px=104
    }
    for(i =0; i<3; i++){
        spr(px + 8*i, py, 8, 8, 8*4, 8*2)//パドル
    }
    if(tx > 128||tx < 0)tvx*=-1
    if(tx > px && tx < px+8*3){//パドルに当たったら
        if(ty > py||ty < 0)tvy*=-1//跳ね返す
    }else{
        if(ty > 128||ty < 0)tvy*=-1//壁で跳ね返る
    }
    tx = tx + tvx
    ty = ty + tvy
    spr(tx, ty, 8, 8, 8*6, 8*5);
    getnum = 0
    for(i =0;i<8;i++){//ブロックの数
        if(tx < oA[i][0] + 8 && tx > oA[i][0] - 8 && ty < oA[i][1] +8 && ty > oA[i][1] - 8)//当たったか判定
        {
            oA[i][2] = 1
        }
        if(oA[i][2]==0){
            spr(oA[i][0], oA[i][1], 8, 8, 8*0, 8*6)
            getnum += 1
        }

    }
    if(getnum==0){
        
        mode = modeover;
    }
}

