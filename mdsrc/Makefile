CC = g++
CFLAGS= -Wall -O2


run: a.out
	./a.out > energy


a.out : main.cc *h
	$(CC) $(CFLAGS) main.cc mdsys.cc

#%.o : %.cc
	#$(CC) $(CFLAGS) -c  $<




frames:
	sh genframe.sh out?????

clean:
	rm -f *.jpg *.ps 
cleanall:
	rm -f *.jpg *.ps a.out out0*

animate: frames
	animate -delay 15 *jpg

bak:
	mv -f out0* log* energy trash

movie:
	mencoder mf://*.jpg -mf fps=15:type=jpg -ovc lavc -ffourcc XVID -nosound -o out.avi
