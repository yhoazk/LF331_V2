APP:=frame_grabber

all: clean comp

clean:
	rm -f ${APP}
	rm -f ${APP}_cpp

comp:
	gcc -Wall -O3 -o ${APP} v4lgrabber.c -lv4l2
	g++ -Wall -O3 -o ${APP}_cpp v4lgrabber.cpp -lv4l2

