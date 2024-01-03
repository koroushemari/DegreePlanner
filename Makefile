# Some settings for the Makefile.  
# 
# In particular, you might need to adjust WT_BASE to wherever Wt was installed.  /usr/local is the default.

CXX=g++
WT_BASE=usr/local/include
CXXFLAGS=--std=c++14 -I$(WT_BASE)
LDFLAGS=-L$(WT_BASE)/lib -Wl,-rpath,$(WT_BASE)/lib -lwthttp -lwt -lsqlite3

# Identify our code base to be building.  Headers are listed under DEPS, and objects under OBJS.

DEPS = DegreePlanner.h Course.h Module.h Grades.h User.h
OBJS = main.o Degreeplanner.o Course.o Module.o Grades.o User.o

# How do we build objects?

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# How do we build the executable?

hey: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) 

# How do we clean up after ourselves?

clean:
	rm -f hey $(OBJS)


