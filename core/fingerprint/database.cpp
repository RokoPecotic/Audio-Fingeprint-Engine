#include <vector>
#include <cstdint>
#include <unordered_map>
#include "database.hpp"
#include "hasher.hpp"

void Database::add(const std::vector<HashEntry>& entries){
    for(const auto& entry : entries){
        uint32_t key = entry.hash.to_key();
        index_[key].push_back({entry.song_id, entry.anchor_frame});
    }

}

std::vector<MatchEntry> Database::lookup(uint32_t hash_key) const {
    auto it = index_.find(hash_key);
    if(it != index_.end()) {
        return it->second;
    }
    return {};
}