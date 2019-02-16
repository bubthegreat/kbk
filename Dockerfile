FROM ubuntu:trusty

RUN apt-get -y update

RUN apt-get -y install make
RUN apt-get -y install gcc
RUN apt-get -y install libmysqlclient-dev
RUN apt-get -y install csh
RUN apt-get -y install git-all
RUN apt-get -y install libc6-dbg
RUN apt-get -y install gdb
RUN apt-get -y install vim
RUN apt-get -y install mysql-client

RUN DEBIAN_FRONTEND=noninteractive apt-get install -y mysql-server

EXPOSE 3306

COPY . /kbk
RUN cd /kbk/src && make -j8