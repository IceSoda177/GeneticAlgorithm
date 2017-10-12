/*
this file discrib an individual
*/
#include "kangaroo.h"
#include "genalg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KANGAROOMAXGENLEN 17

int KangarooPackInit(KangarooPack *pKangarooPack, int iPop, int iSectionMax, int iSectionMin)
{//根据种群个数随机初始化所有个体
	int i = 0;
	int j = 0;
	unsigned int uiGenTmp = 0;
	Kangaroo tKangarooTmp = {0};

	if (!pKangarooPack)
	{
		printf("KangarooPackInit: apply space for KangarooPack fail!\n");
		return;
	}
	memset(pKangarooPack, 0, sizeof(KangarooPack));
	
	pKangarooPack->iSectionMax = iSectionMax;
	pKangarooPack->iSectionMin = iSectionMin;
	pKangarooPack->dTotalFitness = 0;

	for (i = 0; i < iPop; i++)
	{
		memset(&tKangarooTmp, 0, sizeof(Kangaroo));
		uiGenTmp = 0;
		for (j = 0; j < KANGAROOMAXGENLEN; j++)
		{
			uiGenTmp = uiGenTmp << 1;
			if (rand() % 2 == 1)
			{
				uiGenTmp += 1;
			}
		}
		tKangarooTmp.uiGenetic = uiGenTmp;
		CalcKangarooFitFunc(&tKangarooTmp, KANGAROOMAXGENLEN, iSectionMax, iSectionMin);

		AddKangarooToPack(pKangarooPack, &tKangarooTmp);
		printf("KangarooPackInit:Kangaroo[%d] 's gen is %x!\n", i, pKangarooPack->pPack->uiGenetic);
		printf("KangarooPackInit:Kangaroo[%d] 's fitness is %f!\n", i, pKangarooPack->pPack->dFitness);
	}

	printf("KangarooInit: Pack init finish\n");
	return 0;
}

int GetKangarooGenLen()
{
	return KANGAROOMAXGENLEN;
}

double CalcKangarooFitFunc(Kangaroo *pKangaroo, int iGenLen,int iSectionMax, int iSectionMin)
{
	pKangaroo->dFitness = GenMapping(pKangaroo->uiGenetic, iGenLen, iSectionMax, iSectionMin);
	return pKangaroo->dFitness;
}

void KillKangarooPack(KangarooPack *pKangarooPack)
{
	Kangaroo *pKangarooTmp = NULL;

	while (pKangarooPack->pPack)
	{
		pKangarooTmp = pKangarooPack->pPack->pNextKangaroo;
		free(pKangarooPack->pPack);
		pKangarooPack->pPack = pKangarooTmp;
	}
	
	pKangarooPack->iPopulation = 0;
	pKangarooPack->dTotalFitness = 0;
	
	return;
}

void AddKangarooToPack(KangarooPack *pKangarooPack, Kangaroo *pKangaroo)
{
	Kangaroo *pKangarooTmp = NULL;
	if (!pKangarooPack || !pKangaroo)
	{
		printf("AddKangarooToPack: para invalid\n");
		return ;
	}

	pKangarooTmp = (Kangaroo *)malloc(sizeof(Kangaroo));
	if (!pKangarooTmp)
	{
		printf("AddKangarooToPack: memory apply failed!\n");
		exit(1);
	}

	memcpy(pKangarooTmp, pKangaroo, sizeof(Kangaroo));
	pKangarooTmp->pNextKangaroo = pKangarooPack->pPack;
	pKangarooPack->pPack = pKangarooTmp;

	pKangarooPack->iPopulation++;
	pKangarooPack->dTotalFitness += pKangaroo->dFitness;
	return;
}

/*轮盘赌返回个体地址*/
Kangaroo* ChromoRoulette(KangarooPack *pKangarooPack)
{
	int	iKangarooNum = 0;
	double dRoulette = 0;
	double dGrandFitness = 0;
	double dTotalFitness = 0;
	Kangaroo *pKangarooTmp = NULL;

	if (!pKangarooPack)
	{
		printf("ChromoRoulette: pKangarooPack invalid\n");
		return NULL;
	}

	//iKangarooNum = pKangarooPack->iPopulation;
	dTotalFitness = pKangarooPack->dTotalFitness;
	
	dRoulette = ((double)rand()/RAND_MAX) * dTotalFitness;
	
	pKangarooTmp = pKangarooPack->pPack;
	dGrandFitness = 0;
	while (pKangarooTmp)
	{
		dGrandFitness += pKangarooTmp->dFitness;

		if (dRoulette <= dGrandFitness)
		{
			break;
		}
		iKangarooNum++;
		pKangarooTmp = pKangarooTmp->pNextKangaroo;
	}
	if (!pKangarooTmp)
	{
		printf("ChromoRoulette: no result, dGrandFitness %f, iKangarooNum %d!\n", dGrandFitness, iKangarooNum);
		exit(1);
	}
	return pKangarooTmp;
}
