	
/* c206.c **********************************************************}
{* Téma: Dvousmìrnì vázaný lineární seznam
**
**                   Návrh a referenèní implementace: Bohuslav Køena, øíjen 2001
**                            Pøepracované do jazyka C: Martin Tuèek, øíjen 2004
**                                            Úpravy: Bohuslav Køena, øíjen 2015
**
** Implementujte abstraktní datový typ dvousmìrnì vázaný lineární seznam.
** U¾iteèným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován promìnnou
** typu tDLList (DL znamená Double-Linked a slou¾í pro odli¹ení
** jmen konstant, typù a funkcí od jmen u jednosmìrnì vázaného lineárního
** seznamu). Definici konstant a typù naleznete v hlavièkovém souboru c206.h.
**
** Va¹ím úkolem je implementovat následující operace, které spolu
** s vý¹e uvedenou datovou èástí abstrakce tvoøí abstraktní datový typ
** obousmìrnì vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu pøed prvním pou¾itím,
**      DLDisposeList ... zru¹ení v¹ech prvkù seznamu,
**      DLInsertFirst ... vlo¾ení prvku na zaèátek seznamu,
**      DLInsertLast .... vlo¾ení prvku na konec seznamu, 
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek, 
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku, 
**      DLDeleteFirst ... zru¹í první prvek seznamu,
**      DLDeleteLast .... zru¹í poslední prvek seznamu, 
**      DLPostDelete .... ru¹í prvek za aktivním prvkem,
**      DLPreDelete ..... ru¹í prvek pøed aktivním prvkem, 
**      DLPostInsert .... vlo¾í nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vlo¾í nový prvek pøed aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... pøepí¹e obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na dal¹í prvek seznamu,
**      DLPred .......... posune aktivitu na pøedchozí prvek seznamu, 
**      DLActive ........ zji¹»uje aktivitu seznamu.
**
** Pøi implementaci jednotlivých funkcí nevolejte ¾ádnou z funkcí
** implementovaných v rámci tohoto pøíkladu, není-li u funkce
** explicitnì uvedeno nìco jiného.
**
** Nemusíte o¹etøovat situaci, kdy místo legálního ukazatele na seznam 
** pøedá nìkdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodnì komentujte!
**
** Terminologická poznámka: Jazyk C nepou¾ívá pojem procedura.
** Proto zde pou¾íváme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornìní na to, ¾e do¹lo k chybì.
** Tato funkce bude volána z nìkterých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální promìnná -- pøíznak o¹etøení chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L pøed jeho prvním pou¾itím (tzn. ¾ádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádìt nad ji¾ inicializovaným
** seznamem, a proto tuto mo¾nost neo¹etøujte. V¾dy pøedpokládejte,
** ¾e neinicializované promìnné mají nedefinovanou hodnotu.
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
** Zru¹í v¹echny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Ru¹ené prvky seznamu budou korektnì
** uvolnìny voláním operace free. 
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
** Vlo¾í nový prvek na zaèátek seznamu L.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
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
** Vlo¾í nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
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
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný pøíkaz (nepoèítáme-li return),
** ani¾ byste testovali, zda je seznam L prázdný.
**/

	L -> Act = L -> First ;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný pøíkaz (nepoèítáme-li return),
** ani¾ byste testovali, zda je seznam L prázdný.
**/

	L -> Act = L -> Last ;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
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
** Prostøednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
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
** Zru¹í první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se nedìje.
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
** Zru¹í poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se nedìje.
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
** Zru¹í prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se nedìje.
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
** Zru¹í prvek pøed aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se nedìje.
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
** Vlo¾í prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
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
** Vlo¾í prvek pøed aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
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
** Prostøednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
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
** Pøepí¹e obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedìlá nic.
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
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedìlá nic.
** V¹imnìte si, ¾e pøi aktivitì na posledním prvku se seznam stane neaktivním.
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
** Posune aktivitu na pøedchozí prvek seznamu L.
** Není-li seznam aktivní, nedìlá nic.
** V¹imnìte si, ¾e pøi aktivitì na prvním prvku se seznam stane neaktivním.
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
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním pøíkazem return.
**/

	return L -> Act != NULL ;
}

/* Konec c206.c*/
