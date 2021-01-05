#include <stdio.h>

#include "ksiazki.h"

struct klient {
    int telefon;
    char *imie, *nazwisko, *adres;
    struct ksiazki *wypozyczone;
};

void wyswietlMenu();

void wyswietlMenuKsiazek();

void wyswietlMenuKlientow();

int main() {
    wyswietlMenu();
    return 0;
}

void wyswietlMenu() {
    printf("1. Menu ksiazek\n");
    printf("2. Menu klientow\n");
    printf("0. Wyjscie z programu\n");
    int x;
    scanf("%d", &x);
    if (x == 0);
    else if (x == 1) {
        wyswietlMenuKsiazek();
    } else if (x == 2) {
        wyswietlMenuKlientow();
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
    char menu[][70] = {"wyswietl klientow posortowanych alfabetycznie\n",
                       "wyswietl klientow posortowanych wzgledem wypozyczonych ksiazek\n",
                       "dodaj klienta\n",
                       "edytuj klienta\n",
                       "usun klienta\n",
    };
    for (int i = 0; i < 6; ++i) {
        printf("%i. %s", i + 1, menu[i]);
    }
}