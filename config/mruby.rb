MRuby::Build.new do |conf|
  # load specific toolchain settings
  toolchain :gcc

  enable_debug

  conf.bins = ["mrbc", "mirb"]

  conf.gem :core => "mruby-bin-mirb"

  conf.cc do |cc|
    cc.flags = [ENV['CFLAGS'], "-lm"].join(" ")
  end
end
