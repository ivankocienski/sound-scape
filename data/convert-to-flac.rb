#!/usr/bin/ruby

Dir.glob('*.wav').each do |filename|
  name = filename.split('.').first

  command = "ffmpeg -i #{filename} -af aformat=s16:44100 #{name}.flac"

  puts command
  system command
end

