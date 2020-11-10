#include <iostream>
#include <filesystem>
#include "Predictor.h"
#include "Trainer.h"

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Missing argument(s): command and dataset path, usage: Masked train/predict \"dataset_path\""
                  << std::endl;
        return 1;
    }

    std::string command = argv[1];
    fs::path datasetPath = fs::current_path().append(argv[2]);

    if (command == "train")
    {
        Trainer trainer(datasetPath);
        return trainer.Train();
    }
    else if (command == "predict")
    {
        Predictor predictor(datasetPath);
        return predictor.Predict();
    }
    else
    {
        std::cout << "Unknown command: " + command << std::endl;
        return 1;
    }
}
