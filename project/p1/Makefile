CC = clang
CFLAGS = -std=gnu11 -O2 -Wall -Wextra -pedantic -Wconversion
MUMSH_SRC = *.c
MUMSH = mumsh 
MUMSHMC_FLAGS = -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=integer
MUMSHMC = mumsh_memory_check
.PHONY: clean

all: $(MUMSH) $(MUMSHMC)
	@echo mumsh successfully constructed

$(MUMSH): $(MUMSH_SRC) 
	$(CC) $(CFLAGS) -o $(MUMSH) $(MUMSH_SRC)

$(MUMSHMC) : $(MUMSH_SRC) 
	$(CC) $(CFLAGS) $(MUMSHMC_FLAGS) -o $(MUMSHMC) $(MUMSH_SRC)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ 

clean: 
	$(RM) *.o *.a *~ $(MUMSH) $(MUMSHMC)
