#include <iostream>
#include "DFA.hpp"
#include "State.hpp"
#include <ctime>
#include <cstdlib>

int main()
{
    DFA dfa;             //initialize our DFA
    dfa.createStates();  //initialize the states in our DFA
    std::srand(time(NULL));
    std::cout << "Select your starting state: \n";
    int counter = 1;
    for(int i = 0; i < 3; ++i)
    {
        std::cout << "  ";
        for(int j = 0; j < 3; ++j)
        {
            std::cout << "[" << counter << "]";
            counter++;
        }
        std::cout << "\n";
    }
    int choice;
    std::string eater;
    std::cin >> choice;
    if(choice <= 0 || choice > 9)
    {
        std::cout << "Invalid starting state, terminating...\n";
        return 1;
    }
    std::vector<double> x;
    x = dfa.calculateProbability(choice);
    std::cout << "The probability of winning given you started in state " <<
    choice << " is: " << x.at(choice-1) * 100 << "%\n";
    std::cout << "type anything to proceed\n";
    std::cin >> eater;
    std::cout << "In case you were wondering, here are the probabilities for the\n other starting states\n";
    counter = 0;
    for(int i = 0; i < 3; ++i)
    {
        std::cout << "  ";
        for(int j = 0; j < 3; ++j)
        {
            std::cout << "[" << x.at(counter) *100<< "%]";
            counter++;
        }
        std::cout << "\n";
    }
    while(true)
    {
        std::cout << "Now for a simulation, choose a starting state (enter 0 to leave the simulation): \n";
        std::cin >> choice;
        if(choice <= 0 || choice > 9)
            break;
        counter = 1;
        for(int i = 0; i < 3; ++i)
        {
            std::cout << "  ";
            for(int j = 0; j < 3; ++j)
            {
                std::cout << "[" << counter << "]";
                counter++;
            }
            std::cout << "\n";
        }
        dfa.simulate(choice, true);
    }
    while(true)
    {
        std::cout << "Lastly, lets use probability theory to approximate\n the odds of winning from a certain state (enter 0 to leave the simulation): \n";
        std::cin >> choice;
        if(choice <= 0 || choice > 9)
            break;
        counter = 1;
        for(int i = 0; i < 3; ++i)
        {
            std::cout << "  ";
            for(int j = 0; j < 3; ++j)
            {
                std::cout << "[" << counter << "]";
                counter++;
            }
            std::cout << "\n";
        }
        std::cout << "Enter the number of simulations to perform (100 to 1,000,000 are good choices\n";
        int wins = 0;
        int runs;
        std::cin >> runs;
        int n = 1;
        for(int i = 0; i < runs; ++i)
            wins += dfa.simulate(n,false);
        std::cout << "After " << runs << " simulations, the probability is: " << (float)wins/(float)runs * 100<< "%\n";
    }


    return 0;
}
