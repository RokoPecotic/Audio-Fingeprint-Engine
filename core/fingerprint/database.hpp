#pragma once
#include <unordered_map>
#include <vector>
#include <cstdint>
#include "hasher.hpp"

struct MatchEntry{
    size_t song_id;
    size_t anchor_frame;
};

class Database{
    std::unordered_map<uint32_t, std::vector<MatchEntry>> index_;

    public:
    void add(const std::vector<HashEntry>& entry);
    std::vector<MatchEntry> lookup(uint32_t hash_key) const;
};
