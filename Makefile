TARGET	= prog
CXXFLAGS	= -O0 -g

.PHONY: all
all: ${TARGET}

.PHONY: clean
clean:
	${RM} ${TARGET}

${TARGET}: main.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) main.cpp