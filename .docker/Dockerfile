
FROM colllijo/cpp-devcontainer:latest AS BUILDENV

WORKDIR /usr/src
COPY ../sources ./sources
COPY ../CMakeLists.txt .

RUN mkdir -p /usr/src/out/release
RUN /usr/bin/cmake -DCMAKE_BUILD_TYPE=Release -G "CodeBlocks - Unix Makefiles" -S /usr/src -B /usr/src/out/release
RUN /usr/bin/cmake --build /usr/src/out/release --target CC-Server -- -j 12

FROM ubuntu:22.04 AS RUNENV

RUN apt-get update && apt-get -y upgrade
RUN apt-get -y install uuid-dev libpqxx-dev

WORKDIR /app
COPY --from=BUILDENV /usr/src/out/release/CC-Server ./
RUN chmod 770 CC-Server

EXPOSE 42024

CMD ["/app/CC-Server"]