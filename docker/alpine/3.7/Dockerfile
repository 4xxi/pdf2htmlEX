FROM alpine:3.7

# Install 
RUN apk --no-cache --update add \
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
                libjpeg-turbo \
                libjpeg-turbo-dev \
                font-adobe-100dpi \
                ttf-freefont \
                lcms2-dev \
                curl-dev \
                nss-dev && \
    apk --update add msttcorefonts-installer && update-ms-fonts --quiet && apk del msttcorefonts-installer && \
    cd / && wget https://github.com/fontforge/libspiro/releases/download/0.5.20150702/libspiro-0.5.20150702.tar.gz && \
    tar -xvf libspiro-0.5.20150702.tar.gz && cd libspiro-0.5.20150702 && \
    autoreconf -i && automake --foreign -Wall && ./configure && make && make install && rm -rf /libspiro-0.5.20150702 /libspiro-0.5.20150702.tar.gz && \
    cd / && curl -L -O https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.15.tar.gz && tar -xvf libiconv-1.15.tar.gz && cd libiconv-1.15 && ./configure --prefix=/usr/local && make && make install && \
    rm -rf /libiconv-1.15.tar.gz && rm -rf /libiconv-1.15 && \
    cd / && curl -L -O https://poppler.freedesktop.org/poppler-data-0.4.9.tar.gz && tar -xvf poppler-data-0.4.9.tar.gz && cd poppler-data-0.4.9 &&  make && make install && \
    rm -rf /poppler-data-0.4.9.tar.gz /poppler-data-0.4.9 && \        
    cd / && curl -O https://poppler.freedesktop.org/poppler-0.66.0.tar.xz && tar xf poppler-0.66.0.tar.xz && \
    cd / && git clone --depth 1 -o origin -b poppler-0.66 https://github.com/4xxi/poppler.git && \
    cd /poppler && mkdir build && cd build && cmake -DENABLE_XPDF_HEADERS=ON .. && \
    make && make install && rm -rf /poppler \
    cd / && curl -L -O https://github.com/fontforge/fontforge/archive/20170731.tar.gz &&  tar -xvf 20170731.tar.gz && \
    cd fontforge-20170731 && ./bootstrap && ./configure --prefix=/usr --without-libzmq --without-x && make && make install && \
    rm -rf /fontforge-20170731 /20170731.tar.gz && \
    export PKG_CONFIG_PATH=${PKG_CONFIG_PATH}:/usr/local/lib64/pkgconfig && \
    cd / && git clone --depth 1 -o origin -b poppler-update https://github.com/4xxi/pdf2htmlEX.git && \
    cd /pdf2htmlEX && cmake . && make && make install && rm -rf /pdf2htmlEX && \
    ln -s /usr/local/lib64/*poppler* /usr/local/lib && \
    apk del curl-dev nss-dev lcms2-dev libjpeg-turbo-dev tiff-dev libxml2-dev glib-dev openjpeg-dev freetype-dev zlib-dev cmake autoconf automake coreutils python-dev pango-dev cairo-dev && \
    apk --no-cache --update add curl nss lcms2 libjpeg-turbo tiff libxml2 glib openjpeg freetype zlib cairo pango python && \
    rm -rf /var/lib/apt/lists/* && \
    rm /var/cache/apk/*


