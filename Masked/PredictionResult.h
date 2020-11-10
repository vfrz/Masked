#pragma once

#include <iostream>
#include "ComparisonAlgorithm.h"
#include "ComparisonAlgorithmHelper.h"
#include "MaskedType.h"
#include "MaskedTypeHelper.h"

class PredictionResult
{
public:
    PredictionResult(ComparisonAlgorithm algorithm, int good, int bad, double elapsedTimeInSeconds, MaskedType maskedType);
    ComparisonAlgorithm getAlgorithm();
    int getGood();
    int getBad();
    int getTotal();
    double getRatio();
    double getElapsedTimeInSeconds();
    MaskedType getMaskedType();
    void print();

private:
    ComparisonAlgorithm _algorithm;
    int _good;
    int _bad;
    double _elapsedTimeInSeconds;
    MaskedType _maskedType;
};