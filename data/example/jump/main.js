count = 0
hy = 80//地平線の高さ
x = 16
y = hy-8
yv = 0
jumpflag = false
preventcount = 0
grounddrawF = false;
modetitle = 0;
modegame = 1;
modeover = 2;
mode = modetitle;
score = 0;
life =100;
setup = function(){
}
loop = function(){
    if (mode == modetitle)title()
    else if(mode == modegame)game()
    else if(mode == modeover)over()
}
title = function(){
    score = 0;
    life = 100;
    color(8);
    fillrect(0,0,128,128);
    color(1);
    text("「サボテン襲来！」", 27,42);
    text("[^]ジャンプして避けよう",20, 85);
    if (btn(2) == 1){
        grounddrawF = true;
        mode = modegame
        enemy = [];
        var e = new Object();
        e = {x:100, y:hy-8};
        enemy[enemy.length] = e;
    }
}
over = function(){
    color(2)
    fillrect(0,0,128,128)
    color(7)
    text("ゲームオーバー",32, 52)
    if(btn(2)==true){
        mode = modetitle;
    }
}
game = function(){
    color(1)
    fillrect(0,0,128,hy);
    if (btn(1) >  0 && !jumpflag ){ y = y - 30;yv = -7;jumpflag = true;}
    if (btn(2) == 1 && !jumpflag ){ yv = -7;jumpflag = true;}
    if (btn(3) >  0 && !jumpflag ){ y = y + 5;yv = -6;jumpflag = true;}
    if (Math.abs(yv) < 8){yv += 0.3;}
    y = y + yv;
    if (y > hy-8){ y = hy-8; jumpflag = false; grounddrawF =true;}
    if (Math.floor(Math.random() * 100) + 1 === 1 && preventcount === 0 )
    {
        e = {x:128, y:hy-8};
        enemy[enemy.length] = e
        preventcount = 32
    }
    for(i=0;i<enemy.length;i++)
    {
        cx = x + 8
        cy = y + 8
        cex = enemy[i].x + 8
        cey = enemy[i].y + 8
        if (Math.abs(cx - cex) < 8 && Math.abs(cy - cey) < 8){
            mode = modeover
        }
    }
    spr(x, Math.floor(y), 8, 8, 0, 0);
    nextenemy = [];
    for(i=0;i<enemy.length;i++){
        enemy[i].x -= 1;
        if (enemy[i].x > -16){
            nextenemy[nextenemy.length + 1] = enemy[i]
        }
        spr(enemy[i].x, enemy[i].y, 8, 8, 8*11, 8*3)
    }
    nextenemy = enemy;
    if(grounddrawF === true){
        for(j=0; j<2; j++){
            for(i=0; i<16; i++){
            spr(i*8, hy+(j*8), 8, 8, 8*1, 8*5);
            }
        }
        grounddrawF=false;
    }
    count = count + 1
    if (preventcount > 0){
        preventcount -= 1
    }
}