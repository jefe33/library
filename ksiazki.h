#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 50
struct ksiazki {
    unsigned int nr_isbn[13];
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
    char *s = malloc(MAX * sizeof(char)), c;
    int i = 0;
    while ((c = getchar()) != '\n' || i + 1 == MAX) {
        s[i++] = c;
    }
    s[i] = '\0';
    s = realloc(s, i * sizeof(char));
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

int is_positive_number(char *s, int len) {
    for (int i = 0; i < len; i++) {
        if (!isdigit(s[i])) {
            return 0;
        }
        if (s[i] < 0) {
            return 0;
        }
    }
    return 1;
}

struct ksiazki *utworzKsiazke() {
    char z[13];
    int len, y, i;
    clear();
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
        scanf("%s", z);
        len = strlen(z);
    } while (len != 13 || !is_positive_number(z, len));
    for (i = 0; i < 13; ++i) {
        ksiazka->nr_isbn[i] = z[i] - '0';
    }
    FILE *f2 = fopen("../pliki/dane.txt", "r");
    fscanf(f2, "nr_katalogowy:%i", &y);
    ksiazka->nr_katalogowy = y + 1;
    fclose(f2);
    return ksiazka;
}

void wyswietlWedlugAutora() {
    FILE *f = fopen("../pliki/ksiazki.bin", "rb");
    if (f != NULL) {
        printf("-----------------------------------------------------------------------------------------------------------------------\n");
        printf("|nr |         autor       |          tytul           |    kategoria     | wydawnictwo |data_wydania|nr_ISBN|dostepnosc|\n");
        printf("-----------------------------------------------------------------------------------------------------------------------\n");
        struct ksiazki *ks = ReadBooks(f);
        struct ksiazki *tmp = ks;
        while (tmp) {
            printf("|%-3i|", tmp->nr_katalogowy);
            printf(" %-9s ", tmp->autor_nazwisko);
            printf("%-9s | ", tmp->autor_imie);
            printf("%-24s | ", tmp->tytul);
            printf("%-17s |", tmp->kategoria);
            printf(" %-11s |", tmp->wydawnictwo);
            printf("%-10s| ", tmp->data_wydania);
            for (int i = 0; i < 13; ++i) {
                printf("%i", tmp->nr_isbn[i]);
            }
            printf(" | %i |\n", tmp->dostepnosc);
            tmp = tmp->next;
        }
        uwolnicBarabasza(ks);
    }
    fclose(f);
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

void pustyPlik(FILE *f1, char *s, char *s2, FILE *f2, struct ksiazki *k) {
    fclose(f1);
    f1 = fopen(s, "wb");
    if (fwrite(k, sizeof(struct ksiazki), 1, f1) == 1) {
        fclose(f2);
        f2 = fopen(s2, "w");
        fprintf(f2, "nr_katalogowy:%i", 1);
    }
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
                /* printf("%i %i %lld %s %s %s %s %s %s\n", tmp->nr_katalogowy, tmp->dostepnosc, tmp->nr_isbn,
                        tmp->autor_nazwisko, tmp->autor_imie, tmp->tytul, tmp->kategoria, tmp->wydawnictwo,
                        tmp->data_wydania);*/
                tmp = tmp->next;
            }
            int x;
            fscanf(f2, "nr_katalogowy:%i", &x);
            fclose(f2);
            f2 = fopen(dane, "w");
            fprintf(f2, "nr_katalogowy:%i", x + 1);
            uwolnicBarabasza(k2);
        } else {
            pustyPlik(f, Pksiazki, dane, f2, k1);
        }
    } else {
        pustyPlik(f, Pksiazki, dane, f2, k1);
    }
    free(k1);
    fclose(f);
    fclose(f2);
}

struct ksiazki *losowaKsiazka() {
    srand(time(0));
    struct ksiazki *ksiazka = malloc(sizeof(struct ksiazki));
    char imie[][MAX] = {"Jane", "Emily", "George", "Philip", "Charles", "Thomas", "Jan", "Pawel", "Joseph",
                        "Margaret", "Dan", "Leo", "Arthur"};
    char nazwisko[][MAX] = {"Tolkien", "Rowling", "Lee", "Dickens", "Adams", "Carroll", "Grahame", "Orwell",
                            "Brown", "Golding", "Huxley", "Doyle"};
    char tytul[][MAX] = {"Wladca Pierscieni", "Biblia", "Wielkie nadzieje", "Rok 1984", "Zbrodnia i kara",
                         "Folwark zwierzecy", "Kod Da Vinci", "Wladca much", "W drodze", "Moby Dick", "Germinal",
                         "Opetanie", "Okruchy dnia", "Pani Bovary", "A Fine Balance"};
    char kategoria[][MAX] = {"fantasy", "horror", "SF", "literatura faktu", "literatura piekna", "thriller",
                             "kryminal", "romans", "sensacja", "przygodowa", "historyczna", "popularnonaukowa"};
    char data_wydania[][MAX] = {"05-01-1937", "02-01-1923", "04-07-1918", "26-04-1907", "06-11-2003",
                                "01-01-1959", "20-10-2000", "20-02-1939", "15-01-2013", "05-04-1991", "25-03-2020",
                                "09-08-1856"};
    char wydawnictwo[][MAX] = {"Helion", "Znak", "Amber", "Impuls", "Mag", "Replika", "WSiP", "Nowa Era",
                               "W drodze", "SBM", "ISA", "Kos", "Bosz"};
    ksiazka->dostepnosc = 1;
    strcpy(ksiazka->autor_nazwisko, nazwisko[rand() % 12]);
    strcpy(ksiazka->autor_imie, imie[rand() % 13]);
    strcpy(ksiazka->tytul, tytul[rand() % 15]);
    strcpy(ksiazka->kategoria, kategoria[rand() % 12]);
    strcpy(ksiazka->data_wydania, data_wydania[rand() % 12]);
    strcpy(ksiazka->wydawnictwo, wydawnictwo[rand() % 13]);
    int i, x;
    for (i = 0; i < 13; i++) {
        ksiazka->nr_isbn[i] = rand() % 10;
    }
    ksiazka->next = NULL;
    FILE *f2 = fopen("../pliki/dane.txt", "r");
    if (f2) {
        fseek(f2, 0, SEEK_END);
        if (ftell(f2) != 0) {
            rewind(f2);
            fscanf(f2, "nr_katalogowy:%i", &x);
            ksiazka->nr_katalogowy = x + 1;
        } else ksiazka->nr_katalogowy = 1;
    } else ksiazka->nr_katalogowy = 1;
    fclose(f2);
    return ksiazka;
}