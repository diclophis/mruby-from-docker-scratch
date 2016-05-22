FROM scratch
ADD build/mirbrc-build/mirbrc /mirbrc
ADD .mirbrc /.mirbrc
CMD ["/mirbrc"]
