#pragma once

#include <iostream>
#include <fstream>
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

    std::vector<ushort> &getData();

    static std::vector<ushort> getLBP(cv::Mat &image);

    static MaskedLBPModel computeFromImageFile(fs::path &filePath, MaskedType maskedType);

    static std::vector<MaskedLBPModel> loadFromFile(fs::path &filePath);
};
