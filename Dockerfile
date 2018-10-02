FROM ubuntu:14.04

RUN apt-get -y update

# Install the basics
RUN apt-get -y install make
RUN apt-get -y install gcc-4.8
RUN apt-get -y install libmysqlclient-dev
RUN apt-get -y install csh
RUN apt-get -y install git-all
RUN apt-get -y install libc6-dbg
RUN apt-get -y install gdb
RUN apt-get -y install vim

# Make gcc-4.8 our default gcc
RUN ln -s /usr/bin/gcc-4.8 /usr/bin/gcc

RUN cd /opt/kbk/src && make -k -j8

WORKDIR /opt/kbk/area

EXPOSE 8989

ENTRYPOINT  ["bash"]
