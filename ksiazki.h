struct ksiazki {
    unsigned int nr_isbn[13];
    int nr_katalogowy, dostepnosc, data_wydania;
    char autor_nazwisko[MAX], autor_imie[MAX], tytul[MAX], kategoria[MAX], wydawnictwo[MAX];
    struct ksiazki *next;
};

int is_empty(char *s);

void uwolnicKsiazki(struct ksiazki *t);

char *wczytajString();

char *wczytajKsiazka(char *s);

int is_positive_number(char *s, int len);

void wyswietlK(struct ksiazki *k);

void wyswietlKsiazki(int n);

int menuEdycjiKsiazki();

void edytujKsiazke();

struct ksiazki *utworzKsiazke();

struct ksiazki *sortowaniePrzezWstawianie(struct ksiazki *l, int);

struct ksiazki *wstawPosortowane(struct ksiazki *l, struct ksiazki *ks);

struct ksiazki *losowaKsiazka();

void dodajKsiazke(int);

struct ksiazki *wczytajKsiazki(FILE *in_stream, int *max) {
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

void uwolnicKsiazki(struct ksiazki *t) {
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

void wyswietlK(struct ksiazki *k) {
    int i;
    printf("-----------------------------------------------------------------------------------------------------------------------\n");
    printf(" nr       autor                   tytul                       kategoria        wydawnictwo  rok_wyd  numer_ISBN  ilosc\n");
    printf("-----------------------------------------------------------------------------------------------------------------------\n");
    while (k) {
        printf(" %-3i ", k->nr_katalogowy);
        printf(" %-24s ", strcat(strcat(k->autor_nazwisko, " "), k->autor_imie));
        printf(" %-28s ", k->tytul);
        printf(" %-17s ", k->kategoria);
        printf(" %-11s ", k->wydawnictwo);
        printf(" %-4i  ", k->data_wydania);
        for (i = 0; i < 13; ++i) {
            printf("%i", k->nr_isbn[i]);
        }
        printf("  %i \n", k->dostepnosc);
        k = k->next;
    }
}

int menuEdycjiKsiazki() {
    printf("0.Wroc\n1.Zmien tytul ksiazki\n2.Zmien nazwisko autora ksiazki\n3.Zmien imie autora ksiazki\n"
           "4.Zmien kategorie ksiazki\n5.Zmien wydawnictwo ksiazki\n6.Zmien rok wydania ksiazki\n7.Zmien numer ISBN\n8.Zmien dostepnosc\n");
    int x;
    if (scanf("%i", &x) == 0) return 0;
    return x;
}

void edytujKsiazke() {
    int max, i = 1, nr, x;
    clear();
    printf("Podaj numer katalogowy ksiazki ktora chcesz edytowac\n");
    if (scanf("%i", &nr) == 0) return;
    FILE *f = fopen("../pliki/ksiazki.bin", "rb");
    if (f != NULL) {
        struct ksiazki *ks = wczytajKsiazki(f, &max);
        if (nr > max || nr < 1) {
            fclose(f);
            return;
        }
        struct ksiazki *tmp = ks;
        while (tmp) {
            if (tmp->nr_katalogowy == nr) {
                i = 0;
                break;
            }
            tmp = tmp->next;
        }
        if (i == 1) {
            uwolnicKsiazki(ks);
            fclose(f);
            return;
        }
        struct ksiazki *p = tmp->next;
        tmp->next = NULL;
        wyswietlK(tmp);
        tmp->next = p;
        char z[13];
        int len;
        x = menuEdycjiKsiazki();
        clear();
        switch (x) {
            case 0:
                break;
            case 1:
                strcpy(tmp->tytul, wczytajKsiazka("Podaj tytul ksiazki"));
                break;
            case 2:
                strcpy(tmp->autor_nazwisko, wczytajKsiazka("Podaj nazwisko autora ksiazki"));
                break;
            case 3:
                strcpy(tmp->autor_imie, wczytajKsiazka("Podaj imie autora ksiazki"));
                break;
            case 4:
                strcpy(tmp->kategoria, wczytajKsiazka("Podaj kategorie ksiazki"));
                break;
            case 5:
                strcpy(tmp->wydawnictwo, wczytajKsiazka("Podaj wydawnictwo ksiazki"));
                break;
            case 6:
                do {
                    printf("Podaj rok wydania\n");
                    if (scanf("%i", &i) == 0) i = 0;
                } while (i < 1000 || i > 9999);
                tmp->data_wydania = i;
                break;
            case 7:
                do {
                    printf("Podaj 13-nasto cyfrowy numer ISBN\n");
                    scanf("%s", z);
                    len = strlen(z);
                } while (len != 13 || !is_positive_number(z, len));
                for (i = 0; i < 13; ++i) {
                    tmp->nr_isbn[i] = z[i] - '0';
                }
                break;
            case 8:
                do {
                    printf("Podaj numer dostepnych\n");
                    if (scanf("%i", &i) == 0) i = 0;
                } while (i < 0);
                tmp->dostepnosc = i;
                break;
            default:
                break;
        }
        tmp = ks;
        fclose(f);
        f = fopen("../pliki/ksiazki.bin", "wb");
        while (tmp && fwrite(tmp, sizeof(struct ksiazki), 1, f) == 1) {
            tmp = tmp->next;
        }
        uwolnicKsiazki(ks);
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
        if (scanf("%i", &x) == 0) x = 0;
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

struct ksiazki *sortowaniePrzezWstawianie(struct ksiazki *l, int n) {
    if (l == NULL || l->next == NULL) {
        return l;
    }
    struct ksiazki *sorted = NULL;
    struct ksiazki *tmp = l;
    struct ksiazki *tmp2;
    struct ksiazki *t = NULL;
    if (n == 0) {
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
    } else if (n == 1) {
        while (tmp) {
            t = tmp->next;
            if (!sorted || tmp->dostepnosc > sorted->dostepnosc) {
                tmp->next = sorted;
                sorted = tmp;
            } else {
                tmp2 = sorted;
                while (tmp2->next && tmp2->next->dostepnosc > tmp->dostepnosc) {
                    tmp2 = tmp2->next;
                }
                tmp->next = tmp2->next;
                tmp2->next = tmp;
            }
            tmp = t;
        }
    }
    return sorted;
}

void wyswietlKsiazki(int n) {
    FILE *f = fopen("../pliki/ksiazki.bin", "rb");
    int max;
    if (f != NULL) {
        struct ksiazki *ks = wczytajKsiazki(f, &max);
        if (n != 2) {
            ks = sortowaniePrzezWstawianie(ks, n);
        }
        struct ksiazki *tmp = ks;
        if (tmp != NULL) {
            wyswietlK(tmp);
            uwolnicKsiazki(ks);
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
            struct ksiazki *k2 = wczytajKsiazki(f, &max);
            k1->nr_katalogowy = max + 1;
            k2 = wstawPosortowane(k2, k1);
            fclose(f);
            f = fopen(Pksiazki, "wb");
            struct ksiazki *tmp = k2;
            while (tmp && fwrite(tmp, sizeof(struct ksiazki), 1, f) == 1) {
                tmp = tmp->next;
            }
            uwolnicKsiazki(k2);
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