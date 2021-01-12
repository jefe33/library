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

void usunKsiazke();

void usunAutora();

void usunRok();

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
    char menu[][100] = {"Wroc",
                        "wyswietl ksiazki posortowane wzgledem autora",
                        "wyswietl ksiazki posortowane wzgledem tytulu",
                        "wyswietl ksiazki posortowane wzgledem dostepnosci",
                        "dodaj ksiazke",
                        "edytuj ksiazke",
                        "usun ksiazke",
                        "skasuj ksiazki wedlug autora",
                        "skasuj ksiazki wedlug roku wydania",
                        "dodaj losowe dane",
    };
    int x;
    do {
        for (int i = 0; i < 10; ++i) {
            printf("%i. %s\n", i, menu[i]);
        }
        scanf("%d", &x);
        switch (x) {
            case 0:
                wyswietlMenu();
                break;
            case 1:
                wyswietlKsiazki(2);
                break;
            case 2:
                wyswietlKsiazki(0);
                break;
            case 3:
                wyswietlKsiazki(1);
                break;
            case 4:
                dodajKsiazke(0);
                break;
            case 5:
                edytujKsiazke();
                break;
            case 6:
                usunKsiazke();
                break;
            case 7:
                usunAutora();
                break;
            case 8:
                usunRok();
                break;
            case 9:
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
                wyswietlKlientow(0);
                break;
            case 2:
                wyswietlKlientow(1);
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
        if (x == 0) {
            wyswietlMenu();
        } else if (x == 1) {
            dodajWypozyczenie();
        } else if (x == 2) {
            usunWypozyczenie();
        } else if (x == 3) {
            wyswietlWypozyczenia();
        }
    } while (x != 0);

}

int czyWypozyczona(struct wypozyczenia *wy, int num) {
    int i = 0;
    struct wypozyczenia *tmp2 = wy;
    while (tmp2) {
        if (tmp2->index_ksiazki == num) {
            i = 1;
            break;
        }
        tmp2 = tmp2->next;
    }
    return i;
}

void usunKsiazke() {
    int max, i = 0, nr;
    printf("Podaj numer ksiazki ktora chcesz usunac\n");
    scanf("%i", &nr);
    FILE *f = fopen("../pliki/ksiazki.bin", "rb");
    FILE *w = fopen("../pliki/wypozyczenia.bin", "rb");
    if (f) {
        struct ksiazki *ks = wczytajKsiazki(f, &max);
        struct ksiazki *tmp = ks;
        struct wypozyczenia *wyp = wczytajWypozyczenia(w, &max);
        if (tmp->nr_katalogowy == nr) {
            i = 1;
            if (czyWypozyczona(wyp, tmp->nr_katalogowy)) {
                i = 2;
            } else {
                ks = tmp->next;
            }
        } else {
            struct ksiazki *prev = ks;
            tmp = ks;
            while (tmp) {
                if (tmp->nr_katalogowy == nr) {
                    i = 1;
                    if (czyWypozyczona(wyp, tmp->nr_katalogowy)) {
                        i = 2;
                    } else {
                        prev->next = tmp->next;
                    }
                    break;
                } else {
                    prev = tmp;
                }
                tmp = tmp->next;
            }
        }
        if (i == 1) {
            tmp = ks;
            fclose(f);
            f = fopen("../pliki/ksiazki.bin", "wb");
            while (tmp && fwrite(tmp, sizeof(struct ksiazki), 1, f) == 1) {
                tmp = tmp->next;
            }
        } else if (i == 2) {
            printf("Nie mozna usunac ksiazki bo jest wypozyczona\n");
        }
        uwolnicKsiazki(ks);
        uwolnicWyporzyczenia(wyp);
    }
    fclose(f);
}

void usunRok() {
    int max, i = 1, rok;
    clear();
    printf("Podaj rok wydania ktory chcesz usunac\n");
    scanf("%i", &rok);
    FILE *f = fopen("../pliki/ksiazki.bin", "rb");
    FILE *w = fopen("../pliki/wypozyczenia.bin", "rb");
    if (f) {
        struct ksiazki *ks = wczytajKsiazki(f, &max);
        struct ksiazki *tmp = ks;
        struct wypozyczenia *wyp = wczytajWypozyczenia(w, &max);
        while (tmp && i) {
            if (tmp->data_wydania == rok && czyWypozyczona(wyp, tmp->nr_katalogowy) == 0) {
                ks = tmp->next;
            } else {
                i = 0;
            }
            tmp = tmp->next;
        }
        struct ksiazki *prev = ks;
        tmp = ks->next;
        while (tmp) {
            if (tmp->data_wydania == rok && czyWypozyczona(wyp, tmp->nr_katalogowy) == 0) {
                prev->next = tmp->next;
            } else {
                prev = tmp;
            }
            tmp = tmp->next;
        }
        tmp = ks;
        fclose(f);
        f = fopen("../pliki/ksiazki.bin", "wb");
        while (tmp && fwrite(tmp, sizeof(struct ksiazki), 1, f) == 1) {
            tmp = tmp->next;
        }
        uwolnicKsiazki(ks);
        uwolnicWyporzyczenia(wyp);
    }
    fclose(f);
}

void usunAutora() {
    char *s;
    clear();
    printf("Podaj nazwisko autora ktorego chcesz usunac\n");
    s = wczytajString();
    FILE *f = fopen("../pliki/ksiazki.bin", "rb");
    FILE *w = fopen("../pliki/wypozyczenia.bin", "rb");
    int max, i = 1;
    if (f != NULL) {
        struct ksiazki *ks = wczytajKsiazki(f, &max);
        struct ksiazki *tmp = ks;
        struct wypozyczenia *wyp = wczytajWypozyczenia(w, &max);
        while (tmp && i) {
            if (strcmp(tmp->autor_nazwisko, s) == 0 && czyWypozyczona(wyp, tmp->nr_katalogowy) == 0) {
                ks = tmp->next;
            } else {
                i = 0;
            }
            tmp = tmp->next;
        }
        struct ksiazki *prev = ks;
        tmp = ks->next;
        while (tmp) {
            if (strcmp(tmp->autor_nazwisko, s) == 0 && czyWypozyczona(wyp, tmp->nr_katalogowy) == 0) {
                prev->next = tmp->next;
            } else {
                prev = tmp;
            }
            tmp = tmp->next;
        }
        tmp = ks;
        fclose(f);
        f = fopen("../pliki/ksiazki.bin", "wb");
        while (tmp && fwrite(tmp, sizeof(struct ksiazki), 1, f) == 1) {
            tmp = tmp->next;
        }
        uwolnicKsiazki(ks);
        uwolnicWyporzyczenia(wyp);
    }
    fclose(f);
}
