#!/bin/sh

socat unix-listen:/tmp/containersh/containersh.sock,fork,perm-early=0700,user=$1 EXEC:/bin/bash,pty,stderr,setsid,sigint,sane
