CC = gcc
INCLUDES = src/*.h
COMMON_DEPS = $(INCLUDES) makefile opt.conf

TEST = build/test.o \
	build/list.o

all: dir test

test: $(TEST)
	$(CC) -o bin/$@ $^

build/%.o:	src/%.c $(COMMON_DEPS)
	$(CC) -c $< -o $@

dir:
	mkdir -p bin build
