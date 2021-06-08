
#include <iostream>
#include "Lexico/ZDLexico.h"
#include "ZDDictionary.h"
#include "Tree/ZDTree.h"

using namespace std;
using namespace Dico;

//! @brief This this the main app
//! @param argc 
//! @param argv 
//! @return return 1 if succes
int main(int argc, char* argv[])
{
    
	string path = "./Lexico.txt";

    Lexico lexicoBase;
    if (lexicoBase.read(path))
    {
        std::cout << "number of words is " << lexicoBase.getWords().size() << std::endl;
    
        ZDDictionary dictionary;

        //add all words from data base lexico
        for (auto word : lexicoBase.getWords())
        {
            if (!dictionary.insert_word(word))
            {
                std::cout << "can not insert word" << word;
                assert(false);
            }
        }

        bool foundResult = dictionary.find_word("abaissa");

        std::cout << "word " << "abaissa" << " found  = " << foundResult << std::endl;

        foundResult = dictionary.find_word("abaissz");

        std::cout << "word " << "abaissz" << " found  = " << foundResult << std::endl; 

        
        foundResult = dictionary.find_word("abaissaient");

        std::cout << "word " << "abaissaient" << " found  = " << foundResult << std::endl;

        dictionary.remove_word("abaissaient");

        foundResult = dictionary.find_word("abaissaient");

        std::cout << "after remove word " << "abaissaient" << " found  = " << foundResult << std::endl;

        foundResult = dictionary.find_word("abaissai");

        std::cout << "after remove word " << "abaissai" << " found  = " << foundResult << std::endl;

        foundResult = dictionary.find_word("abaissa");

        std::cout << "after remove word " << "abaissa" << " found  = " << foundResult << std::endl;

        dictionary.insert_word("abaissaient");

        foundResult = dictionary.find_word("abaissaient");

        std::cout << "after add word " << "abaissaient" << " found  = " << foundResult << std::endl;

        foundResult = dictionary.find_word("azaissa", 3);

        std::cout << "find sub middle word " << "azaissa" << " found  = " << foundResult << std::endl;

        foundResult = dictionary.find_word("abaisszo", 3);

        std::cout << "find add last word " << "abaissazo" << " found  = " << foundResult << std::endl;

        foundResult = dictionary.find_word("abbaissa", 3);

        std::cout << "find add middle word " << "abbaissa" << " found  = " << foundResult << std::endl;

        foundResult = dictionary.find_word("aaissa", 3);

        std::cout << "find remove middle word " << "aaissa" << " found  = " << foundResult << std::endl;
    }
    else
    {
        std::cout << "Errro reading lexico data base"<<std::endl;
    }
      
	return 1;
}
