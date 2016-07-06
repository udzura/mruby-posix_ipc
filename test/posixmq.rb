##
## PMQ Test
##

assert("PMQ.new") do
  pmq = PMQ.create "/hello"
  assert_true pmq.is_a?(PMQ)
  pmq.unlink rescue nil
end

assert("PMQ send and receive") do
  r, w = PMQ.channel2 "/hello_2"
  Process.fork do
    sleep 0.5
    w.send "foobar"
  end

  res = r.receive
  assert_equal res, "foobar"

  pmq.unlink rescue nil
end
