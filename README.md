# Masked

## Compilation

...

## Usage

...

## Current results (may be wrong)

| Algorithm | MaskedType | Ratio |
|-----------|------------|-------|
| SAD | Good | 66.84% |
| SAD | Bad | 61.14% |
| ChiSquare | Good | 62.26% |
| ChiSquare | Bad | 64.82% |
| Bhattacharyya | Good | 67.22% |
| Bhattacharyya | Bad | 62.76% |
| Intersection | Good | 58.86% |
| Intersection | Bad | 27.26% |
| Correlation | Good | 14.76% |
| Correlation | Bad | 77.34% |

### Raw logs
```
===== Test results =====
Algorithm: SAD
Masked: Good
Good: 3342
Bad: 1658
Total: 5000
Ratio: 0.6684
Elapsed time in seconds: 78.3305
===== Test results =====
Algorithm: SAD
Masked: Bad
Good: 3057
Bad: 1943
Total: 5000
Ratio: 0.6114
Elapsed time in seconds: 76.4814
===== Test results =====
Algorithm: ChiSquare
Masked: Bad
Good: 3241
Bad: 1759
Total: 5000
Ratio: 0.6482
Elapsed time in seconds: 171.166
===== Test results =====
Algorithm: ChiSquare
Masked: Good
Good: 3113
Bad: 1887
Total: 5000
Ratio: 0.6226
Elapsed time in seconds: 172.032
===== Test results =====
Algorithm: Bhattacharyya
Masked: Good
Good: 3361
Bad: 1639
Total: 5000
Ratio: 0.6722
Elapsed time in seconds: 175.152
===== Test results =====
Algorithm: Intersection
Masked: Bad
Good: 1363
Bad: 3637
Total: 5000
Ratio: 0.2726
Elapsed time in seconds: 187.095
===== Test results =====
Algorithm: Intersection
Masked: Good
Good: 2943
Bad: 2057
Total: 5000
Ratio: 0.5886
Elapsed time in seconds: 189.114
===== Test results =====
Algorithm: Correlation
Masked: Bad
Good: 3867
Bad: 1133
Total: 5000
Ratio: 0.7734
Elapsed time in seconds: 216.606
===== Test results =====
Algorithm: Correlation
Masked: Good
Good: 738
Bad: 4262
Total: 5000
Ratio: 0.1476
Elapsed time in seconds: 218.078
===== Test results =====
Algorithm: Bhattacharyya
Masked: Bad
Good: 3138
Bad: 1862
Total: 5000
Ratio: 0.6276
Elapsed time in seconds: 167.785
```