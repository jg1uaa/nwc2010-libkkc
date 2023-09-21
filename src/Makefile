TARGET = calc
OBJS = convert.o convert_ngram.o main.o
CFLAGS = -O2 -Wall
CXXFLAGS = -O2 -Wall -std=c++17

all: $(TARGET)

convert.o: convert.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

convert_ngram.o: convert_ngram.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -lmecab -o $@

clean:
	$(RM) -rf $(TARGET) $(OBJS)
