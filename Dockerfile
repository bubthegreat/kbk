FROM ubuntu:24.04

# Install required packages in a single layer to reduce image size
RUN apt-get -y update && \
    apt-get -y install --no-install-recommends \
        make \
        gcc \
        libc6-dev \
        libmysqlclient-dev \
        default-mysql-client \
        gdb \
        libc6-dbg \
        telnet \
        curl \
        ca-certificates && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

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
