##
## Makefile for compiling and installing people counter on PC
## Silvano Seva
##

## Directory in wich copy executables and "html" folder
INSTALLDIR := 

## Executable name, if you want to change it
EXECNAME := contapersone

## Don't touch anything below this line, except for
## the CXX directive in the case you have to call g++
## with a different command
SRC := main.cpp seriale.cpp
CXX := g++
CXXFLAGS := -std=c++11

all: $(EXECNAME)

$(EXECNAME):
	$(CXX) $(SRC) $(CXXFLAGS) -o $(EXECNAME)

install: $(EXECNAME)
	mkdir $(INSTALLDIR)
	cp $(EXECNAME) $(INSTALLDIR)
	cp -r html $(INSTALLDIR)

clean:
	-rm $(EXECNAME)
