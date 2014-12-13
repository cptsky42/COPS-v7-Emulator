                               COPS v7 Emulator
                               ----------------

                   Copyright (C) 2012 - 2014 Jean-Philippe Boivin
              https://bitbucket.org/jpboivin/cops-v7-emulator/overview

Overview
--------

COPS v7 Emulator is a fork of Faith Emulator, a C++ emulator of the MMORPG
Era of Faith. COPS v7 Emulator is a fork targeting the MMORPG Conquer Online,
at the patch 5065.

The emulator is developed in C++ with Qt4 / Qt5.
N.B. The QMySQL driver must be build to use the QSQL module properly.

Features
--------

COPS v7 Emulator is currently released as a base source. Most functionalities
are not implemented or completed. However, some features have special and
interesting implementation.

+ Support any POSIX and/or WinAPI operating system with Qt
+ Support any architecture (little endian & big endian | 32 bit & 64 bit)
+ Separate worker for logging (see zfserv.log)
+ Self destructed environment (useful for singletons)
+ MySQL5 database based on the official one

+ Security module
  - Custom implementations for:
    * TQ's custom cipher
    * Blowfish (CFB64)
    * RC5 (32/12/16)
    * Diffie-Hellman exchange

+ Map module
  - Parallel loading of Data Map (DMap) files
    * MapManager will detect the number of physical cores and spawn an appropriate
      amount of worker to load DMaps in parallel.
      e.g. On two PowerPC 970MP (dual core) @ 2.5 GHz, 4 workers will be spawn and
           it will takes less than 30s to load all DMap of a 5065 client.
  - Compressed data when unused
    * Each GameMap will detect when no player will be on the map, and than pack
      the data using the LZ4 algorithm and unpack the data when required.
      e.g. With a PowerPC (64 bit), at idle, the emulator will require less than
           10 MB of RAM to keep all DMap of a 5065 client in memory.
  - Shared data among all GameMaps

+ Script module
  - Shared Lua VM for executing scripts
  - Exposed accessors for the player informations
  - Exposed specialized functions (e.g. text(), link())

+ Generator module
  - Based on TQ's one, using the official spawns will result in the same in-game spawns

+ Character creation
+ Character loading / saving
+ Direction / Emotion / Pk Mode
+ Walking / Jumping
+ Portals
+ Points allocation
+ Talking to NPCs [Incomplete]
+ Screen system [Incomplete]

+ Item features
  - Buying items from NPCs
  - Selling items to NPCs
  - Repairing items
  - Using / eating items
  - Equiping items
  - Real item ID generation for monsters [Incomplete]

Documentation about the emulator can be generated with dOxygen.
To generate the documentation, please in the root folder of the source,
and call dOxygen with Doxyfile.

Supported systems
-----------------

The emulator has been tested on the following platforms:
- Windows XP Professional (SP2)
  + x86_64 [Will compile, untested otherwise]
- Mac OS X Leopard (10.5.8)
  + ppc, ppc64
- Mac OS X Mavericks (10.9.2)
  + (x86), x86_x64
- Debian Lenny (5.0)
  + ARM (armel)

The emulator has been tested with Qt 4.6.3, 4.8.4 & 5.2.1.

However, the emulator should work without any modification on any
POSIX-compliant system (Mac OS X, *NIX, GNU/Linux, etc) and on any
WinAPI system.

N.B. This emulator uses the Windows API and/or the POSIX API and/or the Qt API.
