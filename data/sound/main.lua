score = {0,0,0,0,0,0,0,0}
cursor = 1
playCursor = 1
count = 0

svol = 180;

function clearScore()
    for i = 1,8 do
        score[i] = 0
    end
end

function setup()
    clearScore()
end

function _init()--1回だけ
    setup()
end

function input()

--   if btn(1) >= 1 then
--     tone(0, 440, 128)
--   end
--   if btn(2) >= 1 then
--     tone(0, 440, 0)
--   end

if btn(1) >= 2 then
    if cursor == 1 then
        cursor = 8
    else
        cursor = cursor - 1
    end
end
if btn(2) >= 2 then
    if cursor == 8 then
        cursor = 1
    else
        cursor = cursor + 1
    end
end
if btn(3) >= 2 then
    score[cursor] = score[cursor] + 1
end
if btn(4) >= 2 then
    score[cursor] = score[cursor] - 1
end

if btn(7) >= 2 then
    score[cursor] = 0
end

end

function _update()--ループします
  input()
  
  
end

function _draw()--ループします
    
    color(0,0,0)
    fillrect(0,0,128,128)
    
    color(255,255,255)
    for i = 1,8 do
        fillrect((i - 1) * 16, (32 - score[i] - 1) * 4, 16-1,4)
    end
    
    color(0,255,0)
    drawrect((playCursor - 1)*16, 0, 16, 128)

    color(255,255,0)
    drawrect((cursor - 1)*16, 0, 16, 128)


    ------------------------------------


    count = count + 1
    if count % 10 == 0 then
        playCursor = playCursor + 1
        if playCursor > 8 then
            playCursor = 1
        end
        
        if score[playCursor] ~= 0 then
          -- play tone
          tone(0, score[playCursor]*10 + 440, 128)
        else
          tone(0, score[playCursor]*10 + 440, 0)
        end
    end

    if key() == 'q' then
        svol = svol + 1;
        vol(svol)
    end

    if key() == 'a' then
        svol = svol - 1;
        vol(svol)
    end
    

    print(key())
    print(svol)
    

end


-- function loop()
    -- draw()
    -- count = count + 1
    -- if count % 10 == 0 then
    --     playCursor = playCursor + 1
    --     if playCursor > 8 then
    --         playCursor = 1
    --     end
        
    --     if score[playCursor] ~= 0 then
    --       -- play tone
    --       tone(0, score[playCursor]*10 + 440, 128)
    --     else
    --       tone(0, score[playCursor]*10 + 440, 0)
    --     end
    -- end
    -- if btn(1) >= 3 then
    --     if cursor == 1 then
    --         cursor = 8
    --     else
    --         cursor = cursor - 1
    --     end
    -- end
    -- if btn(2) >= 3 then
    --     if cursor == 8 then
    --         cursor = 1
    --     else
    --         cursor = cursor + 1
    --     end
    -- end
    -- if btn(3) >= 2 then
    --     score[cursor] = score[cursor] + 1
    -- end
    -- if btn(4) >= 2 then
    --     score[cursor] = score[cursor] - 1
    -- end
    
    -- if btn(7) >= 2 then
    --     score[cursor] = 0
    -- end
-- end

