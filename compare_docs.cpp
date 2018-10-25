#include "compare_docs.h"

compare_docs::compare_docs()
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
    //stop_words = load_stop_words("stopwords.txt", stop_words_size);
}

compare_docs::~compare_docs()
{
}

// Was unable to integrate resulting matrix with doc2vec comparison for "interesting" rating syste.
void compare_docs::run_tfidf(std::string file_name1, std::string file_name2)
{

    std::vector<std::vector<std::string>> inputData;
    std::string file1_string;
    std::string file2_string;
    std::vector<std::string> doc1_tokens;
    std::vector<std::string> doc2_tokens;

    file1_string = load_document(file_name1);
    file2_string = load_document(file_name2);

    doc1_tokens = tokenize(file1_string, ' ');
    doc2_tokens = tokenize(file2_string, ' ');

    inputData.push_back(doc2_tokens);
    inputData.push_back(doc2_tokens);

    tfidf ins(inputData);
    std::vector<std::vector<double>> mat = ins.weightMat;
    cout.precision(17);
    for (auto const &i : mat.front())
    {
        if (i > 0.0)
        {
            std::cout << "num is: " << i << std::endl;
        }
    }
}


void compare_docs::replace_all(std::string &str, const std::string &from, const std::string &to)
{
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
}

//loads document, massage, tokenize.
std::vector<std::string> compare_docs::prepare_doc(std::string file_name)
{
    std::string file_string;
    std::vector<std::string> d;
    file_string = load_document(file_name);
    replace_all(file_string, "Mr.", "Mr");
    replace_all(file_string, "Mrs.", "Mrs");
    replace_all(file_string, "St.", "St");
    replace_all(file_string, "eg.", "eg");
    replace_all(file_string, "etc.", "etc");
    
    d = doc_to_sentences(file_string);

    return d;
}

//use doc2vec model to compare sentences. 
std::vector<std::pair<int, int>> compare_docs::similar_sentences(float minimum, std::vector<std::string> doc1_sentences, std::vector<std::string> doc2_sentences)
{
    std::vector<std::pair<int, int>> sentence_pairs;

    // create doc2vec model
    // d2vmodel is a storage model of pairs key&vector
    w2v::d2vModel_t d2vModel(w2vModel->vectorSize());

    for (std::vector<int>::size_type i = 0; i < doc1_sentences.size(); i++)
    {
        try
        {
            //sentence to vector
            w2v::doc2vec_t doc2vec(w2vModel, doc1_sentences[i]);
            //add vector with ID = i to the model
            d2vModel.set(i, doc2vec);
        }
        catch (...)
        {
            //printf("Sentence too small\n");
            continue;
        }
    }
    //compare each sentence from doc2 to the collection of sentences from doc1
    for (std::vector<int>::size_type j = 0; j != doc2_sentences.size(); j++)
    {
        try
        {
            w2v::doc2vec_t doc2vecd(w2vModel, doc2_sentences[j]);

            // get nearest sentence IDs from the model
            std::vector<std::pair<std::size_t, float>> nearest;
            d2vModel.nearest(doc2vecd, nearest, d2vModel.modelSize(), minimum);
            for (auto const &i : nearest)
            {
                //std::cout << "for doc2 sentence " << j << "\n";
                //std::cout << i.first << ": " << i.second << std::endl;
                sentence_pairs.push_back(std::make_pair(i.first, j));
            }
        }
        catch (...)
        {
            //printf("Sentence too small\n");
            continue;
        }
    }
    return sentence_pairs;
}

//outputs similar sentence list. 
void compare_docs::print_similar_sentences(std::vector<std::pair<int, int>> pairs, std::vector<std::string> doc1, std::vector<std::string> doc2)
{
    cout << "*************** Number of similar sentences = " << pairs.size() << " ***************" << std::endl << std::endl << std::endl;
    for (auto const &i : pairs)
    {
        std::cout << "Sentence #" <<i.first << " from first document:" << std::endl;
        std::cout << doc1[i.first] << std::endl << std::endl << std::endl;
        std::cout << "IS SIMILAR TO" << std::endl << std::endl << std::endl; 
        std::cout << "Sentence # " << i.second << " from second document:"<< std::endl;
        std::cout << doc2[i.second] << std::endl<< std::endl << std::endl;
        std::cout << "***********************************************************";
        std::cout << std::endl << std::endl << std::endl;
    }
}

std::string compare_docs::load_document(const std::string file_location)
{
    ifstream f(file_location);
    if (!f.is_open())
    {
        throw std::runtime_error("Could not load document");
    }

    std::stringstream buffer;
    buffer << f.rdbuf();

    auto data = buffer.str();
    auto leading_meta_sentinel = string("***\r\n");
    auto trailing_meta_sentinel = string("\r\n***");

    auto stripped_header = data.substr(data.find(leading_meta_sentinel) + leading_meta_sentinel.size());
    return stripped_header.substr(0, stripped_header.find(trailing_meta_sentinel));
}

//EXTREMELY basic sentence tokenization. 
std::vector<std::string> compare_docs::doc_to_sentences(std::string str)
{
    str.erase(remove(str.begin(), str.end(), '\"'), str.end());
    //str.erase(remove(str.begin(), str.end(), '\n'), str.end());
    std::vector<std::string> sentences;
    std::regex r("(\\?|\\.|\\!)");
    sentences = {std::sregex_token_iterator(str.begin(), str.end(), r, -1), std::sregex_token_iterator()};

    return sentences;
}

//tokenization used for tfidf
std::vector<std::string> compare_docs::tokenize(std::string text, char delimeter)
{
    std::vector<std::string> tokens;
    std::stringstream stream(text);
    int size = 0;
    string tok;
    while (getline(stream, tok, delimeter))
    {
        tokens.push_back(tok);
        size++;
    }
    return tokens;
}
