from cmath import sqrt
import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split

# Load data from Excel file
data = pd.read_excel("concrete_data.xlsx")

# Separate features and target
features = data.iloc[:, :-1].values
target = data.iloc[:, -1].values.reshape(-1, 1)

# Normalize data
# Min-Max normalization
min_val = features.min(axis=0)
max_val = features.max(axis=0)
features = (features - min_val) / (max_val - min_val)

# Split data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(features, target, test_size=0.25, random_state=42)

# Sigmoid activation function
def sigmoid(x):
    return 1 / (1 + np.exp(-x))

# Neural Network class
class NeuralNetwork:
    def __init__(self):
        self.input_size = None
        self.hidden_size = None
        self.output_size = None
        self.weights_input_hidden = None
        self.weights_hidden_output = None
        self.bias_hidden = None
        self.bias_output = None
        self.learning_rate = None

    def set_architecture(self, input_size, hidden_size, output_size, learning_rate):
        self.input_size = input_size
        self.hidden_size = hidden_size
        self.output_size = output_size
        self.learning_rate = learning_rate
        # Initialize weights and biases
        self.bias_hidden = np.zeros((1, self.hidden_size))
        self.bias_output = np.zeros((1, self.output_size))
        self.weights_input_hidden = np.random.randn(self.input_size, self.hidden_size) * np.sqrt(2 / self.input_size)
        self.weights_hidden_output = np.random.randn(self.hidden_size, self.output_size) * np.sqrt(2 / self.hidden_size)


    def sigmoid(self, x):
        return 1 / (1 + np.exp(-x))

    def sigmoid_derivative(self, x):
        return x * (1 - x)

    def mean_squared_error(self, y_true, y_pred):
        return np.mean((y_true - y_pred) ** 2)

    def train(self, X, y, epochs):
        for epoch in range(epochs):
            # Forward propagation
            hidden_layer_input = np.dot(X, self.weights_input_hidden) + self.bias_hidden
            hidden_layer_output = self.sigmoid(hidden_layer_input)
            output_layer_input = np.dot(hidden_layer_output, self.weights_hidden_output) + self.bias_output
            predicted_output = self.sigmoid(output_layer_input)

            # Backpropagation
            error = y - predicted_output
            output_error_term = error * self.sigmoid_derivative(predicted_output)
            hidden_error = output_error_term.dot(self.weights_hidden_output.T)
            hidden_error_term = hidden_error * self.sigmoid_derivative(hidden_layer_output)

            # Update weights and biases
            self.weights_hidden_output += hidden_layer_output.T.dot(output_error_term) * self.learning_rate
            self.bias_output += np.sum(output_error_term, axis=0, keepdims=True) * self.learning_rate
            self.weights_input_hidden += X.T.dot(hidden_error_term) * self.learning_rate
            self.bias_hidden += np.sum(hidden_error_term, axis=0, keepdims=True) * self.learning_rate



    def predict(self, X):
        # Forward propagation for prediction
        hidden_layer_input = np.dot(X, self.weights_input_hidden) + self.bias_hidden
        hidden_layer_output = self.sigmoid(hidden_layer_input)
        output_layer_input = np.dot(hidden_layer_output, self.weights_hidden_output) + self.bias_output
        predicted_output = output_layer_input  # Linear activation (no sigmoid)
        return predicted_output
    def calculate_error(self,predicted, actual):
        error = self.mean_squared_error(predicted, actual)
        return error


# Example usage
input_size = X_train.shape[1]
hidden_size = 180
output_size = 1
learning_rate = 0.0001
epochs = 1000

# Create and initialize the neural network
nn = NeuralNetwork()
nn.set_architecture(input_size, hidden_size, output_size, learning_rate)

# Train the neural network and get MSE history
nn.train(X_train, y_train, epochs)

# Make predictions on the test set
predictions = nn.predict(X_test)
#print (predictions)
# Calculate error
mse = nn.calculate_error(predictions, y_test)
print(f"Mean sqare Error on Test Set: {mse}")
