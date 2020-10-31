#include "main.h"

namespace fs = std::filesystem;

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "MaskedLBP requires a command paramater" << std::endl;
        return 1;
    }

    if (argv[1] == std::string("process"))
    {
        process(argc, argv);
    } else if (argv[1] == std::string("compare"))
    {
        compare(argc, argv);
    } else
    {
        std::cout << "Unknown command: " << argv[1] << std::endl;
    }

    return 0;
}

void processDirectory(std::string inputDirectory, Masked masked, std::string outputFile)
{
    ofstream stream;
    stream.open(outputFile, std::ios_base::app);

    for (const auto &entry : fs::directory_iterator(inputDirectory))
    {
        cout << "Processing image: " << entry.path().filename() << endl;

        auto inputImagePath = samples::findFile(entry.path());

        auto model = readLBP(inputImagePath, masked);
        auto data = model.getData();

        stream << masked << " ";

        for (auto i = 0; i < 256; i++)
        {
            stream << data[i];
            if (i < 255)
                stream << " ";
        }

        stream << std::endl;
    }

    stream.close();
}

void process(int argc, char *argv[])
{
    // Well masked
    std::string cmfdInputDirectory = argv[2];
    // Bad masked
    std::string imfdInputDirectory = argv[3];

    std::string outputFile = argv[4];

    processDirectory(cmfdInputDirectory, Masked::Good, outputFile);
    processDirectory(imfdInputDirectory, Masked::Bad, outputFile);
}

void compareDirectory(std::string modelFile, std::string inputDirectory, std::string algorithm, std::string outputFile, Masked expectedMasked)
{
    auto models = parseModelFile(modelFile);

    std::cout << models[0].getData(0) << std::endl;

    ofstream stream;
    stream.open(outputFile, std::ios_base::app);

    for (const auto &entry : fs::directory_iterator(inputDirectory))
    {
        cout << "Comparing image: " << entry.path().filename() << endl;

        auto imageModel = readLBP(entry.path(), Masked::Unknown);

        float minDiff = 8000000;
        Masked currentMasked = Masked::Unknown;
        std::cout << models[0].getData(0) << std::endl;

        for (auto i = 0; i < models.size(); i++)
        {
            std::cout << models[0].getData(0) << std::endl;
            return;

            float diff = getDifference(imageModel, models[i], algorithm);
            //std::cout << "MinDiff: " << to_string(minDiff) << " Diff: " << to_string(diff) << std::endl;
            if (diff < minDiff)
            {
                std::cout << diff << " " << i << std::endl;
                currentMasked = models[i].getMasked();
                minDiff = diff;
            }
            i++;
        }

        return;

        stream << (int) currentMasked << " " << (int) expectedMasked << " " << entry.path().filename() << endl;
    }

    stream.close();
}

void compare(int argc, char *argv[])
{
    std::string modelFile = argv[2];
    std::string cmfdInputDirectory = argv[3];
    std::string imfdInputDirectory = argv[4];
    std::string algorithm = argv[5];
    std::string outputFile = argv[6];

    compareDirectory(modelFile, cmfdInputDirectory, algorithm, outputFile, Masked::Good);
    compareDirectory(modelFile, imfdInputDirectory, algorithm, outputFile, Masked::Bad);
}

MaskedModel readLBP(std::string imageFile, Masked masked)
{
    auto inputImage = imread(imageFile, IMREAD_GRAYSCALE);

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

    auto data = new ushort[256];

    for (auto i = 0; i < 256; i++)
    {
        data[i] = 0;
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

            data[finalValue] = data[finalValue] + 1;
        }
    }

    return MaskedModel(masked, data);
}

std::vector<MaskedModel> parseModelFile(std::string modelFile)
{
    ifstream stream;
    stream.open(modelFile);

    auto result = std::vector<MaskedModel>();

    while (!stream.eof())
    {
        int masked;
        stream >> masked;

        ushort data[256] = {};
        for (auto i = 0; i < 256; i++)
        {
            ushort temp;
            stream >> temp;
            data[i] = temp;
        }

        MaskedModel model((Masked) masked, data);

        /*std::cout << data[0] << std::endl;
        std::cout << data[1] << std::endl;*/

        result.emplace_back(model);

        break;
    }

    return result;
}

float getSAD(MaskedModel imageModel, MaskedModel compared)
{
    float total = 0;

    auto imageData = imageModel.getData();
    auto comparedData = compared.getData();

    for (auto i = 0; i < 256; i++)
    {
        total += (float) abs(imageData[i] - comparedData[i]);
    }

    return total;
}

float getDifference(MaskedModel imageModel, MaskedModel compared, std::string algorithm)
{
    if (algorithm == "SAD")
    {
        return getSAD(imageModel, compared);
    }

    std::cout << "Unknown compare algorithm: " + algorithm << std::endl;
    throw;
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
