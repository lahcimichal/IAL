
/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                      Úpravy: Radek Hranický, říjen 2014
**                      Úpravy: Radek Hranický, listopad 2015
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1 ;
	int keylen = strlen ( key ) ;
	for ( int i = 0 ; i < keylen ; i++ )
		retval += key[i] ;
	return ( retval % HTSIZE ) ;
}


/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht )
{
	if ( ( *ptrht ) != NULL )
	{
		/* Vsechny ukazatele 0 .. ( HTSIZE - 1 ) nastavi na hodnotu NULL */
		for ( int i = 0 ; i < HTSIZE ; i++ )
		{
			( *ptrht )[i] = NULL ;
		}
	}
}


/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není, 
** vrací se hodnota NULL.
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key )
{
	if ( ( *ptrht ) != NULL )
	{
		/* Zjisti hash - index pole */
		int hash_key = hashCode ( key ) ;

		/* Do pomocneho prvku vlozi prvni prvek seznamu, ktery bude prohledavat */
		tHTItem *tmp = ( *ptrht )[hash_key] ;


		while ( tmp != NULL )
		{
			/* Pri shode klice vraci ukazatel na dany prvek */
			if ( tmp -> key == key )
			{
				return tmp ;
			}

			/* Jinak prejde na dalsi prvek */
			else
			{
				tmp = tmp -> ptrnext ;
			}
		}
	}

	return NULL ;
}


/* 
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data )
{
	if ( ( *ptrht ) != NULL )
	{
		/* Pomoci funkce htSearch zjisti, jestli se prvek v tabulce jiz nachazi */
		/* Pokud ano, ulozi ho do pomocneho prvku */
		tHTItem *tmp = htSearch ( ptrht , key ) ;


		/* Pokud prvek najde, aktualizuje data */
		if ( tmp != NULL )
		{
			tmp -> data = data ;
		}
		
		/* Pokud prvek nenajde, vytvori novy prvek a da ho na zacatek seznamu */
		else
		{
			/* Zjisti hash */
			int hash_key = hashCode ( key ) ;


			/* Alokace prostoru pro novy prvek */
			tHTItem *new_item = malloc ( sizeof ( struct tHTItem ) ) ;

			/* Vlozeni klice a dat */
			new_item -> key = key ;
			new_item -> data = data ;

			/* Navazani zbytku seznamu */
			new_item -> ptrnext = ( *ptrht )[hash_key] ;

			/* Navazani do tabulky */
			( *ptrht )[hash_key] = new_item ;
		}
	}
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key )
{
	if ( ( *ptrht ) != NULL )
	{
		/* Pomoci funkce htSearch zjisti, jestli se prvek v tabulce jiz nachazi */
		/* Pokud ano, ulozi ho do pomocneho prvku */
		tHTItem *tmp = htSearch ( ptrht , key ) ;


		/* Pokud prvek najde, vrati ukazatel na jeho data */
		if ( tmp != NULL )
		{
			tData *item_data = &( tmp -> data ) ;
			return item_data ;
		}

		/* Jinak vraci NULL */
		else
		{
			return NULL ;
		}
	}

	return NULL ;
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key )
{
	if ( ( *ptrht ) != NULL )
	{
		/* Zjisti hash */
		int hash_key = hashCode ( key ) ;

		/* Do pomocnych promennych da prvni prvek seznamu */
		tHTItem *tmp1 = ( *ptrht )[hash_key] ;
		tHTItem *tmp2 = ( *ptrht )[hash_key] ;


		/* Prochazi postupne cely seznam */
		while ( tmp1 != NULL )
		{
			/* Pokud najde klic */
			if ( tmp1 -> key == key )
			{
				if ( tmp1 == ( *ptrht )[hash_key] )
				{
					( *ptrht )[hash_key] = tmp1 -> ptrnext ;
				}
				else
				{
					tmp2 -> ptrnext = tmp1 -> ptrnext ;
				}

				free ( tmp1 ) ;
			}

			/* Pokud klic nenajde, postupuje dal */
			tmp2 = tmp1 ;
			tmp1 = tmp1 -> ptrnext ;
		}
	}
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht )
{
	if ( ( *ptrht ) != NULL )
	{
		/* Pomocnou promennou nastavi na NULL */
		tHTItem *tmp = NULL ;
		

		/* Postupne prochazi radky tabulky */
		for ( int i = 0 ; i < HTSIZE ; i++ )
		{
			/* Dokud prvni prvek seznamu neni NULL */
			while ( ( *ptrht )[i] != NULL )
			{
				tmp = ( *ptrht )[i] ;
				( *ptrht )[i] = ( *ptrht )[i] -> ptrnext ;
				free ( tmp ) ;
			}

			/* Prvni prvek seznamu nastavi na NULL */
			( *ptrht )[i] = NULL ;
		}
	}
}
