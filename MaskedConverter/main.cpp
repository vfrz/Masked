#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>

namespace fs = std::filesystem;

using namespace cv;
using namespace std;

int main(int argc, char *argv[]) {

    if (argc != 3) {
        cout << "Wrong number of arguments, first is the input directory path and second is the output directory path." << endl;
        return 1;
    }

    auto inputPath = argv[1];
    auto outputPath = argv[2];

    auto i = 0;

    for (const auto & entry : fs::directory_iterator(inputPath)) {

        cout << "Processing image: " << entry.path().filename() << endl;

        auto inputImagePath = samples::findFile(entry.path());
        auto inputImage = imread(inputImagePath, IMREAD_COLOR);

        Mat outputImage;

        cv::resize(inputImage, outputImage, cv::Size(), 0.1, 0.1, cv::INTER_LINEAR);
        cv::cvtColor(outputImage, outputImage, cv::COLOR_RGB2GRAY);

        auto outputImagePath = outputPath + entry.path().filename().string();

        imwrite(outputImagePath, outputImage);
        i++;
    }

    cout << "Processed images: " << i << endl;

    return 0;
}
