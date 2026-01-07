
void Lisseur1D(float* tab, float gamma, int N)
{
	int i;

	
	float sn_1 = tab[0];
	for (i=1; i<N; i++){
		tab[i] = sn_1 = (1-gamma)*tab[i] + gamma*sn_1;
	}
	
	
/*	sn_1 = tab[N-1];
	for (i=1; i<N; i++) {
		tab[N-i-1] = sn_1 = (1-gamma)*tab[N-i-1] + gamma*sn_1;
	}
*/		
}
