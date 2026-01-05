import os
import re
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

mkdir(os.path.join(src, "sha"))
mkdir(os.path.join(src, "core"))
include_path = "include"
mkdir(include_path)
mkdir(os.path.join(include_path, "xhash"))
mkdir(os.path.join(include_path, "internal"))
mkdir(os.path.join(include_path, "crypto"))

def copy_fixup(input, output):
    with open(input, 'r') as file:
        data = file.read()

        data = data.replace("OPENSSL_", "xhash_")
        data = data.replace("CRYPTO_", "xhash_")

        data = re.sub(r"include <openssl\/([^>]*)>",
                      r"include <xhash/\1>",
                      data, 0, re.MULTILINE)

        data = re.sub(r"(SHA\d+_?\w*\()",
                      lambda match: f"xhash_{match[1].lower()}",
                      data, 0, re.MULTILINE)

    output.replace("openssl", "xhash")

    with open(output, 'w') as file:
        file.write(data)

def perlasm(config, file):
    input = os.path.join(crypto, file + ".pl")
    output = os.path.join(asm, config, file + ".S")

    mkdir(os.path.dirname(output))

    subprocess.call(["perl", input, config, output])
    copy_fixup(output, output)

def source(path, outdir = ""):
    input = os.path.join(crypto, path)
    output = os.path.join(src, outdir, path)

    copy_fixup(input, output)

def include(path):
    input = os.path.join(openssl_include, path)
    output = os.path.join(include_path, path)

    output = output.replace("openssl", "xhash")

    copy_fixup(input, output)

def patch(patchfile):
    print(f"Patching {patchfile}")
    code = subprocess.call(["svn", "patch", patchfile])

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

source("sha/sha_local.h")
source("sha/sha256.c")
source("sha/sha512.c")
source("cpuid.c", "core")
source("ctype.c", "core")
source("ebcdic.c", "core")

include("openssl/sha.h")
include("openssl/e_os2.h")
include("openssl/ebcdic.h")
include("internal/endian.h")
include("internal/common.h")
include("internal/e_os.h")
include("internal/numbers.h")
include("crypto/md32_common.h")
include("crypto/sha.h")
include("crypto/ctype.h")

patch("patches/remove_slop_includes_sources.patch")
patch("patches/remove_slop_includes_headers.patch")
patch("patches/remove_assert.patch")
patch("patches/inline_cleanse.patch")
patch("patches/inline_dummy_export.patch")
patch("patches/sha_cleanup_header.patch")
patch("patches/OPENSSL_IA32CAP_P_MAX_INDEXES.patch")
patch("patches/include_stdlib_for_getenv.patch")
patch("patches/sha1_adjust_export_names.patch")
