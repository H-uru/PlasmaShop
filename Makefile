all:
	( cd src/PrpShop ; qmake && make )
	( cd src/VaultShop ; qmake && make )

clean:
	( cd src/PrpShop ; make clean )
	( cd src/VaultShop ; make clean )
