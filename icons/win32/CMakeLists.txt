add_library(PlasmaShell SHARED PlasmaShell.cpp PlasmaShell.rc)

install(TARGETS PlasmaShell
        DESTINATION .
)
