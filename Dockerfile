# Create base image with dependencies
# needed by both builder and final
FROM ubuntu:16.04 as base-image

ENV CCACHE_DIR=/ccache

RUN apt-get update && apt-get install -y build-essential \
    libmysqlclient-dev \
    liblua5.3-dev \
    libdb5.3-dev \
    libssl-dev \
    libjemalloc-dev

# use ccache (make it appear in path earlier then /usr/bin/gcc etc)
RUN for p in gcc g++ cc c++; do ln -vs /usr/bin/ccache /usr/local/bin/$p;  done

COPY server/scripts /app/scripts
RUN ln -s /app/scripts/swgemu.sh /usr/bin/swgemu

# Create builder image from base and add
# needed items for building the project
FROM base-image as build-image
ENV CCACHE_DIR=/ccache

RUN --mount=type=cache,target=/ccache/ apt-get install -y cmake \
    ninja-build \
    git \
    default-jre \
    curl \
    python-qt4 \
    ccache

# builder image to build Core3
# this is separate to facilicate using
# the prior layer for local development
FROM build-image as builder
ENV CCACHE_DIR=/ccache

RUN curl -L https://github.com/krallin/tini/releases/download/v0.18.0/tini -o /usr/bin/tini

WORKDIR /app
COPY .git/ ./.git/
COPY ./MMOCoreORB MMOCoreORB
COPY ./server/custom_scripts server/custom_scripts/
COPY ./server/scripts server/scripts/
COPY ./server/conf server/conf/


# This is a hack to make the /app folder the root of it's own
# git repo. Without this section git will treat is as a submodule
# of swgemu-docker but will be missing the .git folder and fail all git commands
RUN ls && sed -i 's/..\/..\/MMOCoreORB\///' .git/modules/MMOCoreORB/utils/engine3/config && \
    sed -i 's/worktree.*//' .git/config && \
    sed -i 's/..\/.git\/modules\/MMOCoreORB\//.git\//' MMOCoreORB/utils/engine3/.git

WORKDIR /app/MMOCoreORB
RUN --mount=type=cache,target=/ccache/ make build-ninja-debug
# Create final image that could be used as a 
# lighter-weight production image
FROM base-image as final

COPY --from=builder /usr/bin/tini /usr/bin/tini
RUN chmod a+x /usr/bin/tini

WORKDIR /app/MMOCoreORB/bin
COPY --from=builder /app/MMOCoreORB/bin .

# tini is needed as core3 does not explicitly handle SIGTERM signals
ENTRYPOINT ["tini", "--"]
CMD ["swgemu", "start"]
