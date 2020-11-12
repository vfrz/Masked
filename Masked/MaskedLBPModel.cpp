#include "MaskedLBPModel.h"

MaskedLBPModel::MaskedLBPModel(MaskedType maskedType, std::vector<ushort> &data)
{
    _maskedType = maskedType;
    _data = data;
}

MaskedType MaskedLBPModel::getMaskedType()
{
    return _maskedType;
}

std::vector<ushort> &MaskedLBPModel::getData()
{
    return _data;
}

MaskedLBPModel MaskedLBPModel::computeFromImageFile(fs::path &filePath, MaskedType maskedType)
{
    cv::Mat inputImage = cv::imread(filePath, cv::IMREAD_GRAYSCALE);

    std::vector<ushort> data = getLBP(inputImage);

    return MaskedLBPModel(maskedType, data);
}

std::vector<ushort> MaskedLBPModel::getLBP(cv::Mat &image)
{
    auto offsetsMultipliers = new std::tuple<cv::Point, uchar>[8]{
            std::tuple<cv::Point, int>(cv::Point(-1, -1), 1),
            std::tuple<cv::Point, int>(cv::Point(0, -1), 2),
            std::tuple<cv::Point, int>(cv::Point(1, -1), 4),
            std::tuple<cv::Point, int>(cv::Point(1, 0), 8),
            std::tuple<cv::Point, int>(cv::Point(1, 1), 16),
            std::tuple<cv::Point, int>(cv::Point(0, 1), 32),
            std::tuple<cv::Point, int>(cv::Point(-1, 1), 64),
            std::tuple<cv::Point, int>(cv::Point(-1, 0), 128)
    };

    std::vector<ushort> data(256);

    for (int x = 1; x < image.cols - 1; x++)
    {
        for (int y = 1; y < image.rows - 1; y++)
        {
            uchar middlePixel = image.at<uchar>(cv::Point(x, y));

            uchar finalValue = 0;

            for (auto i = 0; i < 8; i++)
            {
                auto pos = offsetsMultipliers[i];

                cv::Point offsetPoint = std::get<0>(pos);
                uchar multiplier = std::get<1>(pos);

                uchar pixel = image.at<uchar>(cv::Point(x, y) + offsetPoint);

                finalValue += (pixel >= middlePixel ? 1 : 0) * multiplier;
            }

            data[finalValue] = data[finalValue] + 1;
        }
    }

    return data;
}

std::vector<MaskedLBPModel> MaskedLBPModel::loadFromFile(fs::path &filePath)
{
    std::vector<MaskedLBPModel> result;

    std::ifstream inputFileStream;
    inputFileStream.open(filePath);

    while (inputFileStream.good())
    {
        MaskedType maskedType;
        std::vector<ushort> data(256);
        std::string typeString;
        std::string dataString;

        getline(inputFileStream, typeString, ',');
        if (typeString.empty())
        {
            break;
        }
        maskedType = (MaskedType) std::stoi(typeString);

        for (int i = 0; i < 255; i++)
        {
            getline(inputFileStream, dataString, ',');
            data[i] = (ushort) std::stoi(dataString);
        }

        getline(inputFileStream, dataString);
        data[255] = (ushort) std::stoi(dataString);

        result.emplace_back(MaskedLBPModel(maskedType, data));
    }

    inputFileStream.close();

    return result;
}
