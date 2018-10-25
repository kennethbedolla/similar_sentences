#include <fstream>
#include <iostream>
#include <sstream> 
#include <map>
#include <algorithm>

#include <regex>
#include <string>
#include <vector>

#include "word2vec.hpp"
#include "tfidf.h"

class compare_docs
{
    private:
        
        std::unique_ptr<w2v::w2vModel_t> w2vModel;

    public:
        compare_docs();
        ~compare_docs();

        std::vector<std::string> prepare_doc(std::string file_name);
        void run_tfidf(std::string file_name1, std::string file_name2);
        std::vector<std::pair<int,int> > similar_sentences(float minimum, std::vector<std::string> doc1_sentences, std::vector<std::string> doc2_sentences);
        void print_similar_sentences(std::vector<std::pair<int,int> > pairs, std::vector<std::string> doc1, std::vector<std::string> doc2);
        std::vector<std::string> doc_to_sentences(std::string str);
        std::string load_document(const std::string file_location);
        std::vector<std::string> tokenize(std::string text, char delimeter);
        void replace_all(std::string &str, const std::string& from, const std::string& to); 
           
};