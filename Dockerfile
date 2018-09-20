FROM ubuntu:xenial

RUN apt-get -y update

RUN apt-get -y install make
RUN apt-get -y install gcc-4.8
RUN apt-get -y install libmysqlclient-dev
RUN apt-get -y install csh
RUN apt-get -y install git-all

RUN echo "mysql-server mysql-server/root_password password root" | debconf-set-selections
RUN echo "mysql-server mysql-server/root_password_again password root" | debconf-set-selections
RUN apt-get -y install mysql-server

RUN ln -s /usr/bin/gcc-4.8 /usr/bin/gcc

RUN git clone https://github.com/bubthegreat/kbk.git
RUN cd kbk/src && make --silent

EXPOSE 4000

ENTRYPOINT  ["bash"]
