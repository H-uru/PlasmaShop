add_library(PlasmaShell SHARED PlasmaShell.cpp PlasmaShell.rc)

install(TARGETS PlasmaShell
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
)
