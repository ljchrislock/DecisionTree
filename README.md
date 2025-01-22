
# Decision Tree Implementation

## Overview

This project implements a decision tree algorithm for classification tasks. It processes training and testing datasets to classify input data and outputs the decision tree structure and classified test results.

---

## File Structure

### Source Code
- **`src/main.cpp`**: Entry point of the program.
- **`src/CsvUtils.cpp` & `CsvUtils.h`**: Utilities for handling CSV file input/output.
- **`src/DecisionTree.cpp` & `DecisionTree.h`**: Implementation of the decision tree algorithm.

### Data Files
- **`data/TrainingData.csv`**: Training dataset for building the decision tree.
- **`data/TestingData-1.csv`**: Testing dataset for classification.

### Output Files
- **`output/ClassifiedTestData.csv`**: Contains the classified results of the test dataset.
- **`output/DecisionTreeStructure.txt`**: Text representation of the generated decision tree structure.

### Executable
- **`decision_tree.exe`**: Precompiled binary for running the program.

### Configuration Files
- **`.vscode/c_cpp_properties.json`**: Configuration for C++ IntelliSense.
- **`.vscode/launch.json`**: Debugger settings.
- **`.vscode/settings.json`**: VSCode project settings.

---

## Setup and Compilation

### Prerequisites
- A C++ compiler supporting C++17 or later (e.g., `g++`).
- VSCode or a similar IDE (optional).

### Steps
1. Navigate to the `src` directory.
2. Compile the program:
   ```bash
   g++ -o decision_tree main.cpp CsvUtils.cpp DecisionTree.cpp
   ```
3. Run the program:
   ```bash
   ./decision_tree
   ```

---

## Input and Output

### Input
- **Training Dataset**: Specify in `data/TrainingData.csv`.
- **Testing Dataset**: Specify in `data/TestingData-1.csv`.

### Output
1. **Classified Test Data**: Saved in `output/ClassifiedTestData.csv`.
2. **Decision Tree Structure**: Saved in `output/DecisionTreeStructure.txt`.

---

## Contribution

Feel free to fork the repository, suggest improvements, or report issues.
