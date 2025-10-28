# Names: Kaylee Muckerman and Julian To
# REDID: 130526910 and 130405272

# Makefile for pager assigment

CXX = g++
CXXFLAGS = -std=c++11 -Wall -g3 -c

OBJS = vaddr_tracereader.o log_helpers.o pagingwithpr.o pagetable.o pager.o pagereplacement.o

PROGRAM=pagingwithpr

$(PROGRAM) : $(OBJS) 
	$(CXX) -o $(PROGRAM) $^

vaddr_tracereader.o: vaddr_tracereader.h
	$(CXX) $(CXXFLAGS) vaddr_tracereader.cpp

log_helpers.o: log_helpers.h
	$(CXX) $(CXXFLAGS) log_helpers.cpp

pagereplacement.o: pagereplacement.h 
	$(CXX) $(CXXFLAGS) pagereplacement.cpp

pagetable.o: pagetable.h 
	$(CXX) $(CXXFLAGS) pagetable.cpp
	
pager.o: pager.h 
	$(CXX) $(CXXFLAGS) pager.cpp

pagingwithpr.o: pagetable.h  
	$(CXX) $(CXXFLAGS) pagingwithpr.cpp
			
clean :
	rm -f *.o $(PROGRAM)

