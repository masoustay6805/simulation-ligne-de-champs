FLAGS= --pdf-engine=xelatex

pdf: Rapport_RAJOHNSON_David.md
	pandoc Rapport_RAJOHNSON_David.md -o Rapport_RAJOHNSON_David.pdf $(FLAGS)

html: Rapport_RAJOHNSON_David.md
	pandoc -s Rapport_RAJOHNSON_David.md -o Rapport_RAJOHNSON_David.html --metadata pagetitle="Lignes de champs éléctriques"

clean:
	rm -f Rapport_RAJOHNSON_David.pdf

rebuild: clean pdf html