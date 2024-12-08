#ifndef INPUT_DATA_H
#define INPUT_DATA_H

#include <vector>
#include <tuple>
#include <fstream>
#include <iostream>

using namespace std;

// Function to read input data from a file
vector<tuple<double, double, double, double, double>> readInputData(const string& filename) {
    vector<tuple<double, double, double, double, double>> data;
    ifstream inputFile(filename);

    if (!inputFile) {
        cerr << "Error: Could not open input file!" << endl;
        return data; // Return empty vector
    }

    double S, K, T, r, sigma;
    while (inputFile >> S >> K >> T >> r >> sigma) {
        data.emplace_back(S, K, T, r, sigma);
    }

    inputFile.close();
    return data;
}

#endif // INPUT_DATA_H