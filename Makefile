OBJ = index.c ksiazki.c klienci.c wypozyczenia.c

program: $(OBJ)
	gcc -o program $(OBJ)

.PHONY: clean
clean:
	rm program