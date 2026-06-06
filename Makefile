SRC := ./src/*
INC := ./include/
DST := ./bin/a.out
CONF := config.h

default: all run

all:
	gcc $(SRC) -o $(DST) -I$(INC) -I. -fsanitize=address -std=c99 -lm

debug:
	gcc $(SRC) -o $(DST) -I$(INC) -I. -fsanitize=address -std=c99 -g -lm
	gdb $(DST)

run:
	@$(DST)

clean:
	@rm test/* bin/*
