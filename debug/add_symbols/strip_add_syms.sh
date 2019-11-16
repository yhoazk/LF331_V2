#!/usr/bin/bash -ex

script_dir=$(dirname ${0})
script_dir=$(cd ${script_dir}; pwd)
script_name=$(basename ${0})

function usage(){
    echo "${script_name} <file>"
}

function error_exit()
{
    errorcode=${1}
    shift
    echo $@
    exit ${errorcode}
}

tostripdir=$(dirname "${1}")
tostripfile=$(basename "${1}")

if [ -z ${tostripfile} ] ; then
    usage
    error_exit 0 "file to strip not specified"
fi

cd "${tostripdir}"

echo "stripp ${tostripfile} and placing dbg symbols in ${tostripdir}/${tostripfile}.syms"
objcopy --only-keep-debug "${tostripfile}" "${tostripdir}/${tostripfile}.syms"
strip --strip-debug --strip-unneeded "${tostripfile}"
objcopy --add-gnu-debuglink="${tostripdir}/${tostripfile}.syms" "${tostripfile}"
chmod -x "${tostripdir}/${tostripfile}.syms"
