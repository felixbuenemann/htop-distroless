FROM alpine:edge
COPY entrypoint.c /tmp/entrypoint.c
RUN set -x \
 && apk add --no-cache htop gcc musl-dev \
 && mkdir -p /tmp/target \
 && gcc -o /usr/local/bin/htop /tmp/entrypoint.c -s \
 && apk info -qL ncurses-terminfo-base | xargs tar c | tar xC /tmp/target \
 && tar c usr/bin/htop usr/local/bin/htop | tar xC /tmp/target \
 && ldd /usr/bin/htop | awk '/ => /{print $3}' | xargs tar ch | tar xC /tmp/target \
 && find /tmp/target -print
FROM scratch
COPY --from=0 /tmp/target /
ENTRYPOINT ["/usr/local/bin/htop"]
