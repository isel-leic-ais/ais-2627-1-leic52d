#!/bin/bash

ECHOD=server
ECHOD_DIRECTORY=/opt/echod/

mkdir -p $ECHOD_DIRECTORY

cp $ECHOD $ECHOD_DIRECTORY

chown isel $ECHOD_DIRECTORY
chgrp isel $ECHOD_DIRECTORY
chown isel $ECHOD_DIRECTORY/*
chgrp isel $ECHOD_DIRECTORY/*

cp echod.service /etc/systemd/system
cp echod.socket /etc/systemd/system

systemctl daemon-reload

#systemctl start echod.socket
