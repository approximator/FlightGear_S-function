#CC=/path/to/matlab/bin/mex
SOURCES=FgsFunction.c transport/FgsTransport.c memory/OaMemory.c third_party/zlog/zlog.c
INCLUDES=-I"./include" -I"third_party/"

all: linux

linux:
	$(CC) $(SOURCES) $(INCLUDES) -v

win64:
	$(CC) $(SOURCES) $(INCLUDES) -v -f mexopt/win64.sh

clean:
	rm -vf FgsFunction.mexa64 FgsFunction.mexw64
