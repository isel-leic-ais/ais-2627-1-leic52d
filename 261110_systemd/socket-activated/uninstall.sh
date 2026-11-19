#!/bin/bash

ECHOD_DIRECTORY=/opt/echod

rm -rf $ECHOD_DIRECTORY

if systemctl is-active echod.service ; then
	systemctl stop echod.service
fi


systemctl stop echod.socket

rm -f /etc/systemd/system/echod.service
rm -f /etc/systemd/system/echod.socket

