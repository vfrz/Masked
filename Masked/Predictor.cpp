#include "Predictor.h"

Predictor::Predictor(fs::path &datasetPath)
{
    _datasetPath = datasetPath;
}

int Predictor::Predict()
{
    std::cout << "===== Starting prediction =====" << std::endl;

    auto test1Path = fs::path(_datasetPath).append("GRAY/1TEST/");
    auto train1Path = fs::path(_datasetPath).append("GRAY/1TRAIN/train.lbp");
    auto test1CMFDPath = fs::path(test1Path).append("CMFD/");
    auto test1IMFDPath = fs::path(test1Path).append("IMFD/");

    if (!fs::exists(train1Path))
    {
        std::cout << "Couldn't find train file, you need to execute train before predict." << std::endl;
        return 1;
    }

    auto train1Models = MaskedLBPModel::loadFromFile(train1Path);

    std::vector<std::function<void()>> lambdas;

    std::vector<PredictionResult> results;

    // SAD
    auto test1CMFDSADLambda = [&]()
    {
        results.emplace_back(Predict(test1CMFDPath, train1Models, ComparisonAlgorithm::SAD, MaskedType::Good));
    };
    auto test1IMFDSADLambda = [&]()
    {
        results.emplace_back(Predict(test1IMFDPath, train1Models, ComparisonAlgorithm::SAD, MaskedType::Bad));
    };
    lambdas.emplace_back(test1CMFDSADLambda);
    lambdas.emplace_back(test1IMFDSADLambda);

    //Intersection
    auto test1CMFDIntersectionLambda = [&]()
    {
        results.emplace_back(Predict(test1CMFDPath, train1Models, ComparisonAlgorithm::Intersection, MaskedType::Good));
    };
    auto test1IMFDIntersectionLambda = [&]()
    {
        results.emplace_back(Predict(test1IMFDPath, train1Models, ComparisonAlgorithm::Intersection, MaskedType::Bad));
    };
    lambdas.emplace_back(test1CMFDIntersectionLambda);
    lambdas.emplace_back(test1IMFDIntersectionLambda);

    std::for_each(std::execution::par, lambdas.begin(), lambdas.end(), [&](std::function<void()> &lambda)
    {
        lambda();
    });

    for (auto &result : results)
    {
        result.print();
    }

    return 0;
}

PredictionResult
Predictor::Predict(fs::path &testPath, std::vector<MaskedLBPModel> &trainModels, ComparisonAlgorithm algorithm,
                   MaskedType expectedType)
{
    int good = 0, bad = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for (const auto &entry : fs::directory_iterator(testPath))
    {
        std::cout << "[PREDICTION] Processing image: " << entry.path().filename() << std::endl;

        auto imagePath = entry.path();
        auto imageModel = MaskedLBPModel::computeFromImageFile(imagePath, MaskedType::Unknown);

        double minDiff = std::numeric_limits<double>::max();
        MaskedType currentMaskedType = MaskedType::Unknown;

        for (auto &trainModel : trainModels)
        {
            double diff = getDifference(imageModel, trainModel, algorithm);
            if (diff < minDiff)
            {
                currentMaskedType = trainModel.getMaskedType();
                minDiff = diff;
            }
        }

        if (currentMaskedType == expectedType)
        {
            good++;
        } else
        {
            bad++;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    return PredictionResult(algorithm, good, bad, elapsed.count(), expectedType);
}

double Predictor::getDifference(MaskedLBPModel &model1, MaskedLBPModel &model2, ComparisonAlgorithm algorithm)
{
    if (algorithm == ComparisonAlgorithm::SAD)
    {
        return getSAD(model1, model2);
    }

    if (algorithm == ComparisonAlgorithm::Intersection)
    {
        return getIntersect(model1, model2);
    }

    throw std::exception();
}

double Predictor::getSAD(MaskedLBPModel &model1, MaskedLBPModel &model2)
{
    double total = 0;

    auto model1Data = model1.getData();
    auto model2Data = model2.getData();

    for (auto i = 0; i < 256; i++)
    {
        total += std::abs(model1Data[i] - model2Data[i]);
    }

    return total;
}

double Predictor::getIntersect(MaskedLBPModel &model1, MaskedLBPModel &model2)
{
    double total = 0;

    auto model1Data = model1.getData();
    auto model2Data = model2.getData();

    for (auto i = 0; i < 256; i++)
    {
        if (model1Data[i] != model2Data[i])
        {
            total += std::min(model1Data[i], model2Data[i]);
        }
    }

    return total;
}