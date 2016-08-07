CC=cc

vm: main.c
	$(CC) -g -Wall -Werror main.c -o vm
test: generateTestBytecode.c vm
	$(CC) -Wall -Werror generateTestBytecode.c -o gentest
	./gentest
	./vm test.bc
clean: 
	rm -f gentest vm test.bc
