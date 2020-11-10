#pragma once

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

class Predictor
{
public:
    explicit Predictor(fs::path &datasetPath);

    int Predict();

private:
    fs::path _datasetPath;
};