function _draw()
  cls()
  draw_tree(64, 120, -1.57, 20, 8)
end

function draw_tree(x, y, angle, length, depth)
  if depth == 0 then return end
  local x2 = x + cos(angle) * length
  local y2 = y + sin(angle) * length
  line(x, y, x2, y2, 7)
  draw_tree(x2, y2, angle - tp(0) / 360, length - tp(1) / 50, depth - 1)
  draw_tree(x2, y2, angle + tp(0) / 360, length - tp(1) / 50, depth - 1)
end

-------------------------------------------------------
function setup()--init
  _init()
end
function loop()--update --draw
  _draw()
end

