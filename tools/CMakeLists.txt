add_executable(wav_info wav_info.cpp)
target_link_libraries(wav_info PRIVATE audio)
target_include_directories(wav_info PRIVATE ${CMAKE_SOURCE_DIR}/core/audio)