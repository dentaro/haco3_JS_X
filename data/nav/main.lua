
-- 円周率πの取得
local PI = 3.14159265358979323846
local TWO_PI = PI*2
local RAD_ONE = PI/180
x=15
y=15
host ="tokyo"--ルートディレクトリ名です。
latPos = 35.68999241096032--新宿駅の座標
lonPos = 139.70054064148115
-- latPos = 35.728926826032936--池袋駅の座標
-- lonPos = 139.71125071777942
tileZoom = 12.0--座標計算用
walkLatPos = 35.68999241096032--新宿駅の座標
walkLonPos = 139.70054064148115

walkAngle = 0.0
-- ZoomValue = 14
scale = 1.0
n = 0
frame = 0
tx = 64
ty = 60

cx = 64
cy = 60
firstF=true
function setup()
  fillrect(20,20,80,80,7)
end

function _init()--1回だけ
    setup()
end

function input()
  if (btn(6)>=2) then
    if(tileZoom<15.9)then
      tileZoom = tileZoom + 0.1;
    end

  end

  if (btn(8)>=2) then
    if(tileZoom>10.1)then
      tileZoom = tileZoom - 0.1;
    end
  end

  --phbtn(3)は0~4095の値をボリュームから取得
  -- tileZoom = 10 + phbtn(2) * 5 / 4095;

  if phbtn(2) == 1 then 
    tileZoom = tileZoom + 0.1;
  end
  if phbtn(2) == 2 then 
    tileZoom = tileZoom - 0.1;
  end

  -- tx = tp(0)
  -- ty = tp(1)

  -- if (touch()==2) then --タッチされたら
  --   gpos = getgpos(tx, ty, tileZoom );
  --   walkLatPos = gpos.lat
  --   walkLonPos = gpos.lon
  -- end

  -- --アナログスティック
  if phbtn(0,1) == 7 then 
    gpos = getgpos(cx+1, cy, tileZoom );
    walkLatPos = gpos.lat
    walkLonPos = gpos.lon
  end

  if phbtn(0,1) == 6 then 
    gpos = getgpos(cx+1, cy+1, tileZoom );
    walkLatPos = gpos.lat
    walkLonPos = gpos.lon
  end

  if phbtn(0,1) == 5 then 
    gpos = getgpos(cx, cy+1, tileZoom );
    walkLatPos = gpos.lat
    walkLonPos = gpos.lon
  end

  if phbtn(0,1) == 4 then 
    gpos = getgpos(cx-1, cy+1, tileZoom );
    walkLatPos = gpos.lat
    walkLonPos = gpos.lon
  end

  if phbtn(0,1) == 3 then 
    gpos = getgpos(cx-1, cy, tileZoom );
    walkLatPos = gpos.lat
    walkLonPos = gpos.lon
  end

  if phbtn(0,1) == 2 then 
    gpos = getgpos(cx-1, cy-1, tileZoom );
    walkLatPos = gpos.lat
    walkLonPos = gpos.lon
  end

  if phbtn(0,1) == 1 then 
    gpos = getgpos(cx, cy-1, tileZoom );
    walkLatPos = gpos.lat
    walkLonPos = gpos.lon
  end

  if phbtn(0,1) == 0 then 
    gpos = getgpos(cx+1, cy-1, tileZoom );
    walkLatPos = gpos.lat
    walkLonPos = gpos.lon
  end
end

function _update()--ループします
  
  input()

  -- --指定座標（新宿駅）の周辺をぐるぐる回るための処理
  -- walkAngle = walkAngle - 1.5
  -- radius = 0.001 -- 回転半径（0.00002の距離）
  -- ZoomValue = 15--pow(2, getZtile())*0.00625;
  -- local gpos = getgpos(tx, ty, tileZoom );
  --   print(gpos.lat)
  --   print(gpos.lon)
  --   drawmaps( gpos.lat, gpos.lon, tileZoom )
  --   walkLatPos = gpos.lat
  --   walkLonPos = gpos.lon
  -- walkLatPos = latPos + sin(walkAngle*RAD_ONE)*radius--ZoomValue＝15の時204.8;
  -- walkLonPos = lonPos + cos(walkAngle*RAD_ONE)*radius--ZoomValue＝15の時204.8;
end

function _draw()--ループします
  -- cls()
  drawmaps( walkLatPos, walkLonPos, tileZoom )
  print(walkLatPos)
  print(walkLonPos)
  local pos = getmappos( 35.68999241096032, 139.70054064148115, tileZoom )--新宿
  spr8( 64, pos.x, pos.y)--火
  local pos = getmappos( 35.6783071, 139.7631488, tileZoom )
  spr8( 32, pos.x, pos.y)--木

  print(phbtn(0,1))
end








  