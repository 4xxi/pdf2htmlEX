FROM alpine:3.4

ARG POPPLER_VERSION=poppler-0.54.0

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
                glib-dev \
                libxml2-dev \
                tiff-dev \
                cmake \
                libjpeg-turbo-dev \
                font-adobe-100dpi \
                ttf-freefont && \
    export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig && \
    # install msfont
    apk add --update --no-cache --virtual .ms-fonts msttcorefonts-installer --repository http://dl-3.alpinelinux.org/alpine/v3.6/community && \
    update-ms-fonts --quiet && \
    apk del .ms-fonts && \
    #install libspiro
    cd / && wget https://github.com/fontforge/libspiro/releases/download/0.5.20150702/libspiro-0.5.20150702.tar.gz && \
    tar -xvf libspiro-0.5.20150702.tar.gz && cd libspiro-0.5.20150702 && \
    autoreconf -i && automake --foreign -Wall && ./configure --prefix=/usr && make && make install && \
    # install poppler
    cd / && wget https://poppler.freedesktop.org/${POPPLER_VERSION}.tar.xz && tar -xvf ${POPPLER_VERSION}.tar.xz && \
    cd /${POPPLER_VERSION} && ./configure --prefix=/usr --enable-xpdf-headers && make && make install && \
    # install fontforge
    cd / && git clone --depth 1 -o origin -b pdf2htmlEX https://github.com/4xxi/fontforge.git && cd /fontforge && \
    ./autogen.sh && ./configure --prefix=/usr --without-libzmq --without-x --without-iconv --disable-python-scripting --disable-python-extension && \
    make && make install && \
    cd / && \
    rm -rf /libspiro-0.5.20150702.tar.gz && rm -rf libspiro-0.5.20150702 && \
    rm -rf /${POPPLER_VERSION}.tar.xz && rm -rf /${POPPLER_VERSION} && \
    rm -rf /fontforge && \
    rm -rf /var/lib/apt/lists/* && \
    rm /var/cache/apk/*

VOLUME ["/pdf2htmlEX"]
