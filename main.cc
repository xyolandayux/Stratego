#include <iostream>
#include <string>
#include "grid.h"
#include "state.h"
#include <fstream>
#include <sstream>
#include <unistd.h>
using namespace std;

// Do not remove any code; do not add code other than where indicated.

int main(int argc, char *argv[]) {
   bool graphics_enabled = false;

    std::string ability1 = "LFDSP";
    std::string ability2 = "LFDSP";
    std::string link1 = "V1V2V3V4D1D2D3D4";
    std::string link2 = "V1V2V3V4D1D2D3D4";

   for(int arg_num = 1; arg_num < argc; arg_num++){
       std::string cur_arg = argv[arg_num];

       if(cur_arg == "-ability1"){
           ability1 = argv[arg_num + 1];
           if(ability1.size() != 5){
               cout << "Invalid ability1 argument, try again!" << endl;
               return 1;
           }

           int abs_count[5] = {0,0,0,0,0};
           char c_array[5];
           int counter = 0;
           for(char& ability: ability1){
               c_array[counter] = ability;
               counter++;
           }
           for(int i = 0; i < 5; i++){
               for(int j = 0; j < 5; j++){
                   if(i == j) continue;
                   if(c_array[i] == c_array[j]){
                       abs_count[i]++;
                   }
               }
           }
           if(abs_count[0] > 1 || abs_count[1] > 1 || abs_count[2] > 1 || abs_count[3] > 1 || abs_count[4] > 1 ){
               cout << "Invalid ability1 argument, try again!" << endl;
               return 1;
           }
           arg_num++;

       } else if(cur_arg == "-ability2"){
           ability2 = argv[arg_num + 1];
           if(ability2.size() != 5){
               cout << "Invalid ability2 argument, try again!" << endl;
               return 1;
           }

           int abs_count[5] = {0,0,0,0,0};
           char c_array[5];
           int counter = 0;
           for(char& ability: ability2){
               c_array[counter] = ability;
               counter++;
           }
           for(int i = 0; i < 5; i++){
               for(int j = 0; j < 5; j++){
                   if(i == j) continue;
                   if(c_array[i] == c_array[j]){
                       abs_count[i]++;
                   }
               }
           }
           if(abs_count[0] > 1 || abs_count[1] > 1 || abs_count[2] > 1 || abs_count[3] > 1 || abs_count[4] > 1 ){
               cout << "Invalid ability2 argument, try again!" << endl;
               return 1;
           }
           arg_num++;
       } else if(cur_arg == "-link1"){
           link1 = argv[arg_num + 1];
           arg_num++;
       } else if(cur_arg == "-link2"){
           link2 = argv[arg_num + 1];
           arg_num++;
       } else if(cur_arg == "-graphics"){
           graphics_enabled = true;
       }

   }

    cin.exceptions(ios::eofbit|ios::failbit);
    string cmd;
    Grid g(ability1, ability2, link1, link2, graphics_enabled);
    
    istream *in = &cin;
    ifstream testCasestream;

    unsigned int microseconds = 15000000;

    try {
        while (*in >> cmd) {
            if (cmd == "board") {
                //Grid g(ability1, ability2, link1, link2);
                cout << g;
            } else if (cmd == "move") {
                char c;
                string dir;
                *in >> c >> dir;
                if (g.move(c, dir)){
                    usleep(microseconds);
                    return 0;
                } 
            } 
            else if(cmd == "sequence") {
              string tcname;
              cin >> tcname;
              testCasestream.open(tcname);
              in = &testCasestream;
            }
            else if (cmd == "abilities") {
                g.displayAbilities();
            } else if (cmd == "ability"){
                int ability_num;
                *in >> ability_num;
                
                string ability_args;
                getline(*in, ability_args);
                stringstream ss{ability_args};
                
                std::vector<char> args;
                char cur_char;
                while(ss >> cur_char){
                    args.emplace_back(cur_char);
                }
                
                if(args.size() == 0){
                    if(g.useAbility(ability_num - 1)){
                        usleep(microseconds);
                        return 0;
                    }
                } else if (args.size() == 1){
                    if(g.useAbility(ability_num - 1, args.at(0))){
                        usleep(microseconds);
                        return 0;
                    }
                } else if (args.size() == 2){
                    if(g.useAbility(ability_num - 1, args.at(0), args.at(1))){
                        usleep(microseconds);
                        return 0;
                    }
                } else {
                    cerr << "Invalid Input, Try Again" << endl;
                }
                
            } else if (cmd == "quit"){
                return 0;
            }
            
        }
    }
    catch (ios::failure &) {}  // Any I/O failure quits
}

