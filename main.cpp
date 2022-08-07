#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

std::vector<std::string> ReadFile()
{
    std::fstream file;
    file.open("words/2letters.txt", std::ios::in);
    if(!file.is_open()){
        std::cout << "Failed to open file " << std::endl;
    }

    std::cout << "Opened file" << std::endl;
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
        std::cout << temp.first << ": ";
        for(auto word:temp.second){
            std::cout << word << " ";
        }
        std::cout << std::endl;
    }
}

void RemoveDups(std::vector<std::string> &words)
{
    std::unordered_map<int, std::vector<std::string>> chars;
    for(auto word: words){
        int size = 0;
        for(char letter:word){
            size += (int)letter;
        }
        chars[size].push_back(word);
    }
    DisplayMap(chars);

}


int main()
{
    std::vector<std::string> words = ReadFile();
    std::cout << "Number of words: " << words.size() << std::endl;
    RemoveDups(words);
    std::cout << "Number of words: " << words.size() << std::endl;

    return 0;
}
