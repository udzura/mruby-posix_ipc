##
## PMQ Test
##

assert("PMQ.new") do
  pmq = PMQ.new "/hello", PMQ::O_CREAT|PMQ::O_RDONLY
  assert_true pmq.is_a?(PMQ)
end
