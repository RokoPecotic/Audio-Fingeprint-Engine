add_library(dsp INTERFACE)
target_include_directories(dsp INTERFACE ${CMAKE_SOURCE_DIR}/core/dsp)
target_link_libraries(dsp INTERFACE fftw3f)