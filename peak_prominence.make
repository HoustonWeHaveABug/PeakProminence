PEAK_PROMINENCE_C_FLAGS=-O2 -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winline -Wlong-long -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-import -Wpointer-arith -Wredundant-decls -Wshadow -Wstrict-prototypes -Wwrite-strings

peak_prominence: peak_prominence.o
	gcc -o peak_prominence peak_prominence.o

peak_prominence.o: peak_prominence.c peak_prominence.make
	gcc -c ${PEAK_PROMINENCE_C_FLAGS} -o peak_prominence.o peak_prominence.c

clean:
	rm -f peak_prominence peak_prominence.o
