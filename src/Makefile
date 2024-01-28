TARGET = add_yomi
OBJS = ucs2euc.o convert.o convert_ngram.o main.o yomi.o \
	yomi_kakasi.o yomi_mecab.o yomi_chasen.o
CFLAGS = -O2 -Wall -I/usr/local/include -L/usr/local/lib
CXXFLAGS = -O2 -Wall -std=c++17

all: $(TARGET)

create_ucs2euc: create_ucs2euc.c
	$(CC) $(CFLAGS) $< -o $@

ucs2euc.cpp: create_ucs2euc
	./create_ucs2euc $@

ucs2euc.o: ucs2euc.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

convert.o: convert.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

convert_ngram.o: convert_ngram.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

yomi.o: yomi.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

yomi_kakasi.o: yomi_kakasi.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

yomi_mecab.o: yomi_mecab.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

yomi_chasen.o: yomi_chasen.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -lkakasi -lmecab -lchasen -o $@

clean:
	$(RM) -rf $(TARGET) $(OBJS) create_ucs2euc
