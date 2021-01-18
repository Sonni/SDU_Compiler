ifeq ($(detected_OS), Darwin)  # Mac OS X
    LIBS = -ll
endif
ifeq ($(detected_OS), Linux)
    LIBS = -lfl
endif

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic
TARGET = ../../compiler


SRC = lex.pp.c lex.yy.c y.tab.c ../main.c ../frontend/parser/*.c ../utils/*.c
SRC += ../backend/optimization/inliner/*.c
SRC += ../frontend/typecheck/*.c ../frontend/symbol/*.c ../backend/asm/*.c
SRC += ../backend/reg_alloc/*.c ../backend/optimization/*.c
SRC += ../frontend/weeder/*.c ../utils/memory/*.c ../frontend/template/*.c

HEADER = ../frontend/parser/*.h ../utils/*.h ../frontend/typecheck/*.h
HEADER += ../frontend/symbol/*.h ../backend/optimization/inliner/*.h
HEADER += ../backend/asm/*.h ../backend/reg_alloc/*.h
HEADER += ../backend/optimization/*.h
HEADER += ../frontend/weeder/*.h ../utils/memory/*.h ../frontend/template/*.h

OBJ =  $(patsubst %.c, %.o, $(SRC))

PARSER_PATH = ../frontend/parser/
FLEX = $(PARSER_PATH)shere_khan.l
PP = $(PARSER_PATH)preprocessor.l
BISON = $(PARSER_PATH)shere_khan.y

.PHONY: clean

$(TARGET): $(SRC) $(HEADER)
	$(CC) $(CFLAGS) $(SRC) -o $@ $(LIBS)

y.tab.c y.tab.h: $(BISON)
	bison -y -d  $^

lex.yy.c: $(FLEX) y.tab.h
	flex $(FLEX)

lex.pp.c: $(PP)
	flex $(PP)

clean:
	rm -f lex.yy.c lex.pp.c
	rm -f y.tab.c y.tab.h
	rm -f $(OBJ)
