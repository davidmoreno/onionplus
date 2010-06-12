INCLUDE_DIRECTORIES(..)

QT4_ADD_RESOURCES( DIRLIST dirlist/dirlist.qrc )


add_library(jsondirlist SHARED jsondirlist.cpp)
add_library(dirlist SHARED dirlist/dirlist.cpp ${DIRLIST} )

