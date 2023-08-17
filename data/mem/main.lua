x=15
y=15

function setup()
  fillrect(20,20,80,80,7)
end

function _init()--1回だけ
    setup()
end

function input()
  x = tp(0)
  y = tp(1)
end

function _update()--ループします
  input()
end

function _draw()--ループします
  cls()
  fillrect(x,y,15,15,8)
end








  