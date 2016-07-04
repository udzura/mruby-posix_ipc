# mruby-posixmq   [![Build Status](https://travis-ci.org/udzura/mruby-posixmq.svg?branch=master)](https://travis-ci.org/udzura/mruby-posixmq)
PMQ class
## install by mrbgems
- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :github => 'udzura/mruby-posixmq'
end
```
## example
```ruby
p PMQ.hi
#=> "hi!!"
t = PMQ.new "hello"
p t.hello
#=> "hello"
p t.bye
#=> "hello bye"
```

## License
under the MIT License:
- see LICENSE file
