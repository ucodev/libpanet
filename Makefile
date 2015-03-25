all:
	cd src && make && cd ..
	cd example && make && cd ..

install_all:
	mkdir -p ${SYSLIBDIR}
	mkdir -p ${SYSINCLUDEDIR}/panet
	cp src/libpanet.* ${SYSLIBDIR}/
	cp include/*.h ${SYSINCLUDEDIR}/

clean:
	cd src && make clean && cd ..
	cd example && make clean && cd ..

