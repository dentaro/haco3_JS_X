initState = 0;
playState = 1;
overState = 2;
GameState = initState;
size = 6;
raws = 20;
cols = 12;
x = 5;
y = 1;
r = 0;
loopCnt = 0;
dy = 0;
wall = 7;
back = 8;
clist = [8, 9, 10, 11, 12, 13, 14, 6, 7];
stage = [
    [7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7], 
    [7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7], 
    [7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7], 
    [7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7], 
    [7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7],
    [7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7], 
    [7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7], 
    [7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7], 
    [7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7], 
    [7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7],
    [7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7], 
    [7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7], 
    [7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7], 
    [7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7], 
    [7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7],
    [7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7], 
    [7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7], 
    [7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7], 
    [7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7], 
    [7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7]
];
ptnNo = 0;
dx = 0;
px = [0, 0, 0, 0];
py = [0, 0, 0, 0];
ptnxy=[[[-1,0],[1,0],[2,0]],[[-1,-1],[-1,0],[1,0]],[[-1,0],[1,-1],[1,0]],[[-1,0],[0,1],[1,1]],[[1,0],[0,1],[-1,1]],[[-1,0],[0,-1],[1,0]],[[1,0],[0,1],[1,1]]];
ff = 0;
setPosition = function(){px[0]=x;py[0]=y;r%=4;_r = r;for(i=0;i<3;i++){_x = ptnxy[ptnNo][i][0];_y = ptnxy[ptnNo][i][1];for(j=0;j<_r;j++){_w = _x; _x = _y; _y = -_w;}px[i+1] = x+_x;py[i+1] = y +_y;}}
setPtn = function(){setPosition();for(i=0;i<4;i++){_x =px[i];_y =py[i];stage[_y][_x] = ptnNo;}}
delPtn = function(){setPosition();for(i=0;i<4;i++){_x =px[i];_y =py[i];stage[_y][_x] = back;}}
collision = function(){setPosition();_flag1=false;for(i=0;i<4;i++){_x =px[i];_y =py[i];if(stage[_y][_x] != back){_flag1 = true;}}return _flag1;}
complete = function(){for(_y=0;_y<raws-1;_y++){_flag2 = 1;for(_x = 1; _x<cols-1;_x++){if(stage[_y][_x] == back){_flag2 = 0;}if(_flag2){for(upy =_y-1; upy>0;upy--){for(_x=1;_x<cols-1;_x++){stage[upy+1][_x] = stage[upy][_x];}}}}}}
drawStage = function(){
    for(by=0;by<raws;by++){
        for(bx=0;bx<cols;bx++){
            no = stage[by][bx]
            color(clist[no])
            fillrect(size * bx, size * by, size, size)
        }
    }
}
play = function(){
    delPtn();
    dy=0;dx=0;dr=0;
    while(loopCnt==5){dy = 1;loopCnt = 0;}
    loopCnt++;
    if(btn(1)==true){dx = 1;tone(6,10);}
    if(btn(4)==true){dx = -1;tone(6,10);}
    if(btn(2)==true){dr = 1;tone(12,10);}//物理ボタン
    if(btn(3)==true){ff = 1;tone(0,10);}//C4
    if(ff){dy = 1;}
    y+=dy;x+=dx;r+=dr;
    if( collision()){y-=dy;x-=dx;r-=dr;ff = 0;
        if(dy==1&&dx==0&&dr==0){setPtn();complete();y=1;x=5;r=0;ptnNo = Math.floor(Math.random() * 7);
            if( collision()){
                GameState = overState;
            }
        }
    }
    setPtn();
    drawStage();
}
setup = function(){
}
loop = function(){
    if (GameState == initState){init();}else if(GameState == playState){play();}else if(GameState == overState){over();}
}
init = function(){
    if (btn(1) == 1){for(by=0; by<raws; by++){
            stage[by][0] = wall;
            stage[by][cols-1] = wall
            for(bx=1;bx<cols-1;bx++){
                stage[by][bx] = back;
                if(by == raws-1){
                    stage[by][bx] = wall;}}}
        x = 5;y = 1;r = 0;ptnNo = Math.floor(Math.random() * 7);
        GameState = playState;
    }
}
over = function(){drawStage();color(8);text("game over", 16,42);if(btn(1)==1){GameState = initState;}}