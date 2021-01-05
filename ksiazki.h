#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 50
struct ksiazki {
    unsigned int nr_isbn[13];
    int nr_katalogowy, dostepnosc, data_wydania;
    char autor_nazwisko[MAX], autor_imie[MAX], tytul[MAX], kategoria[MAX], wydawnictwo[MAX];
    struct ksiazki *next;
};

struct ksiazki *losowaKsiazka();

struct ksiazki *ReadBooks(FILE *in_stream, int *max) {
    *max = 0;
    struct ksiazki head;
    head.next = NULL; // code only uses the `next` field of head

    struct ksiazki *previous = &head;
    struct ksiazki x;

    // While another record was successfully read ...
    while (fread(&x, sizeof(struct ksiazki), 1, in_stream) == 1) {
        // Fill the next field
        if (x.nr_katalogowy > *max) *max = x.nr_katalogowy;
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

void usunRok() {
    int max, i = 1, rok;
    clear();
    printf("Podaj rok wydania ktory chcesz usunac\n");
    scanf("%i", &rok);
    FILE *f = fopen("../pliki/ksiazki.bin", "rb");
    if (f != NULL) {
        struct ksiazki *ks = ReadBooks(f, &max);
        struct ksiazki *tmp = ks;
        while (tmp && i) {
            if (tmp->data_wydania == rok) {
                ks = tmp->next;
            } else {
                i = 0;
            }
            tmp = tmp->next;
        }
        if (i == 1) {
            uwolnicBarabasza(ks);
            fclose(f);
            return;
        }
        struct ksiazki *prev = ks;
        tmp = ks;
        while (tmp) {
            if (tmp->data_wydania == rok) {
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
        uwolnicBarabasza(ks);
    }
    fclose(f);
}

void usunAutora() {
    char *s;
    clear();
    printf("Podaj nazwisko autora ktorego chcesz usunac\n");
    s = wczytajString();
    FILE *f = fopen("../pliki/ksiazki.bin", "rb");
    int max, i = 1;
    if (f != NULL) {
        struct ksiazki *ks = ReadBooks(f, &max);
        struct ksiazki *tmp = ks;
        while (tmp && i) {
            if (strcmp(tmp->autor_nazwisko, s) == 0) {
                ks = tmp->next;
            } else {
                i = 0;
            }
            tmp = tmp->next;
        }
        if (i == 1) {
            uwolnicBarabasza(ks);
            fclose(f);
            return;
        }
        struct ksiazki *prev = ks;
        tmp = ks;
        while (tmp) {
            if (strcmp(tmp->autor_nazwisko, s) == 0) {
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
        uwolnicBarabasza(ks);
    }
    fclose(f);
}

struct ksiazki *utworzKsiazke() {
    char z[13];
    int len, i, x;
    clear();
    struct ksiazki *ksiazka = malloc(sizeof(struct ksiazki));
    strcpy(ksiazka->tytul, wczytajKsiazka("Podaj tytul ksiazki"));
    strcpy(ksiazka->autor_nazwisko, wczytajKsiazka("Podaj nazwisko autora ksiazki"));
    strcpy(ksiazka->autor_imie, wczytajKsiazka("Podaj imie autora ksiazki"));
    strcpy(ksiazka->kategoria, wczytajKsiazka("Podaj kategorie ksiazki"));
    strcpy(ksiazka->wydawnictwo, wczytajKsiazka("Podaj wydawnictwo ksiazki"));
    do {
        printf("Podaj rok wydania\n");
        scanf("%i", &x);
    } while (x < 1000 || x > 9999);
    ksiazka->data_wydania = x;
    ksiazka->dostepnosc = 1;
    do {
        printf("Podaj 13-nasto cyfrowy numer ISBN\n");
        scanf("%s", z);
        len = strlen(z);
    } while (len != 13 || !is_positive_number(z, len));
    for (i = 0; i < 13; ++i) {
        ksiazka->nr_isbn[i] = z[i] - '0';
    }
    ksiazka->nr_katalogowy = 0;
    return ksiazka;
}

struct ksiazki *sortowaniePrzezWstawianie(struct ksiazki *l) {
    if (l == NULL || l->next == NULL) {
        return l;
    }
    struct ksiazki *sorted = NULL;
    struct ksiazki *tmp = l;
    struct ksiazki *tmp2;
    struct ksiazki *t = NULL;
    while (tmp) {
        t = tmp->next;
        if (!sorted || strcasecmp(tmp->tytul, sorted->tytul) <= 0) {
            tmp->next = sorted;
            sorted = tmp;
        } else {
            tmp2 = sorted;
            while (tmp2->next && strcasecmp(tmp2->next->tytul, tmp->tytul) < 0) {
                tmp2 = tmp2->next;
            }
            tmp->next = tmp2->next;
            tmp2->next = tmp;
        }
        tmp = t;
    }
    return sorted;
}

void wyswietlWedlugAutora() {
    FILE *f = fopen("../pliki/ksiazki.bin", "rb");
    int max, i;
    if (f != NULL) {
        struct ksiazki *ks = ReadBooks(f, &max);
        struct ksiazki *tmp = ks;
        if (tmp != NULL) {
            printf("------------------------------------------------------------------------------------------------------------------------");
            printf("| nr |         autor         |        tytul              |   kategoria       |wydawnictwo|rok_wyd|numer ISBN|dostepnosc|");
            printf("------------------------------------------------------------------------------------------------------------------------");
            while (tmp) {
                printf("| %-3i|", tmp->nr_katalogowy);
                printf(" %-10s ", tmp->autor_nazwisko);
                printf("%-10s | ", tmp->autor_imie);
                printf("%-25s | ", tmp->tytul);
                printf("%-17s |", tmp->kategoria);
                printf(" %-11s |", tmp->wydawnictwo);
                printf(" %-4i | ", tmp->data_wydania);
                for (i = 0; i < 13; ++i) {
                    printf("%i", tmp->nr_isbn[i]);
                }
                printf(" | %i |\n", tmp->dostepnosc);
                tmp = tmp->next;
            }
            uwolnicBarabasza(ks);
        } else printf("Brak ksiazek\n");
    } else printf("Brak ksiazek\n");
    fclose(f);
}

void wyswietlWedlugTytulu() {
    FILE *f = fopen("../pliki/ksiazki.bin", "rb");
    int max, i;
    if (f != NULL) {
        struct ksiazki *ks = ReadBooks(f, &max);
        ks = sortowaniePrzezWstawianie(ks);
        struct ksiazki *tmp = ks;
        if (tmp != NULL) {
            printf("------------------------------------------------------------------------------------------------------------------------");
            printf("| nr |         autor         |        tytul              |   kategoria       |wydawnictwo|rok_wyd|numer ISBN|dostepnosc|");
            printf("------------------------------------------------------------------------------------------------------------------------");
            while (tmp) {
                printf("| %-3i|", tmp->nr_katalogowy);
                printf(" %-10s ", tmp->autor_nazwisko);
                printf("%-10s | ", tmp->autor_imie);
                printf("%-25s | ", tmp->tytul);
                printf("%-17s |", tmp->kategoria);
                printf(" %-11s |", tmp->wydawnictwo);
                printf(" %-4i | ", tmp->data_wydania);
                for (i = 0; i < 13; ++i) {
                    printf("%i", tmp->nr_isbn[i]);
                }
                printf(" | %i |\n", tmp->dostepnosc);
                tmp = tmp->next;
            }
            uwolnicBarabasza(ks);
        } else printf("Brak ksiazek\n");
    } else printf("Brak ksiazek\n");
    fclose(f);
}

struct ksiazki *wstawPosortowane(struct ksiazki *l, struct ksiazki *ks) {
    if (strcasecmp(l->autor_nazwisko, ks->autor_nazwisko) > 0) {
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
            if (tmp->next && strcasecmp(tmp->next->autor_nazwisko, ks->autor_nazwisko) > 0) {
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
    struct ksiazki *k1;
    if (n) {
        k1 = losowaKsiazka();
    } else {
        k1 = utworzKsiazke();
    }
    FILE *f = fopen(Pksiazki, "rb");
    if (f) {
        fseek(f, 0, SEEK_END);
        if (ftell(f) != 0) {
            rewind(f);
            int max;
            struct ksiazki *k2 = ReadBooks(f, &max);
            k1->nr_katalogowy = max + 1;
            k2 = wstawPosortowane(k2, k1);
            fclose(f);
            f = fopen(Pksiazki, "wb");
            struct ksiazki *tmp = k2;
            while (tmp && fwrite(tmp, sizeof(struct ksiazki), 1, f) == 1) {
                tmp = tmp->next;
            }
            uwolnicBarabasza(k2);
        } else {
            fclose(f);
            f = fopen(Pksiazki, "wb");
            k1->nr_katalogowy = 1;
            fwrite(k1, sizeof(struct ksiazki), 1, f);
            free(k1);
        }
    } else {
        fclose(f);
        f = fopen(Pksiazki, "wb");
        k1->nr_katalogowy = 1;
        fwrite(k1, sizeof(struct ksiazki), 1, f);
        free(k1);
    }
    fclose(f);
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
    char wydawnictwo[][MAX] = {"Helion", "Znak", "Amber", "Impuls", "Mag", "Replika", "WSiP", "Nowa Era",
                               "W drodze", "SBM", "ISA", "Kos", "Bosz"};
    ksiazka->dostepnosc = 1;
    strcpy(ksiazka->autor_nazwisko, nazwisko[rand() % 12]);
    strcpy(ksiazka->autor_imie, imie[rand() % 13]);
    strcpy(ksiazka->tytul, tytul[rand() % 15]);
    strcpy(ksiazka->kategoria, kategoria[rand() % 12]);
    ksiazka->data_wydania = rand() % 521 + 1500;
    strcpy(ksiazka->wydawnictwo, wydawnictwo[rand() % 13]);
    int i;
    for (i = 0; i < 13; i++) {
        ksiazka->nr_isbn[i] = rand() % 10;
    }
    ksiazka->next = NULL;
    ksiazka->nr_katalogowy = 0;
    return ksiazka;
}