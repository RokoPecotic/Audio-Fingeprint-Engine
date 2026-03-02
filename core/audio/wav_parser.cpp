#include <fstream>
#include <stdexcept>
#include <string_view>
#include "wav_parser.hpp"

WavFile load_wav(const std::string& path){
    WavFile wav_file;
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()){
        throw std::runtime_error("Could not open file " + path);
    }
    file.read(reinterpret_cast<char*>(&wav_file.header), sizeof(WavHeader));
    if (std::string_view(wav_file.header.riff, 4) != "RIFF" || std::string_view(wav_file.header.wave, 4) != "WAVE"){
        throw std::runtime_error("Invalid WAV file: " + path);
    }
    
    if (wav_file.header.bits_per_sample != 16) {
    throw std::runtime_error("Only 16-bit WAV supported");
    }

    wav_file.samples.resize(wav_file.header.audio_data_size / (wav_file.header.bits_per_sample / 8));
    
    std::vector<int16_t> raw_samples(wav_file.samples.size());

    file.read(reinterpret_cast<char*>(raw_samples.data()), wav_file.header.audio_data_size);
    for(size_t i = 0; i < raw_samples.size(); ++i){
        wav_file.samples[i] = raw_samples[i] / 32768.0f;
    }

    return wav_file;
}