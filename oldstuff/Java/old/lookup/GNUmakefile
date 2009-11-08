# Ben Cherry (bcherry@ucsc.edu) CMPS12B - Winter 05 - Wesley Mackey
# $Id$

JAVASRC    = lookup.java list.java syslib.java pair.java iterator.java file.java
SOURCES    = ${JAVASRC} GNUmakefile README
MAINCLASS  = lookup 
CLASSES    = lookup.class list.class syslib.class pair.class iterator.class file.class
JARCLASSES = ${CLASSES} list\$$listitor.class list\$$node.class file\$$failure.class iterator\$$failure.class
JARFILE    = lookup

SUBMIT	   = submit cmps012b-wm.w05 asg2
CID	   = /afs/cats.ucsc.edu/courses/cmps012b-wm/sparcbin/cid

all : ${JARFILE}

${JARFILE} : ${CLASSES}
	echo Main-class: ${MAINCLASS} >Manifest
	jar cvfm ${JARFILE} Manifest ${JARCLASSES}
	chmod +x ${JARFILE}

%.class : %.java
	javac $<

clean :
	- rm ${JARCLASSES} Manifest

spotless : clean
	- rm ${JARFILE}

ci : ${SOURCES}
	${CID} ${SOURCES}

submit : ${SOURCES}
	${SUBMIT} ${SOURCES}
