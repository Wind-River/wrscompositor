all: duduregi

duduregi: Makefile.duduregi Process.cpp  clusterview.cpp  main.cpp vna_dbusclient.cpp wr_dbusclient.cpp
	make -f Makefile.duduregi

Makefile.duduregi: duduregi.pro
	qmake -o $@ $<
	#qmake-qt4 -o $@ $<

clean:
	make -f Makefile.duduregi clean
	rm -f Makefile.duduregi
	rm -f *.o duduregi
