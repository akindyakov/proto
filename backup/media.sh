#!/usr/bin/env bash

BACKUPER="/home/akindyakov/source/git.proto/backup/backuper.py"
HOST="192.168.1.215"
PORT="6013"
LOGDIR="/home/akindyakov/logs"

$BACKUPER \
    --backup-host $HOST \
    --backup-port $PORT \
    --what /home/akindyakov/data/photo \
    --backup-path /home/akindyakov/backup/backups/akindyakov_photo \
    --log $LOGDIR"/photo_bp.log"


$BACKUPER \
    --backup-host $HOST \
    --backup-port $PORT \
    --what /home/akindyakov/data/video \
    --backup-path /home/akindyakov/backup/backups/akindyakov_video \
    --log $LOGDIR"/video_bp.log"


$BACKUPER \
    --backup-host $HOST \
    --backup-port $PORT \
    --what /home/akindyakov/data/yandex-disk/doc \
    --backup-path /home/akindyakov/backup/backups/akindyakov_doc \
    --log $LOGDIR"/doc_bp.log"

