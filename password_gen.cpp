/* 
password_gen.cpp by Javier Mayol
C++ program that generates a password using the Diceware system by Arnold G. 
Reinhold. After populating a map<string, string> container with key = 5 digits 
number [1..6], and value = words taken from the file diceware.wordlist.asc, 
the program returns a passphrase with a default 90+bits. If an integer parameter
between 1 and 7 is specified in the command line, then the passphrase generated
will have a combination of words count specify by the parameter. 
*/

#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<sstream>
#include<map>
#include<algorithm>
#include<functional>
#include<time.h>
#include<stdexcept>

using namespace std;

std::map<string, string> input(string filename)
{
  ifstream file(filename);
  string line, key, word;

  std::map<string, string> words;

  if(!file.good())
  {
    std::cout <<"\033[1;31m**Error: file no good.\033[0m\n";
  }
 
  getline(file, line);  //skip hearder line.

  while(getline(file, line))
  {
    if(line == "-----BEGIN PGP SIGNATURE-----") break;
    stringstream ss(line);
    getline(ss, key, '\t'); 
    getline(ss, word);

    auto keyvalpair = std::pair<string, string>(key, word);
    words.insert(keyvalpair);
  }
  return words;
} 

int main (int argc, char *argv[])
{
 
  std::cout<<std::fixed;
  std::cout<<std::setprecision(2);

  auto randm_words = input("diceware.wordlist.asc");

//  for(const auto& keyval : randm_words)
  //   cout<<keyval.first<<" : "<<keyval.second<<endl;

  srand(time(NULL));

  int N = 7, dice = 0;

  string password;

  try {
    if(argc > 1)
      N = stoi(argv[1]);
  }
  catch(std::invalid_argument& e){
    cout<<"\033[1;31mThe specified argument failed.\033[0m\n";
    exit(-1);
  }
  catch(std::out_of_range& e){
    cout<<"\033[1;31mCatched out of range exception.\033[0m\n";
    exit(-2);
  }

  if(N < 1 || N > 7){
    cout<<"\033[1;31mERROR :: Argument not [1..7]\033[0m\n";
    exit(-3);
  }

  //Nested loop simulates the roll of dice.
  for(int i = 0; i < N; i++){
    for(int j = 0; j < 5; j++) 
      dice = (dice * 10) + (rand()%6 + 1); 
  //  cout<<dice<<"-";
    string key = to_string(dice);
    auto it = randm_words.find(key);
    if(it == randm_words.end())
      cout<<"not found\n";
    else
      password += it->second;
    dice = 0;
  }
  printf("\n");
  cout << "\033[0;32m"<<password <<"\033[0m\n";
  printf("\n");
  return 0;
}
