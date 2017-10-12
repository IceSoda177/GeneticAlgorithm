#include <math.h>
#include <stdlib.h>
#include "kangaroo.h"
#include "genalg.h"
#include <stdio.h>
#include <string.h>

static double s_dMutateRate = 0.03;
static double s_dCrossRate = 0.7;
static double s_dAsymCrossRate = 0.03;

double GenDecode(unsigned int uiGen, int iGenLen, int iSectionMax, int iSectionMin)
{
	int i = 0;
	double dDecodedValue = 0.0;
	unsigned int uiGenMax = 0;

	for (i = 0; i < iGenLen; i++)
	{
		uiGenMax = uiGenMax << 1;
		uiGenMax++;
	}
	
	dDecodedValue = iSectionMin + uiGen * ( (iSectionMax - iSectionMin) / (double)uiGenMax );
	return dDecodedValue;
}

double GenMapping(unsigned int uiGen, int iGenLen, int iSectionMax, int iSectionMin)
{
	double dDecodedValue = 0.0;
	dDecodedValue = GenDecode(uiGen, iGenLen, iSectionMax, iSectionMin);

	return dDecodedValue + 10 * sin(5 * dDecodedValue) + 7 * cos(4 * dDecodedValue);
}

unsigned int GenMutation(unsigned int uiGenOri, double dMutateRate)
{
	int iGenlen = 0;
	int i = 0;
	int iMutateLocation = 0;
	unsigned int uiMutateMask = 0;
	unsigned int uiMutatedGen = 0;

	uiMutatedGen = uiGenOri;
	//srand(time(NULL));
	
	if ( ((double)rand()/RAND_MAX) <= dMutateRate )
	{//mutate
		iGenlen = GetKangarooGenLen();
		iMutateLocation = rand()%iGenlen;
		printf("GenMutation: location %d\n", iMutateLocation);
		
		for (i = 0; i < iGenlen; i++)
		{
			uiMutateMask = uiMutateMask<<1;
			if (i == iMutateLocation)
			{
				uiMutateMask += 1;
			}
		}

		uiMutatedGen = (~uiGenOri & uiMutateMask) | (uiGenOri & ~uiMutateMask);
		printf("GenMutation: uiMutateMask %x, origen %x, mutatedgen %x\n", uiMutateMask, uiGenOri, uiMutatedGen);
	}

	return uiMutatedGen;
}

void GenCrossOver(unsigned int *puiGenOriA, unsigned int *puiGenOriB, double dCrossRate)
{
	int iCrossStartLocation = 0;
	int iCrossEndLocation = 0;
	int iTmp = 0;
	int i = 0;
	int iGenlen = 0;
	unsigned int uiCrossMask = 0;
	unsigned int uiGenOriATmp = *puiGenOriA;
	unsigned int uiGenOriBTmp = *puiGenOriB;

	iGenlen = GetKangarooGenLen();
	//srand(time(NULL));
	
	if ( ((double)rand()/RAND_MAX) <= dCrossRate )
	{
		iCrossStartLocation = rand()%iGenlen;
		iCrossEndLocation = iCrossStartLocation + rand()%(iGenlen - iCrossStartLocation);
		printf("GenCrossOver: from %d, to %d\n", iCrossStartLocation, iCrossEndLocation );

		for (i = 0; i < iGenlen; i++)
		{
			uiCrossMask = uiCrossMask << 1;
			if (iCrossStartLocation <= i && i <= iCrossEndLocation)
			{
				uiCrossMask += 1;
			}
		}
		printf("GenCrossOver: mask %x, origen %x, %x\n", uiCrossMask, *puiGenOriA, *puiGenOriB );
		*puiGenOriA = (uiGenOriBTmp & uiCrossMask) | (uiGenOriATmp & ~uiCrossMask);
		*puiGenOriB = (uiGenOriATmp & uiCrossMask) | (uiGenOriBTmp & ~uiCrossMask);
		printf("GenCrossOver: after %x, %x\n", *puiGenOriA, *puiGenOriB );
	}
	return;
}

void GenAsymCrossOver(unsigned int *puiGenOriA, unsigned int *puiGenOriB, double dAsymCrossRate)
{	
	int iCrossLen = 0;
	int iCrossStartPointA = 0;
	int iCrossStartPointB = 0;	
	int i = 0;
	int iGenLen = 0;
        unsigned int uiCrossMaskA = 0;
	unsigned int uiCrossMaskB = 0;
        unsigned int uiGenOriATmp = *puiGenOriA;
        unsigned int uiGenOriBTmp = *puiGenOriB;
	unsigned int uiGenSegA = 0;
	unsigned int uiGenSegB = 0;

        iGenLen = GetKangarooGenLen();
	if ( ((double)rand()/RAND_MAX) <= dAsymCrossRate )
        {
                iCrossLen = rand()%iGenLen;
                iCrossStartPointA = rand()%(iGenLen - iCrossLen);
		iCrossStartPointB = rand()%(iGenLen - iCrossLen);

                printf("GenAsymCrossOver: from %d, to %d, CrossLen %d\n", iCrossStartPointA, iCrossStartPointB, iCrossLen);

                for (i = 0; i < iGenLen; i++)
                {
                        uiCrossMaskA = uiCrossMaskA << 1;
			uiCrossMaskB = uiCrossMaskB << 1;
                        if (iCrossStartPointA <= i && i < iCrossStartPointA + iCrossLen)
                        {
                                uiCrossMaskA += 1;
                        }
			
			if (iCrossStartPointB <= i && i < iCrossStartPointB + iCrossLen)
			{
				uiCrossMaskB += 1;
			}
                }
		uiGenSegA = (uiGenOriATmp & uiCrossMaskA);
		uiGenSegB = (uiGenOriBTmp & uiCrossMaskB);
		
		if (iCrossStartPointA < iCrossStartPointB)
		{
			uiGenSegA >> (iCrossStartPointB - iCrossStartPointA);
			uiGenSegB << (iCrossStartPointB - iCrossStartPointA);
		}
		else
		{
			uiGenSegA << (iCrossStartPointA - iCrossStartPointB);
			uiGenSegB >> (iCrossStartPointA - iCrossStartPointB);
		}

		*puiGenOriA = (uiGenOriATmp & ~uiCrossMaskA) | uiGenSegB;
		*puiGenOriB = (uiGenOriBTmp & ~uiCrossMaskB) | uiGenSegA;
                
		printf("GenAsymCrossOver: after %x, %x\n", *puiGenOriA, *puiGenOriB );
        }
        return;
}

int GenFilter(KangarooPack *pKangarooPack)
{
	int i = 0;
	int iPopulation = 0;
	double dTotalFitness = 0;
	Kangaroo *pKangarooA = NULL;
	Kangaroo *pKangarooB = NULL;
	Kangaroo tKangarooTmpA = {0};
	Kangaroo tKangarooTmpB = {0};
	KangarooPack tKangarooPackNew = {0};
		
	if (!pKangarooPack)
	{
		printf("GenFilter: pKangarooPack is NULL\n");
		return -1;
	}

	memcpy(&tKangarooPackNew, pKangarooPack, sizeof(KangarooPack));
	tKangarooPackNew.pPack = NULL;
	tKangarooPackNew.iPopulation = 0;
	tKangarooPackNew.dTotalFitness = 0;

	iPopulation = pKangarooPack->iPopulation;
	dTotalFitness = pKangarooPack->dTotalFitness;
	
	for (i = 0; i < iPopulation/2; i++)
	{
		/* ������ǰ���Ż��� */
		if (0 == i)
		{
			pKangarooA = FindKangarooWithBestGen(pKangarooPack);
			if (!pKangarooA)
			{
				printf("GenFilter: No KangarooWithBestGen\n");
				exit(1);
			}
			memcpy(&tKangarooTmpA, pKangarooA, sizeof(Kangaroo));
			AddKangarooToPack(&tKangarooPackNew, &tKangarooTmpA);
		}
		
		/* ���̶�ѡ��˫�������� */
		pKangarooA = ChromoRoulette(pKangarooPack);
		pKangarooB = ChromoRoulette(pKangarooPack);

		if (!pKangarooA || !pKangarooB)
		{
			printf("GenFilter: ChromoRoulette get no Kangaroo\n");
			exit(1);
		}
		printf("GenFilter: selected gen is %x and %x\n", pKangarooA->uiGenetic, pKangarooB->uiGenetic);

		memcpy(&tKangarooTmpA, pKangarooA, sizeof(Kangaroo));
		memcpy(&tKangarooTmpB, pKangarooB, sizeof(Kangaroo));
		
		//ԭʼ���ݷ����ı�
		/* ˫�׻�����ʽ�������µĻ��� */
		GenCrossOver(&(tKangarooTmpA.uiGenetic), &(tKangarooTmpB.uiGenetic), s_dCrossRate);
		
		GenAsymCrossOver(&(tKangarooTmpA.uiGenetic), &(tKangarooTmpB.uiGenetic), s_dAsymCrossRate);

		/* �»��������ͻ�� */
		tKangarooTmpA.uiGenetic = GenMutation(tKangarooTmpA.uiGenetic, s_dMutateRate);
		tKangarooTmpB.uiGenetic = GenMutation(tKangarooTmpB.uiGenetic, s_dMutateRate);

		/* ������Ӧ�ȸ��� */
		CalcKangarooFitFunc(&tKangarooTmpA, GetKangarooGenLen(), pKangarooPack->iSectionMax, pKangarooPack->iSectionMin);
		CalcKangarooFitFunc(&tKangarooTmpB, GetKangarooGenLen(), pKangarooPack->iSectionMax, pKangarooPack->iSectionMin);

		AddKangarooToPack(&tKangarooPackNew, &tKangarooTmpA);
		AddKangarooToPack(&tKangarooPackNew, &tKangarooTmpB);
	}
	
	KillKangarooPack(pKangarooPack);
	memcpy(pKangarooPack, &tKangarooPackNew, sizeof(KangarooPack));

	return 0;
}

double GetBestFitness(KangarooPack *pKangarooPack)
{
	double dBestFitness = 0;
	Kangaroo *pKangarooTmp = NULL;

	if (!pKangarooPack)
	{
		printf("GetBestFitness: para invalid\n");
		return dBestFitness;
	}
	printf("GetBestFitness\n");

	pKangarooTmp = pKangarooPack->pPack;
	while (pKangarooTmp)
	{
		if (dBestFitness < pKangarooTmp->dFitness)
		{
			dBestFitness = pKangarooTmp->dFitness;
		}
		pKangarooTmp = pKangarooTmp->pNextKangaroo;
	}

	return dBestFitness;
}

unsigned int GetBestGen(KangarooPack *pKangarooPack)
{
	double dBestFitness = 0;
	Kangaroo *pKangarooTmp = NULL;
	unsigned int uiBestGen = 0;

	if (!pKangarooPack)
	{
		printf("GetBestGen: para invalid\n");
		return dBestFitness;
	}
	printf("GetBestGen\n");

	pKangarooTmp = pKangarooPack->pPack;
	while (pKangarooTmp)
	{
		if (dBestFitness < pKangarooTmp->dFitness)
		{
			dBestFitness = pKangarooTmp->dFitness;
			uiBestGen = pKangarooTmp->uiGenetic;
		}
		pKangarooTmp = pKangarooTmp->pNextKangaroo;
	}

	return uiBestGen;
}

Kangaroo *FindKangarooWithBestGen(KangarooPack *pKangarooPack)
{
	double dBestFitness = 0;
	Kangaroo *pKangarooTmp = NULL;
	Kangaroo *pKangarooBest = NULL;

	if (!pKangarooPack)
	{
		printf("GetBestGen: para invalid\n");
		return NULL;
	}
	printf("FindKangarooWithBestGen\n");

	pKangarooTmp = pKangarooPack->pPack;
	while (pKangarooTmp)
	{
		if (dBestFitness < pKangarooTmp->dFitness)
		{
			dBestFitness = pKangarooTmp->dFitness;
			pKangarooBest = pKangarooTmp;
		}
		pKangarooTmp = pKangarooTmp->pNextKangaroo;
	}

	return pKangarooBest;
}

