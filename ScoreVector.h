#pragma once
#include <vector>
#include <cstdint>

struct ScoreVector{
    ScoreVector(size_t num_vertices_managed, uint8_t maximum_score): score_vector(num_vertices_managed, 0), maximum_score(maximum_score){};

    ScoreVector& operator++(){
        size_t i = score_vector.size()-1;
        score_vector[i] += 1;
        while(score_vector[i] > score_vector[i-1]){
            score_vector[i] = 0;
            i--;
            score_vector[i] += 1;
            if(i == 0){
                break;
            }
        }
        return *this;
    }

    bool operator==(const ScoreVector& other) const{
        return other.score_vector == score_vector;
    }

    //this is not equal to the std::iterator end(), but similar. this function returns the first KemenyScoreVector that is invalid, since it contains one kemeny-rank, that is too high
    ScoreVector end(){
        ScoreVector returnvalue(score_vector.size(), maximum_score);
        returnvalue.score_vector[0] = maximum_score+1;
        return returnvalue;
    }

    std::vector<uint8_t> score_vector;
    uint8_t maximum_score;
};
