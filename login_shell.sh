#!/bin/bash

./check || exit

socat - /tmp/$USER/containersh.sock
