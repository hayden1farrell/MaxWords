#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <unordered_map>
#include <set>

std::vector<std::string> ReadFile(const char* filePath)
{
    std::fstream file;
    file.open(filePath, std::ios::in);
    if(!file.is_open()){
        std::cout << "Failed to open file " << std::endl;
    }

    std::cout << "Opened file: " << filePath << std::endl;
    std::vector<std::string> words;
    std::string line;
    while(getline(file, line))
    {
        size_t pos = 0;
        bool space = (line.find(" ") != std::string::npos);
        if(space == false){
            std::string::iterator end_pos = std::remove(line.begin(), line.end(), ' ');
            line.erase(end_pos, line.end());
            words.push_back(line);
        }
        else{
            while (space) {
                pos = line.find(" ");
                words.push_back(line.substr(0, pos));
                line.erase(0, pos + 1);
                space = (line.find(" ") != std::string::npos);
            }
        }
    }
    
    return words;
}

void DisplayMap(std::unordered_map<int, std::vector<std::string>> chars)
{
    for(auto temp:chars){
        std::cout << temp.first << ": "<< std::endl << "\t";
        for(auto word:temp.second){
            std::cout << word << ", ";
        }
        std::cout << std::endl;
    }
}

bool FindSimilar(std::string curWord, std::string word)
{
    int minSize = std::min(curWord.size(), word.size());
    for(int index = 0; index < minSize; index++)
    {
        bool same = false;
        int pos = -1;
        for(int curChar = 0; curChar < word.size(); curChar++)
        {
            if(curWord[index] == word[curChar])
            {
                same = true;
                pos = curChar;
            }
        }
        if(same == false)
            return false;
        else
            word.erase(pos, 1);
    }
    return true;
}

std::vector<std::string> RemoveDups(std::vector<std::string> &words)
{
    std::unordered_map<int, std::vector<std::string>> chars;
    for(auto word: words){
        int size = 0;
        for(char letter:word){
            size += (int)letter;
        }
        chars[size].push_back(word);
    }
    //DisplayMap(chars);

    for(auto sets:chars){
        if(sets.first > 1)
        {
            auto wordSets = sets.second;
            int size = wordSets.size();
            for(int i = 0; i < size; i++){
                std::string curWord = wordSets[i];
                for(int j = i+1; j<size; j++)
                {
                    bool similar = FindSimilar(curWord, wordSets[j]);
                    if(similar){
                        wordSets.erase(wordSets.begin() + j);
                        break;
                    }
                }
            }
            chars[sets.first] = wordSets;
        }
    }
    std::vector<std::string> temp;
    for(auto wo:chars){
        for(int i = 0; i < wo.second.size(); i++){
            temp.push_back(wo.second[i]);
        }
    }

    return temp;
}

void RemoveDoubles(std::vector<std::string> &words)
{
    for(int i = 0; i < words.size(); i++){
        std::set<std::string> string_set;
        for (char c: words[i])
            string_set.insert(std::string{c});
        if(words[i].size() != string_set.size())
            words.erase(words.begin() + i);
    }
}


int main()
{
    const char* filePath = "words/4letters.txt";

    auto currentTime = std::chrono::high_resolution_clock::now();
    std::vector<std::string> words = ReadFile(filePath);
    auto newTime = std::chrono::high_resolution_clock::now();
    float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
    std::cout << "Time to read (seconds) : " << frameTime << std::endl;
    std::cout << "Number of words: " << words.size() << "\n" << std::endl;


    currentTime = std::chrono::high_resolution_clock::now();
    RemoveDoubles(words);
    newTime = std::chrono::high_resolution_clock::now();
    frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
    std::cout << "Time to remove doubles (seconds) : " << frameTime << std::endl;
    std::cout << "Number of words (after doubles gone): " << words.size() << "\n" << std::endl;


    currentTime = std::chrono::high_resolution_clock::now();
    words = RemoveDups(words);
    newTime = std::chrono::high_resolution_clock::now();
    frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
    std::cout << "Time to remove similar (seconds) : " << frameTime << std::endl;
    std::cout << "Number of words (^ + similar gone): " << words.size() << "\n" << std::endl;

    return 0;
}
