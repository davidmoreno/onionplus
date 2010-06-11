INCLUDE_DIRECTORIES(..)

QT4_ADD_RESOURCES( SIMPLEDIRLIST simpledirlist.res )


add_library(jsondirlist SHARED jsondirlist.cpp)
add_library(simpledirlist SHARED simpledirlist.cpp ${SIMPLEDIRLIST} )

