#include "main.h"

namespace fs = std::filesystem;

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "MaskedLBP requires a command paramater" << std::endl;
        return 1;
    }

    if (argv[1] == std::string("process"))
    {
        process(argv[2], argv[3]);
    } else if (argv[1] == std::string("compare"))
    {
        compare(argv[2], argv[3]);
    } else {
        std::cout << "Unknown command: " << argv[1] << std::endl;
    }

    return 0;
}

void process(std::string inputDirectory, std::string outputFile)
{
    auto positions = new tuple<Point, uchar>[8]{
            tuple<Point, int>(Point(-1, -1), 1),
            tuple<Point, int>(Point(0, -1), 2),
            tuple<Point, int>(Point(1, -1), 4),
            tuple<Point, int>(Point(-1, 0), 8),
            tuple<Point, int>(Point(1, 0), 16),
            tuple<Point, int>(Point(-1, 1), 32),
            tuple<Point, int>(Point(0, 1), 64),
            tuple<Point, int>(Point(1, 1), 128)
    };

    ofstream stream;
    stream.open(outputFile);
    stream.clear();

    for (const auto &entry : fs::directory_iterator(inputDirectory))
    {

        cout << "Processing image: " << entry.path().filename() << endl;

        auto inputImagePath = samples::findFile(entry.path());
        auto inputImage = imread(inputImagePath, IMREAD_GRAYSCALE);

        //Mat outputImage = Mat(inputImage.rows - 2, inputImage.cols - 2, CV_8U);

        auto colorigram = new ushort[256];

        for (auto i = 0; i < 256; i++)
        {
            colorigram[i] = 0;
        }

        for (auto x = 1; x < inputImage.cols - 1; x++)
        {
            for (auto y = 1; y < inputImage.rows - 1; y++)
            {

                auto middlePixel = inputImage.at<uchar>(Point(x, y));

                uchar finalValue = 0;

                for (auto i = 0; i < 8; i++)
                {
                    auto pos = positions[i];

                    auto p = get<0>(pos);
                    auto m = get<1>(pos);

                    auto pixel = inputImage.at<uchar>(Point(x, y) + p);

                    finalValue += (pixel >= middlePixel ? 1 : 0) * m;
                }

                colorigram[finalValue] = colorigram[finalValue] + 1;
                //outputImage.at<uchar>(Point(x, y)) = finalValue;
            }
        }

        for (auto i = 0; i < 256; i++)
        {
            stream << colorigram[i];
            if (i < 255)
                stream << ",";
        }

        stream << std::endl;

        //displayColorigram(colorigram);

        //imshow("debug", outputImage);

        //waitKey();
    }

    stream.close();
}

void compare(std::string inputFile, std::string inputDirectory)
{

}

void displayColorigram(ushort colorigram[])
{

    auto total = 0;

    for (auto i = 0; i < 256; i++)
    {
        cout << "Value at " << i << " is " << to_string(colorigram[i]) << endl;

        total += colorigram[i];
    }

    cout << "Total: " << total << endl;
}
