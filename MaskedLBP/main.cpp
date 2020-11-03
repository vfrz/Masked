#include "main.h"

namespace fs = std::filesystem;

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
    std::ofstream stream;
    stream.open(outputFile, std::ios_base::app);

    for (const auto &entry : fs::directory_iterator(inputDirectory))
    {
        std::cout << "Processing image: " << entry.path().filename() << std::endl;

        auto inputImagePath = cv::samples::findFile(entry.path());

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

void compareDirectory(std::string modelFile, std::string inputDirectory, std::string algorithm, std::string outputFile,
                      Masked expectedMasked)
{
    auto models = parseModelFile(modelFile);

    std::ofstream stream;
    stream.open(outputFile, std::ios_base::app);

    for (const auto &entry : fs::directory_iterator(inputDirectory))
    {
        std::cout << "Comparing image: " << entry.path().filename() << std::endl;

        auto imageModel = readLBP(entry.path(), Masked::Unknown);

        float minDiff = 8000000;
        Masked currentMasked = Masked::Unknown;

        for (auto i = 0; i < models.size(); i++)
        {
            float diff = getDifference(imageModel, models[i], algorithm);
            if (diff < minDiff)
            {
                currentMasked = models[i].getMasked();
                minDiff = diff;
            }
        }

        stream << (int) currentMasked << " " << (int) expectedMasked << " " << entry.path().filename() << std::endl;
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
    auto inputImage = imread(imageFile, cv::IMREAD_GRAYSCALE);

    auto positions = new std::tuple<cv::Point, uchar>[8]{
            std::tuple<cv::Point, int>(cv::Point(-1, -1), 1),
            std::tuple<cv::Point, int>(cv::Point(0, -1), 2),
            std::tuple<cv::Point, int>(cv::Point(1, -1), 4),
            std::tuple<cv::Point, int>(cv::Point(-1, 0), 8),
            std::tuple<cv::Point, int>(cv::Point(1, 0), 16),
            std::tuple<cv::Point, int>(cv::Point(-1, 1), 32),
            std::tuple<cv::Point, int>(cv::Point(0, 1), 64),
            std::tuple<cv::Point, int>(cv::Point(1, 1), 128)
    };

    auto data = std::vector<ushort>(256);

    for (auto x = 1; x < inputImage.cols - 1; x++)
    {
        for (auto y = 1; y < inputImage.rows - 1; y++)
        {
            auto middlePixel = inputImage.at<uchar>(cv::Point(x, y));

            uchar finalValue = 0;

            for (auto i = 0; i < 8; i++)
            {
                auto pos = positions[i];

                auto p = get<0>(pos);
                auto m = get<1>(pos);

                auto pixel = inputImage.at<uchar>(cv::Point(x, y) + p);

                finalValue += (pixel >= middlePixel ? 1 : 0) * m;
            }

            data[finalValue] = data[finalValue] + 1;
        }
    }

    return MaskedModel(masked, data);
}

std::vector<MaskedModel> parseModelFile(std::string modelFile)
{
    std::ifstream stream;
    stream.open(modelFile);

    auto result = std::vector<MaskedModel>();

    while (!stream.eof())
    {
        int masked;
        stream >> masked;

        auto data = std::vector<ushort>(256);
        for (auto i = 0; i < 256; i++)
        {
            ushort temp;
            stream >> temp;
            data[i] = temp;
        }

        MaskedModel model((Masked) masked, data);

        result.emplace_back(model);
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
        std::cout << "Value at " << i << " is " << std::to_string(colorigram[i]) << std::endl;

        total += colorigram[i];
    }

    std::cout << "Total: " << total << std::endl;
}
