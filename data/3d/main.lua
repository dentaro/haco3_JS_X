-- 3Dポリゴンの頂点座標
vertices = {
    {-16, -16, 0},
    {16, -16, 0},
    {0, 16, 8},
  }
  -- 描画する3Dポリゴンの回転角度
angle = 0
  function _update()
    angle = angle + 0.1
  end
  
  function _draw()
    
    cls()

  -- 3Dポリゴンの座標を計算する
  local cos = math.cos(angle)
  local sin = math.sin(angle)
  local points = {}

  -- テーブルの初期化
  local index = 1

  for _, vertex in ipairs(vertices) do
    local x, y, z = vertex[1], vertex[2], vertex[3]
    local x2 = x * cos - z * sin
    local z2 = z * cos + x * sin
    local scale = 64 / (64 - z2) -- パースペクティブ補正
    local sx = x2 * scale + 64
    local sy = y * scale + 64

    -- テーブルに座標を追加する
    points[index] = {}
    points[index].x = sx
    points[index].y = sy

    index = index + 1
  end
    -- 3Dポリゴンを描画する
    line(flr(points[1].x), flr(points[1].y), flr(points[2].x), flr(points[2].y), 7)
    line(flr(points[2].x), flr(points[2].y), flr(points[3].x), flr(points[3].y), 7)
    line(flr(points[3].x), flr(points[3].y), flr(points[1].x), flr(points[1].y), 7)
  end  

-------------------------------------------------------
function setup()--init
  _init()
end
function loop()--update --draw
_update()
_draw()
end







