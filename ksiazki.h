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

struct ksiazki *ReadBooks(FILE *in_stream) {
    struct ksiazki head;
    head.next = NULL; // code only uses the `next` field of head

    struct ksiazki *previous = &head;
    struct ksiazki x;

    // While another record was successfully read ...
    while (fread(&x, sizeof(struct ksiazki), 1, in_stream) == 1) {
        // Fill the next field
        x.next = NULL;

        // Allocate space and copy
        previous->next = malloc(sizeof *(previous->next));
        //assert(previous->next);
        *(previous->next) = x;

        // Advance to the next
        previous = previous->next;
    }
    return head.next;
}

int is_empty(char *s) {
    while (*s != '\0') {
        if (!isspace((unsigned char) *s))
            return 0;
        s++;
    }
    return 1;
}

void uwolnicBarabasza(struct ksiazki *t) {
    struct ksiazki *tmp;
    while (t) {
        tmp = t;
        t = t->next;
        free(tmp);
    }
    free(t);
}

void clear() {
    while (getchar() != '\n');
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
    int y;
    FILE *f2 = fopen("../pliki/dane.txt", "r");
    fscanf(f2, "nr_katalogowy:%i", &y);
    ksiazka->nr_katalogowy = y + 1;
    fclose(f2);
    return ksiazka;
}

void wyswietlWedlugAutora() {
    FILE *f = fopen("../pliki/ksiazki.txt", "r");
    char ch[MAX];
    int x;
    long long y;
    if (f != NULL) {
        printf("-----------------------------------------------------------------------------------------------------------------------\n");
        printf("|nr |         autor        |          tytul          |    kategoria     | wydawnictwo |data_wydania|nr_ISBN|dostepnosc|\n");
        printf("-----------------------------------------------------------------------------------------------------------------------\n");
        while (!feof(f)) {
            fscanf(f, "%i", &x);
            printf("|%-3i|", x);
            fscanf(f, "%i", &x);
            fscanf(f, "%lld", &y);
            fgets(ch, MAX, f);
            ch[strlen(ch) - 1] = '\0';
            printf("%-10s ", ch);
            fgets(ch, MAX, f);
            ch[strlen(ch) - 1] = '\0';
            printf("%-10s |", ch);
            fgets(ch, MAX, f);
            ch[strlen(ch) - 1] = '\0';
            printf("%-24s |", ch);
            fgets(ch, MAX, f);
            ch[strlen(ch) - 1] = '\0';
            printf("%-17s |", ch);
            fgets(ch, MAX, f);
            ch[strlen(ch) - 1] = '\0';
            printf("%-12s |", ch);
            fgets(ch, MAX, f);
            ch[strlen(ch) - 1] = '\0';
            printf("%-10s |", ch);
            printf(" %lld |", y);
            printf(" %i |\n", x);


        }
    }
}

struct ksiazki *wstawPosortowane(struct ksiazki *l, struct ksiazki *ks) {
    if (strcmp(l->autor_nazwisko, ks->autor_nazwisko) > 0) {
        ks->next = l;
        l = ks;
        return l;
    }
    struct ksiazki *tmp = l;
    while (tmp) {
        if (tmp->next == NULL) {
            tmp->next = ks;
            break;
        } else {
            if (tmp->next && strcmp(tmp->next->autor_nazwisko, ks->autor_nazwisko) > 0) {
                ks->next = tmp->next;
                tmp->next = ks;
                break;
            }
        }
        tmp = tmp->next;
    }
    return l;
}

void dodajKsiazke(int n) {
    char *Pksiazki = "../pliki/ksiazki.bin";
    char *dane = "../pliki/dane.txt";
    struct ksiazki *k1;
    if (n) {
        k1 = losowaKsiazka();
    } else {
        k1 = utworzKsiazke();
    }
    FILE *f = fopen(Pksiazki, "rb+");
    FILE *f2 = fopen(dane, "r");
    if (f) {
        fseek(f, 0, SEEK_END);
        if (ftell(f) != 0) {
            rewind(f);
            struct ksiazki *k2 = ReadBooks(f);
            k2 = wstawPosortowane(k2, k1);
            fclose(f);
            f = fopen(Pksiazki, "wb");
            struct ksiazki *tmp = k2;
            while (tmp && fwrite(tmp, sizeof(struct ksiazki), 1, f) == 1) {
                printf("%i %i %lld %s %s %s %s %s %s\n", tmp->nr_katalogowy, tmp->dostepnosc, tmp->nr_isbn,
                       tmp->autor_nazwisko, tmp->autor_imie, tmp->tytul, tmp->kategoria, tmp->wydawnictwo,
                       tmp->data_wydania);
                tmp = tmp->next;
            }
            int x;
            fscanf(f2, "nr_katalogowy:%i", &x);
            fclose(f2);
            f2 = fopen(dane, "w");
            fprintf(f2, "nr_katalogowy:%i", x + 1);
            uwolnicBarabasza(k2);
        } else {
            fclose(f);
            f = fopen(Pksiazki, "wb");
            if (fwrite(k1, sizeof(struct ksiazki), 1, f) == 1) {
                fclose(f2);
                f2 = fopen(dane, "w");
                fprintf(f2, "nr_katalogowy:%i", 1);
            }
            /*printf("\n%i %i %lld %s %s %s %s %s %s\n", k1->nr_katalogowy, k1->dostepnosc, k1->nr_isbn,
                   k1->autor_nazwisko, k1->autor_imie, k1->tytul, k1->kategoria, k1->wydawnictwo, k1->data_wydania);
            fseek(f, 0, SEEK_SET);
            struct ksiazki *k = malloc(sizeof(struct ksiazki));
            while (!feof(f)) {
                k = realloc(k, (i++ + 1) * sizeof(struct ksiazki));
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
                fprintf(f, "%i %i %lld%s%s%s%s%s%s", k[m].nr_katalogowy, k[m].dostepnosc, k[m].nr_isbn,
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
            free(k);*/
        }
    } else {
        fclose(f);
        f = fopen(Pksiazki, "wb");
        if (fwrite(k1, sizeof(struct ksiazki), 1, f) == 1) {
            fclose(f2);
            f2 = fopen(dane, "w");
            fprintf(f2, "nr_katalogowy:%i", 1);
        }
    }
    free(k1);
    fclose(f);
    fclose(f2);
}

struct ksiazki *losowaKsiazka() {
    srand(time(0));
    struct ksiazki *ksiazka = malloc(sizeof(struct ksiazki));
    char *imie[MAX] = {"Jane", "Emily", "George", "Philip", "Charles", "Thomas", "Jan", "Pawel", "Joseph",
                       "Margaret", "Dan", "Leo", "Arthur"};
    char *nazwisko[MAX] = {"Tolkien", "Rowling", "Lee", "Dickens", "Adams", "Carroll", "Grahame", "Orwell",
                           "Brown", "Golding", "Huxley", "Conan Doyle"};
    char *tytul[MAX] = {"Wladca Pierscieni", "Biblia", "Wielkie nadzieje", "Rok 1984", "Zbrodnia i kara",
                        "Folwark zwierzecy", "Kod Da Vinci", "Wladca much", "W drodze", "Moby Dick", "Germinal",
                        "Opetanie", "Okruchy dnia", "Pani Bovary", "A Fine Balance"};
    char *kategoria[MAX] = {"fantasy", "horror", "SF", "literatura faktu", "literatura piekna", "thriller",
                            "kryminal", "romans", "sensacja", "przygodowa", "historyczna", "popularnonaukowa"};
    char *data_wydania[MAX] = {"05-01-1937", "02-01-1923", "04-07-1918", "26-04-1907", "06-11-2003",
                               "01-01-1959", "20-10-2000", "20-02-1939", "15-01-2013", "05-04-1991", "25-03-2020",
                               "09-08-1856"};
    char *wydawnictwo[MAX] = {"Helion", "Znak", "Amber", "Impuls", "Mag", "Replika", "WSiP", "Nowa Era",
                              "W drodze", "SBM", "ISA", "Kos", "Bosz"};
    long long isbn[12] = {2005917850652, 7137334183392, 1460338048199, 1771121865414, 3827938534880, 7527675674564,
                          2728983381469, 4113757760531, 5191182644763, 5881868774856, 9613158509137, 5320278067692};
    ksiazka->dostepnosc = 1;
    strcpy(ksiazka->autor_nazwisko, nazwisko[rand() % 12]);
    strcpy(ksiazka->autor_imie, imie[rand() % 13]);
    strcpy(ksiazka->tytul, tytul[rand() % 15]);
    strcpy(ksiazka->kategoria, kategoria[rand() % 12]);
    strcpy(ksiazka->data_wydania, data_wydania[rand() % 12]);
    strcpy(ksiazka->wydawnictwo, wydawnictwo[rand() % 13]);
    ksiazka->nr_isbn = isbn[rand() % 12];
    ksiazka->next = NULL;
    int x;
    FILE *f2 = fopen("../pliki/dane.txt", "r");
    fscanf(f2, "nr_katalogowy:%i", &x);
    ksiazka->nr_katalogowy = x + 1;
    fclose(f2);
    return ksiazka;
}