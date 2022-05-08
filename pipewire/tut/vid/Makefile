APP:=video_play


comp:
	gcc -Wall ${APP}.c -I/usr/include/pipewire-0.3/ -I/usr/include/SDL2/ -I/usr/include/spa-0.2/ -lSDL2 -lSDL -lpipewire-0.3 -o ${APP}

clean:
	rm -f ${APP}
