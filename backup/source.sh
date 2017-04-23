#!/usr/bin/env bash

set -e

BACKUPER="/home/akindyakov/source/git.proto/backup/backuper.py"
LOGDIR="/home/akindyakov/temp/logs"

mkdir --parents "${LOGDIR}"

function git_pull {
    cd $1
    git pull github master
    cd -
}

home_source() {
    local BACKUP_PATH="/home/akindyakov/backup/backups/akindyakov_source"
    mkdir --parents "${BACKUP_PATH}"
    "${BACKUPER}" \
        --local \
        --tail-size 14 \
        --backup-file-fmt '%Y.%m.%d' \
        --what "/home/akindyakov/source" \
        --backup-path "${BACKUP_PATH}" \
        --log "${LOGDIR}/source_bp.log"
}

home_crt() {
    local BACKUP_PATH="/home/akindyakov/backup/backups/akindyakov_crt"
    mkdir --parents "${BACKUP_PATH}"
    "${BACKUPER}" \
        --local \
        --tail-size 14 \
        --backup-file-fmt '%Y.%m.%d' \
        --what "/home/akindyakov/crt" \
        --backup-path "${BACKUP_PATH}" \
        --log "${LOGDIR}/crt_bp.log"
}

git_pull "/home/akindyakov/source/git.allu_zef"
git_pull "/home/akindyakov/source/git.config"
git_pull "/home/akindyakov/source/git.doc"
git_pull "/home/akindyakov/source/git.simple_algo"

home_crt
home_source
