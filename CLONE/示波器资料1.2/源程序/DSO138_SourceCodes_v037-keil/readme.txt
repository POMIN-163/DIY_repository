=================================
DSO138 Source Codes
Ver: 113-13801-037

(C) JYE Tech Ltd. All right reserved

Web:     www.jyetech.com
Forum:   www.jyetech.com/forum
Email:   jyetek@gmail.com
=================================

1. Contents
-----------
Files contained in this package include:

 1 ) 113-13801.c     -- The main program
 2 ) Board.c         -- Functions for initialization and lower level operation
 3 ) Board.h         -- Header file
 4 ) Command.c       -- Functions for key command analysis and execution
 5 ) Command.h       -- Header file
 6 ) Common.c        -- Basic type definitions and general functions
 7 ) Common.h        -- Header file
 8 ) Eeprom.c        -- Emulation of EEPROM 
 9 ) Eeprom.h        -- Header file
10 ) stm32f103_it.c  -- Interrupt handlers
11 ) stm32f103_it.h  -- Header file
12 ) Screen.c        -- Functions for screen related operations
13 ) Screen.h        -- Header file 
14 ) libdso138.a     -- Library of DSO138 core
15 ) libdso138.h     -- Header file
16 ) stm32f10x_conf.h -- Configuration file
17 ) startup_stm32f10x_md.S -- Start-up file 
18 ) makefile        -- Makefile for building the project
19 ) STM32F10x_64k_20k_flash.ld -- Linker script
20 ) readme.txt      -- This file.
21 ) Libraries(folder) -- Libraries for STM32F10x MCUs

2. How To Build
---------------
The project was developped under CodeSourcery G++ Lite. Under Windows environment please follow the steps below to build the project:

1 ) Download Code Sourcery G++ Lite from http://www.mentor.com/. Install it and have environment variables setup properly as instructed in the accompanied documents.
2 ) Unzip all files in this package to a folder.
3 ) Open a command line window under Windows. Switch to the folder that contains the unzipped files with "CD" command. Type and execute "cs-make" from the folder.


3. Support
----------
There is no technical support for questions related to the source codes. However, users are welcome to present their questions to and seek helps in JYE Tech Forum (http://www.jyetech.com/forum).


4. License
----------
DSO138 source codes are free software and open for education, research, and commercial
developments under license policy of the following terms.

Copyright (C)2014 JYE Tech Ltd., All right reserved

1 ) DSO138 source codes are free software and there is NO WARRANTY.
2 ) You can use, modify, and redistribute it for personal, non-profit, or commercial product UNDER YOUR OWN RESPONSIBILITY.
3 ) Redistributions of source codes must retain the above copyright notice.



5. Revision History
-------------------

Version    Date         Summary
--------------------------------
0.37	   2015.03.05	Fixed a bug that sometimes causes hang when timebase is changed from 20ms to 50ms
0.36	   2014.12.25	Minor change to the library file
0.35       2014.12.20   First release.

