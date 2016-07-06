# mruby-posix_ipc   [![Build Status](https://travis-ci.org/udzura/mruby-posix_ipc.svg?branch=master)](https://travis-ci.org/udzura/mruby-posix_ipc)

The POSIX message queue wrapper


## install by mrbgems

- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

  # ... (snip) ...

  conf.gem :github => 'udzura/mruby-posix_ipc'
end
```

## example

```ruby
## One process

mq = PMQ.new('/sample1', PMQ::O_CREAT | PMQ::O_WRONLY)
mq.send("The test message!!1")
# => "The test message!!1"

## Another process

mq = PMQ.new('/tmp/sample1', PMQ::O_CREAT | PMQ::O_RDONLY)
mq.receive
# => "The test message!!1"
# will block if no message
```

## License

under the MIT License:

- see LICENSE file

## See also

- [posix-mqueue MRI gem](https://github.com/Sirupsen/posix-mqueue)
