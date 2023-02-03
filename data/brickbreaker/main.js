
px = 64
py = 112
pstep =3

tx = 0
ty = 0
tvx =1
tvy =1

cw = 16
ch = 8

chrs = []

new_chr = function(){
    var chr = {};
    chr = { x:0, y:0, touchflag:0};
    chrs[chrs.length] = chr;
  return chr
}

setup = function(){
    color(1)
    fillrect(0,0,128,128)
    
    for(cx =0; cx<cw; cx++){
        for(cy =0; cy<ch; cy++){
        chrs[chrs.length] = new_chr()
        }
    }
}


loop = function(){

    color(1)
    fillrect(0,0,128,128)
    
    if(px>=0){
    if(btn(0)>0){px -= pstep;}//>
    }else{
    px=0
    }
    
    if(px<=104){
    if(btn(1)>0){px += pstep;}//<
    }else{
    px=104
    }
    
    
    for(i =0; i<3; i++){
        spr(px + 8*i, py, 8, 8, 8*8, 8*14)//パドル
    }
    
    //跳ね返った時に少しランダムにさせる
    tvx += (Math.random()-0.5)/40
    tvy += (Math.random()-0.5)/40
    
    
    if(tx > 128||tx < 0)tvx*=-1
    
    if(tx > px && tx < px+8*3){//パドルに当たったら
        if(ty > py||ty < 0)tvy*=-1//跳ね返す
    }else{
        if(ty > 128||ty < 0)tvy*=-1//壁で跳ね返る
    }
    
    tx = tx + tvx
    ty = ty + tvy
    
    spr(tx, ty, 8, 8, 8*6, 8*13)//たま
    
    var ccnt = 0
    var xcnt = 0
    var ycnt = 0
    var getnum = 0
    for (key in chrs)//この表記で連想配列内のキーにアクセスできるようになる
    {
        chrs[ccnt].x = xcnt%16 
        chrs[ccnt].y = ycnt 
        
        // if(tx > (chrs[ccnt].x*8) && tx < (chrs[ccnt].x*8)+8 && ty > chrs[ccnt].y*8 && ty < chrs[ccnt].y*8)//当たったか判定
        if(tx < chrs[ccnt].x*8 +4 && tx > chrs[ccnt].x*8 - 4 && ty < chrs[ccnt].y*8 +4 && ty > chrs[ccnt].y*8 - 4)//当たったか判定
        {chrs[ccnt].touchflag = 1}
        
        if(chrs[ccnt].touchflag==0)//当たっていなければ描画
        spr(chrs[ccnt].x*8, chrs[ccnt].y*8, 8, 8, 8*9, 8*0)//コイン
        
        if(chrs[ccnt].touchflag == 1)getnum++
        
        xcnt++
        if(xcnt%16 == 0)ycnt++
        ccnt++
    }
    
    color(255,255,255);
    text(String(cw*ch - getnum), 4, 120);
    
    
}