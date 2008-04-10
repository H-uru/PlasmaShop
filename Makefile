PLSLIBDIR=Plasma/lib

CXX = g++
CXXFLAGS = -fPIC -Wall -g -IPlasma -I3rdp -I3rdp/squish `wx-config --cxxflags`
CXXLIBS = -L$(PLSLIBDIR) -lPlasmaCore -lPlasma -L3rdp/squish -lsquish -L3rdp/wx/stc -lwxstc `wx-config --libs std,gl`

CC = gcc
CCFLAGS = -fPIC -Wall -g -I3rdp
CCLIBS =

OBJFILES=out/wx/imagdds.o out/wx/filter.o \
	out/wxPlasmaShopFrame.o \
	out/wxPlasmaStream.o \
	out/wxPlasmaTextCtrl.o

all: $(PLSLIBDIR)/libPlasmaCore.so $(PLSLIBDIR)/libPlasma.so \
	3rdp/squish/libsquish.a 3rdp/wx/stc/libwxstc.a \
	bin/PlasmaShop

install:
	(cd Plasma ; make install)
	cp bin/PlasmaShop /usr/local/bin/

clean:
	(cd 3rdp/squish ; make clean)
	(cd 3rdp/wx/stc ; make clean)
	rm -f $(OBJFILES)
	rm -f bin/PlasmaShop

# Libs
$(PLSLIBDIR)/libPlasmaCore.so:
$(PLSLIBDIR)/libPlasma.so:
	(cd Plasma ; make)


# 3rd Party stuff
3rdp/squish/libsquish.a:
	(cd 3rdp/squish ; make)

3rdp/wx/stc/libwxstc.a:
	(cd 3rdp/wx/stc ; make)

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

out/wxPlasmaTextCtrl.o: src/wxPlasmaTextCtrl.h src/wxPlasmaTextCtrl.cpp
	$(CXX) $(CXXFLAGS) -c src/wxPlasmaTextCtrl.cpp -o out/wxPlasmaTextCtrl.o
