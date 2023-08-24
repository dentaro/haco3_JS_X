-- 円周率πの取得
local PI = 3.14159265358979323846
local TWO_PI = PI*2
local RAD_ONE = PI/180
x=15
y=15

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

movx = 30--h
movy = 30--d
zoom = 7--4

s = {5, 55, 10.30536, 7, 24, 25.4304}--ベテルギウス
-- s = {6, 45, 8.91728, -16, 42, 58}--シリウス
-- s = {2, 31, 49.09456, 89, 15, 50.7923}--ポラリス
-- s = {4, 35, 55.23907, 16, 30, 33.4885}--アルデバラン
-- s = {7, 39, 18.11950, 5, 13, 29.9552}--プロキオン

function setup()
  initstars()
  -- initstars(s[1], s[2], s[3], s[4], s[5], s[6],  zoom)--ベテルギウス
end

function _init()--1回だけ
    setup()
end

function input()

  if phbtn(2) == 1 then 
    zoom = zoom + 1
    cls()
    -- initstars(s[1], s[2], s[3], s[4], s[5], s[6],  zoom)
    -- initstars(starx, 45, 8.91728, stary, 42, 58, zoom)
    -- initstars(movx*ascunit, rangex, movy*(decunit-1), movy*(decunit+1), 100)
  end

  if phbtn(2) == 2 then 
    zoom = zoom - 1
    cls()
    -- initstars(s[1], s[2], s[3], s[4], s[5], s[6],  zoom)
    -- initstars(starx, 45, 8.91728, stary, 42, 58, zoom)
    -- initstars(movx*ascunit, rangex, movy*(decunit-1), movy*(decunit+1), 100)
  end

  if btnp(1) == true then
    print("視線を左に")
    s[1] = s[1] + movx
    cls()
    -- initstars(s[1], s[2], s[3], s[4], s[5], s[6],  zoom)
    -- initstars(starx, 45, 8.91728, stary, 42, 58, zoom)
  end

  if btnp(2) == true then
    print("視線を右に")
    s[1] = s[1] - movx
    cls()
    -- initstars(s[1], s[2], s[3], s[4], s[5], s[6],  zoom)
    -- initstars(starx, 45, 8.91728, stary, 42, 58, zoom)
  end

  if btnp(3) == true then
    print("視線を上に")
    s[4] = s[4] + movy
    cls()
    -- initstars(s[1], s[2], s[3], s[4], s[5], s[6],  zoom)
    -- initstars(starx, 45, 8.91728, stary, 42, 58, zoom)
  end

  if btnp(4) == true then
    print("視線を下に")
    s[4] = s[4] - movy
    cls()
    -- initstars(s[1], s[2], s[3], s[4], s[5], s[6],  zoom)
    -- initstars(starx, 45, 8.91728, stary, 42, 58, zoom)
  end

  --phbtn(3)は0~4095の値をボリュームから取得
  -- zoom = 10 + phbtn(2) * 5 / 4095;

  -- if phbtn(2) == 1 then 
  --   zoom = zoom + 0.1;
  -- end
  -- if phbtn(2) == 2 then 
  --   zoom = zoom - 0.1;
  -- end

  -- tx = tp(0)
  -- ty = tp(1)

  -- if (touch()==2) then --タッチされたら
  --   gpos = getgpos(tx, ty, zoom );
  --   walkAscPos = gpos.asc
  --   walkDecPos = gpos.dec
  -- end

  --アナログスティック
  if phbtn(0,1) == 7 then 
    s[3] = s[3] - (movx*zoom)
  end

  if phbtn(0,1) == 6 then 
    s[3] = s[3] - (movx*zoom)
    s[6] = s[6] - (movy*zoom)
  end

  if phbtn(0,1) == 5 then 
    s[6] = s[6] - (movy*zoom)
  end

  if phbtn(0,1) == 4 then 
    s[3] = s[3] + (movx*zoom)
    s[6] = s[6] - (movy*zoom)
  end

  if phbtn(0,1) == 3 then 
    s[3] = s[3] + (movx*zoom)
  end

  if phbtn(0,1) == 2 then 
    s[3] = s[3] + (movx*zoom)
    s[6] = s[6] + (movy*zoom)
  end

  if phbtn(0,1) == 1 then 
    s[6] = s[6] + (movy*zoom)
  end

  if phbtn(0,1) == 0 then 
    s[3] = s[3] - (movx*zoom)
    s[6] = s[6] + (movy*zoom)
  end
end

function _update()--ループします
  -- if firstF == true then
  input()
  -- end

  -- --指定座標（新宿駅）の周辺をぐるぐる回るための処理
  -- walkAngle = walkAngle - 1.5
  -- radius = 0.001 -- 回転半径（0.00002の距離）
  -- ZoomValue = 15--pow(2, getZtile())*0.00625;
  -- local gpos = getgpos(tx, ty, zoom );
  --   print(gpos.asc)
  --   print(gpos.dec)
  --   drawmaps( gpos.asc, gpos.dec, zoom )
  --   walkAscPos = gpos.asc
  --   walkDecPos = gpos.dec
  -- walkAscPos = ascPos + sin(walkAngle*RAD_ONE)*radius--ZoomValue＝15の時204.8;
  -- walkDecPos = decPos + cos(walkAngle*RAD_ONE)*radius--ZoomValue＝15の時204.8;
end

function _draw()--ループします
  cls()
  -- drawstars( Asc, Dec, zoom )
  drawstars(s[1], s[2], s[3], s[4], s[5], s[6],  zoom)

  -- print(walkAscPos)
  -- print(walkDecPos)
  -- local pos = getstarpos( 35.68999241096032, 139.70054064148115, zoom )--新宿
  -- spr8( 64, pos.x, pos.y)--火
  -- local pos = getstarpos( 35.6783071, 139.7631488, zoom )
  -- spr8( 32, pos.x, pos.y)--木

  -- print(phbtn(0,1))
end








  