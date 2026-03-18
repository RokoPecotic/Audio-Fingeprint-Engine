#include <vector>
#include <cstdint>
#include <unordered_map>
#include <fstream>
#include <stdexcept>
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

void Database::save(const std::string& path) const {
    std::ofstream file(path, std::ios::binary);
    if(!file.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + path);
    }
    size_t num_keys = index_.size();
    file.write(reinterpret_cast<const char*>(&num_keys), sizeof(num_keys));
    for(const auto& [key, entries] : index_) {
        file.write(reinterpret_cast<const char*>(&key), sizeof(key));
        size_t num_entries = entries.size();
        file.write(reinterpret_cast<const char*>(&num_entries), sizeof(num_entries));
        for(const auto& entry : entries) {
            file.write(reinterpret_cast<const char*>(&entry.song_id), sizeof(entry.song_id));
            file.write(reinterpret_cast<const char*>(&entry.anchor_frame), sizeof(entry.anchor_frame));
        }
    }
}

void Database::load(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if(!file.is_open()) {
        throw std::runtime_error("Could not open file for reading: " + path);
    }
    index_.clear();

    size_t num_keys = 0;
    file.read(reinterpret_cast<char*>(&num_keys), sizeof(num_keys));

    for(size_t i = 0; i < num_keys; ++i){
        uint32_t key;
        file.read(reinterpret_cast<char*>(&key), sizeof(key));

        size_t num_entries;
        file.read(reinterpret_cast<char*>(&num_entries), sizeof(num_entries));

        for(size_t j = 0; j < num_entries; ++j){
            size_t song_id, anchor_frame;
            file.read(reinterpret_cast<char*>(&song_id), sizeof(song_id));
            file.read(reinterpret_cast<char*>(&anchor_frame), sizeof(anchor_frame));
            index_[key].push_back({song_id, anchor_frame});
        }
    }

}