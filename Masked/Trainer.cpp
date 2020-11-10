#include "Trainer.h"

Trainer::Trainer(fs::path &datasetPath)
{
    _datasetPath = datasetPath;
}

int Trainer::Train()
{
    /*auto path = fs::path("/home/vfrz/Documents/Projects/Masked/dataset/GRAY/1TRAIN/CMFD/00001_Mask.jpg");

    auto model = MaskedLBPModel::computeFromImageFile(path, MaskedType::Good);

    std::cout << model.getData(1) << std::endl;*/

    auto path = fs::path("/home/vfrz/Documents/Projects/Masked/test.lbp");

    auto models = MaskedLBPModel::loadFromFile(path);

    std::cout << models[0].getData(255) << std::endl;

    return 0;
}