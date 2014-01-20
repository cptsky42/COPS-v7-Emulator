--
-- ------ COPS v7 Emulator - Open Source ------
-- Copyright (C) 2012 - 2014 Jean-Philippe Boivin
--
-- Please read the WARNING, DISCLAIMER and PATENTS
-- sections in the LICENSE file.
--

function processTask10050(client, idx)

   if (idx == 0) then
      
      text(client, "Stupid NPC for example...")
      text(client, "Testing C functions : money=" .. getMoney(client) .. "$")
      link(client, "Check money", 1)
      link(client, "Check CPs", 2)
      link(client, "I was just passing.", 255)
      pic(client, 1)
      create(client)
      
   elseif (idx == 1) then
      
      text(client, "You have " .. getMoney(client) .. " golds.")
      link(client, "exit", 255)
      pic(client, 1)
      create(client)
      
   elseif (idx == 2) then
   
   end

end
