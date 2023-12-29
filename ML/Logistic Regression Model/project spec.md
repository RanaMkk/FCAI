# Assignment 1: Logistic Regression

## Project Specification

### Introduction
A housing finance company aims to automate the eligibility validation process for interest-free home loans based on customer details provided during the loan application. The company seeks a predictive model for loan decisions using logistic regression. The project involves analyzing the "loan_old.csv" dataset and making predictions on the "loan_new.csv" dataset.

### Datasets
1. **loan_old.csv**: Contains 614 records with 10 feature columns and 2 target columns, representing loan application details.
2. **loan_new.csv**: Consists of 367 records of new applicants' data, aligned with the features of the "loan_old.csv" dataset.

### Requirements

#### Data Analysis
a. Load the "loan_old.csv" dataset.
b. Perform dataset analysis to:
   - i. Check for missing values.
   - ii. Identify the type of each feature (categorical or numerical).
   - iii. Verify if numerical features have the same scale.

#### Data Preprocessing
c. Preprocess the data:
   - i. Remove records with missing values.
   - ii. Separate features and targets.
   - iii. Encode categorical features and targets.
   - iv. Standardize numerical features.

#### Logistic Regression
d. Fit a logistic regression model to predict the loan status using gradient descent (from scratch).
e. Implement a function (from scratch) to calculate the accuracy of the logistic regression model.

#### Predictions on New Data
f. Load the "loan_new.csv" dataset.
g. Perform the same preprocessing on the new data.
h. Utilize the trained logistic regression model to predict loan statuses for the new data.
