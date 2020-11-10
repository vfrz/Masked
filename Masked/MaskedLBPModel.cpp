//
// Created by vfrz on 11/10/20.
//

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

ushort MaskedLBPModel::getData(int i)
{
    return _data[i];
}

MaskedLBPModel MaskedLBPModel::loadFromFile(fs::path &filePath, MaskedType maskedType)
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
            std::tuple<cv::Point, int>(cv::Point(-1, 0), 8),
            std::tuple<cv::Point, int>(cv::Point(1, 0), 16),
            std::tuple<cv::Point, int>(cv::Point(-1, 1), 32),
            std::tuple<cv::Point, int>(cv::Point(0, 1), 64),
            std::tuple<cv::Point, int>(cv::Point(1, 1), 128)
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

                cv::Point offsetPoint = get<0>(pos);
                uchar multiplier = get<1>(pos);

                uchar pixel = image.at<uchar>(cv::Point(x, y) + offsetPoint);

                finalValue += (pixel >= middlePixel ? 1 : 0) * multiplier;
            }

            data[finalValue] = data[finalValue] + 1;
        }
    }

    return data;
}