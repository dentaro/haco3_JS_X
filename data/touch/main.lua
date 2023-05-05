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







