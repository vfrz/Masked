//
// Created by vfrz on 11/10/20.
//

#include "PredictionResult.h"

PredictionResult::PredictionResult(int good, int bad, double elapsedTimeInSeconds)
{
    _good = good;
    _bad = bad;
    _elapsedTimeInSeconds = elapsedTimeInSeconds;
}

int PredictionResult::getGood()
{
    return _good;
}

int PredictionResult::getBad()
{
    return _bad;
}

int PredictionResult::getTotal()
{
    return _good + _bad;
}

double PredictionResult::getElapsedTimeInSeconds()
{
    return _elapsedTimeInSeconds;
}
