clear
#echo compiling debug 
#	DEBUG
#time emcc -g -sALLOW_MEMORY_GROWTH -fsanitize=address -o index.html menu.c trail.c tanks.c utils.c bullet.c index.c camera.c player.c map.c --preload-file ../res -Wall ../raylib/src/libraylib.a -I. -I../raylib/src -L. -L../raylib/src/libraylib.a -s USE_GLFW=3 --shell-file shell.html #-DPLATFORM_WEB
#	RELESE
time emcc -sALLOW_MEMORY_GROWTH -Oz -s -o index.html menu.c trail.c tanks.c utils.c bullet.c index.c camera.c player.c map.c --preload-file ../res -Wall ../raylib/src/libraylib.a -I. -I../raylib/src -L. -L../raylib/src/libraylib.a -s USE_GLFW=3 --shell-file shell.html #-DPLATFORM_WEB
