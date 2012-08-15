# Copyright (c) 2012 The Native Client Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
#
# Environment variable NACL_PACKAGES_BITSIZE should be unset or set to "32"
# for a 32-bit build.  It should be set to "64" for a 64-bit build.


# NAMING CONVENTION
# =================
#
# This file is source'd by other scripts especially those inside libraries/
# and makes functions env variables available to those scripts.
# Only variables beginging with "NACL_" are intended to be used by those
# scripts!

set -o nounset
set -o errexit

# Scripts that source this file must be run from within the naclports src tree.
# Note that default build steps reference the packages directory.
readonly SAVE_PWD=$(pwd)

readonly START_DIR=$(cd "$(dirname "$0")" ; pwd)
readonly NACL_SRC=$(expr ${START_DIR} : '\(.*\/src\).*')
readonly NACL_PACKAGES=${NACL_SRC}
readonly NACL_NATIVE_CLIENT_SDK=$(cd ${NACL_SRC} ; pwd)

# Pick platform directory for compiler.
readonly OS_NAME=$(uname -s)
if [ $OS_NAME = "Darwin" ]; then
  readonly OS_SUBDIR="mac"
  readonly OS_SUBDIR_SHORT="mac"
  readonly OS_JOBS=4
elif [ $OS_NAME = "Linux" ]; then
  readonly OS_SUBDIR="linux"
  readonly OS_SUBDIR_SHORT="linux"
  readonly OS_JOBS=4
else
  readonly OS_SUBDIR="windows"
  readonly OS_SUBDIR_SHORT="win"
  readonly OS_JOBS=1
fi

# Get the desired bit size,
# Note(robertm): we abuse this to introduce a "pnacl" flavor.
export NACL_PACKAGES_BITSIZE=${NACL_PACKAGES_BITSIZE:-"32"}
if [ ${NACL_PACKAGES_BITSIZE} = "32" ] ; then
  readonly CROSS_ID=i686
elif [ ${NACL_PACKAGES_BITSIZE} = "64" ] ; then
  readonly CROSS_ID=x86_64
elif [ ${NACL_PACKAGES_BITSIZE} = "pnacl" ] ; then
  readonly CROSS_ID=pnacl
else
  echo "Unknown value for NACL_PACKAGES_BITSIZE: '${NACL_PACKAGES_BITSIZE}'" 1>&2
  exit -1
fi

if [ ${NACL_PACKAGES_BITSIZE} = "32" ] ; then
  readonly NACL_SEL_LDR=${NACL_SDK_ROOT}/tools/sel_ldr_x86_32
  readonly NACL_IRT=${NACL_SDK_ROOT}/tools/irt_x86_32.nexe
else
  # TODO(eugenis): Is this correct for PNACL?
  readonly NACL_SEL_LDR=${NACL_SDK_ROOT}/tools/sel_ldr_x86_64
  readonly NACL_IRT=${NACL_SDK_ROOT}/tools/irt_x86_64.nexe
fi

# NACL_CROSS_PREFIX is the prefix of the executables in the
# toolchain's "bin" directory. For example: i686-nacl-<toolname>.
if [ ${NACL_PACKAGES_BITSIZE} == "pnacl" ]; then
  export NACL_CROSS_PREFIX=pnacl
else
  export NACL_CROSS_PREFIX=${CROSS_ID}-nacl
fi

# configure spec for if MMX/SSE/SSE2/Assembly should be enabled/disabled
# TODO: Currently only x86-32 will encourage MMX, SSE & SSE2 intrinsics
#       and handcoded assembly.
if [ $NACL_PACKAGES_BITSIZE = "32" ] ; then
  readonly NACL_OPTION="enable"
else
  readonly NACL_OPTION="disable"
fi

if [ -z "${NACL_SDK_ROOT:-}" ]; then
  echo "-------------------------------------------------------------------"
  echo "NACL_SDK_ROOT is unset."
  echo "This environment variable needs to be pointed at some version of"
  echo "the Native Client SDK (the directory containing toolchain/)."
  echo "NOTE: set this to an absolute path."
  echo "-------------------------------------------------------------------"
  exit -1
fi


# sha1check python script
readonly SHA1CHECK=${NACL_SRC}/build_tools/sha1check.py

# packages subdirectories
readonly NACL_PACKAGES_LIBRARIES=${NACL_PACKAGES}/libraries
readonly NACL_PACKAGES_OUT=${NACL_SRC}/out
readonly NACL_PACKAGES_REPOSITORY=${NACL_PACKAGES_OUT}/repository-${CROSS_ID}
readonly NACL_PACKAGES_PUBLISH=${NACL_PACKAGES_OUT}/publish
readonly NACL_PACKAGES_TARBALLS=${NACL_PACKAGES_OUT}/tarballs


InitializeNaClGccToolchain() {
  # locate default nacl_sdk toolchain
  # TODO: x86 only at the moment
  NACL_GLIBC=${NACL_GLIBC:-0}
  if [ $NACL_GLIBC = "1" ] ; then
    # m15-m17 SDK layouts have the glibc toolchain without the _glibc suffix
    local TENTATIVE_NACL_GCC=${NACL_SDK_ROOT}/toolchain/${OS_SUBDIR_SHORT}_x86_glibc/bin/i686-nacl-gcc
    local TENTATIVE_NEWLIB_NACL_GCC=${NACL_SDK_ROOT}/toolchain/${OS_SUBDIR_SHORT}_x86_newlib/bin/i686-nacl-gcc

    echo ${TENTATIVE_NACL_GCC}
    if [ -e ${TENTATIVE_NACL_GCC} ]; then
      local TOOLCHAIN_SUFFIX="_glibc"
    elif [ -e ${TENTATIVE_NEWLIB_NACL_GCC} ]; then
      local TOOLCHAIN_SUFFIX=""
    else
      # if neither _newlib or _glibc suffixes exist,
      # this is a pre-m15 release which has no glibc
      echo "------------------------------------------------------------------"
      echo "error: glibc toolchain not present"
      echo "Your SDK appears to be pre pepper_15, please upgrade to use glibc."
      echo "------------------------------------------------------------------"
      exit -1
    fi
  else
    # Fall back to pre-m15 SDK layout if we can't find i686-nacl-gcc.
    local TENTATIVE_NACL_GCC=${NACL_SDK_ROOT}/toolchain/${OS_SUBDIR_SHORT}_x86_newlib/bin/i686-nacl-gcc

    echo ${TENTATIVE_NACL_GCC}
    if [ -e ${TENTATIVE_NACL_GCC} ]; then
      local TOOLCHAIN_SUFFIX="_newlib"
    else
      local TOOLCHAIN_SUFFIX=""
    fi
  fi


  readonly NACL_TOOLCHAIN_ROOT=${NACL_TOOLCHAIN_ROOT:-${NACL_SDK_ROOT}/toolchain/${OS_SUBDIR_SHORT}_x86${TOOLCHAIN_SUFFIX}}
  # TODO(robertm): can we get rid of NACL_SDK_BASE?
  readonly NACL_SDK_BASE=${NACL_SDK_BASE:-${NACL_TOOLCHAIN_ROOT}}

  readonly NACL_BIN_PATH=${NACL_TOOLCHAIN_ROOT}/bin

  # export nacl tools for direct use in patches.
  export NACLCC=${NACL_BIN_PATH}/${NACL_CROSS_PREFIX}-gcc
  export NACLCXX=${NACL_BIN_PATH}/${NACL_CROSS_PREFIX}-g++
  export NACLAR=${NACL_BIN_PATH}/${NACL_CROSS_PREFIX}-ar
  export NACLRANLIB=${NACL_BIN_PATH}/${NACL_CROSS_PREFIX}-ranlib
  export NACLLD=${NACL_BIN_PATH}/${NACL_CROSS_PREFIX}-ld
  export NACLSTRINGS=${NACL_BIN_PATH}/${NACL_CROSS_PREFIX}-strings
  export NACLSTRIP=${NACL_BIN_PATH}/${NACL_CROSS_PREFIX}-strip

  # NACL_SDK_GCC_SPECS_PATH is where nacl-gcc 'specs' file will be installed
  readonly NACL_SDK_GCC_SPECS_PATH=${NACL_TOOLCHAIN_ROOT}/lib/gcc/x86_64-nacl/4.4.3

  # NACL_SDK_USR is where the headers, libraries, etc. will be installed
  readonly NACL_SDK_USR=${NACL_TOOLCHAIN_ROOT}/${NACL_CROSS_PREFIX}/usr
  readonly NACL_SDK_USR_INCLUDE=${NACL_SDK_USR}/include
  readonly NACL_SDK_LIB=${NACL_TOOLCHAIN_ROOT}/x86_64-nacl/lib${NACL_PACKAGES_BITSIZE}
  readonly NACL_SDK_USR_LIB=${NACL_SDK_USR}/lib
  readonly NACL_SDK_USR_BIN=${NACL_SDK_USR}/bin

  # NACL_SDK_MULITARCH_USR is a version of NACL_SDK_USR that gets passed into
  # the gcc specs file.  It has a gcc spec-file conditional for ${CROSS_ID}
  readonly NACL_SDK_MULTIARCH_USR=${NACL_TOOLCHAIN_ROOT}/\%\(nacl_arch\)/usr
  readonly NACL_SDK_MULTIARCH_USR_INCLUDE=${NACL_SDK_MULTIARCH_USR}/include
  readonly NACL_SDK_MULTIARCH_USR_LIB=${NACL_SDK_MULTIARCH_USR}/lib
}


InitializePNaClToolchain() {
  NACL_GLIBC=${NACL_GLIBC:-0}
  if [ $NACL_GLIBC = "1" ] ; then
    local TOOLCHAIN_SUFFIX="glibc"
  else
    local TOOLCHAIN_SUFFIX="newlib"
  fi
  # TODO(robertm): fix this to account for OS and ARCH.
  #                This needs to be better thought thru:
  #                Currently, SDK and TC seem to be using different paths
  readonly NACL_TOOLCHAIN_ROOT=${NACL_TOOLCHAIN_ROOT:-${NACL_SDK_ROOT}/toolchain/linux_x86_pnacl/${TOOLCHAIN_SUFFIX}}
  readonly NACL_SDK_BASE=${NACL_SDK_BASE:-${NACL_TOOLCHAIN_ROOT}}

  readonly NACL_BIN_PATH=${NACL_TOOLCHAIN_ROOT}/bin

  # export nacl tools for direct use in patches.
  export NACLCC=${NACL_BIN_PATH}/${NACL_CROSS_PREFIX}-clang
  export NACLCXX=${NACL_BIN_PATH}/${NACL_CROSS_PREFIX}-clang++
  export NACLAR=${NACL_BIN_PATH}/${NACL_CROSS_PREFIX}-ar
  export NACLRANLIB=${NACL_BIN_PATH}/${NACL_CROSS_PREFIX}-ranlib
  export NACLLD=${NACL_BIN_PATH}/${NACL_CROSS_PREFIX}-ld
  export NACLSTRINGS=${NACL_BIN_PATH}/${NACL_CROSS_PREFIX}-strings
  export NACLSTRIP=${NACL_BIN_PATH}/${NACL_CROSS_PREFIX}-strip
  # pnacl's translator
  export TRANSLATOR=${NACL_BIN_PATH}/${NACL_CROSS_PREFIX}-translate
  # pnacl's pexe optimizer
  export OPT=${NACL_BIN_PATH}/${NACL_CROSS_PREFIX}-opt
  # TODO(robertm): figure our why we do not have a pnacl-string
  #export NACLSTRINGS=${NACL_BIN_PATH}/pnacl-strings
  # until then use the host's strings tool
  # (used only by the cairo package)
  export NACLSTRINGS="$(which strings)"

  # NACL_SDK_USR is where the headers, libraries, etc. will be installed
  # FIXME:
  readonly NACL_SDK_USR=${NACL_SDK_BASE}/usr
  readonly NACL_SDK_USR_INCLUDE=${NACL_SDK_USR}/include
  readonly NACL_SDK_USR_LIB=${NACL_SDK_USR}/lib
  readonly NACL_SDK_USR_BIN=${NACL_SDK_USR}/bin
}

if [ ${NACL_PACKAGES_BITSIZE} = "pnacl" ] ; then
  InitializePNaClToolchain
else
  InitializeNaClGccToolchain
fi

######################################################################
# Always run
######################################################################

CheckPatchVersion() {
  # refuse patch 2.6
  if [ "`patch --version | sed q`" = "patch 2.6" ]; then
    echo "patch 2.6 is incompatible with these scripts."
    echo "Please install either version 2.5.9 (or earlier)"
    echo "or version 2.6.1 (or later.)"
    exit -1
  fi
}

CheckPatchVersion


######################################################################
# Helper functions
######################################################################

Banner() {
  echo "######################################################################"
  echo $*
  echo "######################################################################"
}


Usage() {
  egrep "^#@" $0 | cut --bytes=3-
}


ReadKey() {
  read
}


Fetch() {
  Banner "Fetching ${PACKAGE_NAME}"
  if which wget ; then
    wget $1 -O $2
  elif which curl ; then
    curl --location --url $1 -o $2
  else
     Banner "Problem encountered"
     echo "Please install curl or wget and rerun this script"
     echo "or manually download $1 to $2"
     echo
     echo "press any key when done"
     ReadKey
  fi

  if [ ! -s $2 ] ; then
    echo "ERROR: could not find $2"
    exit -1
  fi
}


Check() {
  # verify sha1 checksum for tarball
  local IN_FILE=${START_DIR}/${PACKAGE_NAME}.sha1
  if ${SHA1CHECK} <${IN_FILE} ; then
    return 0
  else
    return 1
  fi
}


DefaultDownloadStep() {
  cd ${NACL_PACKAGES_TARBALLS}
  # if matching tarball already exists, don't download again
  if ! Check ; then
    Fetch ${URL} ${PACKAGE_NAME}.tgz
    if ! Check ; then
       Banner "${PACKAGE_NAME} failed checksum!"
       exit -1
    fi
  fi
}


DefaultDownloadBzipStep() {
  cd ${NACL_PACKAGES_TARBALLS}
  # if matching tarball already exists, don't download again
  if ! Check ; then
    Fetch ${URL} ${PACKAGE_NAME}.tbz2
    if ! Check ; then
       Banner "${PACKAGE_NAME} failed checksum!"
       exit -1
    fi
  fi
}

DefaultDownloadZipStep() {
  cd ${NACL_PACKAGES_TARBALLS}
  # if matching zip already exists, don't download again
  if ! Check ; then
    Fetch ${URL} ${PACKAGE_NAME}.zip
    if ! Check ; then
       Banner "${PACKAGE_NAME} failed checksum!"
       exit -1
    fi
  fi
}

Patch() {
  local LOCAL_PACKAGE_NAME=$1
  local LOCAL_PATCH_FILE=$2
  if [ ${#LOCAL_PATCH_FILE} -ne 0 ]; then
    Banner "Patching ${LOCAL_PACKAGE_NAME}"
    cd ${NACL_PACKAGES_REPOSITORY}
    patch -p0 -g0 < ${START_DIR}/${LOCAL_PATCH_FILE}
  fi
}


VerifyPath() {
  # make sure path isn't all slashes (possibly from an unset variable)
  local PATH=$1
  local TRIM=${PATH##/}
  if [ ${#TRIM} -ne 0 ]; then
    return 0
  else
    return 1
  fi
}


ChangeDir() {
  local NAME=$1
  if VerifyPath ${NAME}; then
    cd ${NAME}
  else
    echo "ChangeDir called with bad path."
    exit -1
  fi
}


Remove() {
  local NAME=$1
  if VerifyPath ${NAME}; then
    rm -rf ${NAME}
  else
    echo "Remove called with bad path."
    exit -1
  fi
}


MakeDir() {
  local NAME=$1
  if VerifyPath ${NAME}; then
    mkdir -p ${NAME}
  else
    echo "MakeDir called with bad path."
    exit -1
  fi
}


IsInstalled() {
  local LOCAL_PACKAGE_NAME=$1
  if [ ${#LOCAL_PACKAGE_NAME} -ne 0 ]; then
    if grep -qx ${LOCAL_PACKAGE_NAME} ${NACL_PACKAGES_OUT}/installed.txt \
       &>/dev/null; then
      return 0
    else
      return 1
    fi
  else
    echo "IsInstalled called with possibly unset variable!"
  fi
}


AddToInstallFile() {
  local LOCAL_PACKAGE_NAME=$1
  if ! IsInstalled ${LOCAL_PACKAGE_NAME}; then
    echo ${LOCAL_PACKAGE_NAME} >> ${NACL_PACKAGES_OUT}/installed.txt
  fi
}


PatchSpecFile() {
  # fix up spaces so gcc sees entire path
  local SED_SAFE_SPACES_USR_INCLUDE=${NACL_SDK_MULTIARCH_USR_INCLUDE/ /\ /}
  local SED_SAFE_SPACES_USR_LIB=${NACL_SDK_MULTIARCH_USR_LIB/ /\ /}
  # have nacl-gcc dump specs file & add include & lib search paths
  ${NACLCC} -dumpspecs |\
    awk '/\*cpp:/ {\
      printf("*nacl_arch:\n%%{m64:x86_64-nacl; m32:i686-nacl; :x86_64-nacl}\n\n", $1); } \
      { print $0; }' |\
    sed "/*cpp:/{
      N
      s|$| -I${SED_SAFE_SPACES_USR_INCLUDE}|
    }" |\
    sed "/*link_libgcc:/{
      N
      s|$| -L${SED_SAFE_SPACES_USR_LIB}|
    }" >${NACL_SDK_GCC_SPECS_PATH}/specs
}


DefaultPreInstallStep() {
  cd ${NACL_NATIVE_CLIENT_SDK}/..
  MakeDir ${NACL_SDK_USR}
  MakeDir ${NACL_SDK_USR_INCLUDE}
  MakeDir ${NACL_SDK_USR_LIB}
  MakeDir ${NACL_PACKAGES_REPOSITORY}
  MakeDir ${NACL_PACKAGES_TARBALLS}
  MakeDir ${NACL_PACKAGES_PUBLISH}
  Remove ${NACL_PACKAGES_REPOSITORY}/${PACKAGE_NAME}
}


DefaultExtractStep() {
  Banner "Untaring ${PACKAGE_NAME}.tgz"
  ChangeDir ${NACL_PACKAGES_REPOSITORY}
  Remove ${PACKAGE_NAME}
  if [ $OS_SUBDIR = "windows" ]; then
    tar --no-same-owner -zxf ${NACL_PACKAGES_TARBALLS}/${PACKAGE_NAME}.tgz
  else
    tar zxf ${NACL_PACKAGES_TARBALLS}/${PACKAGE_NAME}.tgz
  fi
}


DefaultExtractBzipStep() {
  Banner "Untaring ${PACKAGE_NAME}.tbz2"
  ChangeDir ${NACL_PACKAGES_REPOSITORY}
  Remove ${PACKAGE_NAME}
  if [ $OS_SUBDIR = "windows" ]; then
    tar --no-same-owner -jxf ${NACL_PACKAGES_TARBALLS}/${PACKAGE_NAME}.tbz2
  else
    tar jxf ${NACL_PACKAGES_TARBALLS}/${PACKAGE_NAME}.tbz2
  fi
}

DefaultExtractZipStep() {
  Banner "Unzipping ${PACKAGE_NAME}.zip"
  ChangeDir ${NACL_PACKAGES_REPOSITORY}
  Remove ${PACKAGE_NAME}
  unzip -d ${PACKAGE_NAME} ${NACL_PACKAGES_TARBALLS}/${PACKAGE_NAME}.zip
}

# TODO(khim): remove this when nacl-gcc -V doesn't lockup.
# See: http://code.google.com/p/nativeclient/issues/detail?id=2074
TemporaryVersionWorkaround() {
  if [ $OS_SUBDIR = "windows" ]; then
    Banner "TEMPORARY: Replacing -V with --version for ${PACKAGE_NAME}"
    cd ${NACL_PACKAGES_REPOSITORY}
    # Generic.
    sed -i 's/-V/--version/g' ${PACKAGE_NAME}/configure || true
    # For freetype.
    sed -i 's/-V/--version/g' ${PACKAGE_NAME}/builds/unix/configure || true
    sed -i 's/-V/--version/g' ${PACKAGE_NAME}/builds/unix/aclocal.m4 || true
  fi
}


DefaultPatchStep() {
  Patch ${PACKAGE_NAME} ${PATCH_FILE}
  TemporaryVersionWorkaround
}


DefaultConfigureStep() {
  local EXTRA_CONFIGURE_OPTS=("${@:-}")
  Banner "Configuring ${PACKAGE_NAME}"
  # export the nacl tools
  export CC=${NACLCC}
  export CXX=${NACLCXX}
  export AR=${NACLAR}
  export RANLIB=${NACLRANLIB}
  export PKG_CONFIG_PATH=${NACL_SDK_USR_LIB}/pkgconfig
  export PKG_CONFIG_LIBDIR=${NACL_SDK_USR_LIB}
  export FREETYPE_CONFIG=${NACL_SDK_USR_BIN}/freetype-config
  export PATH=${NACL_BIN_PATH}:${PATH};
  ChangeDir ${NACL_PACKAGES_REPOSITORY}/${PACKAGE_NAME}
  Remove ${PACKAGE_NAME}-build
  MakeDir ${PACKAGE_NAME}-build
  cd ${PACKAGE_NAME}-build
  echo "Directory: $(pwd)"
  ../configure \
    --host=nacl \
    --disable-shared \
    --prefix=${NACL_SDK_USR} \
    --exec-prefix=${NACL_SDK_USR} \
    --libdir=${NACL_SDK_USR_LIB} \
    --oldincludedir=${NACL_SDK_USR_INCLUDE} \
    --with-http=off \
    --with-html=off \
    --with-ftp=off \
    --${NACL_OPTION}-mmx \
    --${NACL_OPTION}-sse \
    --${NACL_OPTION}-sse2 \
    --${NACL_OPTION}-asm \
    --with-x=no  \
    "${EXTRA_CONFIGURE_OPTS[@]}"
}


DefaultBuildStep() {
  Banner "Build ${PACKAGE_NAME}"
  echo "Directory: $(pwd)"
  # assumes pwd has makefile
  make clean
  make -j${OS_JOBS}
}


DefaultTouchStep() {
  FULL_PACKAGE="${START_DIR/#${NACL_PACKAGES}\//}"
  SENTFILE="${NACL_PACKAGES_OUT}/sentinels/bits${NACL_PACKAGES_BITSIZE}/${FULL_PACKAGE}"
  SENTDIR=$(dirname "${SENTFILE}")
  mkdir -p "${SENTDIR}" && touch "${SENTFILE}"
}


DefaultInstallStep() {
  # assumes pwd has makefile
  make install
  DefaultTouchStep
}


DefaultCleanUpStep() {
  if [ ${NACL_PACKAGES_BITSIZE} != "pnacl" ] ; then
    PatchSpecFile
  fi
  AddToInstallFile ${PACKAGE_NAME}
  ChangeDir ${SAVE_PWD}
}


RunCommand() {
  echo "$@"
  time "$@"
}

RunSelLdrCommand() {
  echo "[sel_ldr] $@"
  if [ $NACL_GLIBC = "1" ] ; then
    time "${NACL_SEL_LDR}" -a -B "${NACL_IRT}" -- \
        "${NACL_SDK_LIB}/runnable-ld.so" --library-path "${NACL_SDK_LIB}" "$@"
  else
    time "${NACL_SEL_LDR}" -a -B "${NACL_IRT}" -- "$@"
  fi
}

DefaultTranslateStep() {
  local package=$1
  local build_dir="${NACL_PACKAGES_REPOSITORY}/${package}"
  local pexe=${build_dir}/$2
  local arches="arm x86-32 x86-64"

  Banner "Translating ${pexe}"
  ls -l ${pexe}

  echo "stripping pexe"
  RunCommand ${NACLSTRIP} ${pexe} -o ${pexe}.stripped
  ls -l ${pexe}.stripped

  for a in ${arches} ; do
    echo
    echo "translating pexe [$a]"
    nexe=${pexe}.$a.nexe
    RunCommand ${TRANSLATOR} -arch $a ${pexe}.stripped -o ${nexe}
    ls -l ${nexe}
  done

  # PIC branch
  for a in ${arches} ; do
    echo
    echo "translating pexe [$a,pic]"
    nexe=${pexe}.$a.pic.nexe
    RunCommand ${TRANSLATOR} -arch $a -fPIC ${pexe}.stripped -o ${nexe}
    ls -l ${nexe}
  done

  # Now the same spiel with an optimized pexe
  opt_args="-O3 -inline-threshold=100"
  echo
  echo "optimizing pexe [${opt_args}]"
  RunCommand ${OPT} ${opt_args} ${pexe}.stripped -o ${pexe}.stripped.opt
  ls -l ${pexe}.stripped.opt

  for a in ${arches} ; do
    echo
    echo "translating pexe [$a]"
    nexe=${pexe}.opt.$a.nexe
    RunCommand ${TRANSLATOR} -arch $a ${pexe}.stripped.opt -o ${nexe}
    ls -l ${nexe}
  done

  # PIC branch
  for a in ${arches}; do
    echo
    echo "translating pexe [$a,pic]"
    nexe=${pexe}.$a.pic.opt.nexe
    RunCommand ${TRANSLATOR} -arch $a -fPIC ${pexe}.stripped.opt -o ${nexe}
    ls -l ${nexe}
  done

  ls -l $(dirname ${pexe})
}


DefaultPackageInstall() {
  DefaultPreInstallStep
  DefaultDownloadStep
  DefaultExtractStep
  DefaultPatchStep
  DefaultConfigureStep
  DefaultBuildStep
  DefaultInstallStep
  DefaultCleanUpStep
}
