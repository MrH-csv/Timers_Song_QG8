#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

/*
Mi4: 329.63 Hz   = 190 counts
Re♯4: 311.13 Hz	=	201 counts
Mi4: 329.63 Hz	=	190 counts
Re♯4: 311.13 Hz = 	201 counts
Mi4: 329.63 Hz	=	190 counts
Si3: 246.94 Hz = 253 counts
Re4: 293.66 Hz = 212 counts
Do♯4: 277.18 Hz = 225 counts
La3: 220.00 Hz =284 counts
 * */



#define  MODULO  0xFFFF
#define  PRESCALER 32
#define  BCLK  4000000
#define  T_OV   MODULO*PRESCALER*(1/BCLK)


#define La3		284
#define	Si3		253
#define Do4		239
#define Do_S4	226
#define Re4		213
#define Re_S4	201
#define Mi4		190
#define Fa4		179
#define Fa_S4	169
#define Sol4	159
#define Sol_S4	151
#define La4		142
#define La_S4	134
#define Si4		127
#define Do5		120
#define Do_S5	113
#define Re5		106
#define Re_S5	100
#define Mi5		95
#define Fa5		90
#define Fa_S5	85
#define Sol5	80
#define Sol_S5	75
#define La5		71
#define La_S5	67
#define Si5		63


typedef struct {
	int count_nota;
	int time_1t;
	int pause_1t;
}Nota;


void init_outputcompare(void);
void delay(void);
void delay_05s(void);
void delay_1s(void);
void output_compare (unsigned int tiempo);

const Nota Nokia [] = { {Mi5,200,5},{Re5,150,2},{Fa_S4,200,2},{Sol_S4,450,2},{Do_S5,200,2},{Si4,250,2},
		{Re4,200,2},{Mi4,400,2},{Si4,300,2},{La4,250,2},{Do_S4,250,2},{Mi4,250,2},{La4,2000,2}};

void main(void) {
	int auxiliar = 0;
	int count = 0;
	int i=0;
	SOPT1_COPE=0;	//WD disable.
	PTBDD_PTBDD7=1;  // LED de la tarjeta  
	PTADD_PTADD0=1;  // Definir usar CH0 como TPM Output Compare
	PTBDD_PTBDD5=0;  // Usar Ch1 como Input Capture
	init_outputcompare();

	count = sizeof(Nokia)/sizeof(Nokia[0]);
	
	for(;;) {
	
	for (i = 0; i < count; i++)
	{
		auxiliar = Nokia[0].time_1t;
		while (auxiliar !=0)
		{
			output_compare(Nokia[i].count_nota);
			PTBD_PTBD7=~PTBD_PTBD7;
			auxiliar--;
		}
	}
	delay_1s();
  }
}


void init_outputcompare(void)
{
	TPMMOD = MODULO; // Maxima cuenta del Contador General del TPM
	TPMC0V = 0x0000;  // Valor de Comparacion para hacer Toggle  	
	TPMC0SC = 0x14; //Modo Output Compare, Toggle
	TPMSC = 0x0D; // Sin IRQ, BusClk=4Mhz, Preescaler=32
}

void output_compare (unsigned int tiempo){
 while(!TPMC0SC_CH0F); // checar si el tiempo ya paso
//TPMSC0SC; // Lectura Tonta
TPMC0SC_CH0F=0; // Borra la bandera
TPMC0V = TPMC0V+tiempo; // Recarga el timer para el siguiente periodo
}

void delay(void) {
    SRTISC = 0x04;
    while(SRTISC_RTIF == 0);
    SRTISC_RTIACK = 1;
    SRTISC = 0x00;
}

void delay_1s(void) {
    SRTISC = 0x07;
    while(SRTISC_RTIF == 0);
    SRTISC_RTIACK = 1;
    SRTISC = 0x00;
}

void delay_05s(void) {
    SRTISC = 0x06;
    while(SRTISC_RTIF == 0);
    SRTISC_RTIACK = 1;
    SRTISC = 0x00;
}

void pause_1t(unsigned int silencio) {
    unsigned int i;
    for (i = 0; i < silencio; ++i) {
        delay();
    }
}
