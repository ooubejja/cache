FROM m1mbert/cxlb-gnuradio-3.7:1.0
########
RUN ${APT} update && ${APT} dist-upgrade
RUN ${APT} install unzip
RUN ${APT} install sudo
ENV INSTALL_PATH "/cortexlab/toolchains/current"
ENV TERM "xterm"
########
# The following command guarantees that git cloning is updated for each new commit
ADD https://api.github.com/repos/ooubejja/cache/git/refs/heads/master version.json
RUN git clone git://github.com/ooubejja/cache.git
########
# WORKDIR cache
RUN mkdir /usr/local/lib/pkgconfig
RUN mkdir /root/cache/project_caching/trasmissioni
RUN mkdir /root/cache/project_caching/cache/UserCache
########
WORKDIR "/root/cache/project_caching/Shared Library/src/"
RUN make clean ; make 
RUN sudo make install
########
WORKDIR /root/cache/project_caching/environment/
RUN make clean ; make
########
WORKDIR /root/cache/project_caching/cache/
RUN make clean ; make
########
WORKDIR /root/cache/gr-projectCACHE/
RUN rm -rf build
RUN mkdir build
WORKDIR build
########
# RUN cmake -DCMAKE_INSTALL_PREFIX="/cortexlab/toolchains/current" ..
RUN cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_PATH} ..
RUN make
RUN sudo make install
########
WORKDIR /root
# RUN /bin/bash -c "source cxlb_toolchain_build/gr-ephyl/ephyl_docker/env_conf "

