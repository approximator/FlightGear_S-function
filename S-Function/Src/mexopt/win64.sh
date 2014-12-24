TMW_ROOT="$MATLAB"
MLIBS="-L$TMW_ROOT/bin/$Arch -lmx -lmex -lmat"
RPATH="-Wl,-rpath-link,$TMW_ROOT/bin/$Arch"


MINGW='-L/usr/x86_64-w64-mingw32/lib'
WINMAT='-L/home/approximator/mp/phd/dev/mtlb/cpp/sfunc/mlibs' # Windows matlab libraries
CUSTOMLIB='-lws2_32' # winsock lib
CC='x86_64-w64-mingw32-gcc' # compiler
CFLAGS='-D_GNU_SOURCE'
CFLAGS="$CFLAGS  -fexceptions"
CFLAGS="$CFLAGS -fno-omit-frame-pointer -pthread -I/usr/x86_64-w64-mingw32/include"
CLIBS="$RPATH $MLIBS $MINGW $WINMAT -lm $CUSTOMLIB"
COPTIMFLAGS='-O -DNDEBUG'
CDEBUGFLAGS='-g'
CLIBS="$CLIBS"

LD="$COMPILER"
LDEXTENSION='.mexw64' # Set extension for our mex file
LDFLAGS="-pthread -shared -Wl,--version-script,$TMW_ROOT/extern/lib/$Arch/$MAPFILE"
LDFLAGS="$LDFLAGS -Wl,--no-undefined -Wl,--verbose"
LDOPTIMFLAGS='-O'
LDDEBUGFLAGS='-g'