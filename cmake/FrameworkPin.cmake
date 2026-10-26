# MafiaHub/Framework dependency pin.
#
# MafiaMP links a prebuilt Framework SDK (cmake/FrameworkBootstrap.cmake), so "which Framework" is part of
# the product, not an environment detail. This file is that record: the release whose archives are
# downloaded at configure time, and the SHA-256 of each archive MafiaMP consumes. A tag is a mutable ref,
# so the version alone would pin nothing; the hashes are what fix the content, and
# file(DOWNLOAD EXPECTED_HASH) fails the configure if an archive is ever repointed or tampered with.
#
# Plain set(), not CACHE entries: a cached value survives in an existing build tree, so bumping the
# pin here and reconfiguring incrementally would silently keep the old release.
#
# Raising this pin is a release-affecting change: rebuild and re-test the client and the server
# together before tagging. If the new Framework crosses a MAJOR boundary its network build token
# changes (NetworkPeer::BuildToken compares major components), so every peer must update together.
#
# The release notes of every Framework release carry this block ready to paste; or:
# shasum -a 256 Framework-<ver>-*. -DFW_SDK_ROOT or -DFW_SOURCE_DIR (see builds/build.bat) override it.
#
# Before the SDK MafiaMP built nested in the Framework tree, on whatever Framework commit was checked out.

# Framework 40.0.0: 38 moved the RPC identifier salt to runtime (InstanceOptions::rpcIdentifierSalt),
# 39 fixed the SDK package config on Linux, 40 ships the scripting-api tooling in the SDK.
set(FRAMEWORK_PIN_VERSION "40.0.0")

# One archive per OS; the Windows one carries the x86 and x64 libraries (MafiaMP uses x64 only).
set(FRAMEWORK_PIN_SHA256_windows "07c96c9f781a378bdedfb5ef33e72f205a4c6d33d7b915c2cfd00b107f88b90b")
set(FRAMEWORK_PIN_SHA256_linux "64cab92c8fe5e53f80185f4c25b1686b0e41c1a8beef29504bdd55ce4fab4154")
