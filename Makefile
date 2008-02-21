PLSLIBDIR=Plasma/lib

CXX = g++
CXXFLAGS = -fPIC -Wall -g -IPlasma -I3rdp -I3rdp/squish `wx-config --cxxflags`
CXXLIBS = -L$(PLSLIBDIR) -lPlasmaCore -lPlasma -L3rdp/squish -lsquish `wx-config --libs std,gl`

OBJFILES=out/wxPlasmaShopFrame.o \
	out/wxDDSHandler.o

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
$(PLSLIBDIR)/libPlasmaCore.so:
$(PLSLIBDIR)/libPlasma.so:
	(cd Plasma ; make)


# 3rd Party stuff
3rdp/squish/libsquish.a:
	(cd 3rdp/squish ; make)

out/wxDDSHandler.o: 3rdp/wx/imagdds.h 3rdp/wx/imagdds.cpp
	$(CXX) $(CXXFLAGS) 3rdp/wx/imagdds.cpp -o out/wxDDSHandler.o


# PlasmaShop
bin/PlasmaShop: $(OBJFILES) src/wxMain.cpp
	$(CXX) $(CXXFLAGS) $(OBJFILES) src/wxMain.cpp $(CXXLIBS) -o bin/PlasmaShop

out/wxPlasmaShopFrame.o: src/wxPlasmaShopFrame.h src/wxPlasmaShopFrame.cpp
	$(CXX) $(CXXFLAGS) src/wxPlasmaShopFrame.cpp -o out/wxPlasmaShopFrame.o
