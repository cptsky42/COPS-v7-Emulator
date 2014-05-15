--
-- ------ COPS v7 Emulator - Open Source ------
-- Copyright (C) 2014 Jean-Philippe Boivin
--
-- Please read the WARNING, DISCLAIMER and PATENTS
-- sections in the LICENSE file.
--

function processTask10052(client, idx)
   name = "Conductress"
   face = 1
   
   if (idx == 0) then
      
      text(client, "Where do you want to go ? I can teleport you for 100 silvers !")
      link(client, "Twin City", 1)
      link(client, "Market", 2)
      link(client, "No, thanks.", 255)
      pic(client, face)
      create(client)
      
   elseif (idx == 1) then

      money = getMoney(client)
      if (money >= 100 and spendMoney(client, 100)) then
         move(client, 1011, 11, 376)
      else
         text(client, "It seems that you don't have enough money !")
         link(client, "Ok.", 255)
         pic(client, face)
         create(client)
      end
      
   elseif (idx == 2) then

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
