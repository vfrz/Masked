#include "Predictor.h"

Predictor::Predictor(fs::path &datasetPath)
{
    _datasetPath = datasetPath;
}

int Predictor::Predict()
{
    auto test1Path = fs::path(_datasetPath).append("GRAY/1TEST/");
    auto train1Path = fs::path(_datasetPath).append("GRAY/1TRAIN/train.lbp");
    auto test1CMFDPath = fs::path(test1Path).append("CMFD/");
    auto test1IMFDPath = fs::path(test1Path).append("IMFD/");

    auto train1Models = MaskedLBPModel::loadFromFile(train1Path);

    auto output1File = fs::path(test1Path).append("results.txt");

    if (fs::exists(output1File))
    {
        fs::remove(output1File);
    }

    auto test1CMFDSADPredictionResult = Predict(test1CMFDPath, train1Models, ComparisonAlgorithm::SAD, MaskedType::Good,
                                                output1File);

    std::cout << test1CMFDSADPredictionResult.getGood() << std::endl;
    std::cout << test1CMFDSADPredictionResult.getBad() << std::endl;
    std::cout << test1CMFDSADPredictionResult.getTotal() << std::endl;

    return 0;
}

PredictionResult
Predictor::Predict(fs::path &testPath, std::vector<MaskedLBPModel> &trainModels, ComparisonAlgorithm algorithm,
                   MaskedType expectedType, fs::path &outputFilePath)
{
    int good = 0, bad = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for (const auto &entry : fs::directory_iterator(testPath))
    {
        std::cout << "[PREDICT] Processing image: " << entry.path().filename() << std::endl;

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

    return PredictionResult(good, bad, elapsed.count());
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

    for (auto i = 0; i < 256; i++)
    {
        total += std::abs(model1.getData(i) - model2.getData(i));
    }

    return total;
}

double Predictor::getIntersect(MaskedLBPModel &model1, MaskedLBPModel &model2)
{
    double total = 0;

    for (auto i = 0; i < 256; i++)
    {
        if (model1.getData(i) != model2.getData(i))
        {
            total += std::min(model1.getData(i), model2.getData(i));
        }
    }

    return total;
}