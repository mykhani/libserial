
LIBS = libserial.so libserial.a
TESTS = serial_test

all: ${LIBS}

libserial.so: serial.h serial.c
	gcc -Wall -shared -fPIC serial.c -o libserial.so
libserial.a: serial.h serial.c
	gcc -c serial.c
	ar rcs libserial.a serial.o 
clean:
	rm -rf *o ${TESTS} ${LIBS}

serial_test: serial_test.c
	gcc -static -o serial_test serial_test.c -lserial -L${PWD}

tests: ${TESTS}
