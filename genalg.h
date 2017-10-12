#ifndef __GENALG_H
#define __GENALG_H
#include "kangaroo.h"

double GenDecode(unsigned int uiGen, int iGenLen, int iSectionMax, int iSectionMin);
double GenMapping(unsigned int uiGen, int iGenLen, int iSectionMax, int iSectionMin);
//Kangaroo* ChromoRoulette(KangarooPack *pKangarooPack);
unsigned int GenMutation(unsigned int uiGenOri, double dMutateRate);
void GenCrossOver(unsigned int *puiGenOriA, unsigned int *puiGenOriB, double dCrossRate);
int GenFilter(KangarooPack *pKangarooPack);
double GetBestFitness(KangarooPack *pKangarooPack);
unsigned int GetBestGen(KangarooPack *pKangarooPack);
Kangaroo *FindKangarooWithBestGen(KangarooPack *pKangarooPack);

#endif
