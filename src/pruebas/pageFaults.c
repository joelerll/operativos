ps -eo min_flt,maj_flt,cmd
http://blog.scoutapp.com/articles/2015/04/10/understanding-page-faults-and-memory-swap-in-outs-when-should-you-worry

ps axo %cpu,%mem,command,pid | sort -nr | head
