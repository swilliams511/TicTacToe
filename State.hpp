#ifndef __STATE
#define __STATE

#include <iostream>
#include <vector>
#include <array>
class State{
public:
    State();           //constructor
    State(const std::array<std::array<int,3>,3>& otherBoard); //parameterized constructor
    ~State();   //destructor
    bool isFinal();    ///will do something like checking if the state is a final state, might also set finalState
    bool getFinal() const;
    bool operator == (const State& otherState) const; //equality operator used for checking if a state already exists
    bool operator != (const State& otherState) const; //the negation of equality
    std::vector<State*> getTransitions() const;       //returns the vector of pointers to connected states
    std::array<std::array<int,3>,3> getBoard() const; //returns the game board at the current state
    void print() const;                               //debug function

    void setStateNumber(int number);
    int getStateNumber() const;
    int getWinner() const;

    void addTransiton(State* transitionState);        //adds a new transition to the collection of transitions
private:
    int stateNumber;
    std::array<std::array<int,3>,3> board;
    std::vector<State*> transitions;
    bool finalState;
    int winner;   //-1 for unset, 0 for O winning 1 for X winning

};


#endif // __STATE
