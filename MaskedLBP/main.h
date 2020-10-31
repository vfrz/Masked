#pragma once

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <limits>

enum Masked
{
    Unknown = -1,
    Bad = 0,
    Good = 1
};

class MaskedModel
{
private:
    Masked _masked;
    ushort *_data;
public:
    MaskedModel(Masked masked, ushort *data)
    {
        _masked = masked;
        _data = data;
    }

    Masked getMasked()
    {
        return _masked;
    }

    ushort getData(int i)
    {
        return _data[i];
    }

    ushort *getData()
    {
        return _data;
    }
};

void process(int argc, char *argv[]);

void compare(int argc, char *argv[]);

float getDifference(MaskedModel imageModel, MaskedModel compared, std::string algorithm);

MaskedModel readLBP(std::string imageFile, Masked masked);

std::vector<MaskedModel> parseModelFile(std::string file);

void displayColorigram(ushort colorigram[]);