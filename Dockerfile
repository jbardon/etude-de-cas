FROM ubuntu:latest

# Update Base System
RUN apt-get update 

RUN apt-get install -y \
    git \
    make \
    cmake \
    gcc \
    chipmunk-dev \
    libsdl1.2-dev \
    libsdl-gfx1.2-dev \
    libsdl-image1.2-dev \
    libsdl-ttf2.0-dev \
    libglib2.0-0

# Install Fake X11 display to connect with VNC
RUN apt-get install -y \
    x11vnc \
    xvfb 

RUN mkdir ~/.vnc
RUN x11vnc -storepasswd 1234 ~/.vnc/passwd

# Install project
WORKDIR /root
# RUN git clone https://jeremy-lr@bitbucket.org/jeremy-lr/etude-de-cas.git
# RUN cmake etude-de-cas/src
# RUN make

# docker build  -t project/etudecas ./  
# docker run --name dev -it --rm -v /Volumes/Data/Projets/docker-try/etude-de-cas:/root project/etudecas
# docker run --name dev -it --rm -v /home/jbardon/Documents/etude-de-cas:/root -p 5900 project/etudecas x11vnc -forever -usepw -create
# docker inspect dev | grep IPAddress

