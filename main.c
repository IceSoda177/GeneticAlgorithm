#include "genalg.h"
#include "kangaroo.h"
#include <stdio.h>

//[0,9]区间上最大值
static int s_iSectionMax = 9;
static int s_iSectionMin = 0;

int main()
{
	int iResult = 0;
	int i = 0;
	double dBestFitness = 0;
	double dDecodedBestGen = 0;
	unsigned int uiBestGen = 0;
	KangarooPack tKangarooPack = {0};
	
	srand(time(NULL));
	
	KangarooPackInit(&tKangarooPack, 30, s_iSectionMax, s_iSectionMin);

	for (i = 0; i < 100; i++)
	{
		printf("round %d\n", i);
		GenFilter(&tKangarooPack);
	}

	dBestFitness = GetBestFitness(&tKangarooPack);
	uiBestGen = GetBestGen(&tKangarooPack);
	dDecodedBestGen = GenDecode(uiBestGen, GetKangarooGenLen(), s_iSectionMax, s_iSectionMin);

	printf ("GetBestFitness %f, GetBestGen %d, DecodedBestGen %f\n", dBestFitness, uiBestGen, dDecodedBestGen);
	return iResult;
}

