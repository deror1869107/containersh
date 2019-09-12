FROM ubuntu:latest

RUN apt update
RUN apt install -y socat

COPY ./docker-entrypoint.sh /

ENTRYPOINT [ "/docker-entrypoint.sh" ]
CMD [ "0" ]
