import numpy as np
print('----------------------')
print('Logistic regression model:')
# Logistic regression model
#Logistic regression Algorithm: σ(z)
#1. Define the Sigmoid Function
#2. Initialize Parameters (θ and B)
#3. Compute the Linear Combination: z = θ1x1+θ2x2+…+θnx n+b
#4. Apply the Sigmoid Function: y = σ(z)
#5. Define the Cost Function: J(θ)=− 1/m∑[y(i)log(y)+(1−y)log(1− y)]
#6. Gradient Descent
#    θj=θj−α∂j/∂θj
#    b = b + −α∂j/∂b

def sigmoid(z):
    # Converting z to array of floats
    z = np.array(z, dtype=float)
    # Calculating sigmoid
    return 1 / (1 + np.exp(-z))

def initialize_parameters(dim):
    # Initialize weights and bias to zero
    theta = np.zeros((1, dim))
    b = 0
    return theta, b

def linear_combination(X, w, b):
    return np.dot(X, w.T) + b

def compute_cost(y, y_hat):
    m = len(y)
    return -1/m * np.sum(y * np.log(y_hat) + (1 - y) * np.log(1 - y_hat))

def predict(X, w, b):
    z = linear_combination(X, w, b)
    return sigmoid(z)

def gradient_descent(X, y, w, b, learning_rate, num_iterations):
    m = len(y)
    for i in range(num_iterations):
        # Compute linear combination
        z = linear_combination(X, w, b)
        # Apply sigmoid function and reshape
        y_hat = sigmoid(z).reshape(-1)
        # Compute cost
        cost = compute_cost(y, y_hat)
        # Compute gradients
        dw = 1/m * np.dot(X.T, (y_hat - y))
        db = 1/m * np.sum(y_hat - y)
        # Update parameters
        w -= (learning_rate * dw.T).astype(float)  # Transpose dw before updating weights
        b -= learning_rate * db
        # Print cost every 100 iterations
        if i % 200 == 0:
            print(f"Cost after iteration {i}: {cost}")
    return w, b


# Calculate accuracy function
def accuracy(X, y, w, b):
    predictions = predict(X, w, b)
    predictions_as_binary = ((predictions >= 0.5).astype(int)).reshape(-1)
    correct_predictions = (predictions_as_binary == y).sum()
    accuracy = (correct_predictions / len(y)) * 100
    return accuracy