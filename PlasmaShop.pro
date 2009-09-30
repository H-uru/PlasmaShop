TEMPLATE = subdirs
SUBDIRS = QScintilla/Qt4/qscintilla.pro \
          src/PlasmaShop \
          src/PrpShop \
          src/VaultShop

win32:SUBDIRS += icons/win32/PlasmaShell.pro
