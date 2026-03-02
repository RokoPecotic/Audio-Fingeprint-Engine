#include <iostream>
#include <stdexcept>
#include <string>
#include "wav_parser.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_wav_file>" << std::endl;
        return 1;
    }
    std::string path = argv[1];
    try{
        WavFile wav_file = load_wav(path);
        std::cout << "Sample Rate: " << wav_file.header.sample_rate << " Hz" << std::endl;
        std::cout << "Channels: " << wav_file.header.num_of_channels << std::endl;
        std::cout << "Bits per Sample: " << wav_file.header.bits_per_sample << std::endl;
        std::cout << "Number of Samples: " << wav_file.samples.size() << std::endl;
        float duration = static_cast<float>(wav_file.samples.size()) / wav_file.header.sample_rate;
        std::cout << "Duration: " << duration << " seconds" << std::endl;
    } 
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}