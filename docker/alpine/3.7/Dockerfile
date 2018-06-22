FROM alpine:3.7

RUN apk --update add \
                alpine-sdk \
                xz \
                pango-dev \
                libtool \
                autoconf \
                automake \
                coreutils \
                python-dev \
                zlib-dev \
                freetype-dev \
                openjpeg-dev \
                glib-dev \
                libxml2-dev \
                tiff-dev \
                cmake \
                libjpeg-turbo-dev \
                font-adobe-100dpi \
                ttf-freefont

#install ms fonts
RUN apk --update add msttcorefonts-installer && update-ms-fonts --quiet && apk del msttcorefonts-installer

#install libspiro
RUN cd / && wget https://github.com/fontforge/libspiro/releases/download/0.5.20150702/libspiro-0.5.20150702.tar.gz && \
    tar -xvf libspiro-0.5.20150702.tar.gz && cd libspiro-0.5.20150702 && \
    autoreconf -i && automake --foreign -Wall && ./configure && make && make install
    
# install poppler
RUN cd / && curl -O https://poppler.freedesktop.org/poppler-0.66.0.tar.xz && tar xf poppler-0.66.0.tar.xz && \
    cd /poppler-0.66.0 && mkdir build && cd build && cmake -DENABLE_XPDF_HEADERS=ON .. && make && make install

RUN cd / && curl -L -O https://github.com/fontforge/fontforge/archive/20170731.tar.gz &&  tar -xvf 20170731.tar.gz && \
    cd fontforge-20170731 && ./bootstrap && ./configure --prefix=/usr --without-libzmq --without-x --without-iconv && make && make install

RUN apk --update add gdb

RUN curl -L -O https://poppler.freedesktop.org/poppler-data-0.4.9.tar.gz && tar -xvf poppler-data-0.4.9.tar.gz && make && make install
