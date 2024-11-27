#ifndef PROBSOFLEVELS_H
#define PROBSOFLEVELS_H
#include <vector>
#include <unordered_map>

class ProbsOfLevels {
    std::vector<std::vector<float>> probabilities{
        {1 / 6.0, 1 / 6.0, 1 / 6.0 , 1 / 6.0, 1 / 12.0, 1 / 12.0, 1 / 6.0},
        {1 / 7.0, 1 / 7.0, 1 / 7.0, 1 / 7.0, 1 / 7.0, 1 / 7.0, 1 / 7.0},
        {1 / 9.0, 1 / 9.0, 1 / 9.0, 1 / 9.0, 2 / 9.0, 2 / 9.0, 1 / 9.0, },
    };
    // hashmap that indicates which level corresponds to which probability index
    // for 'probabilities'
    std::unordered_map<int, int> lvlToProbIdx{
        {1, 0},
        {2, 1},
        {3, 2},
        {4, 2}
    };

    public:
        // given a level number, returns the associated probability
        std::vector<float> obtainLvlProb(int level) {
            return probabilities[lvlToProbIdx[level]];
        }
};

#endif
