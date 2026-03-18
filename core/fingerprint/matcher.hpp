#pragma once
#include <vector>
#include <unordered_map>
#include <cstdint>
#include "hasher.hpp"
#include "database.hpp"

struct MatchResult {
    size_t song_id;
    size_t score;
};

class Matcher {
    const Database& db_;

    public:
    Matcher(const Database &db) : db_(db) {}
    MatchResult match(const std::vector<HashEntry>& query_entries) const;
};