# This file is loaded when VM is done bootstraping

# TODO make those path absolute
load "lib/primitives.rb"
load "lib/object.rb"
load "lib/class.rb"
load "lib/enumerable.rb"
load "lib/array.rb"
load "lib/fixnum.rb"
load "lib/string.rb"
load "lib/range.rb"

$stdin = IO.new(0)
$stdout = IO.new(1)
$stderr = IO.new(2)
