#include <iostream>
#include <algorithm>
#include "wav_parser.hpp"
#include "stft.hpp"
#include "peak_extractor.hpp"
#include "hasher.hpp"
#include "database.hpp"

int main(int argc, char* argv[]) {
    if(argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <database.bin> <song1.wav> ..." << std::endl;
        return 1;
    }

    Database db;
    const size_t CHUNK_SIZE = 44100 * 30;

    for(int i = 2; i < argc; ++i) {
        std::cout << "Loading: " << argv[i] << std::endl;
        WavFile wav = load_wav(argv[i]);
        std::cout << "Loaded, samples: " << wav.samples.size() << std::endl;
        
        size_t total_samples = wav.samples.size();
        size_t offset = 0;
        
        while(offset < total_samples) {
            size_t end = std::min(offset + CHUNK_SIZE, total_samples);
            std::vector<float> chunk(wav.samples.begin() + offset, wav.samples.begin() + end);
            
            STFT<Hann> stft(1024, 512);
            auto spectrogram = stft.compute(chunk);
            PeakExtractor extractor(PEAKS_PER_FRAME);
            auto peaks = extractor.extract(spectrogram);

            Hasher hasher;
            auto entries = hasher.generate(peaks, i - 2);
            db.add(entries);
            
            offset += CHUNK_SIZE;
        }
        std::cout << "Indexed: " << argv[i] << std::endl;
    }
    db.save(argv[1]);
    std::cout << "Number of songs indexed: " << (argc - 2) << std::endl;
    return 0;
}