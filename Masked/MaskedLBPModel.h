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
    std::vector<int> _data;
public:
    MaskedLBPModel(MaskedType maskedType, std::vector<int>& data);

    MaskedType getMaskedType();

    std::vector<int>& getData();

    void appendData(std::vector<int>& data);

    static std::vector<int> getLBP(cv::Mat& image);

    static MaskedLBPModel computeFromImage(cv::Mat image, MaskedType maskedType, bool pyramid = false);

    static std::vector<MaskedLBPModel> loadFromFile(fs::path& filePath, bool pyramid = false);
};
