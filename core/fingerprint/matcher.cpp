#include <vector>
#include <unordered_map>
#include "hasher.hpp"
#include "matcher.hpp"

MatchResult Matcher::match(const std::vector<HashEntry>& query_entries) const {
    std::unordered_map<uint64_t, size_t> histogram;
    
    for(const auto& query_entry : query_entries){
    auto key = query_entry.hash.to_key();
    auto results = db_.lookup(key);

        for(const auto& result : results){
            auto offset = result.anchor_frame - query_entry.anchor_frame;
            uint64_t hist_key = ((uint64_t)result.song_id << 32) | (uint32_t)offset;
            histogram[hist_key]++;
        }
    }

    size_t best_score = 0;
    uint64_t best_key = 0;

    for(const auto& [hist_key, score] : histogram){
        if(score > best_score){
            best_score = score;
            best_key = hist_key;
        }
    }
    size_t winner_song_id = best_key >> 32;
    return {winner_song_id, best_score};
}

