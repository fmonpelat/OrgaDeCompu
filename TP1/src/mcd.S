#include <mips/regdef.h>
#define SS 40

//SRA
#define 0_FP 36
#define 0_GP 32
//LTA
#define O_I2 4
#define O_I1 0


.text
.ent mcd
.globl mcd
mcd:

// Stack Wind
	subu	sp, sp, SS
	sw		fp, O_FP(sp)
	sw		gp, O_GP(sp)
	move	fp, sp

	// Guardo argumentos
	sw		a0, O_I2(fp)
	sw		a1, O_I1(fp)

    
