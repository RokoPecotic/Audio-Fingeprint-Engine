#pragma once
#include <vector>
#include <cmath>

static constexpr size_t PEAKS_PER_FRAME = 5;


struct Peak {
    size_t frame;
    size_t bin;
    float magnitude;
};

class PeakExtractor {
    size_t peaks_per_frame_;

public:
    PeakExtractor(size_t peaks_per_frame) : peaks_per_frame_(peaks_per_frame){}

    std::vector<Peak> extract(const std::vector<std::vector<float>>& spectogram) const {
        std::vector<Peak> peaks;
        for(size_t frame = 0; frame < spectogram.size(); ++frame){
            
            float sum = 0.0f;
            for(float val : spectogram[frame]){
                sum += val;
            }
            float mean = sum / spectogram[frame].size();

            for(size_t bin = 2; bin < spectogram[frame].size() - 2; ++bin){
                float spec_val = spectogram[frame][bin];

                if(spec_val > mean * 1.5f &&
                   spec_val > spectogram[frame][bin - 1] &&
                   spec_val > spectogram[frame][bin - 2] &&
                   spec_val > spectogram[frame][bin + 1] && 
                   spec_val > spectogram[frame][bin + 2]){
                    
                    peaks.push_back({frame, bin, spec_val});
                }
            }
        }
        return peaks;
    }
};