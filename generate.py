import os
import re
import shutil
import subprocess

# https://github.com/openssl/openssl/blob/master/Configurations/10-main.conf
linux_x86_64 = ("elf", ".S", "gcc")
linux_x86 = ("elf", ".S", "gcc")
win32 = ("win32n", ".asm", "cl")
win64 = ("nasm", ".asm", "cl")

configs_x86_64 = [linux_x86_64, win64]
configs_x86 = [linux_x86, win32]

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
mkdir(os.path.join(src, "md5"))
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
                      data, flags=re.MULTILINE)

        data = re.sub(r"(SHA\d+_?\w*\()",
                      lambda match: f"xhash_{match[1].lower()}",
                      data, flags=re.MULTILINE)

        def convert_ctx_name(match):
            name = match[1]
            name = name.replace('SHA_CTX', 'SHA1_CTX')
            name = name.lower()
            return f"xhash_{name}_t"

        data = re.sub(r"((SHA|MD5)\d*_CTX)", convert_ctx_name,
                      data, flags=re.MULTILINE)

    output.replace("openssl", "xhash")

    with open(output, 'w') as file:
        file.write(data)

def perlasm(configs, file, outputname = None):
    for config, ext, compiler in configs:
        if (outputname == None):
            output = os.path.join(asm, config, file.replace("asm/", "") + ext)
        else:
            output = os.path.join(asm, config, outputname + ext)

        print(f"Building {file}.pl -> {output} ({config}, {compiler})")

        os.environ["CC"] = compiler

        input = os.path.join(crypto, file + ".pl")

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

# perlasm("sha/asm/sha1-armv4-large")
# perlasm("sha/asm/sha1-armv8")
perlasm(configs_x86_64, "sha/asm/sha1-x86_64")
perlasm(configs_x86, "sha/asm/sha1-586")

# perlasm("sha/asm/sha256-586")
# perlasm("sha/asm/sha256-armv4")
# perlasm(configs_x86_64, "sha/asm/sha256-mb-x86_64")

# perlasm("sha/asm/sha512-armv8")
# perlasm("sha/asm/sha512-armv8", "sha/sha256-armv8")
perlasm(configs_x86_64, "sha/asm/sha512-x86_64")
perlasm(configs_x86_64, "sha/asm/sha512-x86_64", "sha/sha256-x86_64")
perlasm(configs_x86, "sha/asm/sha512-586")
perlasm(configs_x86, "sha/asm/sha512-586")
# perlasm("sha/asm/sha512-armv4")
# perlasm("sha/asm/sha512-ia64")
# perlasm("sha/asm/sha512-ia64", "sha/sha256-ia64")

perlasm(configs_x86_64, "x86_64cpuid")
perlasm(configs_x86, "x86cpuid")

source("sha/sha_local.h")
source("sha/sha256.c")
source("sha/sha512.c")
source("cpuid.c", "core")
source("ctype.c", "core")
source("ebcdic.c", "core")

source("md5/md5_dgst.c")
source("md5/md5_local.h")
source("md5/md5_one.c")

include("openssl/sha.h")
include("openssl/md5.h")
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
patch("patches/sha_h_cleanup_header.patch")
patch("patches/sha_h_undeprecate.patch")
patch("patches/OPENSSL_IA32CAP_P_MAX_INDEXES.patch")
patch("patches/sha1_adjust_export_names.patch")
patch("patches/sha256_adjust_export_names.patch")
patch("patches/sha256_hash_data.patch")
patch("patches/sha512_hash_data.patch")

patch("patches/md5_h_cleanup_header.patch")
patch("patches/md5_src_cleanup_includes.patch")

patch("patches/export_init_func.patch")

# patch("patches/include_stdlib_for_getenv.patch")
patch("patches/remove_getenv.patch")
