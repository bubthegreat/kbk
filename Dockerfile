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
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y mysql-server

# Required files
COPY . /kbk

# Build steps
RUN cd /kbk/src && make -j8
RUN chmod 777 /kbk/data/startup.sh

EXPOSE 3306
EXPOSE 8989

CMD ["/kbk/data/startup.sh"]
