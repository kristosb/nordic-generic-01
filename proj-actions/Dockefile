FROM ubuntu:latest


RUN apt-get update
RUN apt-get -qq -y install curl cmake jq clang cppcheck

ADD runchecks.sh /entrypoint.sh
COPY . .
RUN chmod +x /entrypoint.sh
ENTRYPOINT /entrypoint.sh
