# file: runme.rb

# This file illustrates the C++ interface created by SWIG.
# All of our C++ classes get converted into Ruby classes.

require './flr'

puts "Flr-Module-Methods: #{Flr::methods}"
puts ""

puts "register message callback for Flr ..."
Flr::register_log_message_callback(Flr::DEFAULT_LOG_MESSAGE_PRINTER)
Flr::register_ui_message_callback(Flr::DEFAULT_UI_MESSAGE_PRINTER)
puts ""

puts "invoke Flr::say_hi ..."
Flr::say_hi
puts ""

puts "invoke Flr::init_one ..."
Flr::init_one("/Users/York/Workspace/Flutterspace/flutter_hello_app", "2.0.3", "2.12.2")
puts ""

puts "Goodbye"
