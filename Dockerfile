FROM ubuntu:latest

RUN apt update
RUN apt install -y socat

ENTRYPOINT "socat" "unix-listen:/tmp/test/test.sock,fork,perm-early=0777" "EXEC:/bin/bash,pty,stderr,setsid,sigint,sane"
