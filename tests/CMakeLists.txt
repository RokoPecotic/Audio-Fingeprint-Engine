add_executable(test_wav_parser test_wav_parser.cpp)
target_link_libraries(test_wav_parser PRIVATE audio dsp fingerprint GTest::GTest GTest::Main)
target_include_directories(test_wav_parser PRIVATE
    ${CMAKE_SOURCE_DIR}/core/audio
    ${CMAKE_SOURCE_DIR}/core/dsp
    ${CMAKE_SOURCE_DIR}/core/fingerprint
)
target_compile_definitions(test_wav_parser PRIVATE 
    TEST_DATA_DIR="${CMAKE_SOURCE_DIR}/test_audio"
)