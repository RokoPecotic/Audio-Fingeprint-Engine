# Audio Fingerprinting Engine

A C++ implementation of an audio fingerprinting system inspired by Shazam's core algorithm. Given an audio query, the engine identifies which song it matches from an indexed database.

## How it works

1. **WAV Parser** - reads 16-bit PCM WAV files into normalized float samples.
2. **STFT Engine** - converts audio signal into a spectrogram using Short-Time Fourier Transform with Hann windowing.
3. **Peak Extractor** - finds prominent peaks in the spectrogram using local maximum detection and adaptive threshold.
4. **Hash Generator** - combines peak pairs into bit-packed fingerprint hashes using target zone algorithm.

## Build

### Requirements
- CMake 3.20+
- GCC 12+ or Clang 15+
- FFTW3
- Google Test
- Google Benchmark

### Instructions
```bash
sudo apt install -y cmake ninja-build libfftw3-dev libgtest-dev
mkdir build && cd build
cmake .. -G Ninja
ninja
```

## Run tests
```bash
./tests/test_wav_parser
```

## Project structure
```
core/
├── audio/        # WAV parser
├── dsp/          # STFT engine, peak extractor
└── fingerprint/  # Hash generator
tests/            # Google Test suite
tools/            # CLI tools
```

## References
- Wang, A. (2003). *An Industrial-Strength Audio Search Algorithm*. Shazam Entertainment.

## Status
Work in progress - matching engine and real-time microphone input coming soon.
