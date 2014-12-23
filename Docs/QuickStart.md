# How to start using FGS

## Compile S-Function

* Clone repo from github 

  `git clone https://github.com/approximator/FlightGear_S-function.git fgs`
  
  `cd fgs`

* Launch your Matlab.
* Change Matlab's current directory to **Src**
* Run `mex` from Matlab's command line.

  `mex -I"./include" -I"third_party/" FgsFunction.c transport/FgsTransport.c memory/OaMemory.c third_party/zlog/zlog.c`

  Now we have _FgsFunction.mexa64_ or _FgsFunction.mexw64_ (name can differ) file.
* Change matlab's current directory to **SimulinkModel**.
* Copy _FgsFunction.mexa64_ to current directory.
* Open model.

## Copy protocol files

Copy protocol files from **Protocol** directory to the FlightGear's protocol directory.
    
For example on Linux system this would be:

    cp Protocol/FgsInputProtocol.xml /usr/share/games/flightgear/Protocol/
    cp Protocol/FgsOutputProtocol.xml /usr/share/games/flightgear/Protocol/

## Launch FlightGear

    fgfs --generic=socket,out,40,localhost,5000,udp,FgsOutputProtocol --generic=socket,in,45,localhost,5010,udp,FgsInputProtocol

## Fly!

Take off as usual, than run Simulink model (_FG_Sfunc.slx_) and enjoy how aircraft is stabilizing.