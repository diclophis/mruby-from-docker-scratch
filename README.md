# run example .mirbrc in blank docker image

Develop a .mirbrc

        $ cat .mirbrc 
        i = 0
        42.times do
          i += 1
        end
        raise "running mruby in docker #{i}"

then build the example image

        $ make

then run it

        $ docker images | grep f95example4778
        <none>              <none>              f95example4778        11 seconds ago       1.658 MB

        $ docker run f95example4778
        Exception in .mirbrc
        trace:
            [0] .mirbrc:7
        .mirbrc:7: running mruby in docker 42 (RuntimeError)
