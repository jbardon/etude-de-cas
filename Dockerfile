FROM ubuntu:latest

# Update Base System
RUN apt-get update 

RUN apt-get install -y \
    make \
    cmake \
    gcc \
    chipmunk-dev \
    libsdl1.2-dev \
    libsdl-gfx1.2-dev \
    libsdl-image1.2-dev \
    libsdl-ttf2.0-dev \
    libglib2.0-0

WORKDIR /root

# RUN cmake etude-de-cas/src

# docker build  -t project/etudecas ./  
# docker run --name dev -it --rm -v /Volumes/Data/Projets/docker-try/etude-de-cas:/root project/etudecas