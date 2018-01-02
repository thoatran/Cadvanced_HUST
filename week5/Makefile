# Declaration of variables
CC = gcc
CC_FLAGS = -Wall
 
# File names
EXEC = phonebook
SOURCES = $(wildcard *.c)

OBJECTS = $(SOURCES:.c=.o)
LIBS = -Ibt/inc -Lbt/lib -lbt -D_FILE_OFFSET_BITS=64

# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LIBS)
 
# To obtain object files
%.o: %.c
	$(CC) -c $(CC_FLAGS) $< -o $@ $(LIBS)

 
# To remove generated files
clean:
	rm -f $(EXEC) *.o
