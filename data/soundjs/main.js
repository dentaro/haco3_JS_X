// row = 8
// col = 8
// r = 120/row/2
// ff = 0
// setup = function(){
// }
// pretbtnid = -1
// loop = function(){
//     tbtnid = tbtns( 0,  0,  120, 120,  row, col, true);//タッチボタン生成（一回）、表示、ID取得まで一括して行う。trueによりボタンIDを連続取得するタッチビットに
//     if(pretbtnid - tbtnid != 0 && tbtnid != -1 ){//同じボタンを押していない　かつ //-1（ボタンが押されていない）状態ではない
//         for(j=0; j<col; j++){
//             for(i=0; i<row; i++){nowBt = j*col+i+5
//                 if(nowBt == tbtnid)rwtb(tbtnid,1)//押されたボタンを第２引数で　0状態だけ取得　1トグル　2鉛筆 3消しゴム にする
//                 if(rwtb(nowBt)==true)color(3)//緑//押されたボタンのbool値をトグルせずにそのまま取得する
//                 if(rwtb(nowBt)==false)color(10)//黄色//押されたボタンのbool値をトグルせずにそのまま取得する
//                 if(ff == 1)fillcircle(r*2*i + r, r*2*j + r, r)//初めは全部描画  
//                 if(ff == 0)if(nowBt == tbtnid)fillcircle(r*2*i + r, r*2*j + r, r)//差分描画
//             }
//         }
//     }
//     ff = 1;
//     pretbtnid = tbtnid  
// }

score = [0,0,0,0,0,0,0,0]
cursor = 0
playCursor = 0

clearScore = function(){
    for(i=0; i<8; i++){
        score[i] = 0
    }
}

draw = function(){
    color(0,0,0)
    fillrect(0,0,128,128)

    color(255,255,255)
    for(i=0; i<8; i++){
        fillrect(i * 16, (32 - score[i] - 1) * 4, 16-1,4)
    }
    color(0,255,0)
    drawrect((playCursor)*16, 0, 16, 128)

    color(255,255,0)
    drawrect((cursor)*16, 0, 16, 128)
}

setup = function(){
    clearScore()
}

count = 0
speed = 10//小さいほど速い

loop = function(){
    draw()
    count = count + 1

    if(count%speed == 0){
        playCursor = playCursor + 1

        if(playCursor > 7){
            playCursor = 0
        }
        if(score[playCursor] != 0){
            tone(0, score[playCursor]*10 + 440, 128)//-- play tone
        }else{
            tone(0, score[playCursor]*10 + 440, 0)
        }
    }

    if(btn(1)==2){
        if(cursor == 0){
            cursor = 7
        }else{
            cursor = cursor - 1
        }
    }

    if(btn(2)==2){
        if(cursor == 7){
            cursor = 0
        }else{
            cursor = cursor + 1
        }
    }

    if(btn(3)>2){
        score[cursor] = score[cursor] + 1
    }

    if(btn(4)>2){
        score[cursor] = score[cursor] - 1
    }

    if(btn(4)>100){
        score[cursor] = 0
    }
}

