#include <iostream>
#include <filesystem>
#include "Predictor.h"
#include "Trainer.h"
#include "include/CLI11.hpp"

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Missing argument(s): command and dataset path, usage: Masked train/predict \"dataset_path\""
                  << std::endl;
        return 1;
    }

	CLI::App app{"Masked"};
	app.require_subcommand();

    auto trainCommand = app.add_subcommand("train");
    auto predictCommand = app.add_subcommand("predict");

    std::string datasetPathString = "dataset";
    trainCommand->add_option("-d,--dataset", datasetPathString, "Dataset directory path, default set to 'dataset/'");
	predictCommand->add_option("-d,--dataset", datasetPathString, "Dataset directory path, default set to 'dataset/'");

	bool pyramid = false;
	trainCommand->add_flag("-p,--pyramid", pyramid);
	predictCommand->add_flag("-p,--pyramid", pyramid);

	CLI11_PARSE(app, argc, argv);

    fs::path datasetPath = fs::path(fs::current_path()).append(datasetPathString);

    if (trainCommand->parsed())
    {
        Trainer trainer(datasetPath, pyramid);
        return trainer.Train();
    }
    else if (predictCommand->parsed())
    {
        Predictor predictor(datasetPath, pyramid);
        return predictor.Predict();
    }
    else
    {
        std::cout << "Unknown command" << std::endl;
        return 1;
    }
}
