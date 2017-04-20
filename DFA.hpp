#ifndef __DFA
#define __DFA
#include <vector>
#include "State.hpp"


class DFA{
public:
    DFA();   //constructor
    ~DFA();  //destructor
    size_t numStates() const; //getter function for number of state in the DFA
    void createStates();      //function that generates the states in the DFA
    std::vector<double> calculateProbability(int firstMoveBoard) const;

    void print() const;       //debug function
    int simulate(int n, bool print);

private:
    std::vector<State*> collectionOfStates; //holds all of the states created
    std::vector<State*> firstTurnStates;    //holds all of the states created on the first turn, subset of all states
    std::vector<State*> secondTurnStates;
    std::vector<State*> thirdTurnStates;
    std::vector<State*> fourthTurnStates;
    std::vector<State*> fifthTurnStates;
    std::vector<State*> sixthTurnStates;
    std::vector<State*> seventhTurnStates;
};




#endif // __DFA
