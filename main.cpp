#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip> 

using namespace std;

// Abstract Base Class for Options
class BaseOption {
protected:
    double S;     
    double K;     
    double T;     
    double r;     
    double sigma; 

public:
    BaseOption(double S, double K, double T, double r, double sigma)
        : S(S), K(K), T(T), r(r), sigma(sigma) {}

    virtual double calculatePrice() const = 0; 
};

// Derived Class for Call Options
class CallOption : public BaseOption {
public:
    CallOption(double S, double K, double T, double r, double sigma)
        : BaseOption(S, K, T, r, sigma) {}

    double calculatePrice() const override {
        double d1 = (log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
        double d2 = d1 - sigma * sqrt(T);

        return S * normCDF(d1) - K * exp(-r * T) * normCDF(d2);
    }

private:
    double normCDF(double x) const {
        return 0.5 * erfc(-x * sqrt(0.5));
    }
};

// Derived Class for Put Options
class PutOption : public BaseOption {
public:
    PutOption(double S, double K, double T, double r, double sigma)
        : BaseOption(S, K, T, r, sigma) {}

    double calculatePrice() const override {
        double d1 = (log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
        double d2 = d1 - sigma * sqrt(T);

        return K * exp(-r * T) * normCDF(-d2) - S * normCDF(-d1);
    }

private:
    double normCDF(double x) const {
        return 0.5 * erfc(-x * sqrt(0.5));
    }
};

// Black-Scholes Model
class BlackScholesModel {
public:
    double priceOption(const BaseOption& option) {
        return option.calculatePrice();
    }
};

// Function to read input data from a file
vector<tuple<double, double, double, double, double>> readInputData(const string& filename) {
    vector<tuple<double, double, double, double, double>> data;
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return data;
    }

    string line;
    while (getline(inputFile, line)) {
        istringstream iss(line);
        double S, K, T, r, sigma;
        if (iss >> S >> K >> T >> r >> sigma) {
            data.emplace_back(S, K, T, r, sigma);
        }
    }
    inputFile.close();
    return data;
}

// Main Function
int main() {
    const string filename = "input.txt";
    vector<tuple<double, double, double, double, double>> inputData = readInputData(filename);

    // Check if input data was successfully read
    if (inputData.empty()) {
        cerr << "No data to process. Exiting program." << endl;
        return 1;
    }

    BlackScholesModel bsModel;
    vector<double> callPrices;
    vector<double> putPrices;

    cout << fixed << setprecision(2); // Format output to 2 decimal places

    // Process each record
    for (size_t i = 0; i < inputData.size(); ++i) {
        double S, K, T, r, sigma;
        tie(S, K, T, r, sigma) = inputData[i];

        // Create Call and Put Option objects
        CallOption callOption(S, K, T, r, sigma);
        PutOption putOption(S, K, T, r, sigma);

        // Calculate prices
        double callPrice = bsModel.priceOption(callOption);
        double putPrice = bsModel.priceOption(putOption);

        callPrices.push_back(callPrice);
        putPrices.push_back(putPrice);

        // Display the calculated prices
        cout << "Option " << i + 1 << " Call Price: $" << callPrice
             << " | Put Price: $" << putPrice << endl;
    }

    return 0;
}
