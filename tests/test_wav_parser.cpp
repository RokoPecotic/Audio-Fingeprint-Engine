#include <gtest/gtest.h>
#include "wav_parser.hpp"
#include "stft.hpp"
#include "peak_extractor.hpp"

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