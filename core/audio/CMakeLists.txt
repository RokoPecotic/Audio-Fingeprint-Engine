add_library(audio
    wav_parser.cpp
)

target_include_directories(audio PUBLIC ${CMAKE_SOURCE_DIR})

