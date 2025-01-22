#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include <vector>
#include <string>
#include <map>

// DecisionTree class handles the training, building, and classification of a decision tree
class DecisionTree
{
public:
    // Structure for each node in the decision tree
    struct Node
    {
        std::string feature;                    // Feature used to split at this node
        std::string label;                      // Label if this is a leaf node
        std::map<std::string, Node *> children; // Map of child nodes for each feature value
        bool isLeaf = false;                    // Flag to indicate if the node is a leaf
    };

    DecisionTree();                                                                             // Constructor to initialize the tree
    ~DecisionTree();                                                                            // Destructor to free memory
    void train(const std::string &trainingDataFile);                                            // Train the tree using a training dataset
    std::string classify(const std::map<std::string, std::string> &instance);                   // Classify a single instance
    void printTree(Node *node, std::ofstream &out, int depth = 0);                              // Print the tree structure to a file
    void outputTreeStructure(const std::string &outputFile);                                    // Output the tree structure to a file
    void outputTestPredictions(const std::string &testDataFile, const std::string &outputFile); // Predict and output results for the test dataset

private:
    Node *root;                                                                                                                 // Root of the decision tree
    double calculateEntropy(const std::vector<std::map<std::string, std::string>> &subset);                                     // Calculate entropy of a subset
    double calculateInformationGain(const std::vector<std::map<std::string, std::string>> &subset, const std::string &feature); // Calculate information gain for a feature
    Node *buildTree(const std::vector<std::map<std::string, std::string>> &subset, const std::vector<std::string> &features);   // Recursive function to build the tree
    void deleteTree(Node *node);                                                                                                // Recursive function to delete nodes and free memory
};

#endif
