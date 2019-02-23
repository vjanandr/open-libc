# $@ The file name of the target.
# $< The name of the first dependency.
# $* The part of a filename which matched a suffix rule.
# $?  The names of all the dependencies newer than the target separated by spaces.
# $^ The names of all the dependencies separated by spaces, but with duplicate names removed.
# $+ The names of all the dependencies separated by spaces with duplicate names included and in the same order as in the rule.

cc = gcc
#CFLAGS = -g -Wuninitialized -Wreturn-type \
		 -Wunused-label -Wunused-value -Wunused-variable \
		 -Wimplicit-function-declaration -Wimplicit-int -Wmissing-braces \
		 -Wnonnull -Wparentheses
CFLAGS = -g

#COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
#COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
#LINK.o = $(CC) $(LDFLAGS) $(TARGET_ARCH)
#OUTPUT_OPTION = -o $@


INC_DIRS = -I./common/include \
		   -I./tree/include

test: treetest

treetest.o: olibc_tree.o ./tree/test/olibc_tree_test.c
	$(cc) $(CFLAGS) $(INC_DIRS) -o treetest $^ -lcunit

treetest: treetest.o
	./treetest


olibc_tree.o: ./tree/src/olibc_tree.c
	$(cc) $(CFLAGS) $(INC_DIRS) -c $^


.PHONY: clean

clean:
	rm -f *.o treetest
