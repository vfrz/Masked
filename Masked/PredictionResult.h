#pragma once

class PredictionResult
{
public:
    PredictionResult(int good, int bad, double elapsedTimeInSeconds);
    int getGood();
    int getBad();
    int getTotal();
    double getElapsedTimeInSeconds();

private:
    int _good;
    int _bad;
    double _elapsedTimeInSeconds;
};