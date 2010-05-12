export RAILS_ENV='development'
export PATH=${PATH}:'/Users/bcherry/.gem/ruby/1.8/bin'
export EDITOR=vim

source ~/.git-completion.sh
export PS1='\[\e[1;37m\]@\t\[\e[m\] \[\e[1;36m\]\w\[\e[m\]\[\e[1;31m\]$(__git_ps1 " [%s]")\[\e[m\]: '

#export CLICOLOR=1
#export LSCOLORS=ExFxCxDxBxegedabagacad

#export PS1="\u \[\033[1;33m\]\w\[\033[0m\] \[\033[1;32m\][\$(git branch 2> /dev/null | fgrep '*' | sed 's/* //')]\[\033[0m\] $ "

alias dev='cd /Users/bcherry/dev'
alias twitter='cd /Users/bcherry/dev/twitter'
alias anywhere='cd /Users/bcherry/dev/anywhere'
alias ag='cd /Users/bcherry/dev/adequatelygood'
alias bc='cd /Users/bcherry/dev/bcherry'
alias web='cd /Users/bcherry/dev/bcherry/bcherry-web'
alias t='cd /Users/bcherry/dev/twitter'
alias a='cd /Users/bcherry/dev/anywhere'

alias t1="cat ~/clobber.sql | mysql -uroot"
alias t2="RAILS_ENV=development rake db:migrate; RAILS_ENV=test rake db:migrate; RAILS_ENV=selenium rake db:migrate; RAILS_ENV=development rake db:fixtures:load; RAILS_ENV=test rake db:fixtures:load; RAILS_ENV=selenium rake db:fixtures:load"
alias t3="rake kestrel:stop; rake memcached:stop; rake memcached:start; rake kestrel:start; rake"


if [[ -s /Users/bcherry/.rvm/scripts/rvm ]] ; then source /Users/bcherry/.rvm/scripts/rvm ; fi

export RUBY_HEAP_MIN_SLOTS=250000
export RUBY_HEAP_SLOTS_INCREMENT=250000
export RUBY_HEAP_SLOTS_GROWTH_FACTOR=1
export RUBY_GC_MALLOC_LIMIT=50000000

UTILS_PATH="/Users/bcherry/dev/twitter-utilities"  # set the path to your utils checkout here
# if $UTILS_PATH/twitter.sh is a file, source it
if [ -f $UTILS_PATH/twitter.sh ]; then
   . $UTILS_PATH/twitter.sh
fi

export JAVA_HOME=/System/Library/Frameworks/JavaVM.framework/Versions/1.6/Home
export PATH=$JAVA_HOME/bin:$PATH
export PATH=/usr/local/mysql/bin:/opt/local/bin:/opt/local/sbin:/usr/local/bin:$PATH

##
# Your previous /Users/bcherry/.bash_profile file was backed up as /Users/bcherry/.bash_profile.macports-saved_2010-04-19_at_15:02:17
##

# MacPorts Installer addition on 2010-04-19_at_15:02:17: adding an appropriate PATH variable for use with MacPorts.
export PATH=/opt/local/bin:/opt/local/sbin:$PATH
# Finished adapting your PATH environment variable for use with MacPorts.

