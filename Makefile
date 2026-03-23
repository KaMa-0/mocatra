SRC := ./src/*
DST := ./bin/a.out
INC := ./include/

default: all run

all:
	gcc $(SRC) -o $(DST) -I$(INC) -fsanitize=address -std=c99

debug:
	gcc $(SRC) -o $(DST) -I$(INC) -fsanitize=address -std=c99 -g
	gdb $(DST)

run:
	@$(DST)

clean:
	@rm test/* bin/*
