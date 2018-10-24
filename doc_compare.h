#include <fstream>
#include <iostream>
#include <sstream> 
#include <map>

#include <regex>
#include <string>
#include <vector>

#include "word2vec.hpp"
#include "tfidf.h"

class doc_compare
{
    private:
        
        std::unique_ptr<w2v::w2vModel_t> w2vModel;
        //fix the parsing efficiency.
        int stop_words_size;
        std::string* stop_words;

    public:
        doc_compare();
        ~doc_compare();

        void load_document(const std::string doc_name, std::string &doc);
        std::string* load_stop_words(const std::string doc_name, int &size);
        std::vector<std::string> split(std::string& str);
        bool inArray(std::string str, std::string *strArray, int strArraySize);
        std::string* removeStopwords(std::string *tokens, int tokensSize, std::string *stopwords, int stopwordsSize, int &size);
        std::vector<std::string> tokenize(std::string text);
        std::string de_tokenize(std::string *text, int size);
        void prepare_doc(std::string file_name, std::vector<std::string> &d);

        std::vector<std::pair<std::string, float>> doc1_sentences;
        std::vector<std::pair<std::string, float>> doc2_sentences;
        //doc2 index , doc1 index
        std::map<int, int> similar;

        void compare_docs(float minimum);
        void run_tfidf();


        void temp_main();
    
};