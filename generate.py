import os
import shutil
import subprocess

def mkdir(path):
    try:
        os.makedirs(path)
    except:
        pass

crypto = "openssl/crypto"
openssl_include = "openssl/include"

asm = "asm"
src = "src"

def perlasm(config, file):
    input = os.path.join(crypto, file + ".pl")
    output = os.path.join(asm, config, file + ".S")

    mkdir(os.path.dirname(output))

    subprocess.call(["perl", input, config, output])

os.environ["CC"] = "gcc"

perlasm("elf", "sha/asm/sha1-586")
perlasm("elf", "sha/asm/sha1-armv4-large")
perlasm("elf", "sha/asm/sha1-armv8")
perlasm("elf", "sha/asm/sha1-x86_64")

perlasm("elf", "sha/asm/sha256-586")
perlasm("elf", "sha/asm/sha256-armv4")

perlasm("elf", "sha/asm/sha512-armv8")
perlasm("elf", "sha/asm/sha512-x86_64")
perlasm("elf", "sha/asm/sha512-586")
perlasm("elf", "sha/asm/sha512-armv4")
perlasm("elf", "sha/asm/sha512-armv8")
perlasm("elf", "sha/asm/sha512-x86_64")

perlasm("elf", "x86cpuid")
perlasm("elf", "x86_64cpuid")

sha = os.path.join(src, "sha")
mkdir(sha)

shutil.copy2(os.path.join(crypto, "sha/sha_local.h"), sha)
shutil.copy2(os.path.join(crypto, "sha/sha256.c"), sha)
shutil.copy2(os.path.join(crypto, "sha/sha512.c"), sha)
shutil.copy2(os.path.join(crypto, "cpuid.c"), src)
shutil.copy2(os.path.join(crypto, "ctype.c"), src)
shutil.copy2(os.path.join(crypto, "ebcdic.c"), src)

include = "include"
mkdir(include)
mkdir(os.path.join(include, "openssl"))
mkdir(os.path.join(include, "internal"))
mkdir(os.path.join(include, "crypto"))

shutil.copy2(os.path.join(openssl_include, "openssl/sha.h"),
             os.path.join(include, "openssl"))
shutil.copy2(os.path.join(openssl_include, "openssl/macros.h"),
             os.path.join(include, "openssl"))
shutil.copy2(os.path.join(openssl_include, "openssl/e_os2.h"),
             os.path.join(include, "openssl"))
shutil.copy2(os.path.join(openssl_include, "openssl/opensslconf.h"),
             os.path.join(include, "openssl"))
shutil.copy2(os.path.join(openssl_include, "openssl/types.h"),
             os.path.join(include, "openssl"))
shutil.copy2(os.path.join(openssl_include, "openssl/symhacks.h"),
             os.path.join(include, "openssl"))
shutil.copy2(os.path.join(openssl_include, "openssl/cryptoerr.h"),
             os.path.join(include, "openssl"))
shutil.copy2(os.path.join(openssl_include, "openssl/cryptoerr_legacy.h"),
             os.path.join(include, "openssl"))
shutil.copy2(os.path.join(openssl_include, "openssl/core.h"),
             os.path.join(include, "openssl"))
shutil.copy2(os.path.join(openssl_include, "openssl/stack.h"),
             os.path.join(include, "openssl"))
shutil.copy2(os.path.join(openssl_include, "openssl/safestack.h"),
             os.path.join(include, "openssl"))
shutil.copy2(os.path.join(openssl_include, "openssl/ebcdic.h"),
             os.path.join(include, "openssl"))

shutil.copy2(os.path.join(openssl_include, "internal/endian.h"),
             os.path.join(include, "internal"))
shutil.copy2(os.path.join(openssl_include, "internal/deprecated.h"),
             os.path.join(include, "internal"))
shutil.copy2(os.path.join(openssl_include, "internal/common.h"),
             os.path.join(include, "internal"))
shutil.copy2(os.path.join(openssl_include, "internal/cryptlib.h"),
             os.path.join(include, "internal"))
shutil.copy2(os.path.join(openssl_include, "internal/e_os.h"),
             os.path.join(include, "internal"))
shutil.copy2(os.path.join(openssl_include, "internal/numbers.h"),
             os.path.join(include, "internal"))
shutil.copy2(os.path.join(openssl_include, "internal/nelem.h"),
             os.path.join(include, "internal"))

shutil.copy2(os.path.join(openssl_include, "crypto/md32_common.h"),
             os.path.join(include, "crypto"))
shutil.copy2(os.path.join(openssl_include, "crypto/sha.h"),
             os.path.join(include, "crypto"))
shutil.copy2(os.path.join(openssl_include, "crypto/ctype.h"),
             os.path.join(include, "crypto"))

shutil.copy2(os.path.join(include, "custom/configuration.h"),
             os.path.join(include, "openssl"))
shutil.copy2(os.path.join(include, "custom/opensslv.h"),
             os.path.join(include, "openssl"))

subprocess.call(["svn", "patch", "patches/remove_slop_includes_sources.patch"])
subprocess.call(["svn", "patch", "patches/remove_slop_includes_headers.patch"])
subprocess.call(["svn", "patch", "patches/remove_assert.patch"])
subprocess.call(["svn", "patch", "patches/inline_cleanse.patch"])
subprocess.call(["svn", "patch", "patches/inline_dummy_export.patch"])
subprocess.call(["svn", "patch", "patches/sha_cleanup_header.patch"])
subprocess.call(["svn", "patch", "patches/OPENSSL_IA32CAP_P_MAX_INDEXES.patch"])

