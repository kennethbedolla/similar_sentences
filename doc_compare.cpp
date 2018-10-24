#include "doc_compare.h"

const int MAX_STOPWORDS = 200;
const int MAX_TOKENS = 400;
const std::string DEFAULT_STOPWORDS_FILENAME = "stopwords.txt";

doc_compare::doc_compare()
{
    try
    {
        w2vModel.reset(new w2v::w2vModel_t());
        // load w2v model file
        if (!w2vModel->load("vectors.bin"))
        {
            throw std::runtime_error(w2vModel->errMsg());
        }
    }
    catch (const std::exception &_e)
    {
        std::cerr << _e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "unknown error" << std::endl;
    }
    stop_words = load_stop_words("stopwords.txt", stop_words_size);
}

doc_compare::~doc_compare()
{
}

void doc_compare::load_document(const std::string doc_name, std::string &doc)
{
    std::ifstream doc_stream;
    doc_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    doc_stream.open(doc_name);
    doc_stream.seekg(0, doc_stream.end);
    auto size = doc_stream.tellg();
    doc.resize(static_cast<std::size_t>(size), 0);
    doc_stream.seekg(0, doc_stream.beg);
    doc_stream.read(&doc[0], size);
    doc_stream.close();
}

std::string *doc_compare::load_stop_words(const std::string filename, int &size)
{
    std::string *stopwords = new std::string[MAX_STOPWORDS];
    std::string stopword;
    std::ifstream file(filename);
    size = 0;
    while (std::getline(file, stopword) && size < MAX_STOPWORDS)
    {
        stopwords[size++] = stopword;
    }
    file.close();
    return stopwords;
}

bool doc_compare::inArray(std::string str, std::string *strArray, int strArraySize)
{
    for (int i = 0; i < strArraySize; ++i)
    {
        if (str == strArray[i])
        {
            return true;
        }
    }
    return false;
}

std::string *doc_compare::removeStopwords(std::string *tokens, int tokensSize, std::string *stopwords, int stopwordsSize, int &size)
{
    std::string *tokensNoStopwords = new std::string[tokensSize];
    size = 0;
    for (int i = 0; i < tokensSize; ++i)
    {
        if (!inArray(tokens[i], stopwords, stopwordsSize))
        {
            tokensNoStopwords[size++] = tokens[i];
        }
    }
    return tokensNoStopwords;
}

std::vector<std::string> doc_compare::split(std::string &str)
{
    std::vector<std::string> sentences;
    std::regex r("([\.!\?])\s+");
    sentences = {std::sregex_token_iterator(str.begin(), str.end(), r, -1), std::sregex_token_iterator()};
    for (uint i = 0; i < sentences.size(); i++)
    {
        printf("%d %s", i, sentences[i].c_str());
    }
    return sentences;
    /*
    std::vector<std::string> elems;

    std::regex rgx(R"(<!\w\.\w.)(<![A-Z][a-z]\.)(<=\.|\?)");

    std::sregex_token_iterator iter(s.begin(), s.end(), rgx, -1);
    std::sregex_token_iterator end;

    while (iter != end)
    {
        std::cout << "S43:" << *iter << std::endl;
        elems.push_back(*iter);
        ++iter;
    }
    printf("size of elements is %d\n", elems.size());
    return elems;*/
    // the following two lines are edited; the remainder are directly from the reference.
    //std::string s(file_string);
    //std::regex e("([.,;-]|[^.,;-]+)"); // matches delimiters or consecutive non-delimiters
    /*std::regex e(R"(\w\.\w.)([A-Z][a-z]\.)(.|\?)\s)");

    std::regex_iterator<std::string::iterator> rit(str.begin(), str.end(), e);
    std::regex_iterator<std::string::iterator> rend;

    std::vector<std::string> sentences;
    while (rit != rend)
    {
        std::cout << rit->str() << std::endl;
        sentences.push_back(rit->str());
        ++rit;
    }

    return sentences;*/
}

std::vector<std::string> doc_compare::tokenize(std::string text)
{
    std::vector<std::string> tokens;
    std::regex r("\\s+");
    tokens = {std::sregex_token_iterator(text.begin(), text.end(), r, -1), std::sregex_token_iterator()};
    for (uint i = 0; i < tokens.size(); i++)
    {
        printf("%d %s\n", i, tokens[i].c_str());
    }
    return tokens;
}

std::string doc_compare::de_tokenize(std::string *text, int size)
{
    std::stringstream stream;
    for (int i = 0; i < size; i++)
    {
        stream << text[i] << ' ';
    }
    return stream.str();
}

void doc_compare::prepare_doc(std::string file_name, std::vector<std::string> &d)
{
    std::string file_string;
    load_document(file_name, file_string);
    int file_tokens_size;
    int file_tokens_no_stop_size;
    std::vector<std::string> file_tokens = tokenize(file_string);
    //std::string *file_tokens_no_stop = removeStopwords(file_tokens, file_tokens_size, stop_words, stop_words_size, file_tokens_no_stop_size);

    //doc1_sentences = split(file_string);
    doc1_sentences.push_back(make_pair("I really like going to the store.", 0.0));
    doc1_sentences.push_back(make_pair("I need to go grocery shopping.", 0.0));
    doc1_sentences.push_back(make_pair("My cousin works at a store where they sell food.", 0.0));
    doc1_sentences.push_back(make_pair("I need to go buy food to make dinner tonight.", 0.0));
    doc1_sentences.push_back(make_pair("I have dinner plans tonight.", 0.0));
    doc2_sentences.push_back(make_pair("Would you like to go the store tonight?", 0.0));
    doc2_sentences.push_back(make_pair("Do you know how to cook dinner?", 0.0));
    doc2_sentences.push_back(make_pair("I need you to go to the store to pick up some groceries for me.", 0.0));
    doc2_sentences.push_back(make_pair("What is for dinner tonight?", 0.0));
    doc2_sentences.push_back(make_pair("Who is coming over for dinner tonight?", 0.0));
}

void doc_compare::temp_main()
{
    try
    {

        std::string file = "sentences/bbc_brazil_meat.txt";
        int stop_words_size;
        std::string *stop_words;
        stop_words = load_stop_words("stopwords.txt", stop_words_size);

        std::string file1_string;
        load_document(file, file1_string);
        int file1_tokens_size;
        int file1_tokens_no_stop_size;
        std::vector<std::string> file1_tokens = tokenize(file1_string);
        //std::string *file1_tokens_no_stop = removeStopwords(file1_tokens, file1_tokens_size, stop_words, stop_words_size, file1_tokens_no_stop_size);

        //std::vector<std::string> doc1_sentences = split(de_tokenize(file1_tokens_no_stop, file1_tokens_no_stop_size));
    }
    catch (const std::exception &_e)
    {
        printf("error\n");
        std::cerr << _e.what() << std::endl;
    }
    catch (...)
    {
        printf("error\n");
        std::cerr << "unknown error" << std::endl;
    }

    //w2v::d2vModel_t d2vModel(w2vModel->vectorSize());
}

void doc_compare::compare_docs(float minimum)
{
    // create doc2vec model
    // d2vmodel is a storage model of pairs key&vector
    w2v::d2vModel_t d2vModel(w2vModel->vectorSize());
    try
    {
        //load all of the sentences to a storage model
        for (std::vector<int>::size_type i = 0; i < doc1_sentences.size(); i++)
        {
            //sentence to vector
            w2v::doc2vec_t doc2vec(w2vModel, doc1_sentences[i].first);
            //add vector with ID = i to the model
            d2vModel.set(i, doc2vec);
        }
        //compare each sentence from doc2 to the collection of sentences from doc1
        for (std::vector<int>::size_type j = 0; j != doc2_sentences.size(); j++)
        {
            w2v::doc2vec_t doc2vecd(w2vModel, doc2_sentences[j].first);

            // get nearest sentence IDs from the model
            std::vector<std::pair<std::size_t, float>> nearest;
            d2vModel.nearest(doc2vecd, nearest, d2vModel.modelSize(), minimum);
            std::cout << "for doc2 sentence " << j << "\n";
            for (auto const &i : nearest)
            {
                std::cout << i.first << ": " << i.second << std::endl;
            }
        }
    }
    catch (const std::exception &_e)
    {
        std::cerr << _e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "unknown error" << std::endl;
    }
}

void doc_compare::run_tfidf()
{
    std::vector<std::vector<std::string>> inputData;

    std::string file_string;
    load_document("sentences/sentence1.txt", file_string);

    printf("about to tokenize\n");
    printf("%s\n", file_string.c_str());
    printf("here\n");
    std::vector<std::string> file_tokens = tokenize(file_string);

    inputData.push_back(file_tokens);

    std::string file2_string;
    load_document("sentences/sentence2.txt", file2_string);
    printf("about to tokenize\n");
    printf("%s\n", file2_string.c_str());
    printf("here\n");
    std::vector<std::string> file2_tokens = tokenize(file2_string);

    inputData.push_back(file2_tokens);

    printf("number of tokens are %d\n", file_tokens.size());
    tfidf ins(inputData);
    std::vector<std::vector<double>> mat = ins.weightMat;
    cout.precision(17);
    printf("size of mat %d\n", mat.size());
    printf("size of mat[0] %d\n", mat[0].size());
    for (auto const &i : mat[0])
    {   
        std::cout << "num is: " << i << std::endl;
    }

    printf("size of mat[1] %d\n", mat[1].size());
    for (auto const &i : mat[1])
    {   
        std::cout << "num is: " << i << std::endl;
    }
}