PREFIX=/usr/local/ssdb

$(shell sh build.sh 1>&2)
include build_config.mk

all:
	mkdir -p var var_slave
	cd "${SNAPPY_PATH}"; ./configure --with-pic --enable-static; make -j 8
	chmod u+x "${ROCKSDB_PATH}/build_tools/build_detect_platform"
	chmod u+x deps/cpy/cpy
	#chmod u+x tools/ssdb-cli
	mkdir -p "${ROCKSDB_PATH}/build"
	cd "${ROCKSDB_PATH}/build"; cmake -DWITH_SNAPPY=ON -DSNAPPY_INCLUDE_DIR="${SNAPPY_PATH}" -DSNAPPY_LIBRARIES="${SNAPPY_PATH}/.libs/libsnappy.a" ..; make -j 8
	cd src/util; ${MAKE}
	cd src/net; ${MAKE}
	cd src/client; ${MAKE}
	cd src/ssdb; ${MAKE}
	cd src; ${MAKE}
	#cd tools; ${MAKE}

.PHONY: ios

ios:
	cd "${ROCKSDB_PATH}"; make clean; CXXFLAGS=-stdlib=libc++ ${MAKE} static_lib PLATFORM=IOS
	cd "${SNAPPY_PATH}"; make clean; make -f Makefile-ios
	mkdir -p ios
	mv ${ROCKSDB_PATH}/libleveldb-ios.a ${SNAPPY_PATH}/libsnappy-ios.a ios/
	cd src/util; make clean; ${MAKE} -f Makefile-ios
	cd src/ssdb; make clean; ${MAKE} -f Makefile-ios

install:
	mkdir -p ${PREFIX}
	mkdir -p ${PREFIX}/_cpy_
	mkdir -p ${PREFIX}/deps
	mkdir -p ${PREFIX}/var
	mkdir -p ${PREFIX}/var_slave
	cp -f ssdb-server ssdb.conf ssdb_slave.conf ${PREFIX}
	cp -rf api ${PREFIX}
	cp -rf \
		tools/ssdb-bench \
		tools/ssdb-cli tools/ssdb_cli \
		tools/ssdb-cli.cpy tools/ssdb-dump \
		tools/ssdb-repair \
		tools/ssdb-ins.sh \
		${PREFIX}
	cp -rf deps/cpy ${PREFIX}/deps
	chmod 755 ${PREFIX}
	chmod -R ugo+rw ${PREFIX}/*
	rm -f ${PREFIX}/Makefile

clean:
	rm -f *.exe.stackdump
	rm -rf api/cpy/_cpy_
	rm -f api/python/SSDB.pyc
	rm -rf db_test
	cd deps/cpy; ${MAKE} clean
	cd src/util; ${MAKE} clean
	cd src/ssdb; ${MAKE} clean
	cd src/net; ${MAKE} clean
	cd src/client; ${MAKE} clean
	cd src; ${MAKE} clean
	#cd tools; ${MAKE} clean

clean_all: clean
	cd "${SNAPPY_PATH}"; ${MAKE} clean
	rm -rf "${ROCKSDB_PATH}/build"

