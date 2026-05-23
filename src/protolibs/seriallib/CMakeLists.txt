add_library(SERIALLIB)

set(SERIALLIB_SRC
    serialport.cpp
)

target_include_directories(SERIALLIB
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(SERIALLIB
    PRIVATE
        ${SERIALLIB_SRC}
)