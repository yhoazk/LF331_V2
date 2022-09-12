APP:=dump_vdso
VDSO:=vdso.dump
# https://berthub.eu/articles/posts/on-linux-vdso-and-clockgettime/
# https://stackoverflow.com/questions/42622427/gettimeofday-not-using-vdso
# https://github.com/cylindroid/bits-and-pieces
all: clean comp show

clean:
	rm -f ${APP}
	rm -f ${VDSO}

comp:
	gcc -Wall -o ${APP} ${APP}.c

show:
	./${APP} > ${VDSO}
	objdump -T ${VDSO}

