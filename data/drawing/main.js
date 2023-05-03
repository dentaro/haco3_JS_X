row = 8
col = 8
r = 120/row/2
ff = 0

setup = function(){
}
pretbtnid = -1
loop = function(){
    
    tbtnid = tbtns( 0,  0,  120, 120,  row, col, true);//タッチボタン生成（一回）、表示、ID取得まで一括して行う。trueによりボタンIDを連続取得するタッチビットに
    if(pretbtnid - tbtnid != 0 && tbtnid != -1 ){//同じボタンを押していない　かつ //-1（ボタンが押されていない）状態ではない
        for(j=0; j<col; j++){
            for(i=0; i<row; i++){nowBt = j*col+i+9
                if(nowBt == tbtnid)rwtb(tbtnid,1)//押されたボタンを第２引数で　0状態だけ取得　1トグル　2鉛筆 3消しゴム にする
                if(rwtb(nowBt)==true)color(3)//緑//押されたボタンのbool値をトグルせずにそのまま取得する
                if(rwtb(nowBt)==false)color(10)//黄色//押されたボタンのbool値をトグルせずにそのまま取得する
                if(ff == 1)fillcircle(r*2*i + r, r*2*j + r, r)//初めは全部描画  
                if(ff == 0)if(nowBt == tbtnid)fillcircle(r*2*i + r, r*2*j + r, r)//差分描画
            }
        }
    }
    ff = 1;

    // x = tp(0)
    // y = tp(1)
    // color(8)
    // fillrect(x,y,5,5)
    // color(255,255,255)
    // text(str(tbtnid), x+10, y)

    pretbtnid = tbtnid
    
}