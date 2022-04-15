#include<string>
#include<map>
#include<utility>
#include<vector>
#include<set>
#include<deque>

/***********************************************************************
* 
 * Header file defines a class of Turing Machine Simulator
 * It recerives an input text file, which contains an information about 
 * Turing Machine and configures such.
 * We can run the machine in two modes: exit and debug.
 * In exit mode the simulator prints the output of Turing Machine with the 
 * given configuration. In debug mode the program prints the output of each 
 * step after pressing Enter. To exit debug mode press Ctrl+Alt+D.
 * **Warning-->When entering the program consider that character corresponding
 * to empty is '.'.
 * ',', '|', '.' are special symbols and are not used as input symbols and are
 * reserved as special characters.
 * 
 * *********************************************************************/

#ifndef TURING_H
#define TURING_H
class Turing {
public:
    Turing(std::string&);
    std::string run(std::string&);
    void debug(std::string&);
private:
    struct decision{
        char symbol;
        int state;
        char direction; //right=r, left=l, stay at current position=s;
    };
    std::vector<char> alphabet; //set of input symbols
    std::set<char> sAlphabet;
    std::vector<int> states;//set of internal states
    std::set<int> sStates;
    std::map<std::pair<char,int>, decision> change;
    void parseAlphabet(std::string);
    void parseStates(std::string);
    void parseFunction(std::string, int);
    void printTape(const std::deque<char>&);
};

#endif
