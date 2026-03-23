add_executable(wav_info wav_info.cpp)
target_link_libraries(wav_info PRIVATE audio dsp fingerprint)
target_include_directories(wav_info PRIVATE 
    ${CMAKE_SOURCE_DIR}/core/audio
    ${CMAKE_SOURCE_DIR}/core/dsp
    ${CMAKE_SOURCE_DIR}/core/fingerprint
)

add_executable(indexer indexer.cpp)
target_link_libraries(indexer PRIVATE audio dsp fingerprint)
target_include_directories(indexer PRIVATE 
    ${CMAKE_SOURCE_DIR}/core/audio
    ${CMAKE_SOURCE_DIR}/core/dsp
    ${CMAKE_SOURCE_DIR}/core/fingerprint
)

add_executable(query query.cpp)
target_link_libraries(query PRIVATE audio dsp fingerprint)
target_include_directories(query PRIVATE 
    ${CMAKE_SOURCE_DIR}/core/audio
    ${CMAKE_SOURCE_DIR}/core/dsp
    ${CMAKE_SOURCE_DIR}/core/fingerprint
)