#include<string>
#include<map>
#include<fstream>


/***********************************************************************
* 
 * Header file defines a class of Turing Machine Simulator
 * It recerives an input text file, which contains an information about 
 * Turing Machine and configures such.
 * We can run the machine in two modes: exit and debug.
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
    std::vector<char> inputSymbol; //set of input symbols
    std::set<char> sInput;
    std::vector<char> outputSymbol;//set of output symbols
    std::set<char> sOutput;
    std::vector<int> states;//set of internal states
    std::set<int> sStates;
    struct decision{
        char symbol;
        int state;
        //right=r, left=l
        char direction;
    }
    std::map<std::pair<char,int>, decision> change;
    void parseInput(std::string);
    void parseOutput(std::string);
    void parseStates(std::string);
    void parseFunction(std::string, int);
};

#endif
