#include <vector>
#include <cstddef>
#include "peak_extractor.hpp"
#include "hasher.hpp"


std::vector<HashEntry> Hasher::generate(const std::vector<Peak>& peaks, size_t song_id) const {
std::vector<HashEntry> entries;

    for(size_t i = 0; i < peaks.size(); ++i){
        for(size_t j = i + 1; j < peaks.size(); ++j){
            size_t delta_frame = peaks[j].frame - peaks[i].frame;

            if(delta_frame >= MIN_DELTA && delta_frame <= MAX_DELTA){
                HashEntry entry;
                entry.hash = {delta_frame, peaks[i].bin, peaks[j].bin};
                entry.song_id = song_id;
                entry.anchor_frame = peaks[i].frame;
                entries.push_back(entry);
            }
        }
    }

    return entries;
}