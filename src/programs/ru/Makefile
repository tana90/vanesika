##
##  Visopsys
##  Copyright (C) 1998-2020 J. Andrew McLaughlin
##
##  Makefile
##

ROOT = ../../..
BUILDDIR = ${ROOT}/build/system/locale/ru
UTILSDIR = ${ROOT}/utils

all:
	mkdir -p ${BUILDDIR}
	${UTILSDIR}/makemsgs.sh . ${BUILDDIR}

clean:
	rm -f *~ core
	${shell if [ -d ${BUILDDIR} ] ; then for FILE in *.po ; \
		do find ${BUILDDIR} -name `basename $$FILE .po`.mo -exec rm {} \; ; \
			done ; fi }

