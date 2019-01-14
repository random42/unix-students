CC = gcc
INCLUDES = src/*.h
COMMON_DEPS = $(INCLUDES) makefile opt.conf

TEST = build/test.o \
	build/list.o \
	build/random.o

all: dir test

test: $(TEST)
	$(CC) -o bin/$@ $^

build/%.o:	src/%.c $(COMMON_DEPS)
	$(CC) -c $< -o $@

dir:
	mkdir -p bin build

clean:
	rm -rf bin build
