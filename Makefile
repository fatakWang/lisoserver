SRC_DIR := src
OBJ_DIR := obj
# all src files
SRC := $(wildcard $(SRC_DIR)/*.c)
# all objects
OBJ := $(OBJ_DIR)/y.tab.o $(OBJ_DIR)/lex.yy.o $(OBJ_DIR)/parse.o $(OBJ_DIR)/example.o $(OBJ_DIR)/haddle_send.o $(OBJ_DIR)/socket_haddle.o $(OBJ_DIR)/log.o
# all binaries
BIN := example liso_server liso_client
# C compiler
CC  := gcc
# C PreProcessor Flag
CPPFLAGS := -Iinclude
# compiler flags
CFLAGS   := -g -Wall
# DEPS = parse.h y.tab.h
TAR :=tar

default: all
all : example liso_server liso_client

example: $(OBJ)
	$(CC) $^ -o $@

$(SRC_DIR)/lex.yy.c: $(SRC_DIR)/lexer.l
	flex -o $@ $^

$(SRC_DIR)/y.tab.c: $(SRC_DIR)/parser.y
	yacc -d $^
	mv y.tab.c $@
	mv y.tab.h $(SRC_DIR)/y.tab.h

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

liso_server: $(OBJ_DIR)/liso_server.o $(OBJ_DIR)/y.tab.o $(OBJ_DIR)/lex.yy.o $(OBJ_DIR)/parse.o $(OBJ_DIR)/haddle_send.o $(OBJ_DIR)/socket_haddle.o $(OBJ_DIR)/log.o
	$(CC) -Werror $^ -o $@

liso_client: $(OBJ_DIR)/liso_client.o
	$(CC) -Werror $^ -o $@

$(OBJ_DIR):
	mkdir $@

clean:
	$(RM) $(OBJ) $(BIN) $(SRC_DIR)/lex.yy.c $(SRC_DIR)/y.tab.*
	$(RM) -r $(OBJ_DIR)

tar:
	$(TAR) -cf webServer.tar src include static_site Makefile 
