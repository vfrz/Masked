#include "MaskedTypeHelper.h"

std::string MaskedTypeHelper::toString(MaskedType maskedType)
{
    switch (maskedType)
    {
        case Unknown:
            return "Unknown";
        case Bad:
            return "Bad";
        case Good:
            return "Good";
        default:
            return "";
    };
}
