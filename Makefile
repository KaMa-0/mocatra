SRC := ./src/*
DST := ./bin/a.out
INC := ./include/

default: all run

all:
	gcc $(SRC) -o $(DST) -I$(INC) -fsanitize=address -std=c99 -lm

debug:
	gcc $(SRC) -o $(DST) -I$(INC) -fsanitize=address -std=c99 -g -lm
	gdb $(DST)

run:
	@$(DST)

clean:
	@rm test/* bin/*
