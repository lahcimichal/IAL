	
/* c206.c **********************************************************}
{* T�ma: Dvousm�rn� v�zan� line�rn� seznam
**
**                   N�vrh a referen�n� implementace: Bohuslav K�ena, ��jen 2001
**                            P�epracovan� do jazyka C: Martin Tu�ek, ��jen 2004
**                                            �pravy: Bohuslav K�ena, ��jen 2015
**
** Implementujte abstraktn� datov� typ dvousm�rn� v�zan� line�rn� seznam.
** U�ite�n�m obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datov� abstrakce reprezentov�n prom�nnou
** typu tDLList (DL znamen� Double-Linked a slou�� pro odli�en�
** jmen konstant, typ� a funkc� od jmen u jednosm�rn� v�zan�ho line�rn�ho
** seznamu). Definici konstant a typ� naleznete v hlavi�kov�m souboru c206.h.
**
** Va��m �kolem je implementovat n�sleduj�c� operace, kter� spolu
** s v��e uvedenou datovou ��st� abstrakce tvo�� abstraktn� datov� typ
** obousm�rn� v�zan� line�rn� seznam:
**
**      DLInitList ...... inicializace seznamu p�ed prvn�m pou�it�m,
**      DLDisposeList ... zru�en� v�ech prvk� seznamu,
**      DLInsertFirst ... vlo�en� prvku na za��tek seznamu,
**      DLInsertLast .... vlo�en� prvku na konec seznamu, 
**      DLFirst ......... nastaven� aktivity na prvn� prvek,
**      DLLast .......... nastaven� aktivity na posledn� prvek, 
**      DLCopyFirst ..... vrac� hodnotu prvn�ho prvku,
**      DLCopyLast ...... vrac� hodnotu posledn�ho prvku, 
**      DLDeleteFirst ... zru�� prvn� prvek seznamu,
**      DLDeleteLast .... zru�� posledn� prvek seznamu, 
**      DLPostDelete .... ru�� prvek za aktivn�m prvkem,
**      DLPreDelete ..... ru�� prvek p�ed aktivn�m prvkem, 
**      DLPostInsert .... vlo�� nov� prvek za aktivn� prvek seznamu,
**      DLPreInsert ..... vlo�� nov� prvek p�ed aktivn� prvek seznamu,
**      DLCopy .......... vrac� hodnotu aktivn�ho prvku,
**      DLActualize ..... p�ep�e obsah aktivn�ho prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na dal�� prvek seznamu,
**      DLPred .......... posune aktivitu na p�edchoz� prvek seznamu, 
**      DLActive ........ zji��uje aktivitu seznamu.
**
** P�i implementaci jednotliv�ch funkc� nevolejte ��dnou z funkc�
** implementovan�ch v r�mci tohoto p��kladu, nen�-li u funkce
** explicitn� uvedeno n�co jin�ho.
**
** Nemus�te o�et�ovat situaci, kdy m�sto leg�ln�ho ukazatele na seznam 
** p�ed� n�kdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodn� komentujte!
**
** Terminologick� pozn�mka: Jazyk C nepou��v� pojem procedura.
** Proto zde pou��v�me pojem funkce i pro operace, kter� by byly
** v algoritmick�m jazyce Pascalovsk�ho typu implemenov�ny jako
** procedury (v jazyce C procedur�m odpov�daj� funkce vracej�c� typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozorn�n� na to, �e do�lo k chyb�.
** Tato funkce bude vol�na z n�kter�ch d�le implementovan�ch operac�.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* glob�ln� prom�nn� -- p��znak o�et�en� chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L p�ed jeho prvn�m pou�it�m (tzn. ��dn�
** z n�sleduj�c�ch funkc� nebude vol�na nad neinicializovan�m seznamem).
** Tato inicializace se nikdy nebude prov�d�t nad ji� inicializovan�m
** seznamem, a proto tuto mo�nost neo�et�ujte. V�dy p�edpokl�dejte,
** �e neinicializovan� prom�nn� maj� nedefinovanou hodnotu.
**/
  
	/*
	 * Pro inicializaci seznamu musi byt vsechny ukazatele 
	 * nastaveny na hodnotu NULL
	 */
	L -> First = NULL ;
	L -> Act = NULL ;
	L -> Last = NULL ;
}

void DLDisposeList (tDLList *L) {
/*
** Zru�� v�echny prvky seznamu L a uvede seznam do stavu, v jak�m
** se nach�zel po inicializaci. Ru�en� prvky seznamu budou korektn�
** uvoln�ny vol�n�m operace free. 
**/

	while ( L -> First != NULL )
	{
		tDLElemPtr pom ;			/* Vytvoreni pomocneho ukazatele */
		pom = L -> First ;
		L -> First = pom -> rptr ;
		free ( pom ) ;				/* Uvolneni pomocneho ukazatele */
	}

	/* V prazdnem seznamu musi byt vsechny ukazatele nastaveny na hodnotu NULL */
	L -> Act = NULL ;
	L -> Last = NULL ;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vlo�� nov� prvek na za��tek seznamu L.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/

	/* Alokovani pameti pro novy prvek a osetreni funkce malloc */
	struct tDLElem *new_item ;
	if ( ( new_item = malloc ( sizeof ( struct tDLElem ) ) ) == NULL )
	{
		DLError () ;
		return ;
	}

	/* Naplneni noveho prvku seznamu */
	new_item -> data = val ;
	new_item -> rptr = NULL ;
	new_item -> lptr = NULL ;

	/* Provazani s prvnim prvkem seznamu */
	new_item -> rptr = L -> First ;

	/* Pokud je vpravo nejaky dalsi prvek */
	if ( new_item -> rptr != NULL )
	{
		new_item -> rptr -> lptr = new_item ;
	}
	else
	{
		L -> Last = new_item ;
	}

	/* Nastaveni noveho prvku na prvni prvek seznamu */
	L -> First = new_item ;
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vlo�� nov� prvek na konec seznamu L (symetrick� operace k DLInsertFirst).
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/ 	

	/* Alokovani pameti pro novy prvek a osetreni funkce malloc */
	struct tDLElem *new_item ;
	if ( ( new_item = malloc ( sizeof ( struct tDLElem ) ) ) == NULL )
	{
		DLError () ;
		return ;
	}

	/* Naplneni noveho prvku seznamu */
	new_item -> data = val ;
	new_item -> rptr = NULL ;
	new_item -> lptr = NULL ;

	/* Provazani s poslednim prvkem seznamu */
	new_item -> lptr = L -> Last ;

	/* Pokud je vlevo nejaky dalsi prvek */
	if ( new_item -> lptr != NULL )
	{
		new_item -> lptr -> rptr = new_item ;
	}
	else
	{
		L -> First = new_item ;
	}

	/* Nastaveni noveho prvku na posledni prvek seznamu */
	L -> Last = new_item ;
}

void DLFirst (tDLList *L) {
/*
** Nastav� aktivitu na prvn� prvek seznamu L.
** Funkci implementujte jako jedin� p��kaz (nepo��t�me-li return),
** ani� byste testovali, zda je seznam L pr�zdn�.
**/

	L -> Act = L -> First ;
}

void DLLast (tDLList *L) {
/*
** Nastav� aktivitu na posledn� prvek seznamu L.
** Funkci implementujte jako jedin� p��kaz (nepo��t�me-li return),
** ani� byste testovali, zda je seznam L pr�zdn�.
**/

	L -> Act = L -> Last ;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu prvn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
**/

	if ( L -> First == NULL )
	{
		DLError () ;
	}
	else
	{
		*val = L -> First -> data ;
	}
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu posledn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
**/

	if ( L -> First == NULL )
	{
		DLError () ;
	}
	else
	{
		*val = L -> Last -> data ;
	}
}

void DLDeleteFirst (tDLList *L) {
/*
** Zru�� prvn� prvek seznamu L. Pokud byl prvn� prvek aktivn�, aktivita 
** se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
**/

	/* Pokud je prvni prvek aktivni, ztrati se aktivita seznamu */
	if ( L -> First == L -> Act )
	{
		L -> Act = NULL ;
	}

	/* Odstraneni prvniho prvku */
	tDLElemPtr pom ;				/* Vytvoreni pomocneho ukazatele */
	if ( L -> First != NULL )
	{
		pom = L -> First ;
		L -> First = pom -> rptr ;
		free ( pom ) ;				/* Uvolneni pomocneho ukazatele */
		L -> First -> lptr = NULL ;
	}
}	

void DLDeleteLast (tDLList *L) {
/*
** Zru�� posledn� prvek seznamu L. Pokud byl posledn� prvek aktivn�,
** aktivita seznamu se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
**/ 

	/* Pokud je posledni prvek aktivni, ztrati se aktivita seznamu */
	if ( L -> Last == L -> Act )
	{
		L -> Act = NULL ;
	}

	/* Odstraneni posledniho prvku */
	tDLElemPtr pom ;
	if ( L -> Last != NULL )
	{
		pom = L -> Last ;			/* Vytvoreni pomocneho ukazatele */
		L -> Last = pom -> lptr ;
		free ( pom ) ;				/* Uvolneni pomocneho ukazatele */
		L -> Last -> rptr = NULL ;
	}
}

void DLPostDelete (tDLList *L) {
/*
** Zru�� prvek seznamu L za aktivn�m prvkem.
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** posledn�m prvkem seznamu, nic se ned�je.
**/

	/* Preskoceni neaktivniho seznamu nebo seznamu s aktivnim poslednim prvkem */
	if ( L -> Act == NULL || L -> Act == L -> Last )
	{
		return ;
	}

	/* Vytvoreni pomocneho ukazatele */
	tDLElemPtr pom ;
	pom = L -> Act -> rptr ;
	
	/* Pokud je za rusenym prvkem dalsi prvek */
	if ( pom -> rptr != NULL )
	{
		L -> Act -> rptr = pom -> rptr ;
		pom -> rptr -> lptr = L -> Act ;
	}
	/* Pokud za rusenym prvkem dalsi prvek neni */
	else
	{
		L -> Act -> rptr = NULL ;
		L -> Last = L -> Act ;
	}

	free ( pom ) ;					/* Uvolneni pomocneho ukazatele */
}

void DLPreDelete (tDLList *L) {
/*
** Zru�� prvek p�ed aktivn�m prvkem seznamu L .
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** prvn�m prvkem seznamu, nic se ned�je.
**/

	/* Preskoceni neaktivniho seznamu nebo seznamu s aktivnim prvnim prvkem */
	if ( L -> Act == NULL || L -> Act == L -> First )
	{
		return ;
	}

	/* Vytvoreni pomocneho ukazatele */
	tDLElemPtr pom ;
	pom = L -> Act -> lptr ;

	/* Pokud je pred rusenym prvkem jeste nejaky prvek */
	if ( pom -> lptr != NULL )
	{
		L -> Act -> lptr = pom -> lptr ;
		pom -> lptr -> rptr = L -> Act ;
	}
	/* Pokud pred rusenym prvkem zadny jiny prvek neni */
	else
	{
		L -> Act -> lptr = NULL ;
		L -> First = L -> Act ;
	}

	free ( pom ) ;				/* Uvolneni pomocneho ukazatele */
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vlo�� prvek za aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/

	/* Preskoceni neaktivniho seznamu */
	if ( L -> Act == NULL )
	{
		return ;
	}

	/* Alokovani pameti pro novy prvek a osetreni funkce malloc */
	struct tDLElem *new_item ;
	if ( ( new_item = malloc ( sizeof ( struct tDLElem ) ) ) == NULL )
	{
		DLError () ;
		return ;
	}

	/* Naplneni noveho prvku seznamu */
	new_item -> data = val ;
	new_item -> rptr = NULL ;
	new_item -> lptr = NULL ;

	/* Pokud je za aktivnim prvkem jeste nejaky jiny prvek */
	if ( L -> Act -> rptr != NULL )
	{
		L -> Act -> rptr -> lptr = new_item ;
		new_item -> rptr = L -> Act -> rptr ;
	}
	/* Pokud za aktivnim prvkem zadny jiny prvek neni */
	else
	{
		L -> Last = new_item ;
	}

	/* Provazani aktivniho a nove pridaneho prvku */
	L -> Act -> rptr = new_item ;
	new_item -> lptr = L -> Act ;
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vlo�� prvek p�ed aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/

	/* Preskoceni neaktivniho seznamu */
	if ( L -> Act == NULL )
	{
		return ;
	}

	/* Alokovani pameti pro novy prvek a osetreni funkce malloc */
	struct tDLElem *new_item ;
	if ( ( new_item = malloc ( sizeof ( struct tDLElem ) ) ) == NULL )
	{
		DLError () ;
		return ;
	}
	
	/* Naplneni noveho prvku seznamu */
	new_item -> data = val ;
	new_item -> rptr = NULL ;
	new_item -> lptr = NULL ;

	/* Pokud je pred aktivnim prvkem jeste nejaky jiny prvek */
	if ( L -> Act -> lptr != NULL )
	{
		L -> Act -> lptr -> rptr = new_item ;
		new_item -> lptr = L -> Act -> lptr ;
	}
	/* Pokud pred aktivnim prvkem zadny jiny prvek neni */
	else
	{
		L -> First = new_item ;
	}

	/* Provazani aktivniho a nove pridaneho prvku */
	L -> Act -> lptr = new_item ;
	new_item -> rptr = L -> Act ;
}

void DLCopy (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, vol� funkci DLError ().
**/

	if ( L -> Act == NULL )
	{
		DLError () ;
		return ;
	}

	*val = L -> Act -> data ;
}

void DLActualize (tDLList *L, int val) {
/*
** P�ep�e obsah aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, ned�l� nic.
**/

	if ( L -> Act == NULL )
	{
		return ;
	}
	else
	{
		L -> Act -> data = val ;
	}
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na n�sleduj�c� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na posledn�m prvku se seznam stane neaktivn�m.
**/

	if ( L -> Act == NULL )
	{
		return ;
	}
	else
	{
		L -> Act = L -> Act -> rptr ;
	}
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na p�edchoz� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na prvn�m prvku se seznam stane neaktivn�m.
**/

	if ( L -> Act == NULL )
	{
		return ;
	}
	else
	{
		L -> Act = L -> Act -> lptr ;
	}
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivn�, vrac� nenulovou hodnotu, jinak vrac� 0.
** Funkci je vhodn� implementovat jedn�m p��kazem return.
**/

	return L -> Act != NULL ;
}

/* Konec c206.c*/
