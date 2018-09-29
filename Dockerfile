FROM ubuntu:xenial

RUN apt-get -y update

RUN apt-get -y install make
RUN apt-get -y install gcc-4.8
RUN apt-get -y install libmysqlclient-dev
RUN apt-get -y install csh
RUN apt-get -y install git-all
RUN apt-get install libc6-dbg
RUN apt-get install gdb

RUN echo "mysql-server mysql-server/root_password password root" | debconf-set-selections
RUN echo "mysql-server mysql-server/root_password_again password root" | debconf-set-selections
RUN apt-get -y install mysql-server

RUN ln -s /usr/bin/gcc-4.8 /usr/bin/gcc

ADD . /opt/kbk

RUN cd /opt/kbk/src && make -k -j8
RUN mkdir -p /opt/kbk/log
RUN mkdir -p /opt/kbk/player

WORKDIR /opt/kbk/area
VOLUME [ "/opt/kbk" ]

EXPOSE 8989

ENTRYPOINT  ["bash"]
