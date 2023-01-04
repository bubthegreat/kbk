FROM ubuntu:trusty

RUN apt-get -y update

# Installation steps
RUN apt-get -y install make
RUN apt-get -y install gcc
RUN apt-get -y install libmysqlclient-dev
RUN apt-get -y install csh
RUN apt-get -y install git-all
RUN apt-get -y install libc6-dbg
RUN apt-get -y install gdb
RUN apt-get -y install vim
RUN apt-get -y install mysql-client
RUN apt-get -y install telnet
RUN apt-get -y install bash
RUN apt-get -y install curl

COPY ./area /kbk/area
COPY ./data /kbk/data
COPY ./src /kbk/src
COPY ./sys /kbk/sys
COPY ./player /kbk/player
COPY ./log /kbk/log

# Build steps
RUN cd /kbk/src && make -j8

RUN chmod 777 /kbk/area/start-kbk.sh

EXPOSE 8989

HEALTHCHECK CMD curl -m 1 localhost:8989 | grep "Revived from the dead by Bub." || exit 1

WORKDIR /kbk/area/
CMD ./start-kbk.sh
