# Project
PROJECT = GeneticAlgoithm
PROJECT_BRIEF = Implementation of the genetic algorithm

#### Specify here external library dependencies as compiler flags

EXT_INCLUDE = 
EXT_LDFLAGS = 
EXT_CXXFLAGS =

# ROOT library
EXT_LDFLAGS += $(shell root-config --ldflags --libs)
EXT_CXXFLAGS += $(shell root-config --cflags)

####

# code structure
OBJDIR = .objs
DEPDIR = .deps
SRCDIR = src
LIBDIR = lib
BINDIR = bin
INCLUDEDIR = include
UTILSDIR = utils
DOCDIR = doc

# general flags
CXX           = g++ 
CXXFLAGS      = -O2 -Wall -fPIC -g -ansi -std=c++0x 
LDFLAGS       = -O -L. 
INCLUDE       = -I. -I$(INCLUDEDIR)

INCLUDE += $(EXT_INCLUDE)
LDFLAGS += $(EXT_LDFLAGS)
CXXFLAGS += $(EXT_CXXFLAGS)

#### Specify here type of outputs: share, static or exec
all : exec
	@echo "All OK"
####

# source and object libraries
SRCS =  $(wildcard $(SRCDIR)/*.cxx)
OBJS = $(SRCS:$(SRCDIR)/%.cxx=$(OBJDIR)/%.o)
UTILS = $(wildcard $(UTILSDIR)/*.cxx)
EXECOBJS = $(UTILS:$(UTILSDIR)/%.cxx=$(OBJDIR)/utils/%.o)
EXECS = $(UTILS:$(UTILSDIR)/%.cxx=$(BINDIR)/%.exe)

# dependency files
DEPS = $(OBJS:$(OBJDIR)/%.o=$(DEPDIR)/%.d)
EXECDEPS = $(EXECOBJS:$(OBJDIR)/utils/%.o=$(DEPDIR)/utils/%.d) 

shared : $(LIBDIR)/lib$(PROJECT).so
	@echo "Shared lib: OK"

static : $(LIBDIR)/lib$(PROJECT).a
	@echo "Static lib: OK"

exec : $(EXECS)
	@echo "Executables: OK"

doc : doxygen
	@echo "Doc OK"

# pull in dependency info for .o files
-include $(DEPS) $(EXECDEPS)

# rules to compile sources and generate dependencies
$(OBJDIR)/%.o : $(SRCDIR)/%.cxx
	@mkdir -p `dirname $@`
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@
	@mkdir -p `dirname $(@:$(OBJDIR)/%.o=$(DEPDIR)/%.d)`
	@echo `dirname $@`/`$(CXX) -MM $(CXXFLAGS) $(INCLUDE) $<` | sed 's, \\, ,g' > $(@:$(OBJDIR)/%.o=$(DEPDIR)/%.d)
	@cp -f $(@:$(OBJDIR)/%.o=$(DEPDIR)/%.d) $(@:$(OBJDIR)/%.o=$(DEPDIR)/%.d.tmp)
	@sed -e 's/.*://' -e 's/\$$//' < $(@:$(OBJDIR)/%.o=$(DEPDIR)/%.d.tmp) | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> $(@:$(OBJDIR)/%.o=$(DEPDIR)/%.d)
	@rm -f $(@:$(OBJDIR)/%.o=$(DEPDIR)/%.d.tmp)

$(OBJDIR)/utils/%.o : $(UTILSDIR)/%.cxx
	@mkdir -p `dirname $@`
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@
	@mkdir -p `dirname $(@:$(OBJDIR)/utils/%.o=$(DEPDIR)/utils/%.d)`
	@echo `dirname $@`/`$(CXX) -MM $(CXXFLAGS) $(INCLUDE) $<` | sed 's, \\, ,g' > $(@:$(OBJDIR)/utils/%.o=$(DEPDIR)/utils/%.d)
	@cp -f $(@:$(OBJDIR)/utils/%.o=$(DEPDIR)/utils/%.d) $(@:$(OBJDIR)/utils/%.o=$(DEPDIR)/utils/%.d.tmp)
	@sed -e 's/.*://' -e 's/\$$//' < $(@:$(OBJDIR)/utils/%.o=$(DEPDIR)/utils/%.d.tmp) | fmt -1 | \
	sed -e 's/^ *//' -e 's/$$/:/' >> $(@:$(OBJDIR)/utils/%.o=$(DEPDIR)/utils/%.d)
	@rm -f $(@:$(OBJDIR)/utils/%.o=$(DEPDIR)/utils/%.d.tmp)

$(DEPDIR)/%.d:
	@rm -f $(@:$(DEPDIR)/%.d=$(OBJDIR)/%.o)

# rule for shared library
$(LIBDIR)/lib$(PROJECT).so : $(DEPS) $(OBJS)
ifneq ($(OBJS),)
	@mkdir -p $(LIBDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -shared $(OBJS) $(LDFLAGS) -o $@
endif

# rule for static library
$(LIBDIR)/lib$(PROJECT).a : $(DEPS) $(OBJS)
ifneq ($(OBJS),)
	@mkdir -p $(LIBDIR)
	ar crs $@ $(OBJS)
	@chmod a+x $@
endif

# executable
$(BINDIR)/%.exe : $(OBJDIR)/utils/%.o $(DEPS) $(EXECDEPS) $(OBJS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< $(OBJS) $(LDFLAGS) -o $@

DOXYDIR = $(DOCDIR)/doxygen
DOXYCFG = $(DOXYDIR)/doxygen.cfg

DOXYINPUT =
DOXYINPUT += $(PWD)/README.md
DOXYINPUT += $(PWD)/$(DOCDIR)
DOXYINPUT += $(PWD)/macros
DOXYINPUT += $(PWD)/$(UTILSDIR)
DOXYINPUT += $(PWD)/$(SRCDIR)
DOXYINPUT += $(PWD)/$(INCLUDEDIR)


# documentation
doxygen : 
	@mkdir -p $(DOXYDIR)
	@doxygen -g $(DOXYCFG)
	@sed -i "/^PROJECT_BRIEF /c\PROJECT_BRIEF = \"$(PROJECT_BRIEF)\"" $(DOXYCFG)
	@sed -i "/^PROJECT_NAME /c\PROJECT_NAME = $(PROJECT)" $(DOXYCFG)
	@sed -i "/^EXTRACT_PRIVATE /c\EXTRACT_PRIVATE = YES" $(DOXYCFG)
	@sed -i "/^EXTRACT_STATIC /c\EXTRACT_STATIC = YES" $(DOXYCFG)
	@sed -i "/^EXTRACT_LOCAL_CLASSES /c\EXTRACT_LOCAL_CLASSES = NO" $(DOXYCFG)
	@sed -i "/^HIDE_UNDOC_MEMBERS /c\HIDE_UNDOC_MEMBERS = YES" $(DOXYCFG)
	@sed -i "/^HIDE_UNDOC_CLASSES /c\HIDE_UNDOC_CLASSES = YES" $(DOXYCFG)
	@sed -i "/^SOURCE_BROWSER /c\SOURCE_BROWSER = YES" $(DOXYCFG)
	@sed -i "/^INPUT /c\INPUT = $(DOXYINPUT)" $(DOXYCFG)
	@sed -i "/^FILE_PATTERNS /c\FILE_PATTERNS = *.md *.h *.cxx *.C" $(DOXYCFG)
	@sed -i "/^RECURSIVE /c\RECURSIVE = YES" $(DOXYCFG)
	@sed -i "/^OUTPUT_DIRECTORY /c\OUTPUT_DIRECTORY = $(DOXYDIR)" $(DOXYCFG)
	@sed -i "/^SORT_MEMBER_DOCS /c\SORT_MEMBER_DOCS = NO" $(DOXYCFG)
	@sed -i "/^STRIP_CODE_COMMENTS /c\STRIP_CODE_COMMENTS = NO" $(DOXYCFG)
	@sed -i "/^USE_MATHJAX /c\USE_MATHJAX = YES" $(DOXYCFG)
	doxygen $(DOXYCFG)
	@cp -r $(DOCDIR)/figures $(DOXYDIR)/html/figures

.SECONDARY : $(OBJS) $(EXECOBJS)

# cleaning
clean : clean~ cleandoc
	rm -rf $(OBJDIR)
	rm -rf $(DEPDIR)
	rm -rf $(BINDIR)
	rm -rf $(LIBDIR)

clean~ :
	find . -name "*~" -exec rm -rf {} \;

cleandoc :
	rm -rf $(DOXYDIR)

