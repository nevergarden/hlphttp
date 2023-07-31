cmake_minimum_required(VERSION 3.0)

project(hlpicohttp)

function(set_as_hdll target)
    set_target_properties(${target}.hdll
        PROPERTIES
        PREFIX ""
        OUTPUT_NAME ${target}
        SUFFIX .hdll
    )
endfunction()

add_library(picohttp.hdll SHARED
	picohttpparser/picohttpparser.c
hlpico.c)
set_as_hdll(picohttp)

target_link_libraries(picohttp.hdll hl)
