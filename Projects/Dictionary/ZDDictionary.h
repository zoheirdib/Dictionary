#pragma once

#include <vector>
#include <algorithm>
#include <tuple>
#include <locale>
#include "Tree/ZDTree.h"

namespace Dico
{
    //! @brief this class encapsulate the dictionary fonctions
    class ZDDictionary
    {

    public:
        //! @brief default constructor
        ZDDictionary()
        {
            //add all alphabetic entries as a root

            ZDTree<char>::iterator head;
            head = m_internalTree.begin();

            for (auto charr : FrenchAlphabet)
            {
                m_internalTree.insert(head, charr);
            }
        };

        //! @brief distructor
        ~ZDDictionary()
        {

        }

        //! @brief insert a new word to the dictionary
        //! @param word the given word to be inserted
        //! @return return true if succes, false otherwise
        bool insert_word(std::string word)
        {
            bool result = false;

            //convert the input word to lower case
            word = to_lower_case_word(word);

            int count = 1;

            //find the root word 
            for (auto charr : word)
            {
                ZDTree<char>::sibling_iterator sib = m_internalTree.begin();

                while (sib != m_internalTree.end())
                {
                    if ((*sib) == charr)
                    {
                        //insert the current word
                        insert_word(m_internalTree, ZDTree<char>::iterator(sib.node), word.erase(0, count));
                        result = true;
                        break;
                    }
                    ++sib;
                }

                if (result)
                {
                    break;
                }
                else
                {
                    ++count;
                }

            }

            return result;
        };

        //! @brief remove a word from the dictionary
        //! @param word the word to be removed
        //! @return true if succes , false otherwise
        bool remove_word(std::string word)
        {
            bool result = false;

            //convert the input word to lower case
            word = to_lower_case_word(word);

            int count = 1;

            //find the root word 
            for (auto charr : word)
            {
                ZDTree<char>::sibling_iterator sib = m_internalTree.begin();
                while (sib != m_internalTree.end())
                {
                    if ((*sib) == charr)
                    {
                        //find the word
                        auto found = find_word(m_internalTree, ZDTree<char>::iterator(sib.node), word.erase(0, count));
                        if (std::get<bool>(found))
                        {
                            //remove the work
                            result = remove_word(m_internalTree, std::get<ZDTree<char>::iterator>(found), ZDTree<char>::iterator(sib.node));
                        }
                    }
                    ++sib;
                }

                if (result)
                {
                    break;
                }
                else
                {
                    ++count;
                }
            }

            return  result;
        }

        //! @brief find if a word exist in the dictionary , this methode is using safe founding; this is leas that no errors are allowed
        //! @param word th e word to be found
        //! @return true if the word is found, false otherwise
        bool find_word(std::string word)
        {
            bool result = false;

            //convert the input word to lower case
            word = to_lower_case_word(word);

            int count = 1;

            //find the root word 
            for (auto charr : word)
            {
                ZDTree<char>::sibling_iterator sib = m_internalTree.begin();
                while (sib != m_internalTree.end())
                {
                    if ((*sib) == charr)
                    {
                        result = std::get<bool>(find_word(m_internalTree, ZDTree<char>::iterator(sib.node), word.erase(0, count)));
                        if (result)
                        {
                            break;
                        }
                    }
                    ++sib;
                }

                if (result)
                {
                    break;
                }
                else
                {
                    ++count;
                }
            }

            return  result;
        }

        //! @brief find if a word existe in the dictionary, this methode allow no safe founding, this is mean errors (addition, deletio, substitution) may be allowed
        //! @param word the word to be found
        //! @param max_error the maximum number of errors
        //! @return true if the word is found, false otherwise
        bool find_word(std::string word, int max_error)
        {
            bool result = false;
            int count_error = 0;
            //convert the input word to lower case
            word = to_lower_case_word(word);

            int count = 1;

            //find the root word 
            for (auto charr : word)
            {
                ZDTree<char>::sibling_iterator sib = m_internalTree.begin();
                while (sib != m_internalTree.end() && count_error < max_error)
                {
                    if ((*sib) == charr)
                    {
                        result = std::get<bool>(find_word(m_internalTree, ZDTree<char>::iterator(sib.node), word.erase(0, count), max_error - count_error));
                        if (result)
                        {
                            break;
                        }
                    }
                    ++sib;
                }

                if (result)
                {
                    break;
                }
                else
                {
                    ++count;
                    ++count_error;
                }
            }

            return  result;
        }



    private:

        //! @brief insert a new work to a given dictionary starting at a given root
        //! @param tr the given dictionary
        //! @param node the starting root
        //! @param word th e word to be insert
        //! @return the last node where the inserted word finish
        static inline ZDTree<char>::iterator insert_word(ZDTree<char>& tr, ZDTree<char>::iterator& node, const std::string& word)
        {
            ZDTree<char>::iterator currentNode = node;

            for (auto charr : word)
            {
                auto result = has_child(tr, currentNode, charr);

                // if the char is not found in the child of the current node
                if (!std::get<bool>(result))
                {
                    //Add the char to the curent node
                    currentNode = tr.append_child(currentNode, charr);
                }
                //if found, make the founded child a current node
                else
                {
                    currentNode = std::get<ZDTree<char>::iterator>(result);
                }
            }

            return currentNode;
        };

        //! @brief find a word in a given dictionary, staring at a given root node
        //! @param tr the given dictionary
        //! @param node the given root
        //! @param word the word to be found
        //! @return a tuple with the following value : 
        //! -   bool                : true if the word is found, false othserwise
        //! - ZDTree<char>::iterator  : if founded, the node corresponding to the last char of the given word is the dictionary, otherwise null_ptr 
        static inline  std::tuple<bool, ZDTree<char>::iterator> find_word(ZDTree<char>& tr, ZDTree<char>::iterator& node, const std::string& word)
        {
            std::tuple<bool, ZDTree<char>::iterator> found(false, nullptr);

            ZDTree<char>::iterator currentNode = node;

            for (auto charr : word)
            {
                found = has_child(tr, currentNode, charr);
                // if the char is found in the child of the current node
                if (std::get<bool>(found))
                {
                    currentNode = std::get<ZDTree<char>::iterator>(found);
                }
                //if not found, the work dont exist
                else
                {
                    found = std::tuple<bool, ZDTree<char>::iterator>(false, nullptr);
                    break;
                }
            }

            return found;
        }

        //! @brief find a word in a given dictionary, staring at a given root node and allowing a number of errors
        //! @param tr the given dictionary
        //! @param node the given root
        //! @param word the word to be found
        //! @param max_error the mawimum allow number of errors (addition, delection susbtitution)
        //! @return a tuple with the following value : 
        //! - bool                  : true if the word is found, false othserwise
        //! - ZDTree<char>::iterator  : if founded, the node corresponding to the last found char of the given word is the dictionary, otherwise null_ptr 
        static inline  std::tuple<bool, ZDTree<char>::iterator, int> find_word(ZDTree<char>& tr, ZDTree<char>::iterator& node, std::string& word, int max_error)
        {
            std::tuple<bool, ZDTree<char>::iterator> found_child(false, nullptr);
            std::tuple<bool, ZDTree<char>::iterator, int> las_found(false, nullptr, -1);

            std::tuple<bool, ZDTree<char>::iterator, int> found_sub(false, nullptr, -1);
            std::tuple<bool, ZDTree<char>::iterator, int> found_add(false, nullptr, -1);
            std::tuple<bool, ZDTree<char>::iterator, int> found_del(false, nullptr, -1);

            ZDTree<char>::iterator currentNode = node;

            int count = 1;
            if (word.size() == 0)
            {
                return las_found;
            }

            for (auto charr : word)
            {
                if (max_error > 0)
                {
                    found_child = has_child(tr, currentNode, charr);
                    // if the char is found in the child of the current node
                    if (std::get<bool>(found_child))
                    {
                        //get the next node
                        currentNode = std::get<ZDTree<char>::iterator>(found_child);

                        //save the lastest founded word
                        las_found = std::tuple<bool, ZDTree<char>::iterator, int>(std::get<0>(found_child), std::get<1>(found_child), max_error);
                    }
                    //if char is not found, max_error is decreased, search continue:
                        //-   with the next depth and the next char (case of substutuation of char)
                        //-   with the same depth and the next char (case of add char)
                        //-   with the next depth and the same char (case of deleate char)
                    else
                    {
                        --max_error;

                        found_sub = find_word(tr, currentNode, word.substr(count, word.size()), max_error);
                        found_add = find_word(tr, currentNode, word.substr(count, word.size()), max_error);
                        found_del = find_word(tr, ZDTree<char>::iterator(currentNode.node->first_child), word.substr(count - 1, word.size()), max_error);

                        las_found = found_add;

                        if (std::get<0>(found_sub)) las_found = found_sub;

                        if (std::get<0>(found_add) && std::get<2>(found_add) > std::get<2>(las_found)) las_found = found_add;

                        if (std::get<0>(found_del) && std::get<2>(found_del) > std::get<2>(las_found)) las_found = found_del;

                        break;
                    }
                }
                else
                {
                    las_found = std::tuple<bool, ZDTree<char>::iterator, int>(false, nullptr, -1);
                }

                ++count;
            }

            return las_found;
        }

        //! @brief remove a word from a giveen dicionary , the word to be removed is identified by the pre_begin_node and pre_end_node
        //! @param tr the given dictionary
        //! @param pre_begin_node the most deeper node of the word in the dictionary
        //! @param pre_end_node the most shallow node of the word in the dictionary
        //! @return  true if the word was sucefully removed , false otherwise
        static inline bool remove_word(ZDTree<char>& tr, ZDTree<char>::iterator& pre_begin_node, ZDTree<char>::iterator& pre_end_node)
        {
            ZDTree<char>::pre_order_iterator pre = pre_begin_node;
            ZDTree<char>::pre_order_iterator toArase = pre_begin_node;
            while (pre != pre_end_node)
            {
                if (!has_child(pre.node))
                {
                    toArase = pre;
                    --pre;
                    tr.erase(toArase);
                }
                else
                {
                    break;
                }
            }

            return true;
        }

        //! @brief check if a node has a leat one child
        //! @param node 
        //! @return 
        static inline bool has_child(const ZDTree<char>::iterator_base& node)
        {
            return (node.node->first_child != 0 || node.node->last_child != 0);
        };

        //! @brief check if a node of a given ZDTree has a child contraining a given char
        //! @param tr the given dictionary
        //! @param node the given node
        //! @param data the given char
        //! @return a tuple with the following value : 
        //! - bool                  : true if the char is found, false othserwise
        //! - ZDTree<char>::iterator  : if founded, the node corresponding to the child countaining the given char
        static inline std::tuple<bool, ZDTree<char>::iterator> has_child(const ZDTree<char>& tr, ZDTree<char>::iterator& node, char data)
        {
            std::tuple<bool, ZDTree<char>::iterator> result(false, nullptr);

            if (has_child(node))
            {
                ZDTree<char>::sibling_iterator sib = tr.begin(node.node);
                while (sib != tr.end(node.node))
                {
                    if ((*sib) == data)
                    {
                        return std::tuple<bool, ZDTree<char>::iterator>(true, sib);
                    }
                    ++sib;
                }
            }

            return result;
        };

        //! @brief convert from upper case string to lower case string, string sould we ansi
        //! @param upperCase the given string in upper case
        //! @return the string in lower case
        static inline std::string to_lower_case_word(const std::string upperCase)
        {
            std::string lowerCase;
            lowerCase.resize(upperCase.size());

            std::transform(upperCase.begin(),
                upperCase.end(),
                lowerCase.begin(),
                ::tolower);

            return lowerCase;
        };

        //! @brief this is a helper vector to stor alphabetic later, used in the initialiszation of the dictionary
        std::vector<char> FrenchAlphabet = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n',
                                       'o','p','q','r','s','t','u','v','w','x','y','z' };

        //! @brief this the internal ZDTree used to manage the dictionary as a ZDTree.
        //! each wod is stored in the ZDTree as follow :
        //! root
        //!
        //!    +----A          
        //!    |    |
        //!    |    R----B-----R------E
        //!    |    |
        //!    |    A----N-----G------R 
        //!    |
        //!    +----B----E-----B------E
        //!    |    |
        //!    |    I----B-----E------R-----O-----N
        //!    |         
        //!    +----C----I-----N------E-----M-----A
        //!    |
        //!    +----N----E-----T------F-----L-----I-----X
        //!         |
        //!         A----N-----O------M-----E------T------T-----R-----E
        ZDTree<char> m_internalTree;
    };
}

