add_library(fingerprint
    hasher.cpp
)

target_include_directories(fingerprint PUBLIC 
    ${CMAKE_SOURCE_DIR}/core/fingerprint
    ${CMAKE_SOURCE_DIR}/core/dsp
)

target_link_libraries(fingerprint PRIVATE dsp)