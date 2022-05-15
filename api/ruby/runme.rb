# file: runme.rb

# This file illustrates the C++ interface created by SWIG.
# All of our C++ classes get converted into Ruby classes.

require './flr'

class LogMessagePrinter < Flr::LogMessageAbstractPrinter   # Inherit from a C++ class
   def initialize
      puts "Creating Ruby LogMessagePrinter"
      super()
   end

   def print_log_message(file, line, func, severity, content)
      puts "[ruby-example] [flr] [#{file}:#{line}:#{func}] #{content}"
   end
end

class UiMessagePrinter < Flr::UiMessageAbstractPrinter   # Inherit from a C++ class
   def initialize
      puts "Creating Ruby UiMessagePrinter"
      super()
   end

   def print_ui_message(severity, content)
      puts "[ruby-example] [flr] #{content}"
   end
end

puts "Flr-Module-Methods: #{Flr::methods}"
puts ""

# puts "register message callback for Flr ..."
# Flr::register_log_message_callback(Flr::DEFAULT_LOG_MESSAGE_PRINTER)
# Flr::register_ui_message_callback(Flr::DEFAULT_UI_MESSAGE_PRINTER)
# puts ""

puts "register log_message_printer ..."
log_message_printer = LogMessagePrinter.new
Flr::register_log_message_printer(log_message_printer)
puts ""

puts "register ui_message_printer ..."
ui_message_printer = UiMessagePrinter.new
Flr::register_ui_message_printer(ui_message_printer)
puts ""

puts "invoke Flr::say_hi ..."
Flr::say_hi
puts ""
return

puts "invoke Flr::init_one ..."
Flr::init_one("/Users/York/Workspace/Flutterspace/flutter_hello_app", "2.0.3", "2.12.2")
puts ""

puts "Goodbye"
