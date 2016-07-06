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

## examples

### POSIX MQ

```ruby
## One process

mq = PMQ.create('/sample1', mode: "w")
mq.send("The test message!!1")
# => "The test message!!1"

## Another process

mq = PMQ.create('/tmp/sample1', mode: "r")
mq.receive
# => "The test message!!1"
# will block if no message

## Use channel

r, w = PMQ.channel2 "/hello_2"
Process.fork do
  sleep 0.5
  w.send "foobar"
end

res = r.receive
#=> "foobar"
```

#### options for `.create` and `.channel`

* `:mode` specify MQ descriptor read/write mode by letters
* `:queue_size` specify MQ's max size
* `:message_size` specify MQ's max message size, default to `/proc/sys/fs/mqueue/msgsize_max`
* NOTE: `.new` is a raw and low-level API.

### POSIX Semaphore

```ruby
## Running jobs by 3

begin
  sem = PSem.open('/job_by_3', value: 3)
 
  pids = (0..10).map do |i|
    Process.fork do
      puts "Waiting job: #{i}"
      sem.wait
      puts "Enter job: #{i}"
      sleep rand(10)
      puts "(Now sem.value = #{sem.value})"
      sem.post
    end
  end
 
  pids.each do |pid|
    Process.waitpid pid
  end
 
  puts "Exit all"
ensure
  sem.unlink
end

# log:

    Waiting job: 4
    Waiting job: 0
    Enter job: 0
    Waiting job: 10
    Enter job: 4
    Waiting job: 2
    Waiting job: 3
    Enter job: 3
    Waiting job: 5
    Waiting job: 7
    Waiting job: 9
    Waiting job: 1
    Waiting job: 6
    Waiting job: 8
    (Now sem.value = 0)
    Enter job: 10
    (Now sem.value = 0)
    Enter job: 5
    (Now sem.value = 0)
    Enter job: 7
    (Now sem.value = 0)
    (Now sem.value = 1)
    (Now sem.value = 2)
    Enter job: 1
    Enter job: 2
    Enter job: 6
    (Now sem.value = 0)
    (Now sem.value = 1)
    (Now sem.value = 2)
    Enter job: 9
    Enter job: 8
    (Now sem.value = 1)
    (Now sem.value = 2)
    Exit all


## trywait

sem = PSem.open "/sample4, value: 1
sem.trywait #=> 0
sem.trywait #=> nonblock, but -1

sem.post
sem.trywait #=> 0
```

* Please examine the `test` directory for example.

## License

under the MIT License:

- see LICENSE file

## See also

- [posix-mqueue MRI gem](https://github.com/Sirupsen/posix-mqueue)
