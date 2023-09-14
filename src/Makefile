TARGET = calc
OBJS = ucs2euc.o convert.o convert_ngram.o main.o
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

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -lkakasi -o $@

clean:
	$(RM) -rf $(TARGET) $(OBJS)
