#include <fstream>
#include <stdexcept>
#include <string_view>
#include "wav_parser.hpp"

WavFile load_wav(const std::string& path) {
    WavFile wav_file;
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Could not open file " + path);

    char riff[4]; file.read(riff, 4);
    uint32_t file_size; file.read(reinterpret_cast<char*>(&file_size), 4);
    char wave[4]; file.read(wave, 4);

    if (std::string_view(riff, 4) != "RIFF" ||
        std::string_view(wave, 4) != "WAVE")
        throw std::runtime_error("Invalid WAV file: " + path);

    bool found_fmt = false, found_data = false;
    while (!found_fmt || !found_data) {
        char chunk_id[4];
        uint32_t chunk_size;
        if (!file.read(chunk_id, 4)) break;
        file.read(reinterpret_cast<char*>(&chunk_size), 4);

        if (std::string_view(chunk_id, 4) == "fmt ") {
            file.read(reinterpret_cast<char*>(&wav_file.header.audio_format), 2);
            file.read(reinterpret_cast<char*>(&wav_file.header.num_of_channels), 2);
            file.read(reinterpret_cast<char*>(&wav_file.header.sample_rate), 4);
            file.read(reinterpret_cast<char*>(&wav_file.header.byte_rate), 4);
            file.read(reinterpret_cast<char*>(&wav_file.header.block_align), 2);
            file.read(reinterpret_cast<char*>(&wav_file.header.bits_per_sample), 2);
            if (chunk_size > 16)
                file.seekg(chunk_size - 16, std::ios::cur);
            found_fmt = true;
        }
        else if (std::string_view(chunk_id, 4) == "data") {
            wav_file.header.audio_data_size = chunk_size;
            found_data = true;
        }
        else {
            file.seekg(chunk_size, std::ios::cur);
        }
    }

    if (wav_file.header.bits_per_sample != 16)
        throw std::runtime_error("Only 16-bit WAV supported");

    wav_file.samples.resize(wav_file.header.audio_data_size / 2);
    std::vector<int16_t> raw_samples(wav_file.samples.size());
    file.read(reinterpret_cast<char*>(raw_samples.data()), wav_file.header.audio_data_size);

    for (size_t i = 0; i < raw_samples.size(); ++i)
        wav_file.samples[i] = raw_samples[i] / 32768.0f;

    return wav_file;
}