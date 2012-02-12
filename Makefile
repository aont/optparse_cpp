TARGET	= prog
CXXFLAGS	= -O0 -g

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	$(RM) $(TARGET)

${TARGET}: main.cpp optparse.hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) main.cpp