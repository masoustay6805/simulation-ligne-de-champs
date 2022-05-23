FLAGS= --pdf-engine=xelatex

pdf: Rapport_RAJOHNSON_David.md
	pandoc Rapport_RAJOHNSON_David.md -o Rapport_RAJOHNSON_David.pdf $(FLAGS)

clean:
	rm -f Rapport_RAJOHNSON_David.pdf

rebuild: clean pdf