
BINDIR = bin
OBJDIR = obj
DIRS = $(BINDIR) $(OBJDIR)
CXX = g++
MOC = moc
EXECNAME = sage
#LIBS = -L/usr/local/qt/lib -lqt
INCLUDES = -I/usr/local/qt/include -I..
EXECPATH = $(BINDIR)/$(EXECNAME)
CFLAGS = -g -Wall
LDFLAGS = $(FLAGS)


SOURCES = \
	Board.cpp \
	BoardUtil.cpp \
	Main.cpp \
	Engine.cpp \
	HumanPolicy.cpp \

OBJECTS = $(addprefix $(OBJDIR)/,$(subst .cpp,.o,$(SOURCES)))

all:
	(cd ../general; $(MAKE))
	$(MAKE) $(DIRS)
	$(MAKE) $(EXECPATH)


$(EXECPATH): $(OBJECTS) $(MOCS)
	$(CXX) $(OBJECTS) $(MOCS) -o $@ $(LIBS) $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp
	$(CXX) -c $< -o $@ $(INCLUDES) $(CFLAGS)

%_moc.cpp: %.h
	$(MOC) $< -o $@

$(DIRS):
	mkdir $@

clean:
	$(RM) $(OBJECTS) $(EXECPATH) $(MOCS)
