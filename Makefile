CPP := g++
CFLAGS := -O2 -Wall -Wextra -fPIC

LUAFLAGS := -I/usr/include/luajit-2.1 -L/usr/local/lib -lluajit-5.1
LUA52FLAGS := -I/usr/include/lua5.2 -L/usr/lib/x86_64-linux-gnu/ -llua5.2

INCLUDEDIR := include
SRCDIR := src
OBJDIR := obj
BINDIR := bin

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

.PHONY: all clean test lua lua52
all: test lua

test: $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CPP) $(CFLAGS) -I$(INCLUDEDIR) $^ test/test.c -o $(BINDIR)/$@ 
	./$(BINDIR)/$@		

lua: $(OBJECTS) 
	@mkdir -p $(OBJDIR)
	$(CPP) $(CFLAGS) $(LUAFLAGS) -I$(INCLUDEDIR) lua/salvager.c $(OBJECTS) -shared -o $(OBJDIR)/salvager.so

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CPP) $(CFLAGS)  -I$(INCLUDEDIR) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)

