# similar_sentences


### Building
You will need C++11 compatible compiler and cmake 3.1 or higher.
First you need to build word2vec library. You can read the README inside word2vec or do the following.
Execute the following commands:
1. `cd word2vec`
2. `mkdir build`
3. `cd build`
4. `cmake -DCMAKE_BUILD_TYPE=Release ../`
5. `make`
6. You can optionally train the model as described in the word2vec and replace vectors.bin that is already provided. 

Now you can build similar_sentences. 
Execute the following commands:
1. `cd similar_sentences`
2. `make`

### Basic usage
`./compare books/frankenstein.txt books/pride-and-prejudice.txt .95`
The `.95` is a minimum similarity float value. Anything above .93 gives best results.