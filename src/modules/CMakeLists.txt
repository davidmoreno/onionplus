INCLUDE_DIRECTORIES(..)

ADD_DEFINITIONS(-Wall -Werror -O2)

add_library(onion_modules proxy.cpp authpam.cpp resources.cpp allow.cpp conf.cpp directory.cpp fake.cpp include.cpp path.cpp server.cpp staticfile.cpp stats.cpp dirlist.cpp)
target_link_libraries(onion_modules pam pam_misc)
