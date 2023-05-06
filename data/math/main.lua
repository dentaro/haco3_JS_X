function _init()

end

function _draw()
  
end

function _update()
  cls(1)
  if tstat() > 0 then -- タッチされたら
    local mx = tp(0) -- マウス座標取得
    local my = tp(1) -- マウス座標取得
    local cx, cy = 64, 60 -- 画面中心座標
    line(cx, cy, mx, my, 7) -- 線を描画
    print(distance(cx, cy, mx, my),0,0)
    a = atan2(my - cy, mx - cx)* (180 / math.pi)
    print(a,0,0,7)
    print(sgn(a),0,0,7)--マイナス値の時-1を返す
    local x = 0x0a0a --なぜか2568にならず2570になる
    local y = 2
    print("x = "..x, 0, 0, 7)
    print("y = "..y, 0, 8, 7)
    print("shr(x, y) = "..shr(x, y), 0, 16, 7)
  else
    -- ランダムシードを設定
    -- srnd(1234)
    -- 0から1までのランダムな浮動小数点数を10回表示する
    for i = 1, 10 do
      print(rnd(1000),0,0,7)
    end
  end
end
-------------------------------------------------------
function setup()--init
  _init()
end
function loop()--update --draw
_update()
_draw()
end

--tstatのタッチ状態は以下の通り
--eventState
-- NO_EVENT -1
-- UNTOUCH 0
-- DRAG 2
-- TAPPED 3
-- WTAPPED 4
-- FLICK 5 //フリックの中心、始まりのイベント
-- FLICKED 6 //フリック終わりのイベント
-- TOUCH 7
-- WAIT 8
-- MOVE 9
-- RELEASE 10


