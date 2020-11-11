#include <sys/regdef.h>
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

//MODULO MCD
.text
.align 2
.ent asmMcd
.globl asmMcd
asmMcd:
    .frame	fp, SS, ra
	.set	noreorder
	.cpload t9
	.set	reorder
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

    

    lw      t0, O_I1(fp)       // t0 <-- m
    lw      t3, O_I2(fp)       // T3 <-- n

    /* if (m == 0) */
    bgtz    t3, llamado_recursivo  //Veo si t3 es mayor a 0, si es asi, salto a llamado_recursivo

valor_retorno:
    /* Este valor sera el mcd de los parametros originales */
    /* return n */
    lw      v0, O_I1(fp)
    
    b       L_RET   

    /* return mcd(n % m, m) */

llamado_recursivo:

    lw      t1,O_I2(fp)     // t1 <-- n 

    /* n % m */      
    divu    t0, t1
    mfhi    t2

    /* Preparo argumentos para llamar a mcd nuevamente */

    add     a0,t1,zero      //El primer parametro con el que voy a llamar a la funcion
                            //sera n

    add     a1,t2,zero      //El segundo parametro con el que voy a llamar a la funcion
                            //sera n % m

    /* mcd(n % m,m) */
    jal     asmMcd             //Llamo recursivamente

    
    /* Stack unwinding */
L_RET:
    lw      fp, O_FP(sp)
    lw      gp, O_GP(sp)
    lw      ra, O_RA(sp)

    addiu   sp, sp, SS
    jr ra

.end asmMcd




//MODULO MCM
.text
.align 2
.ent asmMcm
.globl asmMcm
.extern asmMcd
asmMcm:

    .frame	fp, SS, ra
	.set	noreorder
	.cpload t9
	.set	reorder

	subu	sp, sp, SS

    /* SRA */
    .cprestore O_GP
    sw      ra, O_RA(sp)  
	sw		gp, O_GP(sp)
	sw		fp, O_FP(sp)

	move	fp, sp

	// Guardo argumentos m y n
	sw		a0, O_I1(fp)   //m
	sw		a1, O_I2(fp)   //n

    /* mcd(m,n) */
    jal     asmMcd              //El valor que me devuelve el modulo estara en v0


    lw      t0, O_I1(fp)       // t0 <-- m
    lw      t1, O_I2(fp)       //t1 <-- n

    /*  m*n  */
    mult    t0,t1
    mflo    t2
    
    /* m*n/mcd(m,n) */
    div     t2,v0
    mflo    v0                  //Guardo el valor a retornar por la funcion en v0

    /* Stack unwinding */
    lw      fp, O_FP(sp)
    lw      gp, O_GP(sp)
    lw      ra, O_RA(sp)

    addiu   sp, sp, SS
    jr ra

.end asmMcm
    


