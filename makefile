CC = gcc
INCLUDES = src/*.h
COMMON_DEPS = $(INCLUDES) makefile opt.conf

TEST = build/test.o \
	build/list.o \
	build/random.o \
	build/conf.o \
	build/student.o \
	build/group.o \
	build/shm.o \
	build/sem.o \
	build/sort.o

MANAGER = build/manager.o \
	build/list.o \
	build/random.o \
	build/conf.o \
	build/student.o \
	build/group.o \
	build/msg.o \
	build/sem.o \
	build/shm.o

STUDENT = build/child.o \
	build/list.o \
	build/random.o \
	build/conf.o \
	build/student.o \
	build/group.o \
	build/msg.o \
	build/sem.o \
	build/shm.o

all: dir test manager

manager: $(MANAGER)
	$(CC) -o bin/$@ $^

student: $(STUDENT)
	$(CC) -o bin/$@ $^

test: $(TEST)
	$(CC) -o bin/$@ $^

build/%.o:	src/%.c $(COMMON_DEPS)
	$(CC) -c $< -o $@

dir:
	mkdir -p bin build

clean:
	rm -rf bin build
