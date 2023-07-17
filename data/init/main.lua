--  launcher
-- count = 0
-- color(12)
count = 0

fs = list() --落ちる

cursor = 0
scroll = 0
tabCursor = 0
showmax = 7 --ファイルの最大表示数0~
lineheight = 11
function setup()

end

function _init()
  -- fs = list()
end


function drawTab(x, y, w)
  spr(x, y,8,8,32,0,8,8) -- left
  for i = 1,w do
    spr(x + i*8, y,8,8,40,0,8,8) -- middle
  end
  spr(x + w*8 ,y,8,8,48,0,8,8) -- right
end

function drawDisableTab(x, y, w)
  spr(x, y,8,8,32,8,8,8) -- left
  for i = 1,w do
    spr(x + i*8, y,8,8,40,8,8,8) -- middle
  end
  spr(x + w*8 ,y,8,8,48,8,8,8) -- right
end

function drawTile(x, y, w, h)
  for i=0,h-1 do
    for j=0,w-1 do
      spr(x + j*8, y + i*8,8,8,40,24,8,8)
    end
  end
end

function drawFile()
  drawTab(0,10,4)
  color(1)
  text("file", 12, 10)

  drawDisableTab(40,10,4)
  color(1)
  text("util", 52, 9)

  for i = scroll,#fs do
    if i - scroll >= 0 and i - scroll <= showmax then
      v = fs[i] --パス名
      -- text(i .. v, 10, 20 + lineheight * (i-scroll))--先頭数字
      text(v, 10, 20 + lineheight * (i-scroll))--先頭数字なし
    end
  end

  spr(0, 20 + (cursor-scroll)*lineheight, 8, 8, 32, 16, 8, 8)

  if btn(3) == 2 then
    cursor = cursor - 1
    if cursor < 0 then
      cursor = #fs
    end
  end

  if btn(4) == 2 then
    cursor = cursor + 1
    if cursor > #fs then
      cursor = 0
    end
  end

  if cursor < scroll then
    scroll = cursor
  end
  if cursor > showmax then
    scroll = cursor - showmax
  end

  if btn(1) == 2 then
    run(fs[cursor])
  end
end

utilMenu = {"reload", "wifi on", "self wifi on", "reboot", "tone on", "tone off"}

function drawUtil()
  drawDisableTab(0,10,4)
  color(1)
  text("file", 12, 9)

  drawTab(40,10,4)
  color(1)
  text("util", 52, 10)

  for k, v in pairs(utilMenu) do
    text((k - 1) .. ":" .. v, 10, 20 + 10 * (k - 1))
  end
  --fillrect(0, 20 + cursor * 10, 10, 10)
  spr(0, 20 + cursor*10, 8, 8, 32, 16, 8, 8)

  if btn(3) == 2 then
    cursor = cursor - 1
    if cursor < 0 then
      cursor = #utilMenu - 1
    end
  end

  if btn(4) == 2 then
    cursor = cursor + 1
    if cursor >= #utilMenu then
      cursor = 0
    end
  end

  if btn(1) == 2 then
    if cursor == 0 then
      run("/init/main.lua")
    elseif cursor == 1 then
     --ATPモード：共有のWifiに入るモード（通常はこちら/init/param/wifipass.txtを書き換えることで設定できる）
      if not(iswifidebug()) then
        wifiserve()
      end
    elseif cursor == 2 then
     --APモード：ESP32自体がアクセスポイントになるモード
      if not(iswifidebug()) then
        wifiserve("ap")
      end
    elseif cursor == 3 then
      reboot()
    elseif cursor == 4 then
     --  tone(0, 523)
     --  tone(1, 659)
      -- tone(2, 784)
    elseif cursor == 5 then
      -- tone(0, 0)
      -- tone(1, 0)
      -- tone(2, 0)
    end
  end
end

function _update()
  color(1)
  fillrect(0, 0, 128, 120)
  color(7)
  if iswifidebug() then
    text("wifi: on", 0, 0)
    text(getip(), 50, 0)
  else
    text("wifi: off", 0, 0)
  end

  if btn(0) == 2 then
    tabCursor = tabCursor - 1
    if tabCursor < 0 then
      tabCursor = 1
    end
    cursor = 0
  end
  if btn(2) == 2 then
    tabCursor = tabCursor + 1
    if tabCursor > 1 then
      tabCursor = 0
    end
    cursor = 0
  end

  drawTile(0,18,16,12)

  if tabCursor == 0 then
    drawFile()
  else
    drawUtil()
  end
  count = count + 1
end

-- function _draw()
--   drawTile(0,18,16,12)
--   if tabCursor == 0 then
--     drawFile()
--   else
--     drawUtil()
--   end
-- end

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
-- _draw()
end