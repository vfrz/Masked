#pragma once

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <filesystem>

enum Masked {
    Bad = 0,
    Good = 1
};

void process(int argc, char *argv[]);

void compare(std::string inputFile, std::string inputDirectory, std::string algo, std::string outputFile);

void displayColorigram(ushort colorigram[]);