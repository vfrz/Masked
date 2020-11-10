#include "PredictionResult.h"

PredictionResult::PredictionResult(ComparisonAlgorithm algorithm, int good, int bad, double elapsedTimeInSeconds, MaskedType maskedType)
{
    _algorithm = algorithm;
    _good = good;
    _bad = bad;
    _elapsedTimeInSeconds = elapsedTimeInSeconds;
    _maskedType = maskedType;
}

ComparisonAlgorithm PredictionResult::getAlgorithm()
{
    return _algorithm;
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

double PredictionResult::getRatio()
{
    return (double) _good / (double) getTotal();
}

void PredictionResult::print()
{
    std::cout << "===== Test results =====" << std::endl;
    std::cout << "Algorithm: " << ComparisonAlgorithmHelper::toString(getAlgorithm()) << std::endl;
    std::cout << "Masked: " << MaskedTypeHelper::toString(getMaskedType()) << std::endl;
    std::cout << "Good: " << getGood() << std::endl;
    std::cout << "Bad: " << getBad() << std::endl;
    std::cout << "Total: " << getTotal() << std::endl;
    std::cout << "Ratio: " << getRatio() << std::endl;
    std::cout << "Elapsed time in seconds: " << getElapsedTimeInSeconds() << std::endl;
}

MaskedType PredictionResult::getMaskedType()
{
    return _maskedType;
}
