local i = math.sqrt(-1)--複素数を定義
z0,c0=0,1.5+0*i
z,c=z0,c0
color(6)
-- circfill(64,60,56)

 function _update()
  for i=1,100 do
   z=math.sqrt(z+c)
  end
 end
 
 function _draw()
  for y=0,119 do
   for x=0,127 do
    z=c0+x/127-0.75-(y/119-0.5)*1*i
    for j=1,10 do 
      z=math.sqrt(z+c) end
    pset(x,y,flr(math.abs(z*4)%8)+8)
   end
  end
 end

function _draw()
  
end
-------------------------------------------------------
flag = true
function loop()--update --draw
  if flag then
    -- _init()
    flag = false
  end
_update()
_draw()
end