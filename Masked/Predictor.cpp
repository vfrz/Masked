#include "Predictor.h"

Predictor::Predictor(fs::path &datasetPath, bool pyramid)
{
    _datasetPath = datasetPath;
    _pyramid = pyramid;
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

    //SAD
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

    //Correlation
    auto test1CMFDCorrelationLambda = [&]()
    {
        results.emplace_back(Predict(test1CMFDPath, train1Models, ComparisonAlgorithm::Correlation, MaskedType::Good));
    };
    auto test1IMFDCorrelationLambda = [&]()
    {
        results.emplace_back(Predict(test1IMFDPath, train1Models, ComparisonAlgorithm::Correlation, MaskedType::Bad));
    };
    lambdas.emplace_back(test1CMFDCorrelationLambda);
    lambdas.emplace_back(test1IMFDCorrelationLambda);

    //ChiSquare
    auto test1CMFDChiSquareLambda = [&]()
    {
        results.emplace_back(Predict(test1CMFDPath, train1Models, ComparisonAlgorithm::ChiSquare, MaskedType::Good));
    };
    auto test1IMFDChiSquareLambda = [&]()
    {
        results.emplace_back(Predict(test1IMFDPath, train1Models, ComparisonAlgorithm::ChiSquare, MaskedType::Bad));
    };
    lambdas.emplace_back(test1CMFDChiSquareLambda);
    lambdas.emplace_back(test1IMFDChiSquareLambda);

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

    //Bhattacharyya
    auto test1CMFDBhattacharyyaLambda = [&]()
    {
        results.emplace_back(
                Predict(test1CMFDPath, train1Models, ComparisonAlgorithm::Bhattacharyya, MaskedType::Good));
    };
    auto test1IMFDBhattacharyyaLambda = [&]()
    {
        results.emplace_back(Predict(test1IMFDPath, train1Models, ComparisonAlgorithm::Bhattacharyya, MaskedType::Bad));
    };
    lambdas.emplace_back(test1CMFDBhattacharyyaLambda);
    lambdas.emplace_back(test1IMFDBhattacharyyaLambda);

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
        auto image = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
        auto imageModel = MaskedLBPModel::computeFromImage(image, MaskedType::Unknown, _pyramid);

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
    switch (algorithm)
    {
        case SAD:
            return getSAD(model1, model2);
        case Correlation:
            return getCorrelation(model1, model2);
        case ChiSquare:
            return getChiSquare(model1, model2);
        case Intersection:
            return getIntersect(model1, model2);
        case Bhattacharyya:
            return getBhattacharyya(model1, model2);
        default:
            throw std::exception();
    }
}

double Predictor::getSAD(MaskedLBPModel &model1, MaskedLBPModel &model2)
{
    double total = 0;

    auto model1Data = model1.getData();
    auto model2Data = model2.getData();

    for (auto i = 0; i < model1Data.size(); i++)
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

    for (auto i = 0; i < model1Data.size(); i++)
    {
        if (model1Data[i] != model2Data[i])
        {
            total += std::min(model1Data[i], model2Data[i]);
        }
    }

    return total;
}

double Predictor::getChiSquare(MaskedLBPModel &model1, MaskedLBPModel &model2)
{
    double total = 0;

    auto model1Data = model1.getData();
    auto model2Data = model2.getData();

    for (auto i = 0; i < model1Data.size(); i++)
    {
        if (model2Data[i] != 0)
        {
            total += (model1Data[i] - model2Data[i]) * (model1Data[i] - model2Data[i]) / (double) model2Data[i];
        }
    }

    return total;
}

double Predictor::getBhattacharyya(MaskedLBPModel &model1, MaskedLBPModel &model2)
{
    double mean_v1 = 0, mean_v2 = 0, coefBhattacharyaSum = 0;

    auto model1Data = model1.getData();
    auto model2Data = model2.getData();

    for (auto i = 0; i < model1Data.size(); i++)
    {
        mean_v1 += model1Data[i];
        mean_v2 += model2Data[i];
        coefBhattacharyaSum += sqrt(model1Data[i] * model2Data[i]);
    }

    mean_v1 = mean_v1 / (float) model1Data.size();
    mean_v2 = mean_v2 / (float) model1Data.size();

    return sqrt(1.0f - (1.0f / sqrt(mean_v1 * mean_v2 * model1Data.size() * model1Data.size())) * coefBhattacharyaSum);
}

double Predictor::getCorrelation(MaskedLBPModel &model1, MaskedLBPModel &model2)
{
    double mean_v1 = 0, mean_v2 = 0, dividend = 0, diviseur1 = 0, diviseur2 = 0;

    auto model1Data = model1.getData();
    auto model2Data = model2.getData();

    for (auto i = 0; i < model1Data.size(); i++)
    {
        mean_v1 += model1Data[i];
        mean_v2 += model2Data[i];
    }

    mean_v1 = mean_v1 / (float) model1Data.size();
    mean_v2 = mean_v2 / (float) model1Data.size();

    for (auto i = 0; i < model1Data.size(); i++)
    {
        dividend += (model1Data[i] - mean_v1) * (model2Data[i] - mean_v2);
        diviseur1 += (model1Data[i] - mean_v1) * (model1Data[i] - mean_v1);
        diviseur2 += (model2Data[i] - mean_v2) * (model2Data[i] - mean_v2);
    }

    return dividend / sqrt(diviseur1 * diviseur2);
}

