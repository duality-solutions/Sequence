DIST_SUBDIRS = secp256k1 univalue

AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS)
AM_CXXFLAGS = $(HARDENED_CXXFLAGS)
AM_CPPFLAGS = $(HARDENED_CPPFLAGS)
EXTRA_LIBRARIES =

SEQUENCE_CONFIG_INCLUDES=-I$(builddir)/config
SEQUENCE_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BDB_CPPFLAGS) $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

SEQUENCE_INCLUDES += -I$(srcdir)/secp256k1/include
SEQUENCE_INCLUDES += $(UNIVALUE_CFLAGS)

LIBSEQUENCE_SERVER=libsequence_server.a
LIBSEQUENCE_WALLET=libsequence_wallet.a
LIBSEQUENCE_COMMON=libsequence_common.a
LIBSEQUENCE_CLI=libsequence_cli.a
LIBSEQUENCE_UTIL=libsequence_util.a
LIBSEQUENCE_CRYPTO=crypto/libsequence_crypto.a
LIBSEQUENCE_UNIVALUE=univalue/libsequence_univalue.a
LIBSEQUENCEQT=qt/libsequenceqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la
LIBUNIVALUE=univalue/libunivalue.la

$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

$(LIBUNIVALUE): $(wildcard univalue/lib/*) $(wildcard univalue/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES = \
	crypto/libsequence_crypto.a \
	libsequence_util.a \
	libsequence_common.a \
	libsequence_server.a \
	libsequence_cli.a
if ENABLE_WALLET
EXTRA_LIBRARIES += libsequence_wallet.a
endif

if BUILD_SEQUENCE_LIBS
lib_LTLIBRARIES = libsequenceconsensus.la
LIBSEQUENCE_CONSENSUS=libsequenceconsensus.la
else
LIBSEQUENCE_CONSENSUS=
endif

bin_PROGRAMS =
TESTS =

if BUILD_SEQUENCED
  bin_PROGRAMS += sequenced
endif

if BUILD_SEQUENCE_UTILS
  bin_PROGRAMS += sequence-cli sequence-tx
endif

.PHONY: FORCE
# Sequence #
SEQUENCE_CORE_H = \
	addrman.h \
	alert.h \
	amount.h \
	base58.h \
	bip39_english.h \
	bip39.h \
	bignum.h \
	bloom.h \
	chain.h \
	chainparams.h \
	chainparamsbase.h \
	chainparamsseeds.h \
	checkpoints.h \
	checkqueue.h \
	clientversion.h \
	coins.h \
	compat.h \
	compat/byteswap.h \
	compat/endian.h \
	compat/sanity.h \
	compressor.h \
	consensus/consensus.h \
	consensus/merkle.h \
	consensus/params.h \
	consensus/validation.h \
	core_io.h \
	dns/dns.h \
	dns/seqdns.h \
	eccryptoverify.h \
	ecwrapper.h \
	hash.h \
	hdchain.h \
	hooks.h \
	indirectmap.h \
	init.h \
	key.h \
	keystore.h \
	leveldbwrapper.h \
	limitedmap.h \
	main.h \
	memusage.h \
	merkleblock.h \
	miner.h \
	mruset.h \
	netbase.h \
	net.h \
	noui.h \
	ntp.h \
	primitives/block.h \
	primitives/transaction.h \
	protocol.h \
	pubkey.h \
	random.h \
	reverse_iterator.h \
	reverselock.h \
	rpc/rpcclient.h \
	rpc/rpcprotocol.h \
	rpc/rpcregister.h \
	rpc/rpcserver.h \
	scheduler.h \
	script/interpreter.h \
	script/script.h \
	script/sigcache.h \
	script/sign.h \
	script/standard.h \
	script/script_error.h \
	serialize.h \
	stake.h \
	streams.h \
	support/allocators/secure.h \
	support/allocators/zeroafterfree.h \
	support/cleanse.h \
	support/lockedpool.h \
	sync.h \
	threadsafety.h \
	threadinterrupt.h \
	timedata.h \
	tinyformat.h \
	torcontrol.h \
	txdb.h \
	txmempool.h \
	ui_interface.h \
	uint256.h \
	uint256hm.h \
	undo.h \
	util.h \
	utilstrencodings.h \
	utilmoneystr.h \
	utiltime.h \
	validationinterface.h \
	version.h \
	wallet/coincontrol.h \
	wallet/crypter.h \
	wallet/db.h \
	wallet/wallet.h \
	wallet/wallet_ismine.h \
	wallet/walletdb.h \
	work.h

obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
		$(abs_top_srcdir)
libsequence_util_a-clientversion.$(OBJEXT): obj/build.h

# server: shared between sequenced and sequence-qt
libsequence_server_a_CPPFLAGS = $(AM_CPPFLAGS) $(SEQUENCE_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(EVENT_CFLAGS) $(EVENT_PTHREADS_CFLAGS)
libsequence_server_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libsequence_server_a_SOURCES = \
	addrman.cpp \
	alert.cpp \
	bloom.cpp \
	chain.cpp \
	checkpoints.cpp \
	dns/dns.cpp \
	dns/seqdns.cpp \
	init.cpp \
	leveldbwrapper.cpp \
	main.cpp \
	merkleblock.cpp \
	miner.cpp \
	net.cpp \
	noui.cpp \
	ntp.cpp \
	rest.cpp \
	rpc/rpcblockchain.cpp \
	rpc/rpcmining.cpp \
	rpc/rpcmisc.cpp \
	rpc/rpcnet.cpp \
	rpc/rpcrawtransaction.cpp \
	rpc/rpcserver.cpp \
	script/sigcache.cpp \
	stake.cpp \
	torcontrol.cpp \
	txdb.cpp \
	txmempool.cpp \
	validationinterface.cpp \
	work.cpp \
	$(SEQUENCE_CORE_H)

# wallet: shared between sequenced and sequence-qt, but only linked
# when wallet enabled
libsequence_wallet_a_CPPFLAGS = $(AM_CPPFLAGS) $(SEQUENCE_INCLUDES)
libsequence_wallet_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libsequence_wallet_a_SOURCES = \
	dns/dns.cpp \
	wallet/db.cpp \
	wallet/crypter.cpp \
	wallet/rpcdump.cpp \
	wallet/rpcwallet.cpp rpc/rpccontrol.cpp \
	wallet/wallet.cpp \
	wallet/wallet_ismine.cpp \
	wallet/walletdb.cpp \
	$(SEQUENCE_CORE_H)

# crypto primitives library
crypto_libsequence_crypto_a_CPPFLAGS = $(AM_CPPFLAGS) $(SEQUENCE_CONFIG_INCLUDES)
crypto_libsequence_crypto_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libsequence_crypto_a_SOURCES = \
	crypto/aes.cpp \
	crypto/hmac_sha256.cpp \
	crypto/hmac_sha512.cpp \
	crypto/ripemd160.cpp \
	crypto/rfc6979_hmac_sha256.cpp \
	crypto/sha1.cpp \
	crypto/sha256.cpp \
	crypto/sha512.cpp \
	crypto/aes.h \
	crypto/common.h \
	crypto/hmac_sha256.h \
	crypto/hmac_sha512.h \
	crypto/ripemd160.h \
	crypto/rfc6979_hmac_sha256.h \
	crypto/sha1.h \
	crypto/sha256.h \
	crypto/sha512.h

# common: shared between sequenced, and sequence-qt and non-server tools
libsequence_common_a_CPPFLAGS = $(AM_CPPFLAGS) $(SEQUENCE_INCLUDES)
libsequence_consensus_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libsequence_common_a_SOURCES = \
	amount.cpp \
	base58.cpp \
	bip39.cpp \
	chainparams.cpp \
	coins.cpp \
	compressor.cpp \
	consensus/merkle.cpp \
	core_read.cpp \
	core_write.cpp \
	eccryptoverify.cpp \
	ecwrapper.cpp \
	hash.cpp \
	hdchain.cpp \
	key.cpp \
	keystore.cpp \
	netbase.cpp \
	primitives/block.cpp \
	primitives/transaction.cpp \
	protocol.cpp \
	pubkey.cpp \
	scheduler.cpp \
	script/interpreter.cpp \
	script/script.cpp \
	script/sign.cpp \
	script/standard.cpp \
	script/script_error.cpp \
	$(SEQUENCE_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libsequence_util_a_CPPFLAGS = $(AM_CPPFLAGS) $(SEQUENCE_INCLUDES)
libsequence_util_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libsequence_util_a_SOURCES = \
	chainparamsbase.cpp \
	clientversion.cpp \
	compat/strnlen.cpp \
	compat/glibc_sanity.cpp \
	compat/glibcxx_sanity.cpp \
	random.cpp \
	rpc/rpcprotocol.cpp \
	support/cleanse.cpp \
	support/lockedpool.cpp \
	sync.cpp \
	threadinterrupt.cpp \
	timedata.cpp \
	uint256.cpp \
	util.cpp \
	utilstrencodings.cpp \
	utilmoneystr.cpp \
	utiltime.cpp \
	$(SEQUENCE_CORE_H)

if GLIBC_BACK_COMPAT
libsequence_util_a_SOURCES += compat/glibc_compat.cpp
endif

# cli: shared between sequence-cli and sequence-qt
libsequence_cli_a_CPPFLAGS = $(AM_CPPFLAGS) $(SEQUENCE_INCLUDES)
libsequence_cli_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libsequence_cli_a_SOURCES = \
	rpc/rpcclient.cpp \
	$(SEQUENCE_CORE_H)

nodist_libsequence_util_a_SOURCES = $(srcdir)/obj/build.h
#

# sequenced binary #
sequenced_SOURCES = sequenced.cpp
sequenced_CPPFLAGS = $(AM_CPPFLAGS) $(SEQUENCE_INCLUDES)
sequenced_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
sequenced_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
sequenced_SOURCES += sequenced-res.rc
endif

sequenced_LDADD = \
	$(LIBSEQUENCE_SERVER) \
	$(LIBSEQUENCE_COMMON) \
	$(LIBUNIVALUE) \
	$(LIBSEQUENCE_UTIL) \
	$(LIBSEQUENCE_CRYPTO) \
	$(LIBLEVELDB) \
	$(LIBMEMENV) \
	$(LIBSECP256K1)

if ENABLE_WALLET
sequenced_LDADD += libsequence_wallet.a
endif

sequenced_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS) $(EVENT_PTHREADS_LIBS) $(EVENT_LIBS)
#

# sequence-cli binary #
sequence_cli_SOURCES = sequence-cli.cpp
sequence_cli_CPPFLAGS = $(AM_CPPFLAGS) $(SEQUENCE_INCLUDES) $(EVENT_CFLAGS)
sequence_cli_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
sequence_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)


if TARGET_WINDOWS
sequence_cli_SOURCES += sequence-cli-res.rc
endif

sequence_cli_LDADD = \
	$(LIBSEQUENCE_CLI) \
	$(LIBSEQUENCE_UTIL) \
	$(LIBUNIVALUE) \
	$(EVENT_LIBS)

sequence_cli_LDADD += $(BOOST_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS)
#

# sequence-tx binary #
sequence_tx_SOURCES = sequence-tx.cpp
sequence_tx_CPPFLAGS = $(AM_CPPFLAGS) $(SEQUENCE_INCLUDES)
sequence_tx_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
sequence_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

sequence_tx_LDADD = \
	$(LIBUNIVALUE) \
	$(LIBSEQUENCE_COMMON) \
	$(LIBSEQUENCE_UTIL) \
	$(LIBSEQUENCE_CRYPTO) \
	$(LIBSECP256K1)

sequence_tx_LDADD += $(BOOST_LIBS) $(CRYPTO_LIBS)
#

# sequenceconsensus library #
if BUILD_SEQUENCE_LIBS
include_HEADERS = script/sequenceconsensus.h
libsequenceconsensus_la_SOURCES = \
	crypto/hmac_sha512.cpp \
	crypto/ripemd160.cpp \
	crypto/sha1.cpp \
	crypto/sha256.cpp \
	crypto/sha512.cpp \
	eccryptoverify.cpp \
	ecwrapper.cpp \
	hash.cpp \
	primitives/transaction.cpp \
	pubkey.cpp \
	script/script.cpp \
	script/interpreter.cpp \
	script/sequenceconsensus.cpp \
	script/sequenceconsensus_dummy.cpp \
	uint256.cpp \
	utilstrencodings.cpp

if GLIBC_BACK_COMPAT
  libsequenceconsensus_la_SOURCES += compat/glibc_compat.cpp
endif

libsequenceconsensus_la_LDFLAGS = -no-undefined $(RELDFLAGS)		
libsequenceconsensus_la_LIBADD = $(CRYPTO_LIBS)		
libsequenceconsensus_la_CPPFLAGS = $(CRYPTO_CFLAGS) -I$(builddir)/obj -DBUILD_SEQUENCE_INTERNAL		
if USE_LIBSECP256K1		
libsequenceconsensus_la_LIBADD += secp256k1/libsecp256k1.la		
endif		
endif

CLEANFILES = $(EXTRA_LIBRARIES)		

CLEANFILES = leveldb/libleveldb.a leveldb/libmemenv.a
CLEANFILES += *.gcda *.gcno
CLEANFILES += compat/*.gcda compat/*.gcno
CLEANFILES += consensus/*.gcda consensus/*.gcno
CLEANFILES += crypto/*.gcda crypto/*.gcno
CLEANFILES += policy/*.gcda policy/*.gcno
CLEANFILES += primitives/*.gcda primitives/*.gcno
CLEANFILES += script/*.gcda script/*.gcno
CLEANFILES += support/*.gcda support/*.gcno
CLEANFILES += univalue/*.gcda univalue/*.gcno
CLEANFILES += wallet/*.gcda wallet/*.gcno
CLEANFILES += wallet/test/*.gcda wallet/test/*.gcno

DISTCLEANFILES = obj/build.h

EXTRA_DIST = leveldb crypto/ctaes

clean-local:
	-$(MAKE) -C leveldb clean
	-$(MAKE) -C secp256k1 clean
	-$(MAKE) -C univalue clean
	-rm -rf .libs
	-rm -rf .deps
	-rm -f leveldb/*/*.gcda leveldb/*/*.gcno leveldb/helpers/memenv/*.gcda leveldb/helpers/memenv/*.gcno
	-rm -f config/sequence-config.* config/stamp-h1
	-rm -rf compat/.deps && rm -rf compat/.libs && rm -f compat/.dirstamp
	-rm -rf crypto/.deps && rm -rf crypto/.libs && rm -f crypto/.dirstamp
	-rm -rf primitives/.deps && rm -rf primitives/.libs && rm -f primitives/.dirstamp
	-rm -rf qt/.deps && rm -rf qt/.libs && rm -f qt/.dirstamp && rm -rf qt/test/.deps
	-rm -rf script/.deps && rm -rf script/.libs && rm -f script/.dirstamp
	-rm -rf univalue/.deps && rm -rf univalue/.libs && rm -f univalue/.dirstamp
	-rm -rf test/.deps && rm -rf test/.libs && rm -f test/.dirstamp && rm	-f test/buildenv.py
	-rm -rf secp256k1/.deps && rm -rf secp256k1/.libs && rm -f secp256k1/.dirstamp
	-rm -f Makefile && rm -f Makefile.in
	-rm -f config/sequence-config.h && rm -f config/sequence-config.* && rm -f config/stamp-h1
.rc.o:
	@test -f $(WINDRES)
	$(AM_V_GEN) $(WINDRES) -DWINDRES_PREPROC -i $< -o $@

.mm.o:
	$(AM_V_CXX) $(OBJCXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
		$(CPPFLAGS) $(AM_CXXFLAGS) $(QT_INCLUDES) $(CXXFLAGS)  -c -o $@ $<

%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(abspath $(<D) $<)
# TODO (Amir) put back unit tests
#if ENABLE_TESTS
#include Makefile.test.include
#endif

if EMBEDDED_LEVELDB
include Makefile.leveldb.include
endif

if ENABLE_QT
include Makefile.qt.include
endif

#if ENABLE_QT_TESTS
#include Makefile.qttest.include
#endif
