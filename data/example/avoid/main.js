counter = 0
chrs = []
max_chrs = 4
score = 0
speed = 0.2//敵のスピード
bgdrawF = false;
x = 16
y = 56
modetitle = 0;
modegame = 1;
modeover = 2;
mode = modetitle;
new_chr = function(){
    if (score < 50 ){
        r = 1
    }else if (score < 100 ){
        r = 2
    }else if (score < 150 ){
        r = 3
    }else if (score < 200 ){
        r = 4
    }else{
        r = Math.floor(Math.random() * 4 + 1)
    }
    var chr = {};
    if (r == 1 ){
      chr = {x:-16, y:Math.floor((Math.random()) * 128/16 )*16, xv:speed, yv:0};
    }else if (r == 2 ){
      chr = {x:128, y:Math.floor((Math.random()) * 128/16 )*16, xv:-speed, yv:0};
    }else if (r == 3 ){
      chr = {x:Math.floor((Math.random()) * 128/16 )*16, y:-16, xv:0, yv:speed}
    }else if (r == 4 ){
      chr = {x:Math.floor((Math.random()) * 128/16 )*16, y:128, xv:0,yv:-speed}
    }
    chrs[chrs.length] = chr;
  return chr
}
setup = function(){
}
loop = function(){
    if (mode == modetitle)title()
    else if(mode == modegame)game()
    else if(mode == modeover)over()
    counter++
}
title = function(){
    x = y = 64;
    score = 0;
    life = 100;
    color(8);
    fillrect(0,0,128,128);
    color(1);
    text("「警備員に捕まっちゃう？」", 17,42);
    text("矢印キーで逃げよう！",20, 85);
    chrs = []
    if(btn(2)==true){
        mode = modegame;
    }
}
over = function(){
    color(2)
    fillrect(0,0,128,128)
    color(7)
    
    text("ゲームオーバー",32, 52)
    if(btn(2)==true)mode = modetitle
}
game = function(){
    color(1)
    fillrect(0,0,128,128)
    color(255, 255, 255)
    text(String(score), 0,0)
    if ( counter%10 == 0 )score++
    if (score < 200 ){max_chrs = 4
    }else{max_chrs = (score - 200)/50 + 4}
    spr(x, y, 8, 8, 0, 0)
    if (btn(0) > 0 && x > 0 )x -= 1
    if (btn(1) > 0 && x < 128-16 )x += 1
    if (btn(2) > 0 && y > 0 )y -= 1
    if (btn(3) > 0 && y < 128-16 )y += 1
    var new_chrs = []
    for (key in chrs){
        if (chrs.hasOwnProperty(key)) {
            chrs[i].x += chrs[i].xv
        chrs[i].y += chrs[i].yv
        if (Math.abs((chrs[i].x + 8) - (x + 8)) < 6 && Math.abs((chrs[i].y + 8) - (y + 8)) < 6 ){
            y+=8;
            return mode = modeover
        }
        if(-8 < chrs[i].x && -8 < chrs[i].y && chrs[i].x < 128 && chrs[i].y < 128 ){
            new_chrs[new_chrs.length + 1] = chrs[i];
            spr(chrs[i].x, chrs[i].y, 8, 8, 8*7, 8*11)
        }else{chrs = []}
        }
    }
    chrs = new_chrs
    var add_chr_num = max_chrs - chrs.length
    for(i=1;i<add_chr_num;i++){
    if ( (Math.floor(Math.random() * 50) + 1)){
      chrs[chrs.length + 1] = new_chr()
    }
  }
  mode = modegame
}