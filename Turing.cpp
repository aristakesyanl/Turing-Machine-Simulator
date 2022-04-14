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
#include<csignal>
#include"Turing.h"

using namespace std;

Turing::Turing(string& fileName) {
    //delete empty lines from text file
    std::ifstream in(fileName);
    std::string line;
    vector<string> lines;

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

    //keep the number of lines
    int count = lines.size();
  
    if (count < 4) {
        throw std::invalid_argument("Invalid Text Format: Not Enough Information");
    }

    //check input symbols
    parseInput(lines[0]);

    
    //check output symbols
    parseOutput(lines[1]);

    //check states
    parseStates(lines[2]);

    //Check that the number of states is no less than lines
    //describing state changes.
    //In case of additional lines just ignore them
    //It is assumed that the first lines corresposnds
    //to the first state given in the state format
    //If for example first states are, q1,q2,q0
    //and input symbols are a,b,d
    //the first line describes correspondingly changes for
    //(1,a), (1,b), (1,d) etc.

    if(count-3<(int)states.size()){
        throw std::invalid_argument("Invalid File Format");
    }



    //check (symbol,state) pairs
    for(int i=3; i<(int)lines.size(); i++){
        parseFunction(lines[i],states[i-3]);
    }
    
 }

void Turing::parseInput(std::string str){
    //remove whitespaces
    str.erase(remove(str.begin(), str.end(),' '), str.end());
    //add ',' at the end of the string to make parsing easier
    str.push_back(',');
    
    //parse to check that every second character is ','
    //string has at least two characters(as we added one at the end)
    //so checking is valid
    for(int i=1;i<(int)str.size();i+=2){
        if(str[i]!=','){
            throw std::invalid_argument("Invalid Input Format");
        }
    }
    str.pop_back();

    //use set to check that string contains distinct characters
    //remove repeating ones
    for(int i=0; i<(int)str.size(); i+=2){
        if(sInput.find(str[i])==sInput.end()){
            inputSymbol.push_back(str[i]);
            sInput.insert(str[i]);
        }

    }
    
 }


void Turing::parseOutput(std::string str){
    //remove whitespaces
    str.erase(remove(str.begin(), str.end(),' '), str.end());
    //add ',' at the end of the string to make parsing easier
    str.push_back(',');
    
    //parse to check that every second character is ','
    //string has at least two characters(as we added one at the end)
    //so checking is valid
    for(int i=1;i<(int)str.size();i+=2){
        if(str[i]!=','){
            throw std::invalid_argument("Invalid Output Format");
        }
    }

    str.pop_back();

    //use set to check that string contains distince characters
    //remove repeating ones
    for(int i=0; i<(int)str.size(); i+=2){
        if(sOutput.find(str[i])==sOutput.end()){
            outputSymbol.push_back(str[i]);
            sOutput.insert(str[i]);
        }

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


void Turing::parseFunction(string str, int state) {

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
    if((int)result.size()<(int)inputSymbol.size()){
        throw std::invalid_argument("Not Enough Tuples Given");
    }

    //check that for each Tuple the length requirement is met
    for(int i=0; i<(int)result.size();i++){
        if(result[i].size()<5){
            throw std::invalid_argument("Invalid (symbol,state,direction) Tuple");
        }
    }


    //check that in all symbols are from output
    for(int i=0; i<(int)result.size();i++){
        if(sOutput.find(result[i][0])==sOutput.end()){
            throw std::invalid_argument("Symbol Is Not Contained In Initial Output Set");
        }
    }

    //check that every second character for tuple is ','
    for(int i=0; i<(int)result.size();i++){
        if(result[i][1]!=','){
            throw std::invalid_argument("Wrong Form of Character Given In (symbol,state, direction) Tuple");
        }
    }

    //check that directions are given
    for(int i=0; i<(int)result.size();i++){
        int sz=(int)result.size();
        if(result[i][sz-2]!=','){
            throw std::invalid_argument("Wrong Form of Character Given In (symbol,state, direction) Tuple");
        }
        if(result[i][sz-1]!='r' && result[i][sz-1]!='l' && result[i][sz-1]!='s'){
            throw std::invalid_argument("Directions Not Given In (symbol, state, direction) Tuple");
        }
    }

   
    
    for(int i=0; i<(int)result.size();i++){
        std::string nstr=result[i].substr(3,(int)result[i].size()-4);
        if(nstr!="HALT"){
                for(int j=3; j<(int)result[i].size()-4;j++){
                if(result[i][j]-'0'<0 || result[i][j]-'0'>10){
                    throw std::invalid_argument("Wrong Character Given in (symbol,state,direction) Tuple");
                }
            }
        }
    }
    
    //integer corresponding to HALT state is -1
    for(int i=0; i<(int)result.size();i++){
        int sz=(int)result[i].size();
        for(int j=3; j<(int)result[i].size()-4;j++){
            int len=(int)result[i].size()-4;
            if(result[i].substr(3,len)!="HALT"){
                int num=stoi(result[i].substr(3,len));
                if(sStates.find(num)==sStates.end()){
                    throw std::invalid_argument("State Is Not Contained In Initial State List");
                }
                else{
                    change[std::make_pair(inputSymbol[i],state)].symbol=result[i][0];
                    change[std::make_pair(inputSymbol[i],state)].state=num;
                    change[std::make_pair(inputSymbol[i],state)].direction=result[i][sz-1];
                }
            }
            else{
                change[std::make_pair(inputSymbol[i],state)].symbol=result[i][0];
                change[std::make_pair(inputSymbol[i],state)].state=-1;
                change[std::make_pair(inputSymbol[i],state)].direction=result[i][sz-1];

            }
            
        }
    }

}


std::string Turing::run(std::string& str) {

    int len = str.size();

    deque<char> answer;
    for(int i=0; i<(int)str.size();i++){
        answer.push_back(str[i]);
    }

    int curState=0;
    char curDir;
    int curPos=0;//keeps the position of our current
    char curSymbol;

    while(curState!=-1){
        answer[curPos] = change[make_pair(answer[curPos], curState)].symbol;
        curState=change[make_pair(answer[curPos], curState)].state;
        curDir=change[make_pair(answer[curPos], curState)].direction;
        curSymbol=change[make_pair(answer[curPos], curState)].symbol;

        if(curDir=='l' && curPos==0){
            answer.push_front('.');
        }
        else if(curDir=='r' && curPos==answer.size()-1){
            answer.push_back('.');
            curPos++;
        }
        else if(curDir=='l' && curPos!=0){
            curPos--;
        }
        else{
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
        cout<<deq[i];
    }
    cout<<endl;

}

//if s='e' print the tape after the step
//if s='q' quit running Turing machine
void Turing::debug(std::string& str){
    int len = str.size();

    std::deque<char> answer;
    for(int i=0; i<(int)str.size();i++){
        answer.push_back(str[i]);
    }

    int curState=0;
    char curDir;
    int curPos=0;//keeps the position of our current
    char curSymbol;

    char s;
    while(cin>>s){

            if(s=='e'){
            answer[curPos] = change[make_pair(answer[curPos], curState)].symbol;
            curState=change[make_pair(answer[curPos], curState)].state;
            curDir=change[make_pair(answer[curPos], curState)].direction;
            curSymbol=change[make_pair(answer[curPos], curState)].symbol;

            if(curDir=='l' && curPos==0){
                answer.push_front('.');
            }
            else if(curDir=='r' && curPos==answer.size()-1){
                answer.push_back('.');
                curPos++;
            }
            else if(curDir=='l' && curPos!=0){
                curPos--;
            }
            else{
                curPos++;
            }
            printTape(answer);
            if(curState==-1){
                std::cout<<"Program Finished Successfully"<<std::endl;
                return;
            }
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
