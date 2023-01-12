# compiler executables
CXX = g++
WINDRES = windres

# default source / output directories
SRCDIR = ./src
OBJDIR = ./objectfiles
OUTDIR = ./compiled

# include directories
INCLUDE = -I./headers -I/mingw64/include
WXINCLUDE = `wx-config-3.1 --cxxflags --static`

# compiler flags
DEBUGFLAGS = -Og -g -g3 -fsanitize=address,undefined
CXXFLAGS = -Wall -Wextra -fdiagnostics-color=always
WINDRESFLAGS = --include-dir /mingw64/include --define __WIN32__ --define __GNUWIN32__

# linker flags
LDFLAGS = -L/mingw64/lib -static
WXLDFLAGS = `wx-config-3.1 --libs all --static` -llzma -ldeflate -lzstd -lLerc -lwebp -static

.PHONY: all clean

# builds the stuff
all: chess

# cleans ALL build files and compiled programs, use with caution
clean:
	@echo
	@echo "Removing executables..."
	@cd compiled
	@find . -type f -iname \*.exe -delete
	@cd ../
	@echo "Done removing executables."
	@echo
	@echo "Removing object files..."
	@cd objectfiles
	@find . -type f -iname \*.o -delete
	@echo "Done removing object files."
	@cd ../

CHESSSOURCES = $(wildcard $(SRCDIR)/*.cpp)
CHESSOBJECTS = $(addprefix $(OBJDIR)/, $(patsubst %.cpp, %.o, $(notdir $(CHESSSOURCES))))
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@if [ ! -d "$(OBJDIR)" ]; then mkdir -p $(OBJDIR); fi
	$(CXX) $(INCLUDE) $(WXINCLUDE) $(CXXFLAGS) $< -c -o $@

chess: $(CHESSOBJECTS)
	@if [ ! -d "$(OUTDIR)/images" ]; then cp -r $(SRCDIR)/images $(OUTDIR)/images; fi
	`wx-config-3.1 --rescomp --static` $(SRCDIR)/chess.rc -o $(OBJDIR)/icon.o
	$(CXX) $(CHESSOBJECTS) $(OBJDIR)/icon.o -o $(OUTDIR)/chess.exe $(WXLDFLAGS)