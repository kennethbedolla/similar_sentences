CC = g++                                                                                                                                                                         
INCLUDES := $(addprefix -I, ./ ./word2vec/include)                                                                                                                                     
LIBIS := $(addprefix -L, ../bin/lib)

CFLAGS = -W -O2 -std=c++11 -Wc++11-extensions

all: compare

compare: main.o tfidf.o compare_docs.o ./word2vec/bin/lib/libword2vec.a
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) main.o tfidf.o compare_docs.o ./word2vec/bin/lib/libword2vec.a -o compare

main.o: main.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c main.cpp

compare_docs.o: compare_docs.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c compare_docs.cpp

tfidf_vector.o: tfidf.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c tfidf.cpp

clean: 
	$(RM) count *.o *~