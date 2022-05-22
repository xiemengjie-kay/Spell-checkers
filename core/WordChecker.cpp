// WordChecker.cpp
//
// ICS 46 Winter 2022
// Project #4: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"
#include "HashSet.hpp"
#include "StringHashing.hpp"


WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
    return words.contains(word);
}


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
    std::vector<std::string> suggestions;
    HashSet<std::string> checkDuplicates{hashStringAsProduct};
    unsigned int sz = word.size();
    
    // swap each adjacent pair of characters in the word
    if (word.size() >= 2)  // in order to swap, size must be at least 2
    {
        for (unsigned int i = 0; i < sz - 1; i++)
        {
            std::string alterWord = word;
            char temp = alterWord[i];
            alterWord[i] = alterWord[i+1];
            alterWord[i+1] = temp;
            if (words.contains(alterWord)) 
            {
                if (!checkDuplicates.contains(alterWord))
                {
                    checkDuplicates.add(alterWord);
                    suggestions.push_back(alterWord);
                }
            }
        }
    }

    // insert from 'A' through 'Z' in between each adjacent pair of characters
    // in the word
    for (unsigned int i = 0; i <= sz; i++)
    {
        for (unsigned int j = 0; j < 26; j++)
        {
            std::string alterWord = word;
            alterWord.insert(i, 1, letters[j]);
            if (words.contains(alterWord)) 
            {
                if (!checkDuplicates.contains(alterWord))
                {
                    checkDuplicates.add(alterWord);
                    suggestions.push_back(alterWord);
                }
            }
        }
    }

    // delete each character from the word
    for (unsigned int i = 0; i < sz; i++)
    {
        std::string alterWord = word;
        alterWord.erase(i, 1);
        if (words.contains(alterWord)) 
        {
            if (!checkDuplicates.contains(alterWord))
            {
                checkDuplicates.add(alterWord);
                suggestions.push_back(alterWord);
            }
        }
    }

    // replace each charater in the word with each letter from 'A'
    // through 'Z'
    for (unsigned int i = 0; i < sz; i++)
    {
        for (unsigned int j = 0; j < 26; j++)
        {
            std::string alterWord = word;
            alterWord[i] = letters[j];
            if (words.contains(alterWord)) 
            {
                if (!checkDuplicates.contains(alterWord))
                {
                    checkDuplicates.add(alterWord);
                    suggestions.push_back(alterWord);
                }
            }
        }
    }

    // add a space in between each adjacent pair of characters in the word
    if (sz >= 2)
    {
        for (unsigned int i = 1; i < sz; i++)
        {
            std::string space = " ";
            std::string alterWord = word;
            alterWord.insert(i, 1, space[0]);
            std::string left = alterWord.substr(0, i);
            std::string right = alterWord.substr(i+1, sz-i);
            if (words.contains(left) && words.contains(right))
            {
                if (!checkDuplicates.contains(alterWord))
                {
                    checkDuplicates.add(alterWord);
                    suggestions.push_back(alterWord);
                }
            }
        }
    }

    return suggestions;
}

