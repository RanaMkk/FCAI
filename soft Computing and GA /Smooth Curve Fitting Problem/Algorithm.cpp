#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <fstream>
using namespace std;
// Define the chromosome as a vector of three coefficients
using Chromosome = vector<double>; 
// some constants
double Pm = 0.1; // Probability of mutation
double Pc = 0.7; // Probability of crossover

//nuniform mutation consts
double lowerBound = -5.0; // Lower bound for genes
double upperBound = 5.0;  // Upper bound for genes
double b = 2.0; // Parameter controlling the degree of non-uniformity

int maxGenerations = 5; // Maximum number of generations
int populationSize = 10; // Population size (adjustable)

int tournamentSize = 2;
/*
* representation of the chromosome : a vector of floating point numbers with size D+1
* fitness function: the mean square error ?(y_calc - y_actual)²
* poly equation will be : a? + a? X + a? X² + a? X?
*/
// Function to initialize the population
vector<Chromosome> initializePopulation(int populationSize, int numCoefficients) {
    vector<Chromosome> population;
    srand(static_cast<unsigned>(time(nullptr))); // Seed for random number generation
    for (int i = 0; i < populationSize; ++i) {
        Chromosome chromosome;
        // Initialize random coefficients within the range of -10 to 10
        for (int j = 0; j < numCoefficients; ++j) {
            double randomCoefficient = (rand() % 201 - 100) / 10.0; // Random number between -10 and 10 with two decimal places
            chromosome.push_back(randomCoefficient);
        }
        // Add the chromosome to the population
        population.push_back(chromosome);
    }
    return population;
}

// Function to calculate the mean square error for a given chromosome and data points
double calculateError(const Chromosome& chromosome, const vector<pair<double, double>>& dataPoints) {
    double totalError = 0.0;
    for (const auto& point : dataPoints) {
        double x = point.first;
        double yActual = point.second;
        // Calculate the predicted y value using the polynomial equation
        double yCalc = chromosome[0] + chromosome[1] * x + chromosome[2] * x * x;
        // Calculate the squared error for the current data point
        double error = pow(yCalc - yActual, 2.0);
        // Accumulate the total error
        totalError += error;
    }
    // Calculate the mean square error
    return totalError / dataPoints.size();
}
// Function to evaluate the fitness of each chromosome in the population
vector<double> evaluateFitness(const vector<Chromosome>& population, const vector<pair<double, double>>& dataPoints) {
    vector<double> fitnessValues;
    // Evaluate the fitness for each chromosome in the population
    for (const auto& chromosome : population) {
        double error = calculateError(chromosome, dataPoints);
        double fitness = 1.0 / error; // Using 1/error as the fitness
        fitnessValues.push_back(fitness);
    }
    return fitnessValues;
}

// Function to perform tournament selection and return the selected parents (selects only 2 parents)
vector<Chromosome> tournamentSelection(const vector<Chromosome>& population, const vector<double>& fitnessValues, int tournamentSize) {
    vector<Chromosome> selectedParents;
    for (size_t i = 0; i < 2; ++i) {
        // Randomly select individuals for the tournament
        vector<size_t> tournamentParticipants;
        for (int j = 0; j < tournamentSize; ++j) {
            size_t randomIndex = rand() % population.size();
            tournamentParticipants.push_back(randomIndex);
        }
        // Choose the winner of the tournament (individual with the highest fitness)
        size_t winnerIndex = tournamentParticipants[0];
        for (size_t participantIndex : tournamentParticipants) {
            if (fitnessValues[participantIndex] > fitnessValues[winnerIndex]) {
                winnerIndex = participantIndex;
            }
        }
        // Add the selected chromosome to the list of parents
        selectedParents.push_back(population[winnerIndex]);
    }
    return selectedParents;
}

// Function to perform 2-point crossover between two parents
void twoPointCrossover(Chromosome& parent1, Chromosome& parent2) {
    // Generate two random points within the range of the chromosomes
    int crossoverPoint1 = rand() % parent1.size();
    int crossoverPoint2 = rand() % parent2.size();

    // Ensure crossoverPoint2 is greater than crossoverPoint1
    if (crossoverPoint2 < crossoverPoint1) {
        swap(crossoverPoint1, crossoverPoint2);
    }
    // Perform crossover only if crossoverPoints are valid
    if (crossoverPoint1 > 0 && crossoverPoint1 < parent1.size() &&
        crossoverPoint2 > crossoverPoint1 && crossoverPoint2 < parent1.size()) {
        // Swap genetic material between the two crossover points
        for (int i = crossoverPoint1; i <= crossoverPoint2; ++i) {
            swap(parent1[i], parent2[i]);
        }
    }
}

// Function to perform non-uniform mutation on a chromosome
void nonUniformMutation(vector<double>& chromosome, double mutationProbability, double lowerBound, double upperBound, int currentGeneration, int maxGenerations, double b) {
    for (double& gene : chromosome) {
        double deltaLxi = gene - lowerBound;
        double deltaUxi = upperBound - gene;
        // Generate random number r1
        double r1 = static_cast<double>(rand()) / RAND_MAX;
        double delta;
        if (r1 <= 0.5) {
            delta = deltaLxi;
        }
        else {
            delta = deltaUxi;
        }
        // Calculate ?(t, y)
        double r = static_cast<double>(rand()) / RAND_MAX;
        double t = static_cast<double>(currentGeneration);
        double T = static_cast<double>(maxGenerations);
        double deltaT = delta * (1 - pow(r, pow(1 - t / T, b)));
        // Perform mutation
        if (delta == deltaLxi) {
            gene -= deltaT;
        }
        else {
            gene += deltaT;
        }
        // Check if mutation should be performed based on mutationProbability
        double mutationChance = static_cast<double>(rand()) / RAND_MAX;
        if (mutationChance <= mutationProbability) {
            gene = lowerBound + static_cast<double>(rand()) / RAND_MAX * (upperBound - lowerBound);
        }
    }
}

// Function to perform elitism and replace the current generation with new offspring
void replaceWithElitism(vector<Chromosome>& currentGeneration, const vector<Chromosome>& offspring, 
    int elitismCount, const vector<pair<double, double>>& dataPoints) {
    // Combine the current generation and offspring
    vector<Chromosome> combinedPopulation = currentGeneration;
    combinedPopulation.insert(combinedPopulation.end(), offspring.begin(), offspring.end());
    // Sort the combined population based on fitness
    sort(combinedPopulation.begin(), combinedPopulation.end(), [dataPoints](const Chromosome& a, const Chromosome& b) {
        double fitnessA = calculateError(a, dataPoints);
        double fitnessB = calculateError(b, dataPoints);
        return fitnessA < fitnessB; // Sort in ascending order of fitness (lower error is better)
        });
    // Select the top individuals as elite
    vector<Chromosome> elitePopulation(combinedPopulation.begin(), combinedPopulation.begin() + elitismCount);
    // Replace the current generation with the elite population
    currentGeneration = elitePopulation;
}


// Input
// Dataset represents a testcase
struct Dataset {
    int numDataPoints;
    int polynomialDegree;
    vector<pair<double, double>> dataPoints;
};
vector<Dataset> readInputFile(const string& filename) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error opening file: " << filename << "\n";
        exit(EXIT_FAILURE);
    }
    int numDatasets;
    inputFile >> numDatasets;
    vector<Dataset> datasets;
    for (int i = 0; i < numDatasets; ++i) {
        Dataset dataset;
        inputFile >> dataset.numDataPoints >> dataset.polynomialDegree;
        for (int j = 0; j < dataset.numDataPoints; ++j) {
            double x, y;
            inputFile >> x >> y;
            dataset.dataPoints.push_back(make_pair(x, y));
        }
        datasets.push_back(dataset);
    }
    inputFile.close();
    return datasets;
}
// GA function
void performGeneticAlgorithm(const Dataset& dataset, ofstream& outputFile) {
    int numCoefficients = dataset.polynomialDegree + 1;
    // Step 1 : initialization of population
    vector<Chromosome> population = initializePopulation(populationSize, numCoefficients);
    for (int generation = 0; generation < maxGenerations; ++generation) {
        // Step 2 : Evaluate the fitness of each chromosome
        vector<double> fitnessValues = evaluateFitness(population, dataset.dataPoints);

        // Step 3 : tournament selection
        vector<Chromosome> parents = tournamentSelection(population, fitnessValues,tournamentSize);

        // Step 4 : 2 point crossover
        // generate a random  number between [0,1] and if it's below (pc = 0.9) then apply crossover else ignore crossover
        double crossoverProbability = static_cast<double>(rand()) / RAND_MAX;
        if (crossoverProbability <= Pc) {
            twoPointCrossover(parents[0], parents[1]);
        }
        // Step 5 : Mutation with nonUniform mutation
        for (Chromosome& child : parents) {
            nonUniformMutation(child, Pm, lowerBound, upperBound, generation, maxGenerations, b);
        }
        // Step 6 : Replacement with elitism
        replaceWithElitism(population, parents, 2, dataset.dataPoints);

        // Output best solution in each generation
        cout << "Generation: " << generation << ", Best Fitness: " << 1.0 / calculateError(population[0], dataset.dataPoints) << "\n";
        outputFile << "Generation: " << generation << ", Best Fitness: " << 1.0 / calculateError(population[0], dataset.dataPoints) << "\n";
    }

    // Output the final best solution
    cout << "Best Solution: ";
    outputFile << "Best Solution: ";
    for (const auto& coefficient : population[0]) {
        cout << coefficient << " ";
        outputFile << coefficient << " ";
    }
    cout << "\n";
}
int main() {
    string filename = "input.txt";
    ofstream outputFile("output.txt");
    vector<Dataset> datasets = readInputFile(filename);
    int cnt = 1;
    for (const auto& dataset : datasets) {
        cout << "Testcase no." << cnt << "\n";
        outputFile << "Testcase no." << cnt << "\n";
        cout << "Number of Data Points: " << dataset.numDataPoints << "\n";
        outputFile << "Number of Data Points: " << dataset.numDataPoints << "\n";
        cout << "Polynomial Degree: " << dataset.polynomialDegree << "\n";
        outputFile << "Polynomial Degree: " << dataset.polynomialDegree << "\n";
        cout << "Data Points:" << "\n";
        outputFile << "Data Points:" << "\n";
        for (const auto& point : dataset.dataPoints) {
            cout << point.first << " " << point.second << "\n";
            outputFile << point.first << " " << point.second << "\n";
        }
        performGeneticAlgorithm(dataset,outputFile);
        cout << "------------------------" << "\n";
        outputFile << "\n------------------------" << "\n";
        cnt++;
    }
    return 0;
}
