#!/usr/bin/env bash

BACKUPER="/home/akindyakov/source/git.proto/backup/backuper.py"
LOGDIR="/home/akindyakov/temp/logs"

function git_pull {
    cd $1
    git pull github master
}

git_pull "/home/akindyakov/source/git.allu_zef"
git_pull "/home/akindyakov/source/git.config"
git_pull "/home/akindyakov/source/git.doc"
git_pull "/home/akindyakov/source/git.simple_algo"

$BACKUPER \
    --local \
    --tail-size 14 \
    --backup-file-fmt '%Y.%m.%d' \
    --what /home/akindyakov/source \
    --backup-path /home/akindyakov/backup/backups/akindyakov_source \
    --log $LOGDIR"/source_bp.log"

$BACKUPER \
    --local \
    --tail-size 14 \
    --backup-file-fmt '%Y.%m.%d' \
    --what /home/akindyakov/crt \
    --backup-path /home/akindyakov/backup/backups/akindyakov_crt \
    --log $LOGDIR"/crt_bp.log"
