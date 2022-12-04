FROM docker.io/ubuntu:22.04

# install C++ build dependencies 
RUN apt-get update && apt-get install -y g++ make 

WORKDIR /caff

COPY ./caff-parser ./parser

RUN cd parser && make all

RUN mkdir data && mkdir data/out && mkdir data/preview

# install python dependencies
RUN apt-get install -y python3 python3-pip default-libmysqlclient-dev

COPY ./backend ./backend

RUN cd backend && pip install -r requirements.txt

WORKDIR /caff/backend

EXPOSE 80

ENV SERVICE_NAME="caffservice"

RUN addgroup --gid 1001 --system $SERVICE_NAME
RUN adduser --system --ingroup $SERVICE_NAME --shell /bin/false --disabled-password --no-create-home --uid 1001 $SERVICE_NAME
RUN chown 1001:1001 -R /caff/data
RUN chmod 744 -R /caff/data 

USER $SERVICE_NAME
CMD ["uvicorn", "main:app", "--host", "0.0.0.0", "--port", "80"]