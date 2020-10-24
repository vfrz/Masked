# Masked

## MaskedLBP

### Usage

`MaskedLBP [command] {args}`

#### Process command

`MaskedLBP process {inputDirectory} {outputFile}`

`MaskedLBP process "../dataset/GRAY/1TRAIN/CMFD" "1TRAIN_CMFD.txt"`

#### Compare command

`MaskedLBP compare {inputFile} {inputDirectory} {compareAlgorithm} {outputFile}`

`MaskedLBP compare "1TRAIN_CMFD.txt" "../dataset/GRAY/1TEST/CMFD" "DIST_EUCLIDIAN" "1TRAIN_CMFD_RESULTS.txt"`

## MaskedConverter

### Usage

...