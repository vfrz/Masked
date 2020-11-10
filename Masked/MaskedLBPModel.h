#pragma once

#include <iostream>
#include <vector>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include "MaskedType.h"

namespace fs = std::filesystem;

class MaskedLBPModel
{
private:
    MaskedType _maskedType;
    std::vector<ushort> _data;
public:
    MaskedLBPModel(MaskedType maskedType, std::vector<ushort> &data);

    MaskedType getMaskedType();

    ushort getData(int i);

    static std::vector<ushort> getLBP(cv::Mat &image);

    static MaskedLBPModel loadFromFile(fs::path &filePath, MaskedType maskedType);
};
