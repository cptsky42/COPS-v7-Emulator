--
-- ------ COPS v7 Emulator - Open Source ------
-- Copyright (C) 2012 - 2014 Jean-Philippe Boivin
--
-- Please read the WARNING, DISCLAIMER and PATENTS
-- sections in the LICENSE file.
--

function processTask10050(client, idx)
   name = "Conductress"
   face = 1
   
   if (idx == 0) then
      
      text(client, "Where do you want to go ? I can teleport you for 100 silvers !")
      link(client, "Phoenix Castle", 1)
      link(client, "Desert City", 2)
      link(client, "Ape Mountain", 3)
      link(client, "Bird Islands", 4)
      link(client, "Mine Cave", 5)
      link(client, "Market", 6)
      link(client, "No, thanks.", 255)
      pic(client, face)
      create(client)
      
   elseif (idx == 1) then

      money = getMoney(client)
      if (money >= 100 and spendMoney(client, 100)) then
         move(client, 1002, 958, 555)
      else
         text(client, "It seems that you don't have enough money !")
         link(client, "Ok.", 255)
         pic(client, face)
         create(client)
      end
      
   elseif (idx == 2) then

      money = getMoney(client)
      if (money >= 100 and spendMoney(client, 100)) then
         move(client, 1002, 69, 473)
      else
         text(client, "It seems that you don't have enough money !")
         link(client, "Ok.", 255)
         pic(client, face)
         create(client)
      end

   elseif (idx == 3) then

      money = getMoney(client)
      if (money >= 100 and spendMoney(client, 100)) then
         move(client, 1002, 555, 957)
      else
         text(client, "It seems that you don't have enough money !")
         link(client, "Ok.", 255)
         pic(client, face)
         create(client)
      end

   elseif (idx == 4) then

      money = getMoney(client)
      if (money >= 100 and spendMoney(client, 100)) then
         move(client, 1002, 232, 190)
      else
         text(client, "It seems that you don't have enough money !")
         link(client, "Ok.", 255)
         pic(client, face)
         create(client)
      end

   elseif (idx == 5) then

      money = getMoney(client)
      if (money >= 100 and spendMoney(client, 100)) then
         move(client, 1002, 53, 399)
      else
         text(client, "It seems that you don't have enough money !")
         link(client, "Ok.", 255)
         pic(client, face)
         create(client)
      end

   elseif (idx == 6) then

      money = getMoney(client)
      if (money >= 100 and spendMoney(client, 100)) then
         move(client, 1036, 292, 236)
      else
         text(client, "It seems that you don't have enough money !")
         link(client, "Ok.", 255)
         pic(client, face)
         create(client)
      end
      
   end

end
