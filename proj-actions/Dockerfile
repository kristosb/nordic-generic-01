FROM ubuntu:latest


RUN apt-get update
RUN apt-get -qq -y install curl cmake jq clang cppcheck

ADD runchecks.sh /runchecks.sh
COPY . .
RUN chmod +x /runchecks.sh
ENTRYPOINT /runchecks.sh
