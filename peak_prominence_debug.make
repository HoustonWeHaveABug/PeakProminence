PEAK_PROMINENCE_DEBUG_C_FLAGS=-g -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winline -Wlong-long -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-import -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings

peak_prominence_debug: peak_prominence_debug.o
	gcc -g -o peak_prominence_debug peak_prominence_debug.o

peak_prominence_debug.o: peak_prominence.c peak_prominence_debug.make
	gcc -c ${PEAK_PROMINENCE_DEBUG_C_FLAGS} -o peak_prominence_debug.o peak_prominence.c

clean:
	rm -f peak_prominence_debug peak_prominence_debug.o
