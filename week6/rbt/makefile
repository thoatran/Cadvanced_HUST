#declare the variable
CC=gcc
FLAGS =-g -Wall
INCLUDES = -I../../lib/include
# define any directories containing header files other than /usr/include
LIBS = -L../../lib/implementation
#define any directories containing implementation of the files in INCLUDES
#../../lib means up 2 level then search for /lib
LFLAGS= -lfdr -lutility
#to run all the .a file
CFLAGS=-c $(FLAG)
SRCS=test.c graph_rb.c
OBJS= $(SRCS:.c=.o)
EXECUTABLE=test


#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings

#action
all: $(EXECUTABLE) $(SRCS) 
$(EXECUTABLE): $(OBJS)
	$(CC) $(FLAGS)  $(INCLUDES) -o $(EXECUTABLE) $(OBJS) $(LIBS) $(LFLAGS)	
.c.o: 
	$(CC) $(INCLUDES) $(CFLAGS)  $< -o $@
clean: 
	rm -f core $(OBJS)  $(EXECUTABLE)
