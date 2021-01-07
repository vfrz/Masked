#include "Trainer.h"

Trainer::Trainer(fs::path &datasetPath, bool pyramid)
{
    _datasetPath = datasetPath;
    _pyramid = pyramid;
}

int Trainer::Train()
{
    auto train1Path = fs::path(_datasetPath).append("GRAY/1TRAIN/");
    auto train1CMFDPath = fs::path(train1Path).append("CMFD/");
    auto train1IMFDPath = fs::path(train1Path).append("IMFD/");

    auto outputFile = fs::path(train1Path).append("train.lbp");

    if (fs::exists(outputFile))
    {
        fs::remove(outputFile);
    }

    Train(train1CMFDPath, MaskedType::Good, outputFile);
    Train(train1IMFDPath, MaskedType::Bad, outputFile);

    return 0;
}

void Trainer::Train(fs::path &trainPath, MaskedType maskedType, fs::path &outputFilePath)
{
    std::ofstream stream;
    stream.open(outputFilePath, std::ios_base::app);

    for (const auto &entry : fs::directory_iterator(trainPath))
    {
        std::cout << "[TRAINING] Processing image: " << entry.path().filename() << std::endl;

        auto inputImagePath = entry.path();

        cv::Mat image = cv::imread(inputImagePath, cv::IMREAD_GRAYSCALE);

        MaskedLBPModel model = MaskedLBPModel::computeFromImage(image, maskedType, _pyramid);

        auto modelData = model.getData();

        stream << maskedType;

        for (auto i = 0; i < modelData.size(); i++)
        {
            stream  << "," << modelData[i];
        }

        stream << std::endl;
    }

    stream.close();
}