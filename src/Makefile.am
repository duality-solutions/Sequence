DIST_SUBDIRS = secp256k1 univalue

AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS)
AM_CXXFLAGS = $(HARDENED_CXXFLAGS)
AM_CPPFLAGS = $(HARDENED_CPPFLAGS)
EXTRA_LIBRARIES =

if EMBEDDED_UNIVALUE
LIBUNIVALUE = univalue/libunivalue.la
 
$(LIBUNIVALUE): $(wildcard univalue/lib/*) $(wildcard univalue/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)
else
LIBUNIVALUE = $(UNIVALUE_LIBS)
endif

if EMBEDDED_LEVELDB
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/include
LEVELDB_CPPFLAGS += -I$(srcdir)/leveldb/helpers/memenv
LIBLEVELDB += $(builddir)/leveldb/libleveldb.a
LIBMEMENV  += $(builddir)/leveldb/libmemenv.a

# NOTE: This dependency is not strictly necessary, but without it make may try to build both in parallel, which breaks the LevelDB build system in a race
$(LIBLEVELDB): $(LIBMEMENV)

$(LIBLEVELDB) $(LIBMEMENV):
	@echo "Building LevelDB ..." && $(MAKE) -C $(@D) $(@F) CXX="$(CXX)" \
	  CC="$(CC)" PLATFORM=$(TARGET_OS) AR="$(AR)" $(LEVELDB_TARGET_FLAGS) \
          OPT="$(CXXFLAGS) $(CPPFLAGS)"
endif

SILK_CONFIG_INCLUDES=-I$(builddir)/config
SILK_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BDB_CPPFLAGS) $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

SILK_INCLUDES += -I$(srcdir)/secp256k1/include
SILK_INCLUDES += $(UNIVALUE_CFLAGS)

LIBSILK_SERVER=libsilk_server.a
LIBSILK_WALLET=libsilk_wallet.a
LIBSILK_COMMON=libsilk_common.a
LIBSILK_CLI=libsilk_cli.a
LIBSILK_UTIL=libsilk_util.a
LIBSILK_CRYPTO=crypto/libsilk_crypto.a
LIBSILK_UNIVALUE=univalue/libsilk_univalue.a
LIBSILKQT=qt/libsilkqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la

$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES = \
  crypto/libsilk_crypto.a \
  libsilk_util.a \
  libsilk_common.a \
  univalue/libsilk_univalue.a \
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

if BUILD_SILKD
  bin_PROGRAMS += silkd
endif

if BUILD_SILK_UTILS
  bin_PROGRAMS += silk-cli silk-tx
endif

.PHONY: FORCE
# Silk Core #
SILK_CORE_H = \
  addrdb.h \
  addrman.h \
  alert.h \
  allocators.h \
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
  primitives/block.h \
  primitives/transaction.h \
  core_io.h \
  wallet/crypter.h \
  wallet/db.h \
  eccryptoverify.h \
  ecwrapper.h \
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
  pow.h \
  protocol.h \
  pubkey.h \
  random.h \
  rpc/rpcclient.h \
  rpc/rpcprotocol.h \
  rpc/rpcregister.h \
  rpc/rpcserver.h \
  script/interpreter.h \
  script/script.h \
  script/sigcache.h \
  script/sign.h \
  script/standard.h \
  script/script_error.h \
  serialize.h \
  streams.h \
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
  version.h \
  wallet/wallet.h \
  wallet/wallet_ismine.h \
  wallet/walletdb.h \
  compat/sanity.h \
## --- ppcoin and silk headers start from this line --- ##
  uint256hm.h \
  kernel.h \
  dns/dns.h \
  hooks.h \
  dns/slkdns.h 

obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh $(abs_top_builddir)/src/obj/build.h \
	  $(abs_top_srcdir)
libsilk_util_a-clientversion.$(OBJEXT): obj/build.h

# server: shared between silkd and silk-qt
libsilk_server_a_CPPFLAGS = $(AM_CPPFLAGS) $(SILK_INCLUDES) $(MINIUPNPC_CPPFLAGS)
libsilk_server_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libsilk_server_a_SOURCES = \
  addrman.cpp \
  addrdb.cpp \
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
## --- ppcoin and silk sources start from this line --- ##
  kernel.cpp \
  dns/dns.cpp \
  dns/slkdns.cpp \
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
  dns/dns.cpp \
  $(SILK_CORE_H)

# crypto primitives library
crypto_libsilk_crypto_a_CPPFLAGS = $(AM_CPPFLAGS) $(SILK_CONFIG_INCLUDES)
crypto_libsilk_crypto_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libsilk_crypto_a_SOURCES = \
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
libsilk_consensus_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libsilk_common_a_SOURCES = \
  allocators.cpp \
  amount.cpp \
  base58.cpp \
  chainparams.cpp \
  coins.cpp \
  compressor.cpp \
  primitives/block.cpp \
  primitives/transaction.cpp \
  core_read.cpp \
  core_write.cpp \
  eccryptoverify.cpp \
  ecwrapper.cpp \
  hash.cpp \
  key.cpp \
  keystore.cpp \
  netbase.cpp \
  protocol.cpp \
  pubkey.cpp \
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

silkd_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS)
#

# silk-cli binary #
silk_cli_SOURCES = silk-cli.cpp
silk_cli_CPPFLAGS = $(AM_CPPFLAGS) $(SILK_INCLUDES)
silk_cli_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
silk_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)


if TARGET_WINDOWS
silk_cli_SOURCES += silk-cli-res.rc
endif

silk_cli_LDADD = \
  $(LIBSILK_CLI) \
  $(LIBSILK_UTIL) \
  $(LIBUNIVALUE)

silk_cli_LDADD += $(BOOST_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS)
#

# silk-tx binary #
silk_tx_SOURCES = silk-tx.cpp
silk_tx_CPPFLAGS = $(AM_CPPFLAGS) $(SILK_INCLUDES)
silk_tx_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
silk_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

silk_tx_LDADD = \
  $(LIBUNIVALUE) \
  $(LIBSILK_COMMON) \
  $(LIBSILK_UTIL) \
  $(LIBSILK_CRYPTO) \
  $(LIBSECP256K1)

silk_tx_LDADD += $(BOOST_LIBS) $(CRYPTO_LIBS)
#

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
  ecwrapper.cpp \
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

libsilkconsensus_la_LDFLAGS = -no-undefined $(RELDFLAGS)
libsilkconsensus_la_LIBADD = $(CRYPTO_LIBS)
libsilkconsensus_la_CPPFLAGS = $(CRYPTO_CFLAGS) -I$(builddir)/obj -DBUILD_SILK_INTERNAL
if USE_LIBSECP256K1
libsilkconsensus_la_LIBADD += secp256k1/libsecp256k1.la
endif
endif

CLEANFILES = $(EXTRA_LIBRARIES)

CLEANFILES += *.gcda *.gcno
CLEANFILES += compat/*.gcda compat/*.gcno
CLEANFILES += crypto/*.gcda crypto/*.gcno
CLEANFILES += primitives/*.gcda primitives/*.gcno
CLEANFILES += script/*.gcda script/*.gcno
CLEANFILES += univalue/*.gcda univalue/*.gcno

DISTCLEANFILES = obj/build.h

EXTRA_DIST = leveldb

clean-local:
	-$(MAKE) -C leveldb clean
	-$(MAKE) -C secp256k1 clean
	-$(MAKE) -C univalue clean
	-rm -rf .libs
	-rm -rf .deps
	-rm -f leveldb/*/*.gcda leveldb/*/*.gcno leveldb/helpers/memenv/*.gcda leveldb/helpers/memenv/*.gcno
	-rm -f config.h
	-rm -rf compat/.deps && rm -rf compat/.libs && rm -f compat/.dirstamp
	-rm -rf crypto/.deps && rm -rf crypto/.libs && rm -f crypto/.dirstamp
	-rm -rf primitives/.deps && rm -rf primitives/.libs && rm -f primitives/.dirstamp
	-rm -rf qt/.deps && rm -rf qt/.libs && rm -f qt/.dirstamp && rm -rf qt/test/.deps
	-rm -rf script/.deps && rm -rf script/.libs && rm -f script/.dirstamp
	-rm -rf univalue/.deps && rm -rf univalue/.libs && rm -f univalue/.dirstamp
	-rm -rf test/.deps && rm -rf test/.libs && rm -f test/.dirstamp && rm	-f test/buildenv.py
	-rm -rf secp256k1/.deps && rm -rf secp256k1/.libs && rm -f secp256k1/.dirstamp
	-rm -f Makefile && rm -f Makefile.in
	-rm -f config/silk-config.h && rm -f config/silk-config.* && rm -f config/stamp-h1
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

if ENABLE_QT
include Makefile.qt.include
endif

#if ENABLE_QT_TESTS
#include Makefile.qttest.include
#endif
