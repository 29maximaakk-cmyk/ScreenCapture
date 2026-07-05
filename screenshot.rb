#!/usr/bin/env ruby
# screenshot.rb
# encoding: UTF-8

require 'rmagick'
include Magick
require 'optparse'
require 'time'

options = { output: nil, delay: 0, region: nil }

OptionParser.new do |opts|
  opts.banner = "Usage: screenshot.rb [options]"
  opts.on("-o", "--output FILE", "Выходной файл") { |v| options[:output] = v }
  opts.on("-d", "--delay SEC", Integer, "Задержка в секундах") { |v| options[:delay] = v }
  opts.on("-r", "--region X,Y,W,H", String, "Область захвата") { |v| options[:region] = v }
  opts.on("-h", "--help", "Справка") { puts opts; exit }
end.parse!

if options[:delay] > 0
  puts "Ожидание #{options[:delay]} секунд..."
  sleep options[:delay]
end

output = options[:output] || "screenshot_#{Time.now.strftime('%Y%m%d_%H%M%S')}.png"

begin
  if options[:region]
    x, y, w, h = options[:region].split(',').map(&:to_i)
    # RMagick не имеет прямого захвата экрана, используем системную команду? 
    # Вместо этого используем системную утилиту (как в C++).
    # Для простоты вызовем внешнюю утилиту.
    # На Linux используем import, на macOS screencapture, на Windows PowerShell.
    if RUBY_PLATFORM =~ /linux/
      system("import -window root -crop #{w}x#{h}+#{x}+#{y} #{output}")
    elsif RUBY_PLATFORM =~ /darwin/
      system("screencapture -R #{x},#{y},#{w},#{h} #{output}")
    elsif RUBY_PLATFORM =~ /mswin|mingw|cygwin/
      # PowerShell
      system("powershell -command \"Add-Type -AssemblyName System.Drawing; $b=New-Object System.Drawing.Bitmap(#{w},#{h}); $g=[System.Drawing.Graphics]::FromImage($b); $g.CopyFromScreen(#{x},#{y},0,0,New-Object System.Drawing.Size(#{w},#{h})); $b.Save('#{output}','png')\"")
    end
  else
    # Захват всего экрана через системные утилиты
    if RUBY_PLATFORM =~ /linux/
      system("import -window root #{output}")
    elsif RUBY_PLATFORM =~ /darwin/
      system("screencapture -x #{output}")
    elsif RUBY_PLATFORM =~ /mswin|mingw|cygwin/
      system("powershell -command \"Add-Type -AssemblyName System.Drawing; $b=New-Object System.Drawing.Bitmap([System.Windows.Forms.Screen]::PrimaryScreen.Bounds.Width, [System.Windows.Forms.Screen]::PrimaryScreen.Bounds.Height); $g=[System.Drawing.Graphics]::FromImage($b); $g.CopyFromScreen(0,0,0,0,$b.Size); $b.Save('#{output}','png')\"")
    end
  end
  puts "Скриншот сохранён в #{output}"
rescue => e
  puts "Ошибка: #{e.message}"
  exit 1
end
