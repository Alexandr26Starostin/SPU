CC=g++
FLAGS=-Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wmissing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -D_DEBUG -D_EJUDGE_CLIENT_SIDE -D_EJC
DEFINES=-D NOT_DEBUG_ASSERT -D CANARY_STK_NOT -D CANARY_STK_DATA_NOT -D HASH_STK_NOT -D MEMSET_NOT
FILES=main.o find_file.o translation.o hash.o stk_error.o operations_with_stack.o

TARGET = assembler
# pathsub
# addprefix
# wildcard

PATCHED_FILES = $(addprefix build/, $(FILES))

all: prepare $(TARGET)

prepare:
	mkdir -p build

$(TARGET): $(FILES)
	@echo "Linling..."
	@$(CC) $(FLAGS) $(PATCHED_FILES)  -o $(TARGET)

main.o: src/main.cpp
	@$(CC) -c $(FLAGS) $(DEFINES) src/main.cpp  -o build/main.o

find_file.o: src/find_file.cpp
	@$(CC) -c $(FLAGS) $(DEFINES) src/find_file.cpp  -o build/find_file.o

translation.o: src/translation.cpp
	@$(CC) -c $(FLAGS) $(DEFINES) src/translation.cpp  -o build/translation.o


hash.o: ../stack/src/hash.cpp
	@$(CC) -c $(FLAGS) $(DEFINES) ../stack/src/hash.cpp -o build/hash.o

stk_error.o: ../stack/src/stk_error.cpp
	@$(CC) -c $(FLAGS) $(DEFINES) ../stack/src/stk_error.cpp -o build/stk_error.o

operations_with_stack.o: ../stack/src/operations_with_stack.cpp
	@$(CC) -c $(FLAGS) $(DEFINES) ../stack/src/operations_with_stack.cpp -o build/operations_with_stack.o

clean:
	rm -rf $(TARGET)
	cd build && rm -rf *.o