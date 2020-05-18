#include <iostream>
#include <string>
#include <vector>

void getPatternPrefix(const std::string& pattern, std::vector<int>& prefix) {
    for (int i = 1; i < pattern.size(); ++i) {
        int index = prefix[i - 1];
        while (index > 1 && pattern[index] != pattern[i])
            index = prefix[index - 1];
        if (pattern[index] == pattern[i])
            index += 1;
        prefix[i] = index;
    }
}

void printPrefix(const std::vector<int>& prefix){
    std::cout<<"Prefix: ";
    for (int number : prefix){
           std::cout << number << " ";
    }
    std::cout<<"\n";
}

void printResult(const std::vector<int>& ans){
   std::cout<<"Answers : ";
    if(!ans.empty()){
        std::string separator;
        for (int number : ans){
            std::cout << separator <<number;
            separator = ", ";
        }
    }else{
        std::cout<<"-1";
    }
    std::cout<<"\n";
}

void KMP() {
    std::cout << "Enter pattern: " << std::endl;
    std::string pattern;
    std::getline(std::cin, pattern);
    
    std::vector<int> prefix(pattern.size());
    getPatternPrefix(pattern,prefix);
    printPrefix(prefix);
    
    int index = 0;
    int counter = 0;
    std::vector<int> ans;
    
    std::cout << "Enter text: " << std::endl;
    char character;
    while (std::cin.get(character) && character != '\n') {
        counter++;
        while (index > 0 && character != pattern[index])
            index = prefix[index - 1];
        if (character == pattern[index])
            index++;
        if (index == pattern.size()) {
            ans.push_back(counter - index + 1);
        }
    }
    
    printResult(ans);
}



int main() {
    
    KMP();
    std::cout<<"\nTime Complexity O(m + n) ,where \nm – lenght of text, \nn – lenght of pattern\n";
    
    return 0;
}

