#include <gtest/gtest.h>
#include <iostream>
#include <cstdint>
#include "wav_parser.hpp"
#include "stft.hpp"
#include "peak_extractor.hpp"
#include "hasher.hpp"
#include "database.hpp"
#include "matcher.hpp"

TEST(WavParserTest, LoadsCorrectSampleRate) {
    WavFile wav = load_wav(TEST_DATA_DIR "/test_440hz.wav");
    EXPECT_EQ(wav.header.sample_rate, 44100);
}

TEST(WavParserTest, LoadsCorrectChannels) {
    WavFile wav = load_wav(TEST_DATA_DIR "/test_440hz.wav");
    EXPECT_EQ(wav.header.num_of_channels, 1);
}

TEST(WavParserTest, LoadsCorrectBitsPerSample) {
    WavFile wav = load_wav(TEST_DATA_DIR "/test_440hz.wav");
    EXPECT_EQ(wav.header.bits_per_sample, 16);
}

TEST(WavParserTest, LoadsCorrectNumberOfSamples) {
    WavFile wav = load_wav(TEST_DATA_DIR "/test_440hz.wav");
    EXPECT_EQ(wav.samples.size(), 44100 * 3);
}

TEST(WavParserTest, ThrowsForMissingFile) {
    EXPECT_THROW(load_wav("doesnt_exist.wav"), std::runtime_error);
}

TEST(STFTTest, PeakAt440Hz) {
    WavFile wav = load_wav(TEST_DATA_DIR "/test_440hz.wav");
    STFT<Hann> stft(1024, 512);
    auto spectrogram = stft.compute(wav.samples);
    EXPECT_GT(spectrogram[0][10], spectrogram[0][9]);
    EXPECT_GT(spectrogram[0][10], spectrogram[0][11]);
}

TEST(PeakExtractorTest, FindsPeakAt440Hz) {
    WavFile wav = load_wav(TEST_DATA_DIR "/test_440hz.wav");
    STFT<Hann> stft(1024, 512);

    auto spectogram = stft.compute(wav.samples);

    PeakExtractor extractor(PEAKS_PER_FRAME);
    auto peaks = extractor.extract(spectogram);

    bool found_peak = false;
    for(const auto& peak : peaks){
        if(peak.bin == 10){
            found_peak = true;
            break;
        }
    }
    EXPECT_TRUE(found_peak);
}

TEST(HasherTest, GeneratesHashEntries) {
    WavFile wav = load_wav(TEST_DATA_DIR "/test_440hz.wav");
    STFT<Hann> stft(1024, 512);
    auto spectogram = stft.compute(wav.samples);
    PeakExtractor extractor(PEAKS_PER_FRAME);
    auto peaks = extractor.extract(spectogram);

    Hasher hasher;
    auto entries = hasher.generate(peaks, 0);
    EXPECT_GT(entries.size(), 10);
    std::cout << "Entries: " << entries.size() << std::endl;
}

TEST(DatabaseTest, LookupReturnsMatches) {
    WavFile wav = load_wav(TEST_DATA_DIR "/test_440hz.wav");
    STFT<Hann> stft(1024, 512);
    auto spectogram = stft.compute(wav.samples);
    PeakExtractor extractor(PEAKS_PER_FRAME);
    auto peaks = extractor.extract(spectogram);

    Hasher hasher;
    auto entries = hasher.generate(peaks, 0);

    Database db;
    db.add(entries);

    uint32_t key = entries[0].hash.to_key();
    auto results = db.lookup(key);
    EXPECT_GT(results.size(), 0);
}

TEST(DatabaseTest, LookupMissingKeyReturnsEmpty){
    Database db;
    auto results = db.lookup(999999999);
    EXPECT_EQ(results.size(), 0);
}

TEST(MatcherTest, MatchesSameSong) {
    WavFile wav = load_wav(TEST_DATA_DIR "/test_440hz.wav");
    STFT<Hann> stft(1024, 512);
    auto spectogram = stft.compute(wav.samples);
    PeakExtractor extractor(PEAKS_PER_FRAME);
    auto peaks = extractor.extract(spectogram);

    Hasher hasher;
    auto entries = hasher.generate(peaks, 0);

    Database db;
    db.add(entries);

    Matcher matcher(db);
    auto result = matcher.match(entries);

    EXPECT_EQ(result.song_id, 0);
    EXPECT_GT(result.score, 100);
}