-- --<幻覚の森から>
--     --リセットスタートを押してから矢印キーでハコさんが歩きます。ところが森に入ると、、、。ハコさんこのあとどうする？！
--     --保存ができないので、ブラウザで編集せずに、編集、保存したものをコピペして実行するのがおすすめです。
--     x = 0
--     y = 32
--     t = 0
--     flip = 0
--     count = 0

--     function setup()
--         color(0,0,0)
--         fillrect(0,0,128,128)
-- 	--森の背景を描く
--         for i = 0 , 15 do
--             for j = 0 , 15 do
--         	spr(i*8, j*8, 8, 8, 16, 8)
--         	end
--         end
--     end

--     function loop()
--       t = t + 1

--       walk = 0
--       step = 1 --ここを変えると歩くスピードが変わります

--       if btn(0) > 0 then x = x - step  walk = 1 tone(0, 600, 32) count = 0 end--ここの600は音の高さを表しています
--       if btn(1) > 0 then x = x + step  walk = 1 tone(0, 800, 32) count = 0 end
--       if btn(2) > 0 then y = y - step  walk = 1 tone(0, 1000, 32) count = 0 end
--       if btn(3) > 0 then y = y + step  walk = 1 tone(0, 2000, 32) count = 0 end
--       count = count + 1
--     --   count = count%10
--       if count >= 3 then tone(0, 440, 0) end

--       if
--                 x > 128  then x = -8
--         elseif  x < -8  then  x = 128
--         elseif  y > 120  then y = -8
--         elseif  y < -8  then  y = 120
--       end
--       --背景を描きなおす
--       for i = 0 , 10 do
--             for j = 0 , 10 do
--         	spr(i*8, j*8, 8, 8, 8, 8) --ここを変えると背景を変えられます。
--         	end
--         end

--       if walk == 1 then

--        if t%3 == 0 then
--         flip = flip + 1
--         flip = flip%2
--        end

--        if flip == 0 then spr(x, y, 8, 8, 16, 0) end
--        if flip == 1 then spr(x, y, 8, 8, 24, 0) end

--       else
--        spr(x, y, 8, 8, 8, 0) --ここを変えるとキャラの絵を変えられます
--       end

--     end

--<ハコさんがハコファイターに！？>
      --リセットスタートを押してから矢印キーでハコさんがハコファイターに！？　ハコさんこのあとどうする？！
      --保存ができないので、ブラウザで編集せずに、編集、保存したものをコピペして実行するのがおすすめです。
      x = 0
      y = 32
      t = 0
      flip = 0
      count = 0

      function setup()
          color(0,0,0)
          fillrect(0,0,128,128)
    --森の背景を描く
          for i = 0 , 15 do
              for j = 0 , 15 do
            spr(i*8, j*8, 8, 8, 8*5, 8*13)
            end
          end
      end

      function loop()
          color(0,0,0)
          fillrect(0,0,128,128)
        t = t + 1

        walk = 0
        step = 1 --ここを変えるとスピードが変わります

        if btn(0) > 0 then x = x - step  walk = 1 tone(0, 600, 32) count = 0 end--ここの600は音の高さを表しています
        if btn(1) > 0 then x = x + step  walk = 1 tone(0, 800, 32) count = 0 end
        if btn(2) > 0 then y = y - step  walk = 1 tone(0, 1000, 32) count = 0 end
        if btn(3) > 0 then y = y + step  walk = 1 tone(0, 2000, 32) count = 0 end
        count = count + 1
      --   count = count%10
        if count >= 3 then tone(0, 440, 0) end

        if
                  x > 128  then x = -8
          elseif  x < -8  then  x = 128
          elseif  y > 120  then y = -8
          elseif  y < -8  then  y = 120
        end
        --背景を描きなおす
        for i = 0 , 10 do
              for j = 0 , 10 do
              --ここを変えると背景を変えられます。スプライトを0から数えて１番目の下13行目(8*1, 8*13)という意味です。
            spr(i*8, j*8, 8, 8, 8*1, 8*13)
            end
          end

        if walk == 1 then

         if t%3 == 0 then
          flip = flip + 1
          flip = flip%2
         end

         if flip == 0 then spr(x, y, 8, 8, 8*4, 8*14) end
         if flip == 1 then spr(x, y, 8, 8, 8*4, 8*14) end

        else
         spr(x, y, 8, 8, 8*1, 8*0) --ここを変えるとキャラの絵を変えられます
        end

      end
