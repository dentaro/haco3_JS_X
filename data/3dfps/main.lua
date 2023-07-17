--プレイヤーの座標のスプライト番号を取得する
function get_map_sprn(sprx,spry)
  local celx=flr(sprx/8)
  local cely=flr(spry/8)
  local celc=mget(celx,cely)
  return celc
end

function get_map_flag(sprx,spry)
  local celx=flr(sprx/8)
  local cely=flr(spry/8)
  local celc=mget(celx,cely)
  return fget(celc,0)
end

function collision(x,y)
  return get_map_flag(x-cl,y+2-cl) or get_map_flag(x+6-cl,y+2-cl) or get_map_flag(x-cl,y+7-cl) or get_map_flag(x+6-cl,y+7-cl)
end

x=12
y=12
angle = 0
lefttop=0
righttop=0
leftbottom=0
rightbottom=0

s=1 -- スプライト番号
d=1 -- 方向を示す                                                                                  
ipf=8 -- アニメーション1フレームについての時間(1ipf = 1/30秒)
nf=2 -- アニメーションするフレーム数(足踏みは2フレーム)
t=0

wallheight = 32

closestDistance = 999
centerangle = 32
raylength = 60
divnum = 32
angleStep = 64/divnum
centerno = divnum/2

-- walls = {
--   {30, 10, 60, 80, 90},
--   {30, 10, 10, 90, 180}, 
--   {60, 80, 10, 90, 270} 
-- }

bw = 16
bx = 16
by = 16

bx2 = 32
by2 = 32
-- walls = {
--   {bx+0, by+0, bx+bw, by+0, 10},
--   {bx+bw, by+0, bx+bw, by+bw, 9}, 
--   {bx+bw, by+bw, bx+0, by+bw, 8}, 
--   {bx+0, by+bw, bx+0, by+0, 3} 
-- }

walls = {
  {bx+0, by+0, bx+bw, by+0, 240},
  {bx+bw, by+0, bx+bw, by+bw, 60}, 
  {bx+bw, by+bw, bx+0, by+bw, 120}, 
  {bx+0, by+bw, bx+0, by+0, 180},

  {bx2+bw, by2+0, bx2+bw, by2+bw, 60}, 
  {bx2+bw, by2+bw, bx2+0, by2+bw, 120}, 
  {bx2+0, by2+bw, bx2+0, by+0, 180} 
}

function _init()
  wini(32)
end

function input()
  --wifiserve("ap")
  x = tp(0)
  y = tp(1)

  -- カメラの移動と回転
  if btn(1) >= 2 then
    x = x - 1  -- 左に移動
  elseif btn(2) >= 2 then
    x = x + 1  -- 右に移動
  end

  if btn(3) >= 2 then
    y = y - 1  -- 上に移動
  elseif btn(4) >= 2 then
    y = y + 1  -- 下に移動
  end
  if btn(8) >= 2 then
    angle = angle - 1  -- 左に回転
  elseif btn(7) >= 2 then
    angle = angle + 1    -- 右に回転
  end

  if angle > 360 then 
    angle = 1
  end

  if angle == 0 then 
    angle  = 360
  end
 end

function _update()
  input()
end

function _draw()
  cls(0)

  -- map(0,0,0,0,8,8,"/init/map/0.png")
  -- spr8(9,48,48)
  -- spr8(s+64,x, y)

  -- print(gstat(),68,64+2,7)--ゲーム状態を表示
  -- print(get_map_sprn(tp(0), tp(1)), 68,64+14,7)
  
  
  angle = (angle+360)%360--プレイヤーの角度

  for _, wall in ipairs(walls) do
    local x1, y1, x2, y2, color = table.unpack(wall)
      line(x1, y1, x2, y2, color)--壁の線を描画
      wset(angle, x, y, x1, y1, x2, y2, color)--壁を1枚設置
  end

  wdraw(angle)--壁を３D描画
  drawrect(64,0, 64,64, 7)

  spr8(s+64,x, y)

end

-------------------------------------------------------
firstF = true
function setup()--使えず、、、、
end

function loop()--update --draw
if firstF == true then
  _init()
  firstF = false
end
_update()
_draw()
end
