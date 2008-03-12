PLSLIBDIR=Plasma/lib

CXX = g++
CXXFLAGS = -fPIC -Wall -g -IPlasma -I3rdp -I3rdp/squish `wx-config --cxxflags`
CXXLIBS = -L$(PLSLIBDIR) -lPlasmaCore -lPlasma -L3rdp/squish -lsquish `wx-config --libs std,gl`

CC = gcc
CCFLAGS = -fPIC -Wall -g -I3rdp
CCLIBS =

OBJFILES=out/wx/imagdds.o out/wx/filter.o \
	out/wxPlasmaStream.o \
	out/wxPlasmaShopFrame.o

all: $(PLSLIBDIR)/libPlasmaCore.so $(PLSLIBDIR)/libPlasma.so \
	3rdp/squish/libsquish.a \
	bin/PlasmaShop

install:
	(cd Plasma ; make install)
	cp bin/PlasmaShop /usr/local/bin/

clean:
	(cd Plasma ; make clean)
	(cd 3rdp/squish ; make clean)
	rm -f $(OBJFILES)

distclean:
	(cd Plasma ; make distclean)
	rm -f bin/PlasmaShop

# Libs
$(PLSLIBDIR)/libPlasmaCore.so: Plasma/lib/.keep
$(PLSLIBDIR)/libPlasma.so: Plasma/lib/.keep
	(cd Plasma ; make)


# 3rd Party stuff
3rdp/squish/libsquish.a:
	(cd 3rdp/squish ; make)

out/wx/imagdds.o: 3rdp/wx/imagdds.h 3rdp/wx/imagdds.cpp
	$(CXX) $(CXXFLAGS) -c 3rdp/wx/imagdds.cpp -o out/wx/imagdds.o

out/wx/filter.o: 3rdp/wx/filter.c
	$(CC) $(CCFLAGS) -c 3rdp/wx/filter.c -o out/wx/filter.o


# PlasmaShop
bin/PlasmaShop: $(OBJFILES) src/Main.cpp
	$(CXX) $(CXXFLAGS) $(OBJFILES) src/Main.cpp $(CXXLIBS) -o bin/PlasmaShop

out/wxPlasmaShopFrame.o: src/wxPlasmaShopFrame.h src/wxPlasmaShopFrame.cpp
	$(CXX) $(CXXFLAGS) -c src/wxPlasmaShopFrame.cpp -o out/wxPlasmaShopFrame.o

out/wxPlasmaStream.o: src/wxPlasmaStream.h src/wxPlasmaStream.cpp
	$(CXX) $(CXXFLAGS) -c src/wxPlasmaStream.cpp -o out/wxPlasmaStream.o
