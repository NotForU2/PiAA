#include <iostream>
#include <map>
#include <vector>
#include <list>

using namespace std;

class Node
{
public:
    map<char, int> next_nodes;
    map<char, int> move;

    bool flag;
    
    int pattern_index;
    int sfx_link;
    int sfx_good_link;
    int parent;
    
    char symb;

    Node(int parent, char symb) : parent(parent), symb(symb), flag(false), sfx_link(-1), sfx_good_link(-1) {}
};

class Bor
{
public:
    vector<Node*> bor;
    vector<string> patterns;
    list<pair<int, int>> result;

    Bor()
    {
        bor.push_back(new Node(0, '$'));
    }

    void addToBor(const string& pattern)
    {
        int num = 0;
        for(char symb : pattern)
        {
            if(bor[num]->next_nodes.find(symb) == bor[num]->next_nodes.end())
            {
                bor.push_back(new Node(num, symb));
                bor[num]->next_nodes[symb] = bor.size() - 1;
            }
            num = bor[num]->next_nodes[symb];
        }
        bor[num]->flag = true;
        patterns.push_back(pattern);
        bor[num]->pattern_index = patterns.size() - 1;
    }

    int getSuffixLink(int node)
    {
        if(bor[node]->sfx_link == -1)
        {
            if(node == 0 || bor[node]->parent == 0) bor[node]->sfx_link = 0;
            else bor[node]->sfx_link = getMove(getSuffixLink(bor[node]->parent), bor[node]->symb);
        }
        return bor[node]->sfx_link;
    }

    int getMove(int node, char symb)
    {
        if(bor[node]->move.find(symb) == bor[node]->move.end())
        {
            if(bor[node]->next_nodes.find(symb) != bor[node]->next_nodes.end()) bor[node]->move[symb] = bor[node]->next_nodes[symb];
            else
            if(node == 0) bor[node]->move[symb] = 0;
            else bor[node]->move[symb] = getMove(getSuffixLink(node), symb);
        }
        return bor[node]->move[symb];
    }

    void check(int node, int i)
    {
        for(int u = node; u != 0; u = getSuffixLink(u))
        {
            if(bor[u]->flag) result.emplace_front(i + 2 - patterns[bor[u]->pattern_index].length(), bor[u]->pattern_index + 1);
        }
    }
    
    void find(const string& text)
    {
        int u = 0;
        for(int i = 0; i < text.length(); i++) 
        {
            u = getMove(u, text[i]);
            check(u, i);
        }
    }
};

int main()
{
    auto bor = new Bor();
    string text;
    string input;
    int num;
    cin >> text;
    cin >> num;
    for(int i = 0; i < num; i++)
    {
        cin >> input;
        bor->addToBor(input);
    }
    bor->find(text);
    bor->result.sort([](const pair<int, int>& a, const pair<int, int>& b)
    {
        if(a.first == b.first) return a.second < b.second;
        return a.first < b.first;
    });

    for(auto elem : bor->result) cout << elem.first << " " << elem.second << endl;
    return 0;
}