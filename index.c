#include <stdio.h>
#include <stdlib.h>

struct ksiazki {
    long long nr_isbn;
    int nr_katalogowy, dostepnosc;
    char *autor, *nazwa, *kategoria, *data_wydania, *wydawnictwo;
    struct ksiazki *next;
    struct ksiazki *prev;
};

struct klient {
    int telefon;
    char *imie, *nazwisko, *adres;
    struct ksiazki *wypozyczone;
};

void wyswietlMenu();

void wyswietlMenuKsiazek();

void wyswietlMenuKlientow();

int main() {
    //wyswietlMenu();
    struct ksiazki *ks1 = malloc(sizeof(struct ksiazki));
    ks1->nr_katalogowy = 2137;
    ks1->nr_isbn = 666696964202137;
    ks1->dostepnosc = 5;
    ks1->autor = "wiesio";
    ks1->nazwa = "wiedzmak";
    ks1->kategoria = "fantasy";
    ks1->data_wydania = "21-37-2137";
    ks1->wydawnictwo = "KujwDubie";
    ks1->next = NULL;
    ks1->prev = NULL;
    struct klient *k1 = malloc(sizeof(struct klient));
    k1->adres = "ul. Leczna 44, Chojmianki";
    k1->telefon = 213769666;
    k1->imie = "Zdzisio";
    k1->nazwisko = "Brzeczeszczykiewicz";
    k1->wypozyczone = ks1;
    printf("%s", k1->wypozyczone->data_wydania);
    return 0;
}

void wyswietlMenu() {
    printf("1. Menu ksiazek\n");
    printf("2. Menu klientow\n");
    printf("0. Wyjscie z programu\n");
    int x;
    scanf("%d", &x);
    do {
        if (x == 1) {
            wyswietlMenuKsiazek();
        } else if (x == 2) {
            wyswietlMenuKlientow();
        } else if (x == 0) {
            break;
        }
    } while (x != 0);
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
    for (int i = 0; i < 7; ++i) {
        printf("%i. %s", i, menu[i]);
    }
    int x, end = 0;
    scanf("%d", &x);
    while (!end) {
        switch (x) {
            case 0:
                end = 1;
                break;
            case 4:

                break;
        }
    }
}

void wyswietlMenuKlientow() {
    char menu[][100] = {"wyswietl klientow posortowanych alfabetycznie\n",
                        "wyswietl klientow posortowanych wzgledem wypozyczonych ksiazek\n",
                        "dodaj klienta\n",
                        "edytuj klienta\n",
                        "usun klienta\n",
    };
    for (int i = 0; i < 5; ++i) {
        printf("%i. %s", i + 1, menu[i]);
    }
}