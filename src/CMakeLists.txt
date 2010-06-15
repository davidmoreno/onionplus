SET(ONION_MOC_HDRS
	client.h daemon.h )
SET( QT_USE_QTNETWORK TRUE )
SET( QT_DONT_USE_QTGUI TRUE )

FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE( ${QT_USE_FILE} )

add_subdirectory(modules)
add_subdirectory(moremodules)

ADD_DEFINITIONS(-Wall -Werror -O2)

QT4_ADD_RESOURCES( ONION_RESOURCES resources/onion.qrc )

QT4_WRAP_CPP( ONION_MOC_SRCS ${ONION_MOC_HDRS} )

add_executable (onion mime.cpp onion.cpp client.cpp configparser.cpp daemon.cpp module.cpp 
								modulefactory.cpp modulelist.cpp request.cpp response.cpp ${ONION_MOC_SRCS} ${ONION_RESOURCES} )

TARGET_LINK_LIBRARIES( onion ${QT_LIBRARIES} onion_modules)
