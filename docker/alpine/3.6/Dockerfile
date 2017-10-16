FROM alpine:3.6

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
                ttf-freefont && \
    #install ms fonts
    apk --update add msttcorefonts-installer && update-ms-fonts --quiet && apk del msttcorefonts-installer && \
    #install libspiro
    cd / && wget https://github.com/fontforge/libspiro/releases/download/0.5.20150702/libspiro-0.5.20150702.tar.gz && \
    tar -xvf libspiro-0.5.20150702.tar.gz && cd libspiro-0.5.20150702 && \
    autoreconf -i && automake --foreign -Wall && ./configure && make && make install && \
    # install poppler
    cd / && git clone --depth 1 -o origin -b master https://github.com/4xxi/poppler.git && \
    cd /poppler && ./configure --prefix=/usr --enable-xpdf-headers && make && make install && \
    # install fontforge
    cd / && git clone --depth 1 -o origin -b pdf2htmlEX https://github.com/4xxi/fontforge.git && cd /fontforge && \
    ./autogen.sh && ./configure --without-libzmq --without-x --without-iconv --disable-python-scripting --disable-python-extension && \
    make && make install && \
    # install pdf2htmlEX
    cd / && git clone --depth 1 -o origin -b master https://github.com/4xxi/pdf2htmlEX.git && \
    cd /pdf2htmlEX && cmake . && make && make install && \
    apk del alpine-sdk xz autoconf automake coreutils cmake python-dev pango-dev freetype-dev glib-dev libtool libxml2-dev tiff-dev zlib-dev libjpeg-turbo-dev && \
    apk add python pango libintl libltdl freetype libpng poppler tiff libxml2 libjpeg-turbo openjpeg && \
    cd / && rm -rf /pdf2htmlEX && \
    rm -rf /libspiro-0.5.20150702.tar.gz && rm -rf libspiro-0.5.20150702 && \
    rm -rf /poppler && \
    rm -rf /fontforge && \
    rm -rf /var/lib/apt/lists/* && \
    rm /var/cache/apk/*
