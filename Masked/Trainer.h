#pragma once

#include <iostream>
#include <filesystem>

#include "MaskedLBPModel.h"

namespace fs = std::filesystem;

class Trainer
{
public:
    explicit Trainer(fs::path &datasetPath);

    int Train();

private:
    fs::path _datasetPath;
};