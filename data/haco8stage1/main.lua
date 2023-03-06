cl = 4
sound=-1
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
  -- return celc
  return fget(celc,0)
end

function collition(x,y)
  return get_map_flag(x-cl,y+2-cl) or get_map_flag(x+6-cl,y+2-cl) or get_map_flag(x-cl,y+7-cl) or get_map_flag(x+6-cl,y+7-cl)
end

x=12
y=12
lefttop=0
righttop=0
leftbottom=0
rightbottom=0

s=1 -- スプライト番号
d=1 -- 方向を示す                                                                                  
ipf=8 -- アニメーション1フレームについての時間(1ipf = 1/30秒)
nf=2 -- アニメーションするフレーム数(足踏みは2フレーム)
t=0

last_sound=-1

function _init()
  -- ここに書いてもグローバル変数になるようにしたい
  
  music(0);--引数変えてもまだ音楽は変わりません（コメントアウトすると音楽なしに）
end

function input()
  
  local prex = x
  local prey = y
  local pressed=false
  if (btn(1)>=1) then
  -- sound=0
   x = x-1
   d=1
   pressed=true
  end
  if (btn(2)>=1) then
  -- sound=1
    x = x+1
   d=2
   pressed=true
  end
  if (btn(3)>=1) then
  -- sound=2
   y = y-1
   d=3
   pressed=true
  end
  if (btn(4)>=1) then
  -- sound=3
   y = y+1
   d=4
   pressed=true
  end
  if pressed then
   s=d+flr((t%(nf*ipf))/ipf+1)*8 --8個先が次のコマ
  else
   s=d
  end

  if collition(x,y) == true then
    sound=0
    x=prex
    y=prey
  end
 end

function _update()

  sound=-1
  -- fset(45,0,0)--水スプライト45を通れなく（0ビットを1に）する
  -- fset(45,1,1)--水スプライト45を通れなく（0ビットを1に）する
  -- fset(45,2,1)--水スプライト45を通れなく（0ビットを1に）する
  -- fset(45,3,1)--水スプライト45を通れなく（0ビットを1に）する
  -- fset(45,4,1)--水スプライト45を通れなく（0ビットを1に）する
  -- fset(11,0,0)--岩スプライト11を通れなく（0ビットを1に）する
  -- fset(42,0,0)--緑スプライト11を通れるように（0ビットを0に）する
  -- fset(52,0,0)--スプライトを通れるように（0ビットを0に）する
  t = t+1
  input()
  
  if (btnp(5) == true) then
    sound=1
  end
  if (btnp(6) == true) then
    sound=2
  end
  if (btnp(7) == true) then
    sound=3
  end
  if (btnp(8) == true) then
    sound=4
  end


  -- if sound>-1 then
    sfx(sound)
    last_sound=sound
  -- end

  -- tone(0, 523, 0)
  -- tone(1, 659, 0)
  -- tone(2, 784, 0)

  -- tone(0, 0)
  -- tone(1, 0)
  -- tone(2, 0)

  -- lefttop=get_map_flag(x+1-cl,y+2-cl)
  -- righttop=get_map_flag(x+5-cl,y+2-cl)
  -- leftbottom=get_map_flag(x+1-cl,y+7-cl)
  -- rightbottom=get_map_flag(x+5-cl,y+7-cl)

end

function _draw()
  cls(0)
  map(0,0,0,0,8,8,"/init/map/0.png")
  -- spr8(1,x,y,1,1,false,false)

  if x>46 and y>46 then go2("/haco8stage2/main.lua", 100) end--状態3で立ち上げる
  spr8(9,48,48)
  spr8(s+64,x,y)

  print(gstat(),68,2,7)--ゲーム状態を表示

  -- pset(x+1-cl,y+2-cl, 8)
  -- pset(x+5-cl,y+2-cl, 8)
  -- pset(x+1-cl,y+7-cl, 8)
  -- pset(x+5-cl,y+7-cl, 8)
  -- print(lefttop.."/"..righttop,68,2,7)
  -- print(leftbottom.."/"..rightbottom,68,8,7)

  -- rectfill(0,0,127,127,0)
 -- 前回再生されたSFX番号を画面に表示する
--  if last_sound>-1 then
  print(last_sound, 68,14,7)
--  end
  
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
