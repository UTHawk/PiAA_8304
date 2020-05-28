#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <unordered_map>

using namespace std;

class IOManager
{
private:
    static istream* input;
    static ostream* output;
public:
    static void setStreamsFromArgs(int argc, char** argv)
    {
        if(argc > 1)
        {
            for(int i = 1; i < argc; i++)
            {
                if(strcmp(argv[i], "-infile") == 0)
                {
                    if(i + 1 < argc)
                    {
                        input = new ifstream(argv[i + 1]);
                        i += 1;
                    }
                }
                if(strcmp(argv[i], "-outfile") == 0)
                {
                    if(i + 1 < argc)
                    {
                        output = new ofstream(argv[i + 1]);
                        i += 1;
                    }
                }
            }
        }
    }
    static istream& getIS()
    {
        return *input;
    }
    static ostream& getOS()
    {
        return *output;
    }
    static void resetStreams()
    {
        if(input != & IOManager::getIS())
        {
            delete input;
            input = &IOManager::getIS();
        }
        if(output != & IOManager::getOS())
        {
            delete output;
            output = &IOManager::getOS();

        }
    }
};

class Automat
{
private:
    struct Node
    {
        unordered_map<char, int> directLinks;
        int suffixLink = 0;
        int parentLink = 0;
        bool isFinishNode = false;
        int associatedWord = -1;
    };
private:
    vector<Node> nodes;
    int currPos = 0;
private:
    void addWord(const string& word, int wordNumber);
    int getSuffixLink(int node, char name);
    void generateSuffixLinks();
    unsigned findDirectDeep(int node);
    unsigned findSuffixDeep(int node);
public:
    Automat()
    {
        //добавление корня
        nodes.push_back(Node());
    }
    void fromDict(const  vector<string>& dictionary);
    vector<int> moveTo(char direction);
};

class JokerFinder
{
private:
    int substrSize = 0;
    int partsCount = 0;
    Automat automat;
    unordered_map<string, vector<int>> dictionary;
    unordered_map<int, int> possibleEntries;
private:
    void generateDictionary(const string& substr, char joker);
    void findPossibleEntries(const string& str);
public:
    vector<int> getEntries(const string& str, const string& substr, char joker);
};

void dictionary();
void joker();

int main(int argc, char** argv)
{
    IOManager::setStreamsFromArgs(argc, argv);
    string buf;
    int choice = 0;

    IOManager::getOS() << "Enter 1 if you want to find any word from dictionary." << endl;
    IOManager::getOS() << "Enter 2 if you want to find joker" << endl;
    IOManager::getIS() >> choice;
    getline(IOManager::getIS(), buf);

    if(choice == 1)
    {
        dictionary();
    }
    else if(choice == 2)
    {
        joker();
    }

    return 0;
}

void dictionary()
{
   IOManager::getOS() << "Search from dictionary." << endl;

   Automat bohr;
   string str, buf;
   unsigned count = 0;
   vector<string> words;
   vector<pair<int, int>> entries;

   IOManager::getOS() << "Enter main string.." << endl;
   getline(IOManager::getIS(), str);
   IOManager::getOS() << "Enter word count." << endl;
   IOManager::getIS() >> count;
   getline(IOManager::getIS(), buf);

   IOManager::getOS() << "Enter words." << endl;

   for(unsigned i = 0; i < count; i++)
   {
       string word;
       getline(IOManager::getIS(), word);

       words.push_back(word);
   }

   bohr.fromDict(words);

   for(unsigned i = 0; i < str.size(); i++)
   {
       auto entrie = bohr.moveTo(str[i]);
       if(!entrie.empty())
       {
           for(unsigned j = 0; j < entrie.size(); j++)
           {
               IOManager::getOS() << "Finded word: " << words[entrie[j]] << " in pos: " << i << endl;
               entries.push_back(make_pair(i - words[entrie[j]].size() + 1, entrie[j]));
           }
       }
   }

   sort(entries.begin(), entries.end());

   IOManager::getOS() << "All entries:" << endl;

   for(auto entrie : entries)
   {
       IOManager::getOS() << entrie.first + 1 << " " << entrie.second + 1 << endl;
   }
}

void joker()
{
    IOManager::getOS() << "Joker search." << endl;
    Automat ahoCorasic;
    JokerFinder alg;
    string str;
    string substr;
    char joker;

    IOManager::getOS() << "Enter main string." << endl;
    getline(IOManager::getIS(), str);
    IOManager::getOS() << "Enter substring." << endl;
    getline(IOManager::getIS(), substr);
    IOManager::getOS() << "Enter joker." << endl;
    IOManager::getIS() >> joker;

    auto entries = alg.getEntries(str, substr, joker);

    IOManager::getOS() << "All entries:" << endl;

    for(auto entrie : entries)
    {
        IOManager::getOS() << entrie + 1 << endl;
    }
}

void Automat::addWord(const string& word, int wordNumber)
{
    IOManager::getOS() << "Adding new word in bohr: " << word << endl;
    if(word.length())
    {
        int currNode = 0;

        for(unsigned i = 0; i < word.length(); i++)
        {
            IOManager::getOS() << "   Char - " << word[i] << endl;
            if(nodes[currNode].directLinks.find(word[i]) == nodes[currNode].directLinks.end())
            {
                IOManager::getOS() << "       Creating new node."  << endl;
                nodes.push_back(Node());

                nodes[currNode].directLinks.emplace(word[i], nodes.size() - 1);
                nodes[nodes.size() - 1].parentLink = currNode;
                currNode = nodes.size() - 1;
            }
            else
            {
                IOManager::getOS() << "       Node already exiest. Moving to next node."  << endl;
                currNode = nodes[currNode].directLinks.find(word[i])->second;
            }
        }

        IOManager::getOS() << "Word added. Word number in dictionary: " << wordNumber << endl;

        if(nodes[currNode].associatedWord == -1)
        {
            nodes[currNode].associatedWord = wordNumber;
            nodes[currNode].isFinishNode = true;
        }
    }
}

void Automat::generateSuffixLinks()
{
    IOManager::getOS() << "Generating suffix links" << endl;
    //очеред из пар: вершина - суффикс
    queue<pair<char, int>> open;
    open.push(make_pair('\0', 0));

    while(!open.empty())
    {
        auto current = open.front();
        open.pop();

        for(auto link : nodes[current.second].directLinks)
        {
            open.push(link);
        }

        nodes[current.second].suffixLink = getSuffixLink(current.second, current.first);

        //если суффиксная ссылка указывает на конечную вершину автомата, то данная вершина так же будет конечной
        if(nodes[nodes[current.second].suffixLink].isFinishNode)
        {
            nodes[current.second].isFinishNode = true;
        }
    }
}

int Automat::getSuffixLink(int node, char name)
{
    IOManager::getOS() << "   Getting suffix links for node: " << name << endl;

    int currNode = nodes[node].parentLink;
    int currSuffix = nodes[currNode].suffixLink;

    IOManager::getOS() << "       Moving to parent" << endl;

    while(currNode != 0)
    {
        auto findedNode = nodes[currSuffix].directLinks.find(name);
        if(findedNode != nodes[currSuffix].directLinks.end())
        {
            IOManager::getOS() << "       Suffix finded." << endl;
            return findedNode->second;
        }
        else
        {
            IOManager::getOS() << "       Moving to current suffix" << endl;
            currNode = currSuffix;
            currSuffix = nodes[currNode].suffixLink;
        }
    }

    return 0;
}

void Automat::fromDict(const vector<string>& dictionary)
{
    IOManager::getOS() << "Generating bohr from dictionary." << endl;
    for(unsigned i = 0; i < dictionary.size(); i++)
    {
        addWord(dictionary[i], i);
    }

    generateSuffixLinks();

    IOManager::getOS() << "Bohr generated." << endl;
    IOManager::getOS() << "Direct deep: " << findDirectDeep(0) <<", suffix deep: " << findSuffixDeep(0) << endl;
}

vector<int> Automat::moveTo(char direction)
{
    vector<int> entries;

    IOManager::getOS() << "Moving to node: " << direction << endl;

    auto findedNode = nodes[currPos].directLinks.find(direction);

    while(findedNode == nodes[currPos].directLinks.end() && currPos != 0)
    {
        IOManager::getOS() << "   Direct link not fount. Moving to suffix node." << endl;
        currPos = nodes[currPos].suffixLink;
        findedNode = nodes[currPos].directLinks.find(direction);
    }

    if(findedNode != nodes[currPos].directLinks.end())
    {
        IOManager::getOS() << "   Direct link founded" << endl;
        currPos = nodes[currPos].directLinks.find(direction)->second;
    }

    int entriePos = currPos;
    while(nodes[entriePos].isFinishNode)
    {
        IOManager::getOS() << "Finish node finded." << endl;
        if(nodes[entriePos].associatedWord != - 1)
        {
            IOManager::getOS() << "   Founded word entrie: " << nodes[entriePos].associatedWord << endl;
            entries.push_back(nodes[entriePos].associatedWord);
        }

        entriePos = nodes[entriePos].suffixLink;
    }

    return entries;
}

void JokerFinder::generateDictionary(const string& substr, char joker)
{
    IOManager::getOS() << "Generating dictionary from joker." << endl;
    IOManager::getOS() << "Joker: " << substr << " char: " << joker << endl;

    string word = "";
    for(unsigned offset = 0; offset < substr.size(); offset++)
    {
        if(substr[offset] != joker)
        {
            word += substr[offset];
        }
        if(substr[offset + 1] == joker || offset + 1 >= substr.size())
        {
            if(!word.empty())
            {
                auto finded = dictionary.find(word);
                if(finded == dictionary.end())
                {
                    vector<int> offsets;

                    offsets.push_back(offset - word.size() + 1);

                    dictionary.emplace(word, offsets);

                    partsCount += 1;
                }
                else
                {
                    finded->second.push_back(offset - word.size() + 1);

                    partsCount += 1;
                }

                IOManager::getOS() << "   Part: " << word << " offset: " <<  offset - word.size() + 1 << endl;

                word = "";
            }
        }
    }

}

void JokerFinder::findPossibleEntries(const string& str)
{
    vector<string> dict;

    IOManager::getOS() << "Finding joker entries." << endl;

    for(auto pair : dictionary)
    {
        dict.push_back(pair.first);
    }

    automat.fromDict(dict);

    for(unsigned i = 0; i < str.size(); i++)
    {
        IOManager::getOS() << "Char: " << str[i] << endl;
        for(auto entrie : automat.moveTo(str[i]))
        {
            for(auto offset : dictionary.find(dict[entrie])->second)
            {
                int entriePos = i - dict[entrie].size() - offset + 1;

                if(entriePos >= 0 && entriePos + substrSize <= static_cast<int>(str.size()))
                {
                    IOManager::getOS() << "   Possible entrie founded at pos " << entriePos << endl;
                    auto possibility = possibleEntries.find(entriePos);
                    if(possibility != possibleEntries.end())
                    {
                        possibility->second += 1;
                    }
                    else
                    {
                        possibleEntries.emplace(entriePos, 1);
                    }
                }
            }
        }
    }
}

vector<int> JokerFinder::getEntries(const string& str, const string& substr, char joker)
{
    vector<int> entries;

    substrSize = substr.size();
    generateDictionary(substr, joker);
    findPossibleEntries(str);

    for(auto entrie : possibleEntries)
    {
        if(entrie.second == partsCount)
        {
            IOManager::getOS() << "Entrie founded at " << entrie.first << " entrie: " << str.substr(entrie.first, substrSize) << endl;
            entries.push_back(entrie.first);
        }
    }

    sort(entries.begin(), entries.end());

    return entries;
}

unsigned Automat::findDirectDeep(int node)
{
    unsigned deep = 0;
    if(nodes[node].directLinks.empty())
    {
        return 0;
    }
    else
    {
        for(auto link : nodes[node].directLinks)
        {
            unsigned buf = findDirectDeep(link.second);
            if(buf > deep)
            {
                deep = buf;
            }
        }

        return deep + 1;
    }
}

unsigned Automat::findSuffixDeep(int node)
{   unsigned deep = 0;
    if(node == 0)
    {
        for(unsigned i = 1; i < nodes.size(); i++)
        {
            unsigned buf = findSuffixDeep(i);
            if(buf > deep)
            {
                deep = buf;
            }
        }
    }
    else
    {
        if(nodes[node].suffixLink == 0)
        {
            return 0;
        }
        else
        {
            return findSuffixDeep(nodes[node].suffixLink) + 1;
        }
    }

    return deep;
}

istream* IOManager::input = &cin;
ostream* IOManager::output = &cout;
