#include "DecisionTree.h"
#include "CsvUtils.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <functional> 

// Constructor initializes the root of the tree as nullptr
DecisionTree::DecisionTree() : root(nullptr) {}

// Destructor to free memory allocated for the tree
DecisionTree::~DecisionTree()
{
    deleteTree(root);
}

// Function to train the decision tree from a CSV file
void DecisionTree::train(const std::string &trainingDataFile)
{
    // Load data from the CSV file
    std::vector<std::map<std::string, std::string>> data = CsvUtils::readCSV(trainingDataFile);

    // Filter out instances without the "Decision" label
    std::vector<std::map<std::string, std::string>> filteredData;
    for (const auto &instance : data)
    {
        if (instance.find("Decision") != instance.end())
        {                                     // Check if "Decision" label is present
            filteredData.push_back(instance); // Only add instances that have the "Decision" label
        }
        else
        {
            std::cout << "Warning: Skipping instance without 'Decision' label in training data." << std::endl;
        }
    }

    // Extract feature names, excluding "ID" and "Decision"
    std::vector<std::string> features;
    if (!filteredData.empty())
    {
        for (const auto &pair : filteredData[0])
        {
            if (pair.first != "ID" && pair.first != "Decision")
            {
                features.push_back(pair.first);
            }
        }
    }

    // Build the decision tree using the filtered data
    root = buildTree(filteredData, features);
}

// Classify a single instance by traversing the decision tree
std::string DecisionTree::classify(const std::map<std::string, std::string> &instance)
{
    Node *current = root;

    // Traverse the tree based on the features present in the instance
    while (!current->isLeaf)
    {
        // Check if the feature exists in the instance
        auto featureIt = instance.find(current->feature);
        if (featureIt == instance.end())
        {
            // If the feature is missing in the instance, return a default classification
            std::cout << "Warning: Feature '" << current->feature << "' is missing in the instance during classification." << std::endl;
            return "Unknown"; // Default to "Unknown" if any feature is missing
        }

        std::string featureValue = featureIt->second; // Safely access the feature value

        // Check if the child node for the feature value exists in the tree
        auto childIt = current->children.find(featureValue);
        if (childIt != current->children.end())
        {
            current = childIt->second; // Move to the child node
        }
        else
        {
            // If the specific feature value is missing in the decision tree, return a default classification
            std::cout << "Warning: Feature value '" << featureValue << "' not found in tree children for feature '" << current->feature << "'." << std::endl;
            return "Unknown"; // Default to "Unknown" if the feature value path is missing
        }
    }
    return current->label; // Return the label if a leaf node is reached
}

double DecisionTree::calculateEntropy(const std::vector<std::map<std::string, std::string>> &subset)
{
    std::map<std::string, int> labelCounts;

    // Count occurrences of each label in the subset
    for (const auto &instance : subset)
    {
        auto labelIt = instance.find("Decision");
        if (labelIt != instance.end())
        { // Check if "Decision" key exists
            labelCounts[labelIt->second]++;
        }
        else
        {
            std::cout << "Warning: 'Decision' label missing in instance during entropy calculation." << std::endl;
        }
    }

    double entropy = 0.0;
    for (const auto &labelCount : labelCounts)
    {
        double probability = static_cast<double>(labelCount.second) / subset.size();
        entropy -= probability * log2(probability); // Calculate entropy formula
    }
    return entropy;
}

double DecisionTree::calculateInformationGain(const std::vector<std::map<std::string, std::string>> &subset, const std::string &feature)
{
    double originalEntropy = calculateEntropy(subset); // Initial entropy of the subset

    // Partition data based on feature values
    std::map<std::string, std::vector<std::map<std::string, std::string>>> partitions;
    for (const auto &instance : subset)
    {
        auto featureIt = instance.find(feature);
        if (featureIt != instance.end())
        { // Check if feature key exists
            partitions[featureIt->second].push_back(instance);
        }
        else
        {
            std::cout << "Warning: Feature '" << feature << "' missing in instance during information gain calculation." << std::endl;
        }
    }

    // Calculate weighted entropy for each partition
    double weightedEntropy = 0.0;
    for (const auto &partition : partitions)
    {
        double partitionWeight = static_cast<double>(partition.second.size()) / subset.size();
        weightedEntropy += partitionWeight * calculateEntropy(partition.second);
    }

    // Information gain is the reduction in entropy
    return originalEntropy - weightedEntropy;
}

DecisionTree::Node *DecisionTree::buildTree(const std::vector<std::map<std::string, std::string>> &subset, const std::vector<std::string> &features)
{
    Node *node = new Node();

    // Check if all instances in the subset have the same label
    bool sameLabel = true;
    auto labelIt = subset[0].find("Decision");
    if (labelIt == subset[0].end())
    {
        std::cout << "Error: 'Decision' label missing in subset during tree building." << std::endl;
        return nullptr;
    }
    std::string firstLabel = labelIt->second;

    for (const auto &instance : subset)
    {
        auto labelIt = instance.find("Decision");
        if (labelIt == instance.end() || labelIt->second != firstLabel)
        {
            sameLabel = false;
            break;
        }
    }

    if (sameLabel)
    { // If all labels are the same, create a leaf node
        node->isLeaf = true;
        node->label = firstLabel;
        return node;
    }

    if (features.empty())
    { // If no features left, create a leaf node with the majority label
        node->isLeaf = true;
        node->label = firstLabel;
        return node;
    }

    // Find the feature with the highest information gain
    double bestGain = -1;
    std::string bestFeature;
    for (const auto &feature : features)
    {
        double gain = calculateInformationGain(subset, feature);
        if (gain > bestGain)
        {
            bestGain = gain;
            bestFeature = feature;
        }
    }

    node->feature = bestFeature;

    // Partition the subset by the best feature
    std::map<std::string, std::vector<std::map<std::string, std::string>>> partitions;
    for (const auto &instance : subset)
    {
        auto featureIt = instance.find(bestFeature);
        if (featureIt != instance.end())
        {
            partitions[featureIt->second].push_back(instance);
        }
        else
        {
            std::cout << "Warning: Missing feature '" << bestFeature << "' in instance during tree building." << std::endl;
        }
    }

    // Recursively build subtrees for each partition
    std::vector<std::string> remainingFeatures;
    for (const auto &feature : features)
    {
        if (feature != bestFeature)
            remainingFeatures.push_back(feature);
    }
    for (const auto &partition : partitions)
    {
        node->children[partition.first] = buildTree(partition.second, remainingFeatures);
    }
    return node;
}

// Function to output the tree structure to a file
void DecisionTree::outputTreeStructure(const std::string &outputFile)
{
    std::ofstream out(outputFile);
    printTree(root, out, 0); // Call helper function to print tree to file
}

// Helper function to recursively print each node
void DecisionTree::printTree(Node *node, std::ofstream &out, int depth)
{
    if (node == nullptr)
        return;
    if (node->isLeaf)
    {
        out << std::string(depth, '-') << " " << node->label << "\n";
    }
    else
    {
        out << std::string(depth, '-') << " " << node->feature << "\n";
        for (const auto &child : node->children)
        {
            out << std::string(depth + 1, '-') << " " << child.first << ":\n";
            printTree(child.second, out, depth + 2);
        }
    }
}

// Output predictions for the test dataset to a CSV file
void DecisionTree::outputTestPredictions(const std::string &testDataFile, const std::string &outputFile)
{
    // Load the original test data
    auto testData = CsvUtils::readCSV(testDataFile);

    // Open output file
    std::ofstream out(outputFile);

    // Write header: original columns plus "PredictedWillWait"
    bool headerWritten = false;
    for (const auto &instance : testData)
    {
        if (!headerWritten)
        {
            for (const auto &[key, _] : instance)
            {
                out << key << ","; // Write each original header
            }
            out << "PredictedWillWait\n"; // Add header for predicted column
            headerWritten = true;
        }

        // Write each row of test data, preserving original columns
        for (const auto &[key, value] : instance)
        {
            out << value << ",";
        }

        // Append prediction for each instance
        out << classify(instance) << "\n";
    }

    // Close file
    out.close();
}

// Recursive function to delete the tree and free memory
void DecisionTree::deleteTree(Node *node)
{
    if (node)
    {
        for (auto &child : node->children)
        {
            deleteTree(child.second);
        }
        delete node;
    }
}
