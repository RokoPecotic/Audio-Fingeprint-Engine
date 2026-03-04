#pragma once
#include <vector>
#include <complex>
#include <cmath>
#include <fftw3.h>

template <typename WindowFunc>
class STFT {
    size_t window_size_;
    size_t hop_size_;
    WindowFunc window_func_;

public:

STFT(size_t window_size, size_t hop_size)
    : window_size_(window_size), hop_size_(hop_size) {}

std::vector<std::vector<float>> compute(const std::vector<float>& samples) const{
    size_t num_frames = (samples.size() - window_size_) / hop_size_ + 1;
    std::vector<std::vector<float>> spectrogram(num_frames, std::vector<float>(window_size_ / 2 + 1));

    float* input = fftwf_alloc_real(window_size_);
    fftwf_complex* output = fftwf_alloc_complex(window_size_ / 2 + 1);
    fftwf_plan plan = fftwf_plan_dft_r2c_1d(window_size_, input, output, FFTW_ESTIMATE);

    for(size_t frame = 0; frame < num_frames; ++frame) {
        
        size_t start = frame * hop_size_;
        
        for(size_t n = 0; n < window_size_; ++n){
            size_t start_idx = start + n;
            input[n] = samples[start_idx] * window_func_(n, window_size_);
        }

        fftwf_execute(plan);

        for(size_t k = 0; k < window_size_ / 2 + 1; ++k) {
        spectrogram[frame][k] = std::sqrt(
            output[k][0] * output[k][0] + 
            output[k][1] * output[k][1]
        );
    
        }
    }

    fftwf_destroy_plan(plan);
    fftwf_free(input);
    fftwf_free(output);
    return spectrogram;
}

};

struct Hann {
    float operator()(size_t n, size_t N) const {
        return 0.5f * (1.0f - std::cos(2.0f * M_PI * n / (N - 1)));
    }
};
