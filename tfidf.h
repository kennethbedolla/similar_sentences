#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <cmath>

using namespace std;

class tfidf {
private:
	std::vector<std::vector<double> > dataMat; // converted bag of words matrix
	unsigned int nrow; // matrix row number
	unsigned int ncol; // matrix column number
	std::vector<std::vector<std::string> > rawDataSet; // raw data
	std::vector<std::string> vocabList; // all terms
	std::vector<int> numOfTerms; // used in tf calculation
	
	void createVocabList();
	inline std::vector<double> bagOfWords2VecMN(const std::vector<std::string> & inputSet);
	void vec2mat();
	inline std::vector<double> vec_sum(const std::vector<double>& a, const std::vector<double>& b);
	void calMat();

public:
	std::vector<std::vector<double> > weightMat; // TF-IDF weighting matrix
	tfidf(std::vector<std::vector<std::string> > & input):rawDataSet(input)
	{
		calMat();
	}
};