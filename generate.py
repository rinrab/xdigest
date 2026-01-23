import os
import re
import shutil
import subprocess

# https://github.com/openssl/openssl/blob/master/Configurations/10-main.conf
linux64 = ("elf", ".S", "gcc", "linux64")
linux32 = ("elf", ".S", "gcc", "linux32")
linuxaarch64 = ("aarch64", ".S", "gcc", "linuxaarch64")
win32 = ("win32n", ".asm", "cl", "win32")
win64 = ("nasm", ".asm", "cl", "win64")

configs_x86_64 = [linux64, win64]
configs_x86 = [linux32, win32]
configs_aarch64 = [linuxaarch64]

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
mkdir(os.path.join(src, "md4"))
mkdir(os.path.join(src, "md2"))
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

        remove_includes = [
            "openssl/crypto.h",
            "openssl/configuration.h",
            "openssl/opensslconf.h",
            "openssl/opensslv.h",
            "openssl/macros.h",
            "crypto/cryptlib.h",
            "internal/cryptlib.h",
            "internal/nelem.h",
            "internal/deprecated.h",
        ]

        hidden_includes = [
            "openssl/e_os2.h",
        ]

        def convert_func_name(name):
            # SHA1_Update -> xhash_sha1_update
            name = name.lower()
            return f"xhash_{name}"

        def convert_ctx_name(match):
            name = match[1]
            name = name.replace('SHA_CTX', 'SHA1_CTX')
            name = name.lower()
            return f"xhash_{name}_t"

        for inc in remove_includes:
            inc = re.escape(inc)

            data = re.sub(rf"^#\s*include [<\"]({inc})[>\"]$",
                          r"/* ignored include '\1' */",
                          data, flags=re.MULTILINE)

        for inc in hidden_includes:
            inc = re.escape(inc)

            data = re.sub(rf"^#\s*include [<\"]({inc})[>\"]$",
                          lambda match: f"#include \"{match[1].replace("openssl", "internal")}\"",
                          data, flags=re.MULTILINE)

        data = re.sub(r"include <openssl\/([^>]*)>",
                      r"include <xhash/\1>",
                      data, flags=re.MULTILINE)

        data = re.sub(r"((SHA|MD)\d+_?\w*\()",
                      lambda match: convert_func_name(match[1]),
                      data, flags=re.MULTILINE)

        data = re.sub(r"OSSL_DEPRECATEDIN_3_0\s*", "",
                      data, flags=re.MULTILINE)

        data = re.sub(r"((SHA|MD)\d*_CTX)", convert_ctx_name,
                      data, flags=re.MULTILINE)

        data = re.sub(r"((SHA|MD)([\w_]*)_(DIGEST_LENGTH|LBLOCK|CBLOCK|LONG|LAST_BLOCK))",
                      r"XHASH_\1",
                      data, flags=re.MULTILINE)

        data = re.sub(r"^(#define HASH_\w+\s+)((SHA|MD)(\d*)_(Update|Final|Transform|Init))$",
                      lambda match: f"{match[1]}{convert_func_name(match[2])}",
                      data, flags=re.MULTILINE)

    output.replace("openssl", "xhash")

    with open(output, 'w') as file:
        file.write(data)

    subprocess.run(["svn", "add", "--force", "--parents", output])

def perlasm(configs, file, outputname = None):
    for config, ext, compiler, name in configs:
        if (outputname == None):
            output = os.path.join("src", file)
        else:
            output = os.path.join("src", outputname)

        # sha1-x86_64 -> sha1-linux-x86-64.S
        output = re.sub(r"-?(x86_64|586|x86|armv8|armv4|aarch64|arm64)", "", output)
        output = f"{output}-{name}{ext}"

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

def include(path, outname = None):
    input = os.path.join(openssl_include, path)

    if (outname == None):
        output = os.path.join(include_path, path)
    else:
        output = os.path.join(include_path, outname)

    output = output.replace("openssl", "xhash")

    copy_fixup(input, output)

def patch(patchfile):
    print(f"Patching {patchfile}")
    code = subprocess.call(["svn", "patch", patchfile])

# perlasm("sha/asm/sha1-armv4-large")
# perlasm("sha/asm/sha1-armv8")
perlasm(configs_x86_64, "sha/asm/sha1-x86_64")
perlasm(configs_x86, "sha/asm/sha1-586")
perlasm(configs_aarch64, "sha/asm/sha1-armv8")

# perlasm("sha/asm/sha256-586")
# perlasm("sha/asm/sha256-armv4")
# perlasm(configs_x86_64, "sha/asm/sha256-mb-x86_64")

# perlasm("sha/asm/sha512-armv8")
# perlasm("sha/asm/sha512-armv8", "sha/sha256-armv8")
perlasm(configs_x86_64, "sha/asm/sha512-x86_64")
perlasm(configs_x86_64, "sha/asm/sha512-x86_64", "sha/asm/sha256-x86_64")
perlasm(configs_aarch64, "sha/asm/sha512-armv8")

perlasm(configs_x86, "sha/asm/sha512-586")
perlasm(configs_x86, "sha/asm/sha256-586")
perlasm(configs_aarch64, "sha/asm/sha512-armv8", "sha/asm/sha256-armv8")
# perlasm("sha/asm/sha512-armv4")
# perlasm("sha/asm/sha512-ia64")
# perlasm("sha/asm/sha512-ia64", "sha/sha256-ia64")

perlasm(configs_x86_64, "x86_64cpuid", "core/asm/x86_64cpuid")
perlasm(configs_x86, "x86cpuid", "core/asm/x86cpuid")
perlasm(configs_aarch64, "arm64cpuid", "core/asm/arm64cpuid")

perlasm(configs_x86_64, "md5/asm/md5-x86_64")
perlasm(configs_x86, "md5/asm/md5-586")
perlasm(configs_aarch64, "md5/asm/md5-aarch64")

source("arm_arch.h", "core")
source("armcap.c", "core")
source("cpuid.c", "core")
source("ebcdic.c", "core")
source("mem_clr.c", "core")

source("sha/sha_local.h")
source("sha/sha256.c")
source("sha/sha512.c")

source("md5/md5_dgst.c")
source("md5/md5_local.h")
source("md5/md5_one.c")

source("md4/md4_dgst.c")
source("md4/md4_local.h")
source("md4/md4_one.c")

source("md2/md2_dgst.c")
source("md2/md2_one.c")

include("openssl/sha.h")
include("openssl/md5.h")
include("openssl/md4.h")
include("openssl/md2.h")

include("openssl/ebcdic.h")
include("openssl/e_os2.h", "internal/e_os2.h")

include("internal/endian.h")
include("internal/common.h")
include("internal/e_os.h")
include("internal/numbers.h")

include("crypto/md32_common.h")
include("crypto/sha.h")

patch("patches/xhash_cleanse.patch")
patch("patches/inline_dummy_export.patch")
patch("patches/OPENSSL_IA32CAP_P_MAX_INDEXES.patch")
patch("patches/sha256_hash_data.patch")
patch("patches/sha512_hash_data.patch")
patch("patches/die_and_assert.patch")

patch("patches/mem_clr_remove_callback.patch")
patch("patches/mem_clr_ifdef.patch")

patch("patches/md5_h_cleanup_header.patch")
patch("patches/sha_h_cleanup_header.patch")

patch("patches/export_init_func.patch")

# patch("patches/include_stdlib_for_getenv.patch")
patch("patches/remove_getenv.patch")
patch("patches/armcap_ifdef.patch")
