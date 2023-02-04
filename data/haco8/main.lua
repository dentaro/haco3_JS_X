----------------------------------------
function sum(a,b)
  print(a+b)
end

function pico()
  -- print("位置指定なしだととてもとても長い文章になると改行します")
  -- print("白文字黒背景で自動改行")

  -- print("haco"..8,20,50,9)

  -- color(3)
  -- print("位置指定ありだと折り返します",80,30)

  -- print("タッチ位置",tp(0),tp(1),1)

  sum(3,5)
  -- cls()
  -- rectfill(10,10,107,107,1)

  color(13)
  line(35,30,20,90)
  line(20,90,110,65)
  line(110,65,35,30)

  color(8)
  circ(35,30,2)
  circ(20,90,2)
  circfill(110,65,2)

-- line(35,30,20,90,1)
-- line(20,90,110,65,1)
-- line(110,65,35,30,1)

-- pset(35,30,7)
-- pset(20,90,7)
-- pset(110,65,7)
end

x=60
y=60
r = 0
cnt = 0
celx= 0
cely= 0
function _init()
  -- ここに書いてもグローバル変数になるようにしたい
  -- x=60
  -- y=60
end
function _update()
  if btn(1) > 2 then x = x-1 end
  if btn(2) > 2 then x = x+1 end
  if btn(3) > 2 then y = y-1 end
  if btn(4) > 2 then y = y+1 end
end
function _draw()
  cls(1)
  -- map(0,0,32,30,8,8)
  map(0,0,0,0,8,8)
  -- spr(x, y, 8,8, 1*8,1*8)--o-bakoライクなスプライトの書き方
  spr8(1,tp(0),tp(1),2,3,-2,2, r)--pico8ライクなスプライトの書き方

  if r%3 == 0 then celx = celx + 1 end
  if celx%16 == 0 then cely = cely + 1 end
  
  if celx == 16 then celx = 0 end
  if cely == 16 then cely = 0 end
  --cntは0～64まで回る

  -- spr8(1,tp(0),tp(1),1,1)--pico8ライクなスプライトの書き方
  spr8(mget(celx,cely),110,8,1,1)

  -- spr8(9,110,8,1,1)--第1引数に正しいスプライト番号を入れても適切なスプライトが返らない

   -- map( マップセルX, マップセルY, 表示画面X, 表示画面Y, セルの横の幅, セルの縦の数, レイヤー )
  --  map(0,0,tp(0),tp(1),16,16)
  -- map(1,tp(0),tp(1),1,1,1,1,0)
  r = r+1
  if r == 360 then r = 0 end

  -- cnt = cnt + 1
  -- if cnt > 128 then cnt = 0 end

  -- pico()
end

-- function _draw()

--   cls(7)
--   -- rectfill(10,10,107,107,1)
--   spr(64-4, 64-4, 8,8, 1*8,1*8)
-- end

-- cls()
-- rectfill(10,10,107,107,12)
-- spr(1, 64-4, 64-4)-- pico8の表記法は検討中で使えない
-- spr(64-4, 64-4, 8,8, 1*8,1*8)-- o-bako表記法　
--spr(x,y,w,h,sx,sy[,sw,sh])	(x,y)にspriteを(sx,sy)からサイズはw,hで描画する。sw,shを設定すると拡大、縮小できる。


-------------------------------------------------------
function setup()--init
  _init()
end
function loop()--update --draw
_update()
_draw()
end
