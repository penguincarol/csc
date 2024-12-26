#pragma once
#include "nCrBitmapGenerator.h"
#include "ScoreVector.h"

namespace csc {
class BitMapsForScoreVector {
public:
    BitMapsForScoreVector(const ScoreVector& scoreVector, size_t numberDominators){
        //do *not* contain bitmap defining edges for node1 since we do that manually
        for(size_t i=1; i < scoreVector.score_vector.size(); i++){
            auto k = scoreVector.score_vector[i];
            bitmaps.emplace_back(numberDominators, k);
        }
    }

    bool operator++(){

        size_t i = bitmaps.size()-1;
        bool result_next = bitmaps[i].next();
        while(result_next == false){
            if(i==0){
                return false;
            }
            i--;
            result_next = bitmaps[i].next();
        }
        return true;
    }

    //does *not* contain bitmap defining edges for node1 since we do that manually
    std::vector<BitmapGenerator> bitmaps;
};
}