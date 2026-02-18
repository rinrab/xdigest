import os
import re
import shutil
import subprocess

# https://github.com/openssl/openssl/blob/master/Configurations/10-main.conf
linux64 = ("elf", ".S", "gcc", "linux64")
linux32 = ("elf", ".S", "gcc", "linux32")
linuxaarch64 = ("aarch64", ".S", "gcc", "linuxaarch64")
linuxppc64 = ("ppc64", ".S", "gcc", "ppc64")
linuxppc32 = ("ppc32", ".S", "gcc", "ppc32")
linuxriscv64 = ("riscv64", ".S", "gcc", "riscv64")

win32 = ("win32n", ".asm", "cl", "win32")
win64 = ("nasm", ".asm", "cl", "win64")

macosx64 = ("macosx", ".S", "gcc", "macosx64")
macosx32 = ("macosx", ".S", "gcc", "macosx32")
macosxaarch64 = ("ios64", ".S", "gcc", "macosxaarch64")

configs_x86_64 = [linux64, win64, macosx64]
configs_x86 = [linux32, win32, macosx32]
configs_aarch64 = [linuxaarch64, macosxaarch64]
configs_ppc= [linuxppc64, linuxppc32]
configs_riscv64 = [linuxriscv64]

def mkdir(path):
    try:
        os.makedirs(path)
    except:
        pass

def copy_fixup(input, output):
    with open(input, 'r') as file:
        data = file.read()

        data = data.replace("OPENSSL_", "xdig_")
        data = data.replace("CRYPTO_", "xdig_")

        # SHA1_Update -> xdig_sha1_ctx_update
        def convert_func_name(name):
            name = name.lower()
            match = re.match(r"^([a-z0-9]+)_([a-z0-9\()]+)$", name)

            if (match == None):
                return f"xdig_{name}"
            else:
                namespace = match.group(1)
                verb = match.group(2)
                return f"xdig_{namespace}_ctx_{verb}"

        def convert_ctx_name(match):
            name = match[1]
            name = name.replace('SHA_CTX', 'SHA1_CTX')
            name = name.lower()
            return f"xdig_{name}_t"

        data = re.sub(r"((SHA|MD)([\w_]*)_(DIGEST_LENGTH|LBLOCK|CBLOCK|LONG|LAST_BLOCK))",
                      r"XDIG_\1",
                      data, flags=re.MULTILINE)

        data = re.sub(r"((sha|md)([0-9]*)_block_data_order(_(a-z0-9)*)?)",
                      r"xdig_\1", data, flags=re.MULTILINE)
        data = data.replace("ossl_md5_block_asm_data_order",
                            "xdig_md5_block_data_order")

    output.replace("openssl", "xdigest")

    with open(output, 'w') as file:
        file.write(data)

    subprocess.run(["svn", "add", "--force", "--parents", output])

def perlasm(configs, file, outputname = None):
    for config, ext, compiler, name in configs:
        if (outputname == None):
            output = os.path.join("xdigest", file)
        else:
            output = os.path.join("xdigest", outputname)

        # sha1-x86_64 -> sha1-linux-x86-64.S
        output = re.sub(r"-?(x86_64|586|x86|armv8|armv4|aarch64|arm64|ppc|riscv64)", "", output)
        output = f"{output}-{name}{ext}"

        print(f"Building {file}.pl -> {output} ({config}, {compiler})")
 
        os.environ["CC"] = compiler

        input = os.path.join("openssl/crypto", file + ".pl")

        mkdir(os.path.dirname(output))

        subprocess.call(["perl", input, config, output])
        copy_fixup(output, output)

def patch(patchfile):
    print(f"Patching {patchfile}")
    code = subprocess.call(["svn", "patch", patchfile])

# perlasm("sha/asm/sha1-armv4-large")
# perlasm("sha/asm/sha1-armv8")
perlasm(configs_x86_64, "sha/asm/sha1-x86_64")
perlasm(configs_x86, "sha/asm/sha1-586")
perlasm(configs_aarch64, "sha/asm/sha1-armv8")
perlasm(configs_ppc, "sha/asm/sha1-ppc")

# perlasm("sha/asm/sha256-586")
# perlasm("sha/asm/sha256-armv4")
# perlasm(configs_x86_64, "sha/asm/sha256-mb-x86_64")

# perlasm("sha/asm/sha512-armv8")
# perlasm("sha/asm/sha512-armv8", "sha/sha256-armv8")
perlasm(configs_x86_64, "sha/asm/sha512-x86_64")
perlasm(configs_x86_64, "sha/asm/sha512-x86_64", "sha/asm/sha256-x86_64")
perlasm(configs_aarch64, "sha/asm/sha512-armv8")
perlasm(configs_ppc, "sha/asm/sha512-ppc")
perlasm(configs_riscv64, "sha/asm/sha512-riscv64-zbb")
perlasm(configs_riscv64, "sha/asm/sha512-riscv64-zvkb-zvknhb")

perlasm(configs_x86, "sha/asm/sha512-586")
perlasm(configs_x86, "sha/asm/sha256-586")
perlasm(configs_aarch64, "sha/asm/sha512-armv8", "sha/asm/sha256-armv8")
perlasm(configs_ppc, "sha/asm/sha512-ppc", "sha/asm/sha256-ppc")
perlasm(configs_riscv64, "sha/asm/sha256-riscv64-zbb")
perlasm(configs_riscv64, "sha/asm/sha256-riscv64-zvkb-zvknha_or_zvknhb")
# perlasm("sha/asm/sha512-armv4")
# perlasm("sha/asm/sha512-ia64")
# perlasm("sha/asm/sha512-ia64", "sha/sha256-ia64")

perlasm(configs_x86_64, "x86_64cpuid", "core/asm/x86_64cpuid")
perlasm(configs_x86, "x86cpuid", "core/asm/x86cpuid")
perlasm(configs_aarch64, "arm64cpuid", "core/asm/arm64cpuid")
perlasm(configs_ppc, "ppccpuid", "core/asm/ppccpuid")
perlasm(configs_riscv64, "riscv64cpuid", "core/asm/riscv64cpuid")

perlasm(configs_x86_64, "md5/asm/md5-x86_64")
perlasm(configs_x86, "md5/asm/md5-586")
perlasm(configs_aarch64, "md5/asm/md5-aarch64")
perlasm(configs_riscv64, "md5/asm/md5-riscv64-zbb")
