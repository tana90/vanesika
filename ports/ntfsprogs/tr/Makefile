##
##  Visopsys
##  Copyright (C) 1998-2020 J. Andrew McLaughlin
##
##  Makefile
##

ROOT		= ../../..
UTILSDIR	= ${ROOT}/utils
BUILDDIR	= ${ROOT}/build
MSGDIR		= ${BUILDDIR}/system/locale/tr

all:
	mkdir -p ${MSGDIR}
	${UTILSDIR}/makemsgs.sh . ${MSGDIR}

clean:
	rm -f *~ core
	${shell if [ -d ${MSGDIR} ] ; then for FILE in *.po ; \
		do find ${MSGDIR} -name `basename $$FILE .po`.mo -exec rm {} \; ; \
			done ; fi }

