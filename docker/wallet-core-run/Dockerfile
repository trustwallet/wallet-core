# Docker image with a built, runnable version of wallet-core
FROM trustwallet/wallet-core

ENV CC=/usr/bin/clang
ENV CXX=/usr/bin/clang++
# Prepare fresh git repo
RUN cd /wallet-core && git checkout master && git pull

# Build: generate, cmake, and make
# rbenv trick is needed as Ruby is installed to non-standard location
RUN cd /wallet-core \
    && eval "$(/root/.rbenv/bin/rbenv init -)" \
    && echo PATH $PATH \
    && ./tools/generate-files \
    && cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug \
    && make -Cbuild

CMD ["/bin/bash"]
