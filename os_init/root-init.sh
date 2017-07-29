#!/usr/bin/env bash

set -x -e

adduser akindyakov

apt update
apt upgrade
apt install sudo

usermod --append --groups sudo akindyakov

apt install \
    cron \
    cronutils \
    git \
    tmux \
    vim \

