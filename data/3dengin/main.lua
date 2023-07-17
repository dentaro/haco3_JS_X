--プレイヤーの座標のスプライト番号を取得する
function get_map_sprn(sprx,spry)
  celx=flr(sprx/8)
  cely=flr(spry/8)
  celc=mget(celx,cely)
  return celc
end

function get_map_flag(sprx,spry)
  celx=flr(sprx/8)
  cely=flr(spry/8)
  celc=mget(celx,cely)
  return fget(celc,0)
end

function collision(x,y)
  return get_map_flag(x-cl,y+2-cl) or get_map_flag(x+6-cl,y+2-cl) or get_map_flag(x-cl,y+7-cl) or get_map_flag(x+6-cl,y+7-cl)
end
ypos = 1

px=12
py=12
angle = 0
lefttop=0
righttop=0
leftbottom=0
rightbottom=0

sn=1 -- スプライト番号
wallheight = 32

closestDistance = 999
centerangle = 32

p = { x = 10, 
      y = 10,
      angle = 0,      -- 視点の角度（度数法）
      height = 25 
    }

function _init()

end

function input()
  p.height = btn(3,4,1, p.height)
  p.angle  = btn(7,8,3, p.angle )
  -- if btn(3) >= 2 then
  --   p.height = p.height + 1  -- 上に移動
  -- elseif btn(4) >= 2 then
  --   p.height = p.height - 1  -- 下に移動
  -- end

  -- if btn(8) >= 2 then
  --   p.angle = p.angle - 3  -- 左に回転
  -- elseif btn(7) >= 2 then
  --   p.angle = p.angle + 3    -- 右に回転
  -- end

  p.x = tp(0)
  p.y = tp(1)

  if p.angle > 360 then 
    p.angle = 1
  end

  if p.angle == 0 then 
    p.angle  = 360
  end
 end

-- function fv(v)
--  return 0.25/v
-- end

function _update()
  input()
end

function _draw()
  cls(0)
  -- map(0,0, 0,56, 16,8,"/init/map/0.png")
  -- ypos=ypos+1
  spmap(0,60, 128,60, 0,0, 16,8, p.x, p.y, p.angle, p.height, "/init/map/0.png")
  -- if ypos==16 then ypos = 1 end
  -- spr8(sn+64, p.x, p.y)

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
