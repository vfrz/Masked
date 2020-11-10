#include "Trainer.h"

Trainer::Trainer(fs::path &datasetPath)
{
    _datasetPath = datasetPath;
}

int Trainer::Train()
{
    auto path = fs::path("/home/vfrz/Documents/Projects/Masked/dataset/GRAY/1TRAIN/CMFD/00001_Mask.jpg");

    auto model = MaskedLBPModel::loadFromFile(path, MaskedType::Good);

    std::cout << model.getData(1) << std::endl;

    return 0;
}