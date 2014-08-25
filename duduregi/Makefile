all: duduregi

duduregi: Makefile.duduregi Process.cpp  clusterview.cpp  main.cpp
	make -f Makefile.duduregi

Makefile.duduregi: duduregi.pro
	qmake-qt4 -o $@ $<
	#qmake-qt5 -o $@ $<

clean:
	make -f Makefile.duduregi clean
	rm -f Makefile.duduregi
	rm -f *.o duduregi
