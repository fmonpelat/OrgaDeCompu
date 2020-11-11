#include <sys/regdef.h>
#include "../includes/asmEuclidean.h"
#include </usr/include/mips-linux-gnu/sys/regdef.h>
#include </usr/include/mips-linux-gnu/sys/syscall.h>
#include </usr/include/mips-linux-gnu/sys/asm.h>
#define SS 40

//SRA
#define O_RA 32
#define O_GP 28
#define O_FP 24

//LTA
#define O_I2 44
#define O_I1 40


.text
.align 2
.ent asmMcd
.globl asmMcd
asmMcd:

// SRA
	subu	sp, sp, SS
    .cprestore O_GP
    sw      ra, O_RA(sp)  
	sw		gp, O_GP(sp)
	sw		fp, O_FP(sp)

	move	fp, sp

	// Guardo argumentos
	sw		a0, O_I1(fp)
	sw		a1, O_I2(fp)  

    /* if (m == 0) */

    lw      t0, O_I1(fp)       // t0 <-- m
    beqz    t0, valor_retorno  //Veo si t0 es igual a 0, si es asi, salto a valor_retorno

    /* return mcd(n % m, m) */

    lw      t1,O_I2(fp)     // t1 <-- n
    rem     t2,t1,t0        // t2 <-- n % m

    add     a0,t2,zero      //El primer parametro con el que voy a llamar a la funcion
                            //sera n % m

    add     a1,t1,zero      //El segundo parametro con el que voy a llamar a la funcion
                            //sera n

    jal     mcd             //Llamo recursivamente

    /* return n */
valor_retorno:
    /* Este valor sera el mcd de los parametros originales */
    lw      v0, O_I2(fp)
    lw      fp, O_FP(sp)
    lw      gp, O_GP(sp)
    lw      ra, O_RP(sp)

    addiu   sp, sp, SS
    jr ra

.end mcd

