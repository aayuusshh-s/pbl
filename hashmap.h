#ifndef HASHMAP_H
#define HASHMAP_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

class StayHashMap {
    unordered_map<string, vector<int>> table;

public:

    void insert(string city, int index) {
        table[city].push_back(index);
    }

    vector<int> searchByCity(string city) const {

        auto it = table.find(city);

        if (it != table.end())
            return it->second;

        return {};
    }

    void clear() {
        table.clear();
    }
};

#endif