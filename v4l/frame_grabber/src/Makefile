APP:=frame_grabber

all: clean comp

clean:
	rm -f ${APP}
	rm -f ${APP}_cpp_src*
	rm -f v4l2grab
	rm -f video

comp:
	gcc -Wall -O3 -DVIDEO_SRC=\"/dev/video0\" -o ${APP} v4lgrabber.c -lv4l2
	gcc -Wall -O3 -DVIDEO_SRC=\"/dev/video2\" -o ${APP} v4lgrabber.c -lv4l2

	g++ -Wall -O3 -DVIDEO_SRC=\"/dev/video0\" -o ${APP}_cpp_src0 v4lgrabber.cpp -lv4l2
	g++ -Wall -O3 -DVIDEO_SRC=\"/dev/video2\" -o ${APP}_cpp_src2 v4lgrabber.cpp -lv4l2

	gcc v4l_jpeg_grab.c -o v4l2grab_jpg -Wall -ljpeg -DIO_READ -DIO_MMAP -DIO_USERPTR
	g++ video.cpp -o video -lboost_filesystem -lopencv_core -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -Wall -I/usr/include/opencv4

