CC = ./purify cc
CFLAGS = -g -Wall

SYS_CC = cc
SYS_CFLAGS = -g -Wall

AR = ar
ARFLAGS = curv
RANLIB = true

x : test11

EXES = t_illread test1 test2 test3 test4 test5 test6 test7 test8 test9 \
	test10 test11

all : $(EXES)

check : all
	@for file in $(EXES) ; do \
	    check $$file ; \
	done

% : %.c libpurify.a Makefile purify purify.gawk pbd.o ped.o
	$(CC) $(CFLAGS) $< -o $@

pbd.o : src/pbd.S
	$(SYS_CC) -c $< -o $@

ped.o : src/ped.S
	$(SYS_CC) -c $< -o $@

libpurify.a : obj/init.o obj/util.o obj/hash.o obj/error.o obj/stack.o \
	    obj/access.o obj/stubs.o obj/memory.o obj/posinfo.o \
	    obj/debug.o obj/io.o
	$(AR) $(ARFLAGS) $@ $?
	$(RANLIB) $@

obj/%.o : src/%.c
	$(SYS_CC) $(SYS_CFLAGS) -Isrc -c $< -o $@

obj/%.o : src/%.S
	$(SYS_CC) $(SYS_CFLAGS) -x assembler-with-cpp -c $< -o $@

clean :
	$(RM) obj/*.o libpurify.a t_illread test?

obj/access.o : src/debug.h src/error.h src/error.def src/hash.h
obj/debug.o : src/debug.h src/posinfo.h
obj/error.o : src/error.h src/error.def src/hash.h src/posinfo.h
obj/hash.o : src/hash.h src/util.h src/error.h src/error.def src/memory.h src/debug.h
obj/init.o : src/memory.h
obj/io.o : src/hash.h src/error.h src/error.def
obj/memory.o : src/memory.h src/hash.h src/posinfo.h src/error.h src/error.def \
		src/util.h
obj/posinfo.o : src/posinfo.h src/util.h src/error.h src/error.def
obj/stack.o : src/hash.h src/debug.h
obj/util.o : src/util.h

