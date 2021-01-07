#pragma once

#include <iostream>
#include <filesystem>

#include "MaskedLBPModel.h"

namespace fs = std::filesystem;

class Trainer
{
public:
    explicit Trainer(fs::path &datasetPath, bool pyramid = false);

    int Train();

private:
    fs::path _datasetPath;
    bool _pyramid;
    void Train(fs::path &trainPath, MaskedType maskedType, fs::path &outputFilePath);
};