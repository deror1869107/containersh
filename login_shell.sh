#!/bin/bash

./check || exit

socat - /tmp/$USER/test.sock
