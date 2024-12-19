#include "Source.h"
#include <iostream>

Perceptron::Perceptron(int input_size) {
    // Initialize weights and bias to 0
    weights.resize(input_size, 0);
    bias = 0;
}

double Perceptron::activation_function(double sum) {
    // Step activation function (0 or 1)
    return (sum >= 0) ? 1 : 0;
}

void Perceptron::train(std::vector<std::vector<int>> training_inputs, std::vector<std::vector<int>> training_outputs, int epochs, double learning_rate) {
    // Training the perceptron using the inputs and outputs
    for (int epoch = 0; epoch < epochs; ++epoch) {
        for (size_t i = 0; i < training_inputs.size(); ++i) {
            // Calculate the weighted sum
            double sum = 0;
            for (size_t j = 0; j < training_inputs[i].size(); ++j) {
                sum += training_inputs[i][j] * weights[j];
            }
            sum += bias;

            // Get the prediction (activation function)
            int predicted = activation_function(sum);

            // Update weights and bias if prediction is incorrect
            int error = training_outputs[i][0] - predicted;
            for (size_t j = 0; j < weights.size(); ++j) {
                weights[j] += learning_rate * error * training_inputs[i][j];
            }
            bias += learning_rate * error;
        }
    }
}

int Perceptron::predict(std::vector<int> input) {
    // Predict using the trained weights and bias
    double sum = 0;
    for (size_t i = 0; i < input.size(); ++i) {
        sum += input[i] * weights[i];
    }
    sum += bias;
    return activation_function(sum);
}
