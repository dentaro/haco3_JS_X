-----------------------------------------------------------
-- voronoi diagram
-- (c) time tripper 2016

bit={} -- メタテーブルを初期化する
bn=20
bs=32
bc=9
bf=9

function make_bit()
  local b={}
  b.x=rnd(128)
  b.y=rnd(128)
  b.c=1+flr(rnd(15))
  b.dx=2-rnd(4)
  b.dy=2-rnd(4)
  -- add(bit,b)
  bit[#bit+1] = b
  -- add t v
  -- t[#t+1] = v
end

function move_bit(sp)
 sp.x = sp.x + sp.dx
 sp.y = sp.y + sp.dy
 if sp.x < 0 then sp.dx = -sp.dx end
 if sp.y < 0 then sp.dy = -sp.dy end
 if sp.x > 127 then sp.dx = -sp.dx end
 if sp.y > 127 then sp.dy = -sp.dx end
end

function setup()
 for i=1,bn do
  make_bit()
 end
end

function loop()
  bx = tp(0)
  by = tp(1)
 if btn(0) and bc>1 then bc=bc-1 end
 if btn(1) and bc<32 then bc=bc+1 end
 if btn(2) and bf>0 then bf=bf-1 end
 if btn(3) and bf<15 then bf=bf+1 end
 if btn(4) then make_bit() end
 if btn(5) then 
  -- del(bit,bit[1]) 
  bit[#bit-1] = bit[1]
end
--  foreach(bit,move_bit)
 cls()
 for i=bs,1,-1 do
  for j,b in pairs(bit) do
  -- for b in all(bit) do
  if j == 1 then 
    circfill(bx,by,i,(b.c+bf+flr(i/bc))%16)
  end
   circfill(b.x,b.y,i,(b.c+bf+flr(i/bc))%16)
  end
 end

  table.remove(bit, 1)--9番目を削除

--  print("count="..count(bit),0,0,8)
 print("count="..#bit,0,0,8)
 print("pattern="..bc,0,6,8)
 print("color="..bf,0,12,8)

end