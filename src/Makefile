CXX = g++
CXXFLAGS = -Wall -ggdb -std=c++1z
all: build
build: main.o Search_Functions.o Sort_Functions.o Query.o Video_Functions.o File_Functions.o
	$(CXX) $(CXXFLAGS) -o TagTool main.o Search_Functions.o Sort_Functions.o Query.o Video_Functions.o File_Functions.o
main.o: main.cpp Search_Functions.hpp Sort_Functions.hpp
Search_Functions.o: Search_Functions.hpp Query.hpp Video_Functions.hpp File_Functions.hpp
Sort_Functions.o: Video_Functions.hpp File_Functions.hpp
Query.o: Query.hpp
Video_Functions.o: Video_Functions.hpp
File_Functions.o: File_Functions.hpp
run: build
	./TagTool $(ARGS)
clean:
	rm TagTool main.o Search_Functions.o Sort_Functions.o File_Functions.o Video_Functions.o Query.o