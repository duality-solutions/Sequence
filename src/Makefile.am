DIST_SUBDIRS = secp256k1 univalue

AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS)
AM_CXXFLAGS = $(HARDENED_CXXFLAGS)
AM_CPPFLAGS = $(HARDENED_CPPFLAGS)
EXTRA_LIBRARIES =

SILK_CONFIG_INCLUDES=-I$(builddir)/config
SILK_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

SILK_INCLUDES += -I$(srcdir)/secp256k1/include
SILK_INCLUDES += -I$(srcdir)/univalue/include

LIBSILK_SERVER=libsilk_server.a
LIBSILK_WALLET=libsilk_wallet.a
LIBSILK_COMMON=libsilk_common.a
LIBSILK_CLI=libsilk_cli.a
LIBSILK_UTIL=libsilk_util.a
LIBSILK_CRYPTO=crypto/libsilk_crypto.a
LIBSILKQT=qt/libsilkqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la
LIBUNIVALUE=univalue/libunivalue.la

$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

$(LIBUNIVALUE): $(wildcard univalue/lib/*) $(wildcard univalue/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES += \
  crypto/libsilk_crypto.a \
  libsilk_util.a \
  libsilk_common.a \
  libsilk_server.a \
  libsilk_cli.a
if ENABLE_WALLET
SILK_INCLUDES += $(BDB_CPPFLAGS)
EXTRA_LIBRARIES += libsilk_wallet.a
endif

if BUILD_SILK_LIBS
lib_LTLIBRARIES = libsilkconsensus.la
LIBSILK_CONSENSUS=libsilkconsensus.la
else
LIBSILK_CONSENSUS=
endif

bin_PROGRAMS =
TESTS =
BENCHMARKS =

if BUILD_SILKD
  bin_PROGRAMS += silkd
endif

if BUILD_SILK_UTILS
  bin_PROGRAMS += silk-cli silk-tx
endif

.PHONY: FORCE check-symbols check-security
# silk core #
SILK_CORE_H = \
  addrman.h \
  addressindex.h \
  alert.h \
  amount.h \
  base58.h \
  bignum.h \
  bloom.h \
  chain.h \
  chainparams.h \
  chainparamsbase.h \
  chainparamsseeds.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  wallet/coincontrol.h \
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
  primitives/block.h \
  primitives/transaction.h \
  core_io.h \
  core_memusage.h \
  wallet/crypter.h \
  wallet/db.h \
  eccryptoverify.h \
  hash.h \
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
	policy/fees.h \
  policy/policy.h \
  policy/rbf.h \
  pow.h \
	prevector.h \
  protocol.h \
  pubkey.h \
  random.h \
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
  spentindex.h \
  streams.h \
  support/allocators/secure.h \
  support/allocators/zeroafterfree.h \
  support/cleanse.h \
  support/pagelocker.h \
  sync.h \
  threadsafety.h \
  timedata.h \
  tinyformat.h \
  txdb.h \
  txmempool.h \
  ui_interface.h \
  uint256.h \
  undo.h \
  util.h \
  utilstrencodings.h \
  utilmoneystr.h \
  utiltime.h \
  validationinterface.h \
  version.h \
  wallet/wallet.h \
  wallet/wallet_ismine.h \
  wallet/walletdb.h \
  compat/sanity.h \
  reverselock.h \
 	uint256hm.h \
  kernel.h \
  dns/dns.h \
  hooks.h \
  dns/slkdns.h \
  torcontrol.h 


obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
		$(abs_top_srcdir)
libsilk_util_a-clientversion.$(OBJEXT): obj/build.h

# server: shared between silkd and silk-qt
libsilk_server_a_CPPFLAGS = $(AM_CPPFLAGS) $(SILK_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(EVENT_CFLAGS) $(EVENT_PTHREADS_CFLAGS)
libsilk_server_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libsilk_server_a_SOURCES = \
  addrman.cpp \
  alert.cpp \
  bloom.cpp \
  chain.cpp \
  checkpoints.cpp \
  init.cpp \
  leveldbwrapper.cpp \
  main.cpp \
  merkleblock.cpp \
  miner.cpp \
  net.cpp \
  noui.cpp \
  policy/fees.cpp \
  policy/policy.cpp \
  pow.cpp \
  rest.cpp \
  rpc/rpcblockchain.cpp \
  rpc/rpcmining.cpp \
  rpc/rpcmisc.cpp \
  rpc/rpcnet.cpp \
  rpc/rpcrawtransaction.cpp \
  rpc/rpcserver.cpp \
  script/sigcache.cpp \
  txdb.cpp \
  txmempool.cpp \
  validationinterface.cpp \
  kernel.cpp \
  dns/dns.cpp \
  dns/slkdns.cpp \
  torcontrol.cpp \
  $(SILK_CORE_H)

# wallet: shared between silkd and silk-qt, but only linked
# when wallet enabled
libsilk_wallet_a_CPPFLAGS = $(AM_CPPFLAGS) $(SILK_INCLUDES)
libsilk_wallet_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libsilk_wallet_a_SOURCES = \
  wallet/db.cpp \
  wallet/crypter.cpp \
  wallet/rpcdump.cpp \
  wallet/rpcwallet.cpp \
  wallet/wallet.cpp \
  wallet/wallet_ismine.cpp \
  wallet/walletdb.cpp \
  policy/rbf.cpp \
  dns/dns.cpp \
  $(SILK_CORE_H)

# crypto primitives library
crypto_libsilk_crypto_a_CPPFLAGS = $(AM_CPPFLAGS) $(SILK_CONFIG_INCLUDES) $(PIC_FLAGS)
crypto_libsilk_crypto_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS) $(PIC_FLAGS)
crypto_libsilk_crypto_a_SOURCES = \
  crypto/aes.cpp \
  crypto/aes.h \
  crypto/sha1.cpp \
  crypto/sha256.cpp \
  crypto/sha512.cpp \
  crypto/hmac_sha256.cpp \
  crypto/rfc6979_hmac_sha256.cpp \
  crypto/hmac_sha512.cpp \
  crypto/ripemd160.cpp \
  crypto/common.h \
  crypto/sha256.h \
  crypto/sha512.h \
  crypto/hmac_sha256.h \
  crypto/rfc6979_hmac_sha256.h \
  crypto/hmac_sha512.h \
  crypto/sha1.h \
  crypto/ripemd160.h

# common: shared between silkd, and silk-qt and non-server tools
libsilk_common_a_CPPFLAGS = $(AM_CPPFLAGS) $(SILK_INCLUDES)
libsilk_common_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libsilk_common_a_SOURCES = \
  amount.cpp \
  base58.cpp \
  chainparams.cpp \
  coins.cpp \
  compressor.cpp \
  consensus/merkle.cpp \
  primitives/block.cpp \
  primitives/transaction.cpp \
  core_read.cpp \
  core_write.cpp \
  eccryptoverify.cpp \
  hash.cpp \
  key.cpp \
  keystore.cpp \
  netbase.cpp \
  protocol.cpp \
  pubkey.cpp \
  scheduler.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  script/sign.cpp \
  script/standard.cpp \
  script/script_error.cpp \
  $(SILK_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libsilk_util_a_CPPFLAGS = $(AM_CPPFLAGS) $(SILK_INCLUDES)
libsilk_util_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libsilk_util_a_SOURCES = \
  compat/strnlen.cpp \
  compat/glibc_sanity.cpp \
  compat/glibcxx_sanity.cpp \
  chainparamsbase.cpp \
  clientversion.cpp \
  random.cpp \
  rpc/rpcprotocol.cpp \
  support/cleanse.cpp \
  support/pagelocker.cpp \
  sync.cpp \
  timedata.cpp \
  uint256.cpp \
  util.cpp \
  utilstrencodings.cpp \
  utilmoneystr.cpp \
  utiltime.cpp \
  $(SILK_CORE_H)

if GLIBC_BACK_COMPAT
libsilk_util_a_SOURCES += compat/glibc_compat.cpp
endif

# cli: shared between silk-cli and silk-qt
libsilk_cli_a_CPPFLAGS = $(AM_CPPFLAGS) $(SILK_INCLUDES)
libsilk_cli_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libsilk_cli_a_SOURCES = \
  rpc/rpcclient.cpp \
  $(SILK_CORE_H)

nodist_libsilk_util_a_SOURCES = $(srcdir)/obj/build.h
#

# silkd binary #
silkd_SOURCES = silkd.cpp
silkd_CPPFLAGS = $(AM_CPPFLAGS) $(SILK_INCLUDES)
silkd_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
silkd_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
silkd_SOURCES += silkd-res.rc
endif

silkd_LDADD = \
  $(LIBSILK_SERVER) \
  $(LIBSILK_COMMON) \
  $(LIBUNIVALUE) \
  $(LIBSILK_UTIL) \
  $(LIBSILK_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)

if ENABLE_WALLET
silkd_LDADD += libsilk_wallet.a
endif

silkd_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS) $(EVENT_PTHREADS_LIBS) $(EVENT_LIBS)

# silk-cli binary #
silk_cli_SOURCES = silk-cli.cpp
silk_cli_CPPFLAGS = $(AM_CPPFLAGS) $(SILK_INCLUDES) $(EVENT_CFLAGS)
silk_cli_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
silk_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
silk_cli_SOURCES += silk-cli-res.rc
endif

silk_cli_LDADD = \
  $(LIBSILK_CLI) \
  $(LIBUNIVALUE) \
  $(LIBSILK_UTIL)
silk_cli_LDADD += $(BOOST_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(EVENT_LIBS)
#

# silk-tx binary #
silk_tx_SOURCES = silk-tx.cpp
silk_tx_CPPFLAGS = $(AM_CPPFLAGS) $(SILK_INCLUDES)
silk_tx_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
silk_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
silk_tx_SOURCES += silk-tx-res.rc
endif

silk_tx_LDADD = \
  $(LIBUNIVALUE) \
  $(LIBSILK_COMMON) \
  $(LIBSILK_UTIL) \
  $(LIBSILK_CRYPTO) \
  $(LIBSECP256K1)

silk_tx_LDADD += $(BOOST_LIBS) $(CRYPTO_LIBS)
#

# silkconsensus library #
if BUILD_SILK_LIBS
include_HEADERS = script/silkconsensus.h
libsilkconsensus_la_SOURCES = \
  script/silkconsensus_dummy.cpp \
  primitives/transaction.cpp \
  crypto/hmac_sha512.cpp \
  crypto/sha1.cpp \
  crypto/sha256.cpp \
  crypto/sha512.cpp \
  crypto/ripemd160.cpp \
  eccryptoverify.cpp \
  hash.cpp \
  pubkey.cpp \
  script/script.cpp \
  script/interpreter.cpp \
  script/silkconsensus.cpp \
  uint256.cpp \
  utilstrencodings.cpp

if GLIBC_BACK_COMPAT
  libsilkconsensus_la_SOURCES += compat/glibc_compat.cpp
endif

libsilkconsensus_la_LDFLAGS = $(AM_LDFLAGS) -no-undefined $(RELDFLAGS)
libsilkconsensus_la_LIBADD = $(LIBSECP256K1)
libsilkconsensus_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(builddir)/obj -I$(srcdir)/secp256k1/include -DBUILD_SILK_INTERNAL
libsilkconsensus_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)

endif
#

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
	-rm -f leveldb/*/*.gcda leveldb/*/*.gcno leveldb/helpers/memenv/*.gcda leveldb/helpers/memenv/*.gcno
	-rm -f config.h

.rc.o:
	@test -f $(WINDRES)
	$(AM_V_GEN) $(WINDRES) -DWINDRES_PREPROC -i $< -o $@

.mm.o:
	$(AM_V_CXX) $(OBJCXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
		$(CPPFLAGS) $(AM_CXXFLAGS) $(QT_INCLUDES) $(AM_CXXFLAGS) $(PIE_FLAGS) $(CXXFLAGS) -c -o $@ $<

check-symbols: $(bin_PROGRAMS)
if GLIBC_BACK_COMPAT
	@echo "Checking glibc back compat..."
	$(AM_V_at) READELF=$(READELF) CPPFILT=$(CPPFILT) $(top_srcdir)/contrib/devtools/symbol-check.py < $(bin_PROGRAMS)
endif

check-security: $(bin_PROGRAMS)
if HARDEN
	@echo "Checking binary security..."
	$(AM_V_at) READELF=$(READELF) OBJDUMP=$(OBJDUMP) $(top_srcdir)/contrib/devtools/security-check.py < $(bin_PROGRAMS)
endif

%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(abspath $(<D) $<)

if EMBEDDED_LEVELDB
include Makefile.leveldb.include
endif

if ENABLE_QT
include Makefile.qt.include
endif
