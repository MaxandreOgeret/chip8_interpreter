# CHIP8 Interpreter

CHIP8 Interpreter written in C++.  
Issues and suggestions are welcome !

```
USAGE: 

   ./CHIP8  [-f <value>] [-1] [-2] [-3] [-4] [--] [--version] [-h] <Path>


Where: 

   -f <value>,  --Frequency <value>
     CPU Frequency (Default: 500Hz)

   -1,  --1
     Shift instructions 8xy6 and 8xyE will also set xv to the value of xy.

   -2,  --2
     Jump with offset Bnnn becomes Bxnn and add x offset to the jump.

   -3,  --3
     Store/Load Fx55/Fx65 will increment i.

   -4,  --4
     OR/AND/XOR 8xy1/8xy2/8xy3 will reset vf to 0.

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.

   <Path>
     (required)  Path to CHIP8 rom.
```