
function draw_mandelbrot(x_min, x_max, y_min, y_max, max_iterations)
  local w, h = 128, 128 -- 画面サイズ
  local x_step = (x_max - x_min) / w -- x方向の増分
  local y_step = (y_max - y_min) / h -- y方向の増分
  
  for j=0, h-1 do
    for i=0, w-1 do
      -- ピクセル座標を複素数に変換する
      local x = x_min + i * x_step
      local y = y_min + j * y_step
      local c = {x, y}
      
      -- マンデルブロ集合に属するかどうかを判定する
      local z = {0, 0}
      local k = 0
      
      while k < max_iterations and z[1]^2 + z[2]^2 < 4 do
        local new_z = {z[1]^2 - z[2]^2 + c[1], 2 * z[1] * z[2] + c[2]}
        z = new_z
        k = k + 1
      end
      
      -- マンデルブロ集合に属する場合は白色で塗りつぶす
      if k == max_iterations then
        pset(i, j, 7)
      end
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
    draw_mandelbrot(-2, 1, -1.5, 1.5, 50)
    flag = false
  end

-- _update()
_draw()
end