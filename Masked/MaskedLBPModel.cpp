#include "MaskedLBPModel.h"

MaskedLBPModel::MaskedLBPModel(MaskedType maskedType, std::vector<int>& data)
{
    _maskedType = maskedType;
    _data = data;
}

MaskedType MaskedLBPModel::getMaskedType()
{
    return _maskedType;
}

std::vector<int>& MaskedLBPModel::getData()
{
    return _data;
}

void MaskedLBPModel::appendData(std::vector<int>& data)
{
    for (int i = 0; i < _data.size(); ++i)
    {
        _data[i] += data[i];
    }
}

MaskedLBPModel MaskedLBPModel::computeFromImage(cv::Mat image, MaskedType maskedType, bool pyramid)
{
    std::vector<int> data = getLBP(image);

    if (pyramid) {
        for (int i = 2; i <= 3; i++)
        {
            auto width = image.cols / i;
            auto height = image.rows / i;

            for (int x = 0; x < i; x++)
            {
                for (int y = 0; y < i; y++)
                {
                    cv::Mat sub = cv::Mat(image, cv::Rect(x * width, y * height, width, height));

                    auto subLBP = getLBP(sub);

                    for (int j = 0; j < subLBP.size(); ++j)
                        data.emplace_back(subLBP[j]);
                }
            }
        }
    }

    return MaskedLBPModel(maskedType, data);
}

std::vector<int> MaskedLBPModel::getLBP(cv::Mat& image)
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

    std::vector<int> data(256);

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

std::vector<MaskedLBPModel> MaskedLBPModel::loadFromFile(fs::path& filePath, bool pyramid)
{
    std::vector<MaskedLBPModel> result;

    std::ifstream inputFileStream;
    inputFileStream.open(filePath);

    while (inputFileStream.good())
    {
        MaskedType maskedType;
        std::vector<int> data(pyramid ? 256 * 14 : 256);
        std::string typeString;
        std::string dataString;

        getline(inputFileStream, typeString, ',');
        if (typeString.empty())
        {
            break;
        }
        maskedType = (MaskedType)std::stoi(typeString);

        for (int i = 0; i < data.size() - 1; i++)
        {
            getline(inputFileStream, dataString, ',');
            data[i] = (ushort)std::stoi(dataString);
        }

        getline(inputFileStream, dataString);
        data[data.size() - 1] = (ushort)std::stoi(dataString);

        result.emplace_back(MaskedLBPModel(maskedType, data));
    }

    inputFileStream.close();

    return result;
}