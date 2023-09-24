TARGET = add_yomi
OBJS = ucs2euc.o convert.o convert_ngram.o main.o yomi.o \
	yomi_kakasi.o yomi_mecab.o juman_client.o yomi_juman.o
CFLAGS = -O2 -Wall
CXXFLAGS = -O2 -Wall -std=c++17

all: $(TARGET)

# create_ucs2euc: create_ucs2euc.c
# 	$(CC) $(CFLAGS) $< -o $@
# 
# https://www.unicode.org/Public/MAPPINGS/OBSOLETE/EASTASIA/JIS/JIS0208.TXT
# ucs2euc.cpp: JIS0208.TXT create_ucs2euc
# 	./create_ucs2euc $< $@

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

juman_client.o: juman_client.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

yomi_juman.o: yomi_juman.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -lkakasi -lmecab -o $@

clean:
	$(RM) -rf $(TARGET) $(OBJS)
