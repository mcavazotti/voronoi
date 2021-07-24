CC=g++

CFLAGS= -g -std=c++17 -Wall

LFLAGS= -lm

SRCDIR = src
OBJDIR = bin
INCDIR = include
TEMDIR = template

SOURCES  	:= $(wildcard $(SRCDIR)/*.cpp)
INCLUDES 	:= $(wildcard $(INCDIR)/*.hpp)
TEMPLATES 	:= $(wildcard $(TEMDIR)/*.tpp)
OBJECTS  	:= $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)



voronoi: $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) $(LFLAGS) -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp $(INCLUDES) $(TEMPLATES)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm  bin/* || true

purge: clean
	rm ./voronoi || true