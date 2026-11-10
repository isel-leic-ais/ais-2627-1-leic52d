#!/bin/bash

ECHOD=server
ECHOD_DIRECTORY=/opt/echod

kill -SIGTERM `cat $ECHOD_DIRECTORY/echod.pid`

rm -rf $ECHOD_DIRECTORY