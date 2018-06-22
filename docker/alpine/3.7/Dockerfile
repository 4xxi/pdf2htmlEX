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
RUN    apk --update add msttcorefonts-installer && update-ms-fonts --quiet && apk del msttcorefonts-installer && \
    
    #install libspiro
RUN    cd / && wget https://github.com/fontforge/libspiro/releases/download/0.5.20150702/libspiro-0.5.20150702.tar.gz && \
    tar -xvf libspiro-0.5.20150702.tar.gz && cd libspiro-0.5.20150702 && \
    autoreconf -i && automake --foreign -Wall && ./configure && make && make install
    
    # install poppler
RUN    cd / && git clone --depth 1 -o origin -b master https://github.com/4xxi/poppler.git && \
    cd /poppler && ./configure --prefix=/usr --enable-xpdf-headers && make && make install && \
