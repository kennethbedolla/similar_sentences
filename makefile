CC = g++                                                                                                                                                                         
INCLUDES := $(addprefix -I, ./ ./word2vec/include)                                                                                                                                     
LIBIS := $(addprefix -L, ../bin/lib)

CFLAGS = -W -O2 -std=c++11

all: compare

compare: main.o doc_compare.o tfidf.o compare_docs.o sentence_parser.o ./word2vec/bin/lib/libword2vec.a
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) main.o doc_compare.o tfidf.o compare_docs.o sentence_parser.o ./word2vec/bin/lib/libword2vec.a -o compare

main.o: main.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c main.cpp

doc_compare.o: doc_compare.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c doc_compare.cpp

compare_docs.o: compare_docs.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c compare_docs.cpp

sentence_parser.o: sentence_parser.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c sentence_parser.cpp

tfidf_vector.o: tfidf.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c tfidf.cpp

clean: 
	$(RM) count *.o *~