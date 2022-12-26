SRCDIR			= src
OUTDIR			= out
OBJDIR			= $(OUTDIR)/obj
SRCS			= $(wildcard $(SRCDIR)/*.cpp)
OBJS			= $(subst $(SRCDIR), $(OBJDIR), $(SRCS:.cpp=.o))
TARGET			= $(OUTDIR)/main
CC				= g++
CFLAGS			= -std=c++17 -Wall -O2
CFLAGS_DEBUG	= -std=c++17 -Wall -O0 -g

main: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

debug: $(OBJS)
	$(CC) $(CFLAGS_DEBUG) -o $(TARGET) $^

TESTDIR 		= test
TEST_SRCDIR		= $(TESTDIR)/src
TEST_OUTDIR		= $(TESTDIR)/out
TEST_SRCS		= $(wildcard $(TEST_SRCDIR)/*.cpp)
TEST_TARGETS	= $(subst $(TEST_SRCDIR), $(TEST_OUTDIR), $(TEST_SRCS:.cpp=))
TEST_CFLAGS		= -std=c++17 -Wall -pthread -lgtest_main -lgtest

test: $(TEST_TARGETS)

$(TEST_OUTDIR)/card_test:	$(TEST_SRCDIR)/card_test.cpp $(OBJDIR)/move.o $(OBJDIR)/card.o
	$(CC) $(TEST_CFLAGS) -o $@ $^

$(TEST_OUTDIR)/submission_test:	$(TEST_SRCDIR)/submission_test.cpp $(OBJDIR)/move.o $(OBJDIR)/card.o
	$(CC) $(TEST_CFLAGS) -o $@ $^


$(TEST_OUTDIR)/uno_state_test:	$(TEST_SRCDIR)/uno_state_test.cpp $(OBJDIR)/uno_state.o $(OBJDIR)/move.o $(OBJDIR)/card.o
	$(CC) $(TEST_CFLAGS) -o $@ $^

$(TEST_OUTDIR)/uno_state_bind2_test:

clean:
	rm -f ./out/obj/**/*.o ./out/obj/*.o ./out/main ./test/out/card_test ./test/out/submission_test ./test/out/uno_state_test
