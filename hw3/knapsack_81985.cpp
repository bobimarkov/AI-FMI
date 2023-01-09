#include <iostream>
#include <random>
#include <ctime>
#include <vector>

typedef std::vector<int> Chromosome;

struct Knapsack {
    int weight;
    int items;
};

struct Item {
    int weight;
    int value;
};

int calculateFitness(Chromosome chromosome, std::vector<Item> items, Knapsack knapsack) {
    int fitness = 0, totalWeight = 0;
    for (int i = 0; i < chromosome.size(); i++) {
        fitness += chromosome[i] * items[i].value;
        totalWeight += chromosome[i] * items[i].weight;
    }   
    if (totalWeight > knapsack.weight) {
        fitness = 0;
    }
    return fitness;
}


std::vector<Chromosome> initPopulation (Knapsack knapsack, int limitPopulation) {
    std::vector<Chromosome> population;
    for (int i = 0; i < limitPopulation; i++) {
        Chromosome chromosome(knapsack.items, 0);
        int numberItems = rand() % (knapsack.items - 1) + 1;
        for (int j = 0; j < numberItems; j++) {
            int position = rand() % knapsack.items; 
            if (chromosome[position] == 1) {
                j--;
                continue;
            }
            chromosome[position] = 1;
        }
        population.push_back(chromosome);
    } 
    return population;
}

// ----------------------------------------------------------------------------------------------

std::vector<int> selection (std::vector<Chromosome> population, std::vector<Item> items, Knapsack sack) {
    int maxFitness = 0, firstIndex = 0, secondIndex = 0;
    for (int i = 0; i < population.size(); i++) {
        int fitness = calculateFitness(population[i], items, sack);
        if (fitness > maxFitness) {
            maxFitness = fitness;
            secondIndex = firstIndex;
            firstIndex = i;
        }
    }

    return {firstIndex, secondIndex};
}

//crossover

std::vector<Chromosome> crossover (std::vector<Chromosome> population, std::vector<int> parents) {
    Chromosome firstChild = population[parents[0]], secondChild = population[parents[1]];
    int splitter = rand() % firstChild.size();
    for (int i = splitter; i < firstChild.size(); i++) {
        std::swap(firstChild[i], secondChild[i]);
    }
    return {firstChild, secondChild};
}

//mutation
std::vector<Chromosome> mutation (std::vector<Chromosome> children) {
    double mutationRate = 10;
    for (int j = 0; j < children.size(); j++) {
        for (int i = 0; i < children[j].size(); i++) {
            if (rand() % 100 < mutationRate) {
                children[j][i] += pow(-1, children[j][i]);
            } 
        }
    }

    return children;
}

//gen_algo
std::vector<Chromosome> createNextGeneration (std::vector<Chromosome> currentPopulation, std::vector<Item> items, Knapsack sack) {
    std::vector<Chromosome> nextGeneration;

    while (nextGeneration.size() <= currentPopulation.size()) {
        std::vector<int> selected = selection(currentPopulation, items, sack);
        std::vector<Chromosome> children = crossover(currentPopulation, selected);
        std::vector<Chromosome> mutated = mutation(children);

        for (Chromosome child : mutated) {
            nextGeneration.push_back(child);
            if (nextGeneration.size() == currentPopulation.size()) {
                break;
            }
        }
    }

    return nextGeneration;
}

int maxFitnessOfGeneration (std::vector<Chromosome> generation, std::vector<Item> items, Knapsack sack) {
    int max = 0;
    for (Chromosome c : generation) {
        if (max < calculateFitness(c, items, sack)) {
            max = calculateFitness(c, items, sack);
        }
    }
    return max;
}

Chromosome maxChromosomeOfGeneration (std::vector<Chromosome> generation, std::vector<Item> items, Knapsack sack) {
    int max = 0;
    Chromosome maxChromosome;
    for (Chromosome c : generation) {
        if (max < calculateFitness(c, items, sack)) {
            max = calculateFitness(c, items, sack);
            maxChromosome = c;
        }
    }
    return maxChromosome;
}

void printChromosome (Chromosome chromosome) {
    std::cout << "[";
    for (int i : chromosome) {
        std::cout << i;
    }
    std::cout << "]\n";
}

int main () {
    srand(time(0));

    int populationSize = 10;
    // int a,b;
    // std::cin >> a >> b;



    //  Knapsack sack = {a, b};
     Knapsack sack = {200, 25};
    // std::vector<Item> items;
    // for (int i = 0; i < b; i++) {
    //     int c,d;
    //     std::cin >> c >> d;
    //     items.push_back({c,d});
    // }
    // Item item1 = {5, 9};
    // Item item2 = {3, 2};
    // Item item3 = {4, 5};
    // Item item4 = {7, 3};
    // Item item5 = {1, 4};
    // Item item6 = {19, 2};
    // Item item7 = {3, 1};

    Item item1={1, 5};Item item2={2, 6};Item item3={3, 7};
    Item item4={4, 8};Item item5={5, 9};Item item6={6, 10};
    Item item7={7, 11};Item item8={8, 12};Item item9={9, 13};
    Item item10={10, 14};Item item11={11, 15};Item item12={12, 16};
    Item item13={13, 17};Item item14={18, 18};Item item15={15, 9};
    Item item16={16, 20};Item item17={14, 21};Item item18={18, 2};

    Item item19={9, 23};
    Item item20={1, 2};
    Item item21={3, 4};
    Item item22={7, 27};
    Item item23={6, 33};
    Item item24={25,25};
    Item item25={17, 17};
    std::vector<Item> items = {item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11, item12, item13, item14, item15, item16, item17, item18, item19, item20, item21, item22, item23, item24, item25};

    std::vector<Chromosome> population = initPopulation(sack, populationSize);

    std::vector<Chromosome> newPopulation = population;
    Chromosome maxChromosome;
    int maxGeneration = 0;
    for (int i=0; i<120; i++) {
        newPopulation = createNextGeneration(newPopulation, items, sack);
        int currentPopulationMax = maxFitnessOfGeneration(newPopulation, items, sack); 
        if (maxGeneration < currentPopulationMax) {
            maxGeneration = currentPopulationMax;
            maxChromosome = maxChromosomeOfGeneration(newPopulation, items, sack);
        }
        //std::cout << "\nGeneration "<< (i + 1) <<": Max Fitness = " << currentPopulationMax << "\n";
        // for (Chromosome p : newPopulation) {
        //     printChromosome(p);
        // }
    }

    std::cout << "Max Chromosome: ";
    printChromosome(maxChromosome);
    std::cout << "Max Fitness: " << maxGeneration << std::endl;

    return 0;
}