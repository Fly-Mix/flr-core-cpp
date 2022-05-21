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

def register_default_message_callback
    puts ""
    puts "register default log_message_callback and ui_message_callback now ..."

    puts "register default log_message_callback ..."
    Flr::register_log_message_callback(Flr::DEFAULT_LOG_MESSAGE_PRINTER)

    puts "register default ui_message_callback ..."
    Flr::register_ui_message_callback(Flr::DEFAULT_UI_MESSAGE_PRINTER)

    puts "register default log_message_callback and ui_message_callback done !!!"
    puts ""
end

def unregister_default_message_callback
    puts ""
    puts "unregister default log_message_callback and ui_message_callback now ..."

    puts "register default log_message_callback ..."
    Flr::unregister_log_message_callback()

    puts "register default ui_message_callback ..."
    Flr::unregister_ui_message_callback()

    puts "register default log_message_callback and ui_message_callback done !!!"
    puts ""
end

def register_message_printer
    puts ""
    puts "register log_message_printer and ui_message_printer now ..."

    puts "register log_message_printer ..."
    log_message_printer = LogMessagePrinter.new
    Flr::register_log_message_printer(log_message_printer)

    puts "register ui_message_printer ..."
    ui_message_printer = UiMessagePrinter.new
    Flr::register_ui_message_printer(ui_message_printer)

    puts "register log_message_printer and ui_message_printer done !!!"
    puts ""
end

def unregister_message_printer
    puts ""
    puts "unregister log_message_printer and ui_message_printer now ..."

    puts "unregister log_message_printer ..."
    Flr::unregister_log_message_printer()

    puts "unregister ui_message_printer ..."
    Flr::unregister_ui_message_printer()

    puts "unregister log_message_printer and ui_message_printer done !!!"
    puts ""
end

########### Test Now ###########
puts "Hello, let's test Flr now ..."

# puts "Flr-Module-Methods: #{Flr::methods}"
# puts ""

register_message_printer

puts "invoke Flr::say_hi ..."
Flr::say_hi
puts ""

# puts "invoke Flr::init_one ..."
# Flr::init_one("/Users/York/Workspace/Flutterspace/flutter_hello_app", "2.0.3", "2.12.2")
# puts ""

unregister_message_printer
puts "Flr is tested done, goodbye~"
