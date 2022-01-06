TARGET     = truthtable
CC         = gcc
SANITIZERS = -fsanitize=address $(if $(findstring clang,$(CC)),-fsanitize=undefined)
OPT        =
CFLAGS     = -g -std=c99 -Wall -Wvla -Werror $(SANITIZERS) $(OPT)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $^ -o $@ -lm

clean:
	rm -rf $(TARGET) *.o *.a *.dylib *.dSYM