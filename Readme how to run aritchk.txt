How to compile and run aritchk on Linux
---------------------------------------------

1. This program prints on the terminal the values of some #defines in order to compile CLapack for PC and non-PC architectures.
2. If Linux: Open the terminal on the path where aritchk.c was saved. In this case is in ./F2CLIBS/libf2c.
	a. Compile using gcc: gcc arithchk.c -lm -DNO_FPINIT
	b. lm is a mathematical library for Unix. It is neccesary to define or pass NO_FPINIT and in PC architectures the FPU doesn't exist.
	c. Run executable ./a.out
3. If Arduino or similar ARM platforms: Open Arduino project saved on arithchk folder.
	a. Compile and load the executable on the board.
	b. Open serial monitor and copy the values.
    c. Create the file "arith.h" in ./F2CLIBS/libf2c/
	d. Paste the values in this file.
	e. Compile this project using CMake and the arm toolchain from its website called arm-none-eabi


Example of the output on Linux PC distribution:
#define IEEE_8087
#define Arith_Kind_ASL 1
#define Long int
#define Intcast (int)(long)
#define Double_Align
#define X64_bit_pointers
#define QNaN0 0x0
#define QNaN1 0xfff80000

Example of the output on an ARM M7 platform:
#define IEEE_8087
#define Arith_Kind_ASL 1
#define Double_Align
#define QNaN0 0x0
#define QNaN1 0x7FF80000