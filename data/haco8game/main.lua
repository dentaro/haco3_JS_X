function get_map_flag(sprx,spry)
  local celx=flr(sprx/8)
  local cely=flr(spry/8)
  local celc=mget(celx,cely)
  return fget(celc,0)
end

function collition(x,y)
  return get_map_flag(x+1,y+2) or get_map_flag(x+6,y+2) or get_map_flag(x+1,y+7) or get_map_flag(x+6,y+7)
end

x=8
y=8
lefttop=0
righttop=0
leftbottom=0
rightbottom=0

function _init()
  -- ここに書いてもグローバル変数になるようにしたい
  -- x=8
  -- y=8
end

function _update()

  local prex = x
  local prey = y

  if (btn(1)>=1) then x = x-1 end
  if (btn(2)>=1) then x = x+1 end
  if (btn(3)>=1) then y = y-1 end
  if (btn(4)>=1) then y = y+1 end

  if collition(x,y) == 1 then
    x=prex
    y=prey
  end

  -- lefttop=get_map_flag(x,y)
  -- righttop=get_map_flag(x+7,y)
  -- leftbottom=get_map_flag(x,y+7)
  -- rightbottom=get_map_flag(x+7,y+7)
end

function _draw()
  cls(0)
  map(0,0,0,0,8,8)
  spr8(1,x,y,1,1,false,false)
  print(lefttop.."/"..righttop,68,2,7)
  print(leftbottom.."/"..rightbottom,68,8,7)
  
end
-------------------------------------------------------
function setup()--init
  _init()
end
function loop()--update --draw
_update()
_draw()
end
