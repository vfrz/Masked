#include "ComparisonAlgorithmHelper.h"

std::string ComparisonAlgorithmHelper::toString(ComparisonAlgorithm comparisonAlgorithm)
{
    switch (comparisonAlgorithm)
    {
        case SAD:
            return "SAD";
        case Correlation:
            return "Correlation";
        case ChiSquare:
            return "ChiSquare";
        case Intersection:
            return "Intersection";
        case Bhattacharyya:
            return "Bhattacharyya";
        default:
            return "";
    }
}
