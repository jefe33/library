#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 50
struct ksiazki {
    long long nr_isbn;
    int nr_katalogowy, dostepnosc;
    char autor[MAX], tytul[MAX], kategoria[MAX], data_wydania[MAX], wydawnictwo[MAX];
    struct ksiazki *next;
    struct ksiazki *prev;
};

struct klient {
    int telefon;
    char *imie, *nazwisko, *adres;
    struct ksiazki *wypozyczone;
};

int is_empty(char *s) {
    while (*s != '\0') {
        if (!isspace((unsigned char) *s))
            return 0;
        s++;
    }
    return 1;
}

void wczytajKsiazki();

void wyswietlMenu();

void wyswietlMenuKsiazek();

void wyswietlMenuKlientow();

void dodajKsiazke();

int main() {
    wyswietlMenu();
    /*struct ksiazki *ks1 = malloc(sizeof(struct ksiazki));
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
    printf("%s", k1->wypozyczone->data_wydania);*/
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
    for (int i = 0; i < 7; ++i) {
        printf("%i. %s", i, menu[i]);
    }
    int x;
    scanf("%d", &x);
    switch (x) {
        case 0:
            wyswietlMenu();
            break;
        case 4:
            dodajKsiazke();
            break;
        default:
            wyswietlMenu();
            break;
    }
}

void wyswietlMenuKlientow() {
    char menu[][70] = {"wyswietl klientow posortowanych alfabetycznie\n",
                       "wyswietl klientow posortowanych wzgledem wypozyczonych ksiazek\n",
                       "dodaj klienta\n",
                       "edytuj klienta\n",
                       "usun klienta\n",
    };
    for (int i = 0; i < 5; ++i) {
        printf("%i. %s", i + 1, menu[i]);
    }
}

char *wczytajKsiazka(char *s) {
    char *z = malloc(MAX * sizeof(char));
    do {
        printf("%s\n", s);
        fgets(z, MAX, stdin);
    } while (is_empty(z));
    return z;
}

void dodajKsiazke() {
    FILE *f = fopen("../pliki/ksiazki.txt", "a+");
    char nr[15];
    struct ksiazki *k1 = malloc(sizeof(struct ksiazki));
    int i = 0;
    if (f != NULL) {
        struct ksiazki *k = malloc(sizeof(struct ksiazki));
        while (!feof(f)) {
            fscanf(f, "%i", &k[i].nr_katalogowy);
            fscanf(f, "%i", &k[i].dostepnosc);
            fscanf(f, "%lld", &k[i].nr_isbn);
            printf("%i %i %lld\n", k[i].nr_katalogowy, k[i].dostepnosc, k[i].nr_isbn);
            fgets(k[i].autor, MAX, f);
            puts(k[i].autor);
            fgets(k[i].tytul, MAX, f);
            fgets(k[i].kategoria, MAX, f);
            fgets(k[i].data_wydania, MAX, f);
            fgets(k[i].wydawnictwo, MAX, f);
            k = realloc(k, (++i + 1) * sizeof(struct ksiazki));
        }
        k[1].kategoria;
    }
    /*k1->nr_katalogowy = 1;
    k1->dostepnosc = 1;
    getchar();
    strcpy(k1->tytul, wczytajKsiazka("Podaj tytul ksiazki"));
    strcpy(k1->autor, wczytajKsiazka("Podaj autora ksiazki"));
    strcpy(k1->kategoria, wczytajKsiazka("Podaj kategorie ksiazki"));
    strcpy(k1->data_wydania, wczytajKsiazka("Podaj date wydania w formacie DD-MM-RRRR"));
    strcpy(k1->wydawnictwo, wczytajKsiazka("Podaj wydawnictwo ksiazki"));
    do {
        strcpy(nr, wczytajKsiazka("Podaj 13-nasto cyfrowy numer ISBN"));
    } while (strlen(nr) != 14);
     nr[13] = '\0';
    k1->data_wydania[strlen(k1->data_wydania)-1] = '\0';
    fprintf(f, "\n%i %i %s%s%s%s%s%s", k1->nr_katalogowy, k1->dostepnosc, nr, k1->autor, k1->tytul, k1->kategoria,
            k1->wydawnictwo,k1->data_wydania);*/
    fclose(f);
    wyswietlMenuKsiazek();

}