

run: a.out
	./a.out > energy

a.out: main.cc *.h
	g++ -O2 main.cc


frames:
	sh genframe.sh out*
clean:
	rm -f *.jpg *.ps out0*
