all: sinfonifry.so

test: all
	${TNTNET} tntnet.conf

clean:
	rm *.o
	rm *.so

sinfonifry.so: sinfonifry.o images.o add_host.o get_data.o get_host_status.o jscript.o menu.o css.o host_list.o
	${CXX} -o $@ -shared $^  -lcxxtools -ltntnet  -ltntdb

.SUFFIXES: .ecpp .gif .jpg .css .js .cpp
ECPPC=/usr/local/bin/ecppc
TNTNET=/usr/local/bin/tntnet
CXXFLAGS+=-I/usr/local/include -fPIC -O2
LDFLAGS+=-L/usr/local/lib 

.ecpp.cpp:
	${ECPPC} ${ECPPFLAGS} ${ECPPFLAGS_CPP} -o $@ $<
.gif.cpp:
	${ECPPC} ${ECPPFLAGS} -m image/gif ${ECPPFLAGS_GIF} -b -o $@ $<
.jpg.cpp:
	${ECPPC} ${ECPPFLAGS} -m image/jpg ${ECPPFLAGS_JPG} -b -o $@ $<
.png.cpp:
	${ECPPC} ${ECPPFLAGS} -m image/png ${ECPPFLAGS_PNG} -b -o $@ $<
.ico.cpp:
	${ECPPC} ${ECPPFLAGS} -m image/x-icon ${ECPPFLAGS_ICO} -b -o $@ $<
.css.cpp:
	${ECPPC} ${ECPPFLAGS} -m text/css ${ECPPFLAGS_CSS} -b -o $@ $<
.js.cpp:
	${ECPPC} ${ECPPFLAGS} -m application/javascript ${ECPPFLAGS_JS} -b -o $@ $<
