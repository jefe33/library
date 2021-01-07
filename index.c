#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 50

#include "ksiazki.h"
#include "klienci.h"
#include "wypozyczenia.h"

struct klient {
    int telefon;
    char *imie, *nazwisko, *adres;
    struct ksiazki *wypozyczone;
};

void wyswietlMenu();

void wyswietlMenuKsiazek();

void wyswietlMenuKlientow();

void wyswietlMenuWypozyczen();

int main() {
    wyswietlMenu();
    return 0;
}

void wyswietlMenu() {
    printf("1. Menu ksiazek\n");
    printf("2. Menu klientow\n");
    printf("3. Menu wypozyczen\n");
    printf("0. Wyjscie z programu\n");
    int x;
    scanf("%d", &x);
    if (x == 0);
    else if (x == 1) {
        wyswietlMenuKsiazek();
    } else if (x == 2) {
        wyswietlMenuKlientow();
    } else if (x == 3) {
        wyswietlMenuWypozyczen();
    } else {
        wyswietlMenu();
    }
}

void wyswietlMenuKsiazek() {
    char menu[][100] = {"Wroc\n",
                        "wyswietl ksiazki posortowane wzgledem autora\n",
                        "wyswietl ksiazki posortowane wzgledem tytulu\n",
                        "wyswietl ksiazki posortowane wzgledem dostepnosci\n",
                        "dodaj ksiazke\n",
                        "edytuj ksiazke\n",
                        "skasuj ksiazki wedlug autora\n",
                        "skasuj ksiazki wedlug roku wydania\n",
                        "dodaj losowe dane\n",
    };
    int x;
    do {
        for (int i = 0; i < 9; ++i) {
            printf("%i. %s", i, menu[i]);
        }
        scanf("%d", &x);
        switch (x) {
            case 0:
                wyswietlMenu();
                break;
            case 1:
                wyswietlWedlugAutora();
                break;
            case 2:
                wyswietlWedlugTytulu();
                break;
            case 4:
                dodajKsiazke(0);
                break;
            case 5:
                edytujKsiazke();
                break;
            case 6:
                usunAutora();
                break;
            case 7:
                usunRok();
                break;
            case 8:
                dodajKsiazke(1);
                break;
            default:
                wyswietlMenu();
                break;
        }
    } while (x != 0);
}


void wyswietlMenuKlientow() {
    char menu[][70] = {"Wroc", "wyswietl klientow posortowanych alfabetycznie",
                       "wyswietl klientow posortowanych wzgledem wypozyczonych ksiazek",
                       "dodaj klienta",
                       "edytuj klienta",
                       "usun klienta",
                       "dodaj losowego klienta\n"
    };
    int x;
    do {
        for (int i = 0; i < 7; ++i) {
            printf("\n%i. %s", i, menu[i]);
        }
        scanf("%d", &x);
        switch (x) {
            case 0:
                wyswietlMenu();
                break;
            case 1:
                wyswietlWedlugNazwiska();
                break;
            case 2:
                //wyswietlWedlugTytulu();
                break;
            case 3:
                dodajKlienta(0);
                break;
            case 4:
                edytujKlienta();
                break;
            case 5:
                usunKlienta();
                break;
            case 6:
                dodajKlienta(1);
                break;
            default:
                wyswietlMenu();
                break;
        }
    } while (x != 0);
}

void wyswietlMenuWypozyczen() {
    int x;
    do {
        printf("1. Dodaj wypozyczenie\n");
        printf("2. Usun wypozyczenie\n");
        printf("3. Wyswietl wypozyczenia\n");
        printf("0. Wroc\n");
        scanf("%d", &x);
        if (x == 1) {
            dodajWypozyczenie();
        } else if (x == 2) {
            //
        } else if (x == 3) {
            wyswietlWypozyczenia();
        }
    } while (x != 0);

}