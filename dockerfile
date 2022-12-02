FROM docker.io/ubuntu:22.04

# install C++ build dependencies 
RUN apt-get update && apt-get install -y g++ make 

WORKDIR /caff

COPY ./caff-parser ./parser

RUN cd parser && make all

RUN mkdir data

# install python dependencies
RUN apt-get install -y python3 python3-pip 

COPY ./backend ./backend

CMD ["/bin/bash"]