cc = gcc
CFLAGS = -g -Wuninitialized -Wreturn-type \
		 -Wunused-label -Wunused-value -Wunused-variable \
		 -Wimplicit-function-declaration -Wimplicit-int -Wmissing-braces \
		 -Wnonnull -Wparentheses

INC_DIRS = -I./common/include \
		   -I./tree/include

libtree.o: ./tree/src/olibc_tree.c
	$(cc) $(CFLAGS) $(INC_DIRS) -c ./tree/src/olibc_tree.c

