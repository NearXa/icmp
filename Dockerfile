FROM gcc:13.2.0

ENTRYPOINT ["./docker-entrypoint.sh"]

ARG ARGS="-s"

ENV ARG=${ARGS}

ADD . /usr/icmp

WORKDIR /usr/icmp

RUN make

CMD ./src/icmp.o ${ARG}
