VERSION=0.4
CXXFLAGS=-Wall -fpermissive -Wint-to-pointer-cast

all:
	cd demo; make


VERSION: Makefile
	echo $(VERSION) > VERSION

codedocs: codedocs/index.html 

codedocs/index.html: mtasker.cc mtasker.hh Doxyfile
	doxygen

clean:
	rm -rf *.o *~ $(BINARIES) mtasker-?.?.tar.gz codedocs
	cd demo; make clean

dist:   codedocs VERSION
	rm -rf mtasker-$(VERSION)
	mkdir mtasker-$(VERSION)
	cp -a VERSION ChangeLog mtasker.cc mtasker.hh  \
	 Makefile COPYING README codedocs/ \
	 mtasker-$(VERSION)
	mkdir mtasker-$(VERSION)/demo
	cp demo/{Makefile,*.cc,*.hh,README} \
	 mtasker-$(VERSION)/demo

	tar cvzf mtasker-$(VERSION).tar.gz mtasker-$(VERSION)
	rm -rf mtasker-$(VERSION)
