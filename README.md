# Masked

## MaskedLBP

### Usage

`MaskedLBP [command] {args}`

/!\ You need to put the dataset folder at the root before using it.

#### Process command

`MaskedLBP process {cmfdInputDirectory} {imfdInputDirectory} {outputFile}`

`MaskedLBP process "../dataset/GRAY/1TRAIN/CMFD" "../dataset/GRAY/1TRAIN/IMFD" "1TRAIN.txt"`

#### Compare command

`MaskedLBP compare {inputFile} {inputDirectory} {compareAlgorithm} {outputFile}`

`MaskedLBP compare "1TRAIN_CMFD.txt" "../dataset/GRAY/1TEST/CMFD" "DIST_EUCLIDIAN" "1TRAIN_CMFD_RESULTS.txt"`

## MaskedConverter

### Usage

...