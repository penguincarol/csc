#include <iostream>
#include <vector>
#include <algorithm>

namespace csc {
class BitmapGenerator {
    std::vector<bool> bitmap; // Stores the current configuration
    u_int8_t k;

public:
    BitmapGenerator(size_t size, size_t k) : bitmap(size, false), k(std::min(size,k)) {
        for (size_t i = 0; i < this->k; i++) {
            bitmap[i] = true;
        }
    }

    const std::vector<bool> &current() const {
        return bitmap;
    }

    void reset() {
        for (size_t i = 0; i < k; i++) {
            bitmap[i] = true;
        }
        for (size_t i = k; i < bitmap.size(); i++) {
            bitmap[i] = false;
        }

    }

    bool next() {
        bool has_next = std::next_permutation(bitmap.begin(), bitmap.end());
        reset();
        return has_next;
    }
};
}