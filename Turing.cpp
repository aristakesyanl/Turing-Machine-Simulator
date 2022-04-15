#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<map>
#include<vector>
#include<stdexcept>
#include<set>
#include<algorithm>
#include<deque>
#include"Turing.h"


Turing::Turing(std::string& fileName) {
    //delete empty lines from text file
    std::ifstream in(fileName);
    std::string line;
    std::vector<std::string> lines;

    if (in.is_open()) {
        while (getline(in, line)) {
            if (!(line.empty() || line.find_first_not_of(' ') == std::string::npos)) {
                lines.push_back(line);
            }
        }
    }
    else{
        throw std::runtime_error("Failed To Open Input Files");
    }

    in.close();

    // for(int i=0; i<(int) lines.size(); i++){
    //     std::cout<<lines[i]<<std::endl;
    // }

    //keep the number of lines
    int count = lines.size();
  
    if (count < 4) {
        throw std::invalid_argument("Invalid Text Format: Not Enough Information");
    }

    //check alphabet symbols
    parseAlphabet(lines[0]);

    
    //check states
    parseStates(lines[1]);

    //Check that the number of states is no less than lines
    //describing state changes.
    //In case of additional lines just ignore them
    //It is assumed that the first lines corresposnds
    //to the first state given in the state format
    //If for example first states are, q1,q2,q0
    //and input symbols are a,b,d
    //the first line describes correspondingly changes for
    //(1,a), (1,b), (1,d) etc.

    if(count-2<(int)states.size()){
        throw std::invalid_argument("Invalid File Format");
    }

    //check (symbol,state) pairs
    for(int i=2; i<(int)lines.size(); i++){
        parseFunction(lines[i],states[i-2]);
          
    }

    
 }

void Turing::parseAlphabet(std::string str){
    //remove whitespaces
    str.erase(remove(str.begin(), str.end(),' '), str.end());
    //add ',' at the end of the string to make parsing easier
    str.push_back(',');
    
    //parse to check that every second character is ','
    //string has at least two characters(as we added one at the end)
    //so checking is valid
    for(int i=1;i<(int)str.size();i+=2){
        if(str[i]!=','){
            throw std::invalid_argument("Invalid Alphabet Symbol");
        }
    }
    str.pop_back();

    //use set to check that string contains distinct characters
    //remove repeating ones
    for(int i=0; i<(int)str.size(); i+=2){
        if(sAlphabet.find(str[i])==sAlphabet.end()){
            alphabet.push_back(str[i]);
            sAlphabet.insert(str[i]);
        }

    }
    if(sAlphabet.find('.')==sAlphabet.end()){
        throw std::invalid_argument("Does not contain empty character");
    }
    
 }




 void Turing::parseStates(std::string str){
    //remove whitespaces
    str.erase(remove(str.begin(), str.end(),' '), str.end());
    
    //States should conatain at least 2 states: 0 and HALT
    if((int)str.size()<6){
        throw std::invalid_argument("Invalid States Format");
    }
    

    //add ',' at the end of the string to make parsing easier
    str.push_back(',');
    
    int init=0;
    std::vector<std::string> result;
    for(int i=1; i<(int)str.size(); i++){
        if(str[i]==','){
            int len=i-init;
            if(len>0){
                result.push_back(str.substr(init,len));
            }
            init=i+1;
        }
    }

    
    bool start=false;

    for(int i=0; i<(int)result.size(); i++){
        if(result[i]=="HALT"){
            if(sStates.find(-1)==sStates.end()){
                states.push_back(-1);
                sStates.insert(-1);
            }
        }
        else{

            if(result[i]=="0"){
                start=true;
            }
        
          
            for(int j=0; j<(int)result[i].size();j++){
                if(result[i][j]-'0'<0 || result[i][j]-'0'>10){
                    throw std::invalid_argument("Invalid State Format: Contains Non-Valid Characters");
                }

            }
            
            //check that all states are natural numbers and do not contain leading 0's
            if(result[i][0]=='0' && (int)result[i].size()>1){
                throw std::invalid_argument("Invalid State Format: States Contain Leading Zeros");
            }
            int num=stoi(result[i]);
            if(sStates.find(num)==sStates.end()){
                states.push_back(num);
                sStates.insert(num);
            }

        }
        
    }
    if(start==false){
        throw std::invalid_argument("File Does Not Contain An Initial State");
    }
 }


void Turing::parseFunction(std::string str, int state) {

    str.erase(remove(str.begin(), str.end(),' '), str.end());
    str.push_back('|');
    int init=0;
    if((int)str.size()<5){
        throw std::invalid_argument("Invalid Form Of States");
    }
    std::vector<std::string> result;
    for(int i=0; i<(int)str.size(); i++){
        if(str[i]=='|'){
            int len=i-init;
            if(len!=0){
                result.push_back(str.substr(init,len));
            }
            init=i+1;     
        }
    }

    //check that the number of tuples is no less than the number of input symbols
    //if number of tuples is more they are just going to be ignored
    if((int)result.size()<(int)alphabet.size()){
        throw std::invalid_argument("Not Enough Tuples Given");
    }

    //check that for each Tuple the length requirement is met
    for(int i=0; i<(int)result.size();i++){
        if(result[i].size()<5){
            throw std::invalid_argument("Invalid (symbol,state,direction) Tuple: Length requirment is not met");
        }
    }


    //check that in all symbols are from output
    for(int i=0; i<(int)result.size();i++){
        if(sAlphabet.find(result[i][0])==sAlphabet.end()){
            throw std::invalid_argument("Symbol Is Not Contained In Initial Alphabet");
        }
    }

    //check that every second character for tuple is ','
    for(int i=0; i<(int)result.size();i++){
        if(result[i][1]!=','){
            throw std::invalid_argument("Wrong Form of Character Given In (symbol,state, direction) Tuple: Symbol is not separated by comma");
        }
    }

    //check that directions are given
    for(int i=0; i<(int)result.size();i++){
        int sz=(int)result[i].size();
        if(result[i][sz-2]!=','){
            throw std::invalid_argument("Wrong Form of Character Given In (symbol,state, direction) Tuple: Direction is not separated by comma");
        }
        if(result[i][sz-1]!='r' && result[i][sz-1]!='l' && result[i][sz-1]!='s'){
            throw std::invalid_argument("Directions Not Given In (symbol, state, direction) Tuple: Wrong Character given to indicate direction");
        }
    }

   
    
    for(int i=0; i<(int)result.size();i++){
        std::string nstr=result[i].substr(2,(int)result[i].size()-4);
        if(nstr!="HALT"){
                for(int j=2; j<(int)result[i].size()-4;j++){
                if(result[i][j]-'0'<0 || result[i][j]-'0'>10){
                    throw std::invalid_argument("Wrong Character Given in (symbol,state,direction) Tuple: States contain character other than numbers");
                }
            }
        }
    }
    
    //integer corresponding to HALT state is -1
    for(int i=0; i<(int)alphabet.size();i++){
        int sz=(int)result[i].size();
        int len=(int)result[i].size()-4;
        if(result[i].substr(2,len)!="HALT"){
            int num=stoi(result[i].substr(2,len));
            if(sStates.find(num)==sStates.end()){
                throw std::invalid_argument("State Is Not Contained In Initial State List");
            }
            else{
                change[std::make_pair(alphabet[i],state)].symbol=result[i][0];
                change[std::make_pair(alphabet[i],state)].state=num;
                change[std::make_pair(alphabet[i],state)].direction=result[i][sz-1];
            }
        }
        else{
            change[std::make_pair(alphabet[i],state)].symbol=result[i][0];
            change[std::make_pair(alphabet[i],state)].state=-1;
            change[std::make_pair(alphabet[i],state)].direction=result[i][sz-1];

        }
            
    }


}


std::string Turing::run(std::string& str) {

    int len = (int)str.size();

    std::deque<char> answer;
    for(int i=0; i<(int)str.size();i++){
        answer.push_back(str[i]);
    }

    int curState=0;
    char curDir;
    int curPos=0;//keeps the position of our current
    char curSymbol;

    while(1){
        answer[curPos] = change[std::make_pair(answer[curPos], curState)].symbol;
        curState=change[std::make_pair(answer[curPos], curState)].state;
        if(curState==-1){
            break;
        }
        curDir=change[std::make_pair(answer[curPos], curState)].direction;
        curSymbol=change[std::make_pair(answer[curPos], curState)].symbol;

        if(curDir=='l' && curPos==0){
            answer.push_front('.');
        }
        else if(curDir=='r' && curPos==(int)answer.size()-1){
            answer.push_back('.');
            curPos++;
        }
        else if(curDir=='l' && curPos!=0){
            curPos--;
        }
        else if(curDir=='r' && curPos!=(int)answer.size()-1){
            curPos++;
        }
    
    }

    int sz=(int)answer.size();
    int start=-1;

    for(int i=0; i<sz; i++){
        if(answer[i]!='.') {
            start=i;
            break;
        }
    }
    std::string output_string;
     if(start==-1){
        output_string="";
     }

     int end;
    
     for(int i=sz-1; i>=0; i--){
        if(answer[i]!='.'){
            end=i;
            break;
        }
     }


    for(int i=start; i<=end; i++){
        output_string.push_back(answer[i]);

    }

    return output_string;
}

void Turing::printTape(const std::deque<char>& deq){
    for(int i=0;i<(int)deq.size();i++){
        std::cout<<deq[i];
    }
    std::cout<<std::endl;

}

//if s='e' print the tape after the step
//if s='q' quit running Turing machine
void Turing::debug(std::string& str){
    int len = (int)str.size();

    std::deque<char> answer;
    for(int i=0; i<(int)str.size();i++){
        answer.push_back(str[i]);
    }

    int curState=0;
    char curDir;
    int curPos=0;//keeps the position of our current
    char curSymbol;

    std::cout<<"If you want to continue debugging step by step print 'e'"<<std::endl;
    std::cout<<"If you want to quit debugging print 'q'"<<std::endl;

    char s;
    while(1){

            std::cin>>s;

            if(s=='e'){

            answer[curPos] = change[std::make_pair(answer[curPos], curState)].symbol;
            curState=change[std::make_pair(answer[curPos], curState)].state;
            if(curState==-1){
                std::cout<<"Program Finished Successfully!!"<<std::endl;
                return;
            }
            curDir=change[std::make_pair(answer[curPos], curState)].direction;
            curSymbol=change[std::make_pair(answer[curPos], curState)].symbol;

            if(curDir=='l' && curPos==0){
                answer.push_front('.');
            }
            else if(curDir=='r' && curPos==(int)answer.size()-1){
                answer.push_back('.');
                curPos++;
            }
            else if(curDir=='l' && curPos!=0){
                curPos--;
            }
            else if(curDir=='r' && curPos!=(int)answer.size()-1){
                curPos++;
            }
            printTape(answer);
        }


        else if(s=='q'){
            std::cout<<"Turing Machine Execution aborted"<<std::endl;
            return;
        }
        else{
            std::cerr<<"Wrong Instruction"<<std::endl;
            return;
        }

    }
    
}
