#ifndef EMAIL_TRIE_H
#define EMAIL_TRIE_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
class EmailTrie{
    public:
        class TrieNode;
    private:
        TrieNode* root;
        void dfs(TrieNode* node, const std:: string& prefix, std:: vector<std::pair<std::string, int>>& suggestions){
            if(node -> isEndWord){
                node -> frequency ++;
                suggestions.emplace_back(prefix, node -> frequency);
            }
            for(auto& pair : node->children) {
                dfs(pair.second, prefix + pair.first, suggestions);
            }
        }
        bool removeHelper(TrieNode* node, const std::string& key, int depth) {
            if (!node) return false;
            if (depth == key.size()) {
                if (!node->isEndWord) return false;
                node->isEndWord = false;
                return node->children.empty();
            }
            char c = key[depth];
            auto it = node->children.find(c);
            if (it == node->children.end()) return false;
            bool shouldDeleteChild = removeHelper(it->second, key, depth + 1);
            if (shouldDeleteChild) {
                delete it->second;
                node->children.erase(it);
                return !node->isEndWord && node->children.empty();
            }
            return false;
       }
        
       public:
        EmailTrie(): root(new TrieNode()){}
        ~EmailTrie(){delete root;}
        // insert
        virtual  void  insert(const std:: string& email);
        // sreach
        virtual std :: vector<std :: string> searchList(std:: string& key);
        virtual bool sreachPrefix( std:: string& key);
        //deletion
        virtual bool remove(const std:: string& key);
    public:
        class TrieNode{
            public:
                std:: unordered_map<char, TrieNode*> children;
                int frequency;
                bool isEndWord;
            TrieNode(): isEndWord(false), frequency(0){}
            // TrieNode(int f, bool end, std:: unordered_map<char, TrieNode*> child): frequency(f), isEndWord(end), children(child){}
            ~TrieNode(){
                for(auto& p : children)
                    delete p.second;
            }

        };

};
class AdvancedEmailTrie : public EmailTrie {
private:
    std::unordered_map<std::string, int> emailFrequency;
    
    // Thêm cấu trúc để hỗ trợ infix/postfix search
    std::vector<std::string> allEmails; // Lưu trữ tất cả email
    
public:
    void insert(const std::string& email) override {
        EmailTrie::insert(email);
        emailFrequency[email]++;
        allEmails.push_back(email);
    }
    
    using EmailTrie::searchList;
    
    // Tìm kiếm infix (chứa chuỗi con bất kỳ)
    std::vector<std::string> searchInfix(const std::string& infix);
    // Lấy email phổ biến nhất (giữ nguyên như cũ)
    std::vector<std::string> getMostFrequent(int limit );
};
//===============================DEFINE-METHOD=================================//
void EmailTrie::insert( const std:: string& email){
   
    TrieNode* node = root;
        for(char c : email) {
            if(node->children.find(c) == node->children.end()) {
                node->children[c] = new TrieNode();
            }
            node = node->children[c];
        }
        node->isEndWord = true;
        node->frequency++;
   
 
}
bool EmailTrie:: sreachPrefix( std:: string& key){
            TrieNode* cur = root;
            for(auto c : key){
                if(cur -> children.find(c) == cur -> children.end()){
                    return false;
                }
                cur = cur -> children[c];
            }
            return true;
}
    
std :: vector<std :: string> EmailTrie:: searchList( std:: string& key){
    TrieNode* node = root;
    for(auto c : key){
        if(node -> children.find(c) == node -> children.end()){
            return std :: vector<std:: string>();
        }
        node = node -> children[c];
    }
    std::vector<std::pair<std::string, int>> suggestions;
        dfs(node, key, suggestions);
        
        // Sắp xếp theo tần suất giảm dần
        std::sort(suggestions.begin(), suggestions.end(),
            [](const auto& a, const auto& b) {
                return b.second < a.second;
            });
        
        std::vector<std::string> result;
        for(const auto& pair : suggestions) {
            result.push_back(pair.first);
        }
        
        return result;
}
bool EmailTrie:: remove(const std:: string& key){
   return removeHelper(root, key, 0);
}
//=================================AdvanceTrie DEFINING======================//
std::vector<std::string> AdvancedEmailTrie:: searchInfix(const std::string& infix) {
        std::vector<std::string> result;
        for (const auto& email : allEmails) {
            if (email.find(infix) != std::string::npos) {
                result.push_back(email);
            }
        }
        
        // Sắp xếp theo frequency
        std::sort(result.begin(), result.end(), 
            [this](const std:: string& a, const std:: string& b) {
                return emailFrequency.at(a) > emailFrequency.at(b);
            });
        
        return result;
}
std::vector<std::string> AdvancedEmailTrie:: getMostFrequent(int limit ) {
        std::vector<std::pair<std::string, int>> sortedEmails(
            emailFrequency.begin(), emailFrequency.end());
            
        std::sort(sortedEmails.begin(), sortedEmails.end(),
            [](const auto& a, const auto& b) {
                return b.second < a.second;
            });
        
        std::vector<std::string> result;
        for(int i = 0; i < std::min(limit, (int)sortedEmails.size()); ++i) {
            result.push_back(sortedEmails[i].first);
        }
        
        return result;
}
#endif