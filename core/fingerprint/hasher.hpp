#pragma once
#include <cstddef>
#include <vector>
#include<cstdint>
#include "peak_extractor.hpp"


struct Hash{
    size_t delta_frame;
    size_t bin_anchor;
    size_t bin_target;

    uint32_t to_key() const {
        return (bin_anchor << 20) | (bin_target << 10) | (delta_frame & 0x3FF);
    }
};

struct HashEntry{
    Hash hash;
    size_t song_id;
    size_t anchor_frame;
};

class Hasher{
    static constexpr size_t MIN_DELTA = 1;
    static constexpr size_t MAX_DELTA = 10;

    public:
    Hasher() = default;
    std::vector<HashEntry> generate(const std::vector<Peak>& peaks, size_t song_id) const;
};