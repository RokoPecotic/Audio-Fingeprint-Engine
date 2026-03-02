#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <fstream>

struct WavHeader {
    char riff[4];
    uint32_t file_size;
    char wave[4];
    char fmt[4];
    uint32_t fmt_size;
    uint16_t audio_format;
    uint16_t num_of_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    char data[4];
    uint32_t audio_data_size;
};

struct WavFile {
    WavHeader header;
    std::vector<float> samples;
};

WavFile load_wav(const std::string& path);
