#include "DecisionTree.h"
#include <iostream>

// this is the following command that I use to run my program...g++ -o decision_tree src/main.cpp src/DecisionTree.cpp src/CsvUtils.cpp -std=c++17

int main()
{
    DecisionTree tree;

    // Train on the training dataset
    tree.train("data/TrainingData.csv");

    // Output the decision tree structure
    tree.outputTreeStructure("output/DecisionTreeStructure.txt");

    // Classify the test data and output predictions
    tree.outputTestPredictions("data/TestingData-1.csv", "output/ClassifiedTestData.csv");

    std::cout << "Decision tree training and classification completed." << std::endl;
    return 0;
}
