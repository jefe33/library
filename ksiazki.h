#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 50
struct ksiazki {
    long long nr_isbn;
    int nr_katalogowy, dostepnosc;
    char autor_nazwisko[MAX], autor_imie[MAX], tytul[MAX], kategoria[MAX], data_wydania[MAX], wydawnictwo[MAX];
    struct ksiazki *next;
};

struct ksiazki *losowaKsiazka();

int is_empty(char *s) {
    while (*s != '\0') {
        if (!isspace((unsigned char) *s))
            return 0;
        s++;
    }
    return 1;
}

char *wczytajString() {
    char *s = malloc(sizeof(char)), c;
    int i = 0;
    while ((c = getchar()) != '\n') {
        s[i] = c;
        s = realloc(s, ++i * sizeof(char));
    }
    s[i] = '\0';
    return s;
}

char *wczytajKsiazka(char *s) {
    char *z;
    do {
        printf("%s\n", s);
        z = wczytajString();
    } while (is_empty(z));
    return z;
}

struct ksiazki *utworzKsiazke() {
    long long x;
    getchar();
    struct ksiazki *ksiazka = malloc(sizeof(struct ksiazki));
    strcpy(ksiazka->tytul, wczytajKsiazka("Podaj tytul ksiazki"));
    strcpy(ksiazka->autor_nazwisko, wczytajKsiazka("Podaj nazwisko autora ksiazki"));
    strcpy(ksiazka->autor_imie, wczytajKsiazka("Podaj imie autora ksiazki"));
    strcpy(ksiazka->kategoria, wczytajKsiazka("Podaj kategorie ksiazki"));
    strcpy(ksiazka->wydawnictwo, wczytajKsiazka("Podaj wydawnictwo ksiazki"));
    strcpy(ksiazka->data_wydania, wczytajKsiazka("Podaj date wydania w formacie DD-MM-RRRR"));
    ksiazka->dostepnosc = 1;
    do {
        printf("Podaj 13-nasto cyfrowy numer ISBN\n");
        scanf("%lld", &x);
    } while (x < 1000000000000 || x > 9999999999999);
    ksiazka->nr_isbn = x;
    return ksiazka;
}

void dodajKsiazke(int n) {
    struct ksiazki *k1;
    if (n) {
        k1 = losowaKsiazka();
    } else {
        k1 = utworzKsiazke();
    }
    FILE *f = fopen("../pliki/ksiazki.txt", "r");
    int i = 0, j, m = 0;
    if (f != NULL) {
        fseek(f, 0, SEEK_END);
        if (ftell(f) == 0) {
            fprintf(f, "%i %i %lld%s\n%s\n%s\n%s\n%s\n%s", k1->nr_katalogowy, k1->dostepnosc, k1->nr_isbn,
                    k1->autor_nazwisko,
                    k1->autor_imie,
                    k1->tytul, k1->kategoria,
                    k1->wydawnictwo, k1->data_wydania);
        } else {
            fseek(f, 0, SEEK_SET);
            struct ksiazki *k = malloc(sizeof(struct ksiazki));
            while (!feof(f)) {
                fscanf(f, "%i", &k[i].nr_katalogowy);
                fscanf(f, "%i", &k[i].dostepnosc);
                fscanf(f, "%lld", &k[i].nr_isbn);
                fgets(k[i].autor_nazwisko, MAX, f);
                fgets(k[i].autor_imie, MAX, f);
                fgets(k[i].tytul, MAX, f);
                fgets(k[i].kategoria, MAX, f);
                fgets(k[i].wydawnictwo, MAX, f);
                fgets(k[i].data_wydania, MAX, f);
                k[i].data_wydania[strlen(k[i].data_wydania) - 1] = '\0';
                k = realloc(k, (++i + 1) * sizeof(struct ksiazki));
            }
            k1->nr_katalogowy = i + 1;
            for (j = 0; j < i; ++j) {
                if (strcasecmp(k1->autor_nazwisko, k[j].autor_nazwisko) < 0) {
                    break;
                }
            }
            fclose(f);
            f = fopen("../pliki/ksiazki.txt", "w");
            while (m < i) {
                if (j == m) {
                    if (m != 0) fprintf(f, "\n");
                    fprintf(f, "%i %i %lld%s\n%s\n%s\n%s\n%s\n%s", k1->nr_katalogowy, k1->dostepnosc, k1->nr_isbn,
                            k1->autor_nazwisko,
                            k1->autor_imie,
                            k1->tytul, k1->kategoria,
                            k1->wydawnictwo, k1->data_wydania);
                }
                if (m != 0) fprintf(f, "\n");
                fprintf(f, "%i %i %lld%s%s%s%s%s%s", k[m].nr_katalogowy, k[m].dostepnosc, k1->nr_isbn,
                        k[m].autor_nazwisko,
                        k[m].autor_imie,
                        k[m].tytul, k[m].kategoria,
                        k[m].wydawnictwo, k[m].data_wydania);
                m++;
            }
            if (j == i) {
                fprintf(f, "\n%i %i %lld%s\n%s\n%s\n%s\n%s\n%s", k1->nr_katalogowy, k1->dostepnosc, k1->nr_isbn,
                        k1->autor_nazwisko,
                        k1->autor_imie,
                        k1->tytul, k1->kategoria,
                        k1->wydawnictwo, k1->data_wydania);
            }
        }
    }
    fclose(f);
}

struct ksiazki *losowaKsiazka() {
    srand(time(0));
    struct ksiazki *ksiazka = malloc(sizeof(struct ksiazki));
    char imie[13][MAX] = {"Jane", "Emily", "George", "Philip", "Charles", "Thomas", "Jan", "Pawel", "Joseph",
                          "Margaret", "Dan", "Leo", "Arthur"};
    char nazwisko[12][MAX] = {"Tolkien", "Rowling", "Lee", "Dickens", "Adams", "Carroll", "Grahame", "Orwell", "Brown",
                              "Golding", "Huxley", "Conan Doyle"};
    char tytul[16][MAX] = {"Wladca Pierscieni", "Biblia", "Wielkie nadzieje", "Rok 1984", "Zbrodnia i kara",
                           "Folwark zwierzecy", "Kod Da Vinci", "Wladca much", "W drodze", "Moby Dick", "Germinal",
                           "Opetanie", "Okruchy dnia", "Pani Bovary", "A Fine Balance", "Przygody Scherlocka Holmesa"};
    char kategoria[13][MAX] = {"fantasy", "horror", "SF", "literatura faktu", "literatura piekna", "thriller",
                               "kryminal", "romans", "sensacja", "przygodowa", "historyczna", "popularnonaukowa"};
    char data_wydania[12][MAX] = {"05-01-1937", "02-01-1923", "04-07-1918", "26-04-1907", "06-11-2003", "01-01-1959",
                                  "20-10-2000", "20-02-1939", "15-01-2013", "05-04-1991", "25-03-2020", "09-08-1856"};
    char wydawnictwo[13][MAX] = {"Helion", "Znak", "Amber", "Impuls", "Mag", "Replika", "WSiP", "Nowa Era", "W drodze",
                                 "SBM", "ISA", "Kos", "Bosz"};
    long long isbn[12] = {2005917850652, 7137334183392, 1460338048199, 1771121865414, 3827938534880, 7527675674564,
                          2728983381469, 4113757760531, 5191182644763, 5881868774856, 9613158509137, 5320278067692};
    ksiazka->dostepnosc = 1;
    strcpy(ksiazka->autor_nazwisko, &nazwisko[rand() % 12][MAX]);
    strcpy(ksiazka->autor_imie, &imie[rand() % 13][MAX]);
    strcpy(ksiazka->tytul, &tytul[rand() % 16][MAX]);
    strcpy(ksiazka->kategoria, &kategoria[rand() % 13][MAX]);
    strcpy(ksiazka->data_wydania, &data_wydania[rand() % 12][MAX]);
    strcpy(ksiazka->wydawnictwo, &wydawnictwo[rand() % 13][MAX]);
    ksiazka->nr_isbn = isbn[rand() % 12];
    return ksiazka;
}