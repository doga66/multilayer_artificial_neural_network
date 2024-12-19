#pragma once
#include <vector>

class Perceptron {
public:
    Perceptron(int input_size);  // Constructor
    void train(std::vector<std::vector<int>> training_inputs, std::vector<std::vector<int>> training_outputs, int epochs, double learning_rate);
    int predict(std::vector<int> input);

private:
    std::vector<double> weights;
    double bias;
    double activation_function(double sum);
};
