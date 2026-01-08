///////////////////////////////////////////////////////////////////////////////////////
//NAME:     main.c (Block-based Talkthrough)
//PURPOSE:  Talkthrough framework for sending and receiving samples to the AD1835.
//
//USAGE:    This file contains the main routine calls functions to set up the talkthrough
//          routine.
//
////////////////////////////////////////////////////////////////////////////////////////

#include <sru.h>
#include <cdef21262.h>
#include <signal.h>

extern void InitDAI(void);
extern void Init1835viaSPI(void);
extern void InitSPORT(void);

extern int rx_buf[2], tx_buf[2];

int input_values_count = 0;
int final_input_values_count = 0;

int start_processing = 0;

#define MAX_BUFFER_SIZE 4800

float* buffers_ptrs[3];
float buffers[3][MAX_BUFFER_SIZE];


float gamma = 0.5;

#define INPUT_BUFFER_INDEX 0
#define PROCESSING_BUFFER_INDEX 1
#define OUTPUT_BUFFER_INDEX 2

void Lisseur1D(float* tab, float gamma, int N);

// SPORT0 and SPORT1 Interrupt Service Routines      
//--------------------------------------------------------------------------------------------
/* ISR counters, for debug purposes to see how many times SPORT DMA interrupts are serviced */

void rotate_buffers(void){
    float* temp = buffers_ptrs[OUTPUT_BUFFER_INDEX];
    buffers_ptrs[OUTPUT_BUFFER_INDEX] = buffers_ptrs[PROCESSING_BUFFER_INDEX];
    buffers_ptrs[PROCESSING_BUFFER_INDEX] = buffers_ptrs[INPUT_BUFFER_INDEX];
    buffers_ptrs[INPUT_BUFFER_INDEX] = temp;
}

void AfficheLEDs(int led_value){
//lights as described at the top of the file
	if (led_value >255){
	    led_value = 255;
	}
    *pPPCTL=0;

    *pIIPP=(int) &led_value;
    *pIMPP=1;
    *pICPP=1;
    *pEMPP=1;
    *pECPP=1;
    *pEIPP=0x1400000;

    *pPPCTL=PPTRAN|PPBHC|PPDUR20|PPDEN|PPEN;
}

void ReceptionISR(int sig_int)
{
	buffers_ptrs[INPUT_BUFFER_INDEX][input_values_count] = (float)((rx_buf[0]<<8)>>8);
	tx_buf[0]= (int)buffers_ptrs[OUTPUT_BUFFER_INDEX][input_values_count];
	input_values_count++;
}

/*
void Lisseur1D(float* tab, float gamma, int N)
{
	int i;

	
	sn_1 = tab[0];
	for (i=1; i<N; i++){
		tab[i] = sn_1 = (1-gamma)*tab[i] + gamma*sn_1;
	}
	
	
	sn_1 = tab[N-1];
	for (i=1; i<N; i++) {
		tab[N-i-1] = sn_1 = (1-gamma)*tab[N-i-1] + gamma*sn_1;
	}
			
}
*/

void Button_Interrupt_Handler(int sig_num){
    gamma = gamma +0.125;
    if (gamma >1){
        gamma = 0;
    }
    
    return;
}

void IRQP1_isr ( int sig_num )
{
	static int IT_register;
    IT_register = *pDAI_IRPTL_H;
    
    

    //tester si c'est bien DAI_P1 qui est à l'origine de l'interruption DAI
    if ((IT_register & SRU_EXTMISCB1_INT) != 0)
    {
		
		rotate_buffers();
		final_input_values_count = input_values_count;
		input_values_count = 0;
		start_processing = 1;
		
		
    }
    
    return;
}


void main(void)
{
    // Enable interrupt nesting.
    asm( "#include <def21262.h>" );
    asm( "bit set mode1 IRPTEN;"  ); // Enable interrupts (globally)
    asm( "LIRPTL = SP0IMSK;"  ); 	// Unmask the SPORT0 ISR
    
    // Need to initialize DAI because the sport signals
    // need to be routed
    InitDAI();
    // This function will configure the codec on the kit
    Init1835viaSPI();

    // Finally setup the sport to receive / transmit the data
    InitSPORT();

    *pDAI_IRPTL_PRI = SRU_EXTMISCB1_INT; // Démasquer individuellement l'interruption
	*pDAI_IRPTL_RE = SRU_EXTMISCB1_INT; // Choisir le front montant pour l'interruption
	// Unité de routage : Router (DAI_P1 ----> MISCB1)
	SRU(LOW,DAI_PB01_I);
	SRU(DAI_PB01_O,MISCB1_I);
	SRU(LOW,PBEN01_I);
	

	interrupt(SIG_DAIH,IRQP1_isr);
	
	AfficheLEDs(255);
	
	buffers_ptrs[INPUT_BUFFER_INDEX] = buffers[INPUT_BUFFER_INDEX];
	buffers_ptrs[PROCESSING_BUFFER_INDEX] = buffers[PROCESSING_BUFFER_INDEX];
	buffers_ptrs[OUTPUT_BUFFER_INDEX] = buffers[OUTPUT_BUFFER_INDEX];
	
	
	*pSYSCTL |= IRQ1EN;
	asm("bit set mode2 IRQ1E;");
	interrupt(SIG_IRQ1, Button_Interrupt_Handler);
	
	float chunk_time;
	int frequency;
	
     // Be in infinite loop and do nothing until done.
    while(1){
        if(start_processing == 1){
            Lisseur1D(buffers_ptrs[PROCESSING_BUFFER_INDEX], gamma, final_input_values_count);
            
            // frequency computation
            //chunk_time = ((float)final_input_values_count)/48000;
	        //frequency = (int) (1/chunk_time);
	        //AfficheLEDs(frequency);
	        start_processing = 0;
	     }
      
    }   
}


