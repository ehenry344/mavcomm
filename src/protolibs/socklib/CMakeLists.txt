add_library(SOCKLIB)

set(SOCKLIB_SRC
    udpsock.cpp
)

if(WIN32)
    target_link_libraries(SOCKLIB
        PRIVATE
            ws2_32
    )
endif()

target_include_directories(SOCKLIB
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(SOCKLIB
    PRIVATE
        ${SOCKLIB_SRC}
)