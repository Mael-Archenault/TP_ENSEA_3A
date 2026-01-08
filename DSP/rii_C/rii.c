
#include <stdio.h>
#include <stdlib.h>

/* Prototype de la fonction rii.c */
/**********************************/
extern void rii(float b, float a, int N);

float inbuf[] = 
{
	#include "carre_100b.dat"
};

float outbuf_causal[sizeof(inbuf)]; /* sizeof: Calcule le nombre d'échantillons */
float outbuf_non_causal[sizeof(inbuf)];
float outbuf[sizeof(inbuf)];
unsigned SAMPLES = sizeof(inbuf); 

void main()
{
	float coeff_x = 0.125; /* coeff de xn */
	float coeff_s = 0.875; /* coeff de sn-1 */
	int i;
	
	rii(coeff_x, coeff_s, SAMPLES);

	exit(0);
}

/************************************************
Filtre recursif d'ordre 1 (sn = b*xn + a*sn-1)
Arguments : rii (b,a,in,N,out)
========	
	a,b : coeffs du filtre
	in : tableau des echantillons
	N : Nbre d'echantillons
	out : tableau des echantillons filtres
************************************************/

void rii(float b, float a, int N)
{
	int i;
 
	for (i=1; i<N; i++)
		inbuf[i] = b*inbuf[i] + a*inbuf[i-1];
	

	for (i=N-2; i>=0; i--) {
		inbuf[i] = b*inbuf[i] + a*inbuf[i+1];
	}		
}
/*
void rii_votre(float b, float a, int N)
{
	int i;
	float sn_1=0.;
	for (i=0; i<N; i++)
		outbuf_causal[i] = sn_1 = b*inbuf[i] + a*sn_1;
	
	sn_1 = 0;
	for (i=0; i<N; i++) {
		outbuf_non_causal[N-i-1] = sn_1 = b*inbuf[N-i-1] + a*sn_1;
	}
	
	for (i=0; i<N; i++){
	    outbuf[i] = (outbuf_causal[i] + outbuf_non_causal[i])/2;
	}

	
		
}
*/