FROM ubuntu:14.04

RUN apt-get -y update

# Install the basics
RUN apt-get -yqq install make
RUN apt-get -yqq install gcc-4.8
RUN apt-get -yqq install libmysqlclient-dev
RUN apt-get -yqq install csh
RUN apt-get -yqq install git-all
RUN apt-get -yqq install libc6-dbg
RUN apt-get -yqq install gdb
RUN apt-get -yqq install vim
RUN apt-get -yqq install mysql-client

# Make gcc-4.8 our default gcc
RUN ln -s /usr/bin/gcc-4.9 /usr/bin/gcc
