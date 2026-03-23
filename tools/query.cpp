#include <iostream>
#include "wav_parser.hpp"
#include "stft.hpp"
#include "peak_extractor.hpp"
#include "hasher.hpp"
#include "database.hpp"
#include "matcher.hpp"

int main(int argc, char* argv[]) {
    if(argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <database.bin> <query.wav> ..." << std::endl;
        return 1;
    }

    Database db;
    db.load(argv[1]);

    WavFile wav = load_wav(argv[2]);
    STFT<Hann> stft(1024, 512);
    auto spectogram = stft.compute(wav.samples);
    PeakExtractor extractor(PEAKS_PER_FRAME);
    auto peaks = extractor.extract(spectogram);

    Hasher hasher;
    auto entries = hasher.generate(peaks, 0);

    Matcher matcher(db);
    auto result = matcher.match(entries);

    std::cout << "Match: song_id = " << result.song_id << " score = " << result.score << std::endl;

    return 0;
}

