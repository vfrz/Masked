#pragma once

#include <iostream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <execution>
#include "PredictionResult.h"
#include "MaskedType.h"
#include "ComparisonAlgorithm.h"
#include "MaskedLBPModel.h"

namespace fs = std::filesystem;

class Predictor
{
public:
    explicit Predictor(fs::path &datasetPath, bool pyramid = false);

    int Predict();

private:
    fs::path _datasetPath;
    bool _pyramid;

    PredictionResult Predict(fs::path &testPath, std::vector<MaskedLBPModel> &trainModels, ComparisonAlgorithm algorithm,
                             MaskedType expectedType);

    static double getDifference(MaskedLBPModel &model1, MaskedLBPModel &model2, ComparisonAlgorithm algorithm);

    static double getSAD(MaskedLBPModel &model1, MaskedLBPModel &model2);

    static double getIntersect(MaskedLBPModel &model1, MaskedLBPModel &model2);

    static double getChiSquare(MaskedLBPModel &model1, MaskedLBPModel &model2);

    static double getBhattacharyya(MaskedLBPModel &model1, MaskedLBPModel &model2);

    static double getCorrelation(MaskedLBPModel &model1, MaskedLBPModel &model2);
};