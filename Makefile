edit: edit.c
	gcc -O3 $< -o $@ -lncurses
run: edit
	./$< sampe.txt
clean:
	rm edit
edit.db: edit.c
	gcc -g $< -o $@ -lncurses
