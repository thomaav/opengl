CC=clang++
CFLAGS=-Wall -std=c++11 -g
LDLIBS=-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

OBJ_DIR=objects
OBJS=$(patsubst %.cpp, $(OBJ_DIR)/%.o, $(wildcard *.cpp))
OBJS+=$(patsubst %.cpp, $(OBJ_DIR)/%.o, $(wildcard glad/*.cpp))

opengl: $(OBJS)
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
	+$(MAKE) $(MAKEFLAGS) && ./opengl
