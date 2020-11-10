#include <mips/regdef.h>
#define SS 40

//SRA
#define 0_RA 32
#define 0_GP 28
#define 0_FP 24

//LTA
#define O_I2 44
#define O_I1 40


.text
.ent mcd
.globl mcd
mcd:

// SRA
	subu	sp, sp, SS
    .cprestore 0_GP
    sw      ra, 0_RA(sp)  
	sw		gp, O_GP(sp)
	sw		fp, O_FP(sp)

	move	fp, sp

	// Guardo argumentos
	sw		a0, O_I1(fp)
	sw		a1, O_I2(fp)  

    lw      t0,0_I1(fp)       // t0 <-- m
    beqz    t0,valor_retorno  //Veo si t0 es igual a 0, si es asi, salto a valor_retorno

    lw      t1,0_I2(fp)     // t1 <-- n
    rem     t2,t1,t0        // t2 <-- n % m

    add     a0,t2,zero      //El primer parametro con el que voy a llamar a la funcion
                            //sera n % m

    add     a1,t1,zero      //El segundo parametro con el que voy a llamar a la funcion
                            //sera n

    jal     mcd             //Llamo recursivamente



valor_retorno:
    /* Este valor sera el mcd de los parametros originales */
    lw      v0,




















    
