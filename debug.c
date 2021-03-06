#include <stdint.h>

#include "debug.h"
#include "enc.h"

void debug(void)
{
	DEBUG(ERDPTL);
	DEBUG(ERDPTH);
	DEBUG(EWRPTL);
	DEBUG(EWRPTH);
	DEBUG(ETXSTL);
	DEBUG(ETXSTH);
	DEBUG(ETXNDL);
	DEBUG(ETXNDH);
	DEBUG(ERXSTL);
	DEBUG(ERXSTH);
	DEBUG(ERXNDL);
	DEBUG(ERXNDH);
	DEBUG(ERXRDPTL);
	DEBUG(ERXRDPTH);
	DEBUG(ERXWRPTL);
	DEBUG(ERXWRPTH);
	DEBUG(EDMASTL);
	DEBUG(EDMASTH);
	DEBUG(EDMANDL);
	DEBUG(EDMANDH);
	DEBUG(EDMADSTL);
	DEBUG(EDMADSTH);
	DEBUG(EDMACSL);
	DEBUG(EDMACSH);
	DEBUG(RES00);
	DEBUG(RES01);
	DEBUG(RES02);
	DEBUG(EIE);
	DEBUG(EIR);
	DEBUG(ESTAT);
	DEBUG(ECON2);
	DEBUG(ECON1);

	DEBUG(EHT0);
	DEBUG(EHT1);
	DEBUG(EHT2);
	DEBUG(EHT3);
	DEBUG(EHT4);
	DEBUG(EHT5);
	DEBUG(EHT6);
	DEBUG(EHT7);
	DEBUG(EPMM0);
	DEBUG(EPMM1);
	DEBUG(EPMM2);
	DEBUG(EPMM3);
	DEBUG(EPMM4);
	DEBUG(EPMM5);
	DEBUG(EPMM6);
	DEBUG(EPMM7);
	DEBUG(EPMCSL);
	DEBUG(EPMCSH);
	DEBUG(RES10);
	DEBUG(RES11);
	DEBUG(EPMOL);
	DEBUG(EPMOH);
	DEBUG(EPWOLIE);
	DEBUG(EPWOLIR);
	DEBUG(ERXFCON);
	DEBUG(EPKTCNT);
	DEBUG(RES12);
	DEBUG(EIE_1);
	DEBUG(EIR_1);
	DEBUG(ESTAT_1);
	DEBUG(ECON2_1);
	DEBUG(ECON1_1);

	DEBUG(MACON1);
	DEBUG(MACON2);
	DEBUG(MACON3);
	DEBUG(MACON4);
	DEBUG(MABBIPG);
	DEBUG(RES20);
	DEBUG(MAIPGL);
	DEBUG(MAIPGH);
	DEBUG(MACLCON1);
	DEBUG(MACLCON2);
	DEBUG(MAMXFLL);
	DEBUG(MAMXFLH);
	DEBUG(RES21);
	DEBUG(MAPHSUP);
	DEBUG(RES22);
	DEBUG(RES23);
	DEBUG(RES24);
	DEBUG(MICON);
	DEBUG(MICMD);
	DEBUG(RES25);
	DEBUG(MIREGADR);
	DEBUG(RES26);
	DEBUG(MIWRL);
	DEBUG(MIWRH);
	DEBUG(MIRDL);
	DEBUG(MIRDH);
	DEBUG(RES27);
	DEBUG(EIE_2);
	DEBUG(EIR_2);
	DEBUG(ESTAT_2);
	DEBUG(ECON2_2);
	DEBUG(ECON1_2);

	DEBUG(MAADR1);
	DEBUG(MAADR0);
	DEBUG(MAADR3);
	DEBUG(MAADR2);
	DEBUG(MAADR5);
	DEBUG(MAADR4);
	DEBUG(EBSTSD);
	DEBUG(EBSTCON);
	DEBUG(EBSTCSL);
	DEBUG(EBSTCSH);
	DEBUG(MISTAT);
	DEBUG(RES30);
	DEBUG(RES31);
	DEBUG(RES32);
	DEBUG(RES33);
	DEBUG(RES34);
	DEBUG(RES35);
	DEBUG(RES36);
	DEBUG(EREVID);
	DEBUG(RES37);
	DEBUG(RES38);
	DEBUG(ECOCON);
	DEBUG(RES3_9);
	DEBUG(EFLOCON);
	DEBUG(EPAUSL);
	DEBUG(EPAUSH);
	DEBUG(RES3a);
	DEBUG(EIE_3);
	DEBUG(EIR_3);
	DEBUG(ESTAT_3);
	DEBUG(ECON2_3);
	DEBUG(ECON1_3);

	DEBUG(PHCON1);
	DEBUG(PHSTAT1);
	DEBUG(PHID1);
	DEBUG(PHID2);
	DEBUG(PHCON2);
	DEBUG(PHSTAT2);
	DEBUG(PHIE);
	DEBUG(PHIR);
	DEBUG(PHLCON);

	(void)0;
}
