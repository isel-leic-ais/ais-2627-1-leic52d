#!/bin/bash

ECHOD=server
ECHOD_DIRECTORY=/opt/echod/

mkdir -p $ECHOD_DIRECTORY

cp $ECHOD $ECHOD_DIRECTORY

chown ezequiel $ECHOD_DIRECTORY
chgrp ezequiel $ECHOD_DIRECTORY
chown ezequiel $ECHOD_DIRECTORY/*
chgrp ezequiel $ECHOD_DIRECTORY/*

cp echod.service /etc/systemd/system

systemctl daemon-reload
