#include "compare_docs.h"

int main(int argc, char *const *argv)
{

    if (argc != 4)
    {
        std::cerr << "Usage:" << std::endl
                  << argv[0] << " [word2vec_model_file_name] [sample_file_1] [sample_file_2] [minimum_similarity]" << std::endl;
        return 1;
    }

    std::vector<std::string> doc1sentences;
    std::vector<std::string> doc2sentences;

    compare_docs compare_docs_;
    std::vector<std::pair<int, int>> pairs;

    doc1sentences = compare_docs_.prepare_doc(argv[1]);
    doc2sentences = compare_docs_.prepare_doc(argv[2]);

    /*for (const auto &s : doc1sentences) {
                std::cout << "start" << s << "stop" << std::endl;
            }*/
    
    std::string::size_type sz; // alias of size_t
    float min = std::stof(argv[3], &sz);
    pairs = compare_docs_.similar_sentences(min, doc1sentences, doc2sentences);
    compare_docs_.print_similar_sentences(pairs, doc1sentences, doc2sentences);

    /*printf("*****************************************\n");
    std::string doc_string;
    doc_string = compare_docs_.load_document(argv[1]);
    compare_docs_.doc_to_sentences(doc_string);*/
}