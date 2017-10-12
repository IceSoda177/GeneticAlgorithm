#ifndef __KANGAROO_H
#define __KANGAROO_H

typedef struct Kangaroo
{
	unsigned int uiGenetic;
	double	dFitness;
	struct Kangaroo* pNextKangaroo;
}Kangaroo;

typedef struct KangarooPack
{
	Kangaroo *pPack;

	int iPopulation;
	int iSectionMax;
	int iSectionMin;
	double dTotalFitness;
}KangarooPack;

int KangarooPackInit(KangarooPack *pKangarooPack,int iPop, int iSectionMax, int iSectionMin);
int GetKangarooGenLen();
double CalcKangarooFitFunc(Kangaroo *pKangaroo, int iGenLen,int iSectionMax, int iSectionMin);
void KillKangarooPack(KangarooPack *pKangarooPack);
Kangaroo* ChromoRoulette(KangarooPack *pKangarooPack);
void AddKangarooToPack(KangarooPack *pKangarooPack, Kangaroo *pKangaroo);

#endif

