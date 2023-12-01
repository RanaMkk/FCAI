#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <numeric>
#include <random>
using namespace std;
//knapsack item struct
struct Item {
    int weight;
    int value;
};

//input vector of knapsack items
vector<Item> items;
int knapsackSize;

//function to read input from file
void readInput(ifstream& inputFile) {
    int numItems;
    inputFile >> knapsackSize >> numItems;
    items.resize(numItems);
    for (int i = 0; i < numItems; ++i) {
        inputFile >> items[i].weight >> items[i].value;
    }
}
/* population is 2D array
 * each chromoseome is binary vector with size = knapsackSize
 * representation = 1D binary vector
 * function to generate population
*/
vector<vector<bool>> generatePopulation(int populationSize) {
    vector<vector<bool>> population(populationSize, vector<bool>(items.size()));
    for (int i = 0; i < populationSize; ++i) {
        for (int j = 0; j < items.size(); ++j) {
            population[i][j] = rand() % 2;
        }
    }
    return population;
}
/*
 * individual: single chromosome "solution"
 */
int calculateFitness(const vector<bool>& individual) {
    int totalWeight = 0;
    int totalValue = 0;
    for (int i = 0; i < items.size(); ++i) {
        if (individual[i]) {
            totalWeight += items[i].weight;
            totalValue += items[i].value;
        }
    }
    //neglect this solution if the weight constraint is violated
    if (totalWeight > knapsackSize) {
        totalValue = 0;
    }
    return totalValue;
}
/*
* Rank selection 
* first rank selection
* then apply roulette wheel
*/

vector<bool> selectParents(const vector<vector<bool>>& population) {
    vector<bool> parent;
    // Create a vector to store the ranks of individuals in the population.
    vector<int> ranks(population.size());
    // Initialize the ranks vector with sequential numbers
    iota(ranks.begin(), ranks.end(), 0);
    // Calculate the total fitness sum
    double totalFitness = 0.0;
    for (int i = 0; i < population.size(); ++i) {
        totalFitness += calculateFitness(population[i]);
    }
    // Normalize fitness values by dividing by total number of individuals
    vector<double> normalizedFitness(population.size());
    for (int i = 0; i < population.size(); ++i) {
        normalizedFitness[i] = calculateFitness(population[i]) / static_cast<double>(population.size());
    }
    // Calculate cumulative fitness
    for (int i = 1; i < population.size(); ++i) {
        normalizedFitness[i] += normalizedFitness[i - 1];
    } 
    // Generate a random number between 0 and 1
    double randomValue = (rand() / (double)RAND_MAX);
    // Find the individual whose cumulative fitness crosses the random value
    int selectedRank = 0;
    for (int i = 0; i < population.size(); ++i) {
        if (randomValue <= normalizedFitness[i]) {
            selectedRank = i;
            break;
        }
    }
    // Randomly select a parent based on the selected rank
    parent = population[ranks[selectedRank]];
    return parent;
}
/**
 * Single point crossover operation between two parent individuals
 * to create a pair of offsprings
 */
pair<vector<bool>, vector<bool>> crossover(const vector<bool>& parent1, const vector<bool>& parent2) {
    int crossoverPoint = rand() % items.size();

    // Initialize the offspring individuals with the same size as the parents
    vector<bool> offspring1(items.size());
    vector<bool> offspring2(items.size());

    for (int i = 0; i < items.size(); ++i) {
        if (i < crossoverPoint) {
            // If the gene index is less than the crossover point
            //use the corresponding genes from parent1 for both offspring.
            offspring1[i] = parent1[i];
            offspring2[i] = parent1[i];
        }
        else {
            // Otherwise, use the genes from parent2 for both offspring.
            offspring1[i] = parent2[i];
            offspring2[i] = parent2[i];
        }
    }
    return make_pair(offspring1, offspring2);
}
/**
 * Mutate an individual's genes based on a given mutation rate.
 * appropriate Mutation --> flipping 
 */
void mutate(vector<bool>& individual, double mutationRate) {
    for (int i = 0; i < individual.size(); ++i) {
        if ((rand() / (RAND_MAX + 1.0)) <= mutationRate) {
            individual[i] = !individual[i];
        }
    }
}
/*
* Steps:
*   1. Read Input
*   2. The representation of the chromosomes -> 1D binary array
*   3. Fitness Function -> maximize the total value
*   4. Apply GA
*       4.1. Generate population
*       4.2. evaluate fitness
*       4.3. Selection
*       4.4. CrossOver
*       4.5. Mutation
*   5. Find the best sol (output)
*/
int main() {
    srand(time(NULL)); // Seed with the current time
    //Input
    ifstream inputFile("test.txt");
    int numTestCases;
    inputFile >> numTestCases;
    for (int testCase = 1; testCase <= numTestCases; testCase++) {
        readInput(inputFile);
        //consts 
        int populationSize = 100;
        double mutationRate = 0.1;
        int numGenerations = 100;

        //Generate population
        vector<vector<bool>> population = generatePopulation(populationSize);
        for (int generation = 0; generation < numGenerations; ++generation) {
            vector<vector<bool>> newPopulation;

            for (int i = 0; i < populationSize; ++i) {
                //Evaluate fitness
                vector<int> fitnessValues;
                // Calculate fitness values for each individual in the population
                for (int i = 0; i < population.size(); ++i) {
                    vector<bool>& individual = population[i];
                    int fitness = calculateFitness(individual);
                    fitnessValues.push_back(fitness);
                }
                //select parents
                vector<bool> parent1 = selectParents(population);
                vector<bool> parent2 = selectParents(population);
                //cross over
                pair<vector<bool>, vector<bool>>offsprings = crossover(parent1, parent2);
                vector<bool> offspring1 = offsprings.first;
                vector<bool> offspring2 = offsprings.second;
                //mutation
                mutate(offspring1, mutationRate);
                mutate(offspring2, mutationRate);
                //add new childern to new population
                newPopulation.push_back(offspring1);
                newPopulation.push_back(offspring2);
            }
            //update with the new population
            population = newPopulation;
        }
        //loop over all pop to get the best solution
        int bestFitness = 0;
        int totalWeight = 0;
        vector<bool> bestIndividual;
        for (int i = 0; i < population.size(); ++i) {
            vector<bool>& individual = population[i];
            int fitness = calculateFitness(individual);
            if (fitness > bestFitness) {
                bestFitness = fitness;
                bestIndividual = individual;
            }
        }
        cout << "Test Case " << (testCase) << ": ";
        cout << "Number of Selected Items: " << count(bestIndividual.begin(), bestIndividual.end(), true) << ", ";
        cout << "Total Value: " << bestFitness << endl;
        cout << "Selected Items: " << endl;;
        for (int i = 0; i < items.size(); ++i) {
            if (bestIndividual[i]) {
                cout << "Item " << i + 1 << " - Weight: " << items[i].weight << ", Value: " << items[i].value << endl;
                totalWeight += items[i].weight;
            }
        }
        cout << "Total Weight of Selected Items: " << totalWeight << endl;
        cout << endl;
    }
    inputFile.close();
    return 0;
}
