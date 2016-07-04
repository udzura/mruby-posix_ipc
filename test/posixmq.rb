##
## PMQ Test
##

assert("PMQ.new") do
  pmq = PMQ.new "hello"
  assert_true pmq.is_a?(PMQ)
end
