FROM m1mbert/cxlb-gnuradio-3.7:1.0

RUN ${APT} update && ${APT} dist-upgrade
# RUN ${APT} install unzip
# RUN ${APT} install wget
# RUN ${APT} install libncurses5
# RUN ${APT} install libxmu-dev

WORKDIR /root/cxlb_toolchain_build


# The following command guarantees that git cloning is updated for each new commit
# https://github.com/ooubejja/cache
ADD https://api.github.com/repos/ooubejja/cache/git/refs/heads/master version.json
RUN git clone git://github.com/ooubejja/cache.git
WORKDIR cache

RUN cp include/libTurboFSK_v4.h /usr/include/
RUN cp lib/libTurboFSK_v4.so /usr/lib/
RUN mkdir build
WORKDIR build
RUN cmake -DCMAKE_INSTALL_PREFIX=${INSTALL_PATH} ..
RUN make
RUN make install
RUN cp ../examples/hier_* /root/.grc_gnuradio/
# RUN grcc /root/cxlb_toolchain_build/gr-ephyl/examples/hier_sensor.grc
# RUN grcc /root/cxlb_toolchain_build/gr-ephyl/examples/bs.grc
# RUN grcc /root/cxlb_toolchain_build/gr-ephyl/examples/hier_sensor_turbofsk.grc
# RUN grcc /root/cxlb_toolchain_build/gr-ephyl/examples/hier_bs_turbofsk.grc
WORKDIR /root
RUN /bin/bash -c "source cxlb_toolchain_build/gr-ephyl/ephyl_docker/env_conf "
# CMD [ "source", "cxlb_toolchain_build/gr-ephyl/ephyl_docker/env_conf" ]

## THIS DOESNT WORK (??)
# cp include/libTurboFSK_v4.h /cortexlab/toolchains/current/include/
#cp lib/libTurboFSK_v4.so /cortexlab/toolchains/current/lib/
