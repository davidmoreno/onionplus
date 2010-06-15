INCLUDE_DIRECTORIES(..)

ADD_DEFINITIONS(-Wall -Werror -O2)

add_library(jsondirlist SHARED jsondirlist.cpp)
#add_library(dirlist SHARED dirlist/dirlist.cpp)

