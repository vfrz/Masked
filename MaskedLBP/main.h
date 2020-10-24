#pragma once

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <filesystem>

void process(std::string inputDirectory, std::string outputFile);

void compare(std::string inputFile, std::string inputDirectory);

void displayColorigram(ushort colorigram[]);