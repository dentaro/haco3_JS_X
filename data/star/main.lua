initstars()
creobj()

colangle = 0

-- drawmode = 0;--グリッドなし
starmode = 1;--グリッドあり

camera = {
  x = 0,
  y = 0,
  z = 0,
  x2 = 0,
  y2 = 0,
  z2 = -1,
  anglex = 1,
  angley = 0,
  anglez = 0,
  zoom = 0.1
}

light = {
  x = 1,
  y = 3,
  z = 10
}

function setup()
  fillrect(20,20,80,80,7)
end

function _init()--1回だけ
    setup()
end

function _update()

  if btn(6) >= 2 then--3
    -- appmode(appinfo(0), 1)--エディットモードで開く
    -- appmode("/00sample/main.lua", 1)--エディットモードで開く
  end


  if btnp(5) == true then 
    drawmode = drawmode + 1 
    drawmode = drawmode % 3
  end


  -- if btn(7) >= 2 then 
  --   camera.zoom = camera.zoom - 0.5  -- 左に回転
  -- end
  -- if btn(8) >= 2 then 
  --   camera.zoom = camera.zoom + 0.5  -- 右に回転
  -- end


  if btn(1) >= 2 then   
    -- camera.angley  = camera.angley + 2
    camera.zoom = camera.zoom - 0.5
  end

  if btn(2) >= 2 then 
    -- camera.angley  = camera.angley - 2
    camera.zoom = camera.zoom + 0.5
  end

  if btn(3) >= 2 then   
    camera.anglex  = camera.anglex - 2
  end

  if btn(4) >= 2 then 
    camera.anglex  = camera.anglex + 2
  end

  camera.angley = camera.angley + 1

  if camera.angley > 360 then
    camera.angley = camera.angley-360
  end
  
  camera.zoom = 0.7

end 

function _draw()
  cls()
  drawstars( camera.anglex, camera.angley, camera.zoom, 1)
  spr8(1,starp(camera.anglex, camera.angley, camera.zoom, 0,  20,41,26, 45,16,49), 
         starp(camera.anglex, camera.angley, camera.zoom, 1,  20,41,26, 45,16,49))
  print(camera.angley)

end
