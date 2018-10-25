#include "compare_docs.h"

int main(int argc, char *const *argv)
{

    if (argc != 4)
    {
        std::cerr << "Usage:" << std::endl
                  << argv[0] << " [word2vec_model_file_name] [sample_file_1] [sample_file_2] [minimum_similarity-float-value]" << std::endl;
        return 1;
    }

    std::vector<std::string> doc1sentences;
    std::vector<std::string> doc2sentences;
    std::vector<std::pair<int, int>> pairs;

    compare_docs compare_docs_;
    
    doc1sentences = compare_docs_.prepare_doc(argv[1]);
    doc2sentences = compare_docs_.prepare_doc(argv[2]);

    std::string::size_type sz; // alias of size_t
    try
    {
        float min = std::stof(argv[3], &sz);
        pairs = compare_docs_.similar_sentences(min, doc1sentences, doc2sentences);
        compare_docs_.print_similar_sentences(pairs, doc1sentences, doc2sentences);
    }
    catch (exception ex)
    {
        cerr << "Could not convert minimum number to float. " << std::endl;
    }
}