# MNIST Image Classification Project

## Overview
The MNIST Image Classification project focuses on utilizing machine learning techniques, specifically Artificial Neural Networks (ANNs), for classifying handwritten digits from the MNIST dataset. The dataset provides labeled images for training and testing, with pixel values representing grayscale intensity.

## Dataset
The MNIST dataset is available in CSV format, consisting of two main files:
- **mnist_train.csv:** Training set with labeled images.
- **mnist_test.csv:** Test set for evaluating model performance.

Each row in the CSV files represents an image, and pixel values range from 0 to 255. Images have a fixed size of 28x28 pixels.

## Objectives
1. Acquire proficiency in Tensorflow and Scikit-Learn.
2. Familiarize with Artificial Neural Networks (ANNs) and hyperparameter tuning.

## Project Workflow

### Data Exploration and Preparation
- Load and explore the dataset.
- Identify unique classes and features.
- Check for missing values.
- Normalize pixel values.
- Resize images to 28x28 pixels.
- Split the training data into training and validation sets.

### Experiments and Results
#### ANN Experiment
- Construct and train two ANN architectures.
- Explore variations in hidden neurons, learning rate, and batch size.
- Compare outcomes to determine the highest accuracy on the validation dataset.

#### Evaluation
- Obtain the confusion matrix of the best model.
- Save the best model and reload it for testing on the mnist_test.csv dataset.

Feel free to adapt the structure based on your actual project organization. The code for data exploration and experiments can be organized into Jupyter notebooks within the `notebooks/` directory.

Good luck with your MNIST Image Classification project!

