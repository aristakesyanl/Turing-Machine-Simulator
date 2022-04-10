#include<string>
#include<map>
#include<utility>


/***********************************************************************
* 
 * Header file defines a class of Turing Machine Simulator
 * It recerives an input text file, which contains an information about 
 * Turing Machine and configures such.
 * We can run the machine in two modes: exit and debug.
 * In exit mode the simulator prints the output of Turing Machine with the 
 * given configuration. In debug mode the program prints the output of each 
 * step after pressing Enter. To exit debug mode press Ctrl+Alt+D.
 * 
 * *********************************************************************/

#ifndef TURING_H
#define TURING_H
class Turing {
public:
    Turing(std::string&);
    std::string run(std::string&);
    std::string debug(std::string&, std::string&);
private:
    struct decision{
        char symbol;
        int state;
        char direction; //right=r, left=l
    }
    std::vector<char> inputSymbol; //set of input symbols
    std::set<char> sInput;
    std::vector<char> outputSymbol;//set of output symbols
    std::set<char> sOutput;
    std::vector<int> states;//set of internal states
    std::set<int> sStates;
    std::map<std::pair<char,int>, decision> change;
    void parseInput(std::string);
    void parseOutput(std::string);
    void parseStates(std::string);
    void parseFunction(std::string, int);
};

#endif
