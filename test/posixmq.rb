assert("PMQ.new") do
  pmq = PMQ.create "/hello"
  assert_true pmq.is_a?(PMQ)
  assert_equal "/hello", pmq.name
  pmq.unlink rescue nil
end

assert("PMQ send and receive") do
  r, w = PMQ.channel2 "/hello_2"
  p = Process.fork do
    sleep 0.5
    w.send "foobar"
  end

  res = r.receive
  Process.waitpid p
  assert_equal res, "foobar"

  r.unlink rescue nil
end

assert("PMQ send and receive #2") do
  r, w = PMQ.channel2 "/hello_3", size: 10
  p = Process.fork do
    sleep 0.5
    w.send "foo"
    w.send "bar"
    w.send "buz"
  end

  res1 = r.receive
  res2 = r.receive
  res3 = r.receive
  Process.waitpid p
  assert_equal "foo", res1
  assert_equal "bar", res2
  assert_equal "buz", res3

  r.unlink rescue nil
end
