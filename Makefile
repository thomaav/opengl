CC=clang++
CFLAGS=-Wall -std=c++11
LDLIBS=-lglfw -lGL -lX11 -lpthread -lXrandr -lXi

OBJ_DIR=objects
OBJS=$(patsubst %.cpp, $(OBJ_DIR)/%.o, $(wildcard *.cpp))

main.bin: $(OBJS)
	${CC} ${CFLAGS} $^ ${LDLIBS} -o $@

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	${CC} ${CFLAGS} $< -c -o $@

.PHONY: clean
clean:
	-rm -rf $(OBJ_DIR)
	-rm -rf *.bin *.o

.PHONY: run
run:
	-make && ./main.bin