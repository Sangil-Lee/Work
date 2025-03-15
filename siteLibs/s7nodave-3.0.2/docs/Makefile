SRCS = manual.xml
DESTDIR = generated
HTML_DESTDIR = ${DESTDIR}/html
XSLTPROC_OPTIONS = --nonet
IMAGES_SRC = /usr/share/xml/docbook/stylesheet/docbook-xsl/images

all: html

html: $(patsubst %.xml,${HTML_DESTDIR}/%.html,${SRCS}) copy-images copy-css

${HTML_DESTDIR}/%.html: %.xml ${HTML_DESTDIR}
	xsltproc --xinclude ${XSLTPROC_OPTIONS} -o $@ \
	  xsl/html.xsl $<

copy-images: $(patsubst %,${HTML_DESTDIR}/images/%,$(notdir $(wildcard ${IMAGES_SRC}/*.png)))

${HTML_DESTDIR}/images/%.png: ${IMAGES_SRC}/%.png ${HTML_DESTDIR}/images
	cp $< $@

copy-css: $(patsubst %,${HTML_DESTDIR}/%,$(wildcard css/*.css))

${HTML_DESTDIR}/css/%.css: css/%.css ${HTML_DESTDIR}/css
	cp $< $@

${HTML_DESTDIR}:
	mkdir -p ${HTML_DESTDIR}

${HTML_DESTDIR}/images:
	mkdir -p ${HTML_DESTDIR}/images

${HTML_DESTDIR}/css:
	mkdir -p ${HTML_DESTDIR}/css

.PHONY: clean
clean:
	rm -f ${HTML_DESTDIR}/css/*.css
	if [ -d ${HTML_DESTDIR}/css ]; then rmdir ${HTML_DESTDIR}/css; fi
	rm -f ${HTML_DESTDIR}/images/*.png
	if [ -d ${HTML_DESTDIR}/images ]; then rmdir ${HTML_DESTDIR}/images; fi
	rm -f ${HTML_DESTDIR}/*.html
	if [ -d ${HTML_DESTDIR} ]; then rmdir ${HTML_DESTDIR}; fi
	if [ -d ${DESTDIR} ]; then rmdir ${DESTDIR}; fi
