#!/bin/sh

sysctl -w net.ipv4.icmp_echo_ignore_all=1
exec "$@"
