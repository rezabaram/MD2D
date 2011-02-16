
run: a.out
	./a.out > energy

a.out: main.cc *.h
	g++ -O2 main.cc

frames:
	sh genframe.sh out?????

clean:
	rm -f *.jpg *.ps 

movie:
	mencoder mf://*.jpg -mf fps=15:type=jpg -ovc lavc -ffourcc XVID -nosound -o out.avi
