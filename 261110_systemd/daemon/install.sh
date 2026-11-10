#!/bin/bash

ECHOD=server
ECHOD_DIRECTORY=/opt/echod

mkdir -p $ECHOD_DIRECTORY

cp $ECHOD $ECHOD_DIRECTORY

$ECHOD_DIRECTORY/$ECHOD
sleep 3
chown ezequiel $ECHOD_DIRECTORY
chgrp ezequiel $ECHOD_DIRECTORY
chown ezequiel $ECHOD_DIRECTORY/*
chgrp ezequiel $ECHOD_DIRECTORY/*
