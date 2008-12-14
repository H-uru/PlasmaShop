PLSLIBDIR=../Plasma/core/lib

CXX = g++
CXXFLAGS = -fPIC -Wall -g -I../Plasma/core -I3rdp -I3rdp/squish `wx-config --cxxflags`
CXXLIBS_PLASMASHOP = -L$(PLSLIBDIR) -lPlasma -L3rdp/wx/stc -lwxstc `wx-config --libs std`
CXXLIBS_PRPSHOP = -L$(PLSLIBDIR) -lPlasma `wx-config --libs std,gl`

CC = gcc
CCFLAGS = -fPIC -Wall -g -I3rdp
CCLIBS =

OBJFILES_DDS=out/wx/imagdds.o out/wx/filter.o \
	3rdp/squish/libsquish.a

OBJFILES_SHARED=\
	out/wxPlasmaStream.o

OBJFILES_PLASMASHOP=\
	$(OBJFILES_SHARED) \
	out/wxNewFileFrame.o \
	out/wxPlasmaShopFrame.o \
	out/wxPlasmaTextCtrl.o

OBJFILES_PRPSHOP=\
	$(OBJFILES_SHARED) \
	out/wxPrpShopFrame.o \
	out/wxPrpCanvas.o \
	out/PlasmaTreeItem.o \
	out/wxPrpPlasma.o \
	out/wxObjSelectFrame.o \
	out/PRP/wxPrpGUI.o \
	out/PRP/wxCoordinateInterface.o \
	out/PRP/wxGMaterial.o \
	out/PRP/wxMipmap.o \
	out/PRP/wxSceneNode.o \
	out/PRP/wxSceneObject.o \
	out/PRP/wxSynchedObject.o

all: $(PLSLIBDIR)/libPlasma.so \
	3rdp/squish/libsquish.a 3rdp/wx/stc/libwxstc.a \
	bin/PlasmaShop bin/PrpShop

install:
	(cd ../Plasma/core ; make install)

clean-all:
	(cd 3rdp/squish ; make clean)
	(cd 3rdp/wx/stc ; make clean)
	rm -f $(OBJFILES_DDS)
	rm -f $(OBJFILES_PLASMASHOP)
	rm -f $(OBJFILES_PRPSHOP)
	rm -f bin/PlasmaShop
	rm -f bin/PrpShop

clean:
	rm -f $(OBJFILES_PLASMASHOP)
	rm -f $(OBJFILES_PRPSHOP)
	rm -f bin/PlasmaShop
	rm -f bin/PrpShop


# Libs
$(PLSLIBDIR)/libPlasma.so:
	(cd ../Plasma/core ; make)


# 3rd Party stuff
3rdp/squish/libsquish.a:
	(cd 3rdp/squish ; make)

3rdp/wx/stc/libwxstc.a:
	(cd 3rdp/wx/stc ; make)

out/wx/imagdds.o: 3rdp/wx/imagdds.h 3rdp/wx/imagdds.cpp
	$(CXX) $(CXXFLAGS) -c 3rdp/wx/imagdds.cpp -o out/wx/imagdds.o

out/wx/filter.o: 3rdp/wx/filter.c
	$(CC) $(CCFLAGS) -c 3rdp/wx/filter.c -o out/wx/filter.o


# Shared
out/wxPlasmaStream.o: src/wxPlasmaStream.h src/wxPlasmaStream.cpp
	$(CXX) $(CXXFLAGS) -c src/wxPlasmaStream.cpp -o $@

out/wxPlasmaTextCtrl.o: src/wxPlasmaTextCtrl.h src/wxPlasmaTextCtrl.cpp
	$(CXX) $(CXXFLAGS) -c src/wxPlasmaTextCtrl.cpp -o $@


# PlasmaShop
bin/PlasmaShop: $(OBJFILES_PLASMASHOP) src/PlasmaShop/Main.cpp
	$(CXX) $(CXXFLAGS) $(OBJFILES_PLASMASHOP) src/PlasmaShop/Main.cpp $(CXXLIBS_PLASMASHOP) -o $@

out/wxNewFileFrame.o: src/PlasmaShop/wxNewFileFrame.h src/PlasmaShop/wxNewFileFrame.cpp
	$(CXX) $(CXXFLAGS) -c src/PlasmaShop/wxNewFileFrame.cpp -o $@

out/wxPlasmaShopFrame.o: src/PlasmaShop/wxPlasmaShopFrame.h src/PlasmaShop/wxPlasmaShopFrame.cpp
	$(CXX) $(CXXFLAGS) -c src/PlasmaShop/wxPlasmaShopFrame.cpp -o $@


# PrpShop
bin/PrpShop: $(OBJFILES_PRPSHOP) $(OBJFILES_DDS) src/PrpShop/Main.cpp
	$(CXX) $(CXXFLAGS) $(OBJFILES_PRPSHOP) $(OBJFILES_DDS) src/PrpShop/Main.cpp $(CXXLIBS_PRPSHOP) -o $@

out/wxPrpCanvas.o: src/PrpShop/wxPrpCanvas.h src/PrpShop/wxPrpCanvas.cpp
	$(CXX) $(CXXFLAGS) -c src/PrpShop/wxPrpCanvas.cpp -o out/wxPrpCanvas.o

out/wxPrpShopFrame.o: src/PrpShop/wxPrpShopFrame.h src/PrpShop/wxPrpShopFrame.cpp
	$(CXX) $(CXXFLAGS) -c src/PrpShop/wxPrpShopFrame.cpp -o $@

out/PlasmaTreeItem.o: src/PrpShop/PlasmaTreeItem.h src/PrpShop/PlasmaTreeItem.cpp
	$(CXX) $(CXXFLAGS) -c src/PrpShop/PlasmaTreeItem.cpp -o $@

out/wxPrpPlasma.o: src/PrpShop/wxPrpPlasma.h src/PrpShop/wxPrpPlasma.cpp
	$(CXX) $(CXXFLAGS) -c src/PrpShop/wxPrpPlasma.cpp -o $@

out/wxObjSelectFrame.o: src/PrpShop/wxObjSelectFrame.h src/PrpShop/wxObjSelectFrame.cpp
	$(CXX) $(CXXFLAGS) -c src/PrpShop/wxObjSelectFrame.cpp -o $@

out/PRP/wxPrpGUI.o: src/PrpShop/PRP/wxPrpGUI.h src/PrpShop/PRP/wxPrpGUI.cpp
	$(CXX) $(CXXFLAGS) -c src/PrpShop/PRP/wxPrpGUI.cpp -o $@

out/PRP/wxCoordinateInterface.o: src/PrpShop/PRP/Object/wxCoordinateInterface.h src/PrpShop/PRP/Object/wxCoordinateInterface.cpp
	$(CXX) $(CXXFLAGS) -c src/PrpShop/PRP/Object/wxCoordinateInterface.cpp -o $@

out/PRP/wxGMaterial.o: src/PrpShop/PRP/Surface/wxGMaterial.h src/PrpShop/PRP/Surface/wxGMaterial.cpp
	$(CXX) $(CXXFLAGS) -c src/PrpShop/PRP/Surface/wxGMaterial.cpp -o $@

out/PRP/wxMipmap.o: src/PrpShop/PRP/Surface/wxMipmap.h src/PrpShop/PRP/Surface/wxMipmap.cpp
	$(CXX) $(CXXFLAGS) -c src/PrpShop/PRP/Surface/wxMipmap.cpp -o $@

out/PRP/wxSceneNode.o: src/PrpShop/PRP/wxSceneNode.h src/PrpShop/PRP/wxSceneNode.cpp
	$(CXX) $(CXXFLAGS) -c src/PrpShop/PRP/wxSceneNode.cpp -o $@

out/PRP/wxSceneObject.o: src/PrpShop/PRP/Object/wxSceneObject.h src/PrpShop/PRP/Object/wxSceneObject.cpp
	$(CXX) $(CXXFLAGS) -c src/PrpShop/PRP/Object/wxSceneObject.cpp -o $@

out/PRP/wxSynchedObject.o: src/PrpShop/PRP/Object/wxSynchedObject.h src/PrpShop/PRP/Object/wxSynchedObject.cpp
	$(CXX) $(CXXFLAGS) -c src/PrpShop/PRP/Object/wxSynchedObject.cpp -o $@
